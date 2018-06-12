%Start insynset outsynset comment indefinition inframelist
%o 4000
%a 2500
%{
/*

  wnlex.l - Lexical analyzer for WordNet lexicographer's files

*/

#include "wngrind.h"
#include "grind-wnparse.h"   /* y.tab.h, renamed in the makefile */

/*
  Need to override lex's default output() function, otherwise parsed
  text sometimes preceeds error messages.
*/

#undef output
#define output(c);

static char *Id = "$Id: wnlex.l,v 1.19 2005/02/01 16:13:45 wn Rel $";

int yyinhead=0;
int commentline=0;
int pcount=0;

static void fixdigit();
static void fixbackslash();
static void i_c_err();
static int outmess();
void yyerror(), yywarn();
char *strclone();

/*
 * Now some weird stuff to change the behavior of Lex so that it
 * remembers which state to start in even after returning a token.
 * This code depends on the particular skeleton code used by
 * your local lex.
 * Substituting "flex" for lex would almost certainly break this code
 * (not that I've looked at flex, but I doubt the internal variables
 * go by the same names.)
 * If there's some really weird behavior in some version of the
 * grinder's tokenizing when porting to machines other than Sun-[34]
 * suspect this.
 *
 */

static int yypreservestate=0;
#define REMEMBER(x) yypreservestate = x , BEGIN x 
#define FORGET yypreservestate=0;

%}

/* FiWN: UTF-8 follow-up byte and a complete multibyte UTF-8 representation */
u8fb	[\x80-\xBF]
utf8	([\xC2-\xDF]{u8fb}|[\xE0-\xEF]{u8fb}{u8fb}|[\xF0-\xF4]{u8fb}{u8fb}{u8fb})
/* FiWN: A special character can be quoted by a backslash */
quoted	\\.
file	[A-Za-z.]+":"
az	[a-z]
/* FiWN: Definitions for XML covering simple cases */
/* FiWN: Tag opening, closing and slash, with or without quoting */
tagopen		(<|\\<)
tagclose	(>|\\>)
slash		([/]|\\[/])
/* FiWN: Anything but a tag opening, closing, closing or slash, and slash */
notagopen	([^\\<]|\\[^<])
notagclose	([^\\>]|\\[^>])
notagclosesl	([^\\>/]|\\[^>/])
noslash		([^\\/]|\\[^/])
/* FiWN: XML element start tag, end tag and empty element tag */
starttag	({tagopen}{notagclosesl}+{tagclose})
endtag		({tagopen}{slash}{notagclose}+{tagclose})
emptytag	({tagopen}{notagclose}+{slash}{tagclose})
/* FiWN: XML elements, one level of nesting allowed */
xml     (({emptytag}|{starttag}({emptytag}|{notagopen}|({starttag}({emptytag}|{notagopen})*{endtag}))*{endtag})+)
/* FiWN: Uppercase word character, any word character */
wbaseu	([A-Z./$_\'-]|{quoted}|{xml}|{utf8})
wbase	({wbaseu}|{az})
/* FiWN: The same as above but including numbers */
wnumu	({wbaseu}|[0-9\"])
wnum	({wnumu}|{az})
/* FiWN: Numbers followed by a double quote */
numsq	([0-9.]+\")
/* FiWN: Any word and an uppercase word (modified to use the definitions) */
word	(({wbase}?{wnum}*{wbase})|({wnum}*{numsq}))
WORD	(({wbaseu}?{wnumu}*{wbaseu})|({wnumu}*{numsq}))
num	[1-9][0-9]*
numzero	[0-9]+
ptr	([!*&~><^\\=\$+]|@i?|#[msp]|%[msp]|;[cur]|-[cur])
allwhite	[\n\f\t\ ]
white	[\t\ ]

%%

 /*
  * Now figure out where to begin.  Use a preserved state if there is one
  * or otherwise default to outsynset.
  */

 BEGIN (yypreservestate ? yypreservestate : outsynset);

<outsynset>"{"	{
		REMEMBER(insynset);
		switch(curfilemode()) {
			case NOUN: return (NOUNSYNSET);
			case VERB: return (VERBSYNSET);
			case ADJ: return (ADJSYNSET);
			case ADV: return (ADVSYNSET);
		}
        }

<outsynset>"["	{ 
		if (curfilemode()!= ADJ) {
			yywarn("Open-cluster symbol in non-adjective file (ignored)\n");
		} else {
			yyinhead=1; 
			return(OPENCLUS);  
		}
	}

<outsynset>"]"	{ 
		if (curfilemode()!= ADJ) {
			yywarn("Close-cluster symbol in non-adjective file (ignored)\n");
		} else {
			return(CLOSECLUS); 
		}
	}

<outsynset>^-+$	{ 
		if (curfilemode()!= ADJ) {
			yywarn("Cluster separator symbol in non-adjective file (ignored)\n");
		} else {
			yyinhead=1;
			return('-');  
		}
	}

<outsynset>{numzero} { 
                     REMEMBER(outsynset);
		     yylval.number = atoi(yytext);
		     return(SYNSETKEY);
		     }

<insynset>"frames:"	{
			if (curfilemode() == VERB) {
				REMEMBER(inframelist); 
				return(VERBFRAMES); 
			} else {
			       yyerror("frame list in non-verb file\n");
			}
		}

<insynset>"}"	{ yyinhead=0; REMEMBER(outsynset); return('}'); }

<comment>[^()]	;
<comment>"("	{ pcount++;  }
<comment>")"	{
		if (!(--pcount)) {
			commentline=0;
			REMEMBER(outsynset);
		} else {
			REMEMBER(comment);
		}
	}

<outsynset>"("	{ commentline=yylineno; pcount++; REMEMBER(comment); }

<insynset>"(m)"	{ /* skip this for now */ REMEMBER(insynset); }

<insynset>{WORD}	{
			/* FiWN: Commented out to retain trailing " */
			/* if (yytext[yyleng-1]=='"')  */
			/* 	yytext[yyleng-1]=0; */
			yylval.tx=strclone(yytext);
			fixdigit(yylval.tx);
			/* FiWN: Remove protecting backslashes */
			fixbackslash(yylval.tx);
			REMEMBER(insynset);

			if (curfilemode() == ADJ)
			    return(HEADWYRD);
			else {
			    return(WYRD);
			}
		}

<insynset>{word}	{
			/* FiWN: Commented out to retain trailing " */
			/* if (yytext[yyleng-1]=='"')  */
			/* 	yytext[yyleng-1]=0; */
			yylval.tx=strclone(yytext);
			fixdigit(yylval.tx);
			/* FiWN: Remove protecting backslashes */
			fixbackslash(yylval.tx);
			REMEMBER(insynset);
                        return(WYRD);
		}

<insynset>{file}	{
			yytext[yyleng-1]=0; /* strip colon */
			yylval.tx=strclone(yytext);
			REMEMBER(insynset);
			return(FILENAME); 
		}

<insynset>","{ptr}	{
			yylval.number=ptrkind(yytext+1);
			REMEMBER(insynset);
			return(PTRSYM); 
		}

<insynset>{num}	{
                yylval.number=atoi(yytext);
		REMEMBER(insynset);
		return(NUM);
        	}

<insynset>"["	{ REMEMBER(insynset); return('['); }

<insynset>"]"	{ REMEMBER(insynset); return(']'); }

<insynset>","	{ REMEMBER(insynset); return(','); }

<insynset>"^"	{ REMEMBER(insynset); return('^'); }

<insynset>"(p)"	{
		if (curfilemode() == ADJ) {
		    REMEMBER(insynset);
		    yylval.number=PADJ;
		    return(ADJCLASS);
		} else {
		    yyerror("adjective class specified in non-adjective file\n");
		}
	}

<insynset>"(a)"	{
		if (curfilemode() == ADJ) {
		    REMEMBER(insynset);
		    yylval.number=NPADJ;
		    return(ADJCLASS);
		} else {
		    yyerror("adjective class specified in non-adjective file\n");
		}
	}

<insynset>"(ip)"	{ 
    			if (curfilemode() == ADJ) {
			    REMEMBER(insynset);
			    yylval.number=IPADJ;
			    return(ADJCLASS);
			} else {
			    yyerror("adjective class specified in non-adjective file\n");
			}
		}

<insynset>"("	{ REMEMBER(indefinition); pcount++; }

<indefinition>"("	{ yymore(); pcount++; }

<indefinition>")"	{ 
			if (--pcount) {
				yymore();  
			} else {
				yytext[yyleng-1]=0;
				yylval.tx=strclone(yytext);
				REMEMBER(insynset);
				return(DEFN);
			}
		}

<indefinition>\n	{ 
			yylineno--;
			yywarn("Missing \")\" in definition-- definition ignored\n");
			yylineno++;
			REMEMBER(outsynset);
			pcount=0;
			return('}');
		}

<indefinition>[^\n)]	{ yymore(); }

<inframelist>{numzero}	{ 
			yylval.number=atoi(yytext);
			REMEMBER(inframelist);
			return(NUM);
		}

<inframelist>","	{ REMEMBER(inframelist); return(',');}
<inframelist>"]"	{ REMEMBER(insynset); return(']');}
<inframelist>"}"	{ REMEMBER(outsynset); return('}');}
<inframelist>"("	{ REMEMBER(indefinition); pcount++; }
<inframelist>\n		{ 
			yyerror("Forgot to close previous synset\n");
			REMEMBER(outsynset);
		}

{white}	;

<insynset>.	{ i_c_err(*yytext); }
<insynset>"{"	{ yyerror("Forgot to close previous synset\n"); }
<insynset>.	{ i_c_err(*yytext); }

<outsynset>{allwhite}*\n	;
<outsynset>.	{ i_c_err(*yytext); }

%%
static void i_c_err(c)
char c;
{
    char message[40];

    if (c) 
	sprintf(message,"Illegal character %c\n",c);
    else 
	sprintf(message,"NULL character in file?\n");
    yyerror(message);
}

static int outmess(s,file,lineno)
char *s;
int file,lineno;
{
    /* 
     * YACC somtimes generates more "syntax error" messages than
     * we care to see-- generally there's one every time I generate
     * a message of my own.  I supress those.
     */
    if (strcmp(s,"syntax error") != 0) {
	fprintf(logfile, "%s, line %d: %s",
		PrintFileName(file),
		lineno, s);
	fflush(logfile);
	return(1);
    }
    return(0);
}

void yyerror(s)
char *s;
{
    if (outmess(s,curfilenum(),yylineno))
	errcount++;
}

void yywarn(s)
char *s; 
{
    char mess[500];

    if (!nowarn) {		/* only print warnings if not suppressed */
	sprintf(mess,"warning: %s",s);
	outmess(mess,curfilenum(),yylineno);
    }
    if (verifyflag)		/* if verifying, count as error */
	errcount++;
}

yywrap()
{
    
    if (pcount) {
	fprintf(logfile, "%s: Comment started on line %d left open\n",
	       filelist[curfilenum()], commentline);
	errcount++;
    }
    return 1 ;
}

/*
 * Remove all quote characters from words
 * FiWN: Remove double quotes that are not preceded by a backslash and not
 * within XML tags (unless NO_XML is defined).
 */

static void fixdigit(s)
register char *s;
{
    register int i, j, l;
    int intag = 0;

    l = strlen(s)+ 1;		/* make sure we get the NULL */
    i = 0;
    while (s[i]) {
#ifndef NO_XML
	if (s[i] == '<')
	    intag = 1;
	else if (s[i] == '>' && intag)
	    intag = 0;
#endif
	if (s[i] != '"' || intag || (i > 0 && s[i - 1] == '\\'))
	    i++;
	else {
	    for (j = i; j < l; j++)
		s[j] = s[j + 1];
	    l--;
	}
    }
}

/*
 * FiWN: Remove quoting backslashes
 */

static void fixbackslash(s)
register char *s;
{
    register int i, j, l;

    l = strlen(s)+ 1;		/* make sure we get the NULL */
    i = 0;
    while (s[i]) {
	/* Remove backslash except before a comma or a parenthesis */
	if (s[i] != '\\' || s[i + 1] == ',' || s[i + 1] == '('
	    || s[i + 1] == ')')
	    i++;
	else {
	    for (j = i; j < l; j++)
		s[j] = s[j + 1];
	    l--;
	}
    }
}

/*
  Revision log:

  $Log: wnlex.l,v $
  Revision 1.19  2005/02/01 16:13:45  wn
  fixed includes

  Revision 1.18  2004/01/16 18:13:19  wn
  added @i and ~i pointers

  Revision 1.17  2003/03/27 15:26:49  wn
  removed suffix from "+" in ptr definition

  Revision 1.16  2001/09/06 17:55:38  wn
  added code for synset keys

  Revision 1.15  2001/08/02 14:53:23  wn
  added ";c", ";u", ";r" for CLASSIFICATION

  Revision 1.14  2000/10/27 16:53:03  wn
  added [a-x] in ptr

  Revision 1.13  2000/07/11 15:20:06  wn
  added nominalization pointers

  Revision 1.12  1996/05/30 20:56:48  wn
  *** empty log message ***

 * Revision 1.11  1994/09/27  19:53:17  wn
 * added < to valid pointer llist
 *
 * Revision 1.10  1993/04/21  12:40:32  wn
 * added new ATTRIBUTE pointer (=)
 *
 * Revision 1.9  92/09/08  16:05:13  wn
 * *** empty log message ***
 * 
 * Revision 1.8  92/08/20  13:52:17  wn
 * changed call to PrintFileName
 * 
 * Revision 1.7  92/08/19  16:01:42  wn
 * added code to return ADVSYNSET and changed MARKER to PTRSYM
 * 
 * Revision 1.6  92/03/26  13:42:24  wn
 * removed unused pointers
 * 
 * Revision 1.5  92/02/18  16:56:09  wn
 * removed generic meronym and holonym pointers (% and #)
 * 
 * Revision 1.4  92/01/03  12:17:47  wn
 * changed yywrap() to output msg to logfile and changed syntax of msg.
 * also increment errcount so grinder terminates with syntactic error
 * 
 * Revision 1.3  91/12/13  15:59:27  wn
 * cleaned up static function declarations
 * 
 * Revision 1.2  91/11/26  15:03:43  wn
 * added '\\' as pertainym pointer
 * 
 * Revision 1.1  91/09/11  14:50:52  wn
 * Initial revision
 * 
*/

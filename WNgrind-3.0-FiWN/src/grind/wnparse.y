%{

#include "wngrind.h"

static char *Id = "$Id: wnparse.y,v 1.48 2005/01/31 20:04:03 wn Rel $";

Pointer ptrlist = NULL;		/* keep track of ptrs in syngroup */
Framelist frlist = NULL;	/* keep track of frames in syngroup */
G_Synset lastss = NULL;		/* last synset in list */
Synonym lastsyn = NULL;		/* last synonym in this synset */
Symbol satellite = NULL;	/* satellite pointed to */
int headptr = FALSE;		/* indicate whether ptr is to head word */
int headword = FALSE;		/* indicate whether word is head word */
int foundhyper = FALSE;		/* indicate whether hypernym found in synset */
short wdcnt = 0;		/* index of current synonym in synset */
int aclass = 0;			/* keep track of adjective clss in syngroup */
int wsense = 0;			/* keep track of word's sense number */
int sense = 0;			/* keep track of sense number for ptrs */
int satellite_sense = 0;	/* sense number for pointer to satellite */
int cluscnt = 1;		/* count clusters (needed to resolve ptrs) */
char *label;
char buf[100];			/* useful buffer for building messages */
char fnbuf[100];		/* save cross-file pointer for warning msg */

/* FiWN: Moved declarations here to make the file compile with Bison */
static void AddToSynList();
static G_Synset MakeSynset(int, Synonym, Pointer, Framelist, char *, int);
static Synonym AddSynonym(Synonym, Synonym);
static Pointer AddPointer(Pointer, Pointer);
static Synonym InsertSynonym(Synonym, Synonym);
static void AddSynset(G_Synset);
static void AddToSynList(G_Synset);
static void MakeSyngroup(Pointer, Framelist);
static Pointer DoPointer(Symbol, int);
static int UniqueSynonym(Synonym, Synonym);
static int UniquePointer(Pointer, Pointer);
static int UniqueSynSense(Synonym, int);
static int UniqueFanSynonym(Synonym, int, int);
static char *UseLabel(char *);
static char *normalize_word(char *);
static char *tolower_non_xml(char *);
%}

%start wordnet

%union {
    char *tx;			/* for definitions, WYRD print names */
    int number;			/* file #, sense # */
    G_Synset ss;			/* for pointers to sense space */
    Pointer p;			/* for pointers */
    Synonym s;			/* for synonyms */
    Symbol sym;			/* for pointer into symbol table */
    Framelist fr;		/* for frame lists */
}

%token <tx> NOUNSYNSET VERBSYNSET ADJSYNSET ADVSYNSET
%token <tx> OPENCLUS CLOSECLUS
%token <tx> DEFN WYRD HEADWYRD VERBFRAMES FILENAME
%token <number> NUM PTRSYM ADJCLASS SYNSET SYNSETKEY
%type <number> adjclass key
%type <fr> framelist
%type <tx> defn
%type <p> ptr ptrs
%type <s> syns headsyn
%type <sym> psyn syn syngroup headwyrd
%type <ss> nss vss rss ahead ass fan fanlist acluster asslist

%%
wordnet: wordnet wordelem 
	|
	;

wordelem: nss	{ if ($1) AddSynset($1); }

	| vss	{ if ($1) AddSynset($1); }

	| rss	{ if ($1) AddSynset($1); }

	| acluster { }


        | fan {
	    if ($1) {
		$1->isfanss = FALSE;
		$1->part = ADJ;
		$1->clusnum = 0;
		AddSynset($1);
	    }
	}


	| ']' {	yyerror("']' without matching '['\n"); }

	;

nss: NOUNSYNSET syns ptrs defn '}' key	{ 
            $$ = MakeSynset(NOUN, $2, $3, NULL, $4, $6);
	}

	| NOUNSYNSET syns defn '}' key	{
            $$ = MakeSynset(NOUN, $2, NULL, NULL, $3, $5);
	}

	;

vss: VERBSYNSET syns ptrs framelist defn '}' key	{
            $$ = MakeSynset(VERB, $2, $3, $4, $5, $7);
	}

	| VERBSYNSET syns framelist defn '}' key	{
	    $$ = MakeSynset(VERB, $2, NULL, $3, $4, $6);
	}

        | VERBSYNSET syns ptrs defn '}' key	{
	    $$ = MakeSynset(VERB, $2, $3, NULL, $4, $6);
	}

        | VERBSYNSET syns defn '}' key	{ 
	    $$ = MakeSynset(VERB, $2, NULL, NULL, $3, $5);
	}

        ;

rss: ADVSYNSET syns ptrs defn '}' key	{ 
            $$ = MakeSynset(ADV, $2, $3, NULL, $4, $6);
	}

	| ADVSYNSET syns defn '}' key	{
            $$ = MakeSynset(ADV, $2, NULL, NULL, $3, $5);
	}

	;

syns:	{ $$=NULL; }

        | syns syngroup	{
	    wdcnt++;
	    $$ = AddSynonym($1, CreateSynonym($2,
					      (unsigned char)wsense,
					      (headword ? -wdcnt : wdcnt),
					      (unsigned char)aclass,
					      label));
	    headword = FALSE;
	    aclass = 0;
	}

        | syns syn adjclass	{
	    wdcnt++;
	    $$ = AddSynonym($1, CreateSynonym($2,
					      (unsigned char)wsense,
					      (headword ? -wdcnt : wdcnt),
					      (unsigned char)$3,
					      label));
	    headword = FALSE;
	    aclass = 0;
	}

        | syns error	{ 
	    yyerror("Error in synonym list, "); 
	    if ($1) {
		fprintf(logfile, "last good synonym: \"%s\"\n",
			PrintSynonym(lastsyn));
	    } else if (!wdcnt) {
		fprintf(logfile, "invalid string found\n");
	    } else {
		fprintf(logfile,"first synonym group is bad\n");
	    }
	    $$ = $1;
	}

        ;

syngroup: '[' syn adjclass ptrs ']'	{
            aclass = $3;
	    MakeSyngroup($4, NULL);
	    $$ = $2;
	}

        | '[' syn adjclass ptrs framelist ']'	{
            aclass = $3;
	    MakeSyngroup($4, $5);
	    $$ = $2;
	}

        | '[' syn adjclass framelist ']'	{
            aclass = $3;
	    MakeSyngroup(NULL, $4);
	    $$ = $2;
	}

        ;

syn: WYRD	{
	    /* FiWN: Use normalize_word instead of strlower */
            $$ = CreateSymbol(normalize_word($1));
	    label = UseLabel($1); /* decide whether to save original string */
	    headword = FALSE;
	    wsense = 0;
	}

        | WYRD NUM	{ 
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($1));
	    label = UseLabel($1); /* decide whether to save original string */
	    headword = FALSE;
	    wsense = $2;
	    if (wsense > MAXID) {
		/* FiWN: Use MAXID */
		sprintf(buf, "ID must be less than %d: %s%d\n",
			MAXID + 1, $$->label, wsense);
		yyerror(buf);
	    }
	}

        | HEADWYRD	{
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($1));

/* If file is adj.all, then label must be NULL.  Otherwise, allow upper
   case in synset since there's no head. */

	    if ( ! strcmp(filelist[curfilenum()].fname, "adj.all" ) ) {
#ifndef NO_XML
		/* FiWN: Convert head adjectives to lowercase for the database
		   if XML is to be ignored when indexing */
		if (ignore_xmlwords)
		    label = UseLabel(tolower_non_xml($1));
		else
		    label = NULL;
#else
		label = NULL;
#endif
		headword = TRUE;
	    } else {
		label = UseLabel($1);
		headword = FALSE;
	    }
	    wsense = 0;
	}

        | HEADWYRD NUM	{ 
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($1));

/* If file is adj.all, then label must be NULL.  Otherwise, allow upper
   case in synset since there's no head. */

	    if ( ! strcmp(filelist[curfilenum()].fname, "adj.all" ) ) {
#ifndef NO_XML
		/* FiWN: Convert head adjectives to lowercase for the database
		   if XML is to be ignored when indexing */
		if (ignore_xmlwords)
		    label = UseLabel(tolower_non_xml($1));
		else
		    label = NULL;
#else
		label = NULL;
#endif
		headword = TRUE;
	    } else {
		label = UseLabel($1);
		headword = FALSE;
	    }
	    wsense = $2;
	    if (wsense > MAXID) {
		/* FiWN: Use MAXID */
		sprintf(buf, "ID must be less than %d: %s%d\n",
			MAXID + 1, $$->label, wsense);
		yyerror(buf);
	    }
	}
        ;

psyn: WYRD	{
	    /* FiWN: Use normalize_word instead of strlower */
            $$ = CreateSymbol(normalize_word($1));
	    sense = 0;
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = FALSE;
	    fnbuf[0] = '\0';
	}

        | WYRD NUM	{ 
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($1));
	    sense = $2;
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = FALSE;
	    fnbuf[0] = '\0';
	}

        | HEADWYRD {
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($1));
	    sense = 0;
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = TRUE;
	    fnbuf[0] = '\0';
	}

        | HEADWYRD NUM	{ 
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($1));
	    sense = $2;
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = TRUE;
	    fnbuf[0] = '\0';
	}

        | FILENAME WYRD	{
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($2));
	    sense = 0;
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = FALSE;
	    strcpy(fnbuf, $1);	/* save filename for warning msg */
	    free($1);		/* free space malloc'd for string */
	}

        | FILENAME WYRD NUM	{ 
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($2));
	    sense = $3;
	    satellite = NULL;
	    satellite_sense = 0;
	    headptr = FALSE;
	    strcpy(fnbuf, $1);	/* save filename for warning msg */
	    free($1);		/* free space malloc'd for string */
	}


        | FILENAME WYRD '^' WYRD	{/* new ones start here */
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($2));
	    sense = 0;
	    /* FiWN: Use normalize_word instead of strlower */
	    satellite = CreateSymbol(normalize_word($4));
	    satellite_sense = 0;
	    headptr = FALSE;
	    strcpy(fnbuf, $1);	/* save filename for warning msg */
	    free($1);		/* free space malloc'd for string */
	}

        | FILENAME WYRD NUM '^' WYRD	{
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($2));
	    sense = $3;
	    /* FiWN: Use normalize_word instead of strlower */
	    satellite = CreateSymbol(normalize_word($5));
	    satellite_sense = 0;
	    headptr = FALSE;
	    strcpy(fnbuf, $1);	/* save filename for warning msg */
	    free($1);		/* free space malloc'd for string */
	}

        | FILENAME WYRD NUM '^' WYRD NUM	{
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($2));
	    sense = $3;
	    /* FiWN: Use normalize_word instead of strlower */
	    satellite = CreateSymbol(normalize_word($5));
	    satellite_sense = $6;
	    headptr = FALSE;
	    strcpy(fnbuf, $1);	/* save filename for warning msg */
	    free($1);		/* free space malloc'd for string */
	}

        | FILENAME WYRD '^' WYRD NUM	{
	    /* FiWN: Use normalize_word instead of strlower */
	    $$ = CreateSymbol(normalize_word($2));
	    sense = 0;
	    /* FiWN: Use normalize_word instead of strlower */
	    satellite = CreateSymbol(normalize_word($4));
	    satellite_sense = $5;
	    headptr = FALSE;
	    strcpy(fnbuf, $1);	/* save filename for warning msg */
	    free($1);		/* free space malloc'd for string */
	}

        ;

ptr: psyn PTRSYM	{ $$ = DoPointer($1, $2); } ;

ptrs: ptrs ptr	{
            if ($2) {
		$$ = AddPointer($1, $2);
	    } else { 
		$$=$1;
	    }
	}

        | ptr	{ $$=$1; }

        | ptrs error	{
	    Pointer p;

	    yyerror("Error in pointer list, ");
	    p = $1;
	    if (p) {
		for (; p->pnext; p = p->pnext);	/* find end of list */
		fprintf(logfile,
			"last valid pointer: \"%s\"\n",
			PrintPointer(p));
	    } else
		fprintf(logfile,"first pointer in list is bad\n");
	    $$=$1;
	}

        ;

defn:	{ $$ = NULL; }

        | DEFN	{ $$ = $1; }

        ;

key:    { $$ = NULL; }

        | SYNSETKEY { $$ = $1; }


adjclass: ','	{ $$=0; }

        | ADJCLASS ','	{ $$=$1; }

        ;

framelist: VERBFRAMES NUM	{
            if ($2 < 1 || $2 > NUMFRAMES) {
		sprintf(buf, "Bad frame number: %d\n", $2);
		yyerror(buf);
		$$ = CreateFramelist(0);
	    } else
		$$ = CreateFramelist($2);
	}

        | framelist ',' NUM	{
	    if ($3 < 1 || $3 > NUMFRAMES) {
		sprintf(buf, "Bad frame number: %d\n", $3);
		yyerror(buf);
		$$ = $1;
	    } else {
		$1->frames[($3-1)/32] |= 1<<(($3 - 1) % 32);
		$$ = $1;
	    }
	}

        | framelist error	{ 
	    yyerror("Error in frame list.\n"); 
	    $$ = $1;
	}

        ;

acluster: OPENCLUS asslist CLOSECLUS	{ cluscnt++; $$ = $2; }

        | error ']'	{
	    $$=NULL;
	    yyerror("Had trouble finding the beginning of this cluster\n");
	    yyerrok;
	}
	
        ;

asslist: ass	{ $$ = $1; }

	| asslist '-' ass	{ $$ = $1; }

	;

ass: ahead fanlist	{
            if ($1) {
		$1->fans = $2;
		$1->clusnum = cluscnt;
		AddSynset($1);
	    }
	    $$ = $1;
	}

	| error fanlist {
	    $$=NULL;
	    yyerror("Can't find a head synset for this part of the cluster\n");
	}

	;

fanlist:	{ $$=NULL; }

	| fan fanlist	{ if ($1) $1->ssnext = $2; $$ = $1; }
	    
	;

fan:  ADJSYNSET syns ptrs defn  '}' key	{
	    if ($$ = MakeSynset(SATELLITE, $2, $3, NULL, $4, $6)) {
		$$->isfanss = TRUE;
		$$->clusnum = cluscnt;
		AddToSynList($$); /* need since AddSynset() not done */
	    }
	}

	| ADJSYNSET syns defn '}' key	{
	    if ($$ = MakeSynset(SATELLITE, $2, NULL, NULL, $3, $5)) {
		$$->isfanss = TRUE;
		$$->clusnum = cluscnt;
		AddToSynList($$); /* need since AddSynset() not done */
	    }
	}

	| '[' fan	{
	    yyerror("Opened a '[' in the middle of a cluster somewhere above here\n");
	    $$=$2;
	}

	;

ahead:  ADJSYNSET headsyn syns ptrs defn '}' key	{
	    $$ = MakeSynset(ADJ, InsertSynonym($3, $2), $4, NULL, $5, $7);
	}

	| ADJSYNSET headsyn syns defn '}' key	{
	    $$ = MakeSynset(ADJ, InsertSynonym($3, $2), NULL, NULL, $4, $6);
	}

	;


headsyn: headwyrd	{
           wdcnt = 1;
           $$ = CreateSynonym($1,
			      (unsigned char)wsense,
			      -wdcnt,
			      (unsigned char)aclass,
#ifndef NO_XML
			      /* FiWN: Use label if XML in words is ignored */
			      (ignore_xmlwords ? label : NULL)
#else 
			      NULL
#endif
	       );
	   headword = FALSE;
	   aclass = 0;
       }

       | '[' headwyrd ptrs ']'	{
	   MakeSyngroup($3, NULL);
	   wdcnt = 1;
	   $$ = CreateSynonym($2,
			      (unsigned char)wsense,
			      -wdcnt,
			      (unsigned char)aclass,
#ifndef NO_XML
			      /* FiWN: Use label if XML in words is ignored */
			      (ignore_xmlwords ? label : NULL)
#else 
			      NULL
#endif
	       );
	   headword = FALSE;
	   aclass = 0;
       }
 
       ;

headwyrd: HEADWYRD adjclass	{
	    /* FiWN: Use normalize_word instead of strlower and use label */
	    $$ = CreateSymbol(normalize_word($1));
	    label = UseLabel(tolower_non_xml($1));
	    wsense = 0;
    	    aclass = $2;
	    headword = TRUE;
	}

	| HEADWYRD NUM adjclass	{
	    /* FiWN: Use normalize_word instead of strlower and use label */
	    $$ = CreateSymbol(normalize_word($1));
	    label = UseLabel(tolower_non_xml($1));
	    wsense = $2;
	    aclass = $3;
	    headword = TRUE;
	}

	;

%%

static G_Synset MakeSynset(int type, Synonym syns, Pointer ptrs,
			 Framelist frames, char *defn, int key)
{
    register Synonym syn;
    register Pointer p;
    G_Synset ss;
    Framelist f;
    
    wdcnt = 0;			/* reset word counter for next synset */
    lastsyn = NULL;		/* reset pointer to last synonym */

    if (!syns) {
	yywarn("No valid synonyms: synset discarded\n");
	return(NULL);
    } else if (type == VERB && !frames) {
	/* FiWN: Only warn if no verb frame list */
        yywarn("No frame list in verb synset\n");
	/* yyerror("No frame list in verb synset: synset discarded\n"); */
	/* return(NULL); */
    } else if (type == NOUN && foundhyper == FALSE) {
	yywarn("No hypernyms in synset\n");
	if (verifyflag)
	    errcount--;		/* don't count as real error */
    }
    foundhyper = FALSE;

    /* Link in any pointers and frame lists found in syngroups. */
    
    if (ptrlist) {
	if (ptrs) {
	    for (p = ptrs; p->pnext; p = p->pnext);
	    p->pnext = ptrlist;
	} else {
	    ptrs = ptrlist;
	}
	ptrlist = NULL;
    }
    
    if (frlist) {
	if (frames)
	    frames->fnext = frlist;
	else
	    frames = frlist;
	frlist = NULL;
    }

    ss = CreateSynset((unsigned char)type, syns, ptrs, frames, defn, key,
		      yylineno, curfilenum());

    /* Add pointer to the synset to each synonym in it.  Make sure
       that the synonym is unique (using sense number) within the file. */

    for (syn = syns; syn; syn = syn->synnext) {
	if ((type != SATELLITE && !UniqueSynSense(syn, FALSE)) ||
	    (type == SATELLITE && (curfilenum() != 0) &&
	     !UniqueSynSense(syn, TRUE))) {
	    if (syn->sensenum)
		sprintf(buf,
			"Synonym \"%s%d\" is not unique in file\n",
			syn->word->label, syn->sensenum);
	    else
		sprintf(buf,
			"Synonym \"%s\" is not unique in file\n",
			syn->word->label);
	    yyerror(buf);
	}
	syn->ss = ss;
	syn->infanss = (type == SATELLITE) ? TRUE : FALSE;
    }

    return(ss);
}

static Synonym AddSynonym(Synonym syns, Synonym newsyn)
{
    register Synonym s;

    /* Add synonym to end of list.  First make sure this synonym
       is unique within the synset. */
    
    if (UniqueSynonym(syns, newsyn)) {
	if (syns) {
	    for (s = syns; s->synnext; s = s->synnext);	/* find end of list */
	    s->synnext = newsyn;
	} else
	    syns = newsyn;
	
	lastsyn = newsyn;	/* save last synonym in synset for errors */
    } else {
	if (newsyn->sensenum)
	    sprintf(buf,
		    "Synonym \"%s%d\" is not unique in synset\n",
		    newsyn->word->label, newsyn->sensenum);
	else
	    sprintf(buf,
		    "Synonym \"%s\" is not unique in synset\n",
		    newsyn->word->label);
	yyerror(buf);
	free(newsyn);		/* free up space */
    }
    /* FiWN: Use MAXID */
    if (newsyn->sensenum > MAXID) {
	sprintf(buf,
		"Sense number must be less than %d: \"%s%d\" - changed to %d\n",
		MAXID + 1, newsyn->word->label, newsyn->sensenum,
		newsyn->sensenum % (MAXID + 1));
	yywarn(buf);
    }

    return(syns);
}

static Pointer AddPointer(Pointer ptrs, Pointer newptr)
{
    register Pointer p;
    extern int nominalizations;

    /* Discard nominalization pointer if we're skipping nominalizations */

    if (!nominalizations && (newptr->ptype == DERIVATION) ) {
	free(newptr);
	return (ptrs);
    }

    if (UniquePointer(ptrs, newptr)) {
	if (ptrs) {
	    for (p = ptrs; p->pnext; p = p->pnext);	/* find end of list */
	    p->pnext = newptr;
	} else
	    ptrs = newptr;
    } else {
	if (newptr->pfilenum != curfilenum()) {
	    sprintf(buf,
		    "Pointer \"%s:%s\" is not unique: pointer discarded\n",
		    PrintFileName(newptr->pfilenum), PrintPointer(newptr));
	} else {
	    sprintf(buf, "Pointer \"%s\" is not unique: pointer discarded\n",
		    PrintPointer(newptr));
	}
	yywarn(buf);
	free(newptr);		/* free up space */
    }

    return(ptrs);
}

static Synonym InsertSynonym(Synonym syns, Synonym newsyn)
{
    /* Insert synonym onto front of list.  First make sure this synonym
       is unique within the synset. */
    
    if (UniqueSynonym(syns, newsyn)) {
	newsyn->synnext = syns;
	syns = newsyn;
    } else {
	if (newsyn->sensenum)
	    sprintf(buf,
		    "Synonym \"%s%d\" is not unique in synset\n",
		    newsyn->word->label, newsyn->sensenum);
	else
	    sprintf(buf,
		    "Synonym \"%s\" is not unique in synset\n",
		    newsyn->word->label);
	yyerror(buf);
	free(newsyn);		/* free up space */
    }
    return(syns);
}

static void AddSynset(G_Synset ss)
{
    register G_Synset fan;
    register Synonym fansyn;
    int svlineno;
    
    if (headss == NULL)
	headss = ss;
    else
	lastss->ssnext = ss;
    lastss = ss;		/* set current end of list to this synset  */

    AddToSynList(ss);		/* update synlists for words in synset */

    /* If a cluster, make fans point back to head synset and check for
       use of a HEADWORD in a fan. */

    for (fan = ss->fans; fan; fan = fan->ssnext) {
	for (fansyn = fan->syns; fansyn; fansyn = fansyn->synnext)
	    if (!UniqueFanSynonym(fansyn, ss->clusnum, fan->lineno)) {
		sprintf(buf,
			"\"%s\" is not unique in this cluster\n",
			fansyn->word->label);
		svlineno = yylineno;
		yylineno = fan->lineno;
		yyerror(buf);
		yylineno = svlineno;
	    }
	fan->fans = ss;
    }
}    

static void AddToSynList(G_Synset ss)
{
    register Synonym syn;
    register SynList sl;
    int unique;

    /* For all words in this synset, add a pointer to this synset to
       the word's list of pointers to synsets containing the word.
       (confusing, but if you read it slowly, it makes sense) */

    for (syn = ss->syns; syn; syn = syn->synnext) {

	/* First, make sure that a pointer to this word in this
	   synset isn't already on the list.  This can occur if there
	   are multiple spellings of a word in a synset.  If only
	   one occurrence in the synset is added to the list, only
	   one pointer to this synset will be output in the index
	   file, and pointer resolution will be easier since we
	   don't have to weed out multiples. */

	unique = TRUE;

	for (sl = syn->word->syns; sl; sl = sl->snext)
#ifndef NO_XML
	    /* FiWN: If XML is to be ignored in words, also check the sense
	       number when testing for the uniqueness of a synonym */
	    if (sl->psyn->ss == syn->ss
		&& (! ignore_xmlwords || sl->psyn->sensenum == syn->sensenum)) {
#else
	    if (sl->psyn->ss == syn->ss) {
#endif
		unique = FALSE;
		break;
	    }
	if (unique) {
	    if (sl = (SynList)malloc(sizeof(Synl))) {
		sl->psyn = syn;	/* add this synonym to list */
		sl->snext = syn->word->syns;
		syn->word->syns = sl;
	    } else {
		fprintf(logfile, "Cannot malloc space for sslist\n");
		exit(-1);
	    }
	}
    }
}

static void MakeSyngroup(Pointer ptrs, Framelist frame)
{
    register Pointer p, lp;
    register int wc;

    wc = wdcnt + 1;		/* count this word */
    
    /* Set the word count for all pointers specific to this word.
       Note that since the lists of pointers and synonyms are built
       with the last entry first, the word count counts from the
       wrong direction.  Once the number of words in the synset is
       known, the word counts are adjusted. */
    
    if (ptrs) {
	for (p = ptrs; p; p = p->pnext) {
	    p->fromwdnum = (headword ? -wc : wc);
	    lp = p;
	}
	if (!ptrlist) {
	    ptrlist = ptrs;
	} else {
	    lp->pnext = ptrlist;
	    ptrlist = ptrs;
	}
    }
    
    if (frame) {
	frame->frwdnum = wc;
	if (!frlist)
	    frlist = frame;
	else {
	    frame->fnext = frlist;
	    frlist = frame;
	}
    }
}

static Pointer DoPointer(Symbol sym, int ptrtype)
{
    Pointer p = NULL;
    int mode, fn;
    char ptrbuf[100];
    static int pmode[] = { 0, P_NOUN, P_VERB, P_ADJ, P_ADV };
    extern int nominalizations;

   /* Discard nominalization pointer if we're skipping nominalizations */

    if (!nominalizations && (ptrtype == DERIVATION) ) {
	return (NULL);
    }
    
    /* used later to make sure NOUN synset has HYPERNYM */
    if (ptrtype == HYPERPTR || ptrtype == INSTANCE)
	foundhyper = TRUE;

    fn = (fnbuf[0] ? filenum(fnbuf) : curfilenum());
    mode=filemode(fn);

    p = CreatePointer(sym,
		      satellite,
		      (unsigned char)fn,
		      (unsigned char)sense,
		      (unsigned char)satellite_sense,
		      (unsigned char)ptrtype,
		      ALLWORDS, ALLWORDS);

    p->phead = headptr;

    if (mode && (legalptrs[ptrtype] & pmode[mode])) {
	return(p);
    } else {			/* Construct pretty error message & print it */
	if (fnbuf[0])
	    sprintf(ptrbuf, "\"%s:", fnbuf);
	else
	    sprintf(ptrbuf, "\"");

	sprintf(ptrbuf + strlen(ptrbuf), "%s\"", PrintPointer(p));

	if (!mode) { /* Don't print this message if only verifyng files */
	    if (!verifyflag) {
		sprintf(buf,
			"Pointer to unknown file %s: pointer discarded\n",
			ptrbuf);
		yywarn(buf);
	    }
	} else if (!legalptrs[ptrtype]) {
	    sprintf(buf,
		    "Invalid pointer type %s in %s file: pointer discarded\n",
		    ptrbuf, partnames[curfilemode()]);
	    yywarn(buf);
	} else if (!(legalptrs[ptrtype] & pmode[mode])) {
	    sprintf(buf,
		    "%s can't point to a %s file: pointer discarded\n",
		    ptrbuf, partnames[mode]);
	    yywarn(buf);
	} else {
	    sprintf(buf, "Error in pointer %s: pointer discarded\n", ptrbuf);
	    yywarn(buf);
	}
	free(p);
	return(NULL);
    }
}

static int UniqueSynonym(Synonym syns, Synonym syn)
{
    register Synonym s;

    /* check for uniqueness within this synset */

    for (s = syns; s; s = s->synnext)
	if (s->word == syn->word) {
	    if (s->label || syn->label)
		continue;	/* alternate spelling, ok. */
	    else
		return(0);
	}
    return(1);
}

static int UniquePointer(Pointer ptrs, Pointer ptr)
{
    register Pointer p;

    for (p = ptrs; p; p = p->pnext)
	if (p->pword == ptr->pword &&
	    p->psensenum == ptr->psensenum &&
	    p->pfilenum == ptr->pfilenum &&
	    p->ptype == ptr->ptype &&
	    p->fromwdnum == ptr->fromwdnum &&
	    p->towdnum == ptr->towdnum &&
	    p->pslite == ptr->pslite &&
	    p->pslite_sense == ptr->pslite_sense &&
	    p->phead == ptr->phead)
	    return(0);


    return(1);
}

static int UniqueSynSense(Synonym syn, int test)
{
    register SynList sl;
    
    for (sl = syn->word->syns; sl; sl = sl->snext)
	if ((sl->psyn->sensenum == syn->sensenum) &&
	    (sl->psyn->ss->filenum == curfilenum()) &&
	    (sl->psyn->infanss == test))
	    return(0);

    return(1);
}

static int UniqueFanSynonym(Synonym syn, int clusnum, int lineno)
{
    register SynList sl;

    for (sl = syn->word->syns; sl; sl = sl->snext)
	if ((sl->psyn->ss->clusnum == clusnum) &&
	    (sl->psyn->sensenum == syn->sensenum) && 
	    (sl->psyn->ss->lineno != lineno))
#ifdef FOOP
	if ((sl->psyn->ss->part == ADJ) && /* word is an adjective */
	    (!sl->psyn->infanss) && /* word is in a cluster head */
	    (sl->psyn->ss->clusnum == clusnum)) /* word is in same cluster */
#endif
	    return(0);

    return(1);
}

static char *UseLabel(char *s)
{
    register int i = 0;
    register char *ls;

    /* Decide if string is lower case or not */

    /* FiWN: Use normalize_word instead of strlower */
    ls = normalize_word(s);
    while (*(s + i)) {
	if (*(s + i) != *(ls + i))
	    return(s);
	i++;
    }
    return(NULL);
}

/*
 * FiWN: Strip XML tags and element contents from a string.
 */

static char *strip_xml(char *s)
{
    register int i1 = 0;
    register int i2 = 0;
    int taglevel = 0;
    int intag = 0;
    int tagstart = 0;
    int endtag = 0;
    int slash = 0;
    int bs = 0;

    while (*(s + i1)) {
	if (intag) {
	    switch (*(s + i1)) {
	    case '/':
		endtag = (tagstart == i1 - 1 - bs);
		slash = i1;
		bs = 0;
		break;
	    case '>':
		if (endtag && taglevel > 0)
		    taglevel--;
		else if (! endtag && slash != i1 - 1 - bs)
		    taglevel++;
		intag = endtag = slash = bs = 0;
		break;
	    case '\\':
		bs = 1 - bs;
		break;
	    }
	} else {
	    switch (*(s + i1)) {
	    case '<':
		intag = 1;
		tagstart = i1;
		slash = bs = 0;
		break;
	    case '\\':
		if (! bs) {
		    bs = 1;
		    break;
		} else {
		    /* fall through */
		}
	    default:
		if (taglevel == 0) {
		    if (bs) {
			/* Copy backslash */
			*(s + i2) = *(s + i1 - 1);
			i2++;
			bs = 0;
		    }
		    *(s + i2) = *(s + i1);
		    i2++;
		}
	    }
	}
	i1++;
    }
    *(s + i2) = '\0';
    return (s);
}

/*
 * FiWN: Normalize a word by lowercasing it and removing XML if XML is to be
 * ignored.
 */

static char *normalize_word(char *s)
{
    register char *ls;

    ls = strlower(s);
#ifndef NO_XML
    if (ignore_xmlwords)
	strip_xml(ls);
#endif
    return (ls);
}

/*
 * FiWN: Lowercase a string (the parts that are not within XML tags).
 */

static char *tolower_non_xml(char *s)
{
    register char *ls;

    /* FIXME: This converts to lower case everything, not only non-XML parts;
       it works for the current Finnish data, though. */
    ls = strlower(s);
    return (ls);
}

/*
  Revision Log: (since version 1.5)

  $Log: wnparse.y,v $
  Revision 1.48  2005/01/31 20:04:03  wn
  cleaned up and consolidated include files

  Revision 1.47  2004/07/16 15:15:21  wn
  fixed problem with upper case in adj files that are not in clusters

  Revision 1.46  2004/07/14 16:14:46  wn
  only convert upper case adj to lower case if not in adj.all

  Revision 1.45  2004/01/16 18:13:36  wn
  added INSTANCE

  Revision 1.44  2003/10/31 15:33:56  wn
  added code to support DERIVATION link in adj satellites

  Revision 1.43  2003/03/27 15:27:12  wn
  changed NOMIN_START and NOMIN_END with DERIVATION

  Revision 1.42  2001/09/06 17:55:38  wn
  added code for synset keys

  Revision 1.41  2001/02/16 19:52:06  wn
  *** empty log message ***

  Revision 1.40  2000/10/27 16:53:29  wn
  fixed error message

  Revision 1.39  2000/07/11 15:23:59  wn
  added code to AddPointer to skip nominalization pointers if
  !nominalizations (-n not specified)

  Revision 1.38  1997/08/05 14:23:46  wn
  added function prototypes, other small cleanups

 * Revision 1.37  1996/05/30  20:56:48  wn
 * *** empty log message ***
 *
 * Revision 1.36  1995/07/28  21:29:19  wn
 * added check for wsense > MAXID (only 1 hex digit in databae allowed)
 *
 * Revision 1.35  1995/06/16  21:09:34  wn
 * fixed bug with adj head pointers
 *
 * Revision 1.34  1995/05/25  19:00:09  wn
 * added code to find senses taht are not unique in adj.pert files
 *
 * Revision 1.33  1995/02/14  18:50:40  wn
 * fixed bug in UniquePointer
 *
 * Revision 1.32  1995/02/14  18:24:49  wn
 * *** empty log message ***
 *
 * Revision 1.31  1995/02/07  21:40:17  wn
 * fixed bug in fan and changed NOUNSS to NOUN, etc.
 *
 * Revision 1.30  1994/09/27  19:50:46  wn
 * *** empty log message ***
 *
 * Revision 1.29  1994/04/27  09:49:02  wn
 * *** empty log message ***
 *
 * Revision 1.28  94/03/03  11:56:02  wn
 * no changes
 * 
 * Revision 1.27  93/07/08  16:23:35  wn
 * added test to find noun synsets which don't have hypernyms
 * 
 * Revision 1.26  93/04/21  12:40:33  wn
 * added new ATTRIBUTE pointer (=)
 * 
 * Revision 1.1  91/09/11  11:58:56  wn
 * Initial revision
 * 

*/

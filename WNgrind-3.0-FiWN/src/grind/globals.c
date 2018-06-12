/*

  globals.c - global variables used by grinder

*/

#include <stdio.h>
#include "wngrind.h"

static char *Id = "$Id: globals.c,v 1.23 2005/01/31 20:03:36 wn Rel $";

char **Argv;
int Argc;
int nowarn = 0;			/* flag for suppressing warning messages */
FILE *logfile = NULL;		/* file to output error messages to */
G_Synset headss = NULL;		/* head of synset list */
Flist filelist[256];
int errcount=0;

Symbol hashtab[HASHSIZE];	/* hash table for words */

/* symbols used for pointer characters */ 

char *ptrsymbols[] = { 
    "(bad pointer type)",
    "!",			/* 1 ANTPTR (all) */
    "@",			/* 2 HYPERPTR (noun, verb) */
    "~",			/* 3 HYPOPTR (noun, verb) */
    "*",			/* 4 ENTAILPTR (verb) */
    "&",			/* 5 SIMPTR (adjective) */
    "#m",			/* 6 ISMEMBERPTR (noun) */
    "#s",			/* 7 ISSTUFFPTR (noun) */
    "#p",			/* 8 ISPARTPTR (noun) */
    "%m",			/* 9 HASMEMBERPTR (noun) */
    "%s",			/* 10 HASSTUFFPTR (noun) */
    "%p",			/* 11 HASPARTPTR (noun) */
    "#",			/* 12 MERONYM (noun) */
    "%",			/* 13 HOLONYM (noun) */
    ">",			/* 14 CAUSETO (verb) */
    "<",			/* 15 PPLPTR (adj) */
    "^",			/* 16 SEEALSOPTR (adjective, verb) */
    "\\",			/* 17 PERTAINSTO (adjective, noun, adverb) */
    "=",			/* 18 ATTRIBUTE (adjective, noun) */
    "$",			/* 19 VERBGROUP (verb) */
    "+",			/* 20 DERIVATION */
    ";",			/* 21 CLASSIFICATION (all) */
    "-",			/* 22 CLASS (all) */
    "",				/* 23 SYNS */
    "",				/* 24 FREQ */
    "",				/* 25 FRAMES */
    "",				/* 26 COORDS */
    "",				/* 27 RELATIVES */
    "",				/* 28 HMERONYM */
    "",				/* 29 HHOLONYM */
    "",				/* 30 WNGREP */
    "",				/* 31 OVERVIEW */
    ";c",			/* 32 classification CATEGORY */
    ";u",			/* 33 classification USAGE */
    ";r",			/* 34 classificaiton REGIONAL */
    "-c",			/* 35 class CATEGORY */
    "-u",			/* 36 class USAGE */
    "-r",			/* 37 class REGIONAL */
    "@i", 			/* 38 INSTANCE (noun) */
    "~i",			/* 39 INSTANCES (noun) */
    NULL,
    };

char legalnounptrs[]= { 0,
			    P_NOUN,P_NOUN,P_NOUN,0,0,
			    P_NOUN,P_NOUN,P_NOUN,P_NOUN,P_NOUN,
			    P_NOUN,P_NOUN,P_NOUN,0,0,
			    0,P_ADJ,P_ADJ,0,P_NOUN | P_VERB | P_ADJ | P_ADV,
			    0,0,0,0,0,
			    0,0,0,0,0,
			    0,P_NOUN,P_NOUN,P_NOUN,P_NOUN,
			    P_NOUN,P_NOUN, P_NOUN, P_NOUN,
			    0 };
char legalverbptrs[]= { 0,
			    P_VERB,P_VERB,P_VERB,P_VERB,0,
			    0,0,0,0,0,
			    0,0,0,P_VERB,P_VERB,
			    P_VERB,0,0,P_VERB,P_NOUN | P_VERB | P_ADJ | P_ADV,
			    0,0,0,0,0,
			    0,0,0,0,0,
			    0,P_NOUN,P_NOUN,P_NOUN,P_NOUN,
			    P_NOUN,P_NOUN,0,0,
			    0 };
char legaladjptrs[]=  { 0,
			    P_ADJ,0,0,0,P_ADJ,
			    0,0,0,0,0,
			    0,0,0,0,P_VERB,
			    P_ADJ,P_NOUN | P_ADJ,P_NOUN,0,P_NOUN | P_VERB | P_ADJ | P_ADV,
			    0,0,0,0,0,
			    0,0,0,0,0,
			    0,P_NOUN,P_NOUN,P_NOUN,P_NOUN,
			    P_NOUN,P_NOUN,0,0,
			    0 };
char legaladvptrs[] = { 0, 
			    P_ADV,0,0,0,0,
			    0,0,0,0,0,
			    0,0,0,0,0,
			    0,P_ADJ,0,0,P_NOUN | P_VERB | P_ADJ | P_ADV,
			    0,0,0,0,0,
			    0,0,0,0,0,
			    0,P_NOUN,P_NOUN,P_NOUN,P_NOUN,
			    P_NOUN,P_NOUN,0,0,
			    0 };
char *legalptrsets[]={ NULL, legalnounptrs , legalverbptrs, legaladjptrs,
		       legaladvptrs };
char *legalptrs=NULL;

char *ptrreflects[] = {
    NULL, 
    "!",			/* 1 ANTPTR */
    "~",			/* 2 HOLOPTR */
    "@",			/* 3 HYPERPTR */
    NULL,			/* 4 ENATAILMENT */
    "&",			/* 5 SIMPTR */
    "%m",			/* 6 HASMEMBERPTR */
    "%s",			/* 7 HASSTUFFPTR */
    "%p",			/* 8 HASPARTPTR */
    "#m",			/* 9 ISMEMBERPTR */
    "#s",			/* 10 ISSTUFFPTR */
    "#p",			/* 11 ISPARTPTR */
    "%",			/* 12 HOLONYM */
    "#",			/* 13 MERONYM */
    NULL,			/* 14 CAUSETO */
    NULL,			/* 15 PPLPTR */
    NULL,			/* 16 SEEALSOPTR */
    NULL,			/* 17 PERTAINSTO */
    "=",			/* 18 ATTRIBUTE */
    "$",			/* 19 VERBGROUP */
    "+",			/* 20 DERIVATION */
    NULL,			/* 21 CLASSIFICATION */
    NULL,			/* 22 CLASS */
    NULL,			/* 23 SYNS */
    NULL,			/* 24 FREQ */
    NULL,			/* 25 FRAMES */
    NULL,			/* 26 COORDS */
    NULL,			/* 27 RELATIVES */
    NULL,			/* 28 HMERONYM */
    NULL,			/* 29 HHOLONYM */
    NULL,			/* 30 WNGREP */
    NULL,			/* 31 OVERVIEW */
    "-c",			/* 32 classification CATEGORY */
    "-u",			/* 33 classification USAGE */
    "-r",			/* 34 classification REGIONAL */
    ";c",			/* 35 class CATEGORY */
    ";u",			/* 36 class USAGE */
    ";r",			/* 37 class REGIONAL */
    "~i",			/* 38 INSTANCE */
    "@i",			/* 39 INSTANCES */
    NULL,
    };


/* keep track of what parts of speech we've seen */

char partseen[] = { (char)0, (char)0, (char)0, (char)0, (char)0 };

/*
  Revision log:

  $Log: globals.c,v $
  Revision 1.23  2005/01/31 20:03:36  wn
  cleaned up and consolidated include files

  Revision 1.22  2004/01/16 18:12:06  wn
  addd instance/instances pointer

  Revision 1.21  2003/10/31 15:33:07  wn
  allow DERIVATION poitner between all pos

  Revision 1.20  2003/09/18 19:05:00  wn
  *** empty log message ***

  Revision 1.19  2003/04/15 13:52:01  wn
  *** empty log message ***

  Revision 1.18  2003/03/27 15:26:08  wn
  removed + with suffix

  Revision 1.17  2001/08/02 17:36:37  wn
  added CLASSIFICATION stuff

  Revision 1.16  2000/10/27 16:52:47  wn
  added for "+x"

  Revision 1.15  2000/08/07 17:37:53  wn
  fixed reflexive ptr table

  Revision 1.14  2000/07/17 19:54:48  wn
  moved specific nominalization pointers to end of list

  Revision 1.13  1996/05/30 20:56:48  wn
  *** empty log message ***

 * Revision 1.12  1994/09/27  19:51:30  wn
 * changed CAUSEDBY to PPLPTR
 *
 * Revision 1.11  1993/08/03  12:39:01  wn
 * *** empty log message ***
 *
 * Revision 1.10  93/04/21  12:40:17  wn
 * added new ATTRIBUTE pointer (=)
 * 
 * Revision 1.9  92/08/19  16:01:01  wn
 * added adv stuff
 * 
 * Revision 1.8  92/08/12  11:57:47  wn
 * *** empty log message ***
 * 
 * Revision 1.7  92/03/26  13:42:18  wn
 * removed unused pointers
 * 
 * Revision 1.6  92/01/21  13:37:01  wn
 * changed values in legal{noun,verb,adj}ptrs to use P_{NOUN,VERB,ADJ}
 * then changed P_NOUN to P_NOUN | P_ADJ for pertainyms in legaladjptrs
 * 
 * Revision 1.5  91/12/13  17:09:29  wn
 * changed include for wn.h to wnconsts.h
 * 
 * Revision 1.4  91/12/13  15:56:58  wn
 * removed variables that are now in wnglobals
 * 
 * Revision 1.3  91/11/26  15:05:26  wn
 * added pertainym pointer
 * 
 * Revision 1.2  91/09/27  11:45:26  wn
 * removed automatic reflection of see also (^)
 * 
 * Revision 1.1  91/09/11  14:49:09  wn
 * Initial revision
 * 
*/

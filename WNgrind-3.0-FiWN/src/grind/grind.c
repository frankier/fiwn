/*
  
  grind.c - Driver program for WordNet grinder

*/

#include <stdio.h>
#include "wngrind.h"

static char *Id = "$Id: grind.c,v 1.33 2005/01/31 20:03:36 wn Rel $";

static void badarg(), GetSenseCounts(), Stats(), SymStats();
static int grind();
int verifyflag = 0, statsflag = 0, distribflag = 0;
int senseindex = 0;
int ordersenses = 0;
int nominalizations = 0;
int synsetkeys = 0;
int extraflag = 0;
/* FiWN: If NO_XML, no special support for XML tags in words. */
#ifndef NO_XML
int ignore_xmlwords = 0;
#endif

main(argc,argv)
char **argv;
int argc;
{
    int i;
    char c;
    
    Argv=argv;
    Argc=argc;

    if (argc<2)
	badarg(Argv[0]);

    logfile = stderr;		/* initialize log file */

    while (*(++argv) && **argv=='-') {
	c = (*argv)[1];
	if (c == 'v')		/* verify only (don't build database) */
	    verifyflag = 1;
	else if (c == 's')	/* suppress warnings */
	    nowarn = 1;
	else if (c == 'a')	/* generate statistics */
	    statsflag = 1;
	else if (c == 'd')	/* generate distribution statistics */
	    distribflag = 1;
	else if (c == 'i')	/* if set, build sense index */
	    senseindex = 1;
	else if (c == 'o')	/* if set, order senses */
	    ordersenses = 1;
	else if (c == 'n')	/* if set, do nominalizations */
	    nominalizations = 1;
	else if (c == 'k')	/* if set, generate synset keys */
	    synsetkeys = 1;
	else if (c == 'x')	/* if set, output special database */
	    extraflag = 1;
#ifndef NO_XML
	else if (c == 'X')	/* FiWN: if set, strip XML tags in words for
				   indexing */
	    ignore_xmlwords = 1;
#endif
	else if (c == 'L') {	/* send log to file */
	    if ((logfile = fopen((*argv + 2), "w")) == NULL) {
		fprintf(stdout,
			"%s: cannot open logfile \"%s\" - using stderr\n",
			Argv[0], *argv + 2);
		logfile = stderr;
	    }
	} else
	    badarg(Argv[0]);
    }
    
    exit(grind());		/* process files */
}

static int grind()
{
    int filecnt = 0;

    /* parse input files */

    while (NextFile()) {
	yyparse();
	pcount=0;
	yylineno=1;	
	filecnt++;
    }

    /* If no input files were processed, quit now. */
    
    if (!filecnt) {
	if (verifyflag)
	    fprintf(logfile, "No input files processed.\n");
	else
	    fprintf(logfile,
		    "No input files processed; no database generated.\n");
	return(-1);
    }

    if (statsflag)
	Stats();		/* generate statistics */
    
    if (errcount) {
	fprintf(logfile, "%d syntactic %s found%s\n",
		errcount,
		(errcount == 1 ? "error" : "errors"),
		(verifyflag ? "." : "; no database generated"));
	return(errcount > 255 ? 255 : errcount);
    }

    /* If we're only verifying and the user doesn't want to see
       warnings, skip the pointer reflections. */

    if (!verifyflag || !nowarn) {
	fprintf(logfile, "Resolving pointers...\n");
	ResolvePointers();
	fprintf(logfile, "Done resolving pointers...\n");
    }
    
    if (errcount) {
	fprintf(logfile, "%d structural %s found%s\n",
		errcount,
		(errcount == 1 ? "error" : "errors"),
		(verifyflag ? "." : "; no database generated"));
	return(errcount > 255 ? 255 : errcount);
    }

    /* get sense counts and dump database */

    if (!verifyflag) {
	fprintf(stderr, "No errors...\007\n");
	fprintf(logfile, "Getting sense counts...\n");
	GetSenseCounts();
	fprintf(logfile, "Done with sense counts...\n");
	FindOffsets();		/* calculate byte offsets in data files */
	DumpData();		/* generate data files */
	if (ordersenses)
	    ReadCntlist();
	DumpIndex();		/* generate index files */
    }
 
    if (senseindex) {
	if (verifyflag) {
	  fprintf(logfile, "Getting sense counts...\n");
	  GetSenseCounts();
	  fprintf(logfile, "Done with sense counts...\n");
	  FindOffsets();
	  if (ordersenses)
	    ReadCntlist();
	}
	DumpSenseIndex();
    }

    if (errcount)
	fprintf(logfile, "%d %s\n", errcount, (errcount == 1 ? "error" : "errors"));

    return(errcount > 255 ? 255 : errcount);
}


static void GetSenseCounts()
{
    Symbol sym;
    SynList sl;
    int i, j, pos;

    for (i = 0; i < HASHSIZE; i++) {
	for (sym = hashtab[i]; sym; sym = sym->symnext) {
	    for (j = 1; j < NUMPARTS + 1; j++)
		sym->sensecnt[j] = 0;
	    for (sl = sym->syns; sl; sl = sl->snext) {
		if ((pos = sl->psyn->ss->part) == SATELLITE)
		    pos = ADJ;
		sym->sensecnt[pos]++;
	    }
	}
    }
}

#define PERT_POS	(SATELLITE + 1)

static void Stats()
{
    register G_Synset ss, fan;
    register Pointer p;
    register Synonym syn;
    int i, gotpert;
    int total = 0, dtotal = 0;
    int sscount[NUMPARTS + 3], defns[NUMPARTS + 3];
    int ptrcount = 0, syncount = 0; /* keep track of each kind too? */

    /* add # cross file ptrs when added? */

    fprintf(logfile, "\n*** Statistics for ground files:\n\n");

    for (i = 1; i <= NUMPARTS + 2; i++)
	sscount[i] = defns[i] = 0;

    for (ss = headss; ss; ss = ss->ssnext) {
	gotpert = 0;
	for (syn = ss->syns; syn; syn = syn->synnext)
	    syncount++;
	for (p = ss->ptrs; p; p = p->pnext) {
	    ptrcount++;
	    if (ss->part == ADJ && p->ptype == PERTPTR)
		gotpert = 1;
	}
	if (gotpert) {
	    sscount[PERT_POS]++;
	    if (ss->defn && *ss->defn)
		defns[PERT_POS]++;
	} else {
	    sscount[ss->part]++;
	    if (ss->defn && *ss->defn)
		defns[ss->part]++;
	    if (ss->fans) {
		for (fan = ss->fans; fan; fan = fan->ssnext) {
		    sscount[SATELLITE]++;
		    for (syn = fan->syns; syn; syn = syn->synnext)
			syncount++;
		    for (p = fan->ptrs; p; p = p->pnext)
			ptrcount++;
		    if (fan->defn && *fan->defn)
			defns[SATELLITE]++;
		}
	    }
	}
    }

    for (i = 1; i <= NUMPARTS; i++) {
	fprintf(logfile, "%d %s synsets\n", sscount[i], partnames[i]);
	total += sscount[i];
    }
    if (sscount[PERT_POS]) {
	fprintf(logfile, "%d pertainym synsets\n", sscount[PERT_POS]);
	total += sscount[PERT_POS];
    }
    if (sscount[SATELLITE]) {
	fprintf(logfile, "%d adjective satellite synsets\n",
		sscount[SATELLITE]);
	total += sscount[SATELLITE];
    }
    fprintf(logfile,
	    "%d synsets in total (including satellite and pertainym synsets)\n", total);
    for (i = 1; i <= NUMPARTS; i++) {
	fprintf(logfile, "%d %s synsets have definitional glosses\n",
		defns[i], partnames[i]);
	dtotal += defns[i];
    }
    if (sscount[PERT_POS]) {
	fprintf(logfile,
		"%d pertainym synsets have definitional glosses\n",
		defns[PERT_POS]);
	dtotal += defns[PERT_POS];
    }

    if (sscount[SATELLITE]) {
	fprintf(logfile,
		"%d adjective satellite synsets have definitional glosses\n",
		defns[SATELLITE]);
	dtotal += defns[SATELLITE];
    }
    fprintf(logfile,
	    "%d definitional glosses in total (including adjective satellite synsets)\n", dtotal);
    fprintf(logfile, "%d pointers in total\n", ptrcount);
    fprintf(logfile, "%d synonyms in synsets\n", syncount);

    SymStats();

    fprintf(logfile, "\n");
}

#define MAXCOL_LEN	10

static void SymStats()
{
    register int i, j, k;
    register Symbol sym;
    SynList sl;
    int symcount = 0, ucount;
    int wdcnt[MAXCOL_LEN];
    int sensecnt[NUMPARTS + 1][MAXCOL_LEN][MAXSENSE];
    int totalsenses[NUMPARTS + 1];

    for (i = 1; i < MAXCOL_LEN; i++)
	wdcnt[i] = 0;
    for (i = 1; i < NUMPARTS + 1; i++) {
	totalsenses[i] = 0;
	for (j = 1; j < MAXCOL_LEN; j++)
	    for (k = 0; k < MAXSENSE; k++) {
		sensecnt[i][j][k] = 0;
	    }
    }

    for (i = 0; i < HASHSIZE; i++)
	for (sym = hashtab[i]; sym; sym = sym->symnext) {
	    symcount++;
	    for (j = 0, ucount = 1; sym->label[j]; j++)
		if (sym->label[j] == '_') ucount++;
	    if (ucount >= MAXCOL_LEN)
		fprintf(logfile, "collocation too long: %s\n", sym->label);
	    else 
		wdcnt[ucount]++;
	    if (distribflag) {
		/* go down the list of uses of the word */	
		for (sl = sym->syns; sl; sl = sl->snext)
		    totalsenses[sl->psyn->ss->part]++;
		for (k = 1; k < NUMPARTS + 1; k++) {
		    if (totalsenses[k]) {
			if ((sensecnt[k][ucount][totalsenses[k] + 1]) >
			    MAXSENSE) {
			    fprintf(logfile,
				   "total number of senses exceeded: %s\n",
				   sym->label);
			} else {
			    sensecnt[k][ucount][totalsenses[k]]++;
			    totalsenses[k] = 0;
			}
		    }
		}
	    }
	}

    fprintf(logfile, "%d unique word phrases\n", symcount);
    for (i = 1; i < MAXCOL_LEN; i++)
	if (wdcnt[i])
	    fprintf(logfile, "%d word phrases of length %d\n",
		    wdcnt[i], i);

    if (distribflag) {
	for (i = 1; i < NUMPARTS + 1; i++) {
	    fprintf(logfile,
		    "Distribution of %s senses by string length\n",
		    partnames[i]);
	    fprintf(logfile, "\t\tLength of Compound\n");
	    fprintf(logfile, "1\t2\t3\t4\t5\t6\t7\t8\t9\n\n");
	    for (k = 1; k < MAXSENSE; k++) {
		for (j = 1; j < MAXCOL_LEN; j++)
		    fprintf(logfile, "%d\t",sensecnt[i][j][k]);
		fprintf(logfile, "\n");
	    }
	}
    }
}

static void badarg(cmd)
char *cmd;
{
    fprintf(stderr, "usage: %s [-v] [-s] [-Llogfile] [-a] [-d] [-i] [-o] [-n] filename [filename...]\n", Argv[0]);
    fprintf(stderr, "\t-v\tverifyonly\n");
    fprintf(stderr, "\t-s\t suppress cross file pointer warnings\n");
    fprintf(stderr, "\t-Llogfile\tsend output to \"logfile\"\n");
    fprintf(stderr, "\t-a\tgenerate statistics\n");
    fprintf(stderr, "\t-d\tgenerate distribution statistics\n");
    fprintf(stderr, "\t-i\tgenerate sense index\n");
    fprintf(stderr, "\t-o\torder senses using \"taglist\"\n");
    fprintf(stderr, "\t-n\tinclude nominalization pointers\n");
    exit(-1);
}

/*
  Revision log:

  $Log: grind.c,v $
  Revision 1.33  2005/01/31 20:03:36  wn
  cleaned up and consolidated include files

  Revision 1.32  2003/09/18 19:05:24  wn
  *** empty log message ***

  Revision 1.31  2003/09/18 19:05:00  wn
  *** empty log message ***

  Revision 1.30  2001/10/08 14:07:17  wn
  set line number to 1

  Revision 1.29  2001/09/14 13:39:20  wn
  initialize yylineno to 0 to try to fix "off by one" problem in error messages

  Revision 1.28  2001/09/06 17:55:38  wn
  added code for synset keys

  Revision 1.27  2001/05/23 14:26:42  wn
  changed comment on nominalizations

  Revision 1.26  2001/02/15 19:26:51  wn
  changed 'usage' to include -n

  Revision 1.25  2000/07/11 15:19:33  wn
  added '-n' flag for nominalizations

  Revision 1.24  1998/01/06 18:43:43  wn
  removed "-c" option and senseflag flag

  Revision 1.23  1997/09/04 20:15:46  wn
  *** empty log message ***

  Revision 1.22  1996/08/09 18:27:13  wn
  moved GetSenseCounts here

 * Revision 1.21  1996/05/30  20:56:48  wn
 * *** empty log message ***
 *
 * Revision 1.20  1995/06/23  16:36:37  wn
 * removed -l stuff
 *
 * Revision 1.19  1995/02/07  20:47:07  wn
 * changed FANSS to SATELLITE
 *
 * Revision 1.18  1995/02/02  21:46:08  wn
 * *** empty log message ***
 *
 * Revision 1.17  1994/07/01  19:21:15  wn
 * fixed bug in symstats - off by one in test for ucount
 *
 * Revision 1.16  1994/03/24  09:53:11  wn
 * *** empty log message ***
 *
 * Revision 1.15  94/03/08  13:43:11  wn
 * expanded usage message
 * 
 * Revision 1.14  94/03/08  13:37:25  wn
 * *** empty log message ***
 * 
 * Revision 1.13  94/03/03  11:56:17  wn
 * added ordersenses and call to ReadCntlist
 * 
 * Revision 1.12  94/02/24  11:09:39  wn
 * make bell ring when "No Errors"
 * 
 * Revision 1.11  94/02/03  10:58:50  wn
 * added no errors msg
 * 
 * Revision 1.10  93/04/13  13:24:29  wn
 * added code for index.sense
 * 
 * Revision 1.9  92/09/17  14:39:10  wn
 * added distribflag and associated code
 * 
 * Revision 1.8  92/08/12  15:14:46  wn
 * *** empty log message ***
 * 
 * Revision 1.7  92/01/28  16:16:27  wn
 * removed use of 'mikeflag' and made grinder always output sense number
 * after each word in synset
 * 
 * Revision 1.6  91/12/13  17:10:03  wn
 * no changes
 * 
 * Revision 1.5  91/12/13  15:58:17  wn
 * changed to use constants and variables in wnconsts and wnglobals.
 * removed -w and -m options and stub code
 * 
 * Revision 1.4  91/12/11  10:28:47  wn
 * added stuff to stats code
 * 
 * Revision 1.3  91/10/02  13:57:26  wn
 * added temporary flag (mikeflag) set with start option -x 
 * which tells grinder to generate special database for
 * mike's testing purposes
 * 
 * Revision 1.2  91/10/02  10:27:33  wn
 * made flags global instead of statid
 * 
 * Revision 1.1  91/09/11  14:49:22  wn
 * Initial revision
 * 
*/

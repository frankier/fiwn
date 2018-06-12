/*

  files.c - Functions for argument and file handling

*/

#include <stdio.h>
#include "wngrind.h"

static char *Id = "$Id: files.c,v 1.7 2005/01/31 20:03:36 wn Rel $";

extern FILE *yyin;

static char *curfilename = NULL;
static int curfnum = 0;
static int curfmode = 0;

char *curfile() { return(curfilename); }
int curfilemode() { return(curfmode); }
int curfilenum() { return curfnum; }

static void initfilelist();
static int lexfile(char *), getfiletype(char *);
int filemode(int);

char *NextFile()
{
    int i;
    static int curidx = -1;
    
    /* Set up pointers for lex to get input for parsing */

    if (curidx == -1) {		/* initialize file list */
	initfilelist();
	curidx = 0;
    } else
	curidx++;		/* skip file read last time */

    while (filelist[curidx].present == 0 &&
	   filelist[curidx].fname != NULL)
	curidx++;

    if (yyin) fclose(yyin);	/* close current file if there is one */

    if (filelist[curidx].fname == NULL) { /* end of list */
	yyin = NULL;
	return(NULL);
    } else {
	curfilename = filelist[curidx].fname;
	if ((yyin = fopen(curfilename, "r")) == NULL) {
	    fprintf(logfile, "%s: cannot open %s\n", Argv[0], curfilename);
	    return(NULL);
	} else {
	    curfmode = getfiletype(curfilename);
	    legalptrs = legalptrsets[curfmode];
	    partseen[curfmode] = 1;
	    fprintf(logfile, "Processing %s...\n", curfilename);
	    curfnum = curidx;
	    return(curfilename);
	}
    }
}

static void initfilelist()
{
    int i, fnum, maxfnum;

    for (i = 0; i <= MAXFNUM; i++) {
	filelist[i].fname = lexfiles[i];
	filelist[i].present = 0;
    }

    maxfnum = MAXFNUM;
    for (i = 1; i < Argc; i++) {
	if (getfiletype(Argv[i])) {
	    if ((fnum = lexfile(Argv[i])) == -1) { /* not hard-coded name */
		maxfnum++;
		filelist[maxfnum].fname = Argv[i];
		filelist[maxfnum].present = 1;
	    } else
		filelist[fnum].present = 1;
	}
    }
    filelist[maxfnum + 1].fname = NULL;	/* set to indicate end of list */
}

static int lexfile(char *fn)
{
    int i;

    for (i = 0; i <= MAXFNUM; i++) {
	if (!strcmp(lexfiles[i], fn))
	    return(i);
    }
    return(-1);
}

static int getfiletype(char *x)
{
    int i = 0;
    char *start = x;

    /* Check to see if string is a proper file name.  The string must
       begin with one of the valid parts of speech strings. */
    
    while(*(x + i) != '\0') {	/* find start of the base file name */
	if ((*(x +i) == '.') && (*(x + i + 1) == '.' || *(x + i + 1) == '/')) {
	    i += 2;
	    start = x + i;
	} else if (*(x + i) == '/') {
	    i++;
	    start = x + i;
	} else
	    i++;
    }
    for (i = 1; i <= NUMPARTS; i++) /* find part of speech */
	if (!strncmp(start, partnames[i], strlen(partnames[i]))) return(i);
    return(0);
}

int filemode(int num)
{
    /* Return file type (part of speech number) for file number passed. */

    if ((num < 0) || !filelist[num].present)
	return(0);
    else
	return(getfiletype(filelist[num].fname));
}

/*
  Revision log:

  $Log: files.c,v $
  Revision 1.7  2005/01/31 20:03:36  wn
  cleaned up and consolidated include files

  Revision 1.6  2001/09/14 13:39:02  wn
  added test for bad file name in pointer

  Revision 1.5  1997/08/05 14:23:46  wn
  added function prototypes, other small cleanups

 * Revision 1.4  1994/09/27  19:52:11  wn
 * changed to fixed filenames
 *
 * Revision 1.3  1994/03/03  11:56:49  wn
 * added FileNum function
 *
 * Revision 1.2  91/12/13  15:55:58  wn
 * changed to use defines in wnconsts.h
 * 
 * Revision 1.1  91/09/11  14:48:58  wn
 * Initial revision
 * 
*/


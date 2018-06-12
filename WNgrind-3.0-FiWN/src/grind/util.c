/*

  util.c - Auxiliary functions used by grinder

*/

#include <string.h>
#include <malloc.h>
#ifndef NO_UTF8
/* FiWN: UTF-8 routines from Glib */
#include <glib.h>
#endif
#include "wngrind.h"

static char *Id = "$Id: util.c,v 1.11 2005/01/31 20:03:36 wn Rel $";

/*
 * Some basic lookup functions for converting the file names
 * and pointer symbols into integer values
 */

int arraypos(char **a, char *x)
{
    register int i;
    for (i=0; a[i] != NULL; i++) {
	if (*(a[i]) && !strcmp(a[i],x)) 
	    return (i);
    }
    return (-1);
}

int filenum(char *x)
{
    int i;

    for (i=0; filelist[i].fname != NULL; i++) {
	if (!strcmp(filelist[i].fname, x)) 
	    return (i);
    }
    return (-1);
}

/*
 * A few extra string manipulation functions
 */

char *strclone(char *x)
{
    char *retval;
    
    if (retval=(char *) malloc(strlen(x)+1))
	strcpy(retval,x);
    else {
	fprintf(logfile, "Cannot malloc space to clone string\n");
	exit(-1);
    }
    return(retval);
}

#ifndef NO_UTF8

/* FiWN: UTF-8-versions of strupper and strlower, using GLib functions. */

char *strupper(char *x)
{
    /* FIXME: g_utf8_strup() allocates memory, so this leaks badly */
    return (char *) g_utf8_strup((const gchar *) x, -1);
}

char *strlower(char *x)
{
    /* FIXME: g_utf8_strdown() allocates memory, so this leaks badly */
    return (char *) g_utf8_strdown((const gchar *) x, -1);
}

#else /* NO_UTF8 */

char *strupper(char *x)
{
    static char temp[200];
    register int i=0;
    
    while (x[i]) {
	temp[i]=(islower(x[i])?toupper(x[i]):x[i]);
	i++;
    }
    temp[i]=0;
    return(temp);
}

char *strlower(char *x)
{
    static char temp[200];
    register int i=0;
    
    while (x[i]) {
	temp[i]=(isupper(x[i])?tolower(x[i]):x[i]);
	i++;
    }
    temp[i]=0;
    return(temp);
}

#endif /* ifndef NO_UTF8 - else */

char *PrintFileName(int x)
{
    static char buf[256];

    sprintf(buf, "%s", filelist[x].fname);
    return(buf);
}

char *PrintPointer(Pointer p)
{
    static char buf[100];

    if (p->psensenum) {
	if (p->pslite)
	    if (p->pslite_sense)
		sprintf(buf, "%s%d^%s%d,%s",
			p->pword->label, p->psensenum,
			p->pslite->label, p->pslite_sense,
			ptrsymbols[p->ptype]);
	    else
		sprintf(buf, "%s%d^%s,%s",
			p->pword->label, p->psensenum,
			p->pslite->label, 
			ptrsymbols[p->ptype]);
	else
	    sprintf(buf, "%s%d,%s",
		    (p->phead ?
		     strupper(p->pword->label) : p->pword->label),
		    p->psensenum,
		    ptrsymbols[p->ptype]);
    } else  {
	if (p->pslite) 
	    if (p->pslite_sense)
		sprintf(buf, "%s^%s%d,%s",
			p->pword->label,
			p->pslite->label, p->pslite_sense,
			ptrsymbols[p->ptype]);
	    else
		sprintf(buf, "%s^%s,%s",
			p->pword->label,
			p->pslite->label, 
			ptrsymbols[p->ptype]);
	else
	    sprintf(buf, "%s,%s",
		    (p->phead ?
		     strupper(p->pword->label) : p->pword->label),
		    ptrsymbols[p->ptype]);
    }
    return(buf);
}
    
char *PrintSynonym(Synonym s)
{
    static char buf[256];

    if (s->sensenum)
	sprintf(buf, "%s%d%s",
		(s->label ? s->label : s->word->label),
		s->sensenum,
		(s->adjclass ? adjclass[s->adjclass] : ""));
    else
	sprintf(buf, "%s%s",
		(s->label ? s->label : s->word->label),
		(s->adjclass ? adjclass[s->adjclass] : ""));
    return(buf);
}

/*
  Revision log:

  $Log: util.c,v $
  Revision 1.11  2005/01/31 20:03:36  wn
  cleaned up and consolidated include files

  Revision 1.10  2005/01/27 15:51:34  wn
  *** empty log message ***

  Revision 1.9  2003/09/18 19:05:00  wn
  *** empty log message ***

  Revision 1.8  1997/08/05 14:23:46  wn
  added function prototypes, other small cleanups

 * Revision 1.7  1994/09/27  19:50:32  wn
 * renamed filename to filenum
 *
 * Revision 1.6  1994/09/27  19:48:07  wn
 * added filename function
 *
 * Revision 1.5  1992/08/20  13:52:50  wn
 * changed print functions
 *
 * Revision 1.4  92/08/20  10:19:08  wn
 * changed PrintPointer to handle new pointer syntax (cluster^satellite)
 * 
 * Revision 1.3  91/12/13  15:54:24  wn
 * no changes
 * 
 * Revision 1.2  91/09/25  14:16:46  wn
 * fixed bug with upper/lower case letters in words
 * 
 * 
 * Revision 1.1  91/09/11  14:48:02  wn
 * Initial revision
 * 
*/

/*

  create.c - Build internal representation of lexicographer's files
             as input is parsed

*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "wngrind.h"

static char *Id = "$Id: create.c,v 1.13 2005/01/31 20:02:51 wn Rel $";

G_Synset CreateSynset(unsigned char part, Synonym syns, Pointer ptrs,
		    Framelist frames, char *defn, unsigned int key, int lineno,
		    unsigned char filenum)
{
    G_Synset ss;

    if (ss = (G_Synset)malloc(sizeof(G_Ss))) {
	ss->ssnext = NULL;
	ss->fans = NULL;
	ss->syns = syns;
	ss->ptrs = ptrs;
	ss->frames = frames;
	ss->defn = defn;
	ss->key = key;
	ss->part = part;
	ss->isfanss = FALSE;
	ss->filenum = filenum;
	ss->lineno = lineno;
	ss->clusnum = 0;
	ss->filepos = 0;
    } else {
	fprintf(logfile, "Cannot malloc space for synset\n");
	exit(-1);
    }
    return(ss);
}

Pointer CreatePointer(Symbol sym, Symbol slite, unsigned char fn,
		      unsigned char sense, unsigned char slite_sense,
		      unsigned char type,
		      short from, short to)
{
    Pointer p;

    if (p = (Pointer)malloc(sizeof(Ptr))) {
	p->psynset = NULL;
	p->pnext = NULL;
	p->pword = sym;
	p->pslite = slite;
	p->pfilenum = fn;
	p->psensenum = sense;
	p->pslite_sense = slite_sense;
	p->phead = FALSE;
	p->ptype = type;
	p->fromwdnum = from;
	p->towdnum = to;
	p->status = UNRESOLVED;
    } else {
	fprintf(logfile, "Cannot malloc space for pointer\n");
	exit(-1);
    }
    return(p);
}

Synonym CreateSynonym(Symbol sym, unsigned char sense, short wdnum,
		      unsigned char aclass, char *label)
{
    Synonym s;

    if (s = (Synonym)malloc(sizeof(Syn))) {
	s->synnext = NULL;
	s->ss = NULL;
	s->word = sym;
	s->sensenum = sense;
	s->sswdnum = wdnum;
	s->tagcnt = 0;		/* default in case not tagged */
	s->wnsensenum = 0;	/* default if not sorted */
	s->adjclass = aclass;
	s->label = label;
	s->infanss = FALSE;
    } else {
	fprintf(logfile, "Cannot malloc space for word\n");
	exit(-1);
    }
    return(s);
}

Framelist CreateFramelist(int frameval)
{
    int i;
    Framelist f;
    
    if (f = (Framelist)malloc(sizeof(Fr))) {
	f->fnext = NULL;
	for (i = 0; i <= (NUMFRAMES / 32); i++) 
	    f->frames[i] = 0;	/* clear all frames */
	f->frames[(frameval - 1)/32] |= 1<<((frameval - 1) % 32);
	f->frwdnum = 0;
    } else {
	fprintf(logfile, "Cannot malloc space for verb frames\n");
	exit(-1);
    }
    return(f);
}

static unsigned hash();

Symbol CreateSymbol(char *s)
{
    Symbol sptr;
    unsigned hashval;
    int i;
    static int initflag = 0;

    if (!initflag) {		/* initialize hash table pointers */
	for (i = 0; i < HASHSIZE; i++)
	    hashtab[i] = NULL;
	initflag = 1;
    }

    if (!(sptr = FindSymbol(s))) { /* not found */
	sptr = (Symbol)malloc(sizeof(Sym));
	if (!sptr || (sptr->label = strdup(s)) == NULL) {
	    fprintf(logfile, "Cannot malloc space for symbol name\n");
	    exit(-1);
	}
	sptr->syns = NULL;
	for (i = 0; i <= NUMPARTS; i++)
	    sptr->sensecnt[i] = NOSENSE;
	hashval = hash(s);
	sptr->symnext = hashtab[hashval];
	hashtab[hashval] = sptr;
    }
    return(sptr);
}

static unsigned hash(char *s)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
	hashval = *s + 31 * hashval;
    return(hashval % HASHSIZE);
}

Symbol FindSymbol(char *s)
{
    Symbol sptr;

    for (sptr = hashtab[hash(s)]; sptr != NULL; sptr = sptr->symnext)
	if (strcmp(s, sptr->label) == 0)
	    return(sptr);	/* found */

    return(NULL);		/* not found */
}

/*
  Revision log: (since version 1.5)

  $Log: create.c,v $
  Revision 1.13  2005/01/31 20:02:51  wn
  cleaned up include files

  Revision 1.12  2005/01/18 20:46:52  wn
  *** empty log message ***

  Revision 1.11  2001/09/06 17:55:38  wn
  added code for synset keys

  Revision 1.10  1997/08/05 14:23:46  wn
  added function prototypes, other small cleanups

 * Revision 1.9  1994/03/24  09:53:02  wn
 * *** empty log message ***
 *
 * Revision 1.8  94/03/03  11:55:02  wn
 * added tagcnt field
 *
 * Revision 1.1  91/09/11  14:48:29  wn
 * Initial revision
 * 
*/

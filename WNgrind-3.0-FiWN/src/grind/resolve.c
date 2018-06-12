/*

  resolve.c - Resolve and reflect database pointers

*/

#include <stdio.h>
#include "wngrind.h"

static char *Id = "$Id: resolve.c,v 1.27 2005/04/05 17:42:19 wn Exp $";

static void DoPointers(G_Synset), DoFanPointers(G_Synset);
static void ResolveIt(Pointer, Synonym, G_Synset, int);
static int DuplPointer(Pointer, G_Synset);
static int SamePointer(unsigned char, Pointer, Pointer);
static void CheckSimilars(G_Synset), AddPointer(G_Synset, Pointer);
static void InsertPointer(G_Synset, Pointer);
static Pointer ReflectPointer(G_Synset, G_Synset, unsigned char, short);
static Pointer ReflectSimPointer(G_Synset, G_Synset, short, int);
static Synonym FindUniqueSynonym(Pointer, G_Synset, int, int);
static Synonym FindUniqueSatellite(Pointer, G_Synset);
static int HasSimilar(G_Synset);

void ResolvePointers()
{
    G_Synset ss;

    /* Loop through all the synsets, resolving the pointers for
       each one. */

    for (ss = headss; ss; ss = ss->ssnext) {
	DoPointers(ss);
	if (ss->fans)
	    DoFanPointers(ss);
    }
}

static void DoPointers(G_Synset ss)
{
    Pointer p;
    Synonym syn;

    for (p = ss->ptrs; p; p = p->pnext) {

	if (p->status == UNRESOLVED) { /* don't do if already reflected */

	    /* Find the synset containing the word that this pointer
	       points to.  If there is no synset containing the word,
	       or if there is more than one synset which contains the
	       word, print an error message.  If the synset is the head
	       of a cluster, look for target synset in cluster's fan
	       synsets. */

	    if (ss->clusnum &&
		(p->ptype != PERTPTR && p->ptype != ATTRIBUTE &&
		 p->ptype != DERIVATION) &&
		 (p->ptype < CLASSIF_START || p->ptype > CLASSIF_END)) {
		    
		if
		    /* Resolve antonym in this cluster */

		    ((p->phead && p->ptype == ANTPTR &&
		      (syn = FindUniqueSynonym(p, ss, FALSE, ss->clusnum))) ||

		     /* Resolve pointer to another cluster (negative 
			cluster number to indicate another cluster) */

		     (p->phead &&
		      (syn = FindUniqueSynonym(p, ss, FALSE, -ss->clusnum))) ||

		     /* Resolve to fan synset of this cluster */

		     (!p->phead &&
		      (syn = FindUniqueSynonym(p, ss, TRUE, ss->clusnum))))
			
			ResolveIt(p, syn, ss, FALSE);

	    } else if (syn = FindUniqueSynonym(p, ss, FALSE, 0))
		ResolveIt(p, syn, ss, FALSE);
		    
	    /* If here, pointer is unresolved - no synset contains word
	       with same sense and file number. */
	    
	    if (p->status == UNRESOLVED) {
		fprintf(logfile, "Unresolved pointer: ");
		if (ss->filenum != p->pfilenum)
		    fprintf(logfile, "%s:", filelist[p->pfilenum].fname);
		fprintf(logfile,
			"%s in file %s, line %d\n",
			PrintPointer(p),
			filelist[ss->filenum].fname,
			ss->lineno);
		errcount++;
	    }
	}
    }
}

static void DoFanPointers(G_Synset clusheadss)
{
    Pointer p;
    Synonym syn;
    G_Synset fan;

    for (fan = clusheadss->fans; fan; fan = fan->ssnext) {

	for (p = fan->ptrs; p; p = p->pnext) {

	    if (p->status == UNRESOLVED) { /* don't do if already reflected */

		/* Fan synsets may contain pointers to other head
		   synsets.  Resolve fan synset pointers to other
		   clusters and reflect the cluster pointed to to this
		   fan's head synset (pass ResolveIt 'fan' rather than
		   'clusheadss'). */
		
		/* Find the synset containing the word that this pointer
		   points to.  If there is no synset containing the word,
		   or if there is more than one synset which contains the
		   word, print an error message. */

		if
		    /* Resolve to another cluster */
		    
		    ((p->phead &&
		      (syn = FindUniqueSynonym(p,
					       fan,
					       FALSE,
					       -clusheadss->clusnum))) ||

		     /* Resolve to this cluster */

		     (!p->phead &&
		      (syn = FindUniqueSynonym(p,
					       fan,
					       FALSE,
					       clusheadss->clusnum))) ||

		     /* Resolve to another fan in this cluster */

		     (!p->phead && p->ptype == ANTPTR &&
		      (syn = FindUniqueSynonym(p,
					       fan,
					       TRUE,
					       clusheadss->clusnum))))
#ifdef FOOP
			ResolveIt(p, syn, clusheadss, TRUE);
#endif
			ResolveIt(p, syn, fan, TRUE);
		else if ( ( ( p->ptype >= CLASSIF_START &&
			      p->ptype <= CLASSIF_END ) ||
			      p->ptype == DERIVATION ) &&
			 (syn = FindUniqueSynonym(p, fan, FALSE, 0) ) ) 
		    ResolveIt(p, syn, fan, FALSE);
		
		/* If here, pointer is unresolved - no synset contains word
		   with same sense and file number. */
		
		if (p->status == UNRESOLVED) {
		    fprintf(logfile, 
			    "Unresolved fan pointer: %s in file %s, line %d\n",
			    PrintPointer(p),
			    filelist[clusheadss->filenum].fname,
			    fan->lineno);
		    errcount++;
		}
	    }
	}
    }

    CheckSimilars(clusheadss);	/* make sure all satellites are SIMILARs */
}

static void CheckSimilars(G_Synset clusheadss)
{
    G_Synset fan;
    Pointer ref, lastp;

    for (fan = clusheadss->fans; fan; fan = fan->ssnext) {
	if (!HasSimilar(fan)) {	/* no similar in fan synset */
#ifdef FOOP
	    fprintf(logfile,
		    "Warning: Missing similar pointer in file %s, line %d\n",
		    filelist[clusheadss->filenum].fname,
		    fan->lineno);
#endif

	    /* Automatically reflect pointer from fan to cluster head */

	    ref = CreatePointer(NULL,
				NULL,
				clusheadss->filenum,
				0,
				0,
				SIMPTR,
				0,
				0);
	    ref->psynset = clusheadss;
	    ref->status = RESOLVED;
	    InsertPointer(fan, ref); /* add pointer to beginning of chain */

	    /* Now create pointer from cluster head to satellite */

	    ref = CreatePointer(NULL,
				NULL,
				clusheadss->filenum,
				0,
				0,
				SIMPTR,
				0,
				0);
	    ref->psynset = fan;
	    ref->status = RESOLVED;
	    AddPointer(clusheadss, ref); /* add pointer to end of chain */
	}
    }
}


static void ResolveIt(Pointer p, Synonym syn, G_Synset ss, int fanss)
{
    Pointer ref, lastp;

    if ( syn->ss == ss && p->fromwdnum == ALLWORDS) {
	fprintf(logfile, "Self-referential pointer: %s in file %s, line %d\n",
                PrintPointer(p),
                filelist[ss->filenum].fname,
                ss->lineno);
	p->status = SELF_REF;
	errcount++;
	return;
    }
    
    p->psynset = syn->ss; /* point to target synset */

    p->towdnum = (p->fromwdnum != ALLWORDS) ? syn->sswdnum : ALLWORDS;

    /* If this pointer needs to be reflected, see if
       the lexicographer put in the reflection.  If not,
       automatically reflect the pointer. */

    if (ptrreflects[p->ptype]) {
	if (p->ptype == SIMPTR && ss->clusnum)
	    ref = ReflectSimPointer(syn->ss, ss, p->fromwdnum, fanss);
	else	
	    ref = ReflectPointer(syn->ss,
				 ss,
				 ptrkind(ptrreflects[p->ptype]),
				 p->fromwdnum);
	if (!ref) {
	    ref = CreatePointer(NULL,
				NULL,
				ss->filenum,
				0,
				0,
				ptrkind(ptrreflects[p->ptype]),
				p->towdnum,
				p->fromwdnum);
	    ref->psynset = ss;
	    AddPointer(syn->ss, ref); /* add pointer to end of chain */
	    if (!DuplPointer(ref, syn->ss))
		ref->status = RESOLVED;
	    else
		ref->status = DUPLICATE;
	}
    }

    /*
       Make sure this pointer isn't a duplicate.  It's possible
       for the same semantic relation to inadvertently get put into
       a synset if the same relation is entered for more than one
       target concent, and they're in the same synset:

       ex: {tree, forest,#m woods,#m} where forest and woods are synonyms

    */

    if (DuplPointer(p, ss))
	p->status = DUPLICATE;
    else
	p->status = RESOLVED;
}

static Pointer ReflectPointer(G_Synset ss, G_Synset tss,
			      unsigned char ptype, short wdnum)
{
    Pointer p;
    Synonym syn, head;

    head = (tss->isfanss ? tss->fans->syns : NULL);

    for (p = ss->ptrs; p; p = p->pnext) {

	for (syn = tss->syns; syn; syn = syn->synnext) {

	    if ((head &&
		 p->ptype == ptype &&
		 syn->word == p->pslite &&
		 syn->sensenum == p->pslite_sense &&
		 head->word == p->pword &&
		 head->sensenum == p->psensenum &&
		 tss->filenum == p->pfilenum) ||

		(p->ptype == ptype &&
		 syn->word == p->pword &&
		 syn->sensenum == p->psensenum &&
		 tss->filenum == p->pfilenum) ) {

		if (wdnum == ALLWORDS || syn->sswdnum == wdnum) {
		    p->psynset = tss;	/* point to target synset */
		    p->towdnum = wdnum;	/* set target word number */
		    p->status = RESOLVED;
		    return(p);
		}
	    }
	}
    }
    return(NULL);
}

static Pointer ReflectSimPointer(G_Synset ss, G_Synset tss,
				 short wdnum, int fanss)
{
    Pointer p;
    Synonym syn;

    /* 'fanss' is used to make sure we've found a pointer to the
       right synset.  In a cluster structure, head synsets have
       similar pointers to all fan synsets, and all fan synsets have
       a similar pointer to the cluster head.  These pointers are
       specified in lower case.  When ReflectSimPointer is called,
       fanss is FALSE when trying to match head/fan pointers within
       a single cluster.  It is TRUE when trying to match a pointer
       from a fan to a different cluster head. */

    syn = tss->syns;		/* look for pointer to head word */

    for (p = ss->ptrs; p; p = p->pnext) {
	if (syn->word == p->pword &&
	    p->phead == fanss &&
	    syn->sensenum == p->psensenum &&
	    tss->filenum == p->pfilenum &&
	    p->ptype == SIMPTR) {

	    if (wdnum == ALLWORDS || syn->sswdnum == wdnum) {
		p->psynset = tss; /* point to target synset */
		p->towdnum = wdnum; /* set target word number */
		p->status = RESOLVED;
		return(p);
	    }
	}
    }
    return(NULL);
}

static Synonym FindUniqueSynonym(Pointer p, G_Synset ss, int fanss, int clusnum)
{
    SynList sl;
    Synonym syn = NULL;
    int i = 0;
	    
    for (sl = p->pword->syns; sl; sl = sl->snext)
	if (sl->psyn->sensenum == p->psensenum &&
	    sl->psyn->ss->filenum == p->pfilenum &&
	    sl->psyn->ss->isfanss == fanss) {

	    if (p->pslite) {	/* found cluster head */

		/* Pointer to a satellite of this cluster head */

		if (syn = FindUniqueSatellite(p, sl->psyn->ss))
		    i++;

	    } else if
		
		/* Pointer to a headword (sswdnum < 0) in this cluster */

		(((clusnum > 0) && (sl->psyn->ss->clusnum == clusnum) &&
		  (sl->psyn->sswdnum < 0) && (fanss == FALSE)) ||

		 /* Pointer to a headword in another cluster */

		 ((clusnum < 0 ) && (sl->psyn->ss->clusnum != -clusnum) &&
		  (sl->psyn->sswdnum < 0) && (fanss == FALSE)) ||

		 /* Pointer to a fan synset in this part of cluster */

		 ((clusnum > 0 ) && (sl->psyn->ss->fans == ss) &&
		  (fanss == TRUE)) ||

		 /* Pointer to a fan synset in other part of cluster */

		 ((clusnum > 0 ) && (sl->psyn->ss->clusnum == clusnum) &&
		  (fanss == TRUE)) ||

		 /* Not a cluster, previous test suffice */

		 (clusnum == 0)) {
#ifdef FOOP

		    /* If one synonym has been found already, and
		       another has just been found, if they are in
		       the same synset it's ok.  (Synonym has been
		       entered in one synset with alternate or
		       non-lower-case spelling) If synset is not
		       the same, then the pointer is ambiguous. */

		    if (syn && (syn->ss == sl->psyn->ss))
			continue;
		    else {
#endif
			syn = sl->psyn;	/* get snyonym structure for word */
			i++;
#ifdef FOOP
		    }
#endif
		}
	}

    if (i > 1) {
	fprintf(logfile, "Ambiguous pointer: %s in file %s, line %d\n",
		PrintPointer(p),
		filelist[ss->filenum].fname,
		ss->lineno);
	errcount++;
    }
    return(i == 1 ? syn : NULL);
}

static Synonym FindUniqueSatellite(Pointer p, G_Synset ss)
{
    G_Synset slite;
    Synonym syn;

    for (slite = ss->fans; slite; slite = slite->ssnext)
	for (syn = slite->syns; syn; syn = syn->synnext)
	    if ((p->pslite == syn->word) &&
		(p->pslite_sense == syn->sensenum))
		return(syn);
    return(NULL);
}

static int DuplPointer(Pointer ptr, G_Synset ss)
{
    Pointer p;

    for (p = ss->ptrs; p; p = p->pnext) {
	if (p != ptr && p->status != DUPLICATE &&
	    SamePointer(ss->part, ptr, p)) {

	    fprintf(logfile, "Duplicate pointer: ");
	    if (ss->filenum != ptr->pfilenum)
		fprintf(logfile, "%s:", filelist[ptr->pfilenum].fname);
	    fprintf(logfile, " type=%s,%s in file %s, line %d\n",
		    ptrsymbols[ptr->ptype], ptrsymbols[p->ptype],
		    filelist[ss->filenum].fname, ss->lineno);
	    return(1);
	}
    }
    return(0);
}
	
static int SamePointer(unsigned char pos, Pointer p1, Pointer p2)
{
    if ((p1->psynset == p2->psynset) &&
	(p1->pslite == p2->pslite) &&
	(p1->pfilenum == p2->pfilenum) &&
	(p1->psensenum == p2->psensenum) &&
	(p1->pslite_sense == p2->pslite_sense) &&
	(p1->phead == p2->phead) &&
	(p1->fromwdnum == p2->fromwdnum) &&
	(p1->towdnum == p2->towdnum)) {

	if (pos == NOUN) {	/* NOUN - don't check type (unless
				   nominalization or classification)
				   and fail */
	    if ((p1->ptype == DERIVATION || p2->ptype == DERIVATION) ||
		((p1->ptype >= CLASSIF_START && p1->ptype <= CLASS_END) ||
		 (p2->ptype >= CLASSIF_START && p2->ptype <= CLASS_END)))
		    return(0);
	    else
		return(1);
	} else {
	    if (p1->ptype == p2->ptype)	return(1);
	    else return(0);
	}
    } else return(0);
} 

static void InsertPointer(G_Synset ss, Pointer p)
{
    /* insert at beginning of list */

    if (ss->ptrs)
	p->pnext = ss->ptrs;

    ss->ptrs = p;
}

static void AddPointer(G_Synset ss, Pointer p)
{
    /* add at end of list */

    Pointer lastp;

    if (ss->ptrs) {
	for (lastp = ss->ptrs; lastp->pnext; lastp = lastp->pnext);
	lastp->pnext = p;
    } else
	ss->ptrs = p;
}

static int HasSimilar(G_Synset fan)
{
    Pointer p;

    for (p = fan->ptrs; p; p = p->pnext) {
	if (p->ptype == SIMPTR && p->status == RESOLVED && p->psynset == fan)
	    return(1);
    }
    return(0);
}

/*
  Revision log: (since version 1.5)

  $Log: resolve.c,v $
  Revision 1.27  2005/04/05 17:42:19  wn
  wrote InsertPointer to insert similar pointer at start of list instead of end

  Revision 1.26  2005/01/31 20:03:36  wn
  cleaned up and consolidated include files

  Revision 1.25  2005/01/27 15:51:34  wn
  updated to not give error if similar pointer is missing

  Revision 1.24  2003/11/17 19:11:01  wn
  fixed bug in ReflectPointer for adj satellites

  Revision 1.23  2003/10/31 15:33:24  wn
  added code to allow DERIVATION in adjectives

  Revision 1.22  2003/03/27 15:26:31  wn
  replaced NOMIN_START and NOMIN_END with DERIVATION

  Revision 1.21  2003/02/06 19:02:43  wn
  added SELF_REF error

  Revision 1.20  2002/12/20 15:31:32  wn
  added test for self-referential pointers

  Revision 1.19  2002/10/28 15:23:09  wn
  fixed bug with unresolved classificaiton pointers

  Revision 1.18  2001/11/06 18:52:49  wn
  added code to grind classification stuff

  Revision 1.17  2001/10/04 18:20:52  wn
  worked on duplicate pointer code

  Revision 1.16  2001/09/25 17:19:27  wn
  fixed SamePointer to allow same in nouns if nominalizations

  Revision 1.15  2001/09/14 13:41:09  wn
  added code tofind duplicate pointers in a synset pointing to same target

  Revision 1.14  1997/08/05 14:23:46  wn
  added function prototypes, other small cleanups

 * Revision 1.13  1994/09/27  19:53:01  wn
 * changed to use filelist as structure
 *
 * Revision 1.12  1993/04/21  12:40:29  wn
 * added new ATTRIBUTE pointer (=)
 *
 * Revision 1.1  91/09/11  14:49:59  wn
 * Initial revision
 * 
*/

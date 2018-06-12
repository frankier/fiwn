/*

  makedb.c - Build/dump WordNet database

*/

#include <stdio.h>
#include <string.h>
#include "wngrind.h"

static char *Id = "$Id: makedb.c,v 1.42 2005/01/31 20:03:36 wn Rel $";

#define PADLEN	3

static int GetSize(Synonym, Pointer, Framelist, char *);

void FindOffsets()
{
    register G_Synset ss, fan;
    int count[NUMPARTS + 1];	/* no part 0 */
    int i;
    
    fprintf(logfile, "Figuring out byte offsets...\n");

    for (i = 1; i <= NUMPARTS; i++)
	count[i] = DBLICENSE_SIZE - 1;

    for (ss = headss; ss; ss = ss->ssnext) {

	ss->filepos = count[ss->part];
	count[ss->part] += GetSize(ss->syns, ss->ptrs, ss->frames, ss->defn);

	/* Find size of cluster fans */

	for (fan = ss->fans; fan; fan = fan->ssnext) {
	    fan->filepos = count[ADJ];
	    count[ADJ] += GetSize(fan->syns, fan->ptrs, NULL, fan->defn);
	}
    }
}

#define	SSMINSIZE	23
#define SSFIXSIZE	3
#define PTRSIZE		17
#define DEFNFIXSIZE	2
#define FRAMESIZE	8
#define FIXFRAMESIZE	4

static int GetSize(Synonym s, Pointer p, Framelist f, char *defn)
{
    register int total = SSMINSIZE;
    int flag = 0;

    /* Find size of synonyms */
    
    for (; s; s = s->synnext) {
	/* FiWN: If a word has a label different from the word itself,
	   calculate the size according to it. The length of the label may
	   differ from that of the word to accommodate comments as XML
	   elements. */
	total += (SSFIXSIZE + strlen(s->label ? s->label : s->word->label));
	if (s->adjclass)
	    total += strlen(adjclass[s->adjclass]);
    }
    
    /* Add size of pointers */
    
    for (; p; p = p->pnext) {

	switch (p->status) {
	case RESOLVED:
	    total += (PTRSIZE + strlen(ptrsymbols[p->ptype]));
	    break;
	case UNRESOLVED:
	    fprintf(logfile, "Unresolved pointers exist. Aborting.\n");
	    exit(-1);
	case DUPLICATE:
	    break;
	}
    }
    
    /* If there's a definition, add its length */
    
    if (defn && *defn)
	total += (DEFNFIXSIZE + strlen(defn));
    
    /* For each framelist, add in its size */
    
    for (; f; f = f->fnext) {
	int i;
	
	if (f->frames) {
	    for (i = 0; i < NUMFRAMES; i++)
		if (1 << (i % 32) & f->frames[i / 32]) {
		    total += FRAMESIZE;
		    flag = 1;
		}
	}
    }
    if (flag)
	total += FIXFRAMESIZE;
    else 
	total++;		/* kludge - off by one if no frames */
    
    return(total);
}

static FILE *ofp;
static FILE *keyfp;
static void DumpSynset(G_Synset);
static void DumpSynonyms(Synonym);
static void DumpPointers(Pointer);
static void DumpFrames(Framelist);
static void DumpDefn(char *);
static void DumpKey(unsigned int, long, char);
static void DumpWords(FILE *, int);
static void FixLastRecord(int);
static void DumpSenses(FILE *);

void DumpData()
{
    register G_Synset ss;
    FILE *ofile[NUMPARTS + 1];	/* no part 0 */
    char tempfn[100];
    int i;
    
    fprintf(logfile, "Dumping data files...\n");

    for (i = 1; i <= NUMPARTS; i++)
	ofile[i] = NULL;
    
    for (i = 1; i <= NUMPARTS; i++) {
	if (partseen[i]) {
	    sprintf(tempfn,"data.%s",partnames[i]);
	    if ((ofile[i] = fopen(tempfn,"w")) == NULL) {
		fprintf(logfile,
			"%s: Cannot open output file %s\n", Argv[0], tempfn);
		goto abortdump;
	    }
	    fprintf(ofile[i], "%s", dblicense);
	}
    }

    /* Generate "index.key" file */

    if (synsetkeys) {
	if ((keyfp = popen("env LC_ALL=C sort +0 -1 > index.key", "w")) == NULL) {
	    fprintf(logfile,
		    "%s: Cannot open pipe to output keyfile index.key\n",
		    Argv[0]);
	    exit(-1);
	}
    }

    for (ss = headss; ss; ss = ss->ssnext) {
	if (ftell(ofile[ss->part]) != ss->filepos) {
	    fprintf(logfile,
		    "%s: sanity error - actual pos %d != assigned pos %d!\n",
		    Argv[0], ftell(ofile[ss->part]), ss->filepos);
	    goto abortdump;
	}
	ofp = ofile[ss->part];
	DumpSynset(ss);
    }

abortdump:
    for (i = 1; i <= NUMPARTS; i++)
	if (partseen[i] && ofile[i])
	    fclose(ofile[i]);

    if (synsetkeys) 
	pclose(keyfp);

    fprintf(logfile, "Done dumping data files...\n");
}

static void DumpSynset(G_Synset ss)
{
    register G_Synset fan;

    /* BYTE_OFFSET FILENUMBER [n | v | a] */

    fprintf(ofp,"%8.8d %2.2d %c ",
	    ss->filepos, ss->filenum, partchars[ss->part]);

    DumpSynonyms(ss->syns);
    DumpPointers(ss->ptrs);
    DumpFrames(ss->frames);
    DumpDefn(ss->defn);
    DumpKey(ss->key, ss->filepos, partchars[ss->part]);
    fprintf(ofp,"  \n");

    for (fan = ss->fans; fan; fan = fan->ssnext) {
	/* pos is 's' for satellite */
	fprintf(ofp,"%8.8d %2.2d s ",
		fan->filepos, fan->fans->filenum);

	DumpSynonyms(fan->syns);
	DumpPointers(fan->ptrs);
	DumpDefn(fan->defn);
	DumpKey(fan->key, fan->filepos, partchars[fan->part]);
	fprintf(ofp,"  \n");
    }
}

static void DumpSynonyms(Synonym syns)
{
    register Synonym s;
    register int count;

    /* NUMBER_OF_SYNONYMS */

    count = 0;
    for (s = syns; s; s = s->synnext) count++;
    fprintf(ofp, "%2.2x ", count);

    /* list of synonyms */

    for (s = syns; s; s = s->synnext) {
	if (s->adjclass)
	    fprintf(ofp, "%s%s ",
		    s->label ? s->label : s->word->label,
		    adjclass[s->adjclass]);
	else
	    fprintf(ofp, "%s ", (s->label ? s->label : s->word->label));
	/* FiWN: Print a warning on sense number overflow. */
	if (s->sensenum >= 16)
	    fprintf(logfile, "%s: lex id %d > 15, changed to %d\n",
		    s->word->label, s->sensenum, s->sensenum % 16);
	fprintf(ofp, "%1.1x ", s->sensenum % 16);
    }
}

static void DumpPointers(Pointer ptrs)
{
    register Pointer p;
    register int count;

    /* NUMBER_OF_POINTERS */

    count = 0;
    for (p = ptrs; p; p = p->pnext)
	if (p->status == RESOLVED) { count++; }
    fprintf(ofp, "%3.3d ", count);
    
    /* list of pointers */

    for (p = ptrs; p; p = p->pnext)
	if (p->status == RESOLVED) {
	    fprintf(ofp,"%s %8.8d %c %2.2x%2.2x ",
		    ptrsymbols[p->ptype],
		    p->psynset->filepos,
		    partchars[p->psynset->part],
		    (p->fromwdnum < 0 ? -p->fromwdnum : p->fromwdnum),
		    (p->towdnum < 0 ? -p->towdnum : p->towdnum));
	}
}

static void DumpFrames(Framelist frames)
{
    register Framelist f;
    register int count;
    register unsigned int i;

    /* NUMBER_OF_VERBFRAMES list of verb frames*/

    count = 0;
    for(f = frames; f; f = f->fnext) {
	if (f->frames) {
	    for (i=0;i<NUMFRAMES; i++)
		if ((1<<(i%32)) & f->frames[i/32]) count++;
	}
    }
    if (count) {
	fprintf(ofp, "%2.2d ", count);
	for(f = frames; f; f = f->fnext) {
	    if (f->frames) {
		for (i=0;i<NUMFRAMES; i++)
		    if ((1<<(i%32)) & f->frames[i/32])
			fprintf(ofp,"+ %2.2d %2.2x ", i + 1, f->frwdnum);
	    }
	}
    }
}

static void DumpDefn(char *defn)
{

    /* | DEFINITION */

    if (defn)
	fprintf(ofp, "| %s", defn);
}

static void DumpKey(unsigned int key, long offset, char pos)
{
    /* unique synset key gets printed to file along with offset and pos */

    if (synsetkeys && key != 0) {
	fprintf(keyfp, "%6.6d %c:%8.8d\n", key, pos, offset);
    }
}

void DumpIndex()
{
    int i;
    char cmd[100], fn[16];
    FILE *ofs, *fp;

    fprintf(logfile, "Dumping index files...\n");

    for (i = 1; i <= NUMPARTS; i++) {
	if (partseen[i]) {
	    sprintf(fn, "index.%s", partnames[i]);
	    if ((fp = fopen(fn, "w")) == NULL) {
		fprintf(logfile, "Cannot open file: %s\n", fn);
		exit(-1);
	    }
	    
	    fprintf(fp, "%s", dblicense);
	    fflush(fp);
	    /* FiWN: Specify LC_ALL=C to get a consistent sorting order. */
	    sprintf(cmd, "env LC_ALL=C sort +0 -1 >> %s", fn);
	    if ((ofs = popen(cmd, "w")) == NULL) {
		fprintf(logfile, "Cannot open pipe: %s\n", cmd);
		exit(-1);
	    }
	    DumpWords(ofs, i);
	    pclose(ofs);
	    FixLastRecord(i);	/* make 2nd to last record longer than last */
	}
    }
    fprintf(logfile, "Done dumping index files...\n");
}

static void DumpWords(FILE *fs, int part)
{
    register int i, j;
    register Symbol sym;
    register SynList sl;
    Pointer p;
    G_Synset ss;
    int printflag, ptr_cnt, synset_cnt, hasframes, attest_cnt;
    char ptrused[LASTTYPE + 1];
    int part2, idx, done;
    SynList sl_list[MAXSENSE], tmp; /* array of pointers to senses */
    
    part2 = (part == ADJ ? SATELLITE : part);

    for (i = 0; i < HASHSIZE; i++) {
	for (sym = hashtab[i]; sym; sym = sym->symnext) {

	    printflag = synset_cnt = ptr_cnt = hasframes = idx = 0;
	    for (j = 1; j <= LASTTYPE; j++)
		ptrused[j] = 0;

	    for (sl = sym->syns; sl; sl = sl->snext) {

 		/* First, get the synset that this word belongs to */

		ss = sl->psyn->ss;

		if (ss->part != part && ss->part != part2)
		    continue;	/* wrong part of speech */

		synset_cnt++;	/* count this synset */

		/* Find all the pointers that come from this
		   word.  If this is the first pointer of this type
		   found, increment the type counter and set a flag
		   for this pointer type.  Later the pointer count
		   and list of pointer characters is output. */

		for (p = ss->ptrs; p; p = p->pnext) {
		    if (p->status == RESOLVED &&
			(p->fromwdnum == ALLWORDS ||
			 p->fromwdnum == sl->psyn->sswdnum)) {
			if ( (p->ptype >= CLASSIF_START) &&
			    (p->ptype <= CLASSIF_END) &&
			    !ptrused[CLASSIFICATION] ) {
			    ptrused[CLASSIFICATION] = 1;
			    ptr_cnt++;
			} else if ( (p->ptype >= CLASS_START) &&
				   (p->ptype <= CLASS_END) &&
				   !ptrused[CLASS] ) {
			    ptrused[CLASS] = 1;
			    ptr_cnt++;
			} else if ( p->ptype == INSTANCE &&
				    !ptrused[HYPERPTR] ) {
			    ptrused[HYPERPTR] = 1;
			    ptr_cnt++;
			} else if ( p->ptype == INSTANCES &&
				    !ptrused[HYPOPTR] ) {
			    ptrused[HYPOPTR] = 1;
			    ptr_cnt++;
			} else if ( p->ptype <= LASTTYPE  &&
				   !ptrused[p->ptype]) {
			    ptrused[p->ptype] = 1;
			    ptr_cnt++;
			}
		    }
		}
#ifdef FOOP
		if (ss->frames) hasframes = 1; /* add in verb frames */
#endif

		if (!printflag) {
		    fprintf(fs, "%s %c %d ",
			    strlower(sym->label),
			    partchars[part],
			    (sym->sensecnt[part] != NOSENSE
			     ? sym->sensecnt[part] : 0));
		    printflag = 1;
		}
	    }

	    if (printflag) {

		/* output pointer count and types */

		fprintf(fs, "%d ", ptr_cnt);

#ifdef FOOP
		fprintf(fs, "%d ", ptr_cnt + hasframes);
		if (hasframes)	/* print verb frame symbol */
		    fprintf(fs, "+ ");
#endif

		if (ptr_cnt)
		    for (j = 1; j <= LASTTYPE; j++)
			if (ptrused[j])
			    fprintf(fs, "%s ", ptrsymbols[j]);

		/* output synset count and offsets */

		fprintf(fs, "%d ", synset_cnt);

		/* if ADJ, output cluseter heads, then fans */

		if (part == ADJ) {
		    for (sl = sym->syns; sl; sl = sl->snext) {
			ss = sl->psyn->ss;
			if (ss->part == ADJ)
			    if ((idx + 1) >= MAXSENSE) {
				fprintf(logfile,
					"%s: %s %s has > MAXSENSE senses\n",
					Argv[0],
					partnames[ss->part], sym->label);
			    } else
				sl_list[idx++] = sl;
		    }
		    for (sl = sym->syns; sl; sl = sl->snext) {
			ss = sl->psyn->ss;
			if (ss->part == SATELLITE)
			    if ((idx + 1) >= MAXSENSE) {
				fprintf(logfile,
					"%s: %s %s has > MAXSENSE senses\n",
					Argv[0],
					partnames[ss->part], sym->label);
			    } else
				sl_list[idx++] = sl;
		    }
		} else {
		    for (sl = sym->syns; sl; sl = sl->snext) {
			ss = sl->psyn->ss;
			if (ss->part == part || ss->part == part2)
			    if ((idx + 1) >= MAXSENSE) {
				fprintf(logfile,
					"%s: %s %s has > MAXSENSE senses\n",
					Argv[0],
					partnames[ss->part], sym->label);
			    } else
				sl_list[idx++] = sl;
		    }
		}

		attest_cnt = 0;

		/* Sort senses from most to least frequently tagged. */
		if (ordersenses) {
		    done = 0;
		    while (!done) {
			done = 1;
			for (j = 0; j < idx - 1; j++) {
			    if (sl_list[j]->psyn->tagcnt <
				sl_list[j + 1]->psyn->tagcnt) {
				tmp = sl_list[j];
				sl_list[j] = sl_list[j + 1];
				sl_list[j + 1] = tmp;
				done = 0;
			    }
			}
		    }
		    if (sl_list[0]->psyn->tagcnt == 0) {
			attest_cnt = 0;
		    } else {
			for (j = 1; j < idx; j++) {
			    if (sl_list[j]->psyn->tagcnt == 0)
				break;
			}
			attest_cnt = j;
		    }
		}

		fprintf(fs, "%d ", attest_cnt);

		/* Output offsets and store sense number for sense index */
		for (j = 0; j < idx; j++) {
		    fprintf(fs, "%08d ", sl_list[j]->psyn->ss->filepos);
		    sl_list[j]->psyn->wnsensenum = j + 1;
		}
		fprintf(fs, " \n");
	    }

	}
    }
}

void DumpSenseIndex()
{
    char cmd[100];
    FILE *ofs, *fp;
    
    fprintf(logfile, "Dumping sense index...\n");
    
    if ((fp = fopen("index.sense", "w")) == NULL) {
	fprintf(logfile, "Cannot open file: index.sense\n");
	exit(-1);
    }

    sprintf(cmd, "sort +0 -1 >> index.sense");
    if ((ofs = popen(cmd, "w")) == NULL) {
	fprintf(logfile, "Cannot open pipe: %s\n", cmd);
	exit(-1);
    }
    DumpSenses(ofs);
    pclose(ofs);
    fprintf(logfile, "Done dumping sense index...\n");
}

static void DumpSenses(FILE *fs)
{
    register int i;
    register G_Synset s;
    register Symbol sym;
    register SynList sl;

    for (i = 0; i < HASHSIZE; i++) {
	for (sym = hashtab[i]; sym; sym = sym->symnext) {

	    for (sl = sym->syns; sl; sl = sl->snext) {
		s = sl->psyn->ss;
		if (s->part != SATELLITE)
		    fprintf(fs, "%s%%%-1.1d:%-2.2d:%-2.2d::",
			    strlower(sym->label),
			    s->part,
			    s->filenum,
			    sl->psyn->sensenum);
		else
#ifdef FOOP
		    fprintf(fs, "%s%%%-1.1d:%-2.2d:%-2.2d:%s%s:%-2.2d",
#endif
			    /* don't print adj class */

		    fprintf(fs, "%s%%%-1.1d:%-2.2d:%-2.2d:%s:%-2.2d",
			    strlower(sym->label),
			    s->part,
			    s->filenum,
			    sl->psyn->sensenum,
			    s->fans->syns->word->label,
#ifdef FOOP
			    adjclass[s->fans->syns->adjclass],
#endif
			    s->fans->syns->sensenum);

		/* Dump byte offset, WordNet sense number and
		   number of tags in semantic concordance. */

		fprintf(fs, " %-8.8d %d %d\n",
			s->filepos, sl->psyn->wnsensenum, sl->psyn->tagcnt);
	    }
	}
    }
}

#define BUFSIZE		4196

static void FixLastRecord(int part)
{
    char fn[16];
    FILE *fp;
    char buf[BUFSIZE];
    int i = BUFSIZE;
    int lastlen, secondtolastlen, pad, svpos;

    /* Funky routine to pad the second to the last record of the
       index file to be longer than the last record so the binary
       search in the search code works properly. */

    sprintf(fn, "index.%s", partnames[part]);
    if ((fp = fopen(fn, "r+")) == NULL) {
	fprintf(logfile, "Cannot open file: %s\n", fn);
	exit(-1);
    }

    /* Move to 2 characters from the end of file (don't read final
       newline).  Read the last entry and save it in a buffer.
       Record it's starting location in the buffer.  Then read the
       previous record.  Calculate the length of both records.  If
       the second to last record is shorter than (or equal to) the
       last record, pad spaces onto the end of the second to last
       record, then ouput the last record which was saved in the buffer. */

    buf[--i] = '\0';

    fseek(fp, -2L, 2);

    while ((buf[--i] = getc(fp)) != '\n')
	fseek(fp, -2L, 1);
    fseek(fp, -2L, 1);

    svpos = i;			/* save location of last record */

    while (getc(fp) != '\n') {
	--i;
	fseek(fp, -2L, 1);
    }

    secondtolastlen = svpos - i;
    lastlen = BUFSIZE - svpos;
    pad = lastlen - secondtolastlen;

    if (pad > 0) {
	fseek(fp, (long)(secondtolastlen - 1), 1);
	while(pad--)
	    putc(' ', fp);
	fprintf(fp, "\n%s\n", buf + svpos + 1);
    }
    fclose(fp);
}

void ReadCntlist()
{
    FILE *fp;
    register Symbol sym;
    register SynList sl;
    register Synonym syn;
    int cnt, id, pos, filenum, hid, wnsns;
    char *lparen;
    char wd[100], buf[100], head[100];

    if ((fp = fopen("cntlist", "r")) == NULL) {
	fprintf(logfile, "Cannot open file: cntlist\n");
	fprintf(logfile, "Cannot order senses\n");
	return;
    }

    fprintf(logfile, "Reading cntlist...\n");

    while (fscanf(fp, "%d %[^%]%%%1d:%2d:%2d:%s %d\n",
		  &cnt, wd, &pos, &filenum, &id, buf, &wnsns) != EOF) {

	if (pos == SATELLITE) {
	    sscanf(buf, "%[^:]:%2d", head, &hid);
	    if ((lparen = strchr(head, '(')) != NULL) {
	        *lparen = '\0';	/* truncate string at '(' if present */
	    }
	} else
	    hid = -1;

	sym = FindSymbol(wd);
	if (sym != NULL) {
	    for (sl = sym->syns; sl; sl = sl->snext) {
		syn = sl->psyn;
		if (syn->sensenum == id &&
		    syn->ss->part == pos &&
		    syn->ss->filenum == filenum) {

		    if (hid == -1) {
			syn->tagcnt = cnt;
			break;
		    } else {
			if (syn->ss->fans->syns->sensenum == hid &&
			    !strcmp(syn->ss->fans->syns->word->label, head)) {
			    syn->tagcnt = cnt;
			    break;
			}
		    }
		}
	    }
	}
    }
    fprintf(logfile, "Done reading cntlist\n");
}

/*
  Revision log: (since version 1.5)

  $Log: makedb.c,v $
  Revision 1.42  2005/01/31 20:03:36  wn
  cleaned up and consolidated include files

  Revision 1.41  2005/01/27 15:51:34  wn
  *** empty log message ***

  Revision 1.40  2004/01/16 18:12:30  wn
  updated dumpwords to check for new instance/instances pointer

  Revision 1.39  2003/06/19 17:57:36  wn
  fixed bug in dumpindex

  Revision 1.38  2003/04/15 17:07:09  wn
  fixed temporary loss of domains

  Revision 1.37  2003/04/15 13:52:36  wn
  *** empty log message ***

  Revision 1.36  2001/09/14 13:40:38  wn
  added code to skip generation of DUPLICATE pointers

  Revision 1.35  2001/09/06 17:55:38  wn
  added code for synset keys

  Revision 1.34  2001/05/24 16:25:58  wn
  removed adjective marker from satellite in sense key when printing
  sense index

  Revision 1.33  2000/07/17 19:54:20  wn
  in FixLastRecord, changed "a+" to "r+"

  Revision 1.32  1999/07/22 16:10:12  wn
  fixed bug in ordering senses - wasn't working for SATELLITE with markers
  in 'head' field.

  Revision 1.31  1997/10/09 18:39:16  wn
  fixed bug in calculating number of attested senses

  Revision 1.30  1997/09/04 20:15:59  wn
  *** empty log message ***

 * Revision 1.29  1997/08/08  19:15:04  wn
 * added "attest_cnt" field to index file
 *
 * Revision 1.28  1997/08/05  14:26:01  wn
 * cleanups
 *
 * Revision 1.27  1996/05/30  20:56:48  wn
 * *** empty log message ***
 *
 * Revision 1.26  1995/06/23  16:39:23  wn
 * removed nonlocal stuff
 *
 * Revision 1.25  1995/02/07  20:46:50  wn
 * changed FANSS to SATELLITE
 *
 * Revision 1.24  1994/11/04  16:49:01  wn
 * fixed bug in readcntlist for satellites
 *
 * Revision 1.23  1994/09/27  19:52:33  wn
 * changed to use new filelist structure
 *
 * Revision 1.22  1994/08/18  19:05:42  wn
 * fixed bug in ReadCntlist for satellites (naturally)
 *
 * Revision 1.21  1994/07/14  17:54:46  wn
 * changed sense index code to new fmt
 *
 * Revision 1.20  1994/07/14  16:46:51  wn
 * changed ReadCntlist to read new format
 *
 * Revision 1.19  1994/07/01  19:26:52  wn
 * *** empty log message ***
 *
 * Revision 1.18  94/03/24  09:53:31  wn
 * *** empty log message ***
 * 
 * Revision 1.17  94/03/03  11:57:06  wn
 * added ReadCntlist and code to sort senses by most freq. tagged sense
 * 
 * Revision 1.16  94/02/03  10:59:09  wn
 * changed sort cmmand - took out -z5120
 * 
 * Revision 1.15  93/05/05  14:41:19  wn
 * changed pos field from 2 to 1 byte in DumpSenses
 * 
 * Revision 1.14  93/04/13  13:24:48  wn
 * added code for index.sense
 * 
 * Revision 1.1  91/09/11  14:49:42  wn
 * Initial revision
 * 
*/

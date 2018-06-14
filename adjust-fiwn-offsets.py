import sys
from os.path import join as pjoin
from nltk.corpus import wordnet
from finntk.wordnet.reader import fiwn
from atomicwrites import atomic_write
from more_itertools import peekable

TSV_COL_MAP = {
    ("rels", "lexrels"): {0, 2},
    ("rels", "semrels"): {0, 1},
    ("rels", "synsets"): {0},
    ("rels", "transls"): {0},
    ("rels", "wsenses"): {0},
    ("lists", "semrels-extra"): {0, 3},
    ("lists", "synsets-extra"): {0, 5},
}


def get_extra_synset_info(dir):
    wsenses_f = peekable(open(pjoin(dir, "fiwn-wsenses.tsv")))
    for line in wsenses_f:
        if wsenses_f.peek().startswith("fi:n90"):
            break
    semrels_f = peekable(open(pjoin(dir, "fiwn-semrels.tsv")))
    for line in semrels_f:
        if semrels_f.peek().startswith("fi:n90"):
            break
    while 1:
        semrels_line = next(semrels_f)
        semrels_cols = semrels_line[:-1].split("\t")
        wsenses_line = next(wsenses_f)
        wsenses_cols = wsenses_line[:-1].split("\t")
        assert semrels_cols[0] == wsenses_cols[0]
        orig_posoff = des_fi_synset_key(wsenses_cols[0])
        lemma = wsenses_cols[1]
        assert semrels_cols[3] == "hypernym", "Expected hypernym, got {}".format(semrels_cols[3])
        related = semrels_cols[1]
        rel_posoff = des_fi_synset_key(related)
        yield orig_posoff, lemma, rel_posoff


def make_sense_map(dir):
    # 1. Comm
    fi_synsets = fiwn.all_synsets()
    en_synsets = wordnet.all_synsets()
    en2fi = {}
    try:
        while 1:
            en_synset = next(en_synsets)
            fi_synset = next(fi_synsets)
            while fi_synset.definition() == '[empty]':
                fi_synset = next(fi_synsets)
            assert en_synset.definition() == fi_synset.definition()
            assert en_synset.pos() == fi_synset.pos()
            en2fi[(en_synset.pos(), en_synset.offset())] = \
                fi_synset.offset()
    except StopIteration:
        pass

    covered_fi = [(pos, fi_off) for (pos, _en_off), fi_off in en2fi.items()]

    # 2. Other synsets contain a single uniquely identifiable lemma
    for (pos, offset), lemma, (rel_pos, rel_off) in get_extra_synset_info(dir):
        map_rel_off = en2fi[rel_pos, rel_off]
        fi_lemmas = fiwn.lemmas(lemma)
        cand_synsets = []
        for fi_lemma in fi_lemmas:
            fi_synset = fi_lemma.synset()
            matches = False
            for hypernym in fi_synset.hypernyms():
                if hypernym.pos() == rel_pos and hypernym.offset() == map_rel_off:
                    matches = True
            if matches:
                if (fi_synset.pos(), fi_synset.offset()) in covered_fi:
                    print("Apparent duplicates: {} and {}".format(
                        cons_fi_synset_key(pos, offset),
                        cons_fi_synset_key(fi_synset.pos(), fi_synset.offset())))
                else:
                    cand_synsets.append(fi_synset)
        assert len(cand_synsets) == 1, \
            "Got {} synsets for {}, related to {}".format(
                len(cand_synsets),
                lemma,
                cons_fi_synset_key(hypernym.pos(), hypernym.offset())
            )
        fi_synset = cand_synsets[0]
        assert pos == fi_synset.pos()
        en2fi[(pos, offset)] = fi_synset.offset()
    return en2fi


def cons_fi_synset_key(pos, offset):
    if pos == "s":
        pos = "a"
    return "fi:{}{:08d}".format(pos, offset)


def des_fi_synset_key(synset_key):
    synset = synset_key.split(":", 1)[1]
    return synset[0], int(synset[1:], 10)


def sensemap_to_stringmap(sense_map):
    string_map = {}
    for (pos, en_offset), fi_offset in sense_map.items():
        string_map[cons_fi_synset_key(pos, en_offset)] = cons_fi_synset_key(pos, fi_offset)
    return string_map


def fix_rels_dir(dir, string_map):
    for (dir_name, name), col_nums in TSV_COL_MAP.items():
        fn = pjoin(dir, dir_name, "fiwn-{}.tsv".format(name))
        with open(fn) as inf, atomic_write(fn, overwrite=True) as outf:
            for line in inf:
                cols = line.split("\t")
                for col_num in col_nums:
                    if " " in cols[col_num] or cols[col_num] == "":
                        # Column 5 of synsets-extra
                        if cols[col_num].startswith("fi:"):
                            synset_ref, rest = cols[col_num].split(" ", 1)
                            cols[col_num] = "{} {}".format(string_map[synset_ref], rest)
                    else:
                        #print(cols)
                        cols[col_num] = string_map[cols[col_num]]
                outf.write("\t".join(cols))


if __name__ == '__main__':
    if sys.argv[1] == "dump":
        rels = pjoin(sys.argv[2], "rels")
        mapping = sensemap_to_stringmap(make_sense_map(rels))
        with open(sys.argv[3], "w") as outf:
            for old_key, new_key in mapping.items():
                outf.write("{}\t{}\n".format(old_key, new_key))
    elif sys.argv[1] == "fix":
        rels = pjoin(sys.argv[2], "rels")
        fix_rels_dir(
            sys.argv[2],
            sensemap_to_stringmap(make_sense_map(rels)))
    else:
        print("{} dump | fix".format(sys.argv[0]))

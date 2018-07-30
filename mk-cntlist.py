from nltk.corpus.reader.wordnet import WordNetError
from nltk.corpus import wordnet
from finntk.wordnet.reader import fiwn
import sys
import fractions
from functools import reduce

# This lemma is broken in data.noun
LEMMA_NAME_FIXES = {"moderniusAdditional_synonym": "modernius"}

# These lemmas occur in differently upper/lower case wise in transls and data.POS
IGNORE_CASE_LEMMAS = {"ci", "otc", "rh-negatiivinen", "rh-positiivinen"}


def lcm(a, b):
    return abs(a * b) / fractions.gcd(a, b) if a and b else 0


def surf_to_norm_lemma(surf_lemma):
    return surf_lemma.replace(" ", "_").replace(",", "\\,").replace("(", "\\(").replace(
        ")", "\\)"
    )


def synset_surf_to_lemma(synset, surf):
    lemmas = synset.lemmas()
    for lemma in lemmas:
        normed_lemma = surf_to_norm_lemma(surf)
        if (
            normed_lemma.lower() in IGNORE_CASE_LEMMAS
            and normed_lemma.lower() == lemma.name().lower()
        ):
            return lemma
        fixed_lemma_name = LEMMA_NAME_FIXES.get(lemma.name(), lemma.name())
        if fixed_lemma_name == normed_lemma:
            return lemma


def get_lemma(wn, synset_key, lemma_str):
    pos = synset_key[0]
    offset = int(synset_key[1:], 10)
    synset = wn.synset_from_pos_and_offset(pos, offset)
    return synset_surf_to_lemma(synset, lemma_str)


def get_transl_iter():
    for line in open("data/rels/fiwn-transls.tsv"):
        fi_synset, fi_lemma, en_synset, en_lemma, rel, extra = line[:-1].split("\t")
        _, fi_synset = fi_synset.split(":", 1)
        _, en_synset = en_synset.split(":", 1)
        fi_lemma = fi_lemma.split("<", 1)[0]
        yield fi_synset, fi_lemma, en_synset, en_lemma, rel, extra


def calc_fiwn_counts():
    en2fi = {}
    for (
        fi_synset_key, fi_lemma_str, en_synset_key, en_lemma_str, rel, extra
    ) in get_transl_iter():
        if rel != "synonym":
            continue

        fi_lemma = get_lemma(fiwn, fi_synset_key, fi_lemma_str)
        assert fi_lemma is not None

        en_lemma = get_lemma(wordnet, en_synset_key, en_lemma_str)
        assert en_lemma is not None

        en2fi.setdefault(en_lemma.key(), []).append(fi_lemma.key())
    divisors = set()
    counts = {}
    for en, fis in en2fi.items():
        for fi in fis:
            counts.setdefault(fi, 0.0)
            try:
                en_lemma = wordnet.lemma_from_key(en)
            except WordNetError:
                # The following lemmas are not in the PWN sense index for some reason:
                # ['earth%1:17:02::', 'ddc%1:06:01::', 'kb%1:23:01::', 'sun%1:17:02::',
                # 'moon%1:17:03::', 'earth%1:15:01::', 'ddi%1:06:01::', 'kb%1:23:03::']
                pass
            else:
                div = len(fis)
                divisors.add(div)
                counts[fi] += en_lemma.count() / div
    mult = reduce(lcm, divisors)
    for lemma, cnt in counts.items():
        counts[lemma] = int((cnt * mult) + 0.5)
    return counts


def main():
    fiwn_counts = calc_fiwn_counts().items()
    lemma_cnts = sorted(
        (
            (lemma.encode('utf-8'), -cnt)
            for lemma, cnt in fiwn_counts
        )
    )
    prev_lemma_bytes = None
    number = 1
    for lemma_bytes, cnt in lemma_cnts:
        if lemma_bytes != prev_lemma_bytes:
            number = 1
            prev_lemma_bytes = lemma_bytes
        sys.stdout.buffer.write(b"%b %d %d\n" % (lemma_bytes, number, -cnt))
        number += 1


if __name__ == '__main__':
    main()

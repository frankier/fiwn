# FinnWordNet

This repository contains some changes/fixes to FinnWordNet.

The `data` directory contains the FiWN data files, and the `WNgrind-3.0-FiWN`
directory contains the FiWN version of WNgrind.

## Mapping/adjusting script

There is a script which can either create a false/en based synset id => true fi
synset id mapping tsv, or apply the mapping to the tsvs in data. It needs
[pipenv](https://github.com/pypa/pipenv).

Assuming you put the original data in `data` rather than the already mapped
data included here, you can make a map tsv like so:

    $ pipenv run python adjust-fiwn-offsets.py dump data synset_map.tsv

And you can also modify the original data with the new offsets (i.e. the
following is the command which has been run to change the data in `data` to its
current state):

    $ pipenv run python adjust-fiwn-offsets.py fix data

## Fake word count data script

You can create count data based on the counts in the English data like so:

    $ pipenv run python mk-cntlist.py > data/dict/cntlist.rev

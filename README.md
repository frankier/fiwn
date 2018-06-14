# FinnWordNet

This repository contains some changes/fixes to FinnWordNet.

The `data` directory contains the FiWN data files, and the `WNgrind-3.0-FiWN`
directory contains the FiWN version of WNgrind.

## Script

There is a script which can either create a false/en based synset id => true fi
synset id mapping tsv, or apply the mapping to the tsvs in data. It needs
[pipenv](https://github.com/pypa/pipenv).

Assuming you put the original data in `data` rather than the already mapped
data included here, you can make a map tsv like so:

    $ pipenv run python adjust-fiwn-offsets.py dump data synset_map.tsv

And you can also modify the original data with the new offsets:

    $ pipenv run python adjust-fiwn-offsets.py fix data

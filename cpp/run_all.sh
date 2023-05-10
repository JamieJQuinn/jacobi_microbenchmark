#!/usr/bin/env bash

set -e

for f in $(ls *.x); do
  ./run.sh $f 10
done

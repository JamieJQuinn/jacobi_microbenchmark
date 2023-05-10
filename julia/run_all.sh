#!/usr/bin/env bash

set -e

for f in $(ls *.jl); do
  ./run.sh $f 10
done

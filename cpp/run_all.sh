#!/usr/bin/env bash

set -e

n_iterations=${1:-10}

echo Running for $n_iterations iterations

for f in $(ls *.x); do
  ./run.sh $f $n_iterations
done

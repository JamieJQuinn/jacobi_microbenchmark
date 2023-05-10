#!/usr/bin/env bash

set -e

for f in $(ls *.x); do
  if [ "$f" == "v014_external_parameters.x" ]; then
    echo "Cannot pass parameters to $f. Skipping."
  else
    ./run.sh $f 10
  fi
done

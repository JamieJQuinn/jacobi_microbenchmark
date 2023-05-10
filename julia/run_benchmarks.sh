#!/usr/bin/env bash

set -e

for f in $(ls *.jl); do
  julia $f | tail -n1
done | sort -nk6 | column -t -s' '

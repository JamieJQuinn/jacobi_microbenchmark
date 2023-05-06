#!/usr/bin/env bash

set -e

for f in $(ls *.x); do
  ./$f | tail -n1
done


for f in $(ls v014*.x); do
  ./$f 128 128 65536 | tail -n1
done

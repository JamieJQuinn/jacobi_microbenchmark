#!/usr/bin/env bash

set -e

for f in $(ls *.x); do
  ./$f | tail -n1
done

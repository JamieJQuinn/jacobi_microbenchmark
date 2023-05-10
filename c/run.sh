#!/usr/bin/env bash

set -e

EXE=$1
REPEATS=$2

CSV=${EXE%.x}.csv

echo Running $EXE $REPEATS times

if [ ! -f $CSV ]; then
  echo "exe_name, language, nx, ny, max_iterations, runtime, average_error" > $CSV
fi

for i in $(seq 1 $REPEATS); do
  ./$EXE >> $CSV
done

top_line=$(head -n1 $CSV)

sed -i "s~$top_line~~g" $CSV
sed -i '/^[[:space:]]*$/d' $CSV
sed -i "1s~^~$top_line\n~" $CSV


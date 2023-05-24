#!/usr/bin/env python3

import argparse
import pandas as pd


def main():
    parser = argparse.ArgumentParser(
        description="Process CSVs containing microbenchmark performance data")
    parser.add_argument('files', nargs='*')
    parser.add_argument('--sort', default=True, action='store_true')
    args = parser.parse_args()
    df = pd.DataFrame()
    for f in args.files:
        df = pd.concat([df, pd.read_csv(f, sep=',\s+', engine='python')])

    column = df.groupby('exe_name')['runtime']
    series = [column.min().rename("min"),
              column.mean().rename("mean"),
              column.std().rename("std")]
    df = pd.concat(series, axis=1)

    if args.sort:
        print(df.sort_values(by='mean'))
    else:
        print(df)


main()

#!/usr/bin/env python3

import argparse
import pandas as pd

def main():
    parser = argparse.ArgumentParser(description="Process CSVs containing microbenchmark performance data")
    parser.add_argument('files', nargs='*')
    args = parser.parse_args()
    df = pd.DataFrame()
    for f in args.files:
        df = pd.concat([df, pd.read_csv(f, sep=',\s+', engine='python')])

    print(df.groupby('exe_name')['runtime'].mean())

main()

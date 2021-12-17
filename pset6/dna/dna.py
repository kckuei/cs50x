# Import modules
import sys
import csv
import numpy as np
import pandas as pd


def main():
    # Check arguments
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Get filenames
    filename = sys.argv[1]
    sequence_file = sys.argv[2]

    # Read STR database
    STR_data = pd.read_csv(filename)
    STR_list = STR_data.columns[1:].to_list()

    # Read DNA sequence
    with open(sequence_file, "r") as f:
        sequence = f.readline()

    # print(STR_data)
    # print(STR_list)
    # print(sequence)

    # Store the STR counts in a results dict
    result = {}
    for str_patt in STR_list:
        result[str_patt] = max_STR_repeats(sequence, str_patt)
    # print(result)

    # Check the results against the database
    for i in range(len(STR_data)):
        is_match = True
        for str_patt in result.keys():
            if STR_data.iloc[i][str_patt] != result[str_patt]:
                is_match = False
                break
        if is_match:
            print(STR_data.iloc[i]['name'])
            break
    if not is_match:
        print("No match.")


def max_STR_repeats(sequence, STR_pattern):
    """
    function to find max repeats of consequcutive STR
    """
    n = len(sequence)
    n_str = len(STR_pattern)

    # Get indices of all occurences of STR pattern
    i = 0
    indices = []
    while (i < n - n_str):
        window = sequence[i:i+n_str]
        if window == STR_pattern:
            # print(window)
            indices.append(i)
        i += 1

    # Count the max consecutive occurences of the STR pattern
    # print(indices)
    if indices:
        max_counts = 1
        counter = 1
        for i in range(len(indices)-1):
            # is consecutive
            if indices[i+1] - indices[i] == n_str:
                counter += 1
            # otherwise reset
            else:
                counter = 1
            # update max_counts
            max_counts = max(counter, max_counts)

    else:
        max_counts = 0

    return max_counts


main()
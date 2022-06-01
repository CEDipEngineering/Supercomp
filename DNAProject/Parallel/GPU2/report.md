# Local Search

## Problem

Given two sequences of DNA bases (strings containing A, C, T, G and -, - meaning there is gap in the sequence), determine the greatest possible similarity between any given substring of the two sequences.

## Algorithm

The Local Search algorithm will take a random substring of one of the sequences, and then try to match it to a series of random subsequences in the other sequence, so as to try varying combinations, and hopefully achieve a close solution to the real best fit.


## Implementation

The implementation makes use of the string type and random indexation to test several substrings against a set piece of one of the dna sequences. It accumulates the best result and then returns the best match to that random subsequence.

In this particular scenario, there are two implementations, that make use of the same base calculations, defined in 'func.hpp', however one runs 50 experiments in series, whereas the other runs the 50 in parallel using multithreading (OpenMP).

## Output Example
 
Subsequence size: 4
Sequências:
TA-G
TAC-
Score: 2
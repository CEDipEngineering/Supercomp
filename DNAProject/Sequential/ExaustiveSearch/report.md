# Exaustive Search

## Problem

Given two sequences of DNA bases (strings containing A, C, T, G and -, - meaning there is gap in the sequence), determine the greatest possible similarity between any given substring of the two sequences.

## Algorithm

Exaustive Search consists on a naive implementation of the basic concept of "Search every combination to make sure you know the best one". It is pure exploration, as it needs no information about the structure of the problem to create the space of possibilities.


## Implementation

The implementation of this algorithm is very straightforward. First we iterate over each string, acumulating all possible substrings in a set (set is used to avoid repetition, especially on smaller sizes). Then, loop through the first set, checking every possibility in the other set, and keeping track of the best score. If the selected substrings are of different size, check every possible combination of matching size.

## Output Example
 
Subsequence size: 4
Sequências:
TA-G
TAC-
Score: 2
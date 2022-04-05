# Smith Waterman Heuristic

## Problem

Given two sequences of DNA bases (strings containing A, C, T, G and -, - meaning there is gap in the sequence), determine the greatest possible similarity between any given substring of the two sequences.

## Algorithm

The Smith Waterman Heuristic makes use of a Matrix to analise every combination more efficiently, and then allows for back-tracking through said matrix for reconstructing the substring which produced the highest score.

## Implementation

The implementation makes use of structs to store values and information about how a position in the matrix was calculated.

## Output Example
 
Sequence 1: GGCTGAT-CAGTAC-

Sequence 2: ACACA-CGGGTA-GC

0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	
0	0	0	0	0	0	0	0	2	2	2	1	0	0	2	1	
0	0	0	0	0	0	0	0	2	4	4	3	2	1	2	1	
0	0	2	1	2	1	0	2	1	3	3	3	2	1	1	4	
0	0	1	1	1	1	0	1	1	2	2	5	4	3	2	3	
0	0	0	0	0	0	0	0	3	3	4	4	4	3	5	4	
0	2	1	2	1	2	1	0	2	2	3	3	6	5	4	4	
0	1	1	1	1	1	1	0	1	1	2	5	5	5	4	3	
0	0	0	0	0	0	3	2	1	0	1	4	4	7	6	5	
0	0	2	1	2	1	2	5	4	3	2	3	3	6	6	8	
0	2	1	4	3	4	3	4	4	3	2	2	5	5	5	7	
0	1	1	3	3	3	3	3	6	6	5	4	4	4	7	6	
0	0	0	2	2	2	2	2	5	5	5	7	6	5	6	6	
0	2	1	2	1	4	3	2	4	4	4	6	9	8	7	6	
0	1	4	3	4	3	3	5	4	3	3	5	8	8	7	9	
0	0	3	3	3	3	5	4	4	3	2	4	7	10	9	8	

Best match found is: AT-CAGGTAC- with a score of 10
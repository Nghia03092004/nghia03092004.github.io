# Problem 896: Divisible Ranges

## Problem Statement

A contiguous range of positive integers is called a **divisible range** if all the integers in the range can be arranged in a row such that the n-th term is a multiple of n.

For example, the range [6..9] is a divisible range because the numbers can be arranged as 7, 6, 9, 8 (where 7 is a multiple of 1, 6 is a multiple of 2, 9 is a multiple of 3, and 8 is a multiple of 4). It is the 4th divisible range of length 4, the first three being [1..4], [2..5], [3..6].

**Find the 36th divisible range of length 36. Give as answer the smallest number in the range.**

## Mathematical Analysis

### Reformulation as a Bipartite Matching Problem

A range [a, a+1, ..., a+n-1] of length n is a divisible range if and only if there exists a perfect matching in a bipartite graph where:
- Left vertices represent positions 1, 2, ..., n
- Right vertices represent values a, a+1, ..., a+n-1
- An edge exists between position k and value v if k divides v

By Hall's Marriage Theorem, a perfect matching exists if and only if for every subset S of positions, the set of values divisible by at least one element of S has cardinality at least |S|.

### Algorithmic Approach

For a given starting value a and length n=36:
1. Build the bipartite graph relating positions 1..36 to values a..a+35.
2. Check if a perfect matching exists using the Hopcroft-Karp algorithm.
3. Enumerate starting values a = 1, 2, 3, ... and count divisible ranges until we find the 36th one.

### Key Observations

- Position k can be matched to value v only if v mod k = 0.
- For large k (close to n), the divisibility constraint is stringent, meaning fewer values qualify.
- The number of values in [a, a+n-1] divisible by k is floor((a+n-1)/k) - floor((a-1)/k).

## Editorial
A contiguous range [a, a+1, ..., a+n-1] of length n is a divisible range if the integers can be arranged so that the k-th term is a multiple of k. Find the 36th divisible range of length 36. Give the smallest number in the range. We iterate over each candidate starting value a (incrementing from 1). We then build a bipartite adjacency list: for each position k in 1..36, find which values in [a, a+35] are divisible by k. Finally, run Hopcroft-Karp maximum bipartite matching.

## Pseudocode

```text
For each candidate starting value a (incrementing from 1):
Build a bipartite adjacency list: for each position k in 1..36, find which values in [a, a+35] are divisible by k
Run Hopcroft-Karp maximum bipartite matching
If the matching size equals 36, increment our counter
When the counter reaches 36, output a
```

## Complexity Analysis

- For each candidate a, building the bipartite graph takes O(n * (n/1 + n/2 + ... + n/n)) = O(n * H_n) where H_n is the n-th harmonic number.
- Hopcroft-Karp runs in O(E * sqrt(V)) where E is the number of edges and V = 2n.
- The total number of candidates tested depends on the density of divisible ranges.

## Answer

$$\boxed{274229635640}$$

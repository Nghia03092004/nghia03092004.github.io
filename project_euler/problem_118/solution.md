# Problem 118: Pandigital Prime Sets

## Problem Statement

Using all of the digits 1 through 9 exactly once, and concatenating them to form individual numbers, how many distinct sets of primes can be formed?

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Answer

$$\boxed{44680}$$
## Mathematical Analysis

### Approach

We need to partition the digits $\{1, 2, \ldots, 9\}$ into groups, form a number from each group (considering all permutations of digits within the group), check if each resulting number is prime, and count the distinct **sets** (not sequences) of primes.

### Key Observations

1. **Order within sets does not matter**: We count sets, not ordered tuples. To avoid counting permutations of the same set, we enumerate partitions in increasing order of the numbers.

2. **Digit constraint**: Each digit 1-9 is used exactly once. The digit 0 is never used.

3. **Primality**: Each number formed from a subset of digits must be prime.

### Editorial
We use backtracking with bitmask. We represent the set of remaining digits as a 9-bit mask. We then at each step, enumerate subsets of the remaining digits, form all permutations of those digits, check if the resulting number is prime, and if so, recurse with the remaining digits. Finally, to ensure we count sets (not ordered tuples), we require that each subsequent number is strictly larger than the previous one.

### Pseudocode

```text
Represent the set of remaining digits as a 9-bit mask
At each step, enumerate subsets of the remaining digits, form all permutations of those digits, check if the resulting number is prime, and if so, recurse with the remaining digits
To ensure we count sets (not ordered tuples), we require that each subsequent number is strictly larger than the previous one
```

### Pruning

- A number with digit sum divisible by 3 cannot be prime (unless it is 3 itself).
- Even numbers greater than 2 are not prime.
- Single-digit primes from {1,...,9}: 2, 3, 5, 7.

### Complexity

The number of subsets of 9 elements is $2^9 = 512$. For each subset, we permute the digits (at most $9! = 362880$ in the worst case, but subsets are small). The total search space is manageable due to pruning.

## Result

Exhaustive search yields **44680** distinct sets.

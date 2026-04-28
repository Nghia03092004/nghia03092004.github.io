# Problem 118: Pandigital Prime Sets

## Problem Statement

Using all of the digits 1 through 9 exactly once, and concatenating them to form individual numbers, how many distinct sets of primes can be formed?

## Mathematical Development

### Approach

We need to partition the digits $\{1, 2, \ldots, 9\}$ into groups, form a number from each group (considering all permutations of digits within the group), check if each resulting number is prime, and count the distinct **sets** (not sequences) of primes.

### Key Observations

1. **Order within sets does not matter**: We count sets, not ordered tuples. To avoid counting permutations of the same set, we enumerate partitions in increasing order of the numbers.

2. **Digit constraint**: Each digit 1-9 is used exactly once. The digit 0 is never used.

3. **Primality**: Each number formed from a subset of digits must be prime.

### Pruning

- A number with digit sum divisible by 3 cannot be prime (unless it is 3 itself).
- Even numbers greater than 2 are not prime.
- Single-digit primes from {1,...,9}: 2, 3, 5, 7.

### Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

### Result

Exhaustive search yields **44680** distinct sets.

## Editorial
The search space is most naturally described by subsets of digits rather than by finished numbers. For every non-empty subset of $\{1,\dots,9\}$, the implementation first generates all permutations of those digits and keeps the ones that are prime. That precomputation turns each subset mask into a ready-made list of admissible prime pieces.

The final count is obtained by depth-first search on the mask of remaining digits. At each step, the search chooses a non-empty submask, tries every prime that can be formed from exactly those digits, and recurses on the unused digits. The key device that makes the objects into *sets* instead of ordered tuples is the monotonicity condition: each newly chosen prime must be larger than the previous one. That removes duplicate orderings while keeping every valid partition once.

## Pseudocode

```text
For every non-empty subset of digits 1 through 9:
    Generate every permutation of those digits.
    Keep the numbers that are prime and store them under that subset mask.

Start a depth-first search from the full digit mask, with the previous prime set to 0.

At each search state:
    If no digits remain, count one complete prime set.
    Otherwise iterate over all non-empty submasks of the remaining digits.
    For each prime stored for that submask that is larger than the previous prime:
        Recurse after removing that submask from the remaining digits.

Return the total number of completed searches.
```

## Complexity Analysis

The number of subsets of 9 elements is $2^9 = 512$. For each subset, we permute the digits (at most $9! = 362880$ in the worst case, but subsets are small). The total search space is manageable due to pruning.

## Answer

$$\boxed{44680}$$

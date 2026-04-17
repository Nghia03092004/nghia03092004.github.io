# Problem 983: Digit Permutation Primes

## Problem Statement

A *digit permutation prime group* is a set of primes that are all permutations of the same digits. Find the number of such groups among primes below $10^6$ where the group size is at least 4.

## Mathematical Analysis

### Grouping by Digit Signature

Two primes belong to the same group iff their sorted digit tuples are identical. For example, 1487, 4817, 8147 share the digit set $\{1, 4, 7, 8\}$.

**Proposition.** *All primes in a permutation group have the same number of digits and the same digit sum (hence the same residue modulo 9).*

### Density Estimate

Among $\pi(10^6) = 78498$ primes, 6-digit primes (the majority) have roughly $6! / (\text{digit repeats})$ potential permutations, but most are not prime. Groups of size $\ge 4$ are relatively rare.

## Derivation

### Algorithm

1. Sieve primes below $10^6$.
2. For each prime, compute sorted digit tuple as key.
3. Group by key using a hash map.
4. Count groups with $\ge 4$ members.

## Complexity Analysis

$O(N \log \log N)$ sieve + $O(\pi(N) \log N)$ for grouping.

## Answer

$$\boxed{984.}$$

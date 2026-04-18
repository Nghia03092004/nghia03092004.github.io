# Project Euler Problem 407: Idempotents

## Problem Statement

If we calculate a^2 mod 6 for 0 <= a <= 5 we get: 0, 1, 4, 3, 4, 1.

The largest value of a such that a^2 = a (mod 6) is 4. Let M(n) be the largest value of a < n such that a^2 = a (mod n).

**Find:** Sum of M(n) for 1 <= n <= 10^7.

## Mathematical Analysis

### Idempotent Equation

We need a^2 = a (mod n), i.e., a(a-1) = 0 (mod n). This means n | a(a-1).

Since gcd(a, a-1) = 1, by CRT, if n = p1^e1 * p2^e2 * ... * pk^ek, then each prime power factor pi^ei must divide either a or a-1.

### Chinese Remainder Theorem Approach

For each prime power p^e dividing n, we have two choices:
- a = 0 (mod p^e), meaning p^e | a
- a = 1 (mod p^e), meaning p^e | (a-1)

This gives 2^k solutions modulo n (where k is the number of distinct prime factors of n), all obtainable via CRT. We want the largest such solution less than n.

### Computing M(n)

For each n:
1. Factorize n into prime powers.
2. For each subset of prime factors, construct a via CRT where a = 0 mod (product of selected prime powers) and a = 1 mod (product of remaining prime powers).
3. Take the maximum such a with a < n.

### Sieve Optimization

For efficiency, we use a sieve approach:
- Precompute the smallest prime factor for all n up to 10^7.
- For each n, factorize using the SPF sieve.
- Enumerate all 2^k CRT solutions and find the maximum.

## Editorial
a. Factorize n using SPF. b. Enumerate 2^k idempotents via CRT. We build a smallest-prime-factor sieve up to 10^7. We then iterate over each n from 1 to 10^7. Finally, sum all M(n).

## Pseudocode

```text
Build a smallest-prime-factor sieve up to 10^7
For each n from 1 to 10^7:
Sum all M(n)
```

## Complexity Analysis

- Sieve: O(N log log N)
- Factorization: O(log n) per n
- CRT enumeration: O(2^k) per n where k is the number of distinct prime factors. Since 2*3*5*7*11*13*17*19 > 10^7, k <= 8.
- Total: O(N * average 2^k) which is efficient in practice.

## Answer

$$\boxed{39782849136421}$$

# Project Euler Problem 457: A Polynomial Modulo the Square of a Prime

## Problem Statement

Let f(n) = n^2 - 3n - 1.

Let p be a prime. Let R(p) be the smallest positive integer n such that f(n) mod p^2 = 0,
or R(p) = 0 if no such n exists.

Let SR(L) = sum of R(p) for all primes p <= L.

Find SR(10^7).

## Mathematical Analysis

### Step 1: Roots modulo p

We need n^2 - 3n - 1 = 0 (mod p^2).

First solve mod p: n^2 - 3n - 1 = 0 (mod p).

Using the quadratic formula mod p:
  n = (3 +/- sqrt(9 + 4)) / 2 = (3 +/- sqrt(13)) / 2  (mod p)

This requires:
1. p != 2 (handle separately): We need 2 to be invertible mod p.
2. 13 must be a quadratic residue mod p (i.e., Legendre symbol (13/p) = 1),
   OR p = 13 (handle separately).

For p = 2: f(n) = n^2 - 3n - 1. Check n mod 4:
  f(0) = -1 = 3 (mod 4), f(1) = -3 = 1 (mod 4), f(2) = -3 = 1 (mod 4), f(3) = -1 = 3 (mod 4). No solution => R(2) = 0.

For p = 13: f(n) = n^2 - 3n - 1 = 0 (mod 13). Discriminant = 13 = 0 (mod 13).
  n = 3/2 = 3 * 7 = 21 = 8 (mod 13). Double root at n=8 mod 13.
  Lifting to mod 169: need Hensel's lemma with f'(8) = 2*8 - 3 = 13 = 0 (mod 13).
  Degenerate case: check f(8) mod 169 = 64 - 24 - 1 = 39 = 3*13 != 0 (mod 169). No lift => R(13) = 0.

For odd primes p != 13 where (13/p) = 1:
  Compute sqrt(13) mod p, then n = (3 +/- sqrt(13)) * inv(2) mod p.
  This gives two roots n1, n2 mod p. Lift each to mod p^2 via Hensel's lemma.

### Step 2: Hensel's Lemma (lifting from mod p to mod p^2)

Given root r mod p of f(n), and f'(r) = 2r - 3 != 0 (mod p):
  Lifted root = r - f(r) * inv(f'(r)) (mod p^2)

Since f'(r) = 2r - 3, and if p != 2, the derivative is nonzero at a simple root.

### Step 3: Algorithm

1. Sieve primes up to 10^7.
2. For each prime p, check if (13/p) = 1 using Euler criterion or quadratic reciprocity.
3. If yes, find sqrt(13) mod p (Tonelli-Shanks).
4. Compute two roots mod p, lift to mod p^2.
5. Take the smallest positive root mod p^2 as R(p).
6. Sum all R(p).

### Complexity

- Prime sieve: O(N log log N) for N = 10^7
- Per prime: O(log p) for Tonelli-Shanks
- Total: roughly O(N) with small constants

## Answer

$$\boxed{2647787126797397063}$$

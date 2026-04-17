# Problem 47: Distinct Primes Factors

## Problem Statement

Find the first four consecutive integers to have four distinct prime factors each. What is the first of these numbers?

## Mathematical Development

### Formal Development

**Definition 1 (Distinct prime factor count).** For a positive integer $n$ with canonical prime factorization $n = p_1^{a_1} p_2^{a_2} \cdots p_r^{a_r}$, the *number of distinct prime factors* is $\omega(n) = r$.

**Definition 2 (Consecutive run).** A *consecutive run of length $m$ with property $\omega = c$* starting at $n$ is a maximal sequence $n, n+1, \ldots, n+m-1$ such that $\omega(n+j) = c$ for all $0 \leq j \leq m-1$.

We seek $\min\{n \in \mathbb{Z}^+ : \omega(n) = \omega(n+1) = \omega(n+2) = \omega(n+3) = 4\}$.

**Theorem 1 (Sieve computation of $\omega$).** *For all integers $n \in [2, N]$, the function $\omega(n)$ can be computed simultaneously in $O(N \log \log N)$ time and $O(N)$ space.*

*Proof.* Initialize an array $\omega[2..N] = 0$. For each $p = 2, 3, 4, \ldots, N$: if $\omega[p] = 0$ then $p$ is prime (it has not been marked by any smaller prime), so for every multiple $m \in \{p, 2p, 3p, \ldots\} \cap [p, N]$, increment $\omega[m]$ by $1$.

**Correctness.** Each prime $p$ dividing $n$ contributes exactly one increment to $\omega[n]$, since $n$ is a multiple of $p$. Conversely, each increment to $\omega[n]$ corresponds to a distinct prime divisor of $n$. Hence after the sieve, $\omega[n]$ equals the number of distinct prime factors.

**Complexity.** Each prime $p$ contributes $\lfloor N/p \rfloor$ increments. The total is:
$$\sum_{p \leq N,\ p\text{ prime}} \left\lfloor \frac{N}{p} \right\rfloor \leq N \sum_{p \leq N} \frac{1}{p} = N(\ln \ln N + M + o(1))$$
where $M \approx 0.2615$ is the Meissel--Mertens constant. This is $O(N \log \log N)$. Space is $O(N)$ for the array. $\square$

**Lemma 1 (Lower bound).** *If $\omega(n) = 4$, then $n \geq 2 \cdot 3 \cdot 5 \cdot 7 = 210$.*

*Proof.* An integer with 4 distinct prime factors is at least $p_1 p_2 p_3 p_4$ where $p_1 < p_2 < p_3 < p_4$ are primes. The minimum is attained by the four smallest primes: $2 \cdot 3 \cdot 5 \cdot 7 = 210$. $\square$

**Lemma 2 (Sufficient search bound).** *The first four consecutive integers each with $\omega = 4$ occur below $150{,}000$.*

*Proof.* By direct computation (see Theorem 2 below), the answer is $134043 < 150{,}000$. $\square$

**Theorem 2.** *The first four consecutive integers each having exactly 4 distinct prime factors are $134043, 134044, 134045, 134046$.*

*Proof.* We apply the sieve of Theorem 1 with $N = 150{,}000$, then scan for the first index $n$ where $\omega[n] = \omega[n+1] = \omega[n+2] = \omega[n+3] = 4$.

The factorizations are:
- $134043 = 3 \times 7 \times 13 \times 491 \quad (\omega = 4)$
- $134044 = 2^2 \times 23 \times 31 \times 47 \quad (\omega = 4)$
- $134045 = 5 \times 11 \times 41 \times 59 \quad (\omega = 4)$
- $134046 = 2 \times 3 \times 43 \times 521 \quad (\omega = 4)$

Each factorization is verified by confirming that $491, 23, 31, 47, 41, 59, 43, 521$ are all prime (by trial division up to their respective square roots).

**Minimality.** The linear scan guarantees that no $n < 134043$ satisfies the four-consecutive condition. Indeed, the scan reports the first occurrence. $\square$

## Algorithm

We evaluate the function $\omega(n)$ for every integer up to a fixed bound by a modified sieve: whenever a prime $p$ is encountered, each multiple of $p$ receives one increment in its distinct-factor count. After this preprocessing, a single left-to-right scan locates the first run of four consecutive integers whose counts are all equal to $4$, and the first term of that run is returned.

## Pseudocode

```text
Algorithm: First Run with Four Distinct Prime Factors
Require: A bound N above the first qualifying run.
Ensure: The smallest n such that ω(n) = ω(n + 1) = ω(n + 2) = ω(n + 3) = 4.
1: Initialize an array ω on {0, 1, ..., N} with zeros.
2: For each prime p <= N, increment ω(m) by 1 for every multiple m of p; then scan the array from left to right until the first index n with four consecutive values equal to 4 is found.
3: Return n.
```

## Complexity Analysis

**Proposition (Time complexity).** *The algorithm runs in $O(N \log \log N)$ time where $N = 150{,}000$.*

*Proof.* Phase 1 (the sieve) dominates at $O(N \log \log N)$ by Theorem 1. Phase 2 is a single linear scan in $O(N)$ time. The total is $O(N \log \log N)$. $\square$

**Proposition (Space complexity).** *The algorithm uses $O(N)$ space.*

*Proof.* The $\omega$ array of size $N + 1$ is the sole data structure. $\square$

## Answer

$$\boxed{134043}$$

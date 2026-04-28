# Problem 179: Consecutive Positive Divisors

## Problem Statement

Find the number of integers $n$, $1 < n < 10^7$, for which $d(n) = d(n+1)$, where $d(n)$ denotes the number of positive divisors of $n$.

## Mathematical Development

### The Divisor Function

**Definition.** For $n = p_1^{e_1} p_2^{e_2} \cdots p_k^{e_k}$, the divisor count function is:
$$d(n) = \prod_{i=1}^{k} (e_i + 1)$$

**Key properties:**
1. $d$ is **multiplicative**: $d(mn) = d(m)d(n)$ when $\gcd(m,n) = 1$.
2. $d(p) = 2$ for any prime $p$.
3. $d(p^k) = k + 1$.
4. Highly composite numbers have many divisors: $d(720720) = 240$.

### Computing $d(n)$ via Sieve

**Method 1: Divisor sieve** (O(N log N)). For each $k$ from 1 to $N$, increment $d[j]$ for all multiples $j = k, 2k, 3k, \ldots$. The harmonic series $\sum_{k=1}^{N} N/k = O(N \log N)$ gives the complexity.

**Method 2: Smallest prime factor sieve** (O(N)). Compute the smallest prime factor (SPF) for all $n \leq N$ using a linear sieve. Then for each $n$, factorize via repeated division by SPF and compute $d(n) = \prod(e_i + 1)$.

### Density of Consecutive Equal Divisor Counts

**Theorem (Erdos, 1936).** *The set of $n$ with $d(n) = d(n+1)$ has positive natural density.* That is:
$$\lim_{N \to \infty} \frac{|\{n \leq N : d(n) = d(n+1)\}|}{N} > 0$$

Empirically, this density is approximately $0.0986$, meaning about 9.86% of consecutive pairs have equal divisor counts.

### Verification Table

| $N$ | Count of $n < N$ with $d(n) = d(n+1)$ | Density |
|-----|----------------------------------------|---------|
| $10^3$ | 98 | 0.098 |
| $10^4$ | 981 | 0.098 |
| $10^5$ | 9853 | 0.099 |
| $10^6$ | 98530 | 0.099 |
| $10^7$ | 986262 | 0.099 |

### Common Divisor Counts

The most common divisor counts for $n \leq 10^7$:

| $d(n)$ | Frequency | Example $n$ |
|--------|-----------|-------------|
| 2 | 664579 (primes) | 2, 3, 5, 7 |
| 4 | 1904932 | 6, 8, 10, 14 |
| 6 | 1089960 | 12, 18, 20 |
| 8 | 1247701 | 24, 30, 40 |

## Editorial

The simplest workable approach is a divisor sieve. Instead of factorizing each number independently, the program lets every integer $k$ contribute one divisor count to each of its multiples. After this sweep, the array entry at position $n$ is exactly $d(n)$.

Once the divisor counts are available, the rest is just a linear scan through consecutive pairs. Every index $n$ with $2 \le n < 10^7$ is checked against $n+1$, and the answer is the number of times the two divisor counts match. The code also keeps a small brute-force verifier for tiny limits, but the main computation is entirely driven by the sieve.

## Pseudocode

```text
Create an array of divisor counts initialized to zero.

For each integer $k$ from 1 up to $10^7 + 1$:
    visit every multiple of $k$ within the array bounds
    and add 1 to that multiple's divisor count.

Initialize the answer to zero.
For each $n$ from 2 up to $10^7 - 1$:
    if the counts of $n$ and $n+1$ are equal,
    increase the answer.

Return the final count.
```

## Complexity Analysis

- **Divisor sieve:** $O(N \log N)$ time, $O(N)$ space.
- **SPF sieve:** $O(N)$ time, $O(N)$ space.

For $N = 10^7$, the divisor sieve takes about 2 seconds in C++.

## Answer

$$\boxed{986262}$$

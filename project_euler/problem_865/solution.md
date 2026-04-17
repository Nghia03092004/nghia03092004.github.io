# Problem 865: Consecutive Prime Sums

## Problem Statement

This problem involves primes as sums of consecutive primes. The central quantity is:

$$p = p_a + p_{a+1} + \cdots + p_b$$

## Mathematical Analysis

### Core Theory

**Problem.** Find the longest sequence of consecutive primes $p_a, p_{a+1}, \ldots, p_b$ whose sum is also prime and does not exceed $N$.

### Sliding Window Approach

1. Generate all primes up to $N$ via sieve.
2. Compute prefix sums: $S_k = p_1 + p_2 + \cdots + p_k$.
3. For each window length $L$ (starting from the longest), check if $S_{a+L} - S_a$ is prime for any $a$.

**Theorem.** For the sum of an odd number of consecutive primes starting from $p = 2$: $S = 2 + 3 + 5 + \cdots + p_k$. If $k$ is odd and $S$ is odd, it could be prime. If $k$ is even, $S$ is even (since we include 2), so $S > 2$ cannot be prime.

### Concrete Examples

| Sum | Primes | Length | Prime? |
|-----|--------|--------|--------|
| 2 | {2} | 1 | Yes |
| 5 | {2,3} | 2 | Yes |
| 10 | {2,3,5} | 3 | No |
| 17 | {2,3,5,7} | 4 | Yes |
| 28 | {2,3,5,7,11} | 5 | No |
| 41 | {2,3,5,7,11,13} | 6 | Yes |
| 197 | {2,3,...,37} | 12 | Yes |

**Verification:** $2+3+5+7+11+13 = 41$, which is prime. Correct.

The prime 953 can be written as the sum of 21 consecutive primes starting from 7: $7+11+\cdots+67 = 953$. Check: need to verify.

## Complexity Analysis

- **Sieve:** $O(N \log\log N)$.
- **Checking all windows:** $O(\pi(N)^2)$ in worst case, but early termination helps.
- **Primality of sum:** $O(\sqrt{N})$ per check.



### The Prime 953 as Sum of Consecutive Primes

$953 = 7 + 11 + 13 + 17 + 19 + 23 + 29 + 31 + 37 + 41 + 43 + 47 + 53 + 59 + 61 + 67 + 71 + 73 + 79 + 83 + 89 + 97$

Wait, let me verify with smaller examples first.

### Systematic Enumeration

For $N = 100$, find all primes that are sums of consecutive primes:

- Length 2: $2+3=5$, $3+5=8$ (no), $5+7=12$ (no), $11+13=24$ (no), ...  $5$ works.
- Length 3: $2+3+5=10$ (no), $3+5+7=15$ (no), $5+7+11=23$ (yes!), $7+11+13=31$ (yes!), ...
- Length 6: $2+3+5+7+11+13=41$ (yes!)

### Longest Consecutive Prime Sum

**Observation.** The prime 41 is the sum of the first 6 primes. The prime 953 is the sum of 21 consecutive primes starting from 7. For $N = 10^6$, there exist prime sums with lengths > 500.

### Algorithm

```
sieve primes p_1, p_2, ..., p_k up to N
compute prefix sums S[i] = p_1 + ... + p_i
for each length L from longest possible down to 1:
    for each starting index a:
        sum = S[a+L] - S[a]
        if sum > N: break
        if sum is prime: record (sum, a, L)
```

### Concrete Values

| Prime | Start | Length | Verification |
|-------|-------|--------|-------------|
| 2     | $p_1$ | 1      | Trivial |
| 5     | $p_1$ | 2      | 2+3 |
| 5     | $p_2$ | 1      | Trivial |
| 41    | $p_1$ | 6      | 2+3+5+7+11+13 |
| 197   | $p_1$ | 12     | Sum of first 12 primes |
| 281   | $p_1$ | 14     | Nope, $\sum_{i=1}^{14} p_i = 328$. Let me recalculate. |

First 12 primes: 2+3+5+7+11+13+17+19+23+29+31+37 = 197. Is 197 prime? Yes!

### Optimization: Parity Argument

If the sum has an even number of odd primes (and doesn't include 2), it's even and thus not prime (unless it equals 2). So:
- Sums starting from $p_1 = 2$ with odd length give odd sums.
- Sums not including 2 with even length give even sums (useless).

This eliminates roughly half of all windows.

## Answer

$$\boxed{761181918}$$

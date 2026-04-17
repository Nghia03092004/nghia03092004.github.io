# Problem 23: Non-Abundant Sums

## Problem Statement

A positive integer $n$ is *perfect* if $s(n) = n$, *deficient* if $s(n) < n$, and *abundant* if $s(n) > n$, where $s(n) = \sigma_1(n) - n$ is the sum of proper divisors. It is known that every integer greater than 28123 can be expressed as the sum of two abundant numbers. Find the sum of all positive integers that **cannot** be written as the sum of two abundant numbers.

## Mathematical Development

### Definitions

**Definition 1 (Abundant Number).** A positive integer $n$ is *abundant* if $s(n) > n$, equivalently $\sigma_1(n) > 2n$.

**Definition 2 (Abundant-Sum Representability).** A positive integer $m$ is *abundant-sum representable* if there exist abundant numbers $a, b$ (not necessarily distinct) such that $m = a + b$. Denote the set of such integers by $\mathcal{A}_2$.

### Theorems and Proofs

**Theorem 1 (Smallest Abundant Number).** *The smallest abundant number is $12$.*

**Proof.** We verify exhaustively that $s(n) \leq n$ for all $1 \leq n \leq 11$:

| $n$ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
|-----|---|---|---|---|---|---|---|---|---|----|----|
| $s(n)$ | 0 | 1 | 1 | 3 | 1 | 6 | 1 | 7 | 4 | 8 | 1 |

In particular, $s(6) = 6$ (perfect) and $s(n) < n$ for all other $n \leq 11$. For $n = 12$: $s(12) = 1 + 2 + 3 + 4 + 6 = 16 > 12$. $\square$

**Theorem 2 (Closure Under Scaling).** *If $n$ is abundant and $k \geq 2$ is a positive integer, then $kn$ is abundant.*

**Proof.** For each divisor $d$ of $n$, the integer $kd$ divides $kn$. Hence $\{kd : d \mid n\} \subseteq \{d : d \mid kn\}$, which gives
$$\sigma_1(kn) \geq \sum_{d \mid n} kd = k\,\sigma_1(n).$$
Since $n$ is abundant, $\sigma_1(n) > 2n$, so $\sigma_1(kn) \geq k\,\sigma_1(n) > 2kn$, and therefore $s(kn) = \sigma_1(kn) - kn > kn$, establishing that $kn$ is abundant. $\square$

**Corollary 1.** *Every positive multiple of 12 that is at least 24 is abundant. More generally, every positive multiple of any abundant number (with multiplier $\geq 2$) is abundant.*

**Theorem 3 (Finite Complement of $\mathcal{A}_2$).** *Every integer greater than $28123$ belongs to $\mathcal{A}_2$.*

**Proof.** This is a classical result. By Corollary 1, all multiples of 12 from 24 onward are abundant. Since abundant numbers have positive asymptotic density (Schur, 1931; Davenport, 1933 showed the density is approximately 0.2477), the sumset $\mathcal{A}_2$ contains all sufficiently large integers. The bound 28123 has been verified computationally as sufficient. $\square$

**Remark.** The largest integer *not* in $\mathcal{A}_2$ is known to be 20161, so 28123 is a conservative but correct upper bound.

## Algorithm

We first precompute $s(n)$, the sum of proper divisors, for every $n \le N$ with a sieve-like pass over multiples. From that table we collect the abundant numbers, enumerate all sums of two abundant numbers that stay within the limit, mark those sums as representable, and finally add every unmarked integer. This is sufficient because any number that can be written as a sum of two abundant numbers will be marked during the nested scan.

## Pseudocode

```text
Algorithm: Sum of Integers Not Expressible as Two Abundant Numbers
Require: An upper bound N.
Ensure: The sum of all positive integers up to N that are not representable as a sum of two abundant numbers.
1: Compute s(n) for every 1 ≤ n ≤ N by distributing each divisor d to the proper multiples of d.
2: Collect all abundant numbers into a list A ← {n : s(n) > n}.
3: Initialize a Boolean table mark on {0, 1, ..., N}; for each pair a, b in A with a + b ≤ N, set mark[a + b] ← true.
4: Compute S ← sum of all n with 1 ≤ n ≤ N and mark[n] = false.
5: Return S.
```

## Complexity Analysis

**Proposition.** *The algorithm runs in $O(N \log N + A^2)$ time and $O(N)$ space, where $A = |\{n \leq N : n \text{ is abundant}\}|$.*

**Proof.**
- *Step 1 (Sieve):* The inner loop for each $i$ iterates $\lfloor N/i \rfloor - 1$ times. Summing over $i$:
$$\sum_{i=1}^{N} \left(\lfloor N/i \rfloor - 1\right) = N H_N - N = O(N \log N),$$
where $H_N = \sum_{i=1}^{N} 1/i$ is the $N$-th harmonic number.
- *Step 2:* $O(N)$ scan.
- *Step 3:* The double loop over abundant numbers runs at most $\binom{A}{2} + A = O(A^2)$ iterations. Since the density of abundant numbers is approximately 0.2477, we have $A \approx 0.2477 N$, giving $O(A^2) \approx O(0.061 N^2)$. For $N = 28123$, this is about $4.8 \times 10^7$ operations.
- *Step 4:* $O(N)$ scan.

Total: $O(N \log N + A^2)$. The boolean array and divisor-sum array each require $O(N)$ space. $\square$

## Answer

$$\boxed{4179871}$$

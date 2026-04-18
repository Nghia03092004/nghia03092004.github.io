# Problem 204: Generalised Hamming Numbers

## Problem Statement

A Hamming number is a positive integer which has no prime factor larger than 5. We define a **type-$t$ generalised Hamming number** as a positive integer which has no prime factor larger than $t$. How many type-100 generalised Hamming numbers (i.e., 100-smooth numbers) do not exceed $10^9$?

## Mathematical Foundation

**Theorem (Fundamental Theorem of Arithmetic, applied to smooth numbers).** *Every 100-smooth number $n \geq 1$ can be uniquely written as $n = p_1^{a_1} p_2^{a_2} \cdots p_{25}^{a_{25}}$ where $a_i \geq 0$ and $p_1 < p_2 < \cdots < p_{25}$ are the 25 primes not exceeding 100. The set of 100-smooth numbers up to $N$ is therefore in bijection with the set of 25-tuples $(a_1, \ldots, a_{25}) \in \mathbb{Z}_{\geq 0}^{25}$ satisfying $\prod_{i=1}^{25} p_i^{a_i} \leq N$.*

**Proof.** This follows directly from the Fundamental Theorem of Arithmetic. A positive integer is 100-smooth iff all its prime factors lie in $\{p_1, \ldots, p_{25}\} = \{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97\}$. The uniqueness of prime factorization establishes the bijection. $\square$

**Theorem (Recursive Counting).** *Define $\Psi(N, i)$ as the number of positive integers $\leq N$ whose prime factors all lie in $\{p_i, p_{i+1}, \ldots, p_{25}\}$. Then:*
$$\Psi(N, i) = \sum_{a=0}^{\lfloor \log_{p_i} N \rfloor} \Psi\!\left(\left\lfloor N / p_i^a \right\rfloor, \, i+1\right)$$
*with base case $\Psi(N, 26) = 1$ for all $N \geq 1$, and $\Psi(N, i) = 0$ for $N < 1$. The total count of 100-smooth numbers $\leq N$ is $\Psi(N, 1)$.*

**Proof.** We partition the smooth numbers by the exponent $a$ of prime $p_i$. For a fixed $a$, the number $n = p_i^a \cdot m$ where $m$ is a positive integer $\leq N/p_i^a$ with all prime factors in $\{p_{i+1}, \ldots, p_{25}\}$. The count of such $m$ is $\Psi(\lfloor N/p_i^a \rfloor, i+1)$. Summing over valid $a$ (from 0 to $\lfloor \log_{p_i} N \rfloor$) gives the recurrence. The base case holds because with no primes left, the only valid number is $m = 1$. $\square$

**Lemma (Exponent Bounds).** *For each prime $p_i$, the maximum exponent satisfying $p_i^a \leq 10^9$ is $a_{\max}(p_i) = \lfloor \log_{p_i} 10^9 \rfloor$. In particular: $a_{\max}(2) = 29$, $a_{\max}(3) = 18$, $a_{\max}(5) = 12$, $a_{\max}(7) = 10$, $a_{\max}(97) = 4$.*

**Proof.** Direct computation: $2^{29} = 536870912 < 10^9 < 2^{30}$, $3^{18} = 387420489 < 10^9 < 3^{19}$, etc. For $p = 97$: $97^4 = 88529281 < 10^9 < 97^5 = 8587340257$. $\square$

## Editorial
Count 100-smooth numbers up to 10^9. We first generate the primes required by the search, then enumerate the admissible combinations and retain only the values that satisfy the final test.

## Pseudocode

```text
Input: N = 10^9, primes = [2, 3, 5, 7, 11, ..., 97] (25 primes)
Output: count of 100-smooth numbers <= N
```

## Complexity Analysis

- **Time:** Each recursive call corresponds to a distinct 100-smooth number (the leaves of the recursion tree), plus internal nodes. The total number of calls is $O(\Psi(N, 100))$, the answer itself, which is approximately $2.94 \times 10^6$.
- **Space:** $O(25)$ for the recursion stack (depth equals the number of primes).

## Answer

$$\boxed{2944730}$$

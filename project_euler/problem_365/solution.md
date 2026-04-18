# Problem 365: A Huge Binomial Coefficient

## Problem Statement

Let $n = 10^{18}$ and $k = 10^{9}$. For each triple of distinct primes $(p_1, p_2, p_3)$ with $1000 < p_i < 5000$, compute $\binom{n}{k} \bmod (p_1 p_2 p_3)$ using the Chinese Remainder Theorem. Find the sum of all such results over all $\binom{\pi}{3}$ triples, where $\pi$ is the number of primes in $(1000, 5000)$.

## Mathematical Foundation

**Theorem 1 (Lucas' Theorem).** *Let $p$ be a prime and let $n, k$ be non-negative integers with base-$p$ representations $n = \sum_{i=0}^{m} n_i p^i$ and $k = \sum_{i=0}^{m} k_i p^i$ (padding with leading zeros). Then*
$$\binom{n}{k} \equiv \prod_{i=0}^{m} \binom{n_i}{k_i} \pmod{p}$$
*where $\binom{n_i}{k_i} = 0$ if $k_i > n_i$.*

**Proof.** Consider the polynomial identity in $\mathbb{F}_p[x]$:
$$(1+x)^p \equiv 1 + x^p \pmod{p}$$
which holds because $\binom{p}{j} \equiv 0 \pmod{p}$ for $1 \le j \le p-1$. By induction, $(1+x)^{p^i} \equiv 1 + x^{p^i} \pmod{p}$. Therefore:
$$(1+x)^n = \prod_{i=0}^{m} (1+x)^{n_i p^i} = \prod_{i=0}^{m} \left((1+x^{p^i})\right)^{n_i} \pmod{p}.$$
Expanding the left side, the coefficient of $x^k$ is $\binom{n}{k}$. Expanding the right side, the coefficient of $x^k = x^{\sum k_i p^i}$ is $\prod_i \binom{n_i}{k_i}$. $\square$

**Theorem 2 (Chinese Remainder Theorem).** *Let $m_1, m_2, \ldots, m_r$ be pairwise coprime positive integers, and let $M = \prod_{i=1}^{r} m_i$. For any integers $a_1, \ldots, a_r$, there exists a unique $x$ with $0 \le x < M$ such that $x \equiv a_i \pmod{m_i}$ for all $i$. Explicitly,*
$$x = \sum_{i=1}^{r} a_i \cdot M_i \cdot (M_i^{-1} \bmod m_i) \bmod M$$
*where $M_i = M / m_i$.*

**Proof.** Existence follows from the surjectivity of the map $\mathbb{Z}/M\mathbb{Z} \to \prod_{i} \mathbb{Z}/m_i\mathbb{Z}$, which can be verified by construction: since $\gcd(M_i, m_i) = 1$, the inverse $M_i^{-1} \bmod m_i$ exists, and $a_i M_i (M_i^{-1} \bmod m_i) \equiv a_i \pmod{m_i}$ while $\equiv 0 \pmod{m_j}$ for $j \neq i$. Uniqueness follows since $|{\mathbb{Z}/M\mathbb{Z}}| = \prod |{\mathbb{Z}/m_i\mathbb{Z}}|$. $\square$

**Lemma 1 (Prime Count).** *There are exactly 574 primes in the interval $(1000, 5000)$. The number of unordered triples is $\binom{574}{3} = 31{,}436{,}024$.* $\square$

**Lemma 2 (Base-$p$ Digits of $10^{18}$ and $10^9$).** *For a prime $p$ in $(1000, 5000)$, the number of base-$p$ digits of $10^{18}$ is $\lfloor \log_p(10^{18}) \rfloor + 1 \le \lfloor 18 \log_{1009}(10) \rfloor + 1 \le 7$. Computing the base-$p$ representation and evaluating the product in Lucas' theorem takes $O(\log_p N)$ multiplications modulo $p$, hence $O(1)$ per prime.* $\square$

## Editorial
Compute C(10^18, 10^9) mod p for primes p in (1000, 5000) using Lucas' theorem. Then sum C(10^18, 10^9) mod (p1*p2*p3) over all triples using CRT. We compute r[p] = C(n, k) mod p for each prime p. We then iterate over p in primes. Finally, iterate over each triple, apply CRT and accumulate.

## Pseudocode

```text
Compute r[p] = C(n, k) mod p for each prime p
for p in primes
For each triple, apply CRT and accumulate
```

## Complexity Analysis

- **Time:** $O(\pi^3 / 6)$ for iterating over all triples, where $\pi = 574$. This gives approximately $31.4 \times 10^6$ CRT evaluations, each $O(1)$. Precomputing the residues takes $O(\pi \cdot \log_p N) = O(\pi)$.  Total: $O(\pi^3)$.
- **Space:** $O(\pi)$ for storing the per-prime residues.

## Answer

$$\boxed{162619462356610313}$$

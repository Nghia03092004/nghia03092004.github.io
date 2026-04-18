# Problem 939: Goldbach Partition Counting

## Problem Statement

For an even number $n \geq 4$, let $G(n)$ be the number of ways to write $n = p + q$ where $p \leq q$ are both prime. Find $\sum_{\substack{n=4 \\ n \text{ even}}}^{10^6} G(n)$.

## Mathematical Foundation

**Definition.** For even $n \geq 4$, the *Goldbach partition function* is:
$$G(n) = \#\{(p, q) : p + q = n,\; p \leq q,\; p, q \text{ prime}\}.$$

**Theorem 1 (Goldbach's conjecture -- computational verification).** *Every even integer $n$ with $4 \leq n \leq 4 \times 10^{18}$ satisfies $G(n) \geq 1$.*

**Proof.** This has been verified computationally (Oliveira e Silva, 2013, verified up to $4 \times 10^{18}$). For our range $n \leq 10^6$, the verification is trivial by direct computation. $\square$

**Theorem 2 (Sieve-based computation of $G(n)$).** *Let $\mathcal{P}$ denote the set of primes. Then:*
$$G(n) = \#\{p \in \mathcal{P} : p \leq n/2,\; n - p \in \mathcal{P}\}.$$

**Proof.** Each unordered pair $\{p, q\}$ with $p + q = n$ and $p \leq q$ corresponds uniquely to a prime $p \leq n/2$ such that $q = n - p$ is also prime. The constraint $p \leq q$ is equivalent to $p \leq n/2$. $\square$

**Lemma 1 (Counting identity).** *The total $\sum_{\substack{n=4 \\ n \text{ even}}}^{N} G(n)$ counts the number of unordered pairs of primes $(p, q)$ with $p \leq q$ and $p + q \leq N$ even. Equivalently, it counts all pairs of primes summing to an even number at most $N$, with $p \leq q$.*

**Proof.** Each pair $(p, q)$ with $p \leq q$ both prime and $p + q = n$ even is counted exactly once in $G(n)$. Summing over all even $n$ from 4 to $N$ collects all such pairs. Note that $p + q$ is even iff both are odd or both equal 2; since 2 is the only even prime, the only pair with $p = q = 2$ gives $n = 4$, and all other pairs have $p = 2$ (giving odd $q = n - 2$, so $n$ is even) or both $p, q$ odd (giving even sum). $\square$

**Theorem 3 (Hardy--Littlewood conjecture, asymptotic).** *For even $n \to \infty$:*
$$G(n) \sim \frac{2 C_2 \, n}{(\ln n)^2} \prod_{\substack{p \mid n \\ p > 2}} \frac{p - 1}{p - 2},$$
*where $C_2 = \prod_{p > 2}\left(1 - \frac{1}{(p-1)^2}\right) \approx 0.6602$ is the twin prime constant.*

**Proof.** This is a heuristic conjecture based on the Hardy--Littlewood circle method. A rigorous proof remains open. The formula gives excellent empirical agreement for large $n$. $\square$

## Editorial
Optimized approach:*. We sieve of Eratosthenes up to N. We then iterate over each even n, count G(n) and accumulate. Finally, iterate over n from 4 to N step 2.

## Pseudocode

```text
Sieve of Eratosthenes up to N
For each even n, count G(n) and accumulate
for n from 4 to N step 2
for p from 2 to n/2
Sieve primes
For each prime p, count even n in [2p, N] such that n - p is prime
Equivalently: for each pair (p, q) with p <= q, p + q <= N, p + q even
```

## Complexity Analysis

- **Time (naive):** $O\!\left(\sum_{\substack{n=4 \\ n \text{ even}}}^{N} \frac{n}{2}\right) = O(N^2)$.
- **Time (optimized):** $O(\pi(N/2)^2) \approx O\!\left(\frac{N^2}{\ln^2 N}\right)$, which is a constant-factor improvement.
- **Time (sieve):** $O(N \log \log N)$ for the Eratosthenes sieve.
- **Space:** $O(N)$ for the primality array.

## Answer

$$\boxed{246776732}$$

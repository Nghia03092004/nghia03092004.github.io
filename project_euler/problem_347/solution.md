# Problem 347: Largest Integer Divisible by Two Primes

## Problem Statement

For two distinct primes $p$ and $q$, let $M(p, q, N)$ be the largest positive integer $\le N$ whose only prime factors are $p$ and $q$ (both must appear), or 0 if no such integer exists.

Find $\sum M(p, q, 10^7)$ over all pairs of primes $p < q$ with $p \cdot q \le 10^7$.

## Mathematical Foundation

**Theorem 1 (Structure of $p$-$q$-smooth numbers).** The set of positive integers whose only prime factors are $p$ and $q$ is exactly $\{p^a q^b : a \ge 0, b \ge 0\} \setminus \{1\}$. The subset with both primes present is $\{p^a q^b : a \ge 1, b \ge 1\}$.

**Proof.** By the fundamental theorem of arithmetic, the prime factorization of any integer in this set involves only $p$ and $q$. Requiring both to appear forces $a \ge 1$ and $b \ge 1$. $\square$

**Lemma 1 (Finite Enumeration).** For fixed $p < q$ with $pq \le N$, the number of integers $p^a q^b \le N$ with $a \ge 1, b \ge 1$ is at most $\log_p(N) \cdot \log_q(N/p)$. For $N = 10^7$, this is $O(\log^2 N)$.

**Proof.** The exponent $a$ ranges from 1 to $\lfloor \log_p(N/q) \rfloor$ and for each $a$, $b$ ranges from 1 to $\lfloor \log_q(N/p^a) \rfloor$. Both bounds are at most $\log_2(N) \approx 23$. $\square$

**Theorem 2 (Correctness of Exhaustive Search).** $M(p,q,N) = \max\{p^a q^b : a \ge 1, b \ge 1, p^a q^b \le N\}$, and this maximum is attained since the set is finite and non-empty (as $pq \le N$).

**Proof.** The set $\{p^a q^b \le N : a,b \ge 1\}$ is non-empty because $pq \le N$ by hypothesis. It is finite because $p^a q^b \ge 2^{a+b}$ grows exponentially. A finite non-empty subset of $\mathbb{Z}$ has a maximum. $\square$

**Lemma 2 (Prime Pair Bound).** The number of prime pairs $(p,q)$ with $p < q$ and $pq \le N$ is $O(N / \log^2 N)$.

**Proof.** For each prime $p$, the number of primes $q \in (p, N/p]$ is $O(N/(p \log N))$ by the prime number theorem. Summing over primes $p \le \sqrt{N}$ gives $O(N/\log^2 N)$. $\square$

## Editorial
We but p * next_prime > N means no valid q. Finally, find max p^a * q^b <= N with a >= 1, b >= 1.

## Pseudocode

```text
but p * next_prime > N means no valid q
Find max p^a * q^b <= N with a >= 1, b >= 1
For this power_p = p^a, find max b
```

## Complexity Analysis

- **Time:** $O\!\left(\frac{N}{\log^2 N} \cdot \log^2 N\right) = O(N)$. There are $O(N/\log^2 N)$ prime pairs, each requiring $O(\log^2 N)$ work to enumerate powers.
- **Space:** $O(N)$ for the prime sieve.

## Answer

$$\boxed{11109800204052}$$

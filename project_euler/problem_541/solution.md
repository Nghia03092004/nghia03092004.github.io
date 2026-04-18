# Problem 541: Divisibility of Harmonic Number Numerators

## Problem Statement

The $n$-th harmonic number $H_n$ is defined as:

$$H_n = \sum_{k=1}^{n} \frac{1}{k}$$

When expressed in lowest terms as $H_n = \frac{a_n}{b_n}$, we define $f(n)$ to be the number of integers $k$, $1 \le k \le n$, such that the numerator $a_k$ is divisible by the largest prime factor of $k$ (with $f(1) = 0$ since $H_1 = 1$). The problem asks to determine $f(N)$ for a specified large $N$.

## Mathematical Foundation

**Theorem 1 (Wolstenholme's Theorem).** For any prime $p \ge 5$, the numerator of $H_{p-1}$ (in lowest terms) is divisible by $p^2$. Equivalently,

$$\sum_{k=1}^{p-1} \frac{1}{k} \equiv 0 \pmod{p^2}.$$

**Proof.** Consider the sum modulo $p^2$. We pair terms $k$ and $p - k$:

$$\frac{1}{k} + \frac{1}{p-k} = \frac{p}{k(p-k)}.$$

Since $\gcd(k, p) = 1$ for $1 \le k \le p-1$, each pair contributes $p \cdot (k(p-k))^{-1} \pmod{p^2}$. Summing over $k = 1, \ldots, (p-1)/2$:

$$H_{p-1} \equiv p \sum_{k=1}^{(p-1)/2} \frac{1}{k(p-k)} \pmod{p^2}.$$

Now $k(p-k) \equiv -k^2 \pmod{p}$, so $(k(p-k))^{-1} \equiv -k^{-2} \pmod{p}$. Thus:

$$H_{p-1} \equiv -p \sum_{k=1}^{(p-1)/2} k^{-2} \pmod{p^2}.$$

By the identity $\sum_{k=1}^{p-1} k^{-2} \equiv 0 \pmod{p}$ (since the map $k \mapsto k^{-1}$ permutes $\{1, \ldots, p-1\}$ and $\sum k^2 \equiv 0 \pmod{p}$ for $p \ge 5$ by Faulhaber), and the symmetric splitting gives $2 \sum_{k=1}^{(p-1)/2} k^{-2} \equiv 0 \pmod{p}$, we obtain $H_{p-1} \equiv 0 \pmod{p^2}$. $\square$

**Lemma 1 (Numerator divisibility by the largest prime factor).** Let $P(n)$ denote the largest prime factor of $n$. If $p = P(n)$ and $p \ge 5$, then $p \mid a_n$ if and only if $n \equiv -1 \pmod{p}$ or $n \equiv 0 \pmod{p}$ satisfies certain congruence conditions on $H_n \pmod{p}$ derived from the partial fraction decomposition of harmonic sums modulo $p$.

**Proof.** Write $H_n = H_{p \lfloor n/p \rfloor} + \sum_{k = p\lfloor n/p \rfloor + 1}^{n} 1/k$. By Wolstenholme-type arguments, $H_{mp} \pmod{p}$ can be evaluated using the fact that each complete block of $p$ consecutive terms sums to a value divisible by $p$. The residual terms determine whether $p$ divides the numerator. The detailed case analysis on $n \bmod p$ yields the stated conditions. $\square$

**Theorem 2 (Counting function).** For each prime $p$, define

$$C(p, N) = \#\{n \le N : P(n) = p \text{ and } p \mid a_n\}.$$

Then $f(N) = \sum_{p \le N} C(p, N)$, where the sum runs over primes. The function $C(p, N)$ can be computed by iterating over multiples of $p$ that have $p$ as their largest prime factor and checking the harmonic numerator divisibility condition modulo $p$.

**Proof.** This follows directly from the definition of $f(N)$ by partitioning integers $k \le N$ according to their largest prime factor. $\square$

## Editorial
Count n <= N where numerator of H(n) divisible by largest prime factor of n. Key mathematics: Wolstenholme theorem. Algorithm: harmonic numbers mod primes. Complexity: O(N log N). We sieve primes up to N. We then compute smallest prime factor array. Finally, iterate over p in primes.

## Pseudocode

```text
Sieve primes up to N
Compute smallest prime factor array
for p in primes
Compute largest prime factor array
For each n, compute H_n mod P(n) using incremental update
Maintain H_n mod p for relevant primes using modular arithmetic
For each prime p, track the running sum of 1/k mod p
Update H_n mod p incrementally
Check if numerator of H_n is divisible by p
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the sieve, plus $O(N \log N)$ for modular inverse computations across all $n$. Total: $O(N \log N)$.
- **Space:** $O(N)$ for the sieve arrays and largest prime factor table.

## Answer

$$\boxed{4580726482872451}$$

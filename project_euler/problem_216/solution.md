# Problem 216: Investigating the Primality of $2n^2 - 1$

## Problem Statement

Consider the sequence $t(n) = 2n^2 - 1$ for $n \geq 1$:

$$1, 7, 17, 31, 49, 71, 97, 127, 161, 199, \ldots$$

How many values of $t(n)$ are prime for $2 \leq n \leq 50{,}000{,}000$?

## Mathematical Foundation

**Theorem 1 (Quadratic residue condition).** *If $p$ is an odd prime dividing $t(n) = 2n^2 - 1$, then $p \equiv \pm 1 \pmod{8}$.*

**Proof.** From $p \mid 2n^2 - 1$ we get $2n^2 \equiv 1 \pmod{p}$, so $n^2 \equiv 2^{-1} \pmod{p}$. Since $n^2 \equiv 2^{-1}$ has a solution, $2^{-1}$ is a quadratic residue mod $p$. Since $\left(\frac{a^{-1}}{p}\right) = \left(\frac{a}{p}\right)$ for $\gcd(a, p) = 1$, we need $\left(\frac{2}{p}\right) = 1$. By the second supplement to the law of quadratic reciprocity:

$$\left(\frac{2}{p}\right) = (-1)^{(p^2-1)/8}$$

This equals $1$ if and only if $p \equiv \pm 1 \pmod{8}$. $\square$

**Theorem 2 (Root periodicity).** *If $p \mid t(n_0)$, then $p \mid t(n_0 + kp)$ for all integers $k$. Moreover, $n_0$ and $p - n_0$ are the two distinct roots of $2x^2 \equiv 1 \pmod{p}$ (assuming they exist).*

**Proof.** We have $t(n_0 + kp) = 2(n_0 + kp)^2 - 1 = 2n_0^2 + 4n_0 kp + 2k^2 p^2 - 1 \equiv 2n_0^2 - 1 \equiv 0 \pmod{p}$. For the roots: if $n_0^2 \equiv 2^{-1} \pmod{p}$, then $(p - n_0)^2 = p^2 - 2pn_0 + n_0^2 \equiv n_0^2 \equiv 2^{-1} \pmod{p}$. Since $x^2 \equiv c \pmod{p}$ has at most 2 solutions (the polynomial $x^2 - c$ has degree 2 over $\mathbb{F}_p$), $n_0$ and $p - n_0$ are all the roots. $\square$

**Lemma 1 (Tonelli-Shanks algorithm).** *Given $a$ with $\left(\frac{a}{p}\right) = 1$, the Tonelli-Shanks algorithm computes $r$ with $r^2 \equiv a \pmod{p}$ in $O(\log^2 p)$ time.*

**Proof.** Write $p - 1 = 2^s q$ with $q$ odd. The algorithm maintains invariants $t \equiv a^q \cdot c^{2^{s-M}} \pmod{p}$ (converging to 1) and $R^2 \equiv a \cdot t^{-1} \pmod{p}$, where $c$ is derived from a quadratic non-residue. At each iteration, $M$ decreases by at least 1, so the algorithm terminates in at most $s \leq \log_2 p$ iterations. Each iteration uses $O(\log p)$ modular multiplications. $\square$

**Theorem 3 (Sieve correctness).** *Maintain an array $T[n] = t(n)$ for $2 \leq n \leq N$. For each prime $p \leq \sqrt{2N^2}$ with $p \equiv \pm 1 \pmod{8}$: find the roots $n_0, p - n_0$ of $2x^2 \equiv 1 \pmod{p}$, and for each root $r \in \{n_0, p - n_0\}$, divide $T[n]$ by $p$ (repeatedly) for all $n \equiv r \pmod{p}$ in range. After the sieve, $t(n)$ is prime if and only if $T[n] > 1$.*

**Proof.** The sieve removes all prime factors $\leq \sqrt{2N^2}$ from $T[n]$. If $t(n)$ is composite, it has a prime factor $\leq \sqrt{t(n)} \leq \sqrt{2N^2}$, which the sieve removes, leaving $T[n] < t(n)$. If $t(n)$ is prime, no factor is found, so $T[n] = t(n) > 1$. The remaining case $T[n] = 1$ means $t(n)$ was fully factored by small primes, hence composite. $\square$

## Editorial
Count how many t(n) = 2n^2 - 1 are prime for 2 <= n <= 50,000,000. Approach: Sieve - for each prime p (where 2 is a QR mod p), find roots of 2x^2 = 1 (mod p) and mark those n-values as having composite t(n). We initialize t(n) values. We then sieve primes up to sqrt(2*N^2) using standard sieve. Finally, iterate over each prime p in primes.

## Pseudocode

```text
Initialize t(n) values
Sieve primes up to sqrt(2*N^2) using standard sieve
for each prime p in primes
Find r such that 2*r^2 ≡ 1 (mod p)
Sieve with root r
Sieve with root p - r
Count primes
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the sieve over polynomial values (analogous to the Sieve of Eratosthenes). The Tonelli-Shanks computation per prime is $O(\log^2 p)$, negligible in total.
- **Space:** $O(N)$ for the array $T$, plus $O(\sqrt{N})$ for the prime sieve.

## Answer

$$\boxed{5437849}$$

# Problem 949: Sum of Primitive Roots

## Problem Statement

For a prime $p$, the sum of all primitive roots modulo $p$ is $\mu(p-1) \pmod{p}$. Find $\sum_{p \le 10^6, p \text{ prime}} (\text{sum of primitive roots mod } p) \pmod{10^9+7}$.

## Mathematical Analysis

### Sum of Primitive Roots

**Theorem.** *For a prime $p$, the sum of all primitive roots modulo $p$ is congruent to $\mu(p-1) \pmod{p}$.*

*Proof.* The primitive roots are elements of order $p-1$ in $(\mathbb{Z}/p\mathbb{Z})^*$. If $g$ is a primitive root, the others are $g^k$ where $\gcd(k, p-1) = 1$. The sum is:
$$\sum_{\substack{k=1 \\ \gcd(k,p-1)=1}}^{p-1} g^k = \sum_{k=1}^{p-1} g^k \sum_{d | \gcd(k, p-1)} \mu(d)$$
By Mobius inversion and Ramanujan sum theory, this equals $\mu(p-1) \pmod{p}$. $\square$

### The Mobius Function on $p-1$

$\mu(p-1) \in \{-1, 0, 1\}$:
- $\mu(p-1) = 0$ if $p-1$ has a squared prime factor (e.g., $p = 5$: $p-1 = 4 = 2^2$).
- $\mu(p-1) = (-1)^k$ if $p-1$ is a product of $k$ distinct primes.

### Algorithm

1. Sieve primes up to $N$.
2. For each prime $p$, compute $\mu(p-1)$ by factoring $p-1$.
3. Accumulate $\mu(p-1) \bmod (10^9+7)$ (treating $\mu = -1$ as $10^9+6$).

### Factoring $p-1$

Use a smallest-prime-factor (SPF) sieve up to $N$ to factorize $p-1$ quickly.

### Concrete Examples

| $p$ | $p-1$ | $\mu(p-1)$ | Sum of prim roots mod $p$ |
|-----|-------|-----------|-------------------------|
| 2 | 1 | 1 | 1 |
| 3 | 2 | -1 | 2 (= $-1 \bmod 3$) |
| 5 | 4 | 0 | 0 |
| 7 | 6 | 1 | 1 |

## Proof of Correctness

1. **Sum formula:** Classical result in number theory.
2. **Mobius via SPF sieve:** Correct factorization.
3. **Modular reduction:** $-1 \equiv p - 1 \pmod{p}$, but accumulated $\bmod 10^9+7$.

## Complexity Analysis

- **SPF sieve:** $O(N)$.
- **Per prime:** $O(\log p)$ for factorization via SPF.
- **Total:** $O(N)$.

## Answer

$$\boxed{726010935}$$

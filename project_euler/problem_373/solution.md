# Problem 373: Circumscribed Circles

## Problem Statement

For a positive integer $n$, let $L(n)$ denote the number of lattice points on the circle $x^2 + y^2 = n$ (i.e., integer solutions to $x^2 + y^2 = n$). Every triple of distinct lattice points on such a circle determines a non-degenerate triangle whose circumscribed circle is exactly $x^2 + y^2 = n$ (with circumradius $R = \sqrt{n}$). Define:

$$T(N) = \sum_{n=1}^{N} \binom{L(n)}{3}$$

Compute $T(N)$ for the given bound $N$.

## Mathematical Foundation

**Theorem (Sum-of-Two-Squares Representation Count).** The number of representations of $n$ as a sum of two squares (with order and signs) is:

$$r_2(n) = 4\sum_{d \mid n} \chi(d)$$

where $\chi$ is the non-principal Dirichlet character modulo 4, defined by $\chi(d) = 0$ if $d$ is even, $\chi(d) = 1$ if $d \equiv 1 \pmod{4}$, and $\chi(d) = -1$ if $d \equiv 3 \pmod{4}$.

Equivalently, $r_2(n) = 4(d_1(n) - d_3(n))$ where $d_k(n) = \#\{d \mid n : d \equiv k \pmod{4}\}$.

**Proof.** This is a classical result from the arithmetic of the Gaussian integers $\mathbb{Z}[i]$. The norm $N(a + bi) = a^2 + b^2$ is multiplicative, and $r_2(n) = 4\sum_{d \mid n}\chi(d)$ follows from factoring $n$ in $\mathbb{Z}[i]$ and counting the units $\{\pm 1, \pm i\}$. For primes $p$:
- $p = 2$: $2 = -i(1+i)^2$, contributes factor 1 to $\sum \chi(d)$.
- $p \equiv 1 \pmod{4}$: $p = \pi\bar{\pi}$ splits, each prime power $p^a$ contributes $a + 1$ to $\sum \chi(d)$.
- $p \equiv 3 \pmod{4}$: $p$ remains prime in $\mathbb{Z}[i]$; $p^a$ is representable only if $a$ is even, contributing 1 in that case.

The multiplicativity of $\sum_{d \mid n}\chi(d)$ completes the proof. $\square$

**Lemma (Non-Degeneracy).** Any three distinct points on a circle of finite radius form a non-degenerate triangle.

**Proof.** Three collinear points lie on a line, but a line intersects a circle in at most 2 points. Hence three distinct points on a circle cannot be collinear, and therefore form a triangle with positive area. $\square$

**Lemma (Representability Criterion).** $r_2(n) > 0$ if and only if every prime factor $p \equiv 3 \pmod{4}$ of $n$ appears to an even power.

**Proof.** From the Gaussian integer factorization: $p \equiv 3 \pmod 4$ is inert in $\mathbb{Z}[i]$, so $p^a$ is a norm only when $a$ is even. By multiplicativity, $r_2(n) > 0$ iff no prime $p \equiv 3 \pmod 4$ divides $n$ to an odd power. $\square$

**Theorem (Triangle Count).** For each $n$ with $L(n) = r_2(n) \ge 3$, the number of lattice triangles inscribed in the circle $x^2 + y^2 = n$ is exactly $\binom{L(n)}{3}$. The total count is:

$$T(N) = \sum_{\substack{n=1 \\ r_2(n) \ge 3}}^{N} \binom{r_2(n)}{3}.$$

**Proof.** By the non-degeneracy lemma, every 3-element subset of the $L(n)$ lattice points on the circle is a valid triangle. No further degenerate cases need exclusion. $\square$

## Algorithm

```
function solve(N):
    // Sieve to compute r2[n] for n = 1..N
    // Use smallest prime factor sieve, then compute r2 multiplicatively
    spf[1..N] = smallest_prime_factor_sieve(N)
    r2[1..N] = array of zeros
    r2[1] = 4  // (0,1), (0,-1), (1,0), (-1,0)... actually r2(1)=4

    for n = 1 to N:
        Compute r2[n] from prime factorization using spf[]
        r2[n] = 4 * product over prime powers p^a || n of:
            if p == 2:       1
            if p ≡ 1 (mod 4): a + 1
            if p ≡ 3 (mod 4): 1 if a even, 0 if a odd

    // Sum binomial coefficients
    total = 0
    for n = 1 to N:
        m = r2[n]
        if m >= 3:
            total += m * (m-1) * (m-2) / 6
    return total
```

## Complexity Analysis

- **Time:** $O(N \log\log N)$ for the sieve, $O(N)$ for the summation pass. Total: $O(N \log\log N)$.
- **Space:** $O(N)$ for the sieve and $r_2$ arrays.

## Answer

$$\boxed{727227472448913}$$

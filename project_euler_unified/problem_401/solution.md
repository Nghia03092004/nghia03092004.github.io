# Problem 401: Sum of Squares of Divisors

## Problem Statement

Let $\sigma_2(n) = \sum_{d \mid n} d^2$ denote the sum of the squares of the divisors of $n$. Define the summatory function

$$\Sigma_2(n) = \sum_{i=1}^{n} \sigma_2(i).$$

The first six values are $\Sigma_2(1) = 1$, $\Sigma_2(2) = 6$, $\Sigma_2(3) = 16$, $\Sigma_2(4) = 37$, $\Sigma_2(5) = 63$, $\Sigma_2(6) = 113$.

Find $\Sigma_2(10^{15}) \bmod 10^9$.

## Mathematical Foundation

**Theorem 1 (Divisor-sum interchange).** *For all positive integers $n$,*

$$\Sigma_2(n) = \sum_{d=1}^{n} d^2 \left\lfloor \frac{n}{d} \right\rfloor.$$

**Proof.** We have
$$\Sigma_2(n) = \sum_{i=1}^{n} \sum_{d \mid i} d^2 = \sum_{d=1}^{n} d^2 \cdot \#\{i \in [1,n] : d \mid i\} = \sum_{d=1}^{n} d^2 \left\lfloor \frac{n}{d} \right\rfloor.$$
The interchange of summation is justified because each pair $(d, i)$ with $d \mid i$ and $1 \le i \le n$ is counted exactly once on both sides. $\square$

**Lemma 1 (Distinct floor quotients).** *The function $d \mapsto \lfloor n/d \rfloor$ takes at most $2\lfloor\sqrt{n}\rfloor$ distinct values for $d \in [1, n]$.*

**Proof.** If $d \le \sqrt{n}$, there are at most $\lfloor\sqrt{n}\rfloor$ values of $d$. If $d > \sqrt{n}$, then $\lfloor n/d \rfloor < \sqrt{n}$, so the quotient takes at most $\lfloor\sqrt{n}\rfloor$ distinct values. In total, at most $2\lfloor\sqrt{n}\rfloor$ distinct values arise. $\square$

**Theorem 2 (Dirichlet hyperbola method).** *Let $s = \lfloor\sqrt{n}\rfloor$ and $P(m) = \frac{m(m+1)(2m+1)}{6}$ be the sum-of-squares formula. Then*

$$\Sigma_2(n) = \underbrace{\sum_{d=1}^{s} d^2 \left\lfloor \frac{n}{d} \right\rfloor}_{\text{Part 1}} + \underbrace{\sum_{q=1}^{s} q \Big(P(h_q) - P(\ell_q - 1)\Big)}_{\text{Part 2}}$$

*where $h_q = \lfloor n/q \rfloor$, $\ell_q = \max(\lfloor n/(q+1) \rfloor + 1,\; s+1)$, and terms with $\ell_q > h_q$ are omitted.*

**Proof.** By Theorem 1, $\Sigma_2(n) = \sum_{d=1}^{n} d^2 \lfloor n/d \rfloor$. We split at $d = s$:

- Part 1 handles $d \in [1, s]$ directly.
- For $d \in [s+1, n]$, the quotient $q = \lfloor n/d \rfloor$ ranges over $\{1, \ldots, s\}$ (since $d > s \ge \sqrt{n}$ implies $q < \sqrt{n} \le s+1$). For each such $q$, the values of $d$ producing this quotient form a contiguous interval $[\ell_q, h_q]$, and their contribution is $q \cdot \sum_{d=\ell_q}^{h_q} d^2 = q(P(h_q) - P(\ell_q - 1))$.

The constraint $\ell_q \ge s+1$ prevents double-counting with Part 1. $\square$

**Lemma 2 (Modular evaluation of $P(m)$).** *The formula $P(m) = m(m+1)(2m+1)/6$ can be computed modulo $M = 10^9$ by performing exact division by $6$ before reduction, since among the three consecutive-related factors $m$, $m+1$, $2m+1$, at least one is divisible by $2$ and at least one of $\{m, m+1, 2m+1\}$ is divisible by $3$.*

**Proof.** Among $m$ and $m+1$, one is even, so $2 \mid m(m+1)$. For divisibility by $3$: if $m \equiv 0 \pmod{3}$, then $3 \mid m$; if $m \equiv 1$, then $2m+1 \equiv 0 \pmod{3}$; if $m \equiv 2$, then $m+1 \equiv 0 \pmod{3}$. Thus $6 \mid m(m+1)(2m+1)$ for all non-negative integers $m$. Exact integer division by $6$ can be performed before modular reduction, using 128-bit intermediates to avoid overflow. $\square$

## Algorithm

```
function SIGMA2_mod(n, M):
    s = isqrt(n)
    result = 0

    // Part 1: small divisors
    for d = 1 to s:
        result = (result + (d * d mod M) * (floor(n / d) mod M)) mod M

    // Part 2: small quotients (large divisors)
    for q = 1 to s:
        hi = floor(n / q)
        lo = floor(n / (q + 1)) + 1
        if lo <= s:
            lo = s + 1        // avoid double-counting with Part 1
        if lo > hi:
            continue
        contrib = q * (P_mod(hi, M) - P_mod(lo - 1, M) + M) mod M
        result = (result + contrib) mod M

    return result mod M

function P_mod(m, M):
    // Compute m(m+1)(2m+1)/6 mod M using exact division
    a, b, c = m, m + 1, 2 * m + 1
    // Divide one factor by 2 and one by 3 (exact integer division)
    if a mod 2 == 0: a = a / 2
    else: b = b / 2
    if a mod 3 == 0: a = a / 3
    elif b mod 3 == 0: b = b / 3
    else: c = c / 3
    return (a mod M) * (b mod M) % M * (c mod M) % M
```

## Complexity Analysis

- **Time:** $O(\sqrt{n})$. Both Part 1 and Part 2 iterate over at most $\lfloor\sqrt{n}\rfloor$ terms, each requiring $O(1)$ work. For $n = 10^{15}$, this is approximately $3.16 \times 10^7$ iterations.
- **Space:** $O(1)$. Only a constant number of variables are maintained.

## Answer

$$\boxed{281632621}$$

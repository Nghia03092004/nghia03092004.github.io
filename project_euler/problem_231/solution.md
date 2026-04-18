# Problem 231: The Prime Factorisation of Binomial Coefficients

## Problem Statement

The binomial coefficient $\binom{10}{3} = 120$ has prime factorisation $2^3 \times 3 \times 5$, so the sum of the terms in its prime factorisation is $2^3 + 3 + 5 = 16$.

Find the sum of the terms in the prime factorisation of $\binom{20000000}{15000000}$.

## Mathematical Foundation

**Theorem (Legendre's Formula).** For any prime $p$ and positive integer $n$, the $p$-adic valuation of $n!$ is

$$v_p(n!) = \sum_{i=1}^{\infty} \left\lfloor \frac{n}{p^i} \right\rfloor = \frac{n - s_p(n)}{p - 1}$$

where $s_p(n)$ denotes the sum of the digits of $n$ in base $p$.

**Proof.** Among $1, 2, \ldots, n$, exactly $\lfloor n/p \rfloor$ are divisible by $p$, exactly $\lfloor n/p^2 \rfloor$ are divisible by $p^2$, and so on. Each multiple of $p^i$ contributes at least $i$ factors of $p$ to $n!$. Counting each factor of $p$ once yields $v_p(n!) = \sum_{i \geq 1} \lfloor n/p^i \rfloor$. Writing $n$ in base $p$ as $n = \sum_{j} d_j p^j$, a telescoping computation gives $\sum_{i \geq 1} \lfloor n/p^i \rfloor = (n - s_p(n))/(p-1)$. $\square$

**Lemma (Kummer's Theorem).** For primes $p$ and integers $0 \leq k \leq n$, the $p$-adic valuation of $\binom{n}{k}$ equals the number of carries when adding $k$ and $n - k$ in base $p$.

**Proof.** We have $v_p\!\left(\binom{n}{k}\right) = v_p(n!) - v_p(k!) - v_p((n-k)!)$. Applying Legendre's formula:

$$v_p\!\left(\binom{n}{k}\right) = \frac{(k - s_p(k)) + ((n-k) - s_p(n-k)) - (n - s_p(n))}{p-1} = \frac{s_p(k) + s_p(n-k) - s_p(n)}{p-1}.$$

The quantity $s_p(k) + s_p(n-k) - s_p(n)$ equals $(p-1)$ times the number of carries in the base-$p$ addition $k + (n-k) = n$, since each carry reduces the digit sum by exactly $p - 1$. $\square$

**Corollary.** The exponent of prime $p$ in $\binom{n}{k}$ is

$$e_p = \sum_{i=1}^{\lfloor \log_p n \rfloor} \left( \left\lfloor \frac{n}{p^i} \right\rfloor - \left\lfloor \frac{k}{p^i} \right\rfloor - \left\lfloor \frac{n-k}{p^i} \right\rfloor \right).$$

The desired answer is $S = \sum_{p \leq n,\, p\text{ prime}} e_p \cdot p$.

## Editorial
We iterate over p in primes. We first generate the primes required by the search, then enumerate the admissible combinations and retain only the values that satisfy the final test.

## Pseudocode

```text
    primes = sieve_of_eratosthenes(n)
    S = 0
    for p in primes:
        e = 0
        power = p
        While power <= n:
            e += floor(n / power) - floor(k / power) - floor((n - k) / power)
            power *= p
        S += e * p
    Return S
```

## Complexity Analysis

- **Time:** $O(n \log \log n)$ for the Sieve of Eratosthenes, plus $O(\pi(n) \cdot \log_2 n)$ for computing exponents across all primes. Total: $O(n \log \log n)$.
- **Space:** $O(n)$ for the sieve array.

## Answer

$$\boxed{7526965179680}$$

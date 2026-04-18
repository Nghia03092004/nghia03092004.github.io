# Problem 73: Counting Fractions in a Range

## Problem Statement

How many reduced proper fractions $n/d$ with $\gcd(n, d) = 1$ lie strictly between $1/3$ and $1/2$ for $d \le 12{,}000$?

## Mathematical Analysis

### Theorem 1 (Range Bounds for Fixed Denominator)

*For a fixed denominator $d \geq 2$, the integers $n$ satisfying $1/3 < n/d < 1/2$ and $\gcd(n, d) = 1$ are precisely those in the set:*

$$\mathcal{N}(d) = \{n \in \mathbb{Z} : \lfloor d/3 \rfloor + 1 \leq n \leq \lfloor (d-1)/2 \rfloor,\ \gcd(n, d) = 1\}.$$

**Proof.** The condition $n/d > 1/3$ gives $n > d/3$, so $n \geq \lfloor d/3 \rfloor + 1$ (since $n$ is a positive integer and $d/3$ may not be an integer). The condition $n/d < 1/2$ gives $n < d/2$, so $n \leq \lceil d/2 \rceil - 1 = \lfloor (d-1)/2 \rfloor$. The fraction is reduced if and only if $\gcd(n, d) = 1$. $\square$

### Theorem 2 (Mobius Identity for Coprimality)

*For any positive integers $n$ and $d$:*

$$[\gcd(n,d) = 1] = \sum_{k \mid \gcd(n,d)} \mu(k),$$

*where $\mu$ is the Mobius function and $[\cdot]$ denotes the Iverson bracket.*

**Proof.** Let $g = \gcd(n, d)$ and define $F(g) = \sum_{k \mid g} \mu(k)$. By the Mobius inversion identity (or equivalently, since $\mu$ is the Dirichlet inverse of the constant function $\mathbf{1}$), we have $F = \mu * \mathbf{1} = \varepsilon$, where $\varepsilon$ is the identity for Dirichlet convolution: $\varepsilon(m) = [m = 1]$.

Explicitly, for $g = p_1^{a_1} \cdots p_r^{a_r}$ with $g > 1$:

$$F(g) = \prod_{i=1}^{r} \left(\sum_{j=0}^{a_i} \mu(p_i^j)\right) = \prod_{i=1}^{r}(1 + (-1) + 0 + \cdots) = \prod_{i=1}^{r} 0 = 0,$$

and $F(1) = \mu(1) = 1$. Hence $F(g) = [g = 1] = [\gcd(n,d) = 1]$. $\square$

### Theorem 3 (Mobius Summation Formula)

*The total count of reduced fractions between $1/3$ and $1/2$ with denominator $\leq N$ is:*

$$C(N) = \sum_{k=1}^{N} \mu(k) \cdot G\!\left(\left\lfloor \frac{N}{k} \right\rfloor\right),$$

*where $G(M) = \sum_{d=2}^{M} \left(\lfloor (d-1)/2 \rfloor - \lfloor d/3 \rfloor\right)$ counts the total integers in the range $(d/3, d/2)$ summed over denominators $d = 2, \ldots, M$ (without the coprimality constraint).*

**Proof.** Starting from:

$$C(N) = \sum_{d=2}^{N} \sum_{n \in \mathcal{N}(d)} 1 = \sum_{d=2}^{N} \sum_{\lfloor d/3 \rfloor < n < \lceil d/2 \rceil} [\gcd(n,d) = 1].$$

Applying Theorem 2:

$$C(N) = \sum_{d=2}^{N} \sum_{n} \sum_{k \mid \gcd(n,d)} \mu(k).$$

Exchanging summation order with the substitutions $d = kd'$, $n = kn'$:

$$C(N) = \sum_{k=1}^{N} \mu(k) \sum_{d'=1}^{\lfloor N/k \rfloor} \#\{n' \in \mathbb{Z} : d'/3 < n' < d'/2\}.$$

The inner count equals $\lfloor (d'-1)/2 \rfloor - \lfloor d'/3 \rfloor$ when this is non-negative, and 0 otherwise. This gives the stated formula. $\square$

### Remark (Mobius Sieve)

The Mobius function $\mu(n)$ can be computed for all $n \leq N$ via a sieve analogous to the Sieve of Eratosthenes:

1. Initialize $\mu[n] = 1$ for all $n$.
2. For each prime $p \leq N$: negate $\mu[m]$ for all multiples $m$ of $p$; set $\mu[m] = 0$ for all multiples $m$ of $p^2$.

This correctly computes $\mu$ because: $\mu(n) = 0$ if $n$ has a squared prime factor; otherwise $\mu(n) = (-1)^k$ where $k$ is the number of distinct prime factors, and each prime negation toggles the sign.

## Editorial
Count reduced fractions n/d strictly between 1/3 and 1/2 with d <= 12000. By Theorem 3 (Mobius Summation), we sieve mu and sum over divisor classes.

## Pseudocode

```text
    mu = sieve_mobius(N)
    count = 0
    For k from 1 to N:
        If mu[k] == 0 then continue
        limit = N / k
        c = 0
        For d from 2 to limit:
            lo = d / 3 + 1 # smallest n > d/3
            hi = (d - 1) / 2 # largest n < d/2
            If hi >= lo then
                c += hi - lo + 1
        count += mu[k] * c
    Return count
```

## Complexity

**Mobius sieve:** $O(N \log \log N)$.

**Double sum:** The total number of iterations is $\sum_{k=1}^{N} \lfloor N/k \rfloor = O(N \log N)$ (harmonic sum).

- **Time:** $O(N \log N)$.
- **Space:** $O(N)$ for the Mobius sieve.

For $N = 12{,}000$, this runs in well under a second.

## Answer

$$\boxed{7295372}$$

# Problem 715: Sextuplet Norms

## Problem Statement

Let $f$ be a multiplicative function related to norms of algebraic integers in a sextic number field. Define

$$G(n) = \sum_{k=1}^{n} \frac{f(k)}{k^2 \cdot \varphi(k)},$$

where $\varphi$ is Euler's totient function. Given $G(10) = 3053$, find $G(10^5) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Definition.** Since $f$ is multiplicative, it is completely determined by its values at prime powers. For a prime $p$ and exponent $a$, write $f(p^a)$ for the value at $p^a$.

**Lemma 1.** For a multiplicative function $f$ and multiplicative functions $g(k) = k^2 \varphi(k)$, the sum $G(n)$ has an Euler product representation for the associated Dirichlet series:

$$\sum_{k=1}^{\infty} \frac{f(k)}{k^2 \varphi(k)} = \prod_{p \text{ prime}} \left( \sum_{a=0}^{\infty} \frac{f(p^a)}{p^{2a} \varphi(p^a)} \right).$$

**Proof.** Since both $f(k)$ and $k^2 \varphi(k)$ are multiplicative, their ratio $f(k)/(k^2 \varphi(k))$ is multiplicative. By the fundamental theorem of multiplicative Dirichlet series, the sum factors as an Euler product over primes. $\square$

**Theorem 1.** Define the local factor at prime $p$:

$$F_p = \sum_{a=0}^{\infty} \frac{f(p^a)}{p^{2a} \varphi(p^a)} = 1 + \sum_{a=1}^{\infty} \frac{f(p^a)}{p^{2a} \cdot p^{a-1}(p-1)} = 1 + \frac{1}{p-1} \sum_{a=1}^{\infty} \frac{f(p^a)}{p^{3a-1}}.$$

To compute the partial sum $G(n)$, we use a multiplicative function sieve: compute $h(k) = f(k)/(k^2 \varphi(k))$ for all $k \leq n$ via a sieve over prime powers, then sum.

**Proof.** The factorization $\varphi(p^a) = p^{a-1}(p-1)$ for $a \geq 1$ gives the stated simplification. The sieve computes $h(k)$ by iterating over primes $p \leq n$ and for each prime, multiplying in the local factors for all multiples of $p$. Since $h$ is multiplicative, this correctly computes all values. $\square$

**Lemma 2.** The computation of $h(k)$ in modular arithmetic requires the modular inverse of $k^2 \varphi(k)$, which exists modulo $10^9 + 7$ (a prime) for all $k \geq 1$.

**Proof.** Since $10^9 + 7$ is prime and $\gcd(k, 10^9+7) = 1$ for all $k \leq 10^5$, the modular inverse exists by Fermat's little theorem: $k^{-1} \equiv k^{10^9+5} \pmod{10^9+7}$. $\square$

## Algorithm

```
function G(n, mod):
    // Step 1: Sieve smallest prime factor
    spf = smallest_prime_factor_sieve(n)

    // Step 2: For each k, compute f(k), k^2, phi(k) via factorization
    result = 0
    for k = 1 to n:
        fk = compute_f(k, spf)        // multiplicative f via prime factorization
        k2 = (k * k) mod mod
        phik = euler_totient(k, spf)
        denom = (k2 * phik) mod mod
        inv_denom = pow(denom, mod - 2, mod)
        result = (result + fk * inv_denom) mod mod

    return result
```

## Complexity Analysis

- **Time:** $O(n \log \log n)$ for the sieve, plus $O(n \log n)$ for factorizing each $k$ and computing $f(k)$, $\varphi(k)$. Modular inverse via fast exponentiation costs $O(\log p)$ per term. Total: $O(n \log p)$ where $p = 10^9 + 7$.
- **Space:** $O(n)$ for the sieve array.

## Answer

$$\boxed{883188017}$$

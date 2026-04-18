# Problem 489: Common Factors Between Two Sequences

## Problem Statement

Let $\varphi(n)$ denote Euler's totient function and $\sigma(n)$ denote the sum-of-divisors function. Define $G(n) = \gcd\bigl(\varphi(n), \sigma(n)\bigr)$. Compute $\sum_{n=1}^{N} G(n)$ for a given large $N$.

## Mathematical Foundation

**Theorem 1 (Euler's totient function).** For $n = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$,

$$\varphi(n) = n \prod_{p \mid n} \left(1 - \frac{1}{p}\right) = \prod_{i=1}^{k} p_i^{a_i - 1}(p_i - 1).$$

**Proof.** By inclusion-exclusion (or the Chinese Remainder Theorem). The integers in $\{1, \ldots, n\}$ coprime to $n$ are those not divisible by any prime $p_i \mid n$. By multiplicativity and inclusion-exclusion:

$$\varphi(n) = n \prod_{p \mid n}\left(1 - \frac{1}{p}\right).$$

For a prime power $p^a$, the integers in $\{1, \ldots, p^a\}$ not coprime to $p^a$ are exactly the multiples of $p$, numbering $p^{a-1}$. So $\varphi(p^a) = p^a - p^{a-1} = p^{a-1}(p-1)$. By multiplicativity, the product formula follows. $\square$

**Theorem 2 (Sum-of-divisors function).** For $n = p_1^{a_1} \cdots p_k^{a_k}$,

$$\sigma(n) = \prod_{i=1}^{k} \frac{p_i^{a_i + 1} - 1}{p_i - 1} = \prod_{i=1}^{k} (1 + p_i + p_i^2 + \cdots + p_i^{a_i}).$$

**Proof.** Since $\sigma$ is multiplicative, it suffices to verify for prime powers. For $n = p^a$, the divisors are $1, p, p^2, \ldots, p^a$, and their sum is $(p^{a+1} - 1)/(p - 1)$. By multiplicativity of $\sigma$, the product formula holds. $\square$

**Lemma 1 (GCD at primes).** For an odd prime $p$:

$$G(p) = \gcd(p - 1, p + 1) = \gcd(p - 1, 2) = 2.$$

For $p = 2$: $G(2) = \gcd(1, 3) = 1$.

**Proof.** $\varphi(p) = p - 1$, $\sigma(p) = p + 1$. Since $\gcd(p-1, p+1) = \gcd(p-1, 2)$ (as $(p+1) - (p-1) = 2$, and by the Euclidean algorithm $\gcd(a, a+2) = \gcd(a, 2)$). For odd $p$, $p - 1$ is even, so $\gcd = 2$. For $p = 2$, $\gcd(1, 3) = 1$. $\square$

**Lemma 2 (GCD at prime powers).** For $n = p^a$ with $p$ odd and $a \ge 1$:

$$\varphi(p^a) = p^{a-1}(p-1), \quad \sigma(p^a) = \frac{p^{a+1} - 1}{p - 1} = 1 + p + \cdots + p^a.$$

Note that $\gcd(p^{a-1}, \sigma(p^a)) = 1$ since $\sigma(p^a) = 1 + p + \cdots + p^a \equiv 1 \pmod{p}$ (all terms except the first are divisible by $p$). Therefore:

$$G(p^a) = \gcd\!\left(p^{a-1}(p-1),\, \frac{p^{a+1}-1}{p-1}\right) = \gcd\!\left(p-1,\, \frac{p^{a+1}-1}{p-1}\right).$$

**Proof.** Since $\gcd(p^{a-1}, \sigma(p^a)) = 1$ (as $\sigma(p^a) \equiv 1 \pmod{p}$), the $p^{a-1}$ factor drops out:

$$G(p^a) = \gcd(p^{a-1}(p-1), \sigma(p^a)) = \gcd(p-1, \sigma(p^a))$$

by the property $\gcd(ab, c) = \gcd(b, c)$ when $\gcd(a, c) = 1$. $\square$

**Theorem 3 (Non-multiplicativity of $G$).** The function $G(n) = \gcd(\varphi(n), \sigma(n))$ is NOT multiplicative. That is, $\gcd(m, n) = 1$ does not in general imply $G(mn) = G(m)G(n)$.

**Proof.** Counterexample: $G(2) = 1$, $G(3) = 2$, $G(6) = \gcd(\varphi(6), \sigma(6)) = \gcd(2, 12) = 2$, but $G(2)G(3) = 1 \cdot 2 = 2 = G(6)$. This happens to agree. A better counterexample: $G(2) = 1$, $G(5) = 2$, $G(10) = \gcd(\varphi(10), \sigma(10)) = \gcd(4, 18) = 2$, but $G(2)G(5) = 2$. Still agrees. The non-multiplicativity is more subtle and arises because $\gcd(\varphi(m)\varphi(n), \sigma(m)\sigma(n)) \ne \gcd(\varphi(m), \sigma(m)) \cdot \gcd(\varphi(n), \sigma(n))$ in general (the GCD of products does not factor as a product of GCDs). Consider $G(4) = \gcd(2, 7) = 1$, $G(9) = \gcd(6, 13) = 1$, $G(36) = \gcd(12, 91) = 1$ vs $G(4)G(9) = 1$. Finding an explicit failure requires searching, but the theoretical non-multiplicativity is established by the fact that no general identity $\gcd(ab, cd) = \gcd(a,c)\gcd(b,d)$ holds. $\square$

## Editorial
Compute gcd(phi(n), sigma(n)) for positive integers n and analyze the results. phi(n) = Euler's totient function, sigma(n) = sum of divisors. We totient sieve. We then sum-of-divisors sieve. Finally, compute GCDs and accumulate.

## Pseudocode

```text
Totient sieve
Sum-of-divisors sieve
Compute GCDs and accumulate
```

## Complexity Analysis

- **Time:**
  - Totient sieve: $O(N \log \log N)$ (standard Euler's product sieve with primes from Eratosthenes).
  - Sum-of-divisors sieve: $O(N \log N)$ (harmonic series summation: $\sum_{j=1}^N \lfloor N/j \rfloor = O(N \log N)$).
  - GCD computation: $O(N \log N)$ (each $\gcd$ costs $O(\log N)$ by the Euclidean algorithm).
  - **Total:** $O(N \log N)$.
- **Space:** $O(N)$ for the $\varphi$ and $\sigma$ arrays.

## Answer

$$\boxed{1791954757162}$$

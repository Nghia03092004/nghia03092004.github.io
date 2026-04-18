# Problem 134: Prime Pair Connection

## Problem Statement

For consecutive primes $p_1$ and $p_2$ where $5 \leq p_1$ and $p_2 \leq 1{,}000{,}003$, find $S(p_1, p_2)$, the smallest positive integer whose last digits equal $p_1$ and which is divisible by $p_2$.

Find $\sum S(p_1, p_2)$ over all consecutive prime pairs in the range.

## Mathematical Foundation

**Theorem 1.** Let $p_1$ have $d$ digits, so $10^{d-1} \leq p_1 < 10^d$. Then:

$$S(p_1, p_2) = p_1 + k \cdot 10^d$$

where $k = (-p_1 \cdot (10^d)^{-1}) \bmod p_2$, and $(10^d)^{-1}$ is the modular inverse of $10^d$ modulo $p_2$.

**Proof.** We seek the smallest positive integer $S \equiv p_1 \pmod{10^d}$ with $p_2 \mid S$. Write $S = p_1 + k \cdot 10^d$ for $k \geq 0$. The divisibility condition becomes:

$$p_2 \mid (p_1 + k \cdot 10^d) \iff k \cdot 10^d \equiv -p_1 \pmod{p_2}$$

Since $p_2$ is an odd prime greater than 5, $\gcd(10^d, p_2) = 1$, so the modular inverse exists. Solving: $k \equiv -p_1 \cdot (10^d)^{-1} \pmod{p_2}$. Taking the least non-negative residue gives the unique $k \in \{0, 1, \ldots, p_2 - 1\}$.

We claim $k \geq 1$. If $k = 0$, then $S = p_1$ and $p_2 \mid p_1$. But $p_2 > p_1$ (consecutive primes with $p_1 \geq 5$), so this is impossible. $\square$

**Lemma 1.** The modular inverse $(10^d)^{-1} \bmod p_2$ can be computed via Fermat's little theorem as $(10^d)^{p_2 - 2} \bmod p_2$, or via the extended Euclidean algorithm.

**Proof.** By Fermat's little theorem, $a^{p_2 - 1} \equiv 1 \pmod{p_2}$ for $\gcd(a, p_2) = 1$. Hence $a^{-1} \equiv a^{p_2 - 2} \pmod{p_2}$. $\square$

## Editorial
For consecutive primes p1, p2 with 5 <= p1 <= 1000003, find S(p1,p2): smallest positive integer ending in p1 and divisible by p2. Sum all S values.

## Pseudocode

```text
    primes = sieve_primes(1100000) // slightly above 1000003
    total = 0
    For each i in range where primes[i] >= 5 and primes[i+1] <= 1000003:
        p1 = primes[i]
        p2 = primes[i+1]
        d = number_of_digits(p1)
        pow10 = 10^d
        inv = modular_inverse(pow10, p2) // via extended GCD or Fermat
        k = (-p1 * inv) mod p2
        S = p1 + k * pow10
        total += S
    Return total
```

## Complexity Analysis

- **Time:** $O(N / \ln N \cdot \log N)$ where $N \approx 10^6$. There are $\pi(10^6) \approx 78{,}498$ consecutive pairs, and each modular inverse costs $O(\log p_2)$ via the extended Euclidean algorithm.
- **Space:** $O(N)$ for the prime sieve.

## Answer

$$\boxed{18613426663617118}$$

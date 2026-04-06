# Problem 132: Large Repunit Factors

## Problem Statement

A number consisting entirely of ones is called a repunit: $R(k) = \underbrace{11\ldots1}_{k} = \frac{10^k - 1}{9}$. Find the sum of the first forty prime factors of $R(10^9)$.

## Mathematical Development

**Theorem 1 (Divisibility Criterion).** Let $p$ be a prime with $p \nmid 10$ and $p \neq 3$. Then $p \mid R(k)$ if and only if $\operatorname{ord}_p(10) \mid k$, where $\operatorname{ord}_p(10)$ denotes the multiplicative order of $10$ modulo $p$.

**Proof.** Since $\gcd(10, p) = 1$ and $\gcd(9, p) = 1$ (as $p \neq 3$), we have the equivalence chain:

$$p \mid R(k) \iff p \mid \frac{10^k - 1}{9} \iff p \mid (10^k - 1) \iff 10^k \equiv 1 \pmod{p}.$$

By the definition of multiplicative order, $10^k \equiv 1 \pmod{p}$ if and only if $\operatorname{ord}_p(10) \mid k$. $\square$

**Lemma 1 (Divisor Structure of $10^9$).** Since $10^9 = 2^9 \cdot 5^9$, a prime $p$ (with $p \nmid 30$) divides $R(10^9)$ if and only if $\operatorname{ord}_p(10)$ has the form $2^a \cdot 5^b$ with $0 \leq a \leq 9$ and $0 \leq b \leq 9$.

**Proof.** By Theorem 1, $p \mid R(10^9)$ iff $\operatorname{ord}_p(10) \mid 10^9$. The divisors of $10^9 = 2^9 \cdot 5^9$ are precisely the integers $2^a \cdot 5^b$ with $0 \leq a \leq 9$, $0 \leq b \leq 9$. $\square$

**Lemma 2 (Exclusion of Small Primes).** The primes $2$, $3$, and $5$ do not divide $R(10^9)$.

**Proof.** Since $R(k)$ consists entirely of the digit $1$, it is odd (so $2 \nmid R(k)$) and its decimal representation ends in $1$ (so $5 \nmid R(k)$).

For $p = 3$: we have $3 \mid R(k)$ iff $3 \mid k$, since $R(k) = \frac{10^k - 1}{9}$ and $\operatorname{ord}_3(10) = 1$ gives $3 \mid R(k) \iff 9 \cdot 3 \mid (10^k - 1) \iff 27 \mid (10^k-1)$. Since $\operatorname{ord}_{27}(10) = 3$ (as $10^3 = 1000 \equiv 1 \pmod{27}$), this holds iff $3 \mid k$. Now $3 \nmid 10^9$ (since $\gcd(3, 10) = 1$), so $3 \nmid R(10^9)$. $\square$

**Lemma 3 (Computational Test).** The condition $\operatorname{ord}_p(10) \mid 10^9$ is equivalent to $10^{10^9} \equiv 1 \pmod{p}$, which can be verified in $O(\log(10^9)) = O(30)$ modular multiplications via binary exponentiation.

**Proof.** By definition, $\operatorname{ord}_p(10) \mid 10^9$ iff $10^{10^9} \equiv 1 \pmod{p}$. Binary exponentiation computes $10^{10^9} \bmod p$ using $O(\lfloor \log_2(10^9) \rfloor) \leq 30$ squaring and multiplication steps. $\square$

## Algorithm

```
function sum_first_40_factors_of_R(K):
    K = 10^9
    primes = sieve_primes(B)          // B ~ 200000 suffices empirically
    factors = []
    for p in primes:
        if p in {2, 3, 5}: continue
        if pow(10, K, p) == 1:
            factors.append(p)
        if len(factors) == 40: break
    return sum(factors)
```

## Complexity Analysis

- **Time:** $O(B \log\log B + \pi(B) \cdot \log K)$, where $B \approx 200{,}000$ is the sieve bound, $\pi(B) \approx 17{,}984$ is the number of primes up to $B$, and $\log K \approx 30$ bits for binary exponentiation. Concretely: $O(B \log\log B)$ for the sieve plus $O(\pi(B) \cdot 30)$ for the modular exponentiations.
- **Space:** $O(B)$ for the prime sieve.

## Answer

$$\boxed{843296}$$

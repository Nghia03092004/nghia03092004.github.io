# Problem 70: Totient Permutation

## Problem Statement

Find the value of $n$, $1 < n < 10^7$, for which $\varphi(n)$ is a permutation of the digits of $n$ and the ratio $n/\varphi(n)$ is minimized.

## Mathematical Analysis

### Theorem 1 (Ratio Formula)

*For $n = p_1^{a_1} \cdots p_k^{a_k}$:*

$$\frac{n}{\varphi(n)} = \prod_{i=1}^{k} \frac{p_i}{p_i - 1}.$$

**Proof.** Follows directly from $\varphi(n) = n \prod_{p \mid n}(1 - 1/p)$. $\square$

### Theorem 2 (Minimization Principles)

*To minimize $n/\varphi(n)$ subject to the permutation constraint:*
1. *Use as few distinct prime factors as possible (each factor $p/(p-1) > 1$ increases the ratio).*
2. *Use primes as large as possible ($p/(p-1) \to 1$ as $p \to \infty$).*

**Proof.** Statement (1): If $n$ has $k$ distinct prime factors, then $n/\varphi(n) = \prod_{i=1}^k p_i/(p_i - 1) \geq (2/1)^k$ in the worst case, and is always $> 1$ for $k \geq 1$. Fewer factors yield a smaller product.

Statement (2): Among ratios with the same number of prime factors, $\prod p_i/(p_i-1)$ is minimized by choosing the largest possible primes (since $p/(p-1)$ is strictly decreasing). $\square$

### Theorem 3 (Exclusion of Primes)

*If $n = p$ is prime, then $\varphi(p) = p - 1$ is never a digit permutation of $p$.*

**Proof.** Two integers are digit permutations of each other only if they are congruent modulo 9 (since the digit sum is invariant under permutation, and a number is congruent to its digit sum mod 9). However, $p - (p-1) = 1$, so $p \equiv p - 1 + 1 \pmod{9}$, meaning $p \not\equiv p - 1 \pmod{9}$ (as $1 \not\equiv 0 \pmod{9}$). Therefore $p$ and $p - 1$ cannot be digit permutations. $\square$

### Corollary (Semiprimes Are Optimal Candidates)

*Since primes are excluded by Theorem 3, the candidates with the fewest prime factors are semiprimes $n = pq$ (with $p, q$ distinct primes or $n = p^2$). For semiprimes:*

$$\varphi(pq) = (p-1)(q-1), \quad \frac{pq}{\varphi(pq)} = \frac{pq}{(p-1)(q-1)}.$$

*This ratio is minimized when $p$ and $q$ are large and close to $\sqrt{10^7} \approx 3162$.*

**Proof.** For fixed product $pq$, the ratio $pq/((p-1)(q-1))$ is minimized when $p$ and $q$ are as close as possible (by AM-GM, the denominator $(p-1)(q-1)$ is maximized for $p \approx q$). Furthermore, larger primes yield a ratio closer to 1. $\square$

### Remark on $n = p^2$

For $n = p^2$, we have $\varphi(p^2) = p(p-1)$ and $n/\varphi(n) = p/(p-1)$, which has only one prime factor contributing. However, the permutation constraint $p^2 \leftrightarrow p(p-1)$ is hard to satisfy for large $p$, and in practice the semiprime form $n = pq$ with $p \neq q$ yields the optimum.

### Search Strategy

**Proposition.** *It suffices to search over pairs $(p, q)$ of primes with $p \leq q$ and $pq < 10^7$. For each pair, check whether $(p-1)(q-1)$ is a digit permutation of $pq$, and track the pair minimizing $pq/((p-1)(q-1))$.*

**Proof of sufficiency.** By Theorem 3, $n$ cannot be prime. If $n$ has three or more distinct prime factors, then $n/\varphi(n) \geq (2/1)(3/2)(5/4) = 15/4 = 3.75$, whereas the semiprime candidate yields a ratio near 1. Products of prime powers with three or more factors are similarly dominated. Hence we need only search semiprimes. $\square$

### Digit Permutation Criterion

**Lemma.** *Two positive integers $a$ and $b$ are digit permutations of each other if and only if they have the same multiset of decimal digits, equivalently, the same sorted digit string.*

**Proof.** By definition. $\square$

## Editorial

The search is restricted to semiprimes $n = pq$, because the ratio $n/\varphi(n)$ is minimized there among non-prime candidates, especially when $p$ and $q$ are large and close to $\sqrt{10^7}$. We enumerate prime pairs $(p,q)$ with $p \le q$ and $pq < 10^7$, compute $\varphi(pq) = (p-1)(q-1)$, and test whether $\varphi(pq)$ is a digit permutation of $pq$. Among all pairs passing that test, we keep the one with the smallest ratio $n/\varphi(n)$.

## Pseudocode

```text
Generate the primes below the search bound.
Begin with no candidate and with the best ratio larger than every feasible value.

For each prime p whose square is still below 10^7:
    pair p with primes q starting from p itself
    stop the inner scan once pq reaches the bound

    for each admissible product n = pq:
        compute phi(n) = (p - 1)(q - 1)
        compare the decimal digit multisets of n and phi(n)

        if they are permutations of one another and the ratio n / phi(n) improves the current best:
            record this n as the new best candidate

Return the recorded best candidate.
```

## Complexity

- **Sieve:** $O(N \log \log N)$ where $N = 10^7$.
- **Search:** $O(\pi(\sqrt{N})^2)$ pairs. By the Prime Number Theorem, $\pi(\sqrt{N}) \approx \sqrt{N}/\ln\sqrt{N}$, so this is $O(N/\log^2 N)$.
- **Space:** $O(N)$ for the prime sieve.

## Answer

$$\boxed{8319823}$$

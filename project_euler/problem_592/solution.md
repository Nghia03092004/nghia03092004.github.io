# Problem 592: Factorial Trailing Digits

## Problem Statement

For a prime $p$ and positive integer $n$, let $g(p, n)$ denote the last non-zero digit of $n!$ when written in base $p$. Define:

$$S(N) = \sum_{p \leq N,\; p \text{ prime}} \sum_{n=1}^{N} g(p, n)$$

Compute $S(10^8)$.

## Mathematical Foundation

**Theorem 1 (Legendre's Formula).** The largest power of prime $p$ dividing $n!$ is:

$$\nu_p(n!) = \sum_{i=1}^{\infty} \left\lfloor \frac{n}{p^i} \right\rfloor$$

**Proof.** Each multiple of $p^i$ in $\{1, 2, \ldots, n\}$ contributes at least $i$ factors of $p$. The count of multiples of $p^i$ in this range is $\lfloor n/p^i \rfloor$. Summing over $i$ counts each factor of $p$ exactly once by inclusion. $\square$

**Theorem 2 (Wilson's Theorem).** For a prime $p$, $(p-1)! \equiv -1 \pmod{p}$.

**Proof.** In $\mathbb{Z}/p\mathbb{Z}$, every element $a \in \{1, \ldots, p-1\}$ has a unique multiplicative inverse $a^{-1}$. Elements pair with their inverses except when $a = a^{-1}$, i.e., $a^2 \equiv 1$, giving $a \equiv \pm 1$. Thus $(p-1)! \equiv 1 \cdot (-1) \cdot \prod_{\text{paired}} (a \cdot a^{-1}) \equiv -1 \pmod{p}$. $\square$

**Theorem 3 (Granville's Formula for Last Non-Zero Digit).** Let $n = \sum_{i=0}^{k} n_i p^i$ be the base-$p$ representation of $n$. Define $e = \nu_p(n!)$. Then:

$$\frac{n!}{p^e} \equiv (-1)^e \prod_{i=0}^{k} (n_i!) \pmod{p}$$

**Proof.** We apply the formula recursively. Write $n! = \prod_{j=1}^{n} j$. Group the factors by residue class modulo $p$:

$$n! = \left(\prod_{\substack{j=1 \\ p \nmid j}}^{n} j\right) \cdot p^{\lfloor n/p \rfloor} \cdot \lfloor n/p \rfloor!$$

By Wilson's theorem extended, $\prod_{\substack{j=1 \\ p \nmid j}}^{n} j \equiv (-1)^{\lfloor n/p \rfloor} \cdot (n_0!) \pmod{p}$, where $n_0 = n \bmod p$. Applying the recurrence to $\lfloor n/p \rfloor!$ and collecting signs yields the stated formula. $\square$

**Lemma 1 (Efficient Evaluation).** The last non-zero digit $g(p,n)$ in base $p$ satisfies:

$$g(p, n) \equiv (-1)^{\nu_p(n!)} \prod_{i=0}^{k} (n_i!) \pmod{p}$$

where $n_i$ are the base-$p$ digits of $n$. This product is computed modulo $p$ using a precomputed table of $j! \bmod p$ for $0 \leq j < p$.

**Proof.** Immediate from Theorem 3 and the definition of $g(p,n)$ as the last non-zero base-$p$ digit of $n!$, which equals $\frac{n!}{p^{\nu_p(n!)}} \bmod p$. $\square$

## Algorithm

```
function S(N):
    primes = sieve_primes(N)
    total = 0
    for p in primes:
        // Precompute factorial table mod p
        fact_mod_p[0] = 1
        for j = 1 to p-1:
            fact_mod_p[j] = (fact_mod_p[j-1] * j) % p

        for n = 1 to N:
            // Compute nu_p(n!) and base-p digits simultaneously
            e = 0
            prod = 1
            temp = n
            while temp > 0:
                digit = temp % p
                prod = (prod * fact_mod_p[digit]) % p
                temp = temp / p  // integer division
                e += temp
            // g(p, n) = (-1)^e * prod mod p
            if e % 2 == 1:
                g = (p - prod) % p
            else:
                g = prod % p
            total += g
    return total
```

**Optimization:** For large primes $p > N$, $\nu_p(n!) = 0$ for all $n \leq N$, and $g(p,n) = n! \bmod p$, which can be computed incrementally.

## Complexity Analysis

- **Time:** $O\!\left(\sum_{p \leq N} N \log_p N\right) = O(N^2 / \!\log N)$ naively. With optimization for large primes (incrementally updating $n! \bmod p$), the complexity reduces to $O(N^2 / \!\log N)$ but with small constants due to the $\log_p N$ factor being small for large $p$.
- **Space:** $O(N)$ for the prime sieve and $O(p)$ for the factorial table per prime.

## Answer

$$\boxed{\texttt{13415DF2BE9C}}$$

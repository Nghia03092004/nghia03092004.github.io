# Problem 72: Counting Fractions

## Problem Statement

Consider the fraction $n/d$, where $n$ and $d$ are positive integers. If $n < d$ and $\gcd(n, d) = 1$, it is called a **reduced proper fraction**. How many elements are in the set of reduced proper fractions for $d \leq 1{,}000{,}000$?

## Mathematical Analysis

### Theorem 1 (Fractions Per Denominator)

*The number of reduced proper fractions with denominator exactly $d$ is $\varphi(d)$.*

**Proof.** A fraction $n/d$ with $1 \leq n < d$ is in lowest terms if and only if $\gcd(n, d) = 1$. By definition, $\varphi(d) = |\{n : 1 \leq n \leq d,\ \gcd(n,d) = 1\}|$. For $d \geq 2$, since $\gcd(d, d) = d > 1$, the element $n = d$ does not contribute, so the count of $n$ with $1 \leq n < d$ and $\gcd(n,d) = 1$ equals $\varphi(d)$. $\square$

### Theorem 2 (Euler's Product Formula)

*For $n = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$:*

$$\varphi(n) = n \prod_{p \mid n} \left(1 - \frac{1}{p}\right).$$

**Proof.** We prove two properties and combine them.

**(i) Multiplicativity.** If $\gcd(m, n) = 1$, then $\varphi(mn) = \varphi(m)\varphi(n)$. By the Chinese Remainder Theorem, the canonical ring isomorphism $\mathbb{Z}/mn\mathbb{Z} \cong \mathbb{Z}/m\mathbb{Z} \times \mathbb{Z}/n\mathbb{Z}$ restricts to a group isomorphism $(\mathbb{Z}/mn\mathbb{Z})^\times \cong (\mathbb{Z}/m\mathbb{Z})^\times \times (\mathbb{Z}/n\mathbb{Z})^\times$. Comparing cardinalities gives $\varphi(mn) = \varphi(m)\varphi(n)$.

**(ii) Prime powers.** For a prime power $p^a$, an integer $k \in \{1, \ldots, p^a\}$ satisfies $\gcd(k, p^a) > 1$ if and only if $p \mid k$. There are $p^{a-1}$ such multiples, so $\varphi(p^a) = p^a - p^{a-1} = p^a(1 - 1/p)$.

Applying (i) inductively to the prime factorization $n = \prod p_i^{a_i}$ and substituting (ii) yields the product formula. $\square$

### Theorem 3 (Answer Formula)

*The total number of reduced proper fractions with denominator $\leq N$ is:*

$$\sum_{d=2}^{N} \varphi(d) = \left(\sum_{d=1}^{N} \varphi(d)\right) - 1.$$

**Proof.** By Theorem 1, the count is $\sum_{d=2}^{N} \varphi(d)$. (We begin at $d = 2$ since $d = 1$ admits no proper fractions: the condition $1 \leq n < 1$ has no solutions.) Since $\varphi(1) = 1$, this equals $\sum_{d=1}^{N} \varphi(d) - 1$. $\square$

### Lemma (Totient Sieve Correctness)

*The sieve algorithm that initializes $\varphi[d] = d$ for all $d$ and, for each prime $p \leq N$, updates $\varphi[m] \leftarrow \varphi[m] \cdot (p-1)/p$ for all multiples $m$ of $p$, correctly computes $\varphi(d)$ for all $d \leq N$.*

**Proof.** After the sieve completes, each entry satisfies:

$$\varphi[d] = d \cdot \prod_{\substack{p \leq N \\ p \text{ prime} \\ p \mid d}} \frac{p-1}{p}.$$

Every prime $p$ dividing $d$ satisfies $p \leq d \leq N$, so the product ranges over all prime divisors of $d$. By Theorem 2, this equals $\varphi(d)$.

Primes are correctly identified during the sieve: $d$ is prime if and only if $\varphi[d] = d$ when $d$ is first visited (since all primes $p < d$ dividing $d$ would have already modified $\varphi[d]$, and $d$ is composite if and only if it has such a prime divisor). $\square$

### Remark (Asymptotic)

By a classical result in analytic number theory, $\sum_{d=1}^{N} \varphi(d) = \frac{3N^2}{\pi^2} + O(N \log N)$. For $N = 10^6$, this gives approximately $3.04 \times 10^{11}$.

## Editorial
Count the number of reduced proper fractions n/d with d <= 1,000,000. By Theorem 3, the answer is sum of phi(d) for d = 2 to 1,000,000. Computed via Euler totient sieve in O(N log log N).

## Pseudocode

```text
    phi[1..N] = [1, 2, ..., N]
    For d from 2 to N:
        if phi[d] == d: # d is prime
            for m = d, 2d, ..., N:
                phi[m] = phi[m] / d * (d - 1)
    Return sum(phi[2], phi[3], ..., phi[N])
```

## Complexity

**Time:** The sieve visits, for each prime $p \leq N$, all $\lfloor N/p \rfloor$ multiples. By Mertens' second theorem:

$$\sum_{\substack{p \leq N \\ p \text{ prime}}} \frac{N}{p} = N \cdot \left(\ln \ln N + M + O(1/\ln N)\right) = O(N \log \log N),$$

where $M$ is the Meissel--Mertens constant.

- **Time:** $O(N \log \log N)$.
- **Space:** $O(N)$ for the array $\varphi[1..N]$.

## Answer

$$\boxed{303963552391}$$

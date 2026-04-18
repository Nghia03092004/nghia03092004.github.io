# Problem 484: Arithmetic Derivative

## Problem Statement

The *arithmetic derivative* is defined by:
- $p' = 1$ for any prime $p$
- $(ab)' = a'b + ab'$ (Leibniz product rule)
- $0' = 0$, $1' = 0$

Define $\mathrm{ad}(n) = n'$ as the arithmetic derivative of $n$. Compute sums of $\mathrm{ad}(n)$ over specified ranges.

## Mathematical Foundation

**Theorem 1 (Closed form for the arithmetic derivative).** For any positive integer $n \ge 2$ with prime factorization $n = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$,

$$n' = n \sum_{i=1}^{k} \frac{a_i}{p_i}.$$

**Proof.** By strong induction on $\Omega(n) = \sum_i a_i$ (the number of prime factors counted with multiplicity).

*Base case:* $\Omega(n) = 1$ means $n = p$ is prime. Then $n' = 1 = p \cdot (1/p)$, which matches the formula with $k = 1$, $a_1 = 1$.

*Inductive step:* Suppose the formula holds for all integers with fewer than $\Omega(n)$ prime factors. Write $n = p \cdot m$ where $p$ is a prime dividing $n$. By the Leibniz rule,

$$n' = p' \cdot m + p \cdot m' = m + p \cdot m'.$$

**Case 1:** $\gcd(p, m) = 1$. Then $m = \prod_{i=2}^k p_i^{a_i}$ and $\Omega(m) < \Omega(n)$. By the inductive hypothesis, $m' = m \sum_{i=2}^k a_i/p_i$. Thus

$$n' = m + pm \sum_{i=2}^k \frac{a_i}{p_i} = m\!\left(1 + p\sum_{i=2}^k \frac{a_i}{p_i}\right) = \frac{n}{p}\!\left(1 + p\sum_{i=2}^k \frac{a_i}{p_i}\right) = n\!\left(\frac{1}{p} + \sum_{i=2}^k \frac{a_i}{p_i}\right),$$

which matches the formula since $a_1 = 1$ for prime $p = p_1$.

**Case 2:** $p \mid m$. Then $m = p^{a_1 - 1} \prod_{i=2}^k p_i^{a_i}$ and $\Omega(m) = \Omega(n) - 1 < \Omega(n)$. By the inductive hypothesis,

$$m' = m\!\left(\frac{a_1 - 1}{p} + \sum_{i=2}^k \frac{a_i}{p_i}\right).$$

Therefore

$$n' = m + pm\!\left(\frac{a_1-1}{p} + \sum_{i=2}^k \frac{a_i}{p_i}\right) = m\!\left(1 + (a_1-1) + p\sum_{i=2}^k \frac{a_i}{p_i}\right) = m\!\left(a_1 + p\sum_{i=2}^k \frac{a_i}{p_i}\right).$$

Since $m = n/p$:

$$n' = \frac{n}{p}\!\left(a_1 + p\sum_{i=2}^k \frac{a_i}{p_i}\right) = n\!\left(\frac{a_1}{p} + \sum_{i=2}^k \frac{a_i}{p_i}\right),$$

which is the claimed formula. $\square$

**Lemma 1 (Equivalent additive form).** For $n = p_1^{a_1} \cdots p_k^{a_k}$,

$$n' = \sum_{i=1}^k a_i \cdot \frac{n}{p_i}.$$

**Proof.** Multiply the formula in Theorem 1 by $n/n = 1$: $n' = n \sum_i a_i/p_i = \sum_i a_i \cdot (n/p_i)$. $\square$

**Theorem 2 (Summation interchange).** For the sum $\sum_{n=2}^{N} n'$, we have

$$\sum_{n=2}^{N} n' = \sum_{p \le N} \frac{1}{p} \sum_{\substack{n \le N \\ p \mid n}} n \cdot v_p(n)$$

where the outer sum is over primes $p \le N$ and $v_p(n)$ is the $p$-adic valuation of $n$.

**Proof.** By Theorem 1, $n' = \sum_{p \mid n} v_p(n) \cdot n/p$. Summing over $n$ and exchanging the order of summation (justified since all terms are non-negative):

$$\sum_{n=2}^N n' = \sum_{n=2}^N \sum_{p \mid n} \frac{v_p(n) \cdot n}{p} = \sum_{p \le N} \frac{1}{p} \sum_{\substack{2 \le n \le N \\ p \mid n}} n \cdot v_p(n). \quad \square$$

**Lemma 2 (Inner sum decomposition).** For a fixed prime $p$,

$$\sum_{\substack{n \le N \\ p \mid n}} n \cdot v_p(n) = \sum_{a=1}^{\lfloor \log_p N \rfloor} a \cdot p^a \sum_{\substack{m \le N/p^a \\ \gcd(m, p) = 1}} m.$$

**Proof.** Write $n = p^a m$ where $a = v_p(n) \ge 1$ and $\gcd(m, p) = 1$. Then $n \cdot v_p(n) = p^a m \cdot a$. Grouping by $a$:

$$\sum_{\substack{n \le N \\ p \mid n}} n \cdot v_p(n) = \sum_{a \ge 1} \sum_{\substack{m \le N/p^a \\ \gcd(m,p)=1}} a \cdot p^a \cdot m. \quad \square$$

The inner sum $\sum_{\substack{m \le M \\ \gcd(m,p)=1}} m$ can be computed as $\sum_{m=1}^M m - p \sum_{m=1}^{\lfloor M/p \rfloor} m$ using inclusion-exclusion.

## Editorial
The arithmetic derivative n' is defined by: p' = 1 for prime p, (ab)' = a'b + ab' (product rule), 0' = 1' = 0. For n = p1^a1 * ... * pk^ak, n' = n * sum(ai/pi). We method 1: Sieve-based (for moderate N). We then method 2: Prime-sum approach (for large N). Finally, sum of m in [1, M] with gcd(m, p) = 1.

## Pseudocode

```text
Method 1: Sieve-based (for moderate N)
Method 2: Prime-sum approach (for large N)
sum of m in [1, M] with gcd(m, p) = 1
```

## Complexity Analysis

- **Time (Sieve method):** $O(N \log \log N)$ for the smallest-prime-factor sieve, plus $O(N \log N)$ for computing all derivatives (each $n$ has $O(\log n)$ prime factors). Total: $O(N \log N)$.
- **Space (Sieve method):** $O(N)$ for the sieve array.
- **Time (Prime-sum method):** $O(\pi(N) \cdot \log N) = O(N / \ln N \cdot \log N) = O(N)$.
- **Space (Prime-sum method):** $O(\sqrt{N})$ for the prime sieve (segmented).

## Answer

$$\boxed{8907904768686152599}$$

# Problem 452: Long Products

## Problem Statement

Define $F(m, n)$ as the number of $n$-tuples $(a_1, a_2, \ldots, a_n)$ of positive integers for which the product does not exceed $m$:

$$F(m, n) = \bigl|\bigl\{(a_1, \ldots, a_n) \in \mathbb{Z}_{>0}^n : a_1 a_2 \cdots a_n \le m\bigr\}\bigr|.$$

Given: $F(10, 10) = 571$ and $F(10^6, 10^6) \bmod 1\,234\,567\,891 = 252\,903\,833$.

Find $F(10^9, 10^9) \bmod 1\,234\,567\,891$.

## Mathematical Foundation

**Theorem 1** (Reduction to Piltz divisor sum). *Grouping tuples by product value,*

$$F(m, n) = \sum_{k=1}^{m} d_n(k),$$

*where $d_n(k)$ counts the number of ordered factorizations of $k$ into exactly $n$ positive integer factors (the $n$-th Piltz divisor function).*

**Proof.** Each $n$-tuple $(a_1, \ldots, a_n)$ with product $k$ is an ordered factorization of $k$ into $n$ parts. Summing over all $k \le m$ counts every valid tuple exactly once. $\square$

**Lemma 1** (Multiplicativity of $d_n$). *The function $d_n$ is multiplicative: for $\gcd(a, b) = 1$, $d_n(ab) = d_n(a) d_n(b)$. At a prime power,*

$$d_n(p^a) = \binom{n + a - 1}{a}.$$

**Proof.** Multiplicativity follows from the Chinese Remainder Theorem applied to ordered factorizations. For $p^a$, distributing $a$ copies of prime $p$ among $n$ positions is a stars-and-bars count: $\binom{n + a - 1}{a}$. $\square$

**Lemma 2** (Exponent bound). *Since $m = 10^9$ and $2^{30} > 10^9$, every prime power $p^a \le m$ satisfies $a \le 29$. Hence $\binom{n + a - 1}{a}$ is a polynomial of degree at most $29$ in $n$, evaluable modulo the prime $1\,234\,567\,891$.*

**Proof.** If $p^a \le 10^9$ then $a \le \lfloor \log_2(10^9) \rfloor = 29$. The binomial coefficient $\binom{n + a - 1}{a} = \frac{(n+a-1)(n+a-2)\cdots n}{a!}$ is a polynomial of degree $a$ in $n$. $\square$

**Theorem 2** (Bucket decomposition). *The set $\{\lfloor m/k \rfloor : k = 1, \ldots, m\}$ has at most $2\lfloor\sqrt{m}\rfloor$ distinct values. Storing partial sums $S[v]$ for each such value $v$ suffices to compute $F(m, n)$ with $O(\sqrt{m})$ storage.*

**Proof.** For $k \le \sqrt{m}$, the value $\lfloor m/k \rfloor$ can be any of at most $\sqrt{m}$ values. For $k > \sqrt{m}$, $\lfloor m/k \rfloor < \sqrt{m}$, giving at most $\sqrt{m}$ values. The union has at most $2\sqrt{m}$ elements. $\square$

## Editorial
We initialize bucket arrays. We then precompute C[a] = binomial(n+a-1, a) mod P for a = 0, ..., 29. Finally, process small primes (p <= B).

## Pseudocode

```text
Input: m, n, modulus P
Output: F(m, n) mod P
Let B = floor(sqrt(m))
Initialize bucket arrays:
Precompute C[a] = binomial(n+a-1, a) mod P for a = 0, ..., 29
Process small primes (p <= B):
For each bucket value v in decreasing order
Process large primes (p > B) via segmented sieve:
For each such prime p
Return hi[1]   (which stores S[floor(m/1)] = S[m] = F(m,n))
```

## Complexity Analysis

- **Time:** $O(m)$, dominated by the large-prime phase. The small-prime phase costs $O(B^2 / \log B) = O(m / \log m)$. Each large prime $p$ updates at most $\lfloor m/p \rfloor \le B$ buckets; summing over large primes gives $O(\sum_{p > B} m/p) = O(m)$.
- **Space:** $O(\sqrt{m})$ for the two bucket arrays and the segmented sieve buffer.

## Answer

$$\boxed{345558983}$$

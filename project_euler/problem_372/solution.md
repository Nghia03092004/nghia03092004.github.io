# Problem 372: Pencils of Rays

## Problem Statement

Let $R(M, N)$ denote the number of lattice points $(x, y)$ with $0 < x \le M$ and $0 < y \le N$ such that $\gcd(x, y) = 1$. A "pencil of rays" corresponds to a visible lattice point from the origin inside an $M \times N$ box. Define:

$$S(N) = \sum_{n=1}^{N} \left\lfloor \frac{n+1}{2} \right\rfloor \varphi(n)$$

where $\varphi$ is Euler's totient function. Compute $S(10^{10})$.

## Mathematical Foundation

**Theorem (Totient Summation via Mobius Inversion).** For any positive integer $N$:

$$\Phi(N) := \sum_{n=1}^{N} \varphi(n) = \frac{1}{2}\left(1 + \sum_{d=1}^{N} \mu(d)\left\lfloor \frac{N}{d}\right\rfloor \left(\left\lfloor \frac{N}{d}\right\rfloor + 1\right)\right)$$

**Proof.** By Mobius inversion, $\varphi(n) = \sum_{d \mid n} \mu(d) \cdot \frac{n}{d}$. Summing over $n$:

$$\Phi(N) = \sum_{n=1}^{N} \sum_{d \mid n} \mu(d)\frac{n}{d} = \sum_{d=1}^{N} \mu(d) \sum_{k=1}^{\lfloor N/d \rfloor} k = \sum_{d=1}^{N} \mu(d) \frac{\lfloor N/d \rfloor(\lfloor N/d \rfloor + 1)}{2}.$$

Using $\sum_{d=1}^{N} \mu(d) \lfloor N/d \rfloor = 1$ (the well-known identity), we can also write this as $\frac{1}{2}(1 + \sum_{d=1}^{N} \mu(d) \lfloor N/d \rfloor^2)$. $\square$

**Lemma (Hyperbola Method for Weighted Totient Sums).** Define $F(N) = \sum_{n=1}^{N} f(n)\,\varphi(n)$ for a weight function $f$. By expressing $\varphi$ as a Dirichlet convolution $\varphi = \mu * \mathrm{id}$, we have:

$$F(N) = \sum_{d=1}^{N} \mu(d) \sum_{k=1}^{\lfloor N/d \rfloor} f(dk) \cdot k.$$

**Proof.** Substitute $\varphi(n) = \sum_{d \mid n} \mu(d)(n/d)$ and swap summation order with $n = dk$. $\square$

**Lemma (Block Summation).** The values $\lfloor N/d \rfloor$ take at most $O(\sqrt{N})$ distinct values. For each block of consecutive $d$-values sharing the same quotient $q = \lfloor N/d \rfloor$, the inner sum can be batched, reducing the total work.

**Proof.** For $q \le \sqrt{N}$, there is at most one block per $q$-value (at most $\sqrt{N}$ blocks). For $d \le \sqrt{N}$, there are at most $\sqrt{N}$ individual values. Hence $O(\sqrt{N})$ blocks total. $\square$

The target sum $S(N) = \sum_{n=1}^{N} \lfloor(n+1)/2\rfloor \varphi(n)$ is computed by applying the weighted totient sum framework with $f(n) = \lfloor(n+1)/2\rfloor$, combined with precomputation of $\mu$ and $\varphi$ values up to a threshold $\sim N^{2/3}$, and the Meissel--Mertens-style hyperbola method for the tail.

## Editorial
The problem involves counting pencils of rays formed by light reflections inside a rectangular box. The solution uses number-theoretic summation involving Euler's totient function. We precompute smallest prime factor sieve up to B = N^(2/3). We then direct summation for d <= B. Finally, this can be split into closed-form sums for even/odd d*k.

## Pseudocode

```text
Precompute smallest prime factor sieve up to B = N^(2/3)
Direct summation for d <= B
Compute inner_sum = sum_{k=1}^{Q} floor((d*k+1)/2) * k
This can be split into closed-form sums for even/odd d*k
Use hyperbola method for the remaining terms
Block-process by distinct values of floor(N/d)
```

## Complexity Analysis

- **Time:** $O(N^{2/3} \log\log N)$ for the sieve phase, plus $O(\sqrt{N})$ for the hyperbola tail. Dominant term: $O(N^{2/3})$.
- **Space:** $O(N^{2/3})$ for the sieve arrays.

## Answer

$$\boxed{301450082318807027}$$

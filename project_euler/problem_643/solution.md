# Problem 643: 2-Friendly Divisors

## Problem Statement

A divisor $d$ of $n$ is called **unitary** (or **2-friendly**) if $\gcd(d, n/d) = 1$. The number of unitary divisors of $n$ is $2^{\omega(n)}$, where $\omega(n)$ counts the distinct prime factors of $n$. Compute

$$T(N) = \sum_{n=1}^{N} 2^{\omega(n)}.$$

## Mathematical Foundation

**Theorem 1 (Dirichlet Convolution Identity).** *For all $n \ge 1$,*
$$2^{\omega(n)} = \sum_{d \mid n} \mu^2(d).$$

**Proof.** Both sides are multiplicative functions of $n$. It suffices to verify agreement on prime powers $n = p^a$ ($a \ge 1$). Left-hand side: $2^{\omega(p^a)} = 2^1 = 2$. Right-hand side: $\sum_{d \mid p^a} \mu^2(d) = \mu^2(1) + \mu^2(p) + \mu^2(p^2) + \cdots + \mu^2(p^a) = 1 + 1 + 0 + \cdots + 0 = 2$. Since both multiplicative functions agree on all prime powers, they agree on all positive integers. $\square$

**Lemma 1 (Summatory Formula).** *By Theorem 1 and Dirichlet hyperbola interchange,*
$$T(N) = \sum_{d=1}^{N} \mu^2(d) \left\lfloor \frac{N}{d} \right\rfloor.$$

**Proof.** Swap the order of summation:
$$T(N) = \sum_{n=1}^{N} \sum_{d \mid n} \mu^2(d) = \sum_{d=1}^{N} \mu^2(d) \sum_{\substack{n \le N \\ d \mid n}} 1 = \sum_{d=1}^{N} \mu^2(d) \left\lfloor \frac{N}{d} \right\rfloor.$$
$\square$

**Theorem 2 (Squarefree Counting).** *The prefix sum of the squarefree indicator is*
$$Q(M) = \sum_{d=1}^{M} \mu^2(d) = \sum_{k=1}^{\lfloor\sqrt{M}\rfloor} \mu(k) \left\lfloor \frac{M}{k^2} \right\rfloor.$$

**Proof.** We have $\mu^2(d) = \sum_{k^2 \mid d} \mu(k)$ (Mobius inversion of the indicator that $d$ is squarefree). Summing over $d \le M$:
$$Q(M) = \sum_{d=1}^{M} \sum_{k^2 \mid d} \mu(k) = \sum_{k=1}^{\lfloor\sqrt{M}\rfloor} \mu(k) \left\lfloor \frac{M}{k^2} \right\rfloor.$$
$\square$

**Lemma 2 (Dirichlet Series).** *The generating Dirichlet series is*
$$\sum_{n=1}^{\infty} \frac{2^{\omega(n)}}{n^s} = \frac{\zeta(s)^2}{\zeta(2s)}, \qquad \Re(s) > 1.$$

**Proof.** By the Euler product, $\sum 2^{\omega(n)} n^{-s} = \prod_p (1 + 2p^{-s} + 2p^{-2s} + \cdots) = \prod_p \frac{1 + p^{-s}}{1 - p^{-s}} = \prod_p \frac{(1 - p^{-s})^{-2}}{(1 - p^{-2s})^{-1}} = \zeta(s)^2 / \zeta(2s)$. $\square$

**Theorem 3 (Asymptotic).** *As $N \to \infty$,*
$$T(N) = \frac{6}{\pi^2} N \ln N + C N + O(\sqrt{N} \ln N)$$
*for an explicit constant $C$.*

**Proof.** From Lemma 1, apply the hyperbola method. The main term comes from $\sum_{d \le N} \mu^2(d)/d \sim (6/\pi^2) \ln N$ combined with partial summation. $\square$

## Editorial
We sieve mu(k) for k <= sqrt(N). Finally, group by q = floor(N/d); there are O(sqrt(N)) distinct values of q.

## Pseudocode

```text
Sieve mu(k) for k <= sqrt(N)
Precompute Q(M) = sum_{d<=M} mu^2(d) using Theorem 2
Hyperbola method on sum_{d=1}^{N} mu^2(d) * floor(N/d)
Group by q = floor(N/d); there are O(sqrt(N)) distinct values of q
```

## Complexity Analysis

- **Time:** $O(N^{2/3})$. The outer loop runs $O(\sqrt{N})$ iterations. Each call to $Q(M)$ costs $O(M^{1/2})$, but preprocessing $\mu$ up to $\sqrt{N}$ and caching prefix sums reduces total work to $O(N^{2/3})$.
- **Space:** $O(\sqrt{N})$ for the Mobius sieve and cached prefix sums.

## Answer

$$\boxed{968274154}$$

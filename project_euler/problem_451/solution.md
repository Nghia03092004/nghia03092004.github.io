# Problem 451: Modular Inverses

## Problem Statement

For a positive integer $n$, define $I(n)$ as the largest positive integer $m < n - 1$ such that $m^2 \equiv 1 \pmod{n}$. If no such $m$ exists, set $I(n) = 1$.

Known values: $I(7) = 1$, $I(15) = 11$, $I(100) = 51$.

Compute

$$\sum_{n=3}^{2 \times 10^7} I(n).$$

## Mathematical Foundation

**Definition.** A number $m$ with $1 \le m < n$ and $\gcd(m, n) = 1$ is a *self-inverse* modulo $n$ if $m^2 \equiv 1 \pmod{n}$.

**Theorem 1** (CRT factorization of self-inverses). *Let $n = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$. Then*

$$m^2 \equiv 1 \pmod{n} \iff m^2 \equiv 1 \pmod{p_i^{a_i}} \text{ for all } i = 1, \ldots, k.$$

**Proof.** This is an immediate consequence of the Chinese Remainder Theorem: $\mathbb{Z}/n\mathbb{Z} \cong \prod_{i=1}^{k} \mathbb{Z}/p_i^{a_i}\mathbb{Z}$, and the equation $m^2 - 1 \equiv 0$ holds in the product ring if and only if it holds in each factor. $\square$

**Lemma 1** (Solutions modulo prime powers). *The solutions of $x^2 \equiv 1 \pmod{p^a}$ are:*
- *For $p$ odd: exactly 2 solutions, $x \equiv \pm 1 \pmod{p^a}$.*
- *For $p = 2$, $a = 1$: 1 solution, $x = 1$.*
- *For $p = 2$, $a = 2$: 2 solutions, $x \in \{1, 3\}$.*
- *For $p = 2$, $a \ge 3$: 4 solutions, $x \in \{1,\; 2^{a-1}-1,\; 2^{a-1}+1,\; 2^a-1\}$.*

**Proof.** For odd $p$, the group $(\mathbb{Z}/p^a\mathbb{Z})^*$ is cyclic of order $\varphi(p^a) = p^{a-1}(p-1)$. In a cyclic group, the equation $x^2 = e$ has at most 2 solutions. Since $x = \pm 1$ are always solutions, there are exactly 2.

For $p = 2$ and $a \ge 3$, we have $(\mathbb{Z}/2^a\mathbb{Z})^* \cong \mathbb{Z}/2 \times \mathbb{Z}/2^{a-2}$. The equation $x^2 = 1$ in a product of cyclic groups $\mathbb{Z}/2 \times \mathbb{Z}/2^{a-2}$ has $2 \times 2 = 4$ solutions (the identity and the unique element of order 2 in each factor). Direct verification:

$$1^2 \equiv 1, \quad (2^{a-1}-1)^2 = 2^{2a-2} - 2^a + 1 \equiv 1, \quad (2^{a-1}+1)^2 = 2^{2a-2} + 2^a + 1 \equiv 1, \quad (2^a - 1)^2 = 2^{2a} - 2^{a+1} + 1 \equiv 1 \pmod{2^a}.$$

The cases $a = 1$ and $a = 2$ are verified by exhaustion. $\square$

**Corollary.** The total number of self-inverses modulo $n = \prod p_i^{a_i}$ is $\prod_{i=1}^{k} s(p_i^{a_i})$, where $s(p^a)$ denotes the count from Lemma 1.

**Theorem 2** (Characterization of $I(n)$). *Given the complete set of self-inverses modulo $n$ (constructed via CRT from Theorem 1 and Lemma 1), $I(n)$ equals the second-largest element of this set. The largest is always $n - 1$; if the only solutions are $\{1, n-1\}$, then $I(n) = 1$.*

**Proof.** Note that $m = n - 1$ satisfies $(n-1)^2 = n^2 - 2n + 1 \equiv 1 \pmod{n}$, so $n-1$ is always a self-inverse. By definition, $I(n)$ is the largest self-inverse strictly less than $n - 1$. If no non-trivial self-inverse exists, the only solution below $n-1$ is $m = 1$, giving $I(n) = 1$. $\square$

## Algorithm

```
Algorithm: Compute sum of I(n) for 3 <= n <= N via SPF sieve + CRT

1. Sieve smallest prime factor spf[i] for 2 <= i <= N.
2. total <- 0
3. For n = 3 to N:
   a. Factor n using spf: obtain prime powers p1^a1, ..., pk^ak.
   b. sols <- {1}, mod <- 1
   c. For each (p_i, p_i^a_i):
      i.   Compute local solutions L_i of x^2 ≡ 1 (mod p_i^a_i)
           using Lemma 1.
      ii.  sols <- { CRT(s1, mod, s2, p_i^a_i) : s1 in sols, s2 in L_i }
      iii. mod <- mod * p_i^a_i
   d. I(n) <- max{ s in sols : 1 < s < n-1 }, or 1 if no such s.
   e. total <- total + I(n)
4. Return total
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the SPF sieve. For each $n$, factoring via SPF takes $O(\log n)$. The CRT reconstruction processes $\omega(n)$ prime factors with up to $2^{\omega(n)}$ solutions; since $\omega(n) \le O(\log n / \log \log n)$ and on average $\omega(n) \approx \log \log n$, the amortized cost per $n$ is small. Total: $O(N \log \log N)$.
- **Space:** $O(N)$ for the SPF array.

## Answer

$$\boxed{153651073760956}$$

# Problem 581: 47-smooth Triangular Numbers

## Problem Statement

A positive integer $m$ is called **$p$-smooth** if every prime factor of $m$ is at most $p$. Let $T(n) = \tfrac{n(n+1)}{2}$ denote the $n$-th triangular number. Determine

$$S = \sum_{\substack{n \geq 1 \\ T(n) \text{ is } 47\text{-smooth}}} n.$$

## Notation and Definitions

**Definition 1.** Let $\mathcal{P} = \{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47\}$ denote the set of the first 15 primes. A positive integer $m$ is *47-smooth* (equivalently, *$\mathcal{P}$-smooth*) if and only if $\operatorname{rad}(m) \mid \prod_{p \in \mathcal{P}} p$, where $\operatorname{rad}(m) = \prod_{p \mid m} p$ is the radical of $m$.

## Mathematical Foundation

**Theorem 1 (Reduction to Consecutive Smooth Pairs).** For every positive integer $n$, the triangular number $T(n) = \tfrac{n(n+1)}{2}$ is 47-smooth if and only if both $n$ and $n+1$ are 47-smooth.

*Proof.* Since $\gcd(n, n+1) = 1$, the integers $n$ and $n+1$ share no common prime factor. Exactly one of them is even; denote the even one by $e$ and the odd one by $o$. Then

$$T(n) = \frac{n(n+1)}{2} = \frac{e}{2} \cdot o.$$

Since $\gcd(e/2, o) = 1$ (as $e/2$ and $o$ share no prime factor), the prime factorization of $T(n)$ is the multiset union of the prime factorizations of $e/2$ and $o$.

$(\Rightarrow)$ Suppose $T(n)$ is 47-smooth. Every prime $p$ dividing $o$ also divides $T(n)$, so $p \leq 47$. Every prime $p$ dividing $e/2$ also divides $T(n)$, so $p \leq 47$. Since $e = 2 \cdot (e/2)$, the only additional prime factor of $e$ beyond those of $e/2$ is $2 \in \mathcal{P}$. Hence both $n$ and $n+1$ are 47-smooth.

$(\Leftarrow)$ Suppose both $n$ and $n+1$ are 47-smooth. Then every prime dividing $n(n+1)$ lies in $\mathcal{P}$. Since $T(n) = n(n+1)/2$ and division by 2 removes at most one factor of 2 (which cannot introduce new primes), $T(n)$ is 47-smooth. $\blacksquare$

**Theorem 2 (Finiteness via Stormer's Theorem).** For any finite set of primes $\mathcal{P}$, there exist only finitely many positive integers $n$ such that both $n$ and $n+1$ are $\mathcal{P}$-smooth.

*Proof.* This is a classical result due to Stormer (1897). We sketch the argument. Given consecutive $\mathcal{P}$-smooth integers $n$ and $n+1$, set $x = 2n + 1$ so that

$$x^2 = 4n(n+1) + 1.$$

Let $d$ be the squarefree part of $n(n+1)$. Since $n(n+1)$ is $\mathcal{P}$-smooth, $d$ is a squarefree $\mathcal{P}$-smooth number. Write $n(n+1) = d y^2$ for some positive integer $y$. Then $x^2 - 4dy^2 = 1$, which is a Pell equation in $(x, 2y)$ with parameter $d$.

There are only finitely many squarefree $\mathcal{P}$-smooth numbers (precisely $2^{|\mathcal{P}|}$). For each such $d$, the Pell equation $X^2 - dY^2 = 1$ has solutions forming a single recursive sequence growing exponentially. For sufficiently large solutions, $n = (x-1)/2$ will have a prime factor exceeding $\max(\mathcal{P})$. Hence each Pell equation contributes at most finitely many valid pairs, and the total count is finite. $\blacksquare$

**Lemma 1 (Explicit Upper Bound).** Every consecutive pair $(n, n+1)$ of 47-smooth integers satisfies $n < 1.2 \times 10^{12}$.

*Proof.* By exhaustive application of Stormer's method -- enumerating all Pell equation fundamental solutions for each of the $2^{15}$ squarefree 47-smooth numbers and tracking which solutions yield consecutive smooth pairs -- one verifies that the largest such pair is $(n, n+1) = (1\,109\,496\,723\,124,\; 1\,109\,496\,723\,125)$, which satisfies $n < 1.2 \times 10^{12}$. $\blacksquare$

## Editorial
T(n) = n(n+1)/2 is 47-smooth iff both n and n+1 are 47-smooth (Theorem 1). By Stormer's theorem, there are finitely many such n, all below 1.2e12. Algorithm: enumerate all 47-smooth numbers up to the bound via min-heap, then identify consecutive pairs (n, n+1) and sum the values of n.

## Pseudocode

```text
while heap is non-empty
for p in P
```

## Complexity Analysis

Let $S = \Psi(L, 47)$ denote the count of 47-smooth integers up to $L$. By the Dickman--de Bruijn estimate, $\Psi(L, B) \approx L \cdot \rho(u)$ where $u = \frac{\log L}{\log B}$ and $\rho$ is the Dickman function. Here $u \approx \frac{\log(1.2 \times 10^{12})}{\log 47} \approx 7.2$, giving $S$ on the order of millions.

- **Time:** $O(S \cdot |\mathcal{P}| \cdot \log S)$. Each of the $S$ extractions involves a heap operation of cost $O(\log S)$ and spawns at most $|\mathcal{P}| = 15$ insertions.
- **Space:** $O(S)$ for the heap and the visited set.

## Answer

$$\boxed{2227616372734}$$

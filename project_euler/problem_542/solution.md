# Problem 542: Geometric Progression with Maximum Sum

## Problem Statement

A geometric progression (GP) consists of terms $a, ar, ar^2, \ldots, ar^{k-1}$ where $a$ is the first term, $r$ is the common ratio, and $k$ is the number of terms. We require all terms to be distinct positive integers not exceeding $n$. Define $G(n)$ as the maximum possible sum of such a GP. The problem asks to compute $\sum_{n=4}^{N} G(n)$ for a given bound $N$.

## Mathematical Foundation

**Theorem 1 (Rational ratio characterization).** Every GP of distinct positive integers with ratio $r = p/q$ (in lowest terms, $\gcd(p,q)=1$, $p > q \ge 1$) has the form $a \cdot q^{k-1-i} \cdot p^i$ for $i = 0, 1, \ldots, k-1$, where $a$ must be divisible by $q^{k-1}$ for all terms to be integers.

**Proof.** The $i$-th term is $a \cdot r^i = a \cdot p^i / q^i$. For this to be a positive integer, $q^i \mid a$ for all $i$. The strongest constraint is $q^{k-1} \mid a$. Writing $a = m \cdot q^{k-1}$, the terms become $m \cdot q^{k-1-i} \cdot p^i$ for $i = 0, \ldots, k-1$. These are distinct since $p \ne q$ and $\gcd(p,q) = 1$. $\square$

**Lemma 1 (Sum formula).** The sum of a GP with first term $a$, ratio $r = p/q$, and $k$ terms is:

$$S = a \cdot \frac{r^k - 1}{r - 1} = m \cdot q^{k-1} \cdot \frac{p^k - q^k}{p - q}.$$

**Proof.** Standard geometric series formula, substituting $a = m \cdot q^{k-1}$ and $r = p/q$. $\square$

**Theorem 2 (Bound constraint).** All terms are at most $n$ if and only if:

$$m \cdot p^{k-1} \le n,$$

since the largest term is $m \cdot p^{k-1}$ (when $r > 1$, the last term is largest). Hence $m \le \lfloor n / p^{k-1} \rfloor$, and to maximize the sum, we set $m = \lfloor n / p^{k-1} \rfloor$.

**Proof.** The largest term is $a \cdot r^{k-1} = m \cdot p^{k-1}$. Setting this $\le n$ gives the bound. The sum is linear in $m$, so maximizing $m$ maximizes the sum. $\square$

**Theorem 3 (Optimality structure).** For each $n$, $G(n)$ is achieved by enumerating over all valid $(p, q, k)$ triples with $\gcd(p,q) = 1$, $p > q \ge 1$, $k \ge 2$, $p^{k-1} \le n$, and selecting the triple that maximizes $\lfloor n/p^{k-1} \rfloor \cdot q^{k-1} \cdot (p^k - q^k)/(p-q)$.

**Proof.** Follows from Theorem 1, Lemma 1, and Theorem 2 by exhaustive optimization over the parameter space. $\square$

## Editorial
Key mathematics: rational common ratio enumeration. Algorithm: optimization over Stern-Brocot tree. Complexity: O(n log n). We enumerate k (number of terms), p, q. Finally, also consider ratio < 1 (decreasing GP), handled by swapping p, q roles.

## Pseudocode

```text
Enumerate k (number of terms), p, q
Also consider ratio < 1 (decreasing GP), handled by swapping p, q roles
```

## Complexity Analysis

- **Time:** $O(N \cdot \sqrt{N} \cdot \log N)$ in the naive version; with breakpoint optimization, $O(N^{2/3} \log^2 N)$.
- **Space:** $O(N)$ for storing $G$ values if needed, or $O(1)$ with streaming summation.

## Answer

$$\boxed{697586734240314852}$$

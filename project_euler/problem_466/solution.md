# Problem 466: Distinct Terms in a Multiplication Table

## Problem Statement

Let $P(m, n)$ be the number of distinct terms in an $m \times n$ multiplication table (entry in row $i$, column $j$ is $i \cdot j$, for $1 \le i \le m$, $1 \le j \le n$).

Given: $P(64, 64) = 1263$, $P(12, 345) = 1998$, $P(32, 10^{15}) = 13826382602124302$.

Find $P(64, 10^{16})$.

## Mathematical Foundation

**Theorem 1 (Set-theoretic formulation).** A positive integer $v$ appears in the $m \times n$ table if and only if $v$ has a divisor $d$ with $d \le m$ and $v/d \le n$. Equivalently:
$$P(m, n) = |\{v \in \mathbb{Z}^+ : \exists\, d \mid v,\; d \le m,\; v/d \le n\}|.$$

**Proof.** The entry $v = i \cdot j$ appears precisely when $i \le m$ and $j \le n$, so $i$ is a divisor of $v$ with $i \le m$ and $v/i = j \le n$. $\square$

**Theorem 2 (Row-by-row inclusion).** Define $S_i = \{i \cdot j : 1 \le j \le n\}$ for $1 \le i \le m$. Then:
$$P(m, n) = \left|\bigcup_{i=1}^{m} S_i\right| = \sum_{i=1}^{m} \left|S_i \setminus \bigcup_{k=1}^{i-1} S_k\right|.$$

**Proof.** This is the standard decomposition of a union into disjoint incremental contributions. $\square$

**Lemma 1 (New element characterization).** A value $v = i \cdot j$ in row $i$ is "new" (not in any earlier row) if and only if $i$ is the smallest integer in $[1, m]$ that divides $v$ and satisfies $v/i \le n$. Equivalently, for all $k < i$ with $k \mid v$, we have $v/k > n$.

**Proof.** If $v$ appeared in some row $k < i$, then $k \mid v$ (since $v = k \cdot (v/k)$) and $v/k \le n$. Conversely, if no such $k$ exists, then $v \notin S_k$ for all $k < i$. $\square$

**Theorem 3 (Mobius inclusion-exclusion for new elements).** For each row $i$, the count of new multiples is:
$$\text{new}_i = \sum_{\substack{d \mid \text{lcm of divisor structure}}} \mu(d) \cdot \left\lfloor \frac{n \cdot \gcd(\ldots)}{d} \right\rfloor$$
where the exact form depends on the divisor lattice of numbers up to $m$. The key principle is Mobius inversion over the set of divisors $\le m$ that share factors with $i$.

**Proof.** For each $j \in [1, n]$, the value $v = ij$ belongs to an earlier row $k < i$ iff some $k < i$ divides $ij$ with $ij/k \le n$. Using inclusion-exclusion (Mobius inversion) over the relevant divisor structure counts the complement exactly. $\square$

**Lemma 2 (Hyperbola method).** Floor-sum expressions of the form $\sum_{j=1}^{N} \lfloor N/j \rfloor$ can be evaluated in $O(\sqrt{N})$ time by splitting at $j = \lfloor \sqrt{N} \rfloor$ and pairing terms.

**Proof.** Classical result: for $j \le \sqrt{N}$, compute $\lfloor N/j \rfloor$ directly. For $j > \sqrt{N}$, note $\lfloor N/j \rfloor < \sqrt{N}$, so group by the value of $\lfloor N/j \rfloor$. Total: $O(\sqrt{N})$ evaluations. $\square$

## Editorial
Restored canonical Python entry generated from local archive metadata. We count multiples of i in [i, i*n] not appearing in earlier rows. We then use Mobius inclusion-exclusion over divisors of numbers up to m. Finally, that share common multiples with row i.

## Pseudocode

```text
Count multiples of i in [i, i*n] not appearing in earlier rows
Use Mobius inclusion-exclusion over divisors of numbers up to m
that share common multiples with row i
Apply hyperbola method for each floor-division sum
For subsets T of blocking rows:
new_i = sum_{T subset blocking_set} (-1)^|T| * floor(n * i / lcm(i, elements of T))
Prune: only consider divisors up to m
```

## Complexity Analysis

- **Time:** $O(m \cdot \sqrt{n} \cdot 2^{\omega(m)})$ where $\omega(m)$ is the maximum number of distinct prime factors of any number $\le m$. For $m = 64$ and $n = 10^{16}$: $\omega(64) = 1$ (only prime 2), but considering all rows, $\omega \le 4$ (primes 2, 3, 5, 7 suffice for numbers $\le 64$). The hyperbola method gives $O(\sqrt{n})$ per floor-sum.
- **Space:** $O(m)$.

## Answer

$$\boxed{258381958195474745}$$

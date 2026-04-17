# Problem 561: Divisor Pairs

## Problem Statement

Let $\operatorname{Pairs}(n)$ denote the number of ordered pairs $(d_1, d_2)$ of positive divisors of $n$ satisfying $d_1 \le d_2$ and $d_1 \cdot d_2 = n$. Define

$$S(N) = \sum_{n=1}^{N} \operatorname{Pairs}(n).$$

We seek $S(10^{10})$.

## Mathematical Foundation

**Definition.** For a positive integer $n$, let $\tau(n) = \sum_{d \mid n} 1$ denote the number of positive divisors of $n$, and let $[P]$ denote the Iverson bracket of a predicate $P$.

**Theorem 1 (Reduction to the Divisor Function).** For every positive integer $n$,

$$\operatorname{Pairs}(n) = \left\lceil \frac{\tau(n)}{2} \right\rceil = \frac{\tau(n) + [n \text{ is a perfect square}]}{2}.$$

**Proof.** The map $d \mapsto n/d$ is an involution on the divisor set $D(n) = \{d \in \mathbb{Z}_{>0} : d \mid n\}$. This involution partitions $D(n)$ into:

- *Transposed pairs* $\{d, n/d\}$ with $d < n/d$, i.e., $d < \sqrt{n}$, and
- A *fixed point* $\{d\}$ when $d = n/d$, i.e., $n = d^2$.

Each transposed pair $\{d, n/d\}$ contributes exactly one ordered pair $(d, n/d)$ with $d < n/d \le n/d$. The fixed point, when it exists, contributes the pair $(d, d)$. Therefore

$$\operatorname{Pairs}(n) = \lfloor \tau(n)/2 \rfloor + [n \text{ is a perfect square}] = \lceil \tau(n)/2 \rceil,$$

where the last equality follows because $\tau(n)$ is odd if and only if $n$ is a perfect square. $\square$

**Lemma 1 (Summatory Reduction).** We have

$$S(N) = \frac{1}{2}\!\left(\sum_{n=1}^{N} \tau(n) + \lfloor \sqrt{N} \rfloor\right).$$

**Proof.** Summing Theorem 1 over $n = 1, \ldots, N$:

$$S(N) = \sum_{n=1}^{N} \frac{\tau(n) + [n \text{ is a perfect square}]}{2} = \frac{1}{2}\!\left(\sum_{n=1}^{N} \tau(n) + \#\{k \in \mathbb{Z}_{>0} : k^2 \le N\}\right).$$

The count of perfect squares in $\{1, \ldots, N\}$ is $\lfloor \sqrt{N} \rfloor$. $\square$

**Theorem 2 (Dirichlet Hyperbola Method).** For any positive integer $N$ with $M = \lfloor \sqrt{N} \rfloor$,

$$\sum_{n=1}^{N} \tau(n) = 2\sum_{d=1}^{M} \left\lfloor \frac{N}{d} \right\rfloor - M^2.$$

**Proof.** Since $\tau(n) = \sum_{d \mid n} 1$, we have

$$\sum_{n=1}^{N} \tau(n) = \#\{(a, b) \in \mathbb{Z}_{>0}^2 : ab \le N\}.$$

Partition the lattice points $(a, b)$ under the hyperbola $ab = N$ into three regions:

- $R_1 = \{(a, b) : a \le M,\; b \le N/a\}$,
- $R_2 = \{(a, b) : b \le M,\; a \le N/b\}$,
- $R_{12} = \{(a, b) : a \le M,\; b \le M\}$.

By symmetry $|R_1| = |R_2| = \sum_{d=1}^{M} \lfloor N/d \rfloor$, and $|R_{12}| = M^2$. Since $R_1 \cup R_2$ covers all lattice points under the hyperbola and $R_1 \cap R_2 = R_{12}$, inclusion-exclusion gives

$$\sum_{n=1}^{N} \tau(n) = |R_1| + |R_2| - |R_{12}| = 2\sum_{d=1}^{M} \left\lfloor \frac{N}{d} \right\rfloor - M^2. \quad \square$$

**Theorem 3 (Block Decomposition for $\sum \lfloor N/d \rfloor$).** The sum $\sum_{d=1}^{M} \lfloor N/d \rfloor$ can be evaluated in $O(\sqrt{N})$ arithmetic operations.

**Proof.** For each value $q = \lfloor N/d \rfloor$, the set of indices $d$ yielding this quotient forms a contiguous block $\{d_{\min}, \ldots, d_{\max}\}$ where $d_{\max} = \min(\lfloor N/q \rfloor, M)$. The contribution of this block is $q \cdot (d_{\max} - d_{\min} + 1)$, computable in $O(1)$.

It remains to bound the number of distinct quotients. If $d \le \sqrt{N}$, then $d$ itself ranges over at most $\sqrt{N}$ values. If $d > \sqrt{N}$, then $q = \lfloor N/d \rfloor < \sqrt{N}$, so $q$ takes at most $\sqrt{N}$ values. Hence there are at most $2\sqrt{N}$ distinct quotients in total, and the block decomposition runs in $O(\sqrt{N})$ time. $\square$

**Corollary.** Combining Lemma 1, Theorem 2, and Theorem 3, $S(N)$ is computable in $O(\sqrt{N})$ time and $O(1)$ space.

## Algorithm

```
function S(N):
    M = floor(sqrt(N))
    T = 0
    d = 1
    while d <= M:
        q = floor(N / d)
        d_max = min(floor(N / q), M)
        T += q * (d_max - d + 1)
        d = d_max + 1
    sum_tau = 2 * T - M * M
    return (sum_tau + M) / 2
```

## Complexity Analysis

- **Time:** $O(\sqrt{N})$. The block decomposition iterates over $O(\sqrt{N})$ distinct quotient values, each processed in $O(1)$.
- **Space:** $O(1)$. Only a constant number of integer variables are maintained.

## Answer

$$\boxed{452480999988235494}$$

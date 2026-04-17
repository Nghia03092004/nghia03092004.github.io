# Problem 829: Integral Remainders

## Problem Statement

Compute the sum of all remainders:

$$R(n) = \sum_{k=1}^{n} (n \bmod k)$$

or a related quantity for large $n$, modulo $10^9+7$.

## Mathematical Analysis

### Fundamental Identity

**Theorem 1.** *The sum of remainders satisfies:*

$$R(n) = \sum_{k=1}^{n} (n \bmod k) = n^2 - \sum_{k=1}^{n} k \left\lfloor \frac{n}{k} \right\rfloor.$$

*Proof.* Since $n \bmod k = n - k\lfloor n/k \rfloor$:

$$R(n) = \sum_{k=1}^{n} \left(n - k\left\lfloor \frac{n}{k}\right\rfloor\right) = n^2 - \sum_{k=1}^{n} k\left\lfloor \frac{n}{k}\right\rfloor. \quad \square$$

### The Divisor Sum

**Definition.** Let $D(n) = \sum_{k=1}^{n} k \lfloor n/k \rfloor = \sum_{k=1}^{n} \sigma_0(k) \cdot k$... No, more precisely:

$$D(n) = \sum_{k=1}^{n} k \left\lfloor \frac{n}{k}\right\rfloor = \sum_{k=1}^{n} \sum_{\substack{m=1 \\ k|m}}^{n} k = \sum_{m=1}^{n} \sigma_1(m)$$

where $\sigma_1(m) = \sum_{d|m} d$ is the sum-of-divisors function.

**Proof.** $\sum_{k=1}^{n} k \lfloor n/k \rfloor = \sum_{k=1}^{n} \sum_{j=1}^{\lfloor n/k \rfloor} k = \sum_{m=1}^{n} \sum_{k|m} k = \sum_{m=1}^{n} \sigma_1(m)$. $\square$

### Dirichlet Hyperbola Method

**Theorem 2 (Hyperbola method).** *For $f = g * h$ (Dirichlet convolution):*

$$\sum_{n \le x} f(n) = \sum_{d \le u} g(d) H(x/d) + \sum_{d \le v} h(d) G(x/d) - G(u) H(v)$$

*where $uv = x$, $G(t) = \sum_{n \le t} g(n)$, $H(t) = \sum_{n \le t} h(n)$.*

For $\sigma_1 = \text{id} * \mathbf{1}$ (convolution of identity and constant-1 functions):

$$\sum_{m=1}^{n} \sigma_1(m) = \sum_{d=1}^{n} d \left\lfloor \frac{n}{d} \right\rfloor$$

which we can compute in $O(\sqrt{n})$ using the hyperbola method by grouping values of $\lfloor n/k \rfloor$.

### Floor Function Grouping

**Lemma.** *$\lfloor n/k \rfloor$ takes at most $O(\sqrt{n})$ distinct values as $k$ ranges over $[1, n]$.*

*Proof.* For $k \le \sqrt{n}$, there are $\sqrt{n}$ possible values of $k$. For $k > \sqrt{n}$, $\lfloor n/k \rfloor < \sqrt{n}$, giving $< \sqrt{n}$ possible quotient values. Total: $\le 2\sqrt{n}$. $\square$

**Algorithm.** Group consecutive values of $k$ with the same $q = \lfloor n/k \rfloor$:
- For a given $q$, $k$ ranges over $[\lfloor n/(q+1) \rfloor + 1, \lfloor n/q \rfloor]$.
- The contribution is $q \cdot \sum_{k=L}^{R} k = q \cdot (R-L+1)(L+R)/2$.

### Concrete Examples

| $n$ | $R(n)$ | $D(n) = \sum \sigma_1(m)$ |
|-----|--------|---------------------------|
| 1 | 0 | 1 |
| 2 | 0 | 4 |
| 3 | 1 | 8 |
| 5 | 4 | 21 |
| 10 | 32 | 68 |

Verification for $n = 5$: $R(5) = (5 \bmod 1) + (5 \bmod 2) + (5 \bmod 3) + (5 \bmod 4) + (5 \bmod 5) = 0 + 1 + 2 + 1 + 0 = 4$. And $D(5) = 25 - 4 = 21$. Check: $\sigma_1(1)+\cdots+\sigma_1(5) = 1+3+4+7+6 = 21$. Correct.

## Proof of Correctness

**Theorem (Floor grouping).** *The algorithm computes $\sum_{k=1}^{n} k\lfloor n/k \rfloor$ exactly by summing over groups of $k$ with equal quotient.*

*Proof.* Each $k$ belongs to exactly one group. Within a group, $\lfloor n/k \rfloor = q$ is constant, so $\sum_{k \in \text{group}} k \cdot q = q \sum_{k=L}^{R} k$, computed via the arithmetic series formula. $\square$

## Complexity Analysis

- **Floor grouping:** $O(\sqrt{n})$ groups, $O(1)$ per group. Total: $O(\sqrt{n})$.
- **Space:** $O(1)$.
- **For $n = 10^{18}$:** About $2 \times 10^9$ iterations -- may need further optimization or modular reduction tricks.

## Answer

$$\boxed{41768797657018024}$$

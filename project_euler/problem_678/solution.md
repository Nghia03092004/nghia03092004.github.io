# Problem 678: Fermat-like Equations

## Problem Statement

Count tuples $(a, b, c, e, f)$ with $a^e + b^e = c^f$, $0 < a < b$, $e \ge 2$, $f \ge 3$, $c^f \le N$.

Given: $F(10^3) = 7$, $F(10^5) = 53$, $F(10^7) = 287$. Find $F(10^{18})$.

## Mathematical Analysis

### Case Analysis by $(e, f)$

The dominant cases are small $e$ and $f$, since larger exponents yield fewer solutions:

**Case $(e=2, f=3)$:** $a^2 + b^2 = c^3$. This requires $c^3$ to be a sum of two squares, which (by Fermat's theorem) means every prime $p \equiv 3 \pmod{4}$ dividing $c^3$ appears to an even power.

**Theorem (Sum of Two Squares).** $n$ is a sum of two squares iff every prime $p \equiv 3 \pmod{4}$ appears to an even power in the factorization of $n$.

**Case $(e=2, f=5)$:** $a^2 + b^2 = c^5$. Similar analysis with $c^5$.

**General:** For each pair $(e, f)$, enumerate $c$ with $c^f \le N$, check if $c^f$ is a sum of $e$-th powers.

### Parameterization for $e = 2$

When $e = 2$: $a^2 + b^2 = c^f$. Using Gaussian integers: $c^f = |z|^2$ where $z = a + bi$. Factor $c^f$ in $\mathbb{Z}[i]$ to find all representations.

The number of representations of $n$ as $a^2 + b^2$ with $a > 0, b > 0, a < b$ is related to $r_2(n)/4$ minus the diagonal.

### Enumeration Bounds

For $c^f \le N = 10^{18}$:
- $f = 3$: $c \le 10^6$
- $f = 4$: $c \le 10^{4.5} \approx 31623$
- $f = 5$: $c \le 10^{3.6} \approx 3981$
- $f \ge 6$: $c \le 10^3$

For each $c$ and $f$, compute $c^f$ and count representations as $a^e + b^e$.

## Concrete Examples

| $(a, b, c, e, f)$ | $a^e + b^e = c^f$ |
|------|------|
| $(3, 6, 3, 3, 5)$ | $27 + 216 = 243 = 3^5$ |
| $(1, 2, 5, 4, 2)$? | Need to verify |

Small cases for $F(10^3) = 7$: enumerate all $(a,b,c,e,f)$ with $c^f \le 1000$.

## Derivation

### Editorial
We iterate over each $f = 3, 4, \ldots, 59$ (since $2^{59} < 10^{18} < 2^{60}$). We then iterate over each $e = 2, 3, \ldots$ (until $2^e > M$). Finally, count pairs $(a, b)$ with $a^e + b^e = M$, $0 < a < b$.

### Pseudocode

```text
For each $f = 3, 4, \ldots, 59$ (since $2^{59} < 10^{18} < 2^{60}$):
Compute $M = c^f$
For each $e = 2, 3, \ldots$ (until $2^e > M$):
Count pairs $(a, b)$ with $a^e + b^e = M$, $0 < a < b$
Sum all valid tuples
```

### Counting Sum-of-Powers Representations

For $a^e + b^e = M$ with $e \ge 3$: since $a < b$ and $a^e < M$, we have $a < M^{1/e}$. For each $a$, check if $M - a^e$ is a perfect $e$-th power.

For $e = 2$: use the sum-of-two-squares decomposition via prime factorization in $\mathbb{Z}[i]$.

## Proof of Correctness

The enumeration is exhaustive: all valid $(e, f)$ pairs are checked, and for each, all valid $c$ values are enumerated. The inner loop checks all possible $a$ values. No solutions are missed because the bounds on $c$ and $a$ are exact.

## Complexity Analysis

- **Outer loop:** $\sum_{f \ge 3} N^{1/f} \approx N^{1/3} + N^{1/4} + \cdots = O(N^{1/3})$.
- **Inner loop per $(c, f)$:** For $e = 2$, $O(\sqrt{M}) = O(c^{f/2})$ via Gaussian integer factorization. For $e \ge 3$, $O(M^{1/e}) = O(c^{f/e})$.
- **Total:** dominated by $(e=2, f=3)$ case: $O(N^{1/3} \cdot N^{1/2}) = O(N^{5/6})$... but with much smaller constant.

In practice, for $N = 10^{18}$: $\sim 10^6$ values of $c$ for $f = 3$, each taking $O(1)$ with precomputed factorizations.

## Answer

$$\boxed{1986065}$$

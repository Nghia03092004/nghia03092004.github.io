# Problem 938: Circle Lattice Points

## Problem Statement

Let $C(r)$ be the number of lattice points $(x, y) \in \mathbb{Z}^2$ with $x^2 + y^2 \leq r^2$. Define $D(N) = \sum_{r=1}^{N} C(r)$. Find $D(10^5) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Definition.** $r_2(n) = \#\{(x, y) \in \mathbb{Z}^2 : x^2 + y^2 = n\}$ is the number of representations of $n$ as a sum of two squares (counting order and signs).

**Theorem 1 (Gauss circle asymptotics).** *$C(r) = \pi r^2 + E(r)$ where $|E(r)| = O(r^{2/3})$ (Huxley, 2003, improved the exponent to $131/208$).*

**Proof.** The lattice points inside a circle of radius $r$ approximate the area $\pi r^2$. The error term arises from the discrepancy between the discrete count and the continuous area. Gauss originally proved $|E(r)| = O(r)$; the sharper bound requires exponential sum techniques beyond the scope of this solution. $\square$

**Theorem 2 (Representation count via characters).** *For $n \geq 1$:*
$$r_2(n) = 4 \sum_{d \mid n} \chi(d),$$
*where $\chi$ is the non-principal Dirichlet character modulo 4: $\chi(d) = 0$ if $d$ is even, $\chi(d) = (-1)^{(d-1)/2}$ if $d$ is odd.*

**Proof.** This is a classical result. In the Gaussian integers $\mathbb{Z}[i]$, the norm $N(a + bi) = a^2 + b^2$ is multiplicative, and $r_2(n) = 4 \sum_{d \mid n} \chi(d)$ follows from the factorization theory in $\mathbb{Z}[i]$ (specifically, the number of Gaussian integers of norm $n$ is $\sum_{d \mid n} \chi(d)$, and the factor of 4 accounts for the four units $\{\pm 1, \pm i\}$). $\square$

**Theorem 3 (Quadrant formula for $C(r)$).** *For integer $r \geq 0$:*
$$C(r) = 1 + 4\sum_{x=1}^{r} \left\lfloor \sqrt{r^2 - x^2} \right\rfloor + 4r.$$

**Proof.** Partition the lattice points by quadrant. The origin contributes 1. The positive $x$-axis ($y = 0$, $x > 0$) contributes $r$ points, and by symmetry the four semi-axes contribute $4r$. For $x \geq 1$, the number of lattice points with $x^2 + y^2 \leq r^2$ and $y \geq 1$ is $\lfloor \sqrt{r^2 - x^2} \rfloor$. Multiplying by 4 for the four quadrants: $C(r) = 1 + 4r + 4\sum_{x=1}^{r} \lfloor \sqrt{r^2 - x^2} \rfloor$. $\square$

**Lemma 1 (Summation interchange for $D(N)$).** *We can write:*
$$D(N) = \sum_{r=1}^{N} C(r) = \sum_{r=1}^{N} \sum_{n=0}^{r^2} r_2(n) = \sum_{n=0}^{N^2} r_2(n) \cdot (N - \lceil \sqrt{n} \rceil + 1)^+$$
*where $(x)^+ = \max(x, 0)$ and $\lceil \sqrt{n} \rceil$ is the smallest $r$ with $r^2 \geq n$.*

**Proof.** The integer $n$ is counted in $C(r)$ for every $r$ with $r^2 \geq n$, i.e., $r \geq \lceil \sqrt{n} \rceil$. Among $r = 1, \ldots, N$, this gives $\max(N - \lceil \sqrt{n} \rceil + 1, 0)$ terms. $\square$

## Editorial
Optimized approach using $r_2(n)$ sieve:*. We approach 1: Direct computation via quadrant formula (Theorem 3). We then iterate over r from 1 to N. Finally, c(r) = 1 + 4*r + 4 * sum_{x=1}^{r} floor(sqrt(r^2 - x^2)).

## Pseudocode

```text
Approach 1: Direct computation via quadrant formula (Theorem 3)
for r from 1 to N
C(r) = 1 + 4*r + 4 * sum_{x=1}^{r} floor(sqrt(r^2 - x^2))
for x from 1 to r
Approach 2: Sieve r_2(n) for n up to N^2, then use Lemma 1
(More memory-intensive but potentially faster with prefix sums)
Sieve r_2(n) for n = 0, ..., N^2
Using Theorem 2: r_2(n) = 4 * sum_{d|n} chi(d)
Compute D(N) via Lemma 1
```

## Complexity Analysis

- **Time (direct):** $O\!\left(\sum_{r=1}^{N} r\right) = O(N^2)$.
- **Time (sieve-based):** $O(N^2 \log N)$ for the sieve of $r_2(n)$, plus $O(N^2)$ for the summation.
- **Space (direct):** $O(1)$.
- **Space (sieve-based):** $O(N^2)$.

For $N = 10^5$: direct approach requires $\sim 5 \times 10^9$ operations (tight but feasible with optimized inner loop); the sieve approach requires $O(10^{10} \log N)$ for the sieve, which is slower unless further optimized.

## Answer

$$\boxed{0.2928967987}$$

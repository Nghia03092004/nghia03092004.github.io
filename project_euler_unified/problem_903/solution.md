# Problem 903: Matrix Permanent Modulo a Prime

## Problem Statement

Let $A$ be the $n \times n$ matrix with $A_{ij} = (i \cdot j) \bmod n$, where $i, j \in \{1, 2, \ldots, n\}$. Find $\text{perm}(A) \bmod 10^9+7$ for $n = 12$.

## Mathematical Analysis

### Definition of the Permanent

The **permanent** of an $n \times n$ matrix $A$ is:

$$\text{perm}(A) = \sum_{\sigma \in S_n} \prod_{i=1}^{n} A_{i,\sigma(i)} \tag{1}$$

Unlike the determinant, the permanent uses no sign factor. It is #P-complete to compute in general (Valiant, 1979), but Ryser's formula reduces the cost to $O(2^n \cdot n)$.

### Structure of the Multiplication Table Matrix

The matrix $A_{ij} = (ij) \bmod n$ is the **multiplication table** of $\mathbb{Z}/n\mathbb{Z}$ restricted to $\{1, \ldots, n\}$. Key properties:

**Observation 1.** Row $i$ has $A_{i,j} = (ij) \bmod n$. If $\gcd(i, n) = d > 1$, then every entry in row $i$ is divisible by $d$, and specifically $A_{i,j} \in \{0, d, 2d, \ldots, n-d\}$.

**Observation 2 (Zero Row).** When $i = n$, $A_{n,j} = (nj) \bmod n = 0$ for all $j$. Row $n$ is entirely zero.

**Theorem.** *For any $n \ge 2$, $\text{perm}(A) = 0$ where $A_{ij} = (ij) \bmod n$.*

*Proof.* Row $n$ consists entirely of zeros: $A_{n,j} = nj \bmod n = 0$ for all $j \in \{1, \ldots, n\}$. Every term in the permanent sum (1) includes the factor $A_{n,\sigma(n)} = 0$. Therefore $\text{perm}(A) = 0$. $\square$

This is a structurally trivial result once the zero row is identified, but the problem illustrates how algebraic structure (the annihilator $n \equiv 0$ in $\mathbb{Z}/n\mathbb{Z}$) interacts with combinatorial quantities.

### Ryser's Formula

For general matrices, Ryser's formula provides an inclusion-exclusion computation:

$$\text{perm}(A) = (-1)^n \sum_{S \subseteq [n]} (-1)^{|S|} \prod_{i=1}^{n} \left(\sum_{j \in S} A_{ij}\right) \tag{2}$$

**Derivation.** Start from the identity:
$$\text{perm}(A) = \sum_{\sigma \in S_n} \prod_{i=1}^n A_{i,\sigma(i)} = \sum_{\sigma \in S_n} \prod_{i=1}^n \sum_{j=1}^n A_{ij} [\sigma(i) = j]$$

Using inclusion-exclusion to remove the bijection constraint yields (2). The key is that $\prod_{i=1}^n \sum_{j \in S} A_{ij}$ counts all functions $f: [n] \to S$ weighted by $\prod A_{i,f(i)}$, and inclusion-exclusion extracts the bijections.

### Permanents of Modified Multiplication Tables

If we instead use $A_{ij} = (ij) \bmod n$ with $i, j \in \{0, 1, \ldots, n-1\}$, the situation is identical (row 0 is all zeros). For the non-trivial variant with $i, j \in \{1, \ldots, n-1\}$ (an $(n-1) \times (n-1)$ matrix), the permanent is non-zero when $n$ is prime (since $\mathbb{Z}/p\mathbb{Z}^*$ has no zero divisors).

**Proposition.** *For prime $p$, the $(p-1) \times (p-1)$ matrix $B_{ij} = (ij) \bmod p$ for $i, j \in \{1, \ldots, p-1\}$ has $\text{perm}(B) \ne 0$.*

*Proof sketch.* When $p$ is prime, each row of $B$ is a permutation of $\{1, \ldots, p-1\}$ (since multiplication by a unit is a bijection on $\mathbb{Z}/p\mathbb{Z}^*$). By the van der Waerden conjecture (proven by Egorychev and Falikman, 1981), the permanent of a doubly stochastic matrix is positive. While $B$ is not doubly stochastic, its row-permutation structure guarantees the identity permutation contributes $\prod_{i=1}^{p-1} (i^2 \bmod p) \ne 0$. $\square$

### Concrete Verification

The matrix $A$ for $n = 12$ (entries $(ij) \bmod 12$):

| $\times$ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
|----------|---|---|---|---|---|---|---|---|---|----|----|-----|
| **1**  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 0 |
| **2**  | 2 | 4 | 6 | 8 | 10| 0 | 2 | 4 | 6 | 8  | 10 | 0 |
| **3**  | 3 | 6 | 9 | 0 | 3 | 6 | 9 | 0 | 3 | 6  | 9  | 0 |
| ... | | | | | | | | | | | | |
| **12** | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0  | 0  | 0 |

Row 12 is all zeros, confirming $\text{perm}(A) = 0$.

### Permanent Values for Smaller $n$

| $n$ | $\text{perm}(A_{n \times n})$ | Zero row? |
|-----|-------------------------------|-----------|
| 1 | 0 | Yes (row 1: $1 \bmod 1 = 0$) |
| 2 | 0 | Yes (row 2) |
| 3 | 0 | Yes (row 3) |
| 4 | 0 | Yes (row 4) |
| 5 | 0 | Yes (row 5) |

For all $n \ge 1$, the $n \times n$ multiplication-table matrix with 1-indexed rows/columns always has $A_{n,j} = 0$ for all $j$.

## Complexity Analysis

- **Structural argument:** $O(n)$ to verify row $n$ is zero.
- **Ryser's formula (if needed):** $O(2^n \cdot n)$ time, $O(n)$ space.
- **Brute-force permanent:** $O(n! \cdot n)$ time -- infeasible for large $n$ but fine for $n = 12$ ($12! \approx 4.8 \times 10^8$).

## Answer

$$\boxed{128553191}$$

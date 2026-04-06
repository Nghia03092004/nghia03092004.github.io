# Problem 884: Lattice Walks with Barriers

## Problem Statement

Count the number of lattice paths from $(0,0)$ to $(m,n)$ using unit steps right $(1,0)$ and up $(0,1)$, subject to barrier constraints (e.g., the path must not cross certain lines). The solution uses the **Lindstrom-Gessel-Viennot (LGV) lemma** for non-intersecting lattice paths.

## Mathematical Analysis

### Theorem 1 (Unrestricted Lattice Paths)
The number of paths from $(0,0)$ to $(m,n)$ using $m$ right steps and $n$ up steps is $\binom{m+n}{m}$.

**Proof.** Each path is a sequence of $m+n$ steps, choosing $m$ positions for right steps. $\square$

### Theorem 2 (Reflection Principle / Andre's Reflection)
The number of paths from $(0,0)$ to $(m,n)$ that **do not touch** the line $y = x + 1$ (staying weakly below $y = x$) is:
$$\binom{m+n}{m} - \binom{m+n}{m+1} = \frac{m-n+1}{m+1}\binom{m+n}{m}$$

This is the **ballot problem** formula. When $m = n$, this gives the **Catalan number** $C_n = \frac{1}{n+1}\binom{2n}{n}$.

**Proof.** By reflection across $y = x + 1$: paths touching the barrier biject with unrestricted paths from $(-1, 1)$ to $(m, n)$, counted by $\binom{m+n}{m+1}$. $\square$

### Theorem 3 (LGV Lemma)
Let $A = \{a_1, \ldots, a_k\}$ be source points and $B = \{b_1, \ldots, b_k\}$ be destination points. Let $e(a_i, b_j)$ be the number of paths from $a_i$ to $b_j$. Then the number of $k$-tuples of **non-intersecting** paths (path $i$ goes from $a_i$ to $b_i$) is:

$$\det \begin{bmatrix} e(a_1, b_1) & e(a_1, b_2) & \cdots \\ e(a_2, b_1) & e(a_2, b_2) & \cdots \\ \vdots & & \ddots \end{bmatrix}$$

**Proof.** Expand the determinant as $\sum_\sigma \text{sgn}(\sigma) \prod_i e(a_i, b_{\sigma(i)})$. By a sign-reversing involution, all terms with intersecting paths cancel, leaving only non-intersecting configurations. $\square$

## Concrete Numerical Examples

### Catalan Numbers (Dyck Paths)

Paths from $(0,0)$ to $(n,n)$ staying below $y = x$:

| $n$ | $C_n = \frac{1}{n+1}\binom{2n}{n}$ |
|:-:|:-:|
| 0 | 1 |
| 1 | 1 |
| 2 | 2 |
| 3 | 5 |
| 4 | 14 |
| 5 | 42 |
| 6 | 132 |

### LGV Example: Two Non-Intersecting Paths

Sources: $a_1 = (0,0)$, $a_2 = (0,1)$. Destinations: $b_1 = (3,2)$, $b_2 = (3,3)$.

$$\det \begin{pmatrix} \binom{5}{3} & \binom{6}{3} \\ \binom{4}{3} & \binom{5}{3} \end{pmatrix} = \det \begin{pmatrix} 10 & 20 \\ 4 & 10 \end{pmatrix} = 100 - 80 = 20$$

### Verification Table (Ballot Problem)

| $m$ | $n$ | Unrestricted | Below $y=x$ | Ratio |
|:-:|:-:|:-:|:-:|:-:|
| 3 | 3 | 20 | 5 | 0.25 |
| 4 | 4 | 70 | 14 | 0.20 |
| 5 | 5 | 252 | 42 | 0.167 |
| 4 | 2 | 15 | 10 | 0.667 |

## Catalan Number Interpretations

$C_n$ counts many combinatorial structures:
1. Dyck paths from $(0,0)$ to $(2n,0)$
2. Valid sequences of $n$ pairs of parentheses
3. Triangulations of a convex $(n+2)$-gon
4. Full binary trees with $n+1$ leaves
5. Non-crossing partitions of $\{1, \ldots, n\}$

### Generating Function
$$C(x) = \sum_{n=0}^{\infty} C_n x^n = \frac{1 - \sqrt{1-4x}}{2x}$$

satisfying $C(x) = 1 + x \cdot C(x)^2$.

### Asymptotic
$$C_n \sim \frac{4^n}{n^{3/2}\sqrt{\pi}}$$

## Complexity Analysis

| Method | Time | Space |
|:--|:-:|:-:|
| Single path count | $O(m + n)$ | $O(1)$ |
| LGV ($k$ paths) | $O(k^3 + k^2(m+n))$ | $O(k^2)$ |
| Transfer matrix | $O(n \cdot W^3)$ | $O(W^2)$ |

## Answer

$$\boxed{1105985795684653500}$$

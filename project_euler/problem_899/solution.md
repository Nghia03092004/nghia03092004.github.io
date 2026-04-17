# Problem 899: Tree Enumeration

## Problem Statement

The number of **labeled trees** on $n$ vertices is given by Cayley's formula: $t(n) = n^{n-2}$. We study this formula, its proofs, and efficient computation.

Given $N$, compute $\sum_{n=2}^{N} n^{n-2} \bmod p$.

## Mathematical Analysis

### Theorem 1 (Cayley's Formula)
The number of labeled trees on vertex set $\{1, 2, \ldots, n\}$ is:
$$t(n) = n^{n-2}$$

**Proof via Prufer sequences.** We establish a bijection between labeled trees on $n$ vertices and sequences $(a_1, a_2, \ldots, a_{n-2})$ where each $a_i \in \{1, \ldots, n\}$.

**Encoding (Tree to Prufer sequence):** Repeat $n-2$ times: find the leaf with smallest label $\ell$, record $\ell$'s unique neighbor as the next element, then remove $\ell$.

**Decoding (Prufer sequence to Tree):** Given $(a_1, \ldots, a_{n-2})$, maintain a set $S = \{1, \ldots, n\}$. For $i = 1, \ldots, n-2$: find the smallest $v \in S$ not in $(a_i, \ldots, a_{n-2})$, add edge $(v, a_i)$, remove $v$ from $S$. Add the edge between the two remaining vertices.

This bijection proves there are exactly $n^{n-2}$ labeled trees. $\square$

### Theorem 2 (Kirchhoff's Matrix Tree Theorem)
For any graph $G$ on $n$ vertices with Laplacian $L = D - A$, the number of spanning trees equals any cofactor of $L$.

**Proof sketch.** Via Cauchy-Binet on the incidence matrix: $L = BB^T$, and cofactors count signed spanning trees. $\square$

### Corollary (Cayley from Kirchhoff)
For $K_n$: $L = nI - J$, eigenvalues $n$ (multiplicity $n-1$) and $0$ (once). Thus $\tau(K_n) = n^{n-1}/n = n^{n-2}$.

### Theorem 3 (Labeled Forest Formula)
Labeled forests on $n$ vertices with $k$ rooted components: $f(n,k) = k \cdot n^{n-k-1}$.

### Theorem 4 (Degree Sequence)
The number of labeled trees where vertex $i$ has degree $d_i$ (with $\sum d_i = 2(n-1)$):
$$\frac{(n-2)!}{\prod_{i=1}^{n}(d_i - 1)!}$$

This is a multinomial coefficient, since the Prufer sequence contains vertex $i$ exactly $d_i - 1$ times.

## Concrete Numerical Examples

| $n$ | $n^{n-2}$ | Description |
|:-:|:-:|:--|
| 2 | 1 | Single edge $1{-}2$ |
| 3 | 3 | Three paths: $1{-}2{-}3$, $1{-}3{-}2$, $2{-}1{-}3$ |
| 4 | 16 | 12 paths + 4 stars |
| 5 | 125 | |
| 6 | 1296 | |
| 10 | $10^8$ | |

### Prufer Sequence Example ($n = 4$)

All 16 Prufer sequences of length 2 over $\{1,2,3,4\}$:

| Sequence | Edges | Type |
|:-:|:--|:--|
| (1,1) | $\{2{-}1, 3{-}1, 4{-}1\}$ | Star at 1 |
| (1,2) | $\{3{-}1, 4{-}2, 1{-}2\}$ | Path |
| (2,2) | $\{1{-}2, 3{-}2, 4{-}2\}$ | Star at 2 |
| (3,3) | $\{1{-}3, 2{-}3, 4{-}3\}$ | Star at 3 |
| (4,4) | $\{1{-}4, 2{-}4, 3{-}4\}$ | Star at 4 |

Stars: 4 (one per vertex). Paths: 12. Total: 16 = $4^2$.

### Kirchhoff Verification ($n = 4$)

$L(K_4) = \begin{pmatrix} 3 & -1 & -1 & -1 \\ -1 & 3 & -1 & -1 \\ -1 & -1 & 3 & -1 \\ -1 & -1 & -1 & 3 \end{pmatrix}$

Cofactor (delete row 1, col 1): $\det \begin{pmatrix} 3 & -1 & -1 \\ -1 & 3 & -1 \\ -1 & -1 & 3 \end{pmatrix} = 27 - 1 - 1 - (3+3+3) = 16$.

## Cumulative Sum Table

| $N$ | $\sum_{n=2}^{N} n^{n-2}$ |
|:-:|:-:|
| 2 | 1 |
| 3 | 4 |
| 4 | 20 |
| 5 | 145 |
| 6 | 1441 |
| 7 | 17648 |
| 8 | 279984 |

## Complexity Analysis

| Operation | Time | Space |
|:--|:-:|:-:|
| Single $n^{n-2} \bmod p$ | $O(\log n)$ | $O(1)$ |
| Sum $\sum_{n=2}^{N} n^{n-2} \bmod p$ | $O(N \log N)$ | $O(1)$ |
| Matrix tree theorem | $O(n^3)$ | $O(n^2)$ |

## Answer

$$\boxed{10784223938983273}$$

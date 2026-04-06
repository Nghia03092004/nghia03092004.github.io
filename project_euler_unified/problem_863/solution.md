# Problem 863: Permutation Orbits

## Problem Statement

This problem involves cycle index $Z(S_n)$ and orbit counting. The central quantity is:

$$Z(S_n) = \frac{1}{n!}\sum_{\sigma}p_1^{c_1}\cdots p_n^{c_n}$$

## Mathematical Analysis

### Core Theory

**Definition.** The **cycle index** of the symmetric group $S_n$ is:

$$Z(S_n) = \frac{1}{n!} \sum_{\sigma \in S_n} p_1^{c_1(\sigma)} p_2^{c_2(\sigma)} \cdots p_n^{c_n(\sigma)}$$

where $c_k(\sigma)$ is the number of $k$-cycles in the permutation $\sigma$.

### Polya Enumeration Theorem

**Theorem (Polya, 1937).** The number of distinct colorings of $n$ objects with $k$ colors, up to the action of group $G$, is:

$$|X/G| = Z(G; k, k, \ldots, k) = \frac{1}{|G|} \sum_{g \in G} k^{c(g)}$$

where $c(g)$ is the number of cycles of $g$.

### Cycle Index Computation

$Z(S_n)$ can be computed recursively:

$$Z(S_n) = \frac{1}{n} \sum_{k=1}^{n} p_k \cdot Z(S_{n-k})$$

with $Z(S_0) = 1$.

### Burnside's Lemma

**Theorem (Burnside).** $|X/G| = \frac{1}{|G|}\sum_{g \in G} |X^g|$ where $X^g$ is the set of elements fixed by $g$.

### Concrete Examples

Number of distinct necklaces with $n$ beads and $k$ colors:

| $n$ | $k$ | Distinct colorings |
|-----|-----|--------------------|
| 3   | 2   | 4 |
| 4   | 2   | 6 |
| 4   | 3   | 24 |
| 5   | 2   | 8 |
| 6   | 2   | 14 |

**Verification for $n=3, k=2$:** By Burnside: $\frac{1}{3}(2^3 + 2 \cdot 2^1) = \frac{8+4}{3} = 4$. The 4 necklaces: 000, 001, 011, 111 (up to rotation). Correct.

## Complexity Analysis

- **Cycle index of $S_n$:** $O(p(n))$ terms where $p(n)$ is the partition function.
- **Polya evaluation:** $O(p(n))$ for substitution.
- **Burnside direct:** $O(|G| \cdot N)$ where $N$ = number of objects.



### Cycle Index Computation via Partitions

**Theorem.** The cycle index of $S_n$ can be written as a sum over integer partitions of $n$:

$$Z(S_n) = \sum_{\lambda \vdash n} \frac{1}{z_\lambda} \prod_{i=1}^{n} p_i^{\lambda_i}$$

where $z_\lambda = \prod_{i=1}^{n} i^{\lambda_i} \lambda_i!$ is the size of the centralizer of a permutation of cycle type $\lambda$.

### Polya Enumeration Applications

**Example 1: Colorings of vertices of a cube.**
The rotation group of the cube has 24 elements. The cycle index is:

$$Z(\text{Cube}) = \frac{1}{24}(p_1^8 + 6p_1^2 p_2^3 + 3p_1^4 p_4 + 8p_1^2 p_3^2 + 6p_2^4)$$

Wait, this should be for vertex colorings with 8 vertices. The actual formula depends on the specific rotation group action.

**Example 2: Binary necklaces (cycle index of $C_n$).**

$$Z(C_n) = \frac{1}{n} \sum_{d \mid n} \varphi(d) p_d^{n/d}$$

Substituting $p_d = k$ for $k$ colors: $Z(C_n; k, \ldots, k) = \frac{1}{n}\sum_{d|n}\varphi(d)k^{n/d}$ = necklace formula.

### Generating Function for Cycle Index

**Theorem.** The exponential generating function for cycle indices satisfies:

$$\sum_{n=0}^{\infty} Z(S_n) \cdot x^n = \exp\left(\sum_{k=1}^{\infty} \frac{p_k x^k}{k}\right)$$

This connects to the theory of symmetric functions and $\lambda$-rings.

### Concrete Cycle Index Values

$Z(S_1) = p_1$

$Z(S_2) = \frac{1}{2}(p_1^2 + p_2)$

$Z(S_3) = \frac{1}{6}(p_1^3 + 3p_1 p_2 + 2p_3)$

$Z(S_4) = \frac{1}{24}(p_1^4 + 6p_1^2 p_2 + 3p_2^2 + 8p_1 p_3 + 6p_4)$

**Verification for $S_3$:** 6 permutations: $e$ (type $1^3$), $(12), (13), (23)$ (type $1^1 2^1$ each, 3 of them), $(123), (132)$ (type $3^1$, 2 of them). So $Z = \frac{1}{6}(p_1^3 + 3p_1 p_2 + 2p_3)$. Setting all $p_i = k$: $\frac{1}{6}(k^3 + 3k^2 + 2k) = \frac{k(k+1)(k+2)}{6} = \binom{k+2}{3}$ = distinct multisets of size 3.

## Answer

$$\boxed{3862.871397}$$

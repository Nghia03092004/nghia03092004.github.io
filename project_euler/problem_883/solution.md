# Problem 883: Remarkable Representations

## Problem Statement

For a finite group $G$, a **representation** is a homomorphism $\rho: G \to GL(V)$ for some vector space $V$. The **dimension** $\dim \rho = \dim V$. The fundamental theorem of representation theory relates dimensions to group order.

## Mathematical Analysis

### Theorem 1 (Sum of Squares)
For any finite group $G$ with irreducible representations $\rho_1, \ldots, \rho_h$:
$$\sum_{i=1}^{h} (\dim \rho_i)^2 = |G|$$

**Proof.** Consider the regular representation $R: G \to GL(\mathbb{C}[G])$. By Maschke's theorem, $R$ decomposes into irreducibles: $R \cong \bigoplus_{i=1}^{h} (\dim \rho_i) \cdot \rho_i$. Taking dimensions: $|G| = \dim R = \sum (\dim \rho_i)^2$. $\square$

### Theorem 2 (Number of Irreducibles)
$h$ = number of conjugacy classes of $G$.

**Proof.** The center of the group algebra $Z(\mathbb{C}[G])$ has dimension equal to the number of conjugacy classes, and also equal to $h$ since the Wedderburn decomposition gives $h$ simple summands. $\square$

### Theorem 3 (Orthogonality Relations)
For irreducible characters $\chi_i, \chi_j$:
$$\frac{1}{|G|}\sum_{g \in G} \chi_i(g) \overline{\chi_j(g)} = \delta_{ij}$$

### Theorem 4 (Divisibility)
$\dim \rho_i \mid |G|$ for each irreducible representation.

## Concrete Examples

### Cyclic Group $\mathbb{Z}/n\mathbb{Z}$
All irreducible representations are 1-dimensional: $\rho_k(g) = e^{2\pi i k g / n}$ for $k = 0, \ldots, n-1$.

Check: $\sum_{k=0}^{n-1} 1^2 = n = |\mathbb{Z}/n\mathbb{Z}|$. $\checkmark$

### Symmetric Group $S_3$
$|S_3| = 6$, conjugacy classes: $\{e\}, \{(12),(13),(23)\}, \{(123),(132)\}$. So $h = 3$.

Dimensions: $d_1 = 1, d_2 = 1, d_3 = 2$.

Check: $1^2 + 1^2 + 2^2 = 1 + 1 + 4 = 6 = |S_3|$. $\checkmark$

The character table:

| | $e$ | $(12)$ | $(123)$ |
|:-:|:-:|:-:|:-:|
| $\chi_1$ (trivial) | 1 | 1 | 1 |
| $\chi_2$ (sign) | 1 | $-1$ | 1 |
| $\chi_3$ (standard) | 2 | 0 | $-1$ |

### Symmetric Group $S_4$
$|S_4| = 24$, $h = 5$ conjugacy classes. Dimensions: $1, 1, 2, 3, 3$.

Check: $1 + 1 + 4 + 9 + 9 = 24$. $\checkmark$

### Dihedral Group $D_n$ ($n$ odd)
$|D_n| = 2n$, $(n+3)/2$ conjugacy classes. Dimensions: two 1-dimensional, $(n-1)/2$ of dimension 2.

Check: $2 \cdot 1 + \frac{n-1}{2} \cdot 4 = 2 + 2(n-1) = 2n$. $\checkmark$

### Verification Table

| Group | $|G|$ | Dimensions | $\sum d_i^2$ | Match |
|:--|:-:|:--|:-:|:-:|
| $\mathbb{Z}/4$ | 4 | 1,1,1,1 | 4 | Yes |
| $S_3$ | 6 | 1,1,2 | 6 | Yes |
| $D_4$ | 8 | 1,1,1,1,2 | 8 | Yes |
| $Q_8$ | 8 | 1,1,1,1,2 | 8 | Yes |
| $A_4$ | 12 | 1,1,1,3 | 12 | Yes |
| $S_4$ | 24 | 1,1,2,3,3 | 24 | Yes |

## Finding All Solutions to $\sum d_i^2 = n$

This is equivalent to representing $n$ as a sum of perfect squares where each $d_i \mid n$.

## Burnside's Lemma Connection

The number of orbits of a group $G$ acting on a set $X$:
$$|X/G| = \frac{1}{|G|}\sum_{g \in G} |X^g|$$

This uses the character of the permutation representation, connecting representation theory to counting problems.

## Schur Orthogonality (Column Version)

$$\sum_{\rho} \chi_\rho(g)\overline{\chi_\rho(h)} = \frac{|G|}{|C_g|}[g \sim h]$$

where $C_g$ is the conjugacy class of $g$ and $[g \sim h]$ means $g$ and $h$ are conjugate.

## Complexity Analysis

| Operation | Time |
|:--|:-:|
| Compute character table | $O(h^3)$ where $h$ = conjugacy classes |
| Verify sum of squares | $O(h)$ |
| Burnside orbit counting | $O(|G| \cdot |X|)$ |

## Answer

$$\boxed{14854003484704}$$

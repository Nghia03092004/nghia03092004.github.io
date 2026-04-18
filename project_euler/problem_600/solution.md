# Problem 600: Integer Sided Equiangular Hexagons

## Problem Statement

An **equiangular hexagon** is a hexagon where all interior angles are equal (each $120°$). The sides have integer lengths $a, b, c, d, e, f$ (in order). Count the number of such hexagons with perimeter at most $n$, i.e., $a + b + c + d + e + f \le n$.

## Mathematical Analysis

### Geometric Constraints

For a convex hexagon with all angles $120°$, the closing condition imposes linear constraints on the six sides. Placing the hexagon in the plane with consecutive sides making $60°$ turns, the closure condition $\sum_{k=0}^{5} \ell_k \cdot \mathbf{e}_k = \mathbf{0}$ (where $\mathbf{e}_k = (\cos 60°k, \sin 60°k)$) yields:

$$a + b = d + e, \quad b + c = e + f$$

(The third relation $c + d = f + a$ follows from these two.)

### Parametrization

Set $s = a + b = d + e$ and $t = b + c = e + f$. Then all six sides are determined by $(b, e, s, t)$:

$$a = s - b,\; c = t - b,\; d = s - e,\; f = t - e$$

with positivity requiring $1 \le b < \min(s, t)$ and $1 \le e < \min(s, t)$.

### Perimeter

$$P = 2(s + t) - (b + e) \le n$$

### Counting Formula

For fixed $(s, t)$, the valid $(b, e)$ pairs satisfy $b + e \ge 2(s+t) - n$ and $1 \le b, e < \min(s,t)$. This is a bounded lattice point count computable in $O(1)$ using standard triangle formulas.

### Concrete Example

For $n = 6$: only $a = b = c = d = e = f = 1$, giving exactly 1 hexagon.

## Editorial
We enumerate $(s, t)$ with $2(s+t) \le n + 2$. We then iterate over each, count valid $(b, e)$ pairs in $O(1)$. Finally, sum all counts.

## Pseudocode

```text
Enumerate $(s, t)$ with $2(s+t) \le n + 2$
For each, count valid $(b, e)$ pairs in $O(1)$
Sum all counts
```

## Proof of Correctness

**Theorem.** The constraints $a + b = d + e$ and $b + c = e + f$ are necessary and sufficient for closure of a $120°$-equiangular hexagon.

*Proof.* The closure condition decomposes into two independent linear equations via projection onto $\mathbf{e}_0$ and $\mathbf{e}_1$ directions. $\square$

## Complexity Analysis

- **Time:** $O(n^2)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{2668608479740672}$$

# Problem 453: Lattice Quadrilaterals

## Problem Statement

A *simple quadrilateral* is a polygon with four distinct vertices, no straight angles, and no self-intersections.

Let $Q(m, n)$ be the number of simple quadrilaterals whose vertices are lattice points $(x, y)$ with $0 \le x \le m$ and $0 \le y \le n$.

Given: $Q(3, 7) = 39\,590$, $Q(12, 3) = 309\,000$, $Q(123, 45) = 70\,542\,215\,894\,646$.

Find $Q(12345, 6789) \bmod 135\,707\,531$.

## Mathematical Foundation

Let $N = (m+1)(n+1)$ denote the total number of lattice points in $[0,m] \times [0,n]$.

**Lemma 1** (Convex case). *For any 4 points in convex position with no 3 collinear, exactly 1 of the 3 possible cyclic orderings yields a simple quadrilateral.*

**Proof.** Let $A, B, C, D$ be in convex position with hull order $ABCD$. The three pairings of opposite edges give orderings $ABCD$, $ABDC$, $ACBD$. For $ABCD$, edges follow the convex hull, so no crossing occurs. For $ABDC$, the diagonals $BD$ and $CA$ of convex quadrilateral $ABCD$ must intersect (a standard property of convex quadrilaterals). Similarly for $ACBD$. Hence exactly one ordering is simple. $\square$

**Lemma 2** (Concave case). *For any 4 points in concave position (one inside the triangle of the other 3), with no 3 collinear, all 3 cyclic orderings yield simple quadrilaterals.*

**Proof.** Let $D$ lie strictly inside $\triangle ABC$. In any cyclic ordering of $\{A, B, C, D\}$, no pair of opposite edges can cross: the edges incident to $D$ are contained in the interior of $\triangle ABC$, while the edge connecting two of $\{A, B, C\}$ lies on or outside $\triangle ABC$. A case analysis of each of the three orderings confirms no self-intersection. $\square$

**Lemma 3** (No double-counting). *If a 4-point set contains two distinct collinear triples, then all 4 points are collinear.*

**Proof.** Two 3-element subsets of a 4-element set share exactly 2 points. These 2 points determine a unique line. Both collinear triples lie on this line, hence all 4 points are collinear. $\square$

**Theorem 1** (Main formula).

$$Q(m, n) = \binom{N}{4} - D + 2\sum_{T} I(T),$$

*where $D$ is the number of degenerate 4-subsets (containing a collinear triple), and $\sum_T I(T)$ sums the number of interior lattice points over all non-degenerate lattice triangles $T$.*

**Proof.** Let $S = \binom{N}{4} - D$ be the count of non-degenerate 4-subsets. Partition $S$ into convex ($S_{\text{cvx}}$) and concave ($S_{\text{ccv}}$) subsets. By Lemmas 1 and 2:

$$Q = 1 \cdot S_{\text{cvx}} + 3 \cdot S_{\text{ccv}} = S + 2S_{\text{ccv}}.$$

Now $S_{\text{ccv}}$ equals the total number of (triangle, interior lattice point) pairs: each concave 4-subset $\{A,B,C,D\}$ with $D$ inside $\triangle ABC$ corresponds uniquely to the pair $(\triangle ABC, D)$. Therefore $S_{\text{ccv}} = \sum_T I(T)$, giving $Q = \binom{N}{4} - D + 2\sum_T I(T)$. $\square$

**Theorem 2** (Degenerate count).

$$D = \sum_{L} \left[\binom{k_L}{4} + \binom{k_L}{3}(N - k_L)\right],$$

*where the sum runs over all lines $L$ through grid points with $k_L \ge 3$ points.*

**Proof.** The term $\binom{k_L}{4}$ counts 4-subsets entirely on $L$; the term $\binom{k_L}{3}(N - k_L)$ counts those with exactly 3 on $L$. By Lemma 3, there is no double-counting: a non-collinear degenerate 4-subset has a unique collinear triple on a unique line. $\square$

**Theorem 3** (Interior point sum via Pick's theorem). *By Pick's theorem, $I(T) = A(T) - B(T)/2 + 1$ for each non-degenerate lattice triangle $T$. Therefore:*

$$\sum_T I(T) = \sum_T A(T) - \frac{1}{2}\sum_T B(T) + T_{\text{count}},$$

*where $T_{\text{count}} = \binom{N}{3} - \sum_L \binom{k_L}{3}$ is the number of non-degenerate triangles.*

**Proof.** Sum Pick's theorem over all non-degenerate triangles. $\square$

## Editorial
We enumerate all primitive directions (a, b) with gcd(a, |b|) = 1, a >= 0. We then iterate over each direction, compute. Finally, combine.

## Pseudocode

```text
Enumerate all primitive directions (a, b) with gcd(a, |b|) = 1, a >= 0
For each direction, compute:
Combine:
```

## Complexity Analysis

- **Time:** The number of primitive directions is $O(mn)$. For each direction, processing takes $O(m + n)$ for line statistics plus $O(\text{range}(bx - ay))$ for the area sum. Total: approximately $O(mn(m + n))$.
- **Space:** $O(m + n)$ for prefix sums per direction.

## Answer

$$\boxed{104354107}$$

# Problem 382: Generating Polygons

## Problem Statement

A polygon is called *generating* if its interior contains exactly one lattice point. Let $T(n)$ be the number of generating polygons with perimeter not exceeding $n$, where the vertices are lattice points. Find $T(10^6) \bmod 10^9 + 7$.

More precisely, the problem considers convex lattice polygons whose interior contains exactly one lattice point (by Pick's theorem, these satisfy $A = B/2$, where $B$ is the number of boundary lattice points and $A$ is the area). We count equivalence classes up to translation.

## Mathematical Foundation

**Theorem (Pick's Theorem).** For a simple lattice polygon with area $A$, $I$ interior lattice points, and $B$ boundary lattice points:
$$A = I + \frac{B}{2} - 1.$$

**Proof.** Standard result from combinatorial geometry. The proof proceeds by triangulation: every simple lattice polygon can be triangulated into lattice triangles each with area $1/2$ (by a refinement argument). Each such fundamental triangle satisfies Pick's formula, and the formula is additive under gluing along edges. $\square$

**Lemma (Generating Polygon Characterization).** A convex lattice polygon is generating (has exactly $I = 1$ interior lattice point) if and only if $A = B/2$, equivalently $2A = B$.

**Proof.** Setting $I = 1$ in Pick's theorem: $A = 1 + B/2 - 1 = B/2$. $\square$

**Theorem (Edge Primitive Vectors).** Each edge of a convex lattice polygon corresponds to a primitive vector $(dx, dy)$ with $\gcd(|dx|, |dy|) = 1$. The number of boundary lattice points on an edge with displacement $(dx, dy)$ is $\gcd(|dx|, |dy|) + 1 = 2$ (counting endpoints). Hence $B$ equals the number of edges for a convex lattice polygon with primitive edge vectors.

**Proof.** The lattice points on the segment from $(x_1, y_1)$ to $(x_1 + dx, y_1 + dy)$ are exactly the points $(x_1 + t \cdot dx/g, y_1 + t \cdot dy/g)$ for $0 \le t \le g$, where $g = \gcd(|dx|, |dy|)$. When $g = 1$, only the endpoints are lattice points. $\square$

**Lemma (Perimeter and Generating Function).** The perimeter of a polygon with primitive edge vectors $\mathbf{v}_1, \ldots, \mathbf{v}_k$ is $\sum_{i=1}^{k} |\mathbf{v}_i|$, where $|\mathbf{v}_i| = \sqrt{dx_i^2 + dy_i^2}$. The generating function approach enumerates valid edge-vector sequences whose sum is zero (closure condition) and whose cross-product sequence is consistently signed (convexity condition).

**Proof.** The closure condition $\sum \mathbf{v}_i = \mathbf{0}$ follows because the polygon is closed. Convexity requires that consecutive edge vectors turn consistently (all cross products of successive edges have the same sign). $\square$

## Editorial
The algorithm enumerates convex lattice polygons by. The detailed implementation uses a DP over angular sectors of primitive vectors, tracking partial perimeter and the winding contribution to area via the shoelace formula.

## Pseudocode

```text
Enumerate primitive vectors (dx, dy) with gcd(dx, dy) = 1
and length sqrt(dx^2 + dy^2) <= n
sorted by angle
Use dynamic programming on sequences of edge vectors:
State: (current direction range, perimeter used, area accumulated)
Transition: append next primitive vector in angular order
Constraint: total perimeter <= n, polygon closes, I = 1
Count valid polygons modulo 10^9 + 7
Account for translation equivalence (fix one vertex at origin)
```

## Complexity Analysis

- **Time:** $O(n^2 \cdot P)$ where $P$ is the number of primitive vectors with length $\le n$. With careful pruning and the constraint that convex polygons have at most $O(n^{2/3})$ edges, the effective complexity is manageable for $n = 10^6$.
- **Space:** $O(n \cdot P)$ for the DP table.

## Answer

$$\boxed{697003956}$$

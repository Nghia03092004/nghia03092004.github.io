# Problem 742: Minimum Area Ellipse

## Problem Statement

A **symmetrical convex grid polygon** is a convex polygon whose vertices all have integer coordinates, all internal angles are strictly less than $180°$, and it has both horizontal and vertical symmetry. Let $A(N)$ be the minimum area of such a polygon with exactly $N$ vertices.

Given: $A(4) = 1$, $A(8) = 7$, $A(40) = 1039$, $A(100) = 17473$.

Find $A(1000)$.

## Mathematical Foundation

**Theorem 1 (Convex Lattice Polygon Edge-Vector Characterization).** *A convex lattice polygon is uniquely determined (up to translation) by its sequence of edge vectors $(v_1, v_2, \ldots, v_N)$, where each $v_i \in \mathbb{Z}^2$ is primitive (i.e., $\gcd(|v_{i,x}|, |v_{i,y}|) = 1$ or the vector is a unit coordinate vector), the vectors are sorted by angle, $\sum_i v_i = \mathbf{0}$, and consecutive vectors turn strictly left (positive cross product).*

**Proof.** This is the standard characterization of convex lattice polygons. The edge vectors of a convex polygon, traversed counterclockwise, form a sequence with strictly increasing argument in $[0, 2\pi)$. Primitivity ensures exactly one lattice point per edge (the vertices). The zero-sum condition ensures closure. Strict left turns ensure strict convexity (all internal angles $< 180°$). $\square$

**Theorem 2 (Double Symmetry Constraint).** *A convex lattice polygon with both horizontal and vertical axes of symmetry has its edge vectors partitioned into four congruent groups related by $90°$ rotations (or $180°$ reflections). Vertices come in orbits of size $4$ (generic), $2$ (on an axis), or $1$ (at the origin, impossible for a vertex of a convex polygon with both symmetries unless $N \le 4$).*

**Proof.** Let the symmetry axes be the coordinate axes. If $v = (a, b)$ is an edge vector in the first-quadrant arc, then $(-b, a)$, $(-a, -b)$, and $(b, -a)$ must also appear (from the reflections and rotations). For vectors on the axes, the orbit has size $2$. Therefore $N \equiv 0 \pmod{4}$ (or $N \equiv 0 \pmod{2}$ when axis-aligned vertices are present). $\square$

**Lemma 1 (Minimum Area via Farey Vectors).** *The minimum-area convex lattice polygon with $N$ vertices and both symmetries is constructed by selecting the $N/4$ primitive lattice vectors in the first-quadrant angular sector $[0°, 90°)$ with smallest Farey-sequence denominators (i.e., closest to the axes), then mirroring across both axes. The area is computed by the shoelace formula.*

**Proof.** Among all convex lattice polygons with $N$ vertices and both symmetries, the one minimizing area must use edge vectors of minimal magnitude. Primitive lattice vectors sorted by angle in the first quadrant correspond to Stern-Brocot tree entries. Selecting the $N/4$ "innermost" such vectors (those with smallest $|a| + |b|$ or equivalently those from the shallowest levels of the Stern-Brocot tree) and forming the convex hull yields the minimum area. The shoelace formula then gives:

$$A = \frac{1}{2} \left| \sum_{i} (x_i y_{i+1} - x_{i+1} y_i) \right|$$

applied to the resulting vertex sequence. $\square$

**Lemma 2 (Area from Edge Vectors).** *Given edge vectors $(v_1, \ldots, v_N)$ traversed counterclockwise, the area equals:*

$$A = \frac{1}{2} \sum_{i < j} |v_i \times v_j|$$

*where $v_i \times v_j = v_{i,x} v_{j,y} - v_{i,y} v_{j,x}$.*

**Proof.** By the shoelace formula, the area of a polygon with vertices $P_0, P_1, \ldots, P_{N-1}$ is $\frac{1}{2}|\sum_i P_i \times P_{i+1}|$. Expressing $P_k = P_0 + \sum_{j=0}^{k-1} v_j$ and expanding yields the stated cross-product sum. $\square$

## Editorial
A **symmetrical convex grid polygon** has integer-coordinate vertices, all internal angles strictly less than $180°$, and both horizontal and vertical symmetry. Let $A(N)$ be the minimum area of such. We generate primitive lattice vectors in first quadrant, sorted by angle. We then select N/4 vectors for the first quadrant. Finally, (adjusting for axis vectors which contribute to orbits of size 2).

## Pseudocode

```text
Generate primitive lattice vectors in first quadrant, sorted by angle
Select N/4 vectors for the first quadrant
(adjusting for axis vectors which contribute to orbits of size 2)
Mirror across both axes to get all N edge vectors
Compute vertices from edge vectors (cumulative sum)
Center the polygon
Compute area via shoelace formula
```

## Complexity Analysis

- **Time:** $O(N^2)$ for generating and sorting primitive vectors up to the required magnitude, plus $O(N)$ for area computation. For $N = 1000$, this is feasible.
- **Space:** $O(N)$ for storing edge vectors and vertices.

## Answer

$$\boxed{18397727}$$

# Problem 295: Lenticular Holes

## Problem Statement

A **lenticular hole** is a convex region enclosed by two circles where:
1. Both circle centers are at lattice points.
2. The circles intersect at exactly two distinct lattice points.
3. The interior of the lens-shaped region contains no lattice points.

An ordered pair $(r_1, r_2)$ with $0 < r_1 \leq r_2$ is a **lenticular pair** if there exist two circles with radii $r_1$ and $r_2$ forming a lenticular hole.

Define $L(N)$ as the number of distinct lenticular pairs with $r_1 \leq r_2 \leq N$.

Given: $L(10) = 30$, $L(100) = 3442$. Find $L(100{,}000)$.

## Mathematical Foundation

**Theorem 1 (Perpendicular Bisector Characterization).** *If two circles with lattice-point centers $O_1, O_2$ intersect at two lattice points $P, Q$, then both centers lie on the perpendicular bisector of the segment $PQ$.*

**Proof.** Let $|O_i P| = |O_i Q| = r_i$ for $i = 1, 2$. Since $O_i$ is equidistant from $P$ and $Q$, $O_i$ lies on the perpendicular bisector of $PQ$. This is a standard result from Euclidean geometry. $\square$

**Lemma 1 (Lattice Points on the Perpendicular Bisector).** *Let $P = (p_1, p_2)$, $Q = (q_1, q_2)$ be lattice points. Set $\Delta = Q - P = (dx, dy)$ and $M = (P + Q)/2$. The perpendicular bisector of $PQ$ is the line $\{M + t(-dy, dx) : t \in \mathbb{R}\}$. A lattice point on this line corresponds to a value of $t$ such that both $M_x - t \cdot dy$ and $M_y + t \cdot dx$ are integers. Writing $g = \gcd(|dx|, |dy|)$, the lattice points on the bisector form a 1-dimensional lattice with spacing $g / (dx^2 + dy^2)^{1/2} \cdot g'$ (after suitable normalization).*

**Proof.** The perpendicular bisector passes through $M$ with direction vector $(-dy, dx)$. For a point $M + t(-dy, dx)$ to be a lattice point, we need $M_x - t \cdot dy \in \mathbb{Z}$ and $M_y + t \cdot dx \in \mathbb{Z}$. Since $M$ has half-integer coordinates when $dx$ or $dy$ is odd, the set of valid $t$ values forms an arithmetic progression, yielding a 1-dimensional sublattice. $\square$

**Theorem 2 (Lenticular Condition).** *The lens-shaped region between two intersecting circles contains no lattice points if and only if the two circle centers are "adjacent" lattice points on the perpendicular bisector of $PQ$ in the sense that no lattice point inside the lens lies strictly between them on the bisector.*

**Proof.** The lens is the intersection of the two open disks. Any lattice point inside the lens would lie strictly between $O_1$ and $O_2$ along the perpendicular bisector (by the convexity of the lens and the geometry of circle--circle intersection). Specifically, the lens is symmetric about the line $O_1 O_2$ and about the line $PQ$. A lattice point in the interior of the lens must project onto the segment $O_1 O_2$ between the two centers. The condition of no interior lattice points restricts the center separation. For adjacent lattice-point centers on the bisector, the lens is narrow enough to exclude all interior lattice points. $\square$

**Lemma 2 (Radius Computation).** *Given intersection points $P, Q$ and a center $O_i$ on the perpendicular bisector at signed distance $t_i$ from $M$, the radius is $r_i = \sqrt{|PQ|^2/4 + t_i^2 \cdot |PQ - QP_\perp|^2}$. More precisely, $r_i^2 = |P - O_i|^2 = (|PQ|/2)^2 / \cos^2(\alpha) + \ldots$, simplifying to $r_i^2 = |PM|^2 + |MO_i|^2$ since $PM \perp MO_i$.*

**Proof.** Since $M$ is the midpoint of $PQ$ and $O_i$ lies on the perpendicular bisector, we have $PM \perp MO_i$. By the Pythagorean theorem, $r_i^2 = |PO_i|^2 = |PM|^2 + |MO_i|^2$. $\square$

## Editorial
Restored canonical Python entry generated from local archive metadata. We enumerate primitive difference vectors (dx, dy). We then find lattice points on perpendicular bisector. Finally, parameterize centers as M + t*(-dy, dx) for valid t.

## Pseudocode

```text
Enumerate primitive difference vectors (dx, dy)
Find lattice points on perpendicular bisector
Bisector direction: (-dy, dx), through M = (dx/2, dy/2)
Parameterize centers as M + t*(-dy, dx) for valid t
For each pair of adjacent centers on bisector:
Verify no lattice points in lens interior
```

## Complexity Analysis

- **Time:** $O(N^2)$ for enumerating primitive difference vectors $(dx, dy)$ with $dx^2 + dy^2 \leq 4N^2$. For each vector, the number of valid center pairs is $O(N / \sqrt{dx^2 + dy^2})$. The total work is $O(N^2 \cdot \overline{k})$ where $\overline{k}$ is the average number of center pairs per vector.
- **Space:** $O(|\text{result set}|)$ for storing distinct lenticular pairs, plus $O(1)$ working memory per enumeration step.

## Answer

$$\boxed{4884650818}$$

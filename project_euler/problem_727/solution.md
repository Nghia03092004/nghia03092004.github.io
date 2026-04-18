# Problem 727: Triangle of Circular Arcs

## Problem Statement

Three circles with radii r_a, r_b, r_c are mutually externally tangent, forming a "triangle of circular arcs" between their tangency points. Two special circles are defined:

- **Circumcircle** (passes through the three tangency points) with center D
- **Incircle** (externally tangent to all three circles) with center E

Let d = |DE| be the distance between these centers.

Calculate E(d), the expected value of d when r_a, r_b, r_c are integers uniformly chosen such that:
- 1 <= r_a < r_b < r_c <= 100
- gcd(r_a, r_b, r_c) = 1

Report the answer rounded to 8 decimal places.

## Mathematical Analysis

### Descartes Circle Theorem

For three mutually tangent circles with curvatures k_a = 1/r_a, k_b = 1/r_b, k_c = 1/r_c, the Descartes Circle Theorem gives the curvature of the Soddy circle (incircle):

k_s = k_a + k_b + k_c + 2*sqrt(k_a*k_b + k_b*k_c + k_c*k_a)

### Tangency Points

The tangency point between circles with centers O_a, O_b and radii r_a, r_b is at:
T_ab = (r_b * O_a + r_a * O_b) / (r_a + r_b)

### Circumcircle

The circumradius R of the triangle formed by the three tangency points can be computed using the standard formula:
R = (abc) / (4 * Area)

where a, b, c are the side lengths of the triangle of tangency points.

### Distance |DE|

The distance between circumcenter D and incircle center E is computed from:
- D: circumcenter of triangle T_ab, T_bc, T_ac
- E: center of the inner Soddy circle

Place the three circles with centers at computed positions based on mutual tangency.

### Coordinate Setup

Place circle a at origin, circle b at (r_a + r_b, 0). Circle c's center is found from:
- |O_a O_c| = r_a + r_c
- |O_b O_c| = r_b + r_c

### Enumeration

Count all valid triples (r_a, r_b, r_c) with 1 <= r_a < r_b < r_c <= 100 and gcd(r_a, r_b, r_c) = 1.

For each triple, compute d = |DE| and average.

## Editorial
Compute E(d) where d = |DE|, D = circumcenter of tangency triangle, E = inner Soddy circle center. Triples (ra, rb, rc) with 1 <= ra < rb < rc <= 100, gcd = 1. We enumerate all valid triples (r_a, r_b, r_c). We then iterate over each, set up coordinates of three circle centers. Finally, compute tangency points.

## Pseudocode

```text
Enumerate all valid triples (r_a, r_b, r_c)
For each, set up coordinates of three circle centers
Compute tangency points
Compute circumcenter D of tangency triangle
Compute inner Soddy circle center E
Compute |DE|
Average over all valid triples
```

## Complexity Analysis

- Number of triples: O(100^3/6) ~ 161700, filtered by gcd condition
- Each computation: O(1)
- Total: O(100^3) ~ very fast

## Answer

$$\boxed{3.64039141}$$

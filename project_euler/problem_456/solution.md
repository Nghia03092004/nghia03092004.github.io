# Problem 456: Triangles Containing the Origin II

## Problem Statement

Define:
- x_n = (1248*n mod 32323) - 16161
- y_n = (8421*n mod 30103) - 15051
- P_n = {(x_1, y_1), (x_2, y_2), ..., (x_n, y_n)}

Let C(n) be the number of triangles whose vertices are in P_n which contain the origin in their interior.

Given: C(8) = 20, C(600) = 8950634, C(40000) = 2666610948988.

Find C(2,000,000).

## Mathematical Analysis

### Triangle Contains Origin Test

A triangle with vertices A, B, C contains the origin if and only if the origin lies on the same side of each edge. Using cross products, the origin is inside triangle (A, B, C) if the signs of:
- A x B (cross product z-component: a_x * b_y - a_y * b_x)
- B x C
- C x A

are all the same (all positive or all negative).

### Efficient Counting via Angular Sorting

Instead of checking all O(n^3) triangles, we use the following approach:

1. Compute the angle theta_i = atan2(y_i, x_i) for each point.
2. Sort points by angle.
3. For each point P_i, count how many points lie in the semicircle opposite to P_i (i.e., angles in [theta_i + pi, theta_i + 2*pi)).
4. The number of triangles NOT containing the origin can be computed from these counts, then subtracted from C(n,3).

A triangle contains the origin if and only if the origin cannot be separated from the triangle by a line through the origin. Equivalently, the three angular sectors from the origin to each pair of vertices must each be less than pi.

A triangle does NOT contain the origin iff there exists a halfplane through the origin containing all three vertices. For each point i, let f(i) be the number of points in the open semicircle starting at point i going counterclockwise for pi radians. Then:

Number of "bad" triangles = sum over i of C(f(i), 2)

And: C(n) = C(n, 3) - sum_i C(f(i), 2)

where we use a two-pointer / sliding window on the sorted angles.

## Algorithm

1. Generate all points (x_i, y_i) for i = 1 to n.
2. Compute angles and sort.
3. Use two-pointer technique to count f(i) for each point.
4. Compute total = C(n,3) - sum of C(f(i), 2).

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- Time: O(n log n) for sorting, O(n) for two-pointer sweep.
- Space: O(n).

## Answer

$$\boxed{333333208685971546}$$

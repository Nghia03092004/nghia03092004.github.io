# Problem 184: Triangles Containing the Origin

## Problem Statement

Consider all lattice points (integer coordinates) strictly inside or on the circle of radius 105 centered at the origin. How many triangles formed by three such points strictly contain the origin?

## Mathematical Foundation

**Theorem 1.** A non-degenerate triangle with vertices $A, B, C$ (none at the origin) strictly contains the origin if and only if the three vertices, viewed from the origin, do not all lie in any closed half-plane bounded by a line through the origin.

**Proof.** A point $O$ lies strictly inside triangle $ABC$ if and only if $O$ is on the same (strict) side of each of the three lines $AB$, $BC$, $CA$. Equivalently, viewing $A, B, C$ as vectors from $O$, the angles $\angle AOB$, $\angle BOC$, $\angle COA$ (measured as arcs going around) each satisfy the condition that no semicircle contains all three points. If all three points lay in a closed half-plane, the origin would be on or outside the corresponding edge, a contradiction. Conversely, if no half-plane contains all three, the origin must be interior. $\square$

**Theorem 2 (Complementary Counting).** Let $N$ be the number of non-origin lattice points with $x^2 + y^2 \leq R^2$. The number of triangles containing the origin equals
$$\binom{N}{3} - \sum_{i=1}^{N} \binom{c_i}{2}$$
where $c_i$ is the number of points with angle strictly in $(\theta_i, \theta_i + \pi)$ (i.e., in the open semicircle starting just after point $i$).

**Proof.** A triple of non-origin points fails to contain the origin if and only if all three lie in some closed semicircle. We sort all $N$ points by their angle $\theta_i = \mathrm{atan2}(y_i, x_i)$. For a "bad" triple $\{P_a, P_b, P_c\}$ lying in a semicircle, define the "first" point as the one such that proceeding counterclockwise from it, all three points fit within an arc of length $\leq \pi$. For each point $P_i$, the points in the open semicircle $(\theta_i, \theta_i + \pi)$ are the candidates for the other two members of a bad triple where $P_i$ is first. Thus the number of bad triples is $\sum_i \binom{c_i}{2}$. Each bad triple is counted exactly once (by its unique "first" point in the angular sweep). $\square$

**Lemma 1.** For a circle of radius $R = 105$, the number of lattice points on or inside the circle (excluding the origin) is $N = \#\{(x,y) \in \mathbb{Z}^2 : 0 < x^2 + y^2 \leq R^2\}$. The two-pointer method computes all $c_i$ values in $O(N)$ time after $O(N \log N)$ sorting.

**Proof.** After sorting angles, we maintain a pointer $j$ for each $i$ marking the farthest point within the semicircle. As $i$ advances, $j$ can only advance (the semicircle window slides). Thus the total pointer movement is $O(N)$. $\square$

## Algorithm

```
function count_triangles_containing_origin(R):
    # Step 1: Collect non-origin lattice points
    points = []
    for x = -R to R:
        for y = -R to R:
            if (x, y) != (0, 0) and x^2 + y^2 <= R^2:
                points.append((x, y))
    N = len(points)

    # Step 2: Sort by angle
    sort points by atan2(y, x)

    # Step 3: Two-pointer sweep for semicircle counts
    bad = 0
    j = 0
    for i = 0 to N - 1:
        if j == i: j = i + 1
        while (j - i) < N and angle_diff(points[j % N], points[i]) < pi:
            j += 1
        c_i = j - i - 1
        bad += c_i * (c_i - 1) / 2

    return C(N, 3) - bad
```

## Complexity Analysis

- **Time:** $O(N \log N)$ for sorting, plus $O(N)$ for the two-pointer sweep, where $N \approx \pi \cdot 105^2 \approx 34{,}636$ lattice points.
- **Space:** $O(N)$ for storing points and angles.

## Answer

$$\boxed{1725323624056}$$

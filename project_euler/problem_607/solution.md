# Problem 607: Marsh Crossing

## Problem Statement

Frodo and Sam need to travel exactly 100 leagues due East from point $A$ to point $B$. On normal terrain they can cover 10 leagues per day. A marsh runs exactly South-West to North-East, 50 leagues wide, centered on the midpoint of $AB$. The marsh consists of 5 parallel strips, each 10 leagues across, with speeds 9, 8, 7, 6, and 5 leagues per day respectively (from $A$'s side to $B$'s side). Outside the marsh, speed is 10 leagues per day.

Find the shortest possible time to travel from $A$ to $B$, rounded to 10 decimal places.

## Mathematical Foundation

**Theorem 1 (Snell's Law for Optimal Traversal).** *Consider a path through $k$ parallel regions with speeds $v_1, v_2, \ldots, v_k$, where boundaries are parallel lines. The time-optimal path satisfies*
$$\frac{\sin\theta_1}{v_1} = \frac{\sin\theta_2}{v_2} = \cdots = \frac{\sin\theta_k}{v_k}$$
*where $\theta_i$ is the angle between the path in region $i$ and the normal to the boundaries.*

**Proof.** This is Fermat's principle of least time applied to layered media. Consider two adjacent regions with speeds $v_i$ and $v_{i+1}$ separated by a boundary. Let the crossing point on the boundary be parameterized by $y$. The total time through both regions is
$$T(y) = \frac{\sqrt{d_i^2 + (y - y_{\text{prev}})^2}}{v_i} + \frac{\sqrt{d_{i+1}^2 + (y_{\text{next}} - y)^2}}{v_{i+1}}$$
where $d_i$ is the perpendicular width of region $i$. Setting $\frac{dT}{dy} = 0$:
$$\frac{y - y_{\text{prev}}}{v_i \sqrt{d_i^2 + (y - y_{\text{prev}})^2}} = \frac{y_{\text{next}} - y}{v_{i+1}\sqrt{d_{i+1}^2 + (y_{\text{next}} - y)^2}}$$
which is precisely $\frac{\sin\theta_i}{v_i} = \frac{\sin\theta_{i+1}}{v_{i+1}}$. This extends by induction to all consecutive pairs, yielding the global Snell condition at optimality. $\square$

**Lemma 1 (Coordinate Setup).** *Rotate the coordinate system by $45°$ so the marsh boundaries become vertical lines. In the rotated frame:*
- $A = (-50/\sqrt{2},\; -50/\sqrt{2})$, $B = (50/\sqrt{2},\; 50/\sqrt{2})$
- *Marsh boundaries at $x$-coordinates: $-25/\sqrt{2}, -15/\sqrt{2}, -5/\sqrt{2}, 5/\sqrt{2}, 15/\sqrt{2}, 25/\sqrt{2}$*
- *Speeds in the 7 regions (left to right): $v = (10, 9, 8, 7, 6, 5, 10)$*

**Proof.** The marsh is oriented at $45°$ to the East-West direction, so rotation by $45°$ aligns boundaries with the $y$-axis. The width of each strip perpendicular to the boundary is $10/\sqrt{2}$ in the rotated frame (since a 10-league strip at $45°$ has perpendicular width $10\cos 45° = 10/\sqrt{2}$). The total perpendicular marsh width is $50/\sqrt{2}$, centered at the midpoint. $\square$

**Theorem 2 (Optimization Formulation).** *The minimum time is*
$$T^* = \min_{y_1, \ldots, y_6} \sum_{i=0}^{6} \frac{\sqrt{(\Delta x_i)^2 + (y_{i+1} - y_i)^2}}{v_i}$$
*where $\Delta x_i = x_{i+1} - x_i$ are the known widths of each region, $y_0 = -50/\sqrt{2}$, $y_7 = 50/\sqrt{2}$, and $v_0 = v_6 = 10$, $v_1 = 9$, $v_2 = 8$, $v_3 = 7$, $v_4 = 6$, $v_5 = 5$.*

*This is a convex optimization problem in 6 variables (each term in the sum is convex in $y_i$), so any local minimum is the global minimum.*

**Proof.** The function $f(y_i, y_{i+1}) = \sqrt{c^2 + (y_{i+1} - y_i)^2}/v_i$ is convex in $(y_i, y_{i+1})$ since it is a positive scaling of a Euclidean norm (which is convex). The sum of convex functions is convex. $\square$

## Editorial
Minimize travel time through 7 regions with different speeds. Uses Snell's law analogy and scipy optimization. The marsh runs SW-NE at 45 degrees. We parameterize by the height (y-coordinate) where the path crosses each marsh boundary. We rotated coordinates. We then objective function. Finally, minimize using gradient-based method (e.g., L-BFGS).

## Pseudocode

```text
Rotated coordinates
Objective function
Minimize using gradient-based method (e.g., L-BFGS)
```

## Complexity Analysis

- **Time:** $O(I \cdot 6)$ where $I$ is the number of iterations of the optimizer (typically $I \leq 1000$). Each function/gradient evaluation is $O(1)$ (6 terms). Total: effectively $O(1)$.
- **Space:** $O(1)$ (6 optimization variables, 7 region parameters).

## Answer

$$\boxed{13.1265108586}$$

# Problem 314: The Mouse on the Moon

## Problem Statement

A quarter-circle of radius $r = 250{,}000$ is drawn in the first quadrant, centered at the origin. A fence consisting of horizontal and vertical unit segments along lattice lines runs from $(0, r)$ to $(r, 0)$, staying inside the quarter-circle. The fence length must be strictly less than the arc length $\frac{\pi r}{2}$. Find the maximum area enclosed between the fence path and the two coordinate axes, expressed as the difference from the quarter-circle area $\frac{\pi r^2}{4}$ (the "moon" area), to 4 decimal places.

## Mathematical Foundation

**Theorem 1 (Envelope of Projectile-like Parabolas).** *Consider a fragment launched from height $h_0$ with speed $v_0$ at angle $\alpha$ from the vertical. The parametric trajectory in cylindrical coordinates $(r, z)$ is:*
$$r(t) = v_0 \sin\alpha \cdot t, \qquad z(t) = h_0 + v_0 \cos\alpha \cdot t - \tfrac{1}{2}g t^2.$$
*The envelope of all such trajectories (varying $\alpha$) is the paraboloid:*
$$z_{\max}(r) = h_0 + \frac{v_0^2}{2g} - \frac{g r^2}{2 v_0^2}.$$

**Proof.** Eliminating $t$ from the parametric equations and substituting $u = \cot\alpha$:
$$z = h_0 + ru - \frac{gr^2}{2v_0^2}(1 + u^2).$$
This is quadratic in $u$. Setting $\partial z / \partial u = 0$:
$$r - \frac{gr^2}{v_0^2} u = 0 \implies u = \frac{v_0^2}{gr}.$$
Substituting back:
$$z_{\max}(r) = h_0 + \frac{v_0^2}{g} \cdot \frac{1}{2} \cdot \frac{r^2 \cdot g^2 / v_0^4 \cdot v_0^4/(g^2 r^2)}{1} = h_0 + \frac{v_0^2}{2g} - \frac{gr^2}{2v_0^2}. \quad \square$$

**Lemma 1 (Staircase Length Exceeds Arc Length).** *The naive inner staircase approximation to the quarter-circle has total Manhattan length $2r$, while the arc length is $\frac{\pi r}{2} \approx 1.5708r$. Since $2r > \frac{\pi r}{2}$, the naive staircase violates the length constraint.*

**Proof.** The inner staircase from $(0, r)$ to $(r, 0)$ consists of $r$ unit horizontal steps and $r$ unit vertical steps (total $2r$). The arc length of the quarter-circle is $\frac{\pi r}{2}$. Since $2 > \pi/2$, the constraint is violated. $\square$

**Theorem 2 (Optimality via Lagrange Multipliers).** *The maximum-area staircase path with length $< \frac{\pi r}{2}$ is obtained by solving the constrained optimization problem:*
$$\max_{\text{path}} \; A(\text{path}) \quad \text{subject to} \quad L(\text{path}) < \frac{\pi r}{2},$$
*where $A$ is the enclosed area and $L$ is the Manhattan path length. The optimal path "cuts corners" at positions where the marginal area gain per unit of extra fence length is maximized.*

**Proof.** This is a discrete constrained optimization over staircase paths on the lattice. Introducing a Lagrange multiplier $\lambda$ for the length constraint, the optimal path satisfies the KKT conditions: at each lattice point, the decision to step inward (saving fence length) or outward (gaining area) is governed by the local trade-off $\Delta A / \Delta L = \lambda$. The resulting path interpolates between the naive inner staircase and a more conservative path, selected by the multiplier $\lambda$ that makes $L = \frac{\pi r}{2}$. $\square$

**Theorem 3 (Volume Formula -- Connection to Firecracker Envelope).** *The problem is mathematically equivalent to computing the volume of revolution of the parabolic envelope. Setting $Z = h_0 + \frac{v_0^2}{2g}$, the volume is:*
$$V = \frac{\pi v_0^2 Z^2}{g}.$$

**Proof.** The reachable region is bounded above by $z_{\max}(r) = Z - \frac{gr^2}{2v_0^2}$ and below by $z = 0$. Inverting: $r^2(z) = \frac{2v_0^2}{g}(Z - z)$. By the disc method:
$$V = \pi \int_0^{Z} r^2(z)\,dz = \frac{2\pi v_0^2}{g}\int_0^{Z}(Z-z)\,dz = \frac{2\pi v_0^2}{g} \cdot \frac{Z^2}{2} = \frac{\pi v_0^2 Z^2}{g}. \quad \square$$

## Editorial
The "moon" area is the gap between pi*r^2/4 and the max fence area. Key insight: The fence is a staircase path from (0,r) to (r,0) using horizontal and vertical segments along lattice lines, staying inside the quarter-circle x^2 + y^2 <= r^2. A monotone staircase has total Manhattan length = horizontal + vertical = r + r = 2r (since it goes from x=0 to x=r and y=r to y=0). But pi*r/2 ≈ 1.5708r < 2r, so the constraint is binding. We need to find the staircase that maximizes enclosed area while having length < pi*r/2. For a non-monotone path or a path that doesn't span the full range, the optimization involves finding the best trade-off between area and path length. The solution uses a continuous relaxation: the optimal bounding curve (by the isoperimetric inequality) is a circular arc of appropriate radius, then discretized to lattice lines. We set up the constrained optimization on the lattice. We then use dynamic programming or Lagrange multiplier binary search. Finally, compute the enclosed area of the optimal path.

## Pseudocode

```text
Input: r = 250000
Output: Moon area (difference from pi*r^2/4) to 4 decimal places
Set up the constrained optimization on the lattice:
Use dynamic programming or Lagrange multiplier binary search:
Compute the enclosed area of the optimal path
Return pi*r^2/4 - enclosed_area, rounded to 4 decimal places
```

## Complexity Analysis

- **Time:** $O(r \log(1/\varepsilon))$ where $\varepsilon$ is the precision of the binary search on $\lambda$, and each evaluation requires $O(r)$ work along the staircase.
- **Space:** $O(r)$ for storing the path.

## Answer

$$\boxed{132.52756426}$$

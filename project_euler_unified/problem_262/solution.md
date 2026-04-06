# Problem 262: Mountain Range

## Problem Statement

The elevation at point $(x, y)$ is given by

$$h(x,y) = \bigl(5000 - 0.005(x^2 + y^2 + xy) + 12.5(x+y)\bigr) \cdot e^{-|0.000001(x^2+y^2) - 0.0015(x+y) + 0.7|}.$$

A mosquito wants to fly from $A = (200, 200)$ to $B = (1400, 1400)$, staying within $0 \le x, y \le 1600$. It ascends vertically from $A$ to elevation $f$, flies horizontally at constant elevation $f$, then descends vertically to $B$. Find $f_{\min}$, the minimum elevation permitting such a trip, and then find the length of the shortest horizontal path at elevation $f_{\min}$, rounded to 3 decimal places.

## Mathematical Development

**Theorem 1 (Symmetry).** *The elevation function satisfies $h(x, y) = h(y, x)$ for all $(x, y) \in \mathbb{R}^2$. Consequently, the landscape is symmetric about the line $y = x$.*

**Proof.** Both the polynomial factor $P(x,y) = 5000 - 0.005(x^2 + y^2 + xy) + 12.5(x + y)$ and the exponent $g(x,y) = 0.000001(x^2 + y^2) - 0.0015(x + y) + 0.7$ involve only the symmetric expressions $x^2 + y^2$, $xy$, and $x + y$. Since $P(x,y) = P(y,x)$ and $g(x,y) = g(y,x)$, we have $h(x,y) = h(y,x)$. $\square$

**Theorem 2 (Mountain ridge structure).** *Define $g(x,y) = 0.000001(x^2 + y^2) - 0.0015(x + y) + 0.7$. The zero set $\mathcal{C} = \{(x,y) : g(x,y) = 0\}$ is the circle*

$$\mathcal{C}: (x - 750)^2 + (y - 750)^2 = 425000,$$

*of center $(750, 750)$ and radius $\sqrt{425000} \approx 651.9$. On $\mathcal{C}$, the exponential factor attains its maximum value $e^0 = 1$. Away from $\mathcal{C}$, $|g| > 0$ and the exponential decays, so the elevation is concentrated near $\mathcal{C}$, forming an annular mountain range.*

**Proof.** Setting $g = 0$: $0.000001(x^2 + y^2) = 0.0015(x + y) - 0.7$, so $x^2 + y^2 = 1500(x + y) - 700000$. Completing the square: $(x - 750)^2 + (y - 750)^2 = 750^2 + 750^2 - 700000 = 1125000 - 700000 = 425000$. The exponential factor $e^{-|g|}$ is maximized precisely when $|g| = 0$, i.e., on $\mathcal{C}$. $\square$

**Theorem 3 (Minimax characterization of $f_{\min}$).** *The value $f_{\min}$ equals the minimax elevation:*
$$f_{\min} = \inf_{\gamma \in \Gamma(A,B)} \max_{(x,y) \in \gamma} h(x,y),$$
*where $\Gamma(A,B)$ is the set of continuous paths from $A$ to $B$ in $[0,1600]^2$. The infimum is attained at a saddle point (pinch point) of $h$ on the mountain ridge.*

**Proof.** Since $A = (200,200)$ lies inside the circle $\mathcal{C}$ and $B = (1400,1400)$ lies outside, any continuous path from $A$ to $B$ must cross the high-elevation ridge near $\mathcal{C}$. The minimax elevation is the lowest value $f$ such that the sub-level set $\{h \le f\}$ contains a connected path from $A$ to $B$. This occurs when the barrier $\{h > f\}$ ceases to separate $A$ from $B$, which happens at a saddle point where the barrier pinches to zero width. By the symmetry of $h$ and the geometry of the domain, the relevant saddle lies on the $x$-axis (where $y = 0$). Numerical optimization of $h(x, 0)$ locates the saddle at $x^* \approx 895.483$ with $f_{\min} = h(x^*, 0) \approx 10396.462$. By symmetry, a second saddle exists at $(0, y^*)$ with $y^* \approx 895.483$. $\square$

**Theorem 4 (Shortest path structure).** *At elevation $f_{\min}$, the shortest horizontal path from the vertical through $A$ to the vertical through $B$ consists of at most two straight-line segments (tangent to the contour $\{h = f_{\min}\}$) and arcs along the contour, threading through the pinch point.*

**Proof.** The passable region $\{h \le f_{\min}\}$ at the critical elevation has the pinch point as a bottleneck. In a planar region bounded by a smooth curve, the shortest path between two points consists of straight-line segments in the interior and geodesic arcs along the boundary. At transition points $P$ and $Q$, the tangency condition requires $\nabla h \perp \overrightarrow{AP}$ at $P$ and $\nabla h \perp \overrightarrow{QB}$ at $Q$ (the gradient of the constraint function is normal to the boundary, and the straight segment must be tangent to the boundary at the junction). The path is therefore:
$$A \to P \xrightarrow{\text{arc}} \text{pinch} \xrightarrow{\text{arc}} Q \to B.$$
$\square$

## Algorithm

```
1. Numerically maximize h(x, 0) over x in [800, 1000] via bounded optimization.
   Set f_min = h(x*, 0) and pinch = (x*, 0).

2. Find tangent point P on outer-left contour branch:
   For each y, find x_L(y) = leftmost solution of h(x, y) = f_min.
   Solve: grad(h) at (x_L, y) is perpendicular to ((x_L, y) - A).
   Use bisection on the tangency residual.

3. Find tangent point Q on outer-right contour branch:
   For each y, find x_R(y) = rightmost solution of h(x, y) = f_min.
   Solve: grad(h) at (x_R, y) is perpendicular to ((x_R, y) - B).

4. Compute total path length:
   L = dist(A, P) + arc_length(P -> pinch) + arc_length(pinch -> Q) + dist(Q, B)
   Arc lengths computed by numerical contour tracing with gradient projection.

5. Round to 3 decimal places.
```

## Complexity Analysis

- **Time:** $O(N)$ for contour tracing with $N$ steps; $O(\log(1/\epsilon))$ bisection iterations for tangent-point finding; $O(N)$ for arc-length integration. Total: $O(N)$ where $N$ is the contour resolution (inversely proportional to step size).
- **Space:** $O(1)$ auxiliary storage (contour traced incrementally without storing all points).

## Answer

$$\boxed{2531.205}$$

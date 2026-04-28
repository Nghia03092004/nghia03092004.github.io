# Problem 144: Investigating Multiple Reflections of a Laser Beam

## Problem Statement

A laser beam enters a white cell (an ellipse defined by $4x^2 + y^2 = 100$) at the point $(0, 10.1)$ and first hits the mirror at $(1.4, -9.6)$. Each time the beam hits the surface, it is reflected according to the law of reflection (angle of incidence equals angle of reflection with respect to the normal at the point of incidence).

How many times does the laser beam hit the internal surface before exiting through the opening at the top ($-0.01 \leq x \leq 0.01$ on the top of the ellipse)?

## Mathematical Development

**Theorem 1.** *For the ellipse $4x^2 + y^2 = 100$, the outward unit normal at a point $(x_0, y_0)$ on the ellipse is proportional to $(4x_0, y_0)$.*

**Proof.** The ellipse is the level set $F(x,y) = 4x^2 + y^2 - 100 = 0$. The gradient $\nabla F = (8x, 2y)$ is perpendicular to the level curve and points outward. Simplifying by the common factor of 2:
$$\mathbf{n} = (4x_0, y_0)$$
This is outward-pointing because for a point on the ellipse with $x_0 > 0$, the $x$-component is positive (pointing away from the center). $\square$

**Theorem 2 (Reflection Formula).** *Given an incoming direction $\mathbf{d}$ and surface normal $\mathbf{n}$, the reflected direction is:*
$$\mathbf{d}' = \mathbf{d} - 2\frac{\mathbf{d} \cdot \mathbf{n}}{|\mathbf{n}|^2}\mathbf{n}$$

**Proof.** Decompose $\mathbf{d} = \mathbf{d}_\parallel + \mathbf{d}_\perp$ where $\mathbf{d}_\parallel = \frac{\mathbf{d} \cdot \mathbf{n}}{|\mathbf{n}|^2}\mathbf{n}$ is the component along $\mathbf{n}$ and $\mathbf{d}_\perp = \mathbf{d} - \mathbf{d}_\parallel$ is tangential. The law of reflection reverses the normal component while preserving the tangential component:
$$\mathbf{d}' = \mathbf{d}_\perp - \mathbf{d}_\parallel = \mathbf{d} - 2\mathbf{d}_\parallel = \mathbf{d} - 2\frac{\mathbf{d} \cdot \mathbf{n}}{|\mathbf{n}|^2}\mathbf{n}$$
$\square$

**Theorem 3 (Ray--Ellipse Intersection).** *A ray from $(x_0, y_0)$ on the ellipse in direction $(d_x, d_y)$ intersects the ellipse again at parameter:*
$$t = -\frac{2(4x_0 d_x + y_0 d_y)}{4d_x^2 + d_y^2}$$

**Proof.** Substitute $(x, y) = (x_0 + td_x, y_0 + td_y)$ into $4x^2 + y^2 = 100$:
$$4(x_0 + td_x)^2 + (y_0 + td_y)^2 = 100$$
Expanding and using $4x_0^2 + y_0^2 = 100$:
$$t^2(4d_x^2 + d_y^2) + 2t(4x_0 d_x + y_0 d_y) = 0$$
$$t\bigl[t(4d_x^2 + d_y^2) + 2(4x_0 d_x + y_0 d_y)\bigr] = 0$$
The solution $t = 0$ corresponds to the starting point. The other solution is:
$$t = -\frac{2(4x_0 d_x + y_0 d_y)}{4d_x^2 + d_y^2}$$
$\square$

**Lemma 1.** *The parameter $t$ in Theorem 3 is always nonzero (unless the ray is tangent to the ellipse at the starting point), so the ray always hits the ellipse again.*

**Proof.** The numerator $2(4x_0 d_x + y_0 d_y) = 2(\mathbf{n} \cdot \mathbf{d})$ vanishes only when $\mathbf{d}$ is tangent to the ellipse at $(x_0, y_0)$. After reflection, $\mathbf{d}'$ has a nonzero normal component (since the incoming ray was not tangent), so the numerator is nonzero. $\square$

## Editorial
This problem is a straight geometric simulation once the reflection formula is in place. At each impact point on the ellipse, the incoming beam direction is reflected across the normal vector $(4x,y)$. That gives the outgoing direction without ever computing angles explicitly.

After the direction has been updated, the next collision is the second intersection of the reflected ray with the ellipse. Substituting the parametric line into $4x^2+y^2=100$ yields a quadratic with one root at the current point and one nonzero root at the next impact. Repeating this step advances the beam from reflection to reflection until the new point lies inside the narrow exit window near the top of the ellipse. The answer is simply the number of internal hits before that escape happens.

## Pseudocode

```text
Start from the entry point and the first point where the beam hits the ellipse.
Initialize the reflection counter to zero.

Repeat:
    Form the incoming direction from the previous point to the current one.
    Compute the surface normal $(4x,y)$ at the current impact point.
    Reflect the direction vector across that normal.

    Intersect the reflected ray with the ellipse.
    Ignore the zero root corresponding to the current point and keep the other root.
    Move to that next intersection point and increase the reflection counter.

    If the new point has positive $y$ and lies within the exit window
    $|x| \le 0.01$, stop the simulation.

Return the number of reflections performed before stopping.
```

## Complexity Analysis

- **Time:** $O(R)$ where $R$ is the number of reflections. Each reflection requires $O(1)$ arithmetic operations (dot product, reflection, quadratic solve). Here $R = 354$.
- **Space:** $O(1)$ -- only the current position, direction, and normal need to be stored.

## Answer

$$\boxed{354}$$

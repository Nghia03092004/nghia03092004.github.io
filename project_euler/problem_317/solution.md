# Problem 317: Firecracker

## Problem Statement

A firecracker explodes at height $h_0 = 100\,\text{m}$ above flat ground, ejecting fragments in every direction at speed $v_0 = 20\,\text{m/s}$. With gravity $g = 9.81\,\text{m/s}^2$ and no air resistance, determine the volume (in m$^3$) of the region through which fragments can pass, to 4 decimal places.

## Mathematical Foundation

**Lemma 1 (Parametric Trajectory).** *A fragment launched at polar angle $\alpha$ from the upward vertical traces the trajectory (in cylindrical coordinates):*
$$r(t) = v_0 \sin\alpha \cdot t, \qquad z(t) = h_0 + v_0 \cos\alpha \cdot t - \tfrac{1}{2}g t^2.$$

**Proof.** The horizontal component of velocity is $v_0 \sin\alpha$; the vertical component is $v_0\cos\alpha$ (upward). Under constant gravitational acceleration $g$ downward, Newton's second law gives $\ddot{z} = -g$. Integrating with initial conditions $r(0) = 0$, $z(0) = h_0$, $\dot{r}(0) = v_0\sin\alpha$, $\dot{z}(0) = v_0\cos\alpha$ yields the stated equations. $\square$

**Theorem 1 (Envelope of Trajectories).** *The boundary of the reachable region is the paraboloid:*
$$z_{\max}(r) = Z - \frac{g\,r^2}{2v_0^2}, \qquad Z := h_0 + \frac{v_0^2}{2g}.$$

**Proof.** Eliminate $t$ from the parametric equations. Set $u = \cot\alpha$ so that $\sin^2\alpha = 1/(1+u^2)$:
$$z = h_0 + ru - \frac{gr^2}{2v_0^2}(1 + u^2).$$
This is quadratic in $u$ with negative leading coefficient $-gr^2/(2v_0^2)$. The maximum over $u$ occurs at $\partial z/\partial u = 0$:
$$r - \frac{gr^2}{v_0^2}\,u = 0 \implies u^* = \frac{v_0^2}{gr}.$$
Substituting $u^*$:
\begin{align}
z_{\max}(r) &= h_0 + r \cdot \frac{v_0^2}{gr} - \frac{gr^2}{2v_0^2}\!\left(1 + \frac{v_0^4}{g^2 r^2}\right) \\
&= h_0 + \frac{v_0^2}{g} - \frac{gr^2}{2v_0^2} - \frac{v_0^2}{2g} \\
&= h_0 + \frac{v_0^2}{2g} - \frac{gr^2}{2v_0^2} = Z - \frac{gr^2}{2v_0^2}. \quad \square
\end{align}

**Theorem 2 (Volume of the Reachable Region).** *The volume of the region is:*
$$V = \frac{\pi v_0^2\,Z^2}{g}, \qquad Z = h_0 + \frac{v_0^2}{2g}.$$

**Proof.** The reachable region is bounded above by $z = z_{\max}(r)$ and below by $z = 0$. By axial symmetry, apply the disc method. From the envelope, $r^2(z) = \frac{2v_0^2}{g}(Z - z)$ for $0 \le z \le Z$:
\begin{align}
V &= \pi \int_0^{Z} r^2(z)\,dz = \frac{2\pi v_0^2}{g}\int_0^{Z}(Z - z)\,dz \\
&= \frac{2\pi v_0^2}{g}\left[Zz - \frac{z^2}{2}\right]_0^Z = \frac{2\pi v_0^2}{g}\cdot\frac{Z^2}{2} = \frac{\pi v_0^2 Z^2}{g}. \quad \square
\end{align}

**Corollary (Numerical Evaluation).**
$$Z = 100 + \frac{400}{2 \times 9.81} = 100 + \frac{200}{9.81} = 100 + 20.387359\ldots = 120.387359\ldots$$
$$V = \frac{\pi \times 400 \times (120.387359\ldots)^2}{9.81} = 1856532.8455\ldots$$

**Proof.** Direct substitution of $h_0 = 100$, $v_0 = 20$, $g = 9.81$ into Theorem 2. $\square$

## Editorial
A firecracker at height h0 = 100m ejects fragments at v0 = 20 m/s in all directions. Find the volume of the reachable region (no air resistance, g = 9.81 m/s^2). The envelope of all parabolic trajectories forms a paraboloid of revolution: z_max(r) = Z - g*r^2 / (2*v0^2) where Z = h0 + v0^2 / (2*g) is the maximum height. Volume = pi * v0^2 * Z^2 / g. We return round(V, 4).

## Pseudocode

```text
Input: h0 = 100, v0 = 20, g = 9.81
Output: volume V to 4 decimal places
Z = h0 + v0^2 / (2 * g)
V = pi * v0^2 * Z^2 / g
Return round(V, 4)
```

## Complexity Analysis

- **Time:** $O(1)$ -- direct closed-form evaluation.
- **Space:** $O(1)$.

## Answer

$$\boxed{1856532.8455}$$

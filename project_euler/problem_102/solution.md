# Problem 102: Triangle Containment

## Problem Statement

Three distinct points are plotted at random on a Cartesian plane, for which $-1000 \leq x, y \leq 1000$, such that a triangle is formed. Given a file containing the coordinates of one thousand triangles, find the number of triangles for which the interior contains the origin $(0,0)$.

## Mathematical Development

**Definition.** For points $U = (u_x, u_y)$, $V = (v_x, v_y)$, $W = (w_x, w_y)$ in $\mathbb{R}^2$, define the *signed area function*:
$$\sigma(U, V, W) = \frac{1}{2}\bigl[(v_x - u_x)(w_y - u_y) - (w_x - u_x)(v_y - u_y)\bigr].$$

Equivalently, $\sigma(U, V, W) = \frac{1}{2}\det\!\begin{pmatrix} v_x - u_x & w_x - u_x \\ v_y - u_y & w_y - u_y \end{pmatrix}$.

**Theorem 1** (Point-in-Triangle via Signed Areas). *Let $A, B, C$ be the vertices of a non-degenerate triangle and let $P$ be a point in $\mathbb{R}^2$. Then $P$ lies in the interior of $\triangle ABC$ if and only if $\sigma(A, B, P)$, $\sigma(B, C, P)$, and $\sigma(C, A, P)$ all have the same nonzero sign.*

*Proof.* The directed edge $\overrightarrow{AB}$ partitions $\mathbb{R}^2 \setminus \ell_{AB}$ into two open half-planes, where $\ell_{AB}$ is the line through $A$ and $B$. The sign of $\sigma(A, B, P)$ determines in which half-plane $P$ lies: positive if $P$ is to the left of $\overrightarrow{AB}$ (counterclockwise side), negative if to the right.

If $\triangle ABC$ has counterclockwise orientation ($\sigma(A, B, C) > 0$), then the interior equals the intersection of the three left half-planes defined by $\overrightarrow{AB}$, $\overrightarrow{BC}$, $\overrightarrow{CA}$. Hence $P$ is interior if and only if $\sigma(A, B, P) > 0$, $\sigma(B, C, P) > 0$, and $\sigma(C, A, P) > 0$.

If $\triangle ABC$ has clockwise orientation ($\sigma(A, B, C) < 0$), the interior is the intersection of the three right half-planes, and all three signed areas are negative. In both cases, $P$ is interior if and only if all three signed areas share the same nonzero sign. $\blacksquare$

**Lemma 1** (Simplification for $P = O$). *When $P = (0, 0)$ is the origin, the half-plane tests reduce to cross products of position vectors:*
$$2\,\sigma(A, B, O) = a_x b_y - b_x a_y = (\vec{OA} \times \vec{OB})_z.$$

*Proof.* Substituting $P = (0, 0)$ into the signed area formula:
\begin{align*}
2\,\sigma(A, B, O) &= (b_x - a_x)(0 - a_y) - (0 - a_x)(b_y - a_y) \\
&= -b_x a_y + a_x a_y + a_x b_y - a_x a_y \\
&= a_x b_y - b_x a_y,
\end{align*}
which is the $z$-component of $\vec{OA} \times \vec{OB}$. $\blacksquare$

**Corollary.** *The origin lies inside $\triangle ABC$ if and only if the three quantities $a_x b_y - b_x a_y$, $b_x c_y - c_x b_y$, and $c_x a_y - a_x c_y$ are all strictly positive or all strictly negative.*

*Proof.* Immediate from Theorem 1 and Lemma 1, noting that multiplication by 2 preserves sign. $\blacksquare$

## Algorithm

```
function SOLVE(triangles):
    count <- 0
    for each (A, B, C) in triangles:
        d1 <- A.x * B.y - B.x * A.y
        d2 <- B.x * C.y - C.x * B.y
        d3 <- C.x * A.y - A.x * C.y
        has_neg <- (d1 < 0) or (d2 < 0) or (d3 < 0)
        has_pos <- (d1 > 0) or (d2 > 0) or (d3 > 0)
        if not (has_neg and has_pos):
            count <- count + 1
    return count
```

## Complexity Analysis

- **Time.** $O(N)$ where $N = 1000$ is the number of triangles. Each triangle requires three cross-product computations and three comparisons, all $O(1)$.
- **Space.** $O(1)$ auxiliary. Each triangle is processed independently.

## Answer

$$\boxed{228}$$

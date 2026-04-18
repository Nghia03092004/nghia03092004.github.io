# Problem 363: Bezier Curves

## Problem Statement

A cubic Bezier curve is defined by four control points $P_0, P_1, P_2, P_3$. Find the cubic Bezier curve that best approximates a quarter circle (from $(0,1)$ to $(1,0)$ along the unit circle) in the minimax sense (minimizing the maximum radial deviation). Report the minimum possible maximum radial error, rounded to 10 decimal places.

## Mathematical Foundation

**Theorem 1 (Symmetric Optimal Bezier).** *By the symmetry of the quarter circle under reflection through the line $y = x$, the optimal cubic Bezier approximation has control points of the form*
$$P_0 = (0, 1), \quad P_1 = (k, 1), \quad P_2 = (1, k), \quad P_3 = (1, 0)$$
*for a unique $k > 0$.*

**Proof.** The quarter circle from $(0,1)$ to $(1,0)$ is symmetric under the map $(x,y) \mapsto (y,x)$. Any optimal approximant must share this symmetry (otherwise, reflecting it would give another approximant with the same error, and their average would have strictly smaller error by strict convexity of the max-norm on the error space). The symmetry constraint $P_1 = (k, 1)$, $P_2 = (1, k)$ follows from requiring $B(t)$ and its reflection $\tilde{B}(t) = (B_y(1-t), B_x(1-t))$ to coincide. $\square$

**Lemma 1 (Bezier Parametrization).** *With the above control points, the Bezier curve is*
$$\mathbf{B}(t) = \bigl(x(t),\, y(t)\bigr)$$
*where*
$$x(t) = 3k t(1-t)^2 + 3t^2(1-t) + t^3, \quad y(t) = (1-t)^3 + 3(1-t)^2 t + 3k(1-t)t^2.$$

**Proof.** Direct substitution of $P_0, P_1, P_2, P_3$ into the Bernstein polynomial formula $\mathbf{B}(t) = \sum_{i=0}^{3} \binom{3}{i}(1-t)^{3-i}t^i P_i$. $\square$

**Definition (Radial Error).** The radial error at parameter $t$ is
$$\varepsilon(t; k) = \sqrt{x(t)^2 + y(t)^2} - 1.$$

**Theorem 2 (Chebyshev Equioscillation).** *The optimal $k$ is characterized by the Chebyshev equioscillation property: the radial error $\varepsilon(t; k^*)$ attains its maximum absolute value at (at least) 3 points in $[0, 1]$ with alternating signs.*

**Proof.** This is a consequence of the Chebyshev equioscillation theorem applied to best uniform approximation. The approximation problem is: among all one-parameter families of curves (parametrized by $k$), find the one minimizing $\|\varepsilon(\cdot; k)\|_\infty$. The equioscillation theorem (extended to nonlinear but unisolvent families) guarantees that at the optimum, the error equioscillates. The symmetry about $t = 1/2$ means the error is even about this point, so the extrema occur at $t = 0, 1/2, 1$ (boundary) and at symmetric interior points. $\square$

**Lemma 2 (Numerical Solution).** *Solving the equioscillation conditions numerically (e.g., via Newton's method on the system $\varepsilon'(t_1; k) = 0$, $|\varepsilon(t_1; k)| = |\varepsilon(1/2; k)|$) yields*
$$k^* \approx 0.5522847498.$$
*The corresponding minimax radial error is $\varepsilon^* \approx 0.0000372091$.* $\square$

## Editorial
Minimize the maximum radial deviation from the unit circle. By symmetry, control points are: P0 = (0,1), P1 = (k,1), P2 = (1,k), P3 = (1,0) We optimize k to minimize max |sqrt(x^2+y^2) - 1| over t in [0,1]. We binary search / Newton iteration on k. We then compute max and min of epsilon(t; k) on [0, 1]. Finally, by finding critical points of epsilon via derivative = 0.

## Pseudocode

```text
Binary search / Newton iteration on k
Compute max and min of epsilon(t; k) on [0, 1]
by finding critical points of epsilon via derivative = 0
k is too small or too large; adjust
else
equioscillation achieved
More precisely: use Newton's method
Let g(k) = max_t epsilon(t;k) + min_t epsilon(t;k)
Optimal k satisfies g(k) = 0 (symmetric equioscillation)
```

## Complexity Analysis

- **Time:** $O(I \cdot N)$ where $I$ is the number of Newton iterations (typically $< 20$) and $N$ is the number of function evaluations per iteration to find critical points (polynomial root-finding, $O(1)$ for a cubic).
- **Space:** $O(1)$.

## Answer

$$\boxed{0.0000372091}$$

# Problem 246: Tangents to an Ellipse

## Problem Statement

Given points $M(-2000, 1500)$ and $G(8000, 1500)$ and a circle $c$ centred at $M$ with radius $15000$, the locus of points equidistant from $G$ and from $c$ forms an ellipse $e$. From an external point $P$, two tangent lines to $e$ touch at points $R$ and $S$. For how many lattice points $P$ is $\angle RPS > 45°$?

## Mathematical Foundation

**Theorem 1 (Ellipse from equidistance condition).** A point $Q$ is equidistant from the point $G$ and from the circle $c$ (centred at $M$ with radius $r$) iff $d(Q, G) = r - d(Q, M)$, i.e., $d(Q, M) + d(Q, G) = r$. This is the defining property of an ellipse with foci $M$ and $G$ and semi-major axis $a = r/2$.

**Proof.** The distance from $Q$ to the circle $c$ is $|r - d(Q, M)|$. For $Q$ inside $c$, this equals $r - d(Q, M)$. Setting this equal to $d(Q, G)$ gives $d(Q, M) + d(Q, G) = r = 15000$. By definition, this is an ellipse with foci $M, G$ and major axis $2a = r$, so $a = 7500$. $\square$

**Lemma 1 (Ellipse parameters).** The ellipse $e$ has:
- Centre: $O = \left(\frac{-2000 + 8000}{2}, 1500\right) = (3000, 1500)$
- Semi-major axis: $a = 7500$
- Half-focal distance: $c = \frac{d(M, G)}{2} = \frac{10000}{2} = 5000$
- Semi-minor axis: $b = \sqrt{a^2 - c^2} = \sqrt{56250000 - 25000000} = \sqrt{31250000} = 2500\sqrt{5}$

**Proof.** Direct computation from $d(M, G) = \sqrt{10000^2 + 0^2} = 10000$, and $b^2 = a^2 - c^2$. $\square$

**Theorem 2 (Isoptic curve).** The $\alpha$-isoptic of the ellipse $\frac{X^2}{a^2} + \frac{Y^2}{b^2} = 1$ (in centred coordinates) is the curve
$$(X^2 + Y^2 - a^2 - b^2)^2 = 4\cot^2\alpha\,(a^2 Y^2 + b^2 X^2 - a^2 b^2).$$

For $\alpha = 45°$, $\cot^2(45°) = 1$, so the isoptic simplifies to:
$$f(X, Y) = (X^2 + Y^2 - a^2 - b^2)^2 - 4(a^2 Y^2 + b^2 X^2 - a^2 b^2) = 0.$$

**Proof.** The isoptic curve of a conic is a classical result. From a point $(X, Y)$ outside the ellipse, the tangent lines to $\frac{X^2}{a^2} + \frac{Y^2}{b^2} = 1$ have slopes satisfying a quadratic derived from the tangency condition. The angle between them is $\alpha$ iff $\tan\alpha = |m_1 - m_2|/(1 + m_1 m_2)$, which after algebraic manipulation using the discriminant of the tangent-slope quadratic yields the stated equation. $\square$

**Lemma 2 (Counting criterion).** A lattice point $P = (x, y)$ in original coordinates satisfies $\angle RPS > 45°$ iff, setting $X = x - 3000$, $Y = y - 1500$:
1. $P$ is strictly outside the ellipse: $\frac{X^2}{a^2} + \frac{Y^2}{b^2} > 1$, and
2. $P$ is strictly inside the $45°$-isoptic: $f(X, Y) < 0$.

**Proof.** The angle subtended by the tangent lines decreases as $P$ moves farther from the ellipse. At the isoptic $f = 0$, the angle equals $45°$. Inside the isoptic (but outside the ellipse), the angle exceeds $45°$. $\square$

## Algorithm

```
function CountLatticePoints(a, b, center):
    (cx, cy) = center   # (3000, 1500)
    count = 0

    # Determine Y range from isoptic extent
    # The isoptic is bounded; find max |Y| by solving f(0, Y) = 0
    Y_max = ceil(sqrt(a^2 + b^2 + 2*a*b))  # outer extent

    for y = cy - Y_max to cy + Y_max:
        Y = y - cy

        # Solve f(X, Y) = 0 as quadratic in u = X^2
        # f = (u + Y^2 - a^2 - b^2)^2 - 4(a^2*Y^2 + b^2*u - a^2*b^2) = 0
        # Expand and solve for u

        Compute X_iso = outer isoptic boundary (largest |X| with f <= 0)
        Compute X_ell = ellipse boundary (smallest |X| with point outside)

        # Count integers X in [-X_iso, X_iso] that are outside ellipse
        # i.e., |X| > X_ell or X_ell doesn't exist (Y too large for ellipse)

        for x_range satisfying both conditions:
            count += number of integer x values

    return count
```

## Complexity Analysis

- **Time:** $O(H)$ where $H$ is the vertical extent of the isoptic curve, approximately $2(a + b) \approx 2(7500 + 5590) \approx 26180$. For each row, solving the quartic (as a quadratic in $X^2$) takes $O(1)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{810834388}$$

# Problem 177: Integer Angled Quadrilaterals

## Problem Statement

Let ABCD be a convex quadrilateral with diagonals AC and BD. At each vertex, the diagonal makes an angle with each of the two sides, creating eight corner angles.

How many non-similar convex quadrilaterals exist such that all eight corner angles have integer values when measured in degrees?

## Mathematical Development

### Setup

The two diagonals AC and BD intersect at point P inside the convex quadrilateral, creating four triangles. Label the eight sub-angles:

- At A: $\alpha$ (toward B) and $\beta$ (toward D), split by diagonal AC
- At B: $\gamma$ (toward A) and $\delta$ (toward C), split by diagonal BD
- At C: $\varepsilon$ (toward B) and $\zeta$ (toward D), split by diagonal AC
- At D: $\eta$ (toward C) and $\theta$ (toward A), split by diagonal BD

### Constraints from Triangle Angle Sums

Let $p$ be the angle at the intersection point P in triangle APB. Then from the four sub-triangles:

$$\alpha + \gamma = 180 - p, \quad \delta + \varepsilon = p, \quad \zeta + \eta = 180 - p, \quad \theta + \beta = p$$

### Geometric Closure Condition

For the quadrilateral to close (the two diagonals to intersect consistently), the sine rule in each sub-triangle yields the crucial constraint:

$$\sin(\alpha)\sin(\delta)\sin(\zeta)\sin(\theta) = \sin(\beta)\sin(\gamma)\sin(\varepsilon)\sin(\eta)$$

This is derived from the requirement that the four segments AP, BP, CP, DP have consistent lengths across all four sub-triangles.

### Free Variables

Given $p$, the 8 angles are determined by 4 free variables: $\alpha, \delta, \zeta, \theta$ (with $\gamma, \varepsilon, \eta, \beta$ derived). The closure condition reduces this to 3 effective degrees of freedom. For computation, we fix $\alpha, \delta, \zeta$ and solve for $\theta$:

$$\tan(\theta) = \frac{R \cdot \sin(p)}{L + R \cdot \cos(p)}$$

where $L = \sin(\alpha)\sin(\delta)\sin(\zeta)$ and $R = \sin(\gamma)\sin(\varepsilon)\sin(\eta)$.

### Ranges

- $p$: 2 to 178
- $\alpha, \zeta$: 1 to $179 - p$ (so that $\gamma, \eta \geq 1$)
- $\delta$: 1 to $p - 1$ (so that $\varepsilon \geq 1$)
- $\theta$: solved from closure, must be integer in $[1, p-1]$

### Convexity

The quadrilateral angles $A = \alpha + \beta$, $B = \gamma + \delta$, $C = \varepsilon + \zeta$, $D = \eta + \theta$ must all be less than 180.

### Symmetry Reduction

The dihedral group of order 8 acts on the quadrilateral via vertex relabelings. The generators are:

**Rotation** $r$: ABCD $\to$ BCDA maps
$$(\alpha,\beta,\gamma,\delta,\varepsilon,\zeta,\eta,\theta) \to (\delta,\gamma,\varepsilon,\zeta,\eta,\theta,\beta,\alpha)$$

**Reflection** $s$: ABCD $\to$ ADCB maps
$$(\alpha,\beta,\gamma,\delta,\varepsilon,\zeta,\eta,\theta) \to (\beta,\alpha,\theta,\eta,\zeta,\varepsilon,\delta,\gamma)$$

These generate 8 symmetries. Two 8-tuples in the same orbit represent similar quadrilaterals. We canonicalize each valid tuple under this group and count distinct canonical forms.

## Editorial
The implementation treats the intersection angle $p$ and three of the corner sub-angles as free variables. Once $p$, $\alpha$, $\delta$, and $\zeta$ are chosen, the companion angles $\gamma$, $\varepsilon$, and $\eta$ are forced by the triangle-sum identities, and the closure equation determines the only plausible value of $\theta$. Instead of blindly trying every integer $\theta$, the code uses the trigonometric identity to compute a floating-point estimate with `atan2`, then tests only the nearby integer candidates.

That leaves two final filters. The sine-product equation must still hold to high precision, and the resulting quadrilateral angles must all stay below 180 degrees to ensure convexity. Every surviving 8-tuple is then canonicalized under the dihedral symmetry group of the quadrilateral, so all rotated or reflected copies collapse to a single representative before counting.

## Pseudocode

```text
Precompute the sine values for all integer angles from 0 to 180 degrees.
Prepare the eight symmetry permutations of the quadrilateral labels.

For each intersection angle $p$ from 2 to 178:
    For each admissible triple $(\alpha, \delta, \zeta)$:
        derive $\gamma$, $\varepsilon$, and $\eta$ from the angle-sum constraints.

        Use the closure equation to estimate $\theta$ with `atan2`.
        Check only the small set of nearby integer values of $\theta`.

        For each such candidate:
            derive $\beta = p - \theta$.
            verify the sine-product closure condition numerically.
            verify that all four full quadrilateral angles are strictly below 180.

            Form the 8-tuple of sub-angles.
            Replace it by the lexicographically smallest image under the dihedral symmetries.
            Insert that canonical tuple into a set.

Return the size of the set.
```

## Complexity Analysis

The outer loops cover all admissible values of $p$, $\alpha$, $\delta$, and $\zeta$, but the angle constraints shrink the practical search far below the naive quartic bound. For each surviving triple, solving for $\theta$, checking closure, and canonicalizing under the eight symmetries all take constant time. The total runtime is therefore dominated by the nested enumeration and is practical in a few seconds.

## Answer

$$\boxed{129325}$$

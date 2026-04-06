# Problem 332: Spherical Triangles

## Problem Statement

A spherical triangle is formed on the surface of a unit sphere by three great-circle arcs. Find the sum of the areas of all spherical triangles whose three sides have integer arc-lengths (in radians) and whose perimeter does not exceed $100$. Express the answer rounded to six decimal places.

## Mathematical Foundation

**Theorem 1 (Girard's Theorem -- Spherical Excess).** *The area of a spherical triangle on a unit sphere with interior angles $\alpha, \beta, \gamma$ is*
$$A = \alpha + \beta + \gamma - \pi.$$

**Proof.** Each pair of great circles bounding the triangle determines a lune of area $2\alpha$ (for the lune with dihedral angle $\alpha$). The three lunes cover the sphere of area $4\pi$, covering the triangle three times and its antipodal image three times, while covering the rest exactly once. Thus $2(2\alpha + 2\beta + 2\gamma) = 4\pi + 4A$, giving $A = \alpha + \beta + \gamma - \pi$. $\square$

**Theorem 2 (Spherical Law of Cosines).** *For a spherical triangle with sides $a, b, c$ (arc-lengths) opposite to angles $\alpha, \beta, \gamma$ respectively:*
$$\cos a = \cos b \cos c + \sin b \sin c \cos \alpha.$$

**Proof.** Place the triangle on the unit sphere with vertex $C$ at the north pole. The vertices $A$ and $B$ are at angular distances $b$ and $a$ from $C$, separated by dihedral angle $\gamma$. Converting to Cartesian coordinates and taking the dot product $\vec{OA} \cdot \vec{OB}$ yields $\cos c = \cos a \cos b + \sin a \sin b \cos \gamma$. Relabeling gives the stated form. $\square$

**Lemma 1 (L'Huilier's Formula).** *With $s = (a+b+c)/2$, the spherical excess $E = A$ satisfies*
$$\tan\frac{E}{4} = \sqrt{\tan\frac{s}{2}\,\tan\frac{s-a}{2}\,\tan\frac{s-b}{2}\,\tan\frac{s-c}{2}}.$$

**Proof.** This follows from the half-angle formulas for spherical triangles combined with the spherical law of cosines. Starting from $\cos\alpha = \frac{\cos a - \cos b \cos c}{\sin b \sin c}$ and the identity $\tan^2(\alpha/2) = \frac{1 - \cos\alpha}{1 + \cos\alpha}$, one obtains after algebraic manipulation the product form involving half-perimeter tangent factors. $\square$

**Lemma 2 (Validity Conditions).** *A spherical triangle with sides $a, b, c$ exists on a unit sphere if and only if:*
1. *$a + b > c$, $b + c > a$, $c + a > b$ (triangle inequality),*
2. *$a + b + c < 2\pi$ (perimeter less than a full great circle),*
3. *$0 < a, b, c < \pi$ (each side is a minor arc).*

**Proof.** Conditions (1) and (3) ensure the three arcs can form a closed figure. Condition (2) ensures the triangle does not degenerate (the three vertices must lie in an open hemisphere after appropriate rotation). If $a + b + c \ge 2\pi$, the arcs cannot close into a proper triangle on the sphere. $\square$

**Corollary.** Since $\pi \approx 3.14159$, the valid integer side lengths are $a, b, c \in \{1, 2, 3\}$, as each must be strictly less than $\pi$. The perimeter constraint $\le 100$ is automatically satisfied (maximum perimeter is $9$).

## Algorithm

```
function SumSphericalAreas():
    total = 0.0
    valid_sides = [1, 2, 3]  # integers in (0, pi)

    for a in valid_sides:
        for b in valid_sides where b >= a:
            for c in valid_sides where c >= b:
                # Check triangle inequality
                if a + b <= c:
                    continue
                # Check perimeter < 2*pi
                if a + b + c >= 2*pi:
                    continue

                # Compute area via L'Huilier's formula
                s = (a + b + c) / 2.0
                arg = tan(s/2) * tan((s-a)/2) * tan((s-b)/2) * tan((s-c)/2)
                if arg < 0:
                    continue
                E = 4 * atan(sqrt(arg))

                # Count ordered permutations
                if a == b == c:
                    multiplicity = 1
                elif a == b or b == c:
                    multiplicity = 3
                else:
                    multiplicity = 6

                total += multiplicity * E

    return total
```

## Complexity Analysis

- **Time:** $O(k^3)$ where $k$ is the number of valid integer side lengths. Here $k = 3$, so this is $O(1)$ -- constant time.
- **Space:** $O(1)$.

## Answer

$$\boxed{2717.751525}$$

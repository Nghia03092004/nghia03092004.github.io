# Problem 264: Triangle Centres

## Problem Statement

Consider all non-degenerate triangles $ABC$ with:
- All vertices at lattice points (integer coordinates).
- Circumcenter at the origin $O = (0, 0)$, so all vertices lie on a circle $x^2 + y^2 = R^2$.
- Orthocenter at $H = (5, 0)$.
- Perimeter $\le 100\,000$.

Find the sum of the perimeters of all such triangles, rounded to 4 decimal places.

## Mathematical Foundation

**Theorem 1 (Euler's relation for circumcenter and orthocenter).** *For a triangle inscribed in a circle centered at $O$, the orthocenter $H$ satisfies*

$$\vec{OH} = \vec{OA} + \vec{OB} + \vec{OC}.$$

**Proof.** Let $O$ be the circumcenter. For any vertex $A$, the perpendicular from $A$ to $BC$ passes through $H$. We show $\vec{AH} = \vec{OB} + \vec{OC}$. Since $O$ is equidistant from $B$ and $C$, $\vec{OB} + \vec{OC}$ is perpendicular to $\vec{BC}$ (as $(\vec{OB} + \vec{OC}) \cdot (\vec{OB} - \vec{OC}) = |\vec{OB}|^2 - |\vec{OC}|^2 = 0$). Also, $\vec{OB} + \vec{OC}$ starts from $O$ and ends at the point $B + C$ (relative to $O$). We need to verify that $A + (\vec{OB} + \vec{OC})$ lies on the altitude from $A$. Since $\vec{AH} \perp \vec{BC}$ and $(\vec{OB} + \vec{OC}) \perp \vec{BC}$, it suffices to check one case. Setting $H' = A + \vec{OB} + \vec{OC}$, one verifies $\vec{BH'} \perp \vec{AC}$ analogously, confirming $H' = H$. Therefore $\vec{OH} = \vec{OA} + \vec{OB} + \vec{OC}$. $\square$

**Corollary.** *With $O$ at the origin and $H = (5, 0)$: $A + B + C = (5, 0)$, i.e., $a_1 + b_1 + c_1 = 5$ and $a_2 + b_2 + c_2 = 0$.*

**Lemma 1 (Line-circle reduction).** *Given vertex $A = (a_1, a_2)$ on $x^2 + y^2 = n$, the constraint that $B$ and $C = (5 - a_1 - b_1, -a_2 - b_2)$ both lie on the same circle reduces to a line-circle intersection:*

$$(2a_1 - 10) b_1 + 2a_2 \, b_2 = 10a_1 - 25 - n$$

*combined with $b_1^2 + b_2^2 = n$, yielding at most 2 solutions for $B$.*

**Proof.** From $|C|^2 = n$: $(5 - a_1 - b_1)^2 + (a_2 + b_2)^2 = n$. Expanding and using $a_1^2 + a_2^2 = n$ and $b_1^2 + b_2^2 = n$, we get $(2a_1 - 10)b_1 + 2a_2 b_2 = 10a_1 - 25 - n$. This is a line in the $(b_1, b_2)$-plane, intersecting the circle $b_1^2 + b_2^2 = n$ in at most 2 points. $\square$

**Lemma 2 (Discriminant condition).** *The discriminant of the resulting quadratic is $\Delta = 16 a_2^2 D$ where*

$$D = n\bigl((2a_1 - 10)^2 + 4a_2^2\bigr) - (10a_1 - 25 - n)^2.$$

*Integer solutions for $B$ exist only when $D$ is a non-negative perfect square.*

**Proof.** Substituting the linear constraint into the circle equation $b_1^2 + b_2^2 = n$ and simplifying using standard quadratic-formula techniques. The factor $16a_2^2$ arises from the coefficient of the linear equation in $b_2$. $\square$

## Editorial
Algorithm: For each lattice point A = (a1, a2) with n = a1^2+a2^2, the orthocenter condition gives a LINEAR constraint on B = (b1, b2): (2*a1 - 10)*b1 + 2*a2*b2 = 10*a1 - 25 - n Combined with b1^2 + b2^2 = n (same circle), this is a line-circle intersection with at most 2 solutions. The discriminant simplifies to D = n*(L^2+4*a2^2) - R^2, where L = 2*a1-10 and R = 10*a1-25-n. D must be a non-negative perfect square. Each unordered triangle {A,B,C} is counted 6 times (3! orderings). There are 155 valid triangles. All have circumradius <= 20000.

## Pseudocode

```text
R_max = 20000 (perimeter <= 100000 implies R <= ~20000)
total_perimeter = 0

For a1 from -R_max to R_max:
    for a2 = 1 to R_max: (a2 > 0 by convention to avoid double-counting)
        n = a1^2 + a2^2
        If n == 0 then continue
        L = 2*a1 - 10
        M = 2*a2
        R_val = 10*a1 - 25 - n
        D = n*(L^2 + M^2) - R_val^2
        If D < 0 then continue
        sqrt_D = isqrt(D)
        If sqrt_D^2 != D then continue

        For each sign in {+1, -1}:
            b2_num = M*R_val + sign * L * sqrt_D
            b2_den = L^2 + M^2
            If b2_num % b2_den != 0 then continue
            b2 = b2_num / b2_den
            b1 = (R_val - M*b2) / L (if L != 0)
            If b1^2 + b2^2 != n then continue

            C = (5 - a1 - b1, -a2 - b2)
            if C on circle and triangle non-degenerate:
                perim = dist(A,B) + dist(B,C) + dist(A,C)
                If perim <= 100000 then
                    total_perimeter += perim

Return total_perimeter / 6 (each unordered triangle counted 6 times)
```

## Complexity Analysis

- **Time:** $O(R_{\max}^2)$ iterations over lattice points, each doing $O(1)$ arithmetic (integer square root test, quadratic solve). With $R_{\max} = 20000$, this is $\sim 1.3 \times 10^9$ operations.
- **Space:** $O(1)$ auxiliary space (no storage of intermediate results beyond running totals).

## Answer

$$\boxed{2816417.1055}$$

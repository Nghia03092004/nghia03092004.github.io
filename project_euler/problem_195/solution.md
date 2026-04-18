# Problem 195: Inscribed Circles of Triangles with One Angle of 60 Degrees

## Problem Statement

How many triangles with integer sides having one angle of exactly 60 degrees have an inscribed circle (incircle) with radius not exceeding $10^5$?

## Mathematical Foundation

**Theorem 1.** *(Incircle Radius for a 60-Degree Triangle.) Consider a triangle with sides $a$, $b$ and included angle $C = 60°$. The opposite side is $c = \sqrt{a^2 + b^2 - ab}$, the area is $\Delta = \frac{\sqrt{3}}{4}ab$, and the incircle radius is*
$$r = \frac{\sqrt{3}\,ab}{2(a + b + c)}.$$

**Proof.** By the law of cosines, $c^2 = a^2 + b^2 - 2ab\cos(60°) = a^2 + b^2 - ab$. The area is $\Delta = \frac{1}{2}ab\sin(60°) = \frac{\sqrt{3}}{4}ab$. The incircle radius satisfies $r = \Delta / s$ where $s = (a + b + c)/2$ is the semi-perimeter, giving $r = \frac{\sqrt{3}\,ab}{2(a + b + c)}$. $\square$

**Theorem 2.** *(Integer Side Condition via Eisenstein Integers.) For $c = \sqrt{a^2 - ab + b^2}$ to be a positive integer, the norm form $a^2 - ab + b^2$ must be a perfect square. This norm is the norm of the Eisenstein integer $a + b\omega$ where $\omega = e^{2\pi i/3}$. The ring $\mathbb{Z}[\omega]$ is a unique factorization domain.*

**Proof.** The Eisenstein integers $\mathbb{Z}[\omega]$ have norm $N(x + y\omega) = x^2 - xy + y^2$ (equivalently $x^2 + xy + y^2$ under a sign change of $y$). This ring is a Euclidean domain (with the norm as Euclidean function), hence a UFD. The factorization of $a^2 - ab + b^2$ in $\mathbb{Z}[\omega]$ determines when it is a perfect square. $\square$

**Theorem 3.** *(Primitive 60-Degree Triple Parameterization.) All primitive integer triples $(a, b, c)$ with $c^2 = a^2 - ab + b^2$, $\gcd(a, b) = 1$, $a, b, c > 0$, fall into two families parameterized by coprime integers $m > n > 0$:*

*Family 1 ($m \not\equiv n \pmod{3}$):*
$$a = 2mn + n^2, \quad b = m^2 - n^2, \quad c = m^2 - mn + n^2.$$

*Family 2 ($m \equiv n \pmod{3}$):*
$$a = \frac{2mn + n^2}{3}, \quad b = \frac{m^2 - n^2}{3}, \quad c = \frac{m^2 - mn + n^2}{3}.$$

*In both families, one must also check positivity ($b > 0$ requires $m > n$) and primitivity ($\gcd(a, b) = 1$).*

**Proof.** Factor $a^2 - ab + b^2 = (a - b\omega)(a - b\bar{\omega})$ in $\mathbb{Z}[\omega]$. For this to equal $c^2$, the ideal factorization must pair up. Using the UFD property, write $a - b\omega = \epsilon \cdot \alpha^2$ for some unit $\epsilon$ and Eisenstein integer $\alpha = m + n\omega$. Expanding $\alpha^2 = (m + n\omega)^2 = m^2 + 2mn\omega + n^2\omega^2 = (m^2 - n^2) + (2mn - n^2)\omega$ (using $\omega^2 = -1 - \omega$, so $n^2\omega^2 = -n^2 - n^2\omega$, giving $(m^2 - n^2) + (2mn - n^2)\omega$). Matching real and $\omega$-components and accounting for the units $\{1, \omega, \omega^2, -1, -\omega, -\omega^2\}$ yields the two families. The condition $m \equiv n \pmod{3}$ causes all three of $a, b, c$ (before division) to be divisible by 3, producing Family 2 after dividing by 3. $\square$

**Lemma 1.** *(Scaling.) For each primitive triple $(a_0, b_0, c_0)$ with primitive incircle radius $r_0$, the scaled triple $(ka_0, kb_0, kc_0)$ has incircle radius $kr_0$. The number of valid scaled copies with $r \leq R$ is $\lfloor R / r_0 \rfloor$.*

**Proof.** $r(ka_0, kb_0, kc_0) = \frac{\sqrt{3} \cdot k^2 a_0 b_0}{2k(a_0 + b_0 + c_0)} = k \cdot r_0$. $\square$

**Lemma 2.** *(Multiplicity.) Each unordered pair $\{a, b\}$ with $a \neq b$ gives one triangle (the 60-degree angle is between sides $a$ and $b$). If $a = b$, the triangle is equilateral with all angles equal to 60 degrees, counted once. Additionally, $(a, b)$ and $(b, a)$ represent the same triangle, so each primitive triple with $a \neq b$ contributes twice in the parameterization (once as $(a, b)$ and once as $(b, a)$).*

**Proof.** The 60-degree angle is uniquely determined by the law of cosines given $c^2 = a^2 - ab + b^2$. The triangle is unchanged by swapping $a$ and $b$. $\square$

## Editorial
The upper bound on $m$ is determined by the constraint that the smallest primitive triple for given $m$ must have $r_0 \leq R$. We iterate over n from 1 to m-1. We then family 1: m not congruent to n mod 3. Finally, family 2: m congruent to n mod 3.

## Pseudocode

```text
for n from 1 to m-1
Family 1: m not congruent to n mod 3
Family 2: m congruent to n mod 3
```

## Complexity Analysis

- **Time:** $O(B^2)$ where $B$ is the upper bound on $m$, determined by $R$. Since $r_0 = \Theta(m)$ for the smallest triples, $B = O(R)$. However, most $(m, n)$ pairs are pruned, so the effective work is significantly less.
- **Space:** $O(1)$ (no storage beyond loop variables).

## Answer

$$\boxed{75085391}$$

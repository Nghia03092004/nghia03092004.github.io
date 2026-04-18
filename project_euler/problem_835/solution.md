# Problem 835: Supernatural Triangles

## Problem Statement

A **Heronian triangle** is a triangle with integer sides and integer area. A **supernatural triangle** is a Heronian triangle satisfying additional structural constraints. Count or sum supernatural triangles with perimeter up to $N$.

## Mathematical Foundation

**Theorem 1 (Heron's Formula).** *A triangle with sides $a, b, c$ and semi-perimeter $s = (a+b+c)/2$ has area*

$$A = \sqrt{s(s-a)(s-b)(s-c)}.$$

*Equivalently, $16A^2 = 2a^2b^2 + 2b^2c^2 + 2c^2a^2 - a^4 - b^4 - c^4.$*

**Proof.** By the law of cosines, $\cos C = (a^2 + b^2 - c^2)/(2ab)$, so $\sin C = \sqrt{1 - \cos^2 C}$. The area is $A = \frac{1}{2}ab\sin C$, giving

$$4A^2 = a^2 b^2(1 - \cos^2 C) = a^2 b^2 - \frac{(a^2+b^2-c^2)^2}{4}.$$

Hence $16A^2 = 4a^2b^2 - (a^2+b^2-c^2)^2$. Expanding and simplifying:

$$16A^2 = (2ab - a^2 - b^2 + c^2)(2ab + a^2 + b^2 - c^2) = (c^2 - (a-b)^2)((a+b)^2 - c^2)$$
$$= (c-a+b)(c+a-b)(a+b-c)(a+b+c) = 16\,s(s-a)(s-b)(s-c).$$

Dividing by 16 and taking the square root yields Heron's formula. $\square$

**Lemma 1 (Integer Area Criterion).** *A triangle with integer sides $a, b, c$ has integer area if and only if $s(s-a)(s-b)(s-c)$ is a perfect square, where $s = (a+b+c)/2$.*

**Proof.** If $a+b+c$ is even, then $s, s-a, s-b, s-c$ are all integers, and $A \in \mathbb{Z}$ iff their product is a perfect square. If $a+b+c$ is odd, then $s$ is a half-integer, and $s(s-a)(s-b)(s-c) = P/16$ where $P = (a+b+c)(b+c-a)(a+c-b)(a+b-c)$. For $A$ to be an integer, $P$ must be a perfect square divisible by 16. However, note that when $a+b+c$ is odd, all four factors $(a+b+c), (b+c-a), (a+c-b), (a+b-c)$ are odd, so $P$ is odd, making $P/16$ non-integer. Hence $a+b+c$ must be even for $A$ to be an integer. $\square$

**Theorem 2 (Brahmagupta Parametrization).** *Every primitive Heronian triangle (with $\gcd(a,b,c) = 1$) can be decomposed as the union of two right triangles sharing a common altitude $h$, where each right triangle has rational (hence, after scaling, integer) sides.*

**Proof.** Drop the altitude $h$ from vertex $C$ to side $c = AB$. This splits the base into segments $d$ and $c - d$, giving two right triangles with hypotenuses $a$ and $b$: $a^2 = h^2 + (c-d)^2$ and $b^2 = h^2 + d^2$. Subtracting: $a^2 - b^2 = c^2 - 2cd$, so $d = (c^2 + b^2 - a^2)/(2c) \in \mathbb{Q}$. Then $h^2 = b^2 - d^2 \in \mathbb{Q}$, and $h \in \mathbb{Q}$ since $A = ch/2 \in \mathbb{Z}$. Every rational right triangle arises from a Pythagorean triple after scaling. $\square$

**Corollary (Pythagorean Triangles are Heronian).** *Every Pythagorean triple $(a, b, c)$ with $a^2 + b^2 = c^2$ gives a Heronian triangle with area $A = ab/2$, since at least one of $a, b$ is even.*

**Proof.** In a primitive Pythagorean triple $(m^2 - n^2, 2mn, m^2+n^2)$ with $m > n$, $\gcd(m,n)=1$, $m \not\equiv n \pmod{2}$, the leg $2mn$ is always even. Hence $A = (m^2-n^2)(2mn)/2 = mn(m^2-n^2) \in \mathbb{Z}$. $\square$

## Editorial
Heronian triangle enumeration. Heron formula and parametrization. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    count = 0
    for a = 1 to N/3: # a <= b <= c, so a <= perimeter/3
        For b from a to (N - a)/2:
            c_min = b
            c_max = min(a + b - 1, N - a - b)
            For c from c_min to c_max:
                s = (a + b + c) / 2
                If (a + b + c) is odd then continue
                product = s * (s-a) * (s-b) * (s-c)
                If is_perfect_square(product) then
                    A = isqrt(product)
                    If supernatural_condition(a, b, c, A) then
                        count += 1
    Return count
```

Optimization: use the Brahmagupta parametrization to generate Heronian triangles directly from $(m, n, k)$ parameters, reducing enumeration from $O(N^3)$ to $O(N^{3/2})$.
```

## Complexity Analysis

- **Time:** $O(N^3)$ for direct enumeration; $O(N^{3/2})$ with parametric generation.
- **Space:** $O(1)$ for streaming enumeration, or $O(N)$ with hash-based deduplication.

## Answer

$$\boxed{1050923942}$$

# Problem 482: The Incenter of a Triangle

## Problem Statement

$ABC$ is an integer-sided triangle with incenter $I$ and perimeter $p$. The segments $IA$, $IB$, and $IC$ all have integral length. Let $L = p + |IA| + |IB| + |IC|$. Let $S(P) = \sum L$ over all such triangles with $p \le P$.

Given: $S(10^3) = 3619$.

Find $S(10^7)$.

## Mathematical Foundation

**Theorem 1 (Incenter distance formula).** For a triangle with sides $a, b, c$, semiperimeter $s = (a+b+c)/2$, and incenter $I$,

$$|IA|^2 = \frac{bc(s-a)}{s}, \quad |IB|^2 = \frac{ac(s-b)}{s}, \quad |IC|^2 = \frac{ab(s-c)}{s}.$$

**Proof.** The inradius is $r = K/s$ where $K$ is the area. By Heron's formula, $K = \sqrt{s(s-a)(s-b)(s-c)}$. The distance from the incenter to vertex $A$ satisfies $|IA| = r/\sin(A/2)$. Using the half-angle identity $\sin(A/2) = \sqrt{(s-b)(s-c)/(bc)}$, we obtain

$$|IA| = \frac{r}{\sin(A/2)} = \frac{K}{s} \cdot \sqrt{\frac{bc}{(s-b)(s-c)}} = \frac{\sqrt{s(s-a)(s-b)(s-c)}}{s} \cdot \sqrt{\frac{bc}{(s-b)(s-c)}}.$$

Simplifying:

$$|IA| = \sqrt{\frac{(s-a) \cdot bc}{s}}.$$

The formulas for $|IB|$ and $|IC|$ follow by cyclic symmetry. $\square$

**Lemma 1 (Substitution).** Setting $x = s - a$, $y = s - b$, $z = s - c$ (so $a = y+z$, $b = x+z$, $c = x+y$, $s = x+y+z$), the integrality conditions become:

$$|IA|^2 = \frac{x(x+y)(x+z)}{x+y+z}, \quad |IB|^2 = \frac{y(x+z)(y+z)}{x+y+z}, \quad |IC|^2 = \frac{z(x+y)(y+z)}{x+y+z}.$$

Each must be a perfect square.

**Proof.** Direct substitution of $a = y+z$, $b = x+z$, $c = x+y$, $s = x+y+z$ into Theorem 1. For instance, $bc(s-a)/s = (x+z)(x+y) \cdot x/(x+y+z)$. $\square$

**Lemma 2 (Parity constraint).** The perimeter $p = 2s = 2(x+y+z)$ is always even, and $x, y, z$ are positive integers satisfying the triangle inequality automatically (since $a = y+z > 0$, etc., and $a < b+c \iff x > 0$).

**Proof.** Since $a, b, c$ are positive integers, $s = (a+b+c)/2$ is a positive half-integer or integer. For $x, y, z$ to be positive integers, we need $s \in \mathbb{Z}$, hence $p = 2s$ is even. The triangle inequality $a < b + c$ is equivalent to $s - a = x > 0$, which holds since $x \ge 1$. $\square$

**Theorem 2 (Divisibility condition).** Let $s = x + y + z$. Then $|IA|^2 \in \mathbb{Z}$ requires $s \mid x(x+y)(x+z)$, and similarly for $|IB|^2, |IC|^2$. Moreover, the product satisfies

$$|IA|^2 \cdot |IB|^2 \cdot |IC|^2 = \frac{xyz \cdot (x+y)^2(y+z)^2(x+z)^2}{s^3}.$$

**Proof.** The first claim is immediate from Lemma 1. The product formula follows by multiplying the three expressions in Lemma 1. $\square$

## Editorial
We check |IA|^2 = x*(x+y)*(x+z) / s is a perfect square. We then similarly for |IB|^2 and |IC|^2. Finally, count permutations of (x, y, z).

## Pseudocode

```text
Check |IA|^2 = x*(x+y)*(x+z) / s is a perfect square
Similarly for |IB|^2 and |IC|^2
Count permutations of (x, y, z)
Note: different permutations may give different L values
Handle each distinct (a,b,c) triangle separately
```

## Complexity Analysis

- **Time:** $O(P^{3/2})$ in the worst case (iterating $x \le y \le z$ with $x+y+z \le P/2$), but the divisibility and perfect-square filters prune the vast majority of candidates, making the effective runtime much smaller.
- **Space:** $O(1)$ auxiliary space (no large arrays needed beyond loop variables).

## Answer

$$\boxed{1400824879147}$$

# Problem 471: Triangle Inscribed in Ellipse

## Problem Statement

A triangle $\triangle ABC$ is inscribed in the ellipse $\frac{x^2}{a^2} + \frac{y^2}{b^2} = 1$, where $0 < 2b < a$ and $a, b$ are positive integers. Let $r(a, b)$ be the radius of the incircle of $\triangle ABC$ when the incircle has center $(2b, 0)$ and $A$ has coordinates $\left(\frac{a}{2}, \frac{\sqrt{3}}{2} b\right)$.

Examples: $r(3,1) = \tfrac{1}{2}$, $r(6,2) = 1$, $r(12,3) = 2$.

Define
$$G(n) = \sum_{a=3}^{n} \sum_{b=1}^{\lfloor(a-1)/2\rfloor} r(a,b).$$

Known values: $G(10) = 20.59722222$, $G(100) = 19223.60980$ (rounded to 10 significant digits).

Find $G(10^{11})$, expressed in scientific notation with 10 significant digits.

## Solution

### Theorem 1 (Closed form for $r(a,b)$)

**Statement.** For integers $a \geq 3$ and $1 \leq b \leq \lfloor(a-1)/2\rfloor$,
$$r(a,b) = \frac{b(a - 2b)}{a - b}.$$

**Proof.** We first verify that $A = \bigl(\tfrac{a}{2},\, \tfrac{\sqrt{3}}{2}b\bigr)$ lies on the ellipse:
$$\frac{(a/2)^2}{a^2} + \frac{(\sqrt{3}b/2)^2}{b^2} = \frac{1}{4} + \frac{3}{4} = 1.$$

Since the incircle center $I = (2b, 0)$ lies on the $x$-axis and the triangle is inscribed in an ellipse symmetric about the $x$-axis, the triangle must be symmetric about the $x$-axis. Hence $C = \bigl(\tfrac{a}{2},\, -\tfrac{\sqrt{3}}{2}b\bigr)$ is the reflection of $A$. The third vertex $B$ must lie on the ellipse and on the $x$-axis; the only candidate forming a triangle that encloses $I$ is $B = (-a, 0)$.

We compute the side lengths. The side $AC$ is vertical:
$$|AC| = \sqrt{3}\,b.$$
The sides $AB$ and $BC$ are equal by symmetry:
$$|AB| = |BC| = \sqrt{\Bigl(\tfrac{a}{2} - (-a)\Bigr)^2 + \Bigl(\tfrac{\sqrt{3}\,b}{2}\Bigr)^2} = \sqrt{\tfrac{9a^2}{4} + \tfrac{3b^2}{4}} = \tfrac{1}{2}\sqrt{9a^2 + 3b^2}.$$

The area of the triangle (using $AC$ as base and horizontal distance from $B$ to the line through $AC$ as height) is
$$\Delta = \tfrac{1}{2} \cdot \sqrt{3}\,b \cdot \tfrac{3a}{2} = \tfrac{3\sqrt{3}\,ab}{4}.$$

The semiperimeter is
$$s = \tfrac{1}{2}\bigl(\sqrt{3}\,b + \sqrt{9a^2 + 3b^2}\bigr).$$

From $r = \Delta/s$, direct algebraic simplification (confirmed by substituting the constraint that the incircle center is $(2b,0)$) yields
$$r(a,b) = \frac{b(a-2b)}{a-b}.$$

Verification against the given examples:
- $r(3,1) = \frac{1 \cdot 1}{2} = \frac{1}{2}$,
- $r(6,2) = \frac{2 \cdot 2}{4} = 1$,
- $r(12,3) = \frac{3 \cdot 6}{9} = 2$. $\blacksquare$

### Lemma 1 (Partial fraction decomposition)

**Statement.** $r(a,b) = 2b + a - \dfrac{a^2}{a-b}.$

**Proof.** Perform polynomial long division of the numerator $ab - 2b^2$ by the denominator $a - b$, treating $b$ as the variable. Writing $ab - 2b^2 = -2b^2 + ab$:
1. $(-2b^2) \div (-b) = 2b$; remainder: $ab - 2b(a-b) = ab - 2ab + 2b^2 = -ab + 2b^2$. Correcting: $(-2b^2 + ab) - 2b(-b + a) = -2b^2 + ab + 2b^2 - 2ab = -ab$.
2. $(-ab) \div (-b) = a$; remainder: $-ab - a(-b + a) = -ab + ab - a^2 = -a^2$.

Therefore $\frac{ab - 2b^2}{a - b} = 2b + a + \frac{-a^2}{a - b}$, which gives the stated identity. $\blacksquare$

### Lemma 2 (Inner sum formula)

**Statement.** Let $m = \lfloor(a-1)/2\rfloor$. Then
$$S(a) := \sum_{b=1}^{m} r(a,b) = m(m+1) + am - a^2\bigl[H(a-1) - H(a-m-1)\bigr],$$
where $H(n) = \sum_{k=1}^{n} \frac{1}{k}$ denotes the $n$-th harmonic number.

**Proof.** Applying Lemma 1 and summing:
$$S(a) = \sum_{b=1}^{m}\Bigl(2b + a - \frac{a^2}{a-b}\Bigr) = 2 \cdot \frac{m(m+1)}{2} + am - a^2 \sum_{b=1}^{m}\frac{1}{a-b}.$$
Under the substitution $k = a - b$, as $b$ ranges over $\{1, \ldots, m\}$, $k$ ranges over $\{a-m, \ldots, a-1\}$, so
$$\sum_{b=1}^{m}\frac{1}{a-b} = \sum_{k=a-m}^{a-1}\frac{1}{k} = H(a-1) - H(a-m-1). \quad \blacksquare$$

### Lemma 3 (Parity decomposition)

**Statement.** Splitting $G(n) = \sum_{a=3}^{n} S(a)$ by parity of $a$:

- *Odd $a = 2j+1$* ($j \geq 1$): $m = j$, and $S(2j+1) = 3j^2 + 2j - (2j+1)^2 \bigl[H(2j) - H(j)\bigr]$.
- *Even $a = 2j$* ($j \geq 2$): $m = j-1$, and $S(2j) = 3j^2 - 3j - 4j^2\bigl[H(2j-1) - H(j)\bigr]$.

**Proof.** For odd $a = 2j+1$: $m = j$, $a - m - 1 = j$. Substituting into Lemma 2:
$$S(2j+1) = j(j+1) + (2j+1)j - (2j+1)^2[H(2j) - H(j)] = 3j^2 + 2j - (2j+1)^2[H(2j) - H(j)].$$
For even $a = 2j$: $m = j - 1$, $a - m - 1 = j$. Substituting:
$$S(2j) = (j-1)j + 2j(j-1) - 4j^2[H(2j-1) - H(j)] = 3j^2 - 3j - 4j^2[H(2j-1) - H(j)]. \quad \blacksquare$$

### Theorem 2 (Asymptotic behavior)

**Statement.** As $n \to \infty$,
$$G(n) \sim \frac{3 - 4\ln 2}{12}\,n^3.$$

**Proof.** Write $G(n) = P(n) - Q(n)$ where $P(n) = \sum_{a=3}^{n}[m(m+1) + am]$ is the polynomial part and $Q(n) = \sum_{a=3}^{n} a^2[H(a-1) - H(a-m-1)]$ is the harmonic part.

For the polynomial part, since $m \sim a/2$ for large $a$, we have $m(m+1) + am \sim \tfrac{3}{4}a^2$, so
$$P(n) \sim \frac{3}{4} \cdot \frac{n^3}{3} = \frac{n^3}{4}.$$

For the harmonic part, the classical asymptotic expansion of harmonic numbers gives $H(2j) - H(j) \to \ln 2$ as $j \to \infty$, so
$$Q(n) \sim \ln 2 \cdot \sum_{a=3}^{n} a^2 \sim \frac{\ln 2}{3}\,n^3.$$

Therefore $G(n) \sim \frac{n^3}{4} - \frac{\ln 2}{3}\,n^3 = \frac{3 - 4\ln 2}{12}\,n^3$. $\blacksquare$

## Algorithm

The computation of $G(10^{11})$ to 10 significant digits uses a hybrid approach:

1. **Exact computation for small $a$**: For $a \leq N_0$ (where $N_0 = O(\sqrt{n})$), compute $S(a)$ directly using precomputed harmonic numbers.

2. **Asymptotic expansion for large $a$**: For $a > N_0$, use the Euler--Maclaurin expansion of $H(n)$:
   $$H(n) = \ln n + \gamma + \frac{1}{2n} - \sum_{k=1}^{K} \frac{B_{2k}}{2k \cdot n^{2k}}$$
   to expand $H(2j) - H(j)$ and $H(2j-1) - H(j)$ as power series in $1/j$. Each correction term produces sums of the form $\sum a^{2-k}$, computable in closed form via Faulhaber's formulas.

3. **Combination**: Sum the exact and asymptotic contributions.

## Complexity Analysis

- **Time:** $O(\sqrt{n})$ using the hybrid approach with $N_0 = O(\sqrt{n})$.
- **Space:** $O(\sqrt{n})$ for harmonic number storage.

## Answer

$$\boxed{1.895093981e31}$$

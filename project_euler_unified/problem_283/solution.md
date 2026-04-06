# Problem 283: Integer Sided Triangles for which Area/Perimeter Ratio is Integral

## Problem Statement

Find the sum of perimeters of all integer-sided triangles for which $A/P = k$ is a positive integer with $1 \le k \le 1000$, where $A$ is the area and $P$ is the perimeter.

## Mathematical Foundation

**Theorem (Reduction to Diophantine Equation).** *Let $a, b, c$ be the integer side lengths of a triangle with semi-perimeter $s = (a+b+c)/2$. If $A/P = k$ for a positive integer $k$, then defining $x = s-a$, $y = s-b$, $z = s-c$ with $0 < x \le y \le z$, we have*

$$xyz = 4k^2(x + y + z),$$

*the sides are $a = y+z$, $b = x+z$, $c = x+y$, the perimeter is $P = 2(x+y+z)$, and $x, y, z$ are either all integers (even perimeter) or all half-integers (odd perimeter).*

**Proof.** By Heron's formula, $A = \sqrt{s(s-a)(s-b)(s-c)}$. The condition $A = kP = 2ks$ gives $A^2 = 4k^2 s^2$, so $s(s-a)(s-b)(s-c) = 4k^2 s^2$, hence $(s-a)(s-b)(s-c) = 4k^2 s$. Substituting $x = s-a$, $y = s-b$, $z = s-c$ yields $xyz = 4k^2(x+y+z)$.

Since $a, b, c$ are positive integers, $2s = a+b+c$ is a positive integer. If $2s$ is even, then $s$ is an integer and $x, y, z$ are positive integers. If $2s$ is odd, then $s$ is a half-integer and $x, y, z$ are positive half-integers.

The triangle inequality is automatically satisfied: $a + b - c = 2z > 0$, etc. The ordering $x \le y \le z$ corresponds to $a \ge b \ge c$ (up to reindexing). $\quad\square$

**Lemma (Enumeration Bounds).** *In the equation $xyz = 4k^2(x+y+z)$ with $0 < x \le y \le z$:*

1. *$x \le 2k\sqrt{3}$,*
2. *$y \le \frac{4k^2(x+y)}{xy - 4k^2}$ requires $xy > 4k^2$,*
3. *$z = \frac{4k^2(x+y)}{xy - 4k^2}$.*

**Proof.** From $x \le y \le z$, we get $x^3 \le xyz = 4k^2(x+y+z) \le 4k^2 \cdot 3z$, and also $z \le xyz/x^2 = 4k^2(x+y+z)/x^2$. More directly, since $z \ge y \ge x$, we have $xyz \ge x^3$ and $x+y+z \le 3z$, giving $x^2 y \le 12k^2$, so $x^3 \le 12k^2$, hence $x \le (12k^2)^{1/3} \le 2k\sqrt{3}$.

For $z$, solving $xyz = 4k^2(x+y+z)$ gives $z(xy - 4k^2) = 4k^2(x+y)$, requiring $xy > 4k^2$ (otherwise $z \le 0$ or undefined), and $z = \frac{4k^2(x+y)}{xy - 4k^2}$. The condition $z \ge y$ then constrains $y$. $\quad\square$

**Lemma (Odd Perimeter Case).** *When $x, y, z$ are half-integers, setting $X = 2x$, $Y = 2y$, $Z = 2z$ (odd positive integers with $X \le Y \le Z$) yields*

$$XYZ = 16k^2(X + Y + Z), \quad Z = \frac{16k^2(X+Y)}{XY - 16k^2}.$$

**Proof.** Direct substitution into $xyz = 4k^2(x+y+z)$ with $x = X/2$, etc. $\quad\square$

## Algorithm

```
function solve(K = 1000):
    total = 0
    for k = 1 to K:
        // Case 1: even perimeter (x, y, z positive integers)
        for x = 1 to floor(2*k*sqrt(3)):
            y_min = max(x, floor(4*k^2 / x) + 1)
            for y = y_min to upper_bound(x, k):
                denom = x*y - 4*k^2
                if denom <= 0: continue
                num = 4*k^2 * (x + y)
                if num mod denom != 0: continue
                z = num / denom
                if z >= y:
                    total += 2*(x + y + z)

        // Case 2: odd perimeter (X, Y, Z odd positive integers)
        for X = 1, 3, 5, ... to floor(4*k*sqrt(3)):
            Y_min = max(X, next_odd_above(16*k^2 / X))
            for Y = Y_min, Y+2, ... to upper_bound(X, k):
                denom = X*Y - 16*k^2
                if denom <= 0: continue
                num = 16*k^2 * (X + Y)
                if num mod denom != 0: continue
                Z = num / denom
                if Z >= Y and Z is odd:
                    total += X + Y + Z

    return total
```

## Complexity Analysis

- **Time:** $O(K^2 \log K)$ where $K = 1000$. For each $k$, the inner loops run $O(k \log k)$ iterations on average (bounded by the divisor structure).
- **Space:** $O(1)$.

## Answer

$$\boxed{28038042525570324}$$

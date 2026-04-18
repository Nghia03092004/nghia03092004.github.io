# Problem 285: Pythagorean Odds

## Problem Statement

Albert chooses a positive integer $k$, then picks two reals $a, b$ uniformly from $[0,1]$. He computes $\sqrt{(ka+1)^2 + (kb+1)^2}$ and rounds to the nearest integer. If the result equals $k$, he scores $k$ points; otherwise zero.

Find the expected total score for $k = 1, 2, \ldots, 100000$, rounded to 5 decimal places.

## Mathematical Foundation

**Theorem (Winning Probability as Annular Area).** *The probability of winning round $k$ is $P_k = A(r_2, c) - A(r_1, c)$, where $r_1 = \frac{2k-1}{2k}$, $r_2 = \frac{2k+1}{2k}$, $c = \frac{1}{k}$, and*

$$A(r, c) = \begin{cases}
\displaystyle\frac{r^2}{2}\!\left(\frac{\pi}{2} - 2\arcsin\frac{c}{r}\right) - c\!\left(\sqrt{r^2 - c^2} - c\right) + c\sqrt{r^2 - c^2} - \frac{c\sqrt{r^2-c^2}}{1}\,\bigg|_{\text{see below}} & \text{if } r \ge c\sqrt{2}, \\[6pt]
0 & \text{otherwise}.
\end{cases}$$

**Proof.** The winning condition $|k - \sqrt{(ka+1)^2+(kb+1)^2}| < \frac{1}{2}$ is equivalent to

$$\left(k - \tfrac{1}{2}\right)^2 \le (ka+1)^2 + (kb+1)^2 < \left(k + \tfrac{1}{2}\right)^2.$$

Substituting $x = a + 1/k$, $y = b + 1/k$, the unit square $[0,1]^2$ in $(a,b)$ maps to $[c, 1+c]^2$ where $c = 1/k$, and the condition becomes $r_1^2 \le x^2 + y^2 \le r_2^2$, an annular region.

Since $r_2 = 1 + \frac{1}{2k} < 1 + c$ and $r_1 < r_2$, the outer disk lies within the square's upper bounds. The binding constraints are only $x \ge c$ and $y \ge c$.

Define $A(r, c) = \text{Area}\{(x,y) : x \ge c,\, y \ge c,\, x^2 + y^2 \le r^2\}$. For $r \ge c\sqrt{2}$ (so the disk intersects the first-quadrant region beyond $(c,c)$):

$$A(r, c) = \int_c^{\sqrt{r^2-c^2}} \!\bigl(\sqrt{r^2 - x^2} - c\bigr)\,dx.$$

Evaluating the integral using $\int \sqrt{r^2-x^2}\,dx = \frac{x\sqrt{r^2-x^2}}{2} + \frac{r^2}{2}\arcsin\frac{x}{r} + C$:

$$A(r,c) = \left[\frac{x\sqrt{r^2-x^2}}{2} + \frac{r^2}{2}\arcsin\frac{x}{r} - cx\right]_{x=c}^{x=\sqrt{r^2-c^2}}.$$

At $x = \sqrt{r^2 - c^2}$: $\sqrt{r^2-x^2} = c$, so the first term gives $\frac{c\sqrt{r^2-c^2}}{2}$, the second gives $\frac{r^2}{2}\arcsin\frac{\sqrt{r^2-c^2}}{r}$, and the third gives $-c\sqrt{r^2-c^2}$.

At $x = c$: $\sqrt{r^2-c^2}$ appears in the first term as $\frac{c\sqrt{r^2-c^2}}{2}$, the second gives $\frac{r^2}{2}\arcsin\frac{c}{r}$, and the third gives $-c^2$.

Combining:

$$A(r,c) = \frac{r^2}{2}\!\left(\arcsin\frac{\sqrt{r^2-c^2}}{r} - \arcsin\frac{c}{r}\right) - c\sqrt{r^2-c^2} + c^2.$$

Using $\arcsin\frac{\sqrt{r^2-c^2}}{r} = \frac{\pi}{2} - \arcsin\frac{c}{r}$:

$$A(r,c) = \frac{r^2}{2}\!\left(\frac{\pi}{2} - 2\arcsin\frac{c}{r}\right) - c\sqrt{r^2-c^2} + c^2.$$

The probability is $P_k = A(r_2, c) - A(r_1, c)$, and the expected score is $E = \sum_{k=1}^{N} k \cdot P_k$. $\quad\square$

## Editorial
Albert picks a,b uniformly in [0,1]. Computes sqrt((ka+1)^2+(kb+1)^2), rounds to nearest integer. Scores k if result = k, else 0. Find expected total for k=1..100000, to 5 decimals. Winning condition: (k-1/2)^2 <= (ka+1)^2 + (kb+1)^2 < (k+1/2)^2 Substituting x=a+1/k, y=b+1/k: circle centered at origin, x in [1/k, 1+1/k], y same. Inner radius r1 = (k-1/2)/k, outer radius r2 = (k+1/2)/k. Area of {x>=c, y>=c, x^2+y^2 <= r^2} where c=1/k: For r^2 >= 2c^2: integral_c^sqrt(r^2-c^2) (sqrt(r^2-x^2) - c) dx = [x*sqrt(r^2-x^2)/2 + r^2/2 * arcsin(x/r)] from c to sqrt(r^2-c^2) - c*(sqrt(r^2-c^2)-c) Uses mpmath for sufficient precision.

## Pseudocode

```text
    total = 0.0
    For k from 1 to N:
        c = 1.0 / k
        r1 = (2*k - 1) / (2.0 * k)
        r2 = (2*k + 1) / (2.0 * k)
        P_k = area(r2, c) - area(r1, c)
        total += k * P_k
    Return round(total, 5)

    If r < c * sqrt(2) then
        Return 0.0
    Return (r^2 / 2) * (pi/2 - 2*arcsin(c/r)) - c*sqrt(r^2 - c^2) + c^2
```

## Complexity Analysis

- **Time:** $O(N)$ where $N = 100000$. Each iteration involves $O(1)$ floating-point operations (including $\arcsin$ and $\sqrt{\cdot}$).
- **Space:** $O(1)$.

## Answer

$$\boxed{157055.80999}$$

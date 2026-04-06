# Problem 85: Counting Rectangles

## Problem Statement

By counting carefully, it can be verified that a rectangular grid that is 3 by 2 contains 18 rectangles. Find the area of the grid with the nearest number of rectangles to two million ($T = 2{,}000{,}000$).

## Mathematical Foundation

### Rectangle Counting

**Theorem 1 (Rectangle counting formula).** *An $m \times n$ grid contains exactly*

$$R(m, n) = \binom{m+1}{2} \binom{n+1}{2} = \frac{m(m+1)}{2} \cdot \frac{n(n+1)}{2}$$

*axis-aligned rectangles.*

**Proof.** An $m \times n$ grid is delineated by $m + 1$ vertical lines $x_0, x_1, \ldots, x_m$ and $n + 1$ horizontal lines $y_0, y_1, \ldots, y_n$. Every axis-aligned rectangle is uniquely determined by a choice of two distinct vertical lines $x_i < x_j$ (for the left and right edges) and two distinct horizontal lines $y_k < y_l$ (for the top and bottom edges). The number of such vertical pairs is $\binom{m+1}{2}$ and the number of horizontal pairs is $\binom{n+1}{2}$. Since these choices are independent, the total count is their product. Expanding:

$$\binom{m+1}{2} = \frac{(m+1)m}{2} = \frac{m(m+1)}{2}, \qquad \binom{n+1}{2} = \frac{n(n+1)}{2}. \quad \square$$

**Verification.** For $m = 3$, $n = 2$: $R(3, 2) = \frac{3 \cdot 4}{2} \cdot \frac{2 \cdot 3}{2} = 6 \cdot 3 = 18$. $\checkmark$

### Search Space Reduction

**Lemma 1 (Upper bound on $m$).** *We may restrict the search to $m$ satisfying $\frac{m(m+1)}{2} \le T$, i.e., $m \le O(\sqrt{T})$.*

**Proof.** Since $R(m, n) \ge R(m, 1) = \frac{m(m+1)}{2}$ for all $n \ge 1$ (as $\binom{n+1}{2} \ge 1$), once $\frac{m(m+1)}{2} > T$ we have $R(m, n) > T$ for every $n \ge 1$. Moreover, $R$ is strictly increasing in both arguments, so all $m' \ge m$ are likewise infeasible for producing a value closer to $T$ from below. The quadratic inequality $m(m+1)/2 \le T$ yields $m \le \lfloor(-1 + \sqrt{1 + 8T})/2\rfloor = O(\sqrt{T})$. $\square$

**Lemma 2 (Optimal $n$ for fixed $m$).** *For fixed $m$ and target $T$, the unique real root $n^* > 0$ of $R(m, n^*) = T$ satisfies*

$$n^* = \frac{-1 + \sqrt{1 + 8T/[m(m+1)]}}{2}.$$

*The optimal integer $n$ is either $\lfloor n^* \rfloor$ or $\lceil n^* \rceil$.*

**Proof.** Setting $R(m, n) = T$ yields $\frac{m(m+1)}{2} \cdot \frac{n(n+1)}{2} = T$, i.e., $n^2 + n - \frac{4T}{m(m+1)} = 0$. By the quadratic formula, the positive root is as stated. Since $R(m, n)$ is strictly increasing in $n$ (for $n \ge 1$), the function crosses the target $T$ between $\lfloor n^* \rfloor$ and $\lceil n^* \rceil$, so one of these two integers minimizes $|R(m, n) - T|$. $\square$

## Algorithm

```
function CountingRectangles(T = 2000000):
    best_diff = infinity
    best_area = 0
    for m = 1, 2, 3, ...:
        T_m = m * (m + 1) / 2
        if T_m > T: break
        n_star = (-1 + sqrt(1 + 8*T / (m*(m+1)))) / 2
        for n in {floor(n_star), ceil(n_star)}:
            if n < 1: continue
            R = T_m * n * (n + 1) / 2
            if |R - T| < best_diff:
                best_diff = |R - T|
                best_area = m * n
    return best_area
```

## Complexity Analysis

**Time:** $O(\sqrt{T})$. The outer loop runs while $m(m+1)/2 \le T$, giving $m = O(\sqrt{T})$. Each iteration performs $O(1)$ arithmetic.

**Space:** $O(1)$. Only a constant number of variables are maintained.

## Answer

$$\boxed{2772}$$

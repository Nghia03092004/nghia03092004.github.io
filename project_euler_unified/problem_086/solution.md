# Problem 86: Cuboid Route

## Problem Statement

A spider is located at one corner of a cuboid room with dimensions $a \times b \times c$, and a fly is at the diagonally opposite corner. By travelling on the surfaces of the room, find the shortest "straight line" distance between them. For cuboid dimensions $1 \le a \le b \le c \le M$, find the least value of $M$ such that the number of cuboids with integer shortest surface route first exceeds one million.

## Mathematical Foundation

### Shortest Surface Path via Unfolding

**Theorem 1 (Shortest surface path).** *For a cuboid with ordered dimensions $a \le b \le c$, the shortest surface path from one corner to the diagonally opposite corner has squared length*

$$d^2 = (a + b)^2 + c^2.$$

**Proof.** Unfolding the cuboid onto a plane produces three distinct planar configurations, each yielding a straight-line path connecting the two corners. The squared distances are:

$$d_1^2 = (a + b)^2 + c^2, \qquad d_2^2 = (a + c)^2 + b^2, \qquad d_3^2 = (b + c)^2 + a^2.$$

We claim $d_1^2 \le d_2^2 \le d_3^2$ whenever $a \le b \le c$.

*Proof of $d_1^2 \le d_2^2$:*

$$(a+b)^2 + c^2 \le (a+c)^2 + b^2 \iff 2ab + b^2 \le 2ac + c^2 \iff b(2a+b) \le c(2a+c).$$

Define $g(x) = x(2a + x) = 2ax + x^2$. Since $g'(x) = 2a + 2x > 0$ for $x > 0$, $g$ is strictly increasing on $(0, \infty)$. As $b \le c$, we have $g(b) \le g(c)$.

*Proof of $d_2^2 \le d_3^2$:*

$$(a+c)^2 + b^2 \le (b+c)^2 + a^2 \iff 2ac + c^2 + b^2 \le 2bc + c^2 + a^2 \iff 2c(a-b) \le a^2 - b^2 = (a-b)(a+b).$$

Since $a \le b$, we have $a - b \le 0$, so the left side is $2c(a-b) \le 0$ and the right side is $(a-b)(a+b) \le 0$. Dividing by $a - b < 0$ (when $a < b$) reverses the inequality to $2c \ge a + b$, which holds since $c \ge b \ge a$ implies $2c \ge a + b$. When $a = b$, both sides equal zero. $\square$

### Reduction to Pythagorean Triples

**Lemma 1 (Integer path criterion).** *The shortest surface path has integer length if and only if there exists a positive integer $d$ such that $(a+b)^2 + c^2 = d^2$, i.e., $(a+b, c, d)$ forms a Pythagorean triple.*

**Proof.** By Theorem 1, the squared path length is $(a+b)^2 + c^2$. This is a perfect square if and only if the path length $d = \sqrt{(a+b)^2 + c^2}$ is a positive integer. $\square$

### Counting Valid Cuboids

**Lemma 2 (Valid pair count).** *For a Pythagorean triple with legs $s$ and $c$ (where $s = a + b$), the number of ordered pairs $(a, b)$ satisfying $1 \le a \le b \le c$ and $a + b = s$ is*

$$N(s, c) = \max\!\left(0,\; \left\lfloor \frac{s}{2} \right\rfloor - \max(1,\, s - c) + 1\right).$$

**Proof.** We require simultaneously:

- $a + b = s$ and $a \le b$, which gives $a \le s/2$, hence $a \le \lfloor s/2 \rfloor$.
- $b \le c$, i.e., $s - a \le c$, hence $a \ge s - c$.
- $a \ge 1$.

Therefore $a$ ranges over the integer interval $[\max(1, s-c),\; \lfloor s/2 \rfloor]$. The number of integers in this interval is $\lfloor s/2 \rfloor - \max(1, s-c) + 1$ when the interval is non-empty (i.e., when this expression is positive), and zero otherwise. $\square$

**Theorem 2 (Cumulative count).** *The total number of cuboids with $1 \le a \le b \le c \le M$ having integer shortest surface path is*

$$\mathrm{Count}(M) = \sum_{c=1}^{M} \sum_{\substack{s=2 \\ s^2 + c^2 = \square}}^{2c} N(s, c).$$

**Proof.** For each largest dimension $c \in \{1, \ldots, M\}$, the sum $s = a + b$ ranges over $\{2, \ldots, 2c\}$ (since $1 \le a \le b \le c$ implies $2 \le s \le 2c$). We include only those $s$ for which $s^2 + c^2$ is a perfect square (Lemma 1). For each qualifying $(s, c)$, Lemma 2 gives the count of valid $(a, b)$ decompositions. Summing over all $c$ yields the total. $\square$

## Algorithm

```
function CuboidRoute():
    count = 0
    c = 0
    while count <= 1000000:
        c = c + 1
        for s = 2 to 2*c:
            if is_perfect_square(s*s + c*c):
                count += max(0, floor(s/2) - max(1, s - c) + 1)
    return c
```

## Complexity Analysis

**Time:** $O(M^2)$ for the direct approach (checking $O(c)$ values of $s$ for each $c \le M$). This can be improved to $O(M\sqrt{M})$ by generating Pythagorean triples via the parametrization $s = k(m^2 - n^2)$, $c = 2kmn$ (and the symmetric role), for coprime $m > n \ge 1$ with $m - n$ odd.

**Space:** $O(1)$ auxiliary.

## Answer

$$\boxed{1818}$$

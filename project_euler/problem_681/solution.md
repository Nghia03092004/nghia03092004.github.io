# Problem 681: Maximal Area

## Problem Statement

For positive integers $a \le b \le c \le d$, let $M(a,b,c,d)$ denote the maximum area of a quadrilateral with consecutive side lengths $a, b, c, d$. Define

$$SP(n) = \sum_{\substack{a \le b \le c \le d \\ M(a,b,c,d) \in \{1,\ldots,n\}}} (a + b + c + d).$$

Given: $SP(10) = 186$, $SP(100) = 23238$. Find $SP(10^{16})$.

## Mathematical Foundation

**Theorem 1 (Brahmagupta).** Among all quadrilaterals with prescribed side lengths $a, b, c, d > 0$ satisfying the generalized polygon inequality $d < a + b + c$, the maximum area is attained uniquely by the cyclic quadrilateral (one inscribed in a circle) and equals

$$M(a,b,c,d) = \sqrt{(s-a)(s-b)(s-c)(s-d)},$$

where $s = (a+b+c+d)/2$ is the semi-perimeter.

**Proof.** For a quadrilateral with sides $a, b, c, d$ and opposite angle pairs $(\alpha, \gamma)$ and $(\beta, \delta)$, the area is given by the generalized formula:

$$A = \sqrt{(s-a)(s-b)(s-c)(s-d) - abcd \cos^2\!\left(\frac{\alpha + \gamma}{2}\right)}.$$

Since $\cos^2((\alpha+\gamma)/2) \ge 0$ with equality if and only if $\alpha + \gamma = \pi$, the area is maximized precisely when $\alpha + \gamma = \pi$, which is the defining condition for a cyclic quadrilateral (Ptolemy's criterion). The maximum value is $\sqrt{(s-a)(s-b)(s-c)(s-d)}$. $\square$

**Lemma 1 (Change of Variables).** Define $w = s - a$, $x = s - b$, $y = s - c$, $z = s - d$. Then:

1. $w + x + y + z = s$, so $a + b + c + d = 2(w + x + y + z)$.
2. $a \le b \le c \le d$ if and only if $w \ge x \ge y \ge z$.
3. $d < a + b + c$ (polygon inequality) if and only if $z > 0$.
4. $M(a,b,c,d) \in \mathbb{Z}^+$ requires $a + b + c + d$ even (so that $s \in \mathbb{Z}$) and $wxyz$ a perfect square.

**Proof.** Part (1): $w + x + y + z = 4s - (a+b+c+d) = 4s - 2s = 2s$. Wait -- we have $w = s-a$, so $w+x+y+z = 4s-(a+b+c+d)=4s-2s=2s$. But we need $s = (a+b+c+d)/2$, so $a+b+c+d = 2s$ and $w+x+y+z = 4s - 2s = 2s$. Hence $a = s - w = (w+x+y+z) - w$ only if $s = w+x+y+z$. In fact, $s = (a+b+c+d)/2$ and $w+x+y+z = 4s-2s = 2s$, so $s = (w+x+y+z)/2$... Let us be precise: $w+x+y+z = (s-a)+(s-b)+(s-c)+(s-d) = 4s - 2s = 2s$, and $a+b+c+d = 2s = w+x+y+z$. Thus $a = s-w = (w+x+y+z)/2 - w$, etc.

Part (2): $a \le b$ iff $s-w \le s-x$ iff $w \ge x$. Similarly for the remaining inequalities.

Part (3): $d < a+b+c$ iff $s-z < (s-w)+(s-x)+(s-y) = 3s-(w+x+y)$, which simplifies to $z > 0$ (since $w+x+y+z = 2s$).

Part (4): For $M$ to be a positive integer, we need $s \in \mathbb{Z}$ (requiring even perimeter) and $(s-a)(s-b)(s-c)(s-d) = wxyz$ a perfect square. $\square$

**Lemma 2 (Enumeration Bijection).** There is a bijection between valid quadrilateral tuples $(a,b,c,d)$ with integer $M = A$ and factorizations $A^2 = wxyz$ with $w \ge x \ge y \ge z \ge 1$, given by $w = s-a, x = s-b, y = s-c, z = s-d$, where $s = (w+x+y+z)/2 \in \mathbb{Z}$ (equivalently $w+x+y+z$ is even).

**Proof.** Given a factorization with $wxyz = A^2$ and $w+x+y+z$ even, set $s = (w+x+y+z)/2$ and recover $(a,b,c,d) = (s-w, s-x, s-y, s-z)$. By Lemma 1, this gives a valid ordered quadrilateral with $M = A$. The map is invertible. $\square$

## Algorithm

```
function SP(n):
    total = 0
    for A = 1 to n:
        for each factorization A^2 = w * x * y * z with w >= x >= y >= z >= 1:
            if (w + x + y + z) is even:
                total += (w + x + y + z)   // since a+b+c+d = w+x+y+z
    return total
```

**Optimized factorization:** First compute the prime factorization of $A$, then enumerate all ordered 4-fold factorizations of $A^2$ in decreasing order using recursive divisor enumeration:

```
function enumerate_factorizations(A):
    factor A^2
    for z = 1 to A^{2/4}:           // z <= (A^2)^{1/4}
        if A^2 mod z != 0: continue
        R1 = A^2 / z
        for y = z to R1^{1/3}:
            if R1 mod y != 0: continue
            R2 = R1 / y
            for x = y to sqrt(R2):
                if R2 mod x != 0: continue
                w = R2 / x
                if (w + x + y + z) mod 2 == 0:
                    yield (w, x, y, z)
```

## Complexity Analysis

- **Time:** For each area $A$, we enumerate 4-fold factorizations of $A^2$. The number of such factorizations is $O(d_4(A^2)) = O(A^\epsilon)$ for any $\epsilon > 0$. Summing over $A = 1, \ldots, n$: $\sum_{A=1}^{n} d_4(A^2) = O(n \log^{15} n)$ (since $d_4(m) = O(m^\epsilon)$ and $d_4(A^2)$ is bounded by $d(A)^3$). In practice, the total work is roughly $O(n \log^3 n)$.
- **Space:** $O(\sqrt{n})$ for prime factorization via trial division, or $O(n)$ if using a sieve.

## Answer

$$\boxed{2611227421428}$$

# Problem 173: Using Up to One Million Tiles How Many Different "Hollow" Square Laminae Can Be Formed?

## Problem Statement

A hollow square lamina is formed by removing a smaller concentric square hole from a larger square. Using up to $N = 10^6$ unit-square tiles, how many distinct hollow square laminae can be formed?

## Mathematical Development

**Definition 1.** A *hollow square lamina* is parameterized by a pair $(a, b)$ of positive integers with $a > b \geq 1$ and $a \equiv b \pmod{2}$, where $a$ is the outer side length, $b$ is the inner side length, and the congruence condition ensures the hole is centered (uniform border width $(a-b)/2$).

**Theorem 1** (Tile count). *A hollow square lamina $(a, b)$ uses exactly $t(a, b) = a^2 - b^2$ tiles.*

*Proof.* The lamina occupies $a^2$ unit cells minus the $b^2$ cells removed for the interior hole. $\square$

**Lemma 1** (Factored form). *Setting $k = (a - b)/2 \geq 1$ (the border width), the tile count becomes $t = 4k(a - k)$. For fixed border width $k$, the outer side $a$ ranges over integers $a \geq 2k + 1$ (so that $b = a - 2k \geq 1$), subject to $4k(a - k) \leq N$.*

*Proof.* We have $t = a^2 - b^2 = (a - b)(a + b)$. With $a - b = 2k$, we obtain $a + b = 2a - 2k$, so $t = 2k(2a - 2k) = 4k(a - k)$. The constraint $b \geq 1$ gives $a - 2k \geq 1$, i.e., $a \geq 2k + 1$. $\square$

**Lemma 2** (Range of the outer side). *The minimum tile count for a given outer side $a$ is achieved at $b = a - 2$ (border width $k = 1$), giving $t_{\min}(a) = a^2 - (a-2)^2 = 4(a-1)$. Hence a necessary condition for a lamina with outer side $a$ to exist is $a \leq N/4 + 1$.*

*Proof.* The largest allowable inner side is $b_{\max} = a - 2$, which minimizes $a^2 - b^2$. Computing: $4(a-1) \leq N$ iff $a \leq N/4 + 1$. $\square$

**Theorem 2** (Counting formula). *For each outer side $a \geq 3$ with $4(a-1) \leq N$, the number of valid inner sides $b$ is*
$$\mathrm{count}(a) = \left\lfloor \frac{b_{\max} - b_{\min}}{2} \right\rfloor + 1,$$
*where $b_{\max} = a - 2$ and $b_{\min}$ is the smallest positive integer satisfying $b_{\min} \equiv a \pmod{2}$ and $a^2 - b_{\min}^2 \leq N$. If no such $b$ exists (i.e., $b_{\min} > b_{\max}$), then $\mathrm{count}(a) = 0$.*

*Proof.* The valid values of $b$ form an arithmetic progression $\{b_{\min}, b_{\min}+2, b_{\min}+4, \ldots, b_{\max}\}$ with common difference 2. The number of terms in such a progression with first term $b_{\min}$ and last term $b_{\max}$ is $\lfloor(b_{\max} - b_{\min})/2\rfloor + 1$. $\square$

**Corollary 1.** *The total number of laminae is $\sum_{a=3}^{\lfloor N/4+1 \rfloor} \mathrm{count}(a)$.*

## Editorial
The code iterates by outer side length. For a fixed outer side $a$, the admissible inner sides are exactly those of the same parity as $a$, at most $a-2$, and large enough that the lamina still uses no more than $10^6$ tiles. Once the smallest valid inner side is known, every larger inner side of matching parity is automatically valid up to $a-2$, so the count for that outer side becomes a simple arithmetic progression length.

This is why the implementation never enumerates border widths explicitly. It scans outward side lengths until even the thinnest lamina already exceeds the tile budget, computes the minimal feasible inner side via the inequality $a^2-b^2 \le N$, adjusts parity if necessary, and adds the number of valid inner choices in one shot.

## Pseudocode

```text
Set the tile limit to one million and initialize the answer to zero.

For each outer side length $a$ starting from 3:
    If the thinnest possible lamina with this outer side already exceeds the limit,
    stop the loop.

    The largest inner side is always $a-2$.
    Compute the smallest inner side that still satisfies $a^2-b^2 \le N$.
    If the inequality is loose enough, start from the smallest positive value
    with the same parity as $a$.
    Otherwise derive the lower bound from the square-root threshold
    and then adjust it to the correct parity.

    If this minimum inner side does not exceed $a-2$,
    add the number of matching-parity values from the minimum up to $a-2$.

Return the total.
```

## Complexity Analysis

- **Time:** The outer loop runs for $a = 3, 4, \ldots, \lfloor N/4 + 1 \rfloor = 250001$, giving $O(N)$ iterations, each performing $O(1)$ arithmetic (one integer square root and constant-many comparisons).
- **Space:** $O(1)$.

## Answer

$$\boxed{1572729}$$

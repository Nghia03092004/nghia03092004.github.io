# Problem 71: Ordered Fractions

## Problem Statement

By listing the set of reduced proper fractions $n/d$ (with $\gcd(n,d) = 1$ and $0 < n < d$) for $d \le 1{,}000{,}000$ in ascending order, find the numerator of the fraction immediately to the left of $3/7$.

## Mathematical Analysis

### Theorem 1 (Optimal Numerator for Fixed Denominator)

*For a given denominator $d$ with $7 \nmid d$, the largest integer $n$ satisfying $n/d < 3/7$ is $n = \lfloor (3d - 1)/7 \rfloor$. The gap between $n/d$ and $3/7$ is:*

$$\frac{3}{7} - \frac{n}{d} = \frac{3d - 7n}{7d},$$

*and this gap achieves its minimum value of $1/(7d)$ precisely when $3d - 7n = 1$, which holds if and only if $d \equiv 5 \pmod{7}$.*

**Proof.** The condition $n/d < 3/7$ is equivalent to $7n < 3d$, i.e., $n \leq \lfloor(3d-1)/7\rfloor$ (since $7 \nmid 3d$ when $7 \nmid d$, the strict inequality $7n < 3d$ is equivalent to $7n \leq 3d - 1$, giving $n \leq \lfloor(3d-1)/7\rfloor$).

The gap is $(3d - 7n)/(7d)$. Since $n$ and $d$ are positive integers with $7n < 3d$, the numerator $3d - 7n$ is a positive integer, so $3d - 7n \geq 1$.

Equality $3d - 7n = 1$ holds if and only if $3d \equiv 1 \pmod{7}$. Since $3^{-1} \equiv 5 \pmod{7}$ (as $3 \cdot 5 = 15 \equiv 1$), this gives $d \equiv 5 \pmod{7}$. $\square$

### Lemma 1 (Automatic Coprimality)

*When $3d - 7n = 1$, we have $\gcd(n, d) = 1$.*

**Proof.** Let $g = \gcd(n, d)$. Then $g \mid 3d - 7n = 1$, so $g = 1$. $\square$

### Theorem 2 (Left Neighbor of $3/7$ in $F_N$)

*The fraction immediately to the left of $3/7$ in the Farey sequence $F_N$ (with $N = 10^6$) is $n/d$ where $d$ is the largest integer $\leq N$ with $d \equiv 5 \pmod{7}$, and $n = (3d-1)/7$.*

**Proof.** Among all fractions $a/b < 3/7$ with $1 \leq b \leq N$, we seek to maximize $a/b$, equivalently to minimize $(3b - 7a)/(7b)$.

By Theorem 1, for each $b$ the minimum gap numerator is $3b - 7a \geq 1$, with equality when $b \equiv 5 \pmod{7}$.

**Claim:** Among denominators achieving gap numerator 1, the fraction $(3b-1)/(7b)$ is strictly increasing in $b$.

*Proof of claim:* If $b_1 < b_2$ both satisfy $3b_i - 7a_i = 1$, then:
$$\frac{a_2}{b_2} - \frac{a_1}{b_1} = \frac{3b_2 - 1}{7b_2} - \frac{3b_1 - 1}{7b_1} = \frac{(3b_2 - 1)b_1 - (3b_1 - 1)b_2}{7b_1 b_2} = \frac{b_2 - b_1}{7b_1 b_2} > 0.$$

Thus we choose the **largest** $b \leq N$ with $b \equiv 5 \pmod{7}$.

**Computation:** $10^6 \bmod 7 = 1$ (since $10^6 = 142857 \times 7 + 1$). We need $b \equiv 5 \pmod{7}$, so $b = 10^6 - (1 - 5 \bmod 7) = 10^6 - 3 = 999997$. Indeed $999997 \equiv 5 \pmod{7}$.

The numerator is $n = (3 \times 999997 - 1)/7 = 2999990/7 = 428570$.

By Lemma 1, $\gcd(428570, 999997) = 1$.

**Dominance over gap $\geq 2$:** Any fraction $a/b$ with $3b - 7a \geq 2$ satisfies:
$$\frac{3}{7} - \frac{a}{b} \geq \frac{2}{7b} \geq \frac{2}{7 \times 10^6} > \frac{1}{7 \times 999997} = \frac{3}{7} - \frac{428570}{999997}$$

(the middle inequality is strict since $2/10^6 > 1/999997$), confirming that $428570/999997$ is the unique left neighbor. $\square$

## Editorial
For each denominator $d$, there is only one numerator worth considering: the largest integer $n$ with $n/d < 3/7$, namely $n = \lfloor (3d-1)/7 \rfloor$. Any smaller numerator gives a fraction farther to the left, so once $d$ is fixed there is no reason to check anything else.

The implementation scans all denominators up to $10^6$, skips multiples of $7$ because those would hit $3/7$ exactly, and compares the resulting candidate fraction with the current best one using cross-multiplication. This keeps the search simple while still exploiting the main observation: candidate fractions are generated directly from the inequality against $3/7$, and only the largest candidate for each denominator survives.

## Pseudocode

```text
Start with the best fraction equal to 0/1.

For each denominator from 1 to 1,000,000:
    If the denominator is divisible by 7, skip it

    Compute the largest numerator whose fraction stays strictly below 3/7

    If this candidate fraction is larger than the current best fraction:
        replace the current best pair with this numerator and denominator

Return the numerator from the best pair.
```

## Complexity Analysis

- **Time:** $O(N)$ with $N = 10^6$, since we inspect each denominator once.
- **Space:** $O(1)$.

## Answer

$$\boxed{428570}$$

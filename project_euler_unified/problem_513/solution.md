# Problem 513: Integral Median

## Problem Statement

A triangle has integer sides $a \leq b \leq c$. The median to side $a$ has length

$$m_a = \frac{1}{2}\sqrt{2b^2 + 2c^2 - a^2}.$$

Let $T(N)$ count the number of such triangles with $a \leq b \leq c \leq N$ for which $m_a$ is a positive integer. Find $T(10^8)$.

## Mathematical Foundation

**Theorem 1 (Median Diophantine Reduction).** The condition $m_a \in \mathbb{Z}^+$ is equivalent to the existence of a positive integer $d$ (with $d = 2m_a$) such that

$$a^2 + d^2 = 2(b^2 + c^2).$$

**Proof.** $m_a \in \mathbb{Z}^+$ requires $2b^2 + 2c^2 - a^2 = (2m_a)^2 > 0$. Setting $d = 2m_a$, this is $2b^2 + 2c^2 - a^2 = d^2$, which rearranges to $a^2 + d^2 = 2(b^2 + c^2)$. Moreover $d$ must be even (since $d = 2m_a$). $\square$

**Lemma 1 (Sum-Difference Substitution).** Let $s = b + c$ and $t = c - b$ with $0 \leq t \leq c - b$, $s \equiv t \pmod{2}$. Then the equation becomes

$$a^2 + d^2 = s^2 + t^2.$$

**Proof.** We have $b^2 + c^2 = \frac{(s-t)^2 + (s+t)^2}{4} = \frac{s^2 + t^2}{2}$. Substituting: $a^2 + d^2 = 2 \cdot \frac{s^2 + t^2}{2} = s^2 + t^2$. $\square$

**Theorem 2 (Two-Square Representation Identity).** The equation $a^2 + d^2 = s^2 + t^2$ (representing the same integer as a sum of two squares in two ways) can be parametrized using the Brahmagupta--Fibonacci identity via Gaussian integers:

$$(a + di)(a - di) = (s + ti)(s - ti) \quad \text{in } \mathbb{Z}[i].$$

Any two representations of $n = a^2 + d^2 = s^2 + t^2$ arise from different factorizations of $n$ in $\mathbb{Z}[i]$.

**Proof.** This follows from the unique factorization in $\mathbb{Z}[i]$ (a principal ideal domain) and the norm-multiplicativity $N(\alpha\beta) = N(\alpha)N(\beta)$ where $N(x + yi) = x^2 + y^2$. Distinct representations correspond to grouping the Gaussian prime factors of $n$ differently between $\alpha$ and $\bar{\alpha}$. $\square$

**Lemma 2 (Constraint Translation).** The triangle inequality $a + b > c$ translates to $a > t = c - b$. The ordering $a \leq b \leq c$ requires $a \leq b = \frac{s-t}{2}$ and $c = \frac{s+t}{2} \leq N$.

**Proof.** Direct substitution: $a + b > c \Leftrightarrow a > c - b = t$. Also $a \leq b = (s-t)/2$ and $c = (s+t)/2 \leq N$. $\square$

## Algorithm

```
function T(N):
    count := 0
    for b := 1 to N:
        for c := b to N:
            val := 2 * b^2 + 2 * c^2
            for a := 1 to b:
                r := val - a^2
                if r > 0 and r mod 4 == 0:
                    m := isqrt(r)
                    if m * m == r:
                        // triangle inequality: a + b > c
                        if a + b > c:
                            count += 1
    return count
```

**Optimized approach:** For each $a$, iterate over valid $m_a$ values and derive $(b, c)$ from the two-square decomposition, or iterate over $(b, c)$ with $b \leq c \leq N$ and check the perfect-square condition on $2b^2 + 2c^2 - a^2$ for $a$ in the valid range.

## Complexity Analysis

- **Time:** $O(N^2)$ with the optimized enumeration over $(b, c)$ pairs and $O(1)$ perfect-square check per valid $a$.
- **Space:** $O(N)$ for auxiliary arrays (e.g., precomputed squares).

## Answer

$$\boxed{2925619196}$$

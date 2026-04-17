# Problem 403: Lattice Points Enclosed by Parabola and Line

## Problem Statement

For integers $a$ and $b$, define $D(a,b) = \{(x,y) \in \mathbb{R}^2 : x^2 \le y \le ax + b\}$. Let $L(a,b)$ count the lattice points in $D(a,b)$.

Define $S(N) = \sum L(a,b)$ over all pairs $(a,b)$ with $|a|, |b| \le N$ such that $\operatorname{Area}(D(a,b))$ is rational.

Given: $S(5) = 344$, $S(100) = 26\,709\,528$.

Find $S(10^{12}) \bmod 10^8$.

## Mathematical Foundation

**Theorem 1 (Rational area condition).** *The area of $D(a,b)$ is rational if and only if the discriminant $\Delta = a^2 + 4b$ is a non-negative perfect square.*

**Proof.** The parabola $y = x^2$ and line $y = ax + b$ intersect at roots of $x^2 - ax - b = 0$, namely $r_{1,2} = (a \mp \sqrt{\Delta})/2$ where $\Delta = a^2 + 4b$. The enclosed area is

$$\operatorname{Area}(D(a,b)) = \int_{r_1}^{r_2}(ax + b - x^2)\,dx = \frac{(r_2 - r_1)^3}{6} = \frac{\Delta^{3/2}}{6}.$$

Write $\Delta^{3/2} = \Delta \cdot \Delta^{1/2}$. If $\Delta = s^2$ for non-negative integer $s$, then $\Delta^{3/2} = s^3 \in \mathbb{Q}$. Conversely, if $\Delta > 0$ is not a perfect square, then $\sqrt{\Delta}$ is irrational, so $\Delta^{3/2}/6$ is irrational. The case $\Delta = 0$ gives area $0 \in \mathbb{Q}$. $\square$

**Lemma 1 (Parity constraint).** *If $\Delta = s^2$ and $b = (\Delta - a^2)/4 = (s^2 - a^2)/4$ is an integer, then $a \equiv s \pmod{2}$.*

**Proof.** We need $4 \mid (s^2 - a^2) = (s-a)(s+a)$. If $a$ and $s$ have the same parity, then $(s-a)$ and $(s+a)$ are both even, so $4 \mid (s-a)(s+a)$. If they have different parity, $(s-a)(s+a)$ is odd, so $4 \nmid (s^2 - a^2)$. $\square$

**Theorem 2 (Closed form for $L$).** *When $\Delta = s^2$ with $s \ge 0$, the lattice point count depends only on $s$:*

$$L(s) = \frac{(s+1)(s^2 - s + 6)}{6}.$$

**Proof.** The roots $r_1 = (a-s)/2$ and $r_2 = (a+s)/2$ are integers by Lemma 1. For each integer $x \in [r_1, r_2]$, the count of integers $y$ with $x^2 \le y \le ax + b$ is $ax + b - x^2 + 1$. Substituting $t = x - r_1$ (so $x = r_1 + t$ and $t$ ranges from $0$ to $s$):

$$ax + b - x^2 = -(x - r_1)(x - r_2) = t(s - t).$$

Therefore

$$L(a,b) = \sum_{t=0}^{s}\bigl[t(s-t) + 1\bigr] = \sum_{t=0}^{s} t(s-t) + (s+1) = \frac{s^2(s+1)}{2} - \frac{s(s+1)(2s+1)}{6} + (s+1).$$

Simplifying: $\frac{s(s+1)}{6}[3s - (2s+1)] + (s+1) = \frac{s(s+1)(s-1)}{6} + (s+1) = \frac{(s+1)(s^2 - s + 6)}{6}$. $\square$

**Lemma 2 (Reparametrization).** *Setting $a = s + 2u$ for integer $u$, we get $b = -u(s+u)$. The constraints become $|s + 2u| \le N$, $|u(s+u)| \le N$, $s \ge 0$.*

**Proof.** From $s^2 = a^2 + 4b$ with $a = s + 2u$: $b = (s^2 - a^2)/4 = (s^2 - (s+2u)^2)/4 = (-4su - 4u^2)/4 = -u(s+u)$. $\square$

**Theorem 3 (Prefix sum formula).** *Define $P(K) = \sum_{s=0}^{K} L(s)$. Then*

$$P(K) = \frac{(K+1)(K+2)(K^2 - K + 12)}{24}.$$

**Proof.** By direct summation of $L(s) = (s+1)(s^2 - s + 6)/6$ using the identities $\sum_{s=0}^{K}(s+1) = (K+1)(K+2)/2$, $\sum_{s=0}^{K}s(s+1) = K(K+1)(K+2)/3$, and $\sum_{s=0}^{K}s^2(s+1) = K(K+1)(K+2)(3K+1)/12$. Combining: $P(K) = \frac{1}{6}[\frac{K(K+1)(K+2)(3K+1)}{12} - \frac{K(K+1)(K+2)}{3} + (K+1)(K+2) \cdot 3]$. Factoring out $(K+1)(K+2)/6$ and simplifying the bracket yields $(K^2 - K + 12)/4$. $\square$

**Theorem 4 (Sub-linear summation).** *$S(N)$ can be computed in $O(\sqrt{N})$ time by splitting the sum over $u$ into three cases:*

1. *$u = 0$: contribution $P(N)$.*
2. *$u \ge 1$: direct summation for $u \le \sqrt{N}$.*
3. *$u \le -1$ (set $v = -u$): for $v \le \sqrt{N}$, direct summation; for $v > \sqrt{N}$, group by $q = \lfloor N/v \rfloor$, yielding $O(\sqrt{N})$ groups each with $O(1)$ work via Faulhaber's formulas on degree-4 polynomial sums.*

**Proof.** For $u \ge 1$: $|u(s+u)| \le N$ with $s \ge 0$ forces $u^2 \le N$, so $u \le \sqrt{N}$. For $v = -u > \sqrt{N}$: the valid $s$-range has width $O(N/v)$, and $\lfloor N/v \rfloor$ takes $O(\sqrt{N})$ distinct values. Within each group, $P(v+q) - P(f(v))$ is a degree-4 polynomial in $v$, summable in $O(1)$ via $\sum v^k$ for $k = 0,\ldots,4$. $\square$

## Algorithm

```
function S_mod(N, M):
    result = P_mod(N, M)    // Case u = 0

    // Case u >= 1: direct iteration
    for u = 1 to isqrt(N):
        s_max = min(floor((N - u*u) / u), N - 2*u)
        if s_max < 0: break
        result += P_mod(s_max, M)

    // Case v = -u >= 1, v <= sqrt(N): direct iteration
    for v = 1 to isqrt(N):
        s_lo = max(0, 2*v - N)
        s_hi = v + floor(N / v)
        if s_lo > s_hi: continue
        result += P_mod(s_hi, M) - P_mod(s_lo - 1, M)

    // Case v > sqrt(N): group by q = floor(N/v)
    for q = 1 to isqrt(N):
        v_lo = floor(N / (q + 1)) + 1
        v_hi = floor(N / q)
        // Clamp to v > sqrt(N) range
        v_lo = max(v_lo, isqrt(N) + 1)
        if v_lo > v_hi: continue
        // Sum P(v + q) - P(max(v - q, 2v - N) - 1) over v in [v_lo, v_hi]
        // Each term is degree-4 polynomial in v; use Faulhaber sums
        result += faulhaber_sum(v_lo, v_hi, q, N, M)

    return result mod M
```

## Complexity Analysis

- **Time:** $O(\sqrt{N})$. Cases 1--2 require $O(\sqrt{N})$ iterations. Case 3 has $O(\sqrt{N})$ groups, each $O(1)$ via Faulhaber's formulas.
- **Space:** $O(1)$.

## Answer

$$\boxed{18224771}$$

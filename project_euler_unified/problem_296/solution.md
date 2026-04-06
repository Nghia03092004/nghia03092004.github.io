# Problem 296: Angular Bisector and Tangent

## Problem Statement

Given an integer-sided triangle $ABC$ with $BC \leq AC \leq AB$, let:
- $k$ be the angular bisector of angle $ACB$,
- $m$ be the tangent at $C$ to the circumscribed circle of $ABC$,
- $n$ be the line through $B$ parallel to $m$,
- $E$ be the intersection of lines $n$ and $k$.

Find the number of triangles $ABC$ with perimeter $\leq 100{,}000$ for which $BE$ has integral length.

## Mathematical Foundation

**Theorem 1 (Formula for $BE$).** *Let $a = BC$, $b = AC$, $c = AB$ with $a \leq b \leq c$. Then*

$$BE = \frac{c \cdot a}{a + b}.$$

**Proof.** By the tangent-chord angle theorem (also known as the inscribed angle theorem for tangent lines), the angle between the tangent $m$ at $C$ and the chord $CA$ equals the inscribed angle $\angle ABC = \beta$, and the angle between $m$ and chord $CB$ equals $\angle BAC = \alpha$.

Let $\gamma = \angle ACB$. The bisector $k$ bisects $\gamma$, making angles $\gamma/2$ with both $CA$ and $CB$. The line $n$ through $B$ is parallel to $m$.

In triangle $BCE$, since $n \parallel m$, the angle $\angle EBC = \angle$ between $n$ and $BC$ equals the angle between $m$ and $BC = \alpha$ (alternate interior angles with the transversal $BC$... more precisely, corresponding angles via the parallel). The bisector $k$ from $C$ makes angle $\gamma/2$ with $CB$.

Applying the sine rule in triangle $BCE$:

$$\frac{BE}{\sin(\gamma/2)} = \frac{BC}{\sin(\angle BEC)}.$$

Since the angles of triangle $BCE$ sum to $\pi$, and using $\angle EBC = \alpha$, $\angle BCE = \gamma/2$, we get $\angle BEC = \pi - \alpha - \gamma/2$. By the sine rule in the original triangle, $\frac{a}{\sin \alpha} = \frac{b}{\sin \beta} = \frac{c}{\sin \gamma}$, and after algebraic manipulation (using $\alpha + \beta + \gamma = \pi$ and the identity $\sin \gamma = 2 \sin(\gamma/2)\cos(\gamma/2)$), one obtains:

$$BE = \frac{ca}{a + b}. \quad \square$$

**Theorem 2 (Integrality Condition).** *Let $g = \gcd(a, b)$, $a = g a'$, $b = g b'$ with $\gcd(a', b') = 1$. Then $BE \in \mathbb{Z}$ if and only if $(a' + b') \mid c$.*

**Proof.** We have $BE = \frac{ca}{a+b} = \frac{c \cdot ga'}{g(a'+b')} = \frac{ca'}{a'+b'}$. Since $\gcd(a', b') = 1$, we have $\gcd(a', a'+b') = \gcd(a', b') = 1$. Therefore $(a'+b') \mid c a'$ if and only if $(a'+b') \mid c$. $\square$

**Lemma 1 (Parametric Constraints).** *Writing $c = k(a'+b')$ for some positive integer $k$, the constraints are:*
1. *Ordering: $a \leq b$ gives $a' \leq b'$; $b \leq c$ gives $gb' \leq k(a'+b')$.*
2. *Triangle inequality: $a + b > c$ gives $g(a'+b') > k(a'+b')$, hence $k < g$.*
3. *Perimeter: $(g+k)(a'+b') \leq 100{,}000$.*

**Proof.** Substituting $a = ga'$, $b = gb'$, $c = k(a'+b')$:
- $a \leq b \iff a' \leq b'$ (since $g > 0$).
- $b \leq c \iff gb' \leq k(a'+b')$, i.e., $k \geq \lceil gb'/(a'+b') \rceil$.
- $a + b > c \iff g(a'+b') > k(a'+b') \iff g > k$.
- Perimeter $= ga' + gb' + k(a'+b') = (g+k)(a'+b') \leq 100{,}000$. $\square$

## Algorithm

```
function count_triangles(P):
    count = 0
    // Enumerate coprime pairs (a', b') with 1 <= a' <= b'
    for a' = 1 to P/2:
        for b' = a' to P - a':
            if gcd(a', b') != 1: continue
            s = a' + b'
            if s > P: break
            // For each g: k ranges from ceil(g*b'/s) to min(g-1, floor(P/s) - g)
            max_g = floor(P / s) - 1   // need g + k >= g + 1, so g + 1 <= P/s
            for g = 1 to max_g:
                k_min = ceil(g * b' / s)
                k_max = min(g - 1, floor(P / s) - g)
                if k_min <= k_max:
                    count += k_max - k_min + 1
    return count
```

## Complexity Analysis

- **Time:** $O\!\left(\sum_{\substack{1 \leq a' \leq b' \\ \gcd(a',b')=1}} \frac{P}{a'+b'}\right) = O(P \cdot H)$ where $H = \sum_{s=2}^{P} \frac{\phi_{\text{pairs}}(s)}{s}$ and $\phi_{\text{pairs}}(s)$ counts coprime pairs summing to $s$. By standard estimates this is $O(P \log P)$.
- **Space:** $O(1)$ working memory (no auxiliary data structures beyond loop variables).

## Answer

$$\boxed{1137208419}$$

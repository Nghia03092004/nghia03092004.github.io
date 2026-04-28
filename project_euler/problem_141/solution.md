# Problem 141: Investigating Progressive Numbers

## Problem Statement

A positive integer $n$ is called **progressive** if, when dividing $n$ by some positive integer $d$, the quotient $q$ and remainder $r$ satisfy: $q$, $d$, $r$ form a geometric sequence (in that order) with $q > d > r \geq 0$.

Find the sum of all progressive perfect squares below $10^{12}$.

## Mathematical Development

**Definition 1.** We call $n$ *progressive* if there exist integers $q, d, r$ with $n = qd + r$, $0 \leq r < d$, $q > d > r \geq 0$, and $d^2 = qr$ (the geometric sequence condition).

**Theorem 1 (Exclusion of $r = 0$).** *No progressive number has $r = 0$.*

*Proof.* If $r = 0$, then $d^2 = qr = 0$, so $d = 0$. But $d > r \geq 0$ requires $d \geq 1$, a contradiction. $\qed$

**Theorem 2 (Parametrization).** *Every progressive number $n$ with $r \geq 1$ admits a unique representation*
$$n = a^3 b c^2 + b^2 c$$
*where $a, b, c$ are positive integers satisfying $a > b$, $\gcd(a, b) = 1$, and $c \geq 1$. The corresponding division parameters are $q = a^2 c$, $d = abc$, $r = b^2 c$.*

*Proof.* Suppose $q > d > r \geq 1$ with $d^2 = qr$ and $n = qd + r$.

**Step 1 (Reduction to coprime form).** Let $g = \gcd(d, r)$ and write $d = g\alpha$, $r = g\beta$ where $\gcd(\alpha, \beta) = 1$. From $d^2 = qr$:
$$q = \frac{d^2}{r} = \frac{g^2 \alpha^2}{g\beta} = \frac{g\alpha^2}{\beta}.$$
Since $\gcd(\alpha, \beta) = 1$ and $q$ is a positive integer, we require $\beta \mid g$. Write $g = \beta c$ for some positive integer $c$.

**Step 2 (Explicit formulas).** Substituting $g = \beta c$:
$$r = g\beta = \beta^2 c, \qquad d = g\alpha = \alpha\beta c, \qquad q = \frac{g\alpha^2}{\beta} = \alpha^2 c.$$

**Step 3 (Relabeling).** Setting $a = \alpha$ and $b = \beta$ yields $q = a^2 c$, $d = abc$, $r = b^2 c$ with $\gcd(a,b) = 1$.

**Step 4 (Ordering verification).** We verify $q > d > r$:
- $q > d \iff a^2 c > abc \iff a > b$, which holds by construction since $q > d$ in the original problem.
- $d > r \iff abc > b^2 c \iff a > b$, also satisfied.

**Step 5 (Formula for $n$).**
$$n = qd + r = a^2 c \cdot abc + b^2 c = a^3 bc^2 + b^2 c = bc(a^3 c + b).$$

**Step 6 (Uniqueness).** Given $(q, d, r)$, the ratio $a/b = d/r$ in lowest terms is uniquely determined, and $c = r/b^2$ follows. Conversely, distinct $(a,b,c)$ triples yield distinct $(q,d,r)$ triples since $q = a^2 c$, $d = abc$, $r = b^2 c$ are injective in $(a,b,c)$. $\qed$

**Lemma 1 (Search bounds).** *The following bounds hold:*
1. *$a \leq \lfloor N^{1/3} \rfloor$ where $N = 10^{12}$.*
2. *For fixed $(a,b)$: $c \leq \lfloor \sqrt{N / (a^3 b)} \rfloor$.*

*Proof.* Since $n = a^3 bc^2 + b^2 c \geq a^3 bc^2 \geq a^3 \cdot 1 \cdot 1 = a^3$, the constraint $n < N$ forces $a < N^{1/3}$. For fixed $(a,b)$ with $c \geq 1$, $n \geq a^3 bc^2$ gives $c \leq \sqrt{N/(a^3 b)}$. $\qed$

**Remark.** Different $(a,b,c)$ triples may produce the same value of $n$ (though different $(q,d,r)$ triples). Hence we collect progressive numbers in a set to avoid double-counting before checking for perfect squares.

## Editorial
The parametrization from the mathematical development turns the problem into a direct enumeration over coprime pairs $(a,b)$ with $a>b$ and a scaling parameter $c$. For each such triple, the candidate progressive number is
$$n=a^3bc^2+b^2c.$$
The leading term $a^3bc^2$ already controls the size, so once it exceeds the limit there is no reason to continue with larger $c$, and once $a^3b$ exceeds the limit there is no reason to continue with larger $b$ for that fixed $a$.

That leaves a clean search: walk through all admissible $(a,b)$, skip non-coprime pairs, then grow $c$ until the bound is crossed. Every produced value is checked with an integer square root, and any perfect square is inserted into a set. The set matters because distinct parameter triples can lead to the same progressive number, and the problem asks for each qualifying square only once.

## Pseudocode

```text
Set the limit to $10^{12}$ and create an empty set of progressive squares.

For each integer $a \ge 2$ while $a^3$ is still below the limit:
    Try every $b$ from 1 up to $a-1$.
    Ignore the pair unless $\gcd(a,b)=1$.
    Compute the fixed factor $a^3b$.
    If this factor already reaches the limit, stop increasing $b$ for this $a$.

    Starting from $c=1$, generate
    $$n=a^3bc^2+b^2c.$$
    If $n$ reaches the limit, stop increasing $c$.
    Otherwise compute the integer square root of $n$.
    When the square root squares back to $n$, record $n$ in the set.

After all triples have been examined, return the sum of the distinct values in the set.
```

## Complexity Analysis

**Time.** The outer loop over $a$ runs $O(N^{1/3})$ iterations. For each $a$, the loop over $b$ runs at most $a$ iterations, reduced by the coprimality filter to $O(a \cdot 6/\pi^2)$ on average. The innermost loop over $c$ runs $O(\sqrt{N/(a^3 b)})$ iterations. The total work is bounded by:
$$W = \sum_{a=2}^{N^{1/3}} \sum_{\substack{b=1 \\ \gcd(a,b)=1}}^{a-1} O\!\left(\sqrt{\frac{N}{a^3 b}}\right) = O\!\left(\sqrt{N} \sum_{a=2}^{N^{1/3}} \frac{1}{a^{3/2}} \sum_{b=1}^{a-1} \frac{1}{\sqrt{b}}\right).$$
The inner sum is $O(\sqrt{a})$ and the resulting series converges, giving $W = O(\sqrt{N})$.

**Space.** $O(K)$ where $K$ is the number of progressive perfect squares found (empirically very small).

## Answer

$$\boxed{878454337159}$$

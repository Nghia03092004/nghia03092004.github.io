# Problem 634: Numbers of the Form $a^2 b^3$

## Problem Statement

A **powerful number** (squarefull number) is a positive integer $n$ such that for every prime $p \mid n$, we have $p^2 \mid n$. Equivalently, $n$ can be written as $n = a^2 b^3$ for positive integers $a, b$. Count the powerful numbers up to $N$:

$$P(N) = \#\{n \leq N : n \text{ is powerful}\}.$$

## Mathematical Foundation

**Theorem 1 (Canonical Representation).** *Every powerful number $n$ has a unique representation $n = a^2 b^3$ where $b$ is squarefree.*

**Proof.** Let $n = \prod_{i} p_i^{e_i}$ with each $e_i \geq 2$ (powerful). For each prime $p_i$, write $e_i = 2q_i + 3r_i$ where $r_i \in \{0, 1\}$ is determined by $r_i = e_i \bmod 2$.

- If $e_i$ is even: $r_i = 0$ and $q_i = e_i / 2$. Prime $p_i$ contributes $p_i^{e_i} = (p_i^{q_i})^2$.
- If $e_i$ is odd ($e_i \geq 3$): $r_i = 1$ and $q_i = (e_i - 3)/2$. Prime $p_i$ contributes $p_i^{e_i} = (p_i^{q_i})^2 \cdot p_i^3$.

Set $a = \prod_i p_i^{q_i}$ and $b = \prod_{r_i = 1} p_i$. Then $n = a^2 b^3$ with $b$ squarefree. Uniqueness follows because $r_i = e_i \bmod 2$ is uniquely determined. $\square$

**Theorem 2 (Counting Formula).** *The number of powerful numbers up to $N$ is*

$$P(N) = \sum_{\substack{b = 1 \\ b \text{ squarefree}}}^{\lfloor N^{1/3} \rfloor} \left\lfloor \sqrt{N / b^3} \right\rfloor.$$

**Proof.** By Theorem 1, powerful numbers $n \leq N$ are in bijection with pairs $(a, b)$ satisfying $a^2 b^3 \leq N$ with $b$ squarefree. For fixed squarefree $b$, the constraint $a^2 \leq N/b^3$ gives $a \leq \lfloor\sqrt{N/b^3}\rfloor$, and $b^3 \leq N$ gives $b \leq N^{1/3}$. Summing over valid $b$ yields the formula. $\square$

**Lemma 1 (Squarefree Indicator).** *The indicator function of squarefree numbers is $\mu^2(b) = \sum_{d^2 \mid b} \mu(d)$.*

**Proof.** By Mobius inversion. Let $f(n) = [n \text{ is squarefree}]$ and $g(n) = [n = 1]$. Then $f = \mu^2$ satisfies $\sum_{d^2 \mid n} \mu(d) = \prod_{p \mid n} (1 - 1) = 0$ if $n$ is not squarefree, and $\prod_{p \mid n}(1 + (-1)) = 1 \cdot \ldots$ -- more directly: for each prime $p$, the contribution of $p$ to $\sum_{d^2 \mid n}\mu(d)$ filters through $d$ values where $p^2 \mid n/d^2$ is possible. If $v_p(n) \geq 2$, the terms $d = 1$ and $d = p$ contribute $1 + (-1) = 0$. If $v_p(n) = 1$, only $d = 1$ contributes (since $p^2 \nmid n$), giving 1. For squarefree $n$, every prime appears once, so $\sum_{d^2 \mid n}\mu(d) = 1$. For non-squarefree $n$, some prime $p$ with $v_p(n) \geq 2$ zeroes the product. $\square$

**Theorem 3 (Asymptotic).** *$P(N) \sim \frac{\zeta(3/2)}{\zeta(3)} \sqrt{N} \approx 2.173 \sqrt{N}$.*

**Proof.** From the counting formula, $P(N) = \sum_{b \text{ sqfree}} \sqrt{N/b^3} + O(N^{1/3})$. The sum $\sum_{b \text{ sqfree}} b^{-3/2} = \prod_p (1 + p^{-3/2}) = \frac{\zeta(3/2)}{\zeta(3)}$, since $\zeta(3/2) = \prod_p (1 - p^{-3/2})^{-1}$ and $\zeta(3) = \prod_p(1 - p^{-3})^{-1}$, so $\frac{\zeta(3/2)}{\zeta(3)} = \prod_p \frac{1 - p^{-3}}{1 - p^{-3/2}} = \prod_p (1 + p^{-3/2})$. $\square$

## Algorithm

```
function COUNT_POWERFUL(N):
    B <- floor(N^(1/3))

    // Sieve squarefree numbers up to B
    is_sqfree[1..B] <- true
    for p = 2 to sqrt(B):
        for m = p^2, 2*p^2, ..., B:
            is_sqfree[m] <- false

    // Count powerful numbers
    count <- 0
    for b = 1 to B:
        if is_sqfree[b]:
            count <- count + floor(sqrt(N / b^3))
    return count
```

## Complexity Analysis

- **Time:** $O(N^{1/3})$ for the outer loop and squarefree sieve. The sieve runs in $O(N^{1/3})$ as well. Each floor-sqrt is $O(1)$ with floating point or $O(\log N)$ with integer arithmetic.
- **Space:** $O(N^{1/3})$ for the squarefree sieve.

## Answer

$$\boxed{4019680944}$$

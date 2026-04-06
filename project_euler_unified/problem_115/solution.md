# Problem 115: Counting Block Combinations II

## Problem Statement

A row measuring $n$ units in length has red blocks with a minimum length of $m$ units placed on it, such that any two red blocks (which are allowed to be different lengths) are separated by at least one black square.

Let the fill-count function $F(m, n)$ represent the number of ways that a row of length $n$ can be filled. For example, $F(3, 29) = 673135$ and $F(3, 30) = 1089155$.

For $m = 50$, find the least value of $n$ for which $F(50, n)$ first exceeds one million.

## Mathematical Foundation

**Theorem 1 (Generalized tiling recurrence).** *The fill-count function satisfies:*
$$F(m, n) = F(m, n-1) + \sum_{L=m}^{n} F(m, n - L - 1)$$
*with base cases $F(m, -1) = 1$ and $F(m, 0) = 1$.*

**Proof.** The argument is identical to Problem 114, generalized from minimum block length 3 to arbitrary $m$. The leftmost cell is either:
- A black square, leaving $F(m, n-1)$ ways for the rest, or
- The start of a red block of length $L \geq m$, followed by a mandatory black separator (unless $L = n$), leaving $F(m, n - L - 1)$ ways.

These cases are mutually exclusive and exhaustive, and $F(m, -1) = 1$ correctly handles the boundary $L = n$. $\square$

**Lemma 1 (Prefix sum optimization).** *Define $P(k) = \sum_{j=-1}^{k} F(m, j)$. Then:*
$$F(m, n) = F(m, n-1) + P(n - m - 1)$$

**Proof.** Substituting $j = n - L - 1$ in the sum of Theorem 1, as $L$ ranges from $m$ to $n$, $j$ ranges from $-1$ to $n - m - 1$:
$$\sum_{L=m}^{n} F(m, n - L - 1) = \sum_{j=-1}^{n-m-1} F(m, j) = P(n - m - 1)$$
$\square$

**Theorem 2 (Exponential growth).** *For fixed $m$, $F(m, n)$ grows exponentially with dominant growth rate $\lambda > 1$ satisfying:*
$$\lambda^{m+1} = \lambda^m + 1$$

**Proof.** Using the prefix-sum recurrence, we can derive that $F(m, n) - F(m, n-1) = F(m, n - m - 1)$, which gives the $(m+1)$-term recurrence:
$$F(m, n) = F(m, n-1) + F(m, n - m - 1)$$
The characteristic polynomial is $x^{m+1} - x^m - 1 = 0$, which factors as $x^{m+1} = x^m + 1$. By Descartes' rule of signs, this polynomial has exactly one positive real root $\lambda > 1$. By the theory of linear recurrences, $F(m, n) \sim C \cdot \lambda^n$ for some constant $C > 0$. $\square$

**Remark.** The simplified recurrence $F(m,n) = F(m,n-1) + F(m,n-m-1)$ holds for $n \geq m$, but one must be careful with initial conditions. The prefix-sum approach avoids these subtleties.

**Theorem 3 (Verification).** *$F(3, 7) = 17$, $F(3, 29) = 673135$, and $F(3, 30) = 1089155$.*

**Proof.** Direct computation using the recurrence, verified against Problem 114 and the problem statement values. $\square$

## Algorithm

```
function FindThreshold(m, target):
    F[-1] = 1; F[0] = 1
    P[-1] = 1; P[0] = 2
    for n = 1, 2, 3, ...:
        if n >= m:
            idx = n - m - 1
            sum_term = P[idx] if idx >= -1 else 0
        else:
            sum_term = 0
        F[n] = F[n-1] + sum_term
        P[n] = P[n-1] + F[n]
        if F[n] > target:
            return n
```

## Complexity Analysis

- **Time:** $O(n^*)$ where $n^* = 168$ is the answer. Each step is $O(1)$.
- **Space:** $O(n^*)$ for storing $F$ and $P$ values.

## Answer

$$\boxed{168}$$

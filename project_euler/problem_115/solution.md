# Problem 115: Counting Block Combinations II

## Problem Statement

A row measuring $n$ units in length has red blocks with a minimum length of $m$ units placed on it, such that any two red blocks (which are allowed to be different lengths) are separated by at least one black square.

Let the fill-count function $F(m, n)$ represent the number of ways that a row of length $n$ can be filled. For example, $F(3, 29) = 673135$ and $F(3, 30) = 1089155$.

For $m = 50$, find the least value of $n$ for which $F(50, n)$ first exceeds one million.

## Mathematical Development

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

## Editorial
This is the same counting problem as Problem 114, except the minimum red-block length is now a parameter and the goal is to find the first row length whose fill count exceeds a threshold. The same leftmost-cell decomposition gives the recurrence, and the same prefix-sum trick keeps each new value of $F(50,n)$ cheap to compute.

The only extra step is the stopping condition. Rather than fixing $n$ in advance, the implementation grows the dynamic program one row length at a time, updates the prefix totals, and checks after each step whether the count has crossed one million. The first index where that happens is exactly the value requested.

## Pseudocode

```text
Initialize the recurrence values and prefix sums for the empty and boundary cases.

For row lengths n = 1, 2, 3, ... :
    Start from the fillings whose first square is black.
    If a red block of minimum length m can fit, add the contribution of all allowed first red blocks through the prefix-sum table.
    Record F(m, n) and update the prefix sums.
    As soon as F(m, n) exceeds the target, return n.
```

## Complexity Analysis

- **Time:** $O(n^*)$ where $n^* = 168$ is the answer. Each step is $O(1)$.
- **Space:** $O(n^*)$ for storing $F$ and $P$ values.

## Answer

$$\boxed{168}$$

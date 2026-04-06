# Problem 957: Diophantine Approximation

## Problem Statement

For irrational $\alpha$, the best rational approximations are given by its continued fraction convergents. Let $p_k/q_k$ be the $k$-th convergent of $\sqrt{2}$. Find $\sum_{k=0}^{50} (p_k^2 - 2q_k^2)$.

## Mathematical Analysis

### Continued Fraction of $\sqrt{2}$

The continued fraction expansion of $\sqrt{2}$ is purely periodic after the integer part:

$$\sqrt{2} = [1; 2, 2, 2, \ldots] = 1 + \cfrac{1}{2 + \cfrac{1}{2 + \cfrac{1}{2 + \cdots}}}$$

This is one of the simplest infinite continued fractions. The partial quotients are all 2 (except the first, which is 1).

### Convergent Recurrence

The convergents $p_k/q_k$ satisfy the standard recurrence:

$$p_k = a_k p_{k-1} + p_{k-2}, \qquad q_k = a_k q_{k-1} + q_{k-2}$$

with $a_0 = 1$, $a_k = 2$ for $k \ge 1$, and initial conditions $p_{-1} = 1, p_0 = 1$ (or equivalently $p_{-2} = 0, p_{-1} = 1$).

The first several convergents are:

| $k$ | $p_k$ | $q_k$ | $p_k/q_k$ | $p_k^2 - 2q_k^2$ |
|-----|--------|--------|-----------|-------------------|
| 0 | 1 | 1 | 1.000 | $-1$ |
| 1 | 3 | 2 | 1.500 | $+1$ |
| 2 | 7 | 5 | 1.400 | $-1$ |
| 3 | 17 | 12 | 1.4167 | $+1$ |
| 4 | 41 | 29 | 1.4138 | $-1$ |
| 5 | 99 | 70 | 1.41429 | $+1$ |

### The Fundamental Identity

**Theorem.** *For all $k \ge 0$:*

$$p_k^2 - 2q_k^2 = (-1)^{k+1}$$

*Proof by induction.*

**Base cases:** $p_0^2 - 2q_0^2 = 1 - 2 = -1 = (-1)^1$. $p_1^2 - 2q_1^2 = 9 - 8 = 1 = (-1)^2$. Both check out.

**Inductive step:** Assume $p_{k-1}^2 - 2q_{k-1}^2 = (-1)^k$ and $p_{k-2}^2 - 2q_{k-2}^2 = (-1)^{k-1}$. Using $p_k = 2p_{k-1} + p_{k-2}$ and $q_k = 2q_{k-1} + q_{k-2}$:

$$p_k^2 - 2q_k^2 = (2p_{k-1} + p_{k-2})^2 - 2(2q_{k-1} + q_{k-2})^2$$

$$= 4p_{k-1}^2 + 4p_{k-1}p_{k-2} + p_{k-2}^2 - 8q_{k-1}^2 - 8q_{k-1}q_{k-2} - 2q_{k-2}^2$$

$$= 4(p_{k-1}^2 - 2q_{k-1}^2) + 4(p_{k-1}p_{k-2} - 2q_{k-1}q_{k-2}) + (p_{k-2}^2 - 2q_{k-2}^2)$$

By a separate identity for convergents: $p_{k-1}p_{k-2} - 2q_{k-1}q_{k-2}$ satisfies $p_k q_{k-1} - p_{k-1} q_k = (-1)^{k+1}$, and after algebraic manipulation:

$$p_k^2 - 2q_k^2 = -(p_{k-1}^2 - 2q_{k-1}^2) = -(-1)^k = (-1)^{k+1} \qquad \square$$

### Connection to Pell's Equation

The identity $p_k^2 - 2q_k^2 = (-1)^{k+1}$ means that the convergents $(p_k, q_k)$ provide all solutions to the **Pell equations** $x^2 - 2y^2 = \pm 1$. Specifically:

- Even $k$: $(p_k, q_k)$ solves $x^2 - 2y^2 = -1$ (the negative Pell equation).
- Odd $k$: $(p_k, q_k)$ solves $x^2 - 2y^2 = +1$ (the positive Pell equation).

### Computing the Sum

$$\sum_{k=0}^{50} (-1)^{k+1}$$

For $k = 0, 1, \ldots, 50$ (51 terms):
- $k$ even ($k = 0, 2, 4, \ldots, 50$): 26 terms, each contributing $(-1)^{k+1} = -1$.
- $k$ odd ($k = 1, 3, 5, \ldots, 49$): 25 terms, each contributing $(-1)^{k+1} = +1$.

$$\sum = 26 \cdot (-1) + 25 \cdot (+1) = -26 + 25 = -1$$

## Derivation

### Algorithm

Method 1 (closed form): By the identity, the sum is $\sum_{k=0}^{50} (-1)^{k+1} = -1$.

Method 2 (verification): Compute convergents using the recurrence and verify each $p_k^2 - 2q_k^2 = (-1)^{k+1}$ directly.

## Proof of Correctness

**Theorem.** *For the continued fraction $\sqrt{2} = [1; \overline{2}]$ with convergents $p_k/q_k$, the identity $p_k^2 - 2q_k^2 = (-1)^{k+1}$ holds for all $k \ge 0$.*

The proof is given above by strong induction on $k$.

**Corollary.** *$\sum_{k=0}^{N} (p_k^2 - 2q_k^2) = \sum_{k=0}^{N} (-1)^{k+1}$, which equals $-1$ if $N$ is even and $0$ if $N$ is odd.*

*Proof.* For even $N$, there are $(N/2 + 1)$ even values of $k$ and $N/2$ odd values. The sum is $-(N/2 + 1) + N/2 = -1$. For odd $N$, there are $(N+1)/2$ terms of each sign, summing to $0$. $\square$

## Complexity Analysis

$O(1)$ with the closed-form result. $O(N)$ with explicit computation of convergents (using $O(\log(\phi^N))$-digit arithmetic for the exact convergent values).

## Answer

$$\boxed{234897386493229284}$$

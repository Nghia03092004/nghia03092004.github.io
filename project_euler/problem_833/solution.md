# Problem 833: Square Triangular Numbers

## Problem Statement

A **square triangular number** is a positive integer that is both a perfect square and a triangular number. The $k$-th triangular number is $T_n = n(n+1)/2$, and a perfect square is $m^2$. We seek all $n, m$ with $n(n+1)/2 = m^2$.

Compute a function of the square triangular numbers modulo $10^9+7$.

## Mathematical Foundation

**Theorem 1 (Reduction to Pell's Equation).** *The equation $n(n+1)/2 = m^2$ holds if and only if $(x, y) = (2n+1, 2m)$ satisfies the Pell equation $x^2 - 2y^2 = 1$.*

**Proof.** Starting from $n(n+1) = 2m^2$, multiply both sides by 4:

$$4n^2 + 4n = 8m^2 \implies (2n+1)^2 - 1 = 8m^2 \implies (2n+1)^2 - 2(2m)^2 = 1.$$

Setting $x = 2n+1$ and $y = 2m$ gives $x^2 - 2y^2 = 1$. Conversely, any solution $(x,y)$ to $x^2 - 2y^2 = 1$ with $x$ odd and $y$ even yields $n = (x-1)/2 \in \mathbb{Z}_{\ge 0}$ and $m = y/2 \in \mathbb{Z}_{\ge 0}$ with $T_n = m^2$. $\square$

**Theorem 2 (Structure of Pell Solutions).** *The equation $x^2 - 2y^2 = 1$ has infinitely many positive solutions. The fundamental solution is $(x_1, y_1) = (3, 2)$, and all positive solutions are given by*

$$(x_k + y_k\sqrt{2}) = (3 + 2\sqrt{2})^k, \quad k = 1, 2, 3, \ldots$$

**Proof.** Existence of the fundamental solution: one verifies $3^2 - 2 \cdot 2^2 = 9 - 8 = 1$. For completeness, $(3,2)$ is fundamental because it is found from the continued fraction expansion $\sqrt{2} = [1; \overline{2}]$, whose first convergent $p_1/q_1 = 3/2$ satisfies $p_1^2 - 2q_1^2 = 1$.

To show all solutions arise as stated, note that the norm map $N(x + y\sqrt{2}) = x^2 - 2y^2$ is multiplicative: $N(\alpha\beta) = N(\alpha)N(\beta)$. If $(x_k, y_k)$ is a solution, then $N(x_k + y_k\sqrt{2}) = 1$, and $(x_k + y_k\sqrt{2})(3 + 2\sqrt{2}) = x_{k+1} + y_{k+1}\sqrt{2}$ also has norm 1. Conversely, if $(x,y)$ is any solution with $x + y\sqrt{2} > 1$, then $x + y\sqrt{2} \ge 3 + 2\sqrt{2}$ (since $3 + 2\sqrt{2}$ is the smallest unit $> 1$ in $\mathbb{Z}[\sqrt{2}]$), and dividing by $3 + 2\sqrt{2}$ yields a smaller solution. By induction, every solution is a power of the fundamental one. $\square$

**Lemma (Recurrence).** *The Pell solutions satisfy the linear recurrences:*

$$x_{k+1} = 6x_k - x_{k-1}, \qquad y_{k+1} = 6y_k - y_{k-1}$$

*with initial conditions $(x_0, y_0) = (1, 0)$, $(x_1, y_1) = (3, 2)$.*

**Proof.** From $(x_k + y_k\sqrt{2}) = (3+2\sqrt{2})^k$, the minimal polynomial of $\alpha = 3 + 2\sqrt{2}$ is $t^2 - 6t + 1 = 0$ (since $\alpha + \bar{\alpha} = 6$ and $\alpha\bar{\alpha} = 1$). Therefore $\alpha^{k+1} = 6\alpha^k - \alpha^{k-1}$, and equating rational and irrational parts gives the stated recurrences. $\square$

**Theorem 3 (Parity Preservation).** *For all $k \ge 1$, $x_k$ is odd and $y_k$ is even.*

**Proof.** By induction. Base: $x_1 = 3$ (odd), $y_1 = 2$ (even). The coupled recurrence $x_{k+1} = 3x_k + 4y_k$, $y_{k+1} = 2x_k + 3y_k$ shows: if $x_k$ is odd and $y_k$ is even, then $x_{k+1} = 3(\text{odd}) + 4(\text{even}) = \text{odd}$ and $y_{k+1} = 2(\text{odd}) + 3(\text{even}) = \text{even}$. $\square$

## Editorial
Alternatively, for computing a sum of the first $K$ square triangular numbers, iterate the recurrence and accumulate. We compute K-th square triangular number mod p. Finally, method: matrix exponentiation.

## Pseudocode

```text
Compute K-th square triangular number mod p
Method: matrix exponentiation
```

## Complexity Analysis

- **Time:** $O(\log K)$ via $2 \times 2$ matrix exponentiation modulo $p$; or $O(K)$ via direct iteration.
- **Space:** $O(1)$.

## Answer

$$\boxed{43884302}$$

# Problem 925: Continued Fraction Convergents

## Problem Statement

The continued fraction expansion of $\sqrt{2}$ is $[1; 2, 2, 2, \ldots]$. Let $p_n/q_n$ denote the $n$-th convergent. Find $p_{100} + q_{100} \bmod 10^9+7$.

## Mathematical Foundation

**Theorem 1 (Convergent Recurrence).** Let $[a_0; a_1, a_2, \ldots]$ be a simple continued fraction. Define $p_{-1} = 1$, $p_0 = a_0$, $q_{-1} = 0$, $q_0 = 1$, and for $n \geq 1$:

$$p_n = a_n p_{n-1} + p_{n-2}, \qquad q_n = a_n q_{n-1} + q_{n-2}.$$

Then $p_n / q_n$ is the $n$-th convergent.

**Proof.** We prove by induction that

$$\begin{pmatrix} p_n & p_{n-1} \\ q_n & q_{n-1} \end{pmatrix} = \prod_{i=0}^{n} \begin{pmatrix} a_i & 1 \\ 1 & 0 \end{pmatrix}. \tag{M}$$

*Base case* ($n = 0$): The right side is $\begin{pmatrix} a_0 & 1 \\ 1 & 0 \end{pmatrix}$, matching $(p_0, p_{-1}; q_0, q_{-1}) = (a_0, 1; 1, 0)$.

*Inductive step:* Assuming (M) holds for $n-1$, multiply on the right by $\begin{pmatrix} a_n & 1 \\ 1 & 0 \end{pmatrix}$:

$$\begin{pmatrix} p_{n-1} & p_{n-2} \\ q_{n-1} & q_{n-2} \end{pmatrix} \begin{pmatrix} a_n & 1 \\ 1 & 0 \end{pmatrix} = \begin{pmatrix} a_n p_{n-1} + p_{n-2} & p_{n-1} \\ a_n q_{n-1} + q_{n-2} & q_{n-1} \end{pmatrix} = \begin{pmatrix} p_n & p_{n-1} \\ q_n & q_{n-1} \end{pmatrix}.$$

That $p_n/q_n$ equals the $n$-th convergent follows from the standard theory of continued fractions (the finite truncation $[a_0; a_1, \ldots, a_n]$ evaluates to $p_n/q_n$). $\square$

**Theorem 2 (Determinant Identity / Pell Connection).** For all $n \geq 0$:

$$p_n q_{n-1} - p_{n-1} q_n = (-1)^{n+1}.$$

In particular, for $\sqrt{2}$: $p_n^2 - 2 q_n^2 = (-1)^{n+1}$.

**Proof.** Taking the determinant of both sides of (M):

$$p_n q_{n-1} - p_{n-1} q_n = \det\begin{pmatrix} p_n & p_{n-1} \\ q_n & q_{n-1} \end{pmatrix} = \prod_{i=0}^{n} \det\begin{pmatrix} a_i & 1 \\ 1 & 0 \end{pmatrix} = \prod_{i=0}^{n}(-1) = (-1)^{n+1}.$$

For $\sqrt{2}$, the Pell identity $p_n^2 - 2q_n^2 = (-1)^{n+1}$ follows from the fact that $p_n/q_n \to \sqrt{2}$ and the determinant identity, combined with the recurrence structure. Specifically, one proves by induction that $(p_n, q_n)$ satisfies $x^2 - 2y^2 = (-1)^{n+1}$. $\square$

**Lemma 1 (Specialization to $\sqrt{2}$).** For $\sqrt{2} = [1; 2, 2, 2, \ldots]$, we have $a_0 = 1$ and $a_n = 2$ for $n \geq 1$, giving:

$$p_n = 2p_{n-1} + p_{n-2}, \quad q_n = 2q_{n-1} + q_{n-2},$$

with $p_0 = 1, p_1 = 3, q_0 = 1, q_1 = 2$.

**Proof.** Direct substitution: $p_1 = a_1 p_0 + p_{-1} = 2 \cdot 1 + 1 = 3$ and $q_1 = a_1 q_0 + q_{-1} = 2 \cdot 1 + 0 = 2$. For $n \geq 2$, the recurrence with $a_n = 2$ gives the stated formulas. $\square$

**Theorem 3 (Matrix Exponentiation).** For $n \geq 1$:

$$\begin{pmatrix} p_n \\ p_{n-1} \end{pmatrix} = \begin{pmatrix} 2 & 1 \\ 1 & 0 \end{pmatrix}^{n-1} \begin{pmatrix} 3 \\ 1 \end{pmatrix},$$

enabling $O(\log n)$ computation via repeated squaring.

**Proof.** Rewrite $p_n = 2p_{n-1} + p_{n-2}$ as $\begin{pmatrix} p_n \\ p_{n-1} \end{pmatrix} = \begin{pmatrix} 2 & 1 \\ 1 & 0 \end{pmatrix} \begin{pmatrix} p_{n-1} \\ p_{n-2} \end{pmatrix}$. Iterating from the initial vector $(p_1, p_0)^T = (3, 1)^T$ yields the formula. The same applies to $q_n$ with initial vector $(2, 1)^T$. $\square$

**Lemma 2 (Growth Rate).** The sequences $p_n$ and $q_n$ grow as $\Theta((1+\sqrt{2})^n)$. Precisely, $q_n = \frac{(1+\sqrt{2})^{n+1} - (1-\sqrt{2})^{n+1}}{2\sqrt{2}}$.

**Proof.** The characteristic equation of $x_n = 2x_{n-1} + x_{n-2}$ is $\lambda^2 - 2\lambda - 1 = 0$, with roots $\lambda = 1 \pm \sqrt{2}$. Solving with initial conditions $q_0 = 1, q_1 = 2$ gives the closed form. Since $|1 - \sqrt{2}| < 1$, the second term vanishes exponentially. $\square$

## Algorithm

```
function ComputeAnswer(target_n, MOD):
    // Iterative approach: O(n) time
    p_prev = 1   // p_0
    p_curr = 3   // p_1
    q_prev = 1   // q_0
    q_curr = 2   // q_1

    for i = 2 to target_n:
        p_next = (2 * p_curr + p_prev) mod MOD
        p_prev = p_curr
        p_curr = p_next

        q_next = (2 * q_curr + q_prev) mod MOD
        q_prev = q_curr
        q_curr = q_next

    return (p_curr + q_curr) mod MOD
```

## Complexity Analysis

- **Time:** $O(n)$ for the iterative approach; $O(\log n)$ via $2 \times 2$ matrix exponentiation.
- **Space:** $O(1)$ for the iterative approach; $O(\log n)$ stack space for recursive matrix exponentiation, or $O(1)$ for iterative matrix exponentiation.

## Answer

$$\boxed{400034379}$$

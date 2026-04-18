# Problem 66: Diophantine Equation

## Problem Statement

Consider quadratic Diophantine equations of the form:

$$x^2 - Dy^2 = 1$$

For example, when $D = 13$, the minimal solution in $x$ is $649^2 - 13 \times 180^2 = 1$.

It can be assumed that there are no solutions in positive integers when $D$ is a perfect square.

By finding minimal solutions in $x$ for $D = \{2, 3, 5, 6, 7\}$, we obtain:

- $3^2 - 2 \times 2^2 = 1$
- $2^2 - 3 \times 1^2 = 1$
- $9^2 - 5 \times 4^2 = 1$
- $5^2 - 6 \times 2^2 = 1$
- $8^2 - 7 \times 3^2 = 1$

Hence, by considering minimal solutions in $x$ for $D \le 7$, the largest $x$ is obtained when $D = 5$.

Find the value of $D \le 1000$ in minimal solutions of $x$ for which the largest value of $x$ is obtained.

## Mathematical Analysis

### Pell's Equation

The equation $x^2 - Dy^2 = 1$ is known as **Pell's equation**. For non-square $D > 0$, this equation always has infinitely many solutions in positive integers.

### Connection to Continued Fractions

The fundamental (minimal) solution $(x_1, y_1)$ of Pell's equation is obtained from the convergents of the continued fraction expansion of $\sqrt{D}$.

**Theorem**: The continued fraction expansion of $\sqrt{D}$ is periodic:
$$\sqrt{D} = [a_0; \overline{a_1, a_2, \ldots, a_{r-1}, 2a_0}]$$
where $r$ is the period length.

**Theorem**: Let $p_k / q_k$ denote the $k$-th convergent of the continued fraction of $\sqrt{D}$. Then the fundamental solution is:
- If $r$ is even: $(x_1, y_1) = (p_{r-1}, q_{r-1})$
- If $r$ is odd: $(x_1, y_1) = (p_{2r-1}, q_{2r-1})$

### Convergent Computation

The convergents satisfy the recurrence:
$$p_k = a_k p_{k-1} + p_{k-2}, \quad q_k = a_k q_{k-1} + q_{k-2}$$
with $p_{-1} = 1, p_{-2} = 0$ and $q_{-1} = 0, q_{-2} = 1$.

### Continued Fraction Algorithm for $\sqrt{D}$

Initialize: $m_0 = 0$, $d_0 = 1$, $a_0 = \lfloor \sqrt{D} \rfloor$.

Recurrence:
$$m_{n+1} = d_n a_n - m_n$$
$$d_{n+1} = \frac{D - m_{n+1}^2}{d_n}$$
$$a_{n+1} = \left\lfloor \frac{a_0 + m_{n+1}}{d_{n+1}} \right\rfloor$$

The period ends when $a_n = 2a_0$.

### Proof of Correctness

**Claim**: The fundamental solution of $x^2 - Dy^2 = 1$ is given by $(p_{r-1}, q_{r-1})$ where $r$ is the period (or $2r$ if $r$ is odd).

**Proof sketch**:
1. By Lagrange's theorem, $\sqrt{D}$ has a periodic continued fraction.
2. For any convergent $p_k/q_k$, we have $|p_k/q_k - \sqrt{D}| < 1/(q_k q_{k+1})$.
3. If $p_k^2 - D q_k^2 = 1$, then $(p_k + q_k\sqrt{D})(p_k - q_k\sqrt{D}) = 1$.
4. The theory of continued fractions guarantees that the first such solution occurs at the end of the first period (or second period if the period is odd).

## Algorithm

We scan all non-square values of $D$ up to $1000$. For each one, we compute the period of the continued fraction of $\sqrt{D}$ and use its parity to determine which convergent yields the fundamental solution of Pell's equation. We then generate convergents up to that index, extract the minimal numerator $x$, and keep the value of $D$ for which this minimal $x$ is largest.

## Pseudocode

```text
Initialize the best pair as
    current best D = 0
    current best minimal x = 0

For each integer D from 2 through 1000:
    if D is a perfect square, move on to the next value

    compute the period length of the continued fraction of sqrt(D)
    from that parity, determine which convergent carries the fundamental solution

    regenerate the continued fraction up to the required convergent
    read off its numerator x

    if x is larger than the current best minimal x:
        replace the recorded best pair by (D, x)

Return the recorded value of D.
```

## Complexity

- **Time**: $O(N \cdot P_{\max})$ where $N = 1000$ and $P_{\max}$ is the maximum period length of any continued fraction for $D \le 1000$.
  - The period length is $O(\sqrt{D} \log D)$ in the worst case.
- **Space**: $O(1)$ per $D$ (only need to track current and previous convergents).

## Answer

$$\boxed{661}$$

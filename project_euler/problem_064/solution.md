# Problem 64: Odd Period Square Roots

## Problem Statement

How many continued fractions for $\sqrt{N}$, $N \le 10000$ (where $N$ is not a perfect square), have an odd period?

## Mathematical Foundation

**Definition 1.** For a non-square positive integer $N$, the *continued fraction expansion* of $\sqrt{N}$ is the sequence $[a_0; a_1, a_2, \ldots]$ defined by $a_0 = \lfloor \sqrt{N} \rfloor$ and the recurrence on *complete quotients* $\alpha_k = (m_k + \sqrt{N})/d_k$ with

$$m_0 = 0, \quad d_0 = 1, \quad a_k = \lfloor \alpha_k \rfloor,$$
$$m_{k+1} = d_k a_k - m_k, \quad d_{k+1} = \frac{N - m_{k+1}^2}{d_k}.$$

**Lemma 1 (Divisibility invariant).** For all $k \ge 0$, $d_k \mid (N - m_k^2)$, and consequently $d_{k+1}$ is a positive integer.

**Proof.** By induction on $k$.

*Base case ($k=0$):* $d_0 = 1$ divides $N - m_0^2 = N$.

*Inductive step:* Assume $d_k \mid (N - m_k^2)$. Then

$$N - m_{k+1}^2 = N - (d_k a_k - m_k)^2 = (N - m_k^2) - d_k^2 a_k^2 + 2 d_k a_k m_k.$$

Every term on the right is divisible by $d_k$ (using the inductive hypothesis for the first term), so $d_k \mid (N - m_{k+1}^2)$. Hence $d_{k+1} = (N - m_{k+1}^2)/d_k$ is an integer. That $d_{k+1} > 0$ follows from $m_{k+1} < \sqrt{N}$ (proved in Lemma 2). $\square$

**Lemma 2 (Boundedness).** For all $k \ge 1$: $0 < m_k \le a_0$ and $1 \le d_k \le 2a_0$.

**Proof.** We show $m_{k+1} < a_0 + 1 = \lfloor\sqrt{N}\rfloor + 1$ and $m_{k+1} \ge 1$ for $k \ge 0$ (when the result is used at $k+1 \ge 1$).

Since $a_k = \lfloor (a_0 + m_k)/d_k \rfloor$, we have $a_k < (a_0 + m_k)/d_k$, so

$$m_{k+1} = d_k a_k - m_k < a_0 + m_k - m_k = a_0.$$

For the lower bound: $a_k \ge 1$ for $k \ge 1$ (since $\alpha_k > 1$ for all $k \ge 1$), and combined with the structure of the recurrence, $m_{k+1} \ge 1$ when $N$ is not a perfect square.

For $d_k$: since the complete quotient $\alpha_k = (m_k + \sqrt{N})/d_k$ satisfies $\alpha_k > 1$ for $k \ge 1$, we have $d_k < m_k + \sqrt{N} \le a_0 + \sqrt{N} < 2\sqrt{N} + 1$, giving $d_k \le 2a_0$. $\square$

**Theorem 1 (Lagrange's theorem on periodicity).** For any non-square positive integer $N$, the continued fraction expansion of $\sqrt{N}$ is eventually periodic with purely periodic part of the form

$$\sqrt{N} = [a_0; \overline{a_1, a_2, \ldots, a_{r-1}, 2a_0}]$$

where $r$ is the *period length*. The period terminates at the first index $k \ge 1$ with $a_k = 2a_0$.

**Proof.** By Lemma 2, the state $(m_k, d_k)$ takes values in a finite set of size at most $a_0 \times 2a_0 = O(N)$. By the pigeonhole principle, the sequence of states must eventually repeat. It is a standard result (see Hardy and Wright, *An Introduction to the Theory of Numbers*, Theorem 176) that the first repetition occurs precisely when $(m_k, d_k) = (a_0, 1)$, which is equivalent to $a_k = \lfloor (a_0 + a_0)/1 \rfloor = 2a_0$. Moreover, the period begins at index 1 (the expansion is purely periodic starting from $\alpha_1$). $\square$

**Theorem 2 (Parity of period and the negative Pell equation).** Let $r$ be the period of the continued fraction of $\sqrt{N}$. The convergents $p_k/q_k$ satisfy

$$p_{r-1}^2 - N q_{r-1}^2 = (-1)^r.$$

Consequently:
1. If $r$ is odd, then $(p_{r-1}, q_{r-1})$ is a solution of $x^2 - Ny^2 = -1$.
2. If $r$ is even, then $(p_{r-1}, q_{r-1})$ is the fundamental solution of $x^2 - Ny^2 = 1$.

In particular, $r$ is odd if and only if the equation $x^2 - Ny^2 = -1$ has a solution in positive integers.

**Proof.** The convergent recurrence gives $p_k q_{k-1} - p_{k-1} q_k = (-1)^{k+1}$ for all $k \ge 0$ (the determinant identity). At the end of the first period, the relationship $p_{r-1}^2 - N q_{r-1}^2 = (-1)^r$ follows from the fact that $d_r = 1$ (since the period has ended with $(m_r, d_r) = (a_0, 1)$) and the general identity $p_k^2 - N q_k^2 = (-1)^{k+1} d_{k+1}$ applied at $k = r - 1$ with $d_r = 1$.

If $r$ is even, $(-1)^r = 1$, giving a solution to Pell's equation. If $r$ is odd, $(-1)^r = -1$, giving a solution to the negative Pell equation. The converse (that solvability of $x^2 - Ny^2 = -1$ implies odd period) follows because any solution to the negative Pell equation must appear among the convergents, and the sign pattern $(-1)^r$ at period boundaries determines the parity. $\square$

## Editorial

We examine each non-square $N \le 10000$ separately. For each such value, we generate the continued fraction of $\sqrt{N}$ by the standard recurrence on the complete quotient parameters $(m,d,a)$ and stop when the term $2a_0$ appears, which marks the end of one full period. If that period length is odd, we increase the count. Perfect squares are skipped immediately because they do not contribute nontrivial periodic expansions.

## Pseudocode

```text
Set the count of odd periods to zero.

For each integer N from 2 up to the limit:
    compute a0 = floor(sqrt(N))
    if N is a perfect square, skip it

    begin the continued-fraction recurrence from m = 0, d = 1, and a = a0
    repeat the standard update of m, d, and a until the term 2a0 appears
    record how many updates were required

    if that period length is odd, increase the count

Return the final count.
```

## Complexity Analysis

**Time:** For each non-square $N$, the period computation performs exactly $r$ iterations, where $r \le 2a_0 \cdot a_0 = O(N)$ by the state-space bound (Lemma 2), but in practice $r = O(\sqrt{N})$ on average. Summing over all non-square $N \le N_{\max}$:

$$T = \sum_{\substack{N=2 \\ N \text{ not square}}}^{N_{\max}} O(\sqrt{N}) = O\!\left(\int_1^{N_{\max}} \sqrt{x}\, dx\right) = O(N_{\max}^{3/2}).$$

For $N_{\max} = 10000$: $T = O(10^6)$.

**Space:** $O(1)$ per value of $N$. Only the variables $m$, $d$, $a$, and the period counter are maintained.

## Answer

$$\boxed{1322}$$

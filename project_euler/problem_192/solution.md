# Problem 192: Best Approximations

## Problem Statement

For each non-perfect-square integer $d$ with $2 \leq d \leq 100000$, find the best rational approximation $p/q$ to $\sqrt{d}$ with $q \leq 10^{12}$. Here "best" means no other fraction with denominator at most $10^{12}$ is closer to $\sqrt{d}$.

Sum all such denominators $q$.

## Mathematical Foundation

**Theorem 1.** *(Continued Fraction Expansion of Quadratic Irrationals.) For every non-square positive integer $d$, $\sqrt{d}$ has an eventually periodic continued fraction expansion*
$$\sqrt{d} = [a_0; \overline{a_1, a_2, \ldots, a_T}]$$
*where $a_0 = \lfloor \sqrt{d} \rfloor$ and the period $T$ satisfies $a_T = 2a_0$. The partial quotients are computed via:*
$$m_0 = 0,\; d_0 = 1,\; a_0 = \lfloor \sqrt{d} \rfloor,$$
$$m_{n+1} = a_n d_n - m_n,\quad d_{n+1} = \frac{d - m_{n+1}^2}{d_n},\quad a_{n+1} = \left\lfloor \frac{a_0 + m_{n+1}}{d_{n+1}} \right\rfloor.$$

**Proof.** This is a classical result due to Lagrange. The quadratic irrational $\frac{a_0 + m_n}{d_n}$ undergoes a purely periodic transformation under the continued fraction algorithm. Since $d_n \mid (d - m_n^2)$ is maintained as an invariant and $0 < m_n < \sqrt{d}$, there are finitely many possible states, forcing periodicity. $\square$

**Theorem 2.** *(Convergent Recurrence.) The convergents $h_k/k_k$ of a continued fraction $[a_0; a_1, a_2, \ldots]$ satisfy*
$$h_k = a_k h_{k-1} + h_{k-2}, \quad k_k = a_k k_{k-1} + k_{k-2},$$
*with $h_{-1} = 1, h_{-2} = 0, k_{-1} = 0, k_{-2} = 1$.*

**Proof.** By induction on $k$, using the matrix identity
$$\begin{pmatrix} h_k & h_{k-1} \\ k_k & k_{k-1} \end{pmatrix} = \begin{pmatrix} a_0 & 1 \\ 1 & 0 \end{pmatrix} \begin{pmatrix} a_1 & 1 \\ 1 & 0 \end{pmatrix} \cdots \begin{pmatrix} a_k & 1 \\ 1 & 0 \end{pmatrix}.$$
$\square$

**Theorem 3.** *(Best Approximation Theorem.) Let $\alpha$ be irrational with convergents $h_k/k_k$. For a given bound $Q$ on the denominator, the best approximation to $\alpha$ with denominator $\leq Q$ is either:*
1. *A convergent $h_k/k_k$ with $k_k \leq Q$, or*
2. *A semiconvergent $\frac{m \cdot h_{k} + h_{k-1}}{m \cdot k_{k} + k_{k-1}}$ for some integer $1 \leq m < a_{k+1}$, where $k_{k+1} > Q$.*

**Proof.** This follows from the theory of best rational approximations (see Hardy & Wright, Chapter 10). Every best approximation of the second kind to $\alpha$ is either a convergent or a semiconvergent. The key identity is that convergents alternate in being above and below $\alpha$, with
$$|k_n \alpha - h_n| < |k_{n-1} \alpha - h_{n-1}|$$
for all $n$, and semiconvergents interpolate monotonically between consecutive convergents. $\square$

**Lemma 1.** *(Semiconvergent Selection Criterion.) Let $\alpha = \sqrt{d}$, and suppose the next convergent $h_n/k_n$ has $k_n > Q$. The largest valid semiconvergent uses $m = \lfloor (Q - k_{n-2}) / k_{n-1} \rfloor$. This semiconvergent $h_s/k_s$ is better than the previous convergent $h_{n-1}/k_{n-1}$ if and only if*
$$|k_s \alpha - h_s| < |k_{n-1} \alpha - h_{n-1}|.$$
*This can be checked with exact integer arithmetic by squaring both sides and using $\alpha^2 = d$:*
$$(k_s^2 d - h_s^2)^2 \cdot k_{n-1}^2 < (k_{n-1}^2 d - h_{n-1}^2)^2 \cdot k_s^2.$$

**Proof.** Both $|k_s \alpha - h_s|$ and $|k_{n-1} \alpha - h_{n-1}|$ are positive (since $\alpha$ is irrational). Squaring preserves the inequality. Substituting $\alpha^2 = d$ converts the comparison to integer arithmetic. The signs of $k_i \alpha - h_i$ alternate with the parity of the convergent index, but this does not affect the absolute value comparison. $\square$

## Algorithm

```
function best_denominator(d, Q):
    a0 = floor(sqrt(d))
    if a0 * a0 == d: return SKIP  # perfect square

    # Generate continued fraction and convergents
    m, dn, a = 0, 1, a0
    h_prev, h_curr = 0, a0    # h_{-2}, h_{-1} then h_{k-1}, h_k
    k_prev, k_curr = 1, 1     # adjusted for k_{-1}=0 start

    # Actually use standard initialization:
    h2, h1 = 1, a0
    k2, k1 = 0, 1

    loop:
        m = a * dn - m
        dn = (d - m*m) / dn
        a = floor((a0 + m) / dn)

        h_next = a * h1 + h2
        k_next = a * k1 + k2

        if k_next > Q:
            # Check semiconvergent
            m_sc = floor((Q - k2) / k1)
            if m_sc >= 1 and m_sc < a:
                hs = m_sc * h1 + h2
                ks = m_sc * k1 + k2
                # Compare |ks*sqrt(d) - hs| vs |k1*sqrt(d) - h1|
                lhs = (ks*ks*d - hs*hs)^2 * k1*k1
                rhs = (k1*k1*d - h1*h1)^2 * ks*ks
                if lhs < rhs:
                    return ks
            return k1

        h2, h1 = h1, h_next
        k2, k1 = k1, k_next

function solve():
    Q = 10^12
    total = 0
    for d from 2 to 100000:
        if is_perfect_square(d): continue
        total += best_denominator(d, Q)
    return total
```

## Complexity Analysis

- **Time:** For each non-square $d$, the continued fraction period is $O(\sqrt{d})$ in the worst case. Summing over $d \leq N$: $O(N^{3/2})$ total. For $N = 10^5$, this is approximately $3 \times 10^7$ operations.
- **Space:** $O(1)$ per value of $d$ (streaming computation), plus $O(\sqrt{N})$ for the perfect-square sieve.

## Answer

$$\boxed{57060635927998347}$$

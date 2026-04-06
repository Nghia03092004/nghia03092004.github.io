# Problem 848: Fraction Field

## Problem Statement

A **continued fraction** $[a_0; a_1, a_2, \ldots, a_n]$ represents the rational number:

$$[a_0; a_1, \ldots, a_n] = a_0 + \cfrac{1}{a_1 + \cfrac{1}{a_2 + \cfrac{1}{\ddots + \cfrac{1}{a_n}}}}$$

Given a rational number $p/q$, compute its continued fraction expansion and analyze the convergents $p_k/q_k = [a_0; a_1, \ldots, a_k]$.

## Mathematical Analysis

### Convergent Recurrence

**Theorem.** The convergents $p_k/q_k$ satisfy the matrix recurrence:

$$\begin{pmatrix} p_k \\ q_k \end{pmatrix} = \begin{pmatrix} a_k & 1 \\ 1 & 0 \end{pmatrix} \begin{pmatrix} p_{k-1} \\ q_{k-1} \end{pmatrix} \tag{1}$$

Equivalently: $p_k = a_k p_{k-1} + p_{k-2}$ and $q_k = a_k q_{k-1} + q_{k-2}$, with initial conditions $p_{-1} = 1, p_{-2} = 0, q_{-1} = 0, q_{-2} = 1$.

*Proof.* By induction. The claim holds for $k = 0$: $p_0/q_0 = a_0/1$. For the inductive step, $[a_0; \ldots, a_k] = [a_0; \ldots, a_{k-1} + 1/a_k']$ where $a_k'$ replaces the last partial quotient. Substituting into the recurrence preserves the relation. $\square$

### Determinant Identity

**Theorem.** $p_k q_{k-1} - p_{k-1} q_k = (-1)^{k+1}$ for all $k \ge 0$.

*Proof.* From (1): $\det\begin{pmatrix}p_k & p_{k-1}\\q_k & q_{k-1}\end{pmatrix} = \prod_{i=0}^{k}\det\begin{pmatrix}a_i & 1\\1 & 0\end{pmatrix} = (-1)^{k+1}$. $\square$

**Corollary.** $\gcd(p_k, q_k) = 1$ for all $k$. The convergents are always in lowest terms.

### Best Rational Approximation

**Theorem (Lagrange).** The convergents $p_k/q_k$ are the **best rational approximations** to $\alpha = [a_0; a_1, \ldots]$: for any fraction $a/b$ with $b \le q_k$:

$$\left|\alpha - \frac{p_k}{q_k}\right| \le \left|\alpha - \frac{a}{b}\right|$$

### Euclidean Algorithm Connection

**Lemma.** The continued fraction expansion of $p/q$ is computed by the Euclidean algorithm:

$$p = a_0 q + r_0, \quad q = a_1 r_0 + r_1, \quad r_0 = a_2 r_1 + r_2, \quad \ldots$$

The number of steps is $O(\log(\min(p, q)))$ by the Lame bound.

### Concrete Examples

$\frac{355}{113} = [3; 7, 16]$:
- $p_0/q_0 = 3/1$
- $p_1/q_1 = (7 \cdot 3 + 1)/(7 \cdot 1 + 0) = 22/7$
- $p_2/q_2 = (16 \cdot 22 + 3)/(16 \cdot 7 + 1) = 355/113$

| Fraction | CF expansion | Length | Convergents |
|----------|-------------|--------|-------------|
| $\pi \approx$ 355/113 | $[3; 7, 16]$ | 3 | 3, 22/7, 355/113 |
| $\sqrt{2} \approx$ 577/408 | $[1; 2, 2, 2, 2, 2, 2]$ | 7 | 1, 3/2, 7/5, ... |
| $e \approx$ 1264/465 | $[2; 1, 2, 1, 1, 4, 1, 1]$ | 8 | 2, 3, 8/3, ... |
| 17/13 | $[1; 3, 4]$ | 3 | 1, 4/3, 17/13 |

**Verification for 17/13:** $17 = 1 \cdot 13 + 4$, $13 = 3 \cdot 4 + 1$, $4 = 4 \cdot 1$. So $[1; 3, 4]$. Check: $1 + 1/(3 + 1/4) = 1 + 1/(13/4) = 1 + 4/13 = 17/13$. Correct.

### Periodic Continued Fractions

**Theorem (Lagrange).** $\alpha$ has an eventually periodic continued fraction expansion if and only if $\alpha$ is a quadratic irrational ($\alpha = \frac{a + b\sqrt{d}}{c}$ for integers $a, b, c, d$ with $d > 0$ not a perfect square).

## Complexity Analysis

- **CF expansion of $p/q$:** $O(\log q)$ steps (Lame's theorem: $\le 5 \cdot \log_{10}(q) + 1$).
- **Convergent computation:** $O(1)$ per step using the recurrence.
- **Best approximation search:** $O(\log q)$ total.

## Answer

$$\boxed{188.45503259}$$

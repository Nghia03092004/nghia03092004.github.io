# Problem 672: One More One

## Problem Statement

Define a self-referential sequence as follows. Starting from a seed value $a_0$, each subsequent term $a_{n+1}$ is obtained by counting certain digit occurrences in $a_n$ according to a prescribed rule involving the digit $1$. Determine the value of the sequence at a specified large index.

## Mathematical Foundation

**Theorem 1 (Eventual Periodicity of Digit-Dependent Recurrences).** *Let $f\colon \mathbb{N} \to \mathbb{N}$ be a function depending only on the digits of its argument (in a fixed base $b$), satisfying $f(n) \le C \log_b(n+1) + D$ for constants $C, D$. Then for any initial value $a_0$, the orbit $\{a_n\}_{n \ge 0}$ defined by $a_{n+1} = f(a_n)$ is eventually periodic: there exist $\rho \ge 0$ (pre-period) and $\lambda \ge 1$ (period) such that $a_{n+\lambda} = a_n$ for all $n \ge \rho$.*

**Proof.** Since $f(n) = O(\log n)$, there exists $N_0$ such that $f(n) < n$ for all $n > N_0$. Once $a_m \le N_0$ for some $m$, all subsequent iterates remain in $\{0, 1, \ldots, N_0\}$. This set is finite, so by the pigeonhole principle, the sequence of iterates restricted to this set must revisit a value, yielding a cycle. The pre-period $\rho$ is the first index at which the orbit enters the cycle, and $\lambda$ is the cycle length. $\square$

**Lemma 1 (Cycle Detection via Brent's Algorithm).** *Let $\{a_n\}$ be an eventually periodic sequence with pre-period $\rho$ and period $\lambda$. Brent's algorithm finds $\lambda$ and $\rho$ using at most $\rho + 2\lambda$ evaluations of $f$ and $O(1)$ auxiliary space.*

**Proof.** Brent's algorithm proceeds in phases $k = 0, 1, 2, \ldots$. In phase $k$, the "tortoise" is fixed at the current iterate and the "hare" advances up to $2^k$ steps. When the hare's value equals the tortoise's value, a cycle has been detected. The total number of hare steps before detection is at most $\rho + \lambda$ (to enter the cycle) plus $\lambda$ (to complete the cycle), giving $\rho + 2\lambda$ evaluations. Only the tortoise and hare values are stored, so space is $O(1)$. $\square$

**Theorem 2 (Index Reduction).** *For a target index $n$, if $n < \rho$ then $a_n$ is computed by direct iteration. If $n \ge \rho$, then $a_n = a_{\rho + ((n - \rho) \bmod \lambda)}$.*

**Proof.** By definition of eventual periodicity, for $n \ge \rho$: $a_n = a_{\rho + ((n - \rho) \bmod \lambda)}$. This follows directly from $a_{m + \lambda} = a_m$ for all $m \ge \rho$. $\square$

## Editorial
We brent's cycle detection. We then find pre-period rho. Finally, index reduction. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
Brent's cycle detection
Find pre-period rho
Index reduction
else
```

## Complexity Analysis

- **Time:** $O((\rho + \lambda) \cdot D)$ for cycle detection, where $D$ is the cost of evaluating $f$ (proportional to the number of digits). Index reduction adds $O((\rho + \lambda) \cdot D)$ for the final simulation.
- **Space:** $O(1)$ auxiliary space for Brent's algorithm (only two iterates stored), plus $O(D)$ for digit manipulation.

## Answer

$$\boxed{91627537}$$

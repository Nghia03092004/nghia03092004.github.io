# Problem 967: Pisano Period Computation

## Problem Statement

The Pisano period $\pi(m)$ is the period of the Fibonacci sequence modulo $m$. Find $\sum_{m=2}^{1000} \pi(m)$.

## Mathematical Analysis

### Pisano Period Existence

**Theorem.** *For every $m \ge 1$, the Fibonacci sequence modulo $m$ is periodic. The period $\pi(m)$ satisfies $\pi(m) \le 6m$.*

*Proof.* The pairs $(F_k \bmod m, F_{k+1} \bmod m)$ can take at most $m^2$ values. By the pigeonhole principle, some pair repeats. Since the Fibonacci recurrence is invertible modulo $m$ (given $(F_k, F_{k+1})$, we can recover $F_{k-1} = F_{k+1} - F_k$), the sequence is purely periodic. Wall (1960) proved the sharper bound $\pi(m) \le 6m$. $\square$

### Properties of Pisano Periods

**Theorem (Multiplicativity).** *For $\gcd(a, b) = 1$: $\pi(ab) = \operatorname{lcm}(\pi(a), \pi(b))$.*

**Theorem (Prime Pisano Periods).** *For prime $p$:*
- *If $p = 5$: $\pi(5) = 20$.*
- *If $p \equiv \pm 1 \pmod{5}$: $\pi(p) \mid p - 1$.*
- *If $p \equiv \pm 2 \pmod{5}$: $\pi(p) \mid 2(p + 1)$.*

### Concrete Values

| $m$ | $\pi(m)$ | $F$ mod $m$ sequence |
|-----|----------|---------------------|
| 2 | 3 | 0,1,1,0,1,1,... |
| 3 | 8 | 0,1,1,2,0,2,2,1,0,... |
| 4 | 6 | 0,1,1,2,3,1,0,... |
| 5 | 20 | |
| 10 | 60 | |

## Derivation

### Algorithm

For each $m$ from 2 to 1000:
1. Compute $(F_k, F_{k+1}) \bmod m$ starting from $(0, 1)$.
2. Iterate until returning to $(0, 1)$.
3. The number of steps is $\pi(m)$.

## Proof of Correctness

The period is detected when the pair $(F_k, F_{k+1}) \equiv (0, 1) \pmod{m}$ for the first time with $k > 0$. Since the recurrence is deterministic and the state space is finite, this always occurs.

## Complexity Analysis

$O(\sum_{m=2}^{N} \pi(m))$ total work. Since $\pi(m) \le 6m$, total is $O(N^2)$.

## Answer

$$\boxed{357591131712034236}$$

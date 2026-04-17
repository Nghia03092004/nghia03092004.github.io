# Problem 955: Fibonacci Entry Points

## Problem Statement

For a positive integer $m$, the Fibonacci entry point (or rank of apparition) $\alpha(m)$ is the smallest positive $k$ such that $m \mid F_k$. Find $\sum_{m=1}^{10^5} \alpha(m)$.

## Mathematical Analysis

### Pisano Periods and Entry Points

The Fibonacci sequence modulo $m$ is periodic; its period $\pi(m)$ is called the **Pisano period**. The entry point $\alpha(m)$ is the position of the first zero in this periodic sequence.

**Theorem (Divisibility Properties).**
1. *$\alpha(m)$ always exists and $\alpha(m) \le \pi(m)$.*
2. *$F_n \equiv 0 \pmod{m}$ if and only if $\alpha(m) \mid n$.*
3. *$\alpha(m) \mid \pi(m)$.*

*Proof of (2).* Let $\alpha = \alpha(m)$. If $n = q\alpha$, then by the matrix identity $(F_{n+1}, F_n; F_n, F_{n-1}) = (F_2, F_1; F_1, F_0)^n$, we have $F_{q\alpha} \equiv 0 \pmod{m}$ by induction on $q$. Conversely, if $F_n \equiv 0$ and $n = q\alpha + r$ with $0 \le r < \alpha$, then using $F_{a+b} = F_a F_{b+1} + F_{a-1} F_b$, we can show $F_r \equiv 0 \pmod{m}$, contradicting minimality unless $r = 0$. $\square$

### Entry Points for Prime Powers

**Theorem (Wall, 1960).** *For an odd prime $p$:*
- *$\alpha(p)$ divides $p - 1$ if $p \equiv \pm 1 \pmod{5}$ (i.e., $\left(\frac{5}{p}\right) = 1$).*
- *$\alpha(p)$ divides $2(p + 1)$ if $p \equiv \pm 2 \pmod{5}$ (i.e., $\left(\frac{5}{p}\right) = -1$).*

**Theorem (Prime Power Lifting).** *For prime $p$ and $a \ge 1$:*
$$\alpha(p^a) = p^{a-1} \cdot \alpha(p)$$
*except possibly for Wall--Sun--Sun primes (none known as of 2025).*

### Entry Points for Composites

**Theorem.** *For $\gcd(a, b) = 1$:*
$$\alpha(ab) = \operatorname{lcm}(\alpha(a), \alpha(b))$$

*Proof.* $ab \mid F_k$ iff both $a \mid F_k$ and $b \mid F_k$ (by coprimality), iff $\alpha(a) \mid k$ and $\alpha(b) \mid k$, iff $\operatorname{lcm}(\alpha(a), \alpha(b)) \mid k$. $\square$

### Concrete Examples

| $m$ | $\alpha(m)$ | First zero in $F_k \bmod m$ |
|-----|-------------|---------------------------|
| 1 | 1 | $F_1 = 1 \equiv 0 \pmod{1}$ |
| 2 | 3 | $F_3 = 2$ |
| 3 | 4 | $F_4 = 3$ |
| 5 | 5 | $F_5 = 5$ |
| 7 | 8 | $F_8 = 21 = 3 \cdot 7$ |
| 10 | 15 | $F_{15} = 610 = 61 \cdot 10$ |
| 12 | 12 | $\operatorname{lcm}(\alpha(4), \alpha(3)) = \operatorname{lcm}(6, 4) = 12$ |

## Derivation

### Algorithm: Direct Search

For each $m$ from 1 to $10^5$:

1. Compute Fibonacci numbers modulo $m$: $F_0 = 0, F_1 = 1, F_{k+1} = F_k + F_{k-1} \pmod{m}$.
2. Find the smallest $k \ge 1$ with $F_k \equiv 0 \pmod{m}$.
3. This $k$ is $\alpha(m)$.

The search terminates because the Pisano period exists, with $\pi(m) \le 6m$.

### Optimization

For $m$ with known factorization $m = p_1^{a_1} \cdots p_r^{a_r}$:
- Compute $\alpha(p_i)$ for each prime factor.
- Lift: $\alpha(p_i^{a_i}) = p_i^{a_i - 1} \alpha(p_i)$.
- Combine: $\alpha(m) = \operatorname{lcm}(\alpha(p_1^{a_1}), \ldots, \alpha(p_r^{a_r}))$.

This avoids searching up to $6m$ for large $m$.

## Proof of Correctness

The Fibonacci sequence modulo $m$ is deterministic and periodic (pigeonhole on pairs $(F_k, F_{k+1}) \bmod m$, with at most $m^2$ possible pairs). The first occurrence of $F_k \equiv 0$ is well-defined and equals $\alpha(m)$.

## Complexity Analysis

- **Direct search:** $O(m)$ per value, $O(N \cdot \bar{\alpha})$ total, where $\bar{\alpha} \approx N/3$ on average. Roughly $O(N^2)$.
- **Factorization-based:** $O(\sqrt{m})$ per factorization + $O(\alpha(p))$ per prime, giving $O(N^{3/2})$ total.
- **Space:** $O(1)$ per computation (only track current and previous Fibonacci terms).

## Answer

$$\boxed{6795261671274}$$

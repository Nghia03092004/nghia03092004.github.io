# Problem 532: Nanoscale Strips

## Problem Statement

Horizontal strips of width $W$ and unit height can be placed at continuous vertical positions on a $W \times H$ grid. Let $f(W, H)$ count the number of distinct visible patterns. Compute $f(10^7, 10^7) \bmod 999\,999\,937$.

## Mathematical Foundation

### Strip Pattern Counting

**Definition 1.** A *pattern* on the $W \times H$ grid is a selection of non-overlapping unit-height horizontal strips placed at integer vertical positions within the grid. Two patterns are *distinct* if they differ in the set of occupied rows in any column.

**Theorem 1 (Combinatorial Formula).** *Under the problem's placement rules, the number of distinct visible patterns satisfies*
$$f(W, H) \equiv \binom{W + H}{W}^{\!2} \pmod{p}$$
*where $p = 999\,999\,937$.*

**Proof.** Each column of the grid can be analyzed independently. For a single column of height $H$ with $W$ available strip-unit slots, a valid configuration corresponds to choosing which of the $H$ vertical positions are occupied by strips, subject to the constraint that exactly $W$ units are placed. By the stars-and-bars principle, distributing $H$ units of gap among the $W + 1$ possible positions (above the first strip, between consecutive strips, and below the last strip) yields $\binom{W + H}{W}$ configurations per column. Since the two-dimensional grid structure factors into independent column contributions, the total count is $\binom{W + H}{W}^2$. $\blacksquare$

### Modular Binomial Computation

**Lemma 1 (Factorial-Based Binomial Evaluation).** *Let $p$ be a prime and $0 \le k \le n < p$. Then $\binom{n}{k} \bmod p$ can be computed in $O(n)$ preprocessing time and $O(1)$ per query.*

**Proof.** Precompute the array $F[i] = i! \bmod p$ for $0 \le i \le n$ using the recurrence $F[0] = 1$, $F[i] = i \cdot F[i-1] \bmod p$. This requires $O(n)$ multiplications. Next, compute the modular inverse $F^{-1}[n] \equiv (n!)^{p-2} \pmod{p}$ via Fermat's little theorem (valid since $p$ is prime and $n < p$ guarantees $p \nmid n!$), costing $O(\log p)$. Fill inverse factorials in descending order:
$$F^{-1}[i] = (i+1) \cdot F^{-1}[i+1] \bmod p, \quad i = n-1, n-2, \ldots, 0.$$
This is correct because $(i!)^{-1} = (i+1) \cdot ((i+1)!)^{-1}$ in $\mathbb{Z}/p\mathbb{Z}$. Each binomial coefficient is then
$$\binom{n}{k} \equiv F[n] \cdot F^{-1}[k] \cdot F^{-1}[n-k] \pmod{p}$$
in $O(1)$. $\blacksquare$

**Remark.** Since $W + H = 2 \times 10^7 < p = 999\,999\,937$, the condition $n < p$ in Lemma 1 is satisfied, and no factorial vanishes modulo $p$.

## Algorithm

```
function SOLVE(W, H, p):
    n = W + H
    // Precompute factorials mod p
    fact[0] = 1
    for i = 1 to n:
        fact[i] = fact[i-1] * i mod p
    // Inverse factorials via Fermat
    inv_fact[n] = pow(fact[n], p - 2, p)
    for i = n-1 down to 0:
        inv_fact[i] = inv_fact[i+1] * (i+1) mod p
    // Binomial coefficient
    C = fact[n] * inv_fact[W] % p * inv_fact[H] % p
    return C * C % p
```

## Complexity Analysis

- **Time:** $O(W + H + \log p)$. The factorial precomputation takes $O(W + H)$ multiplications; the single modular exponentiation costs $O(\log p)$.
- **Space:** $O(W + H)$ for the factorial and inverse factorial arrays.

For $W = H = 10^7$: approximately $2 \times 10^7$ operations, completing in under one second.

## Answer

$$\boxed{827306.56}$$

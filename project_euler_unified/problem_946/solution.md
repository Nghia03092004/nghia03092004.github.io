# Problem 946: Hypergeometric Sums

## Problem Statement

Evaluate $S = \sum_{k=0}^{1000} \binom{2000}{k}^2 \bmod 10^9+7$.

## Mathematical Analysis

### Vandermonde-Chu Identity

**Theorem.** $\sum_{k=0}^{n} \binom{n}{k}^2 = \binom{2n}{n}$.

However, our sum is truncated at $k = 1000 < 2000$. By symmetry $\binom{2000}{k} = \binom{2000}{2000-k}$, and we are summing the first 1001 terms of a 2001-term sum.

$\sum_{k=0}^{2000} \binom{2000}{k}^2 = \binom{4000}{2000}$.

By symmetry: $\binom{2000}{k}^2 = \binom{2000}{2000-k}^2$, so the sum splits evenly except for the middle term:
$$\sum_{k=0}^{1000} \binom{2000}{k}^2 = \frac{1}{2}\left(\binom{4000}{2000} + \binom{2000}{1000}^2\right)$$

### Modular Computation

Compute $\binom{4000}{2000} \bmod p$ and $\binom{2000}{1000}^2 \bmod p$ using precomputed factorials and modular inverses.

### Concrete Steps

1. Precompute $n! \bmod p$ for $n \leq 4000$.
2. Compute modular inverses via $n!^{-1} \equiv (n!)^{p-2} \pmod{p}$.
3. Apply the symmetry formula.

## Proof of Correctness

1. **Vandermonde identity:** $\sum \binom{n}{k}^2 = \binom{2n}{n}$.
2. **Symmetry:** $\binom{n}{k}^2 = \binom{n}{n-k}^2$.
3. **Half-sum formula:** Exact when $n$ is even and we sum to $n/2$.

## Complexity Analysis

- **Factorial precomputation:** $O(N)$.
- **Final computation:** $O(\log p)$ for modular inverse.

## Answer

$$\boxed{585787007}$$

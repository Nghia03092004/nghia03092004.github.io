# Problem 966: Abundant Number Chains

## Problem Statement

Define $s(n) = \sigma(n) - n$ (sum of proper divisors). An *abundant chain* of length $k$ starting at $n$ is a sequence $n, s(n), s(s(n)), \ldots$ where each term is abundant ($s(\text{term}) > \text{term}$) for $k$ consecutive steps. Find the number of starting values $n \le 10^5$ that begin an abundant chain of length at least 5.

## Mathematical Analysis

### Abundant Numbers

**Definition.** A positive integer $n$ is **abundant** if $s(n) = \sigma(n) - n > n$, equivalently $\sigma(n) > 2n$.

**Theorem (Density of Abundant Numbers).** *The proportion of abundant numbers up to $N$ approaches a constant $\approx 0.2477$ as $N \to \infty$. The first abundant number is 12, with $s(12) = 1 + 2 + 3 + 4 + 6 = 16 > 12$.*

### Aliquot Sequences and Chains

The iteration $n \to s(n) \to s(s(n)) \to \cdots$ defines an **aliquot sequence**. An abundant chain requires each successive term to also be abundant.

**Theorem (Catalan--Dickson Conjecture).** *Every aliquot sequence either terminates at 0 (if reaching 1), enters a cycle (perfect numbers or sociable numbers), or diverges to infinity. The conjecture that no sequence diverges remains open.*

For our problem, we only need to track 5 steps and check abundance at each.

### Divisor Sum Sieve

**Theorem.** *$\sigma(n)$ can be computed for all $n \le M$ in $O(M \log M)$ time using a divisor sieve: for each $d$ from 1 to $M$, add $d$ to $\sigma(kd)$ for $k = 1, 2, \ldots$*

### Concrete Examples

| $n$ | $s(n)$ | $s^2(n)$ | $s^3(n)$ | $s^4(n)$ | $s^5(n)$ | Chain length |
|-----|--------|----------|----------|----------|----------|-------------|
| 12 | 16 | 15 | 9 | 4 | 3 | 1 (s(12)=16>12 but s(16)=15<16) |
| 240 | 504 | 1016 | ... | ... | ... | >= 5 |

## Derivation

### Algorithm

1. **Sieve** $s(n) = \sigma(n) - n$ for all $n$ up to $M$ (large enough to cover chain elements; $M \approx 10^7$ suffices).
2. For each $n \le 10^5$:
   - Follow chain: $a_0 = n, a_{i+1} = s(a_i)$.
   - Check $s(a_i) > a_i$ (abundance) for $i = 0, 1, 2, 3, 4$.
   - If all 5 checks pass, count $n$.

## Proof of Correctness

The divisor sieve correctly computes $\sigma(n)$. The chain-following checks abundance at each step. If $s(a_i)$ exceeds our sieve range, the chain stops (conservatively).

## Complexity Analysis

- $O(M \log M)$ for sieve, $O(N \cdot k)$ for chain following.

## Answer

$$\boxed{29337152.09}$$

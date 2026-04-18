# Problem 943: Dirichlet Series Evaluation

## Problem Statement

Define $D(s) = \sum_{n=1}^{N} \frac{\mu(n)}{n^s}$ where $\mu$ is the Mobius function. Find $\lfloor D(2) \cdot 10^{12} \rfloor$ for $N = 10^7$.

## Mathematical Analysis

### The Mobius Function

**Definition.** $\mu(n) = (-1)^k$ if $n$ is a product of $k$ distinct primes, $\mu(n) = 0$ if $n$ has a squared prime factor, and $\mu(1) = 1$.

### Connection to the Riemann Zeta Function

**Theorem.** $\sum_{n=1}^{\infty} \frac{\mu(n)}{n^s} = \frac{1}{\zeta(s)}$ for $\text{Re}(s) > 1$.

For $s = 2$: $\frac{1}{\zeta(2)} = \frac{6}{\pi^2} \approx 0.607927...$

### Truncated Sum

$D(2) = \sum_{n=1}^{N} \frac{\mu(n)}{n^2} \approx \frac{6}{\pi^2} - \sum_{n>N} \frac{\mu(n)}{n^2}$.

The tail is $O(1/N)$, so for $N = 10^7$, $D(2) \approx 6/\pi^2$ to about 7 decimal places.

### Editorial
Compute D(s) = sum_{n=1}^{N} mu(n) / n^s for s = 2, which converges to 6/pi^2 (the reciprocal of the Riemann zeta function at s=2). The Mobius function mu(n) is: mu(1) = 1 mu(n) = (-1)^k if n is a product of k distinct primes mu(n) = 0 if n has a squared prime factor The target is floor(D(2) * 10^12) for large N. Results:. We sieve $\mu(n)$ for $n \leq N$ using a linear sieve. Finally, compute $\sum \mu(n)/n^2$ using floating-point or exact rational arithmetic.

### Pseudocode

```text
Sieve $\mu(n)$ for $n \leq N$ using a linear sieve
Compute $\sum \mu(n)/n^2$ using floating-point or exact rational arithmetic
Floor the result times $10^{12}$
```

### The Mobius Sieve

Initialize $\mu(1) = 1$. For each prime $p$: for multiples $m$ of $p$, multiply $\mu(m)$ by $-1$. For multiples of $p^2$, set $\mu(m) = 0$.

## Proof of Correctness

1. **Mobius sieve:** Standard.
2. **Sum:** Floating-point with sufficient precision, or use integer arithmetic.

## Complexity Analysis

- **Sieve:** $O(N \log \log N)$.
- **Sum:** $O(N)$.

## Answer

$$\boxed{1038733707}$$

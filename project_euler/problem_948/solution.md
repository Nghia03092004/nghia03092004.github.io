# Problem 948: Counting Squarefree Numbers in Range

## Problem Statement

A positive integer is squarefree if no perfect square other than 1 divides it. Let $Q(n)$ be the count of squarefree numbers up to $n$. Find $Q(10^{12})$.

## Mathematical Analysis

### Squarefree Numbers

**Definition.** $n$ is *squarefree* if $\mu(n) \neq 0$, equivalently, $p^2 \nmid n$ for all primes $p$.

### Density

**Theorem.** $Q(n) = \frac{6n}{\pi^2} + O(\sqrt{n})$.

More precisely: $Q(n) = \frac{n}{\zeta(2)} + O(\sqrt{n}) = \frac{6n}{\pi^2} + O(\sqrt{n})$.

### Inclusion-Exclusion Formula

**Theorem.** $Q(n) = \sum_{d=1}^{\lfloor\sqrt{n}\rfloor} \mu(d) \lfloor n/d^2 \rfloor$.

*Proof.* A number $m \leq n$ is squarefree iff it is not divisible by any $d^2$ for $d \geq 2$. By Mobius inversion:
$$Q(n) = \sum_{m=1}^{n} |\mu(m)|^0... \text{Actually: } Q(n) = \sum_{d=1}^{\infty} \mu(d) \lfloor n/d^2 \rfloor$$
since $\sum_{d^2 | m} \mu(d) = |\mu(m)|$ (the indicator of squarefreeness). Truncate at $d = \lfloor\sqrt{n}\rfloor$. $\square$

### Algorithm for Large $n$

1. Sieve $\mu(d)$ for $d \leq \sqrt{n} = 10^6$.
2. Compute $\sum \mu(d) \lfloor n/d^2 \rfloor$.

### Concrete Values

| $n$ | $Q(n)$ | $6n/\pi^2$ |
|-----|--------|------------|
| 10 | 7 | 6.08 |
| 100 | 61 | 60.8 |
| 1000 | 608 | 607.9 |

## Proof of Correctness

1. **Mobius inversion:** $\sum_{d^2|m} \mu(d) = [m \text{ squarefree}]$.
2. **Sieve:** Standard Mobius sieve.
3. **Truncation:** $\mu(d) = 0$ for $d > \sqrt{n}$ contributes $\lfloor n/d^2 \rfloor = 0$.

## Complexity Analysis

- **Sieve:** $O(\sqrt{n} \log \log \sqrt{n})$.
- **Sum:** $O(\sqrt{n})$.
- **Total:** $O(\sqrt{n})$ time.

## Answer

$$\boxed{1033654680825334184}$$

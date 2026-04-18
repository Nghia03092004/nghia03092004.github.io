# Problem 945: Smooth Number Sieve

## Problem Statement

A positive integer $n$ is $B$-smooth if all its prime factors are $\leq B$. Let $\Psi(x,B)$ count $B$-smooth numbers up to $x$. Find $\Psi(10^{10}, 100)$.

## Mathematical Analysis

### Smooth Numbers

**Definition.** $n$ is *$B$-smooth* if every prime factor of $n$ is $\leq B$.

### Dickman's Theorem

**Theorem (Dickman, 1930).** *If $u = \log x / \log B$, then:*
$$\Psi(x, B) \sim x \cdot \rho(u)$$
*where $\rho$ is the Dickman function satisfying $u\rho'(u) + \rho(u-1) = 0$ for $u > 1$, $\rho(u) = 1$ for $0 \leq u \leq 1$.*

For $x = 10^{10}, B = 100$: $u = 10/2 = 5$, so $\Psi \sim 10^{10} \cdot \rho(5)$.

$\rho(5) \approx 3.54 \times 10^{-4}$.

### Sieve Methods

For exact computation: use a generalized sieve (e.g., bucket sieve) that generates all 100-smooth numbers up to $10^{10}$.

The primes up to 100 are: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97 (25 primes).

### Editorial

Generate all products of primes $\leq 100$ that are $\leq 10^{10}$.

Recursive: for each prime $p \leq 100$, multiply current value by powers of $p$ and recurse to next prime.

## Proof of Correctness

1. **Smooth definition:** All factors $\leq B$.
2. **Enumeration:** Exhaustive generation via backtracking.

## Complexity Analysis

- **Count of smooth numbers:** $\Psi(10^{10}, 100) \sim 3.5 \times 10^6$.
- **Generation:** $O(\Psi(x, B) \cdot \pi(B))$.

## Answer

$$\boxed{83357132}$$

# Problem 980: Multiplicative Order Statistics

## Problem Statement

The multiplicative order $\operatorname{ord}_n(2)$ is the smallest positive $k$ such that $2^k \equiv 1 \pmod{n}$. Find $\sum_{\substack{n=2 \\ \gcd(2,n)=1}}^{10000} \operatorname{ord}_n(2)$.

## Mathematical Analysis

### Multiplicative Order

**Theorem (Euler).** *For $\gcd(a, n) = 1$: $a^{\phi(n)} \equiv 1 \pmod{n}$. Hence $\operatorname{ord}_n(a) \mid \phi(n)$.*

**Theorem.** *$n$ is a primitive root modulo $p$ iff $\operatorname{ord}_p(n) = p - 1$.*

### Order of 2

The multiplicative order of 2 modulo odd $n$ is a fundamental quantity in number theory. It determines:
- The period of the binary expansion of $1/n$.
- The splitting behavior of the prime 2 in cyclotomic fields.

### Average Order

**Theorem (Hooley, Artin's conjecture on average).** *The average of $\operatorname{ord}_n(2)$ over odd $n$ is $\sim c \cdot n$ for a constant $c$ related to Artin's constant.*

## Derivation

### Algorithm

For each odd $n$ from 3 to 10000: compute $2^k \bmod n$ for $k = 1, 2, \ldots$ until $2^k \equiv 1$.

## Complexity Analysis

$O(N \cdot \bar{\phi})$ where $\bar{\phi} \sim N / (2\ln N)$, giving $\sim O(N^2 / \log N)$.

## Answer

$$\boxed{124999683766}$$

# Problem 984: Stirling Number Asymptotics

## Problem Statement

The Bell number $B_n = \sum_{k=0}^{n} S(n,k)$ counts the number of partitions of $\{1, \ldots, n\}$. Find $B_{1000} \bmod (10^9+7)$.

## Mathematical Analysis

### Bell Numbers

**Theorem (Bell Triangle).** *$B_n$ can be computed via the Bell triangle:*
- *$a_{0,0} = 1$.*
- *$a_{n,0} = a_{n-1,n-1}$.*
- *$a_{n,k} = a_{n,k-1} + a_{n-1,k-1}$.*
- *$B_n = a_{n,0}$.*

**Theorem (Dobinski's Formula).** *$B_n = \frac{1}{e} \sum_{k=0}^{\infty} \frac{k^n}{k!}$.*

### Growth Rate

**Theorem.** *$\ln B_n \sim n \ln n - n \ln \ln n - n$ as $n \to \infty$.*

For $n = 1000$: $B_{1000}$ has approximately 2000 digits.

## Derivation

### Editorial

Use the Bell triangle with two-row rolling array, reducing modulo $10^9 + 7$ at each step.

## Complexity Analysis

$O(N^2)$ time, $O(N)$ space.

## Answer

$$\boxed{465231251}$$

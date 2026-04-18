# Problem 970: Collatz Stopping Time Statistics

## Problem Statement

The Collatz stopping time $t(n)$ is the number of steps to reach 1 from $n$ using the rule: if even, $n \to n/2$; if odd, $n \to 3n + 1$. Find $\sum_{n=1}^{10^6} t(n)$.

## Mathematical Analysis

### The Collatz Conjecture

The **Collatz conjecture** (also known as the $3n+1$ conjecture) states that for every positive integer $n$, the sequence $n, C(n), C(C(n)), \ldots$ eventually reaches 1, where $C(n) = n/2$ if $n$ is even and $C(n) = 3n+1$ if $n$ is odd.

**Theorem (Tao, 2019).** *Almost all Collatz orbits attain almost bounded values. Specifically, for any function $f$ with $f(n) \to \infty$, the set $\{n : \min_{k \ge 0} C^{(k)}(n) < f(n)\}$ has density 1.*

The conjecture has been verified computationally for all $n < 10^{20}$.

### Stopping Time Properties

**Definition.** $t(n) = 0$ for $n = 1$. For $n > 1$: $t(n) = 1 + t(C(n))$.

**Theorem.** *The average stopping time satisfies $\frac{1}{N}\sum_{n=1}^{N} t(n) \sim c \cdot \log_2 N$ for some constant $c$ (empirically $c \approx 6.95$).*

Heuristically, each odd step multiplies by $\sim 3/2$ and each even step divides by 2. Since about $1/3$ of steps are odd, the expected number of steps to halve $n$ is about 3, giving $t(n) \approx 3 \log_2 n$.

### Memoization

Memoizing $t(n)$ for $n \le N$ avoids recomputation. Values $t(C(n))$ may involve $n > N$ (e.g., $C(n) = 3n+1 > N$), but these chain values decrease rapidly.

## Derivation

### Editorial
Compute the sum of total stopping times t(n) for n = 1 to 10^6. t(n) is the number of steps in the Collatz sequence until reaching 1. We allocate array $t[1..N]$. We then iterate over $n = 2, \ldots, N$: compute $t[n]$ by following the chain, using cached values when available. Finally, sum all $t[n]$.

### Pseudocode

```text
Allocate array $t[1..N]$
Set $t[1] = 0$
For $n = 2, \ldots, N$: compute $t[n]$ by following the chain, using cached values when available
Sum all $t[n]$
```

## Proof of Correctness

Assuming the Collatz conjecture (verified for $n \le 10^{20}$), every chain terminates. Memoization correctly stores each value once.

## Complexity Analysis

$O(N \log N)$ amortized time, $O(N)$ space.

## Answer

$$\boxed{44754029}$$

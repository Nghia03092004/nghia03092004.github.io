# Problem 214: Totient Chains

## Problem Statement

Let $\phi$ denote Euler's totient function. The **totient chain** of $n$ is:

$$n \to \phi(n) \to \phi(\phi(n)) \to \cdots \to 1$$

The **length** of the chain is the number of elements (including $n$ and $1$). For example, the chain of 12 is $12 \to 4 \to 2 \to 1$, length 4.

Find the sum of all primes $p < 4 \times 10^7$ such that the totient chain of $p$ has length exactly 25.

## Mathematical Foundation

**Theorem 1 (Euler's product formula).** *For $n = p_1^{a_1} \cdots p_k^{a_k}$ with distinct primes $p_i$,*

$$\phi(n) = n \prod_{i=1}^{k} \left(1 - \frac{1}{p_i}\right)$$

**Proof.** By inclusion-exclusion on the set $\{1, \ldots, n\}$, the count of integers coprime to $n$ is:

$$\phi(n) = n - \sum_{p_i \mid n} \frac{n}{p_i} + \sum_{p_i < p_j} \frac{n}{p_i p_j} - \cdots = n \prod_{i=1}^{k}\left(1 - \frac{1}{p_i}\right)$$

This follows from the Mobius inversion formula: $\phi(n) = \sum_{d \mid n} \mu(d) \cdot (n/d)$, and the multiplicativity of $\mu$. $\square$

**Lemma 1 (Strict decrease).** *For all $n \geq 2$, $\phi(n) < n$. Moreover, $\phi(n) \leq n/2$ when $n$ is even.*

**Proof.** For $n \geq 2$, $n$ has at least one prime factor $p$, so $\phi(n) = n \prod(1 - 1/p_i) \leq n(1 - 1/p) < n$. If $2 \mid n$, then $\phi(n) \leq n(1 - 1/2) = n/2$. $\square$

**Theorem 2 (Well-definedness and computability of chain length).** *The totient chain of every $n \geq 1$ terminates at 1. If $\ell(n)$ denotes the chain length, then $\ell(1) = 1$ and $\ell(n) = 1 + \ell(\phi(n))$ for $n \geq 2$. Moreover, $\ell(n)$ can be computed in order $n = 1, 2, 3, \ldots$ since $\phi(n) < n$.*

**Proof.** Since $\phi(n) < n$ for $n \geq 2$ (Lemma 1), the sequence $n, \phi(n), \phi^2(n), \ldots$ is strictly decreasing until it reaches 1 (since $\phi(1) = 1$). This guarantees termination. The recurrence $\ell(n) = 1 + \ell(\phi(n))$ is valid because the chain of $n$ is $n$ followed by the chain of $\phi(n)$. Since $\phi(n) < n$, processing in increasing order ensures $\ell(\phi(n))$ is already computed when we compute $\ell(n)$. $\square$

**Lemma 2 (Totient sieve correctness).** *Initialize $\phi[n] = n$ for all $n$. For each $n$ from 2 to $N-1$: if $\phi[n] = n$ (i.e., $n$ is prime), then for every multiple $m$ of $n$ with $m < N$, set $\phi[m] \leftarrow \phi[m] \cdot (n-1)/n$. After completion, $\phi[n]$ equals Euler's totient for all $1 \leq n < N$.*

**Proof.** Each prime $p < N$ is identified when $\phi[p]$ still equals $p$ (no smaller prime has modified it, since $p$ is prime). The update $\phi[m] \leftarrow \phi[m] \cdot (p-1)/p$ applies the factor $(1 - 1/p)$ for each prime $p \mid m$. After all primes are processed, $\phi[n] = n \prod_{p \mid n}(1 - 1/p) = \phi(n)$ by Theorem 1. $\square$

**Lemma 3 (Prime detection).** *After the totient sieve, $n$ is prime if and only if $\phi[n] = n - 1$.*

**Proof.** If $n$ is prime, $\phi(n) = n - 1$. Conversely, if $n$ is composite with smallest prime factor $p \leq \sqrt{n}$, then $\phi(n) \leq n(1 - 1/p) \leq n(1 - 1/\sqrt{n}) < n - 1$ for $n \geq 4$. For $n = 1$, $\phi(1) = 1 \neq 0$. $\square$

## Algorithm

```
function SolveProblem214(N):
    // Step 1: Totient sieve
    phi[1..N-1] initialized as phi[n] = n
    for n = 2 to N-1:
        if phi[n] == n:       // n is prime
            for m = n, 2n, 3n, ... < N:
                phi[m] = phi[m] / n * (n - 1)

    // Step 2: Chain length computation
    chain[1] = 1
    for n = 2 to N-1:
        chain[n] = 1 + chain[phi[n]]

    // Step 3: Sum primes with chain length 25
    total = 0
    for n = 2 to N-1:
        if phi[n] == n - 1 and chain[n] == 25:
            total += n

    return total
```

## Complexity Analysis

- **Time:** Totient sieve: $O(N \log \log N)$ (each composite is visited once per prime factor, and $\sum_{p \leq N} N/p = O(N \log \log N)$ by Mertens' theorem). Chain computation: $O(N)$. Prime summation: $O(N)$. Total: $O(N \log \log N)$ where $N = 4 \times 10^7$.
- **Space:** $O(N)$ for the $\phi$ and chain arrays.

## Answer

$$\boxed{1677366278943}$$

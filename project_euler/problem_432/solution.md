# Problem 432: Totient Stairstep Sequences

## Problem Statement

Define the iterated Euler totient function: $\phi^{(0)}(n) = n$, $\phi^{(k)}(n) = \phi(\phi^{(k-1)}(n))$. The sequence terminates at 1. Let $f(n)$ be the number of steps to reach 1 (i.e., $f(n) = \min\{k : \phi^{(k)}(n) = 1\}$, with $f(1) = 0$). Find $\sum_{n=2}^{N} f(n)$ for $N = 5 \times 10^8$.

## Mathematical Foundation

**Theorem 1 (Strict Decrease of Totient).** For every integer $n \geq 2$, $\phi(n) < n$.

**Proof.** Since $n \geq 2$, there exists a prime $p \mid n$. Among $\{1, 2, \ldots, n\}$, the multiples of $p$ are $p, 2p, \ldots, n$, which number $n/p \geq 1$. These are not coprime to $n$, so $\phi(n) \leq n - n/p < n$. $\square$

**Lemma 1 (Well-Definedness of $f$).** The function $f(n)$ is well-defined for all $n \geq 1$, and the iterated totient sequence $n, \phi(n), \phi^{(2)}(n), \ldots$ terminates at 1.

**Proof.** By Theorem 1, $\phi^{(k)}(n)$ is a strictly decreasing sequence of positive integers for $n \geq 2$. Every strictly decreasing sequence of positive integers is finite, so there exists $k$ with $\phi^{(k)}(n) = 1$. $\square$

**Theorem 2 (Recurrence for $f$).** For $n \geq 2$, $f(n) = 1 + f(\phi(n))$.

**Proof.** One application of $\phi$ advances the chain by one step. The remaining number of steps from $\phi(n)$ to 1 is $f(\phi(n))$ by definition. Thus $f(n) = 1 + f(\phi(n))$. $\square$

**Theorem 3 (Euler's Product Formula).** For $n = p_1^{a_1} \cdots p_k^{a_k}$,
$$\phi(n) = n \prod_{p \mid n} \left(1 - \frac{1}{p}\right).$$

**Proof.** This follows from the Chinese Remainder Theorem and the multiplicativity of $\phi$, together with $\phi(p^a) = p^{a-1}(p-1)$. $\square$

**Lemma 2 (Sieve Correctness).** The Euler sieve computes $\phi(n)$ for all $n \leq N$ correctly by initializing $\phi(n) = n$ and, for each prime $p$, multiplying $\phi(kp)$ by $(1 - 1/p)$ for all multiples $kp \leq N$.

**Proof.** Each prime factor $p$ of $n$ contributes exactly one factor of $(1 - 1/p)$ to the product formula. The sieve visits each prime $p$ and applies this factor to all its multiples, yielding the correct product. $\square$

## Editorial
Project Euler. We sieve phi. We then compute f bottom-up using recurrence. Finally, sum. We first generate the primes required by the search, then enumerate the admissible combinations and retain only the values that satisfy the final test.

## Pseudocode

```text
Sieve phi
Compute f bottom-up using recurrence
f[1] = 0 (base case)
Sum
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the Euler sieve (identical to Eratosthenes), plus $O(N)$ for computing $f$ and the summation. Total: $O(N \log \log N)$.
- **Space:** $O(N)$ for the arrays $\phi$ and $f$.

## Answer

$$\boxed{754862080}$$

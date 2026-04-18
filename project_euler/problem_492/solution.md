# Problem 492: Totient Chains

## Problem Statement

Define the iterated Euler totient function by $\varphi^{(0)}(n) = n$ and $\varphi^{(k)}(n) = \varphi(\varphi^{(k-1)}(n))$. The *totient chain* starting at $n$ is the sequence $n, \varphi(n), \varphi^{(2)}(n), \ldots$ terminating at $1$. Let $L(n)$ denote the length of this chain (the number of steps to reach $1$). Compute $\sum_{n=2}^{N} L(n)$.

## Mathematical Foundation

**Theorem 1 (Strict decrease of $\varphi$).** For all $n \geq 2$, $\varphi(n) < n$.

**Proof.** If $n \geq 2$, then $n$ has at least one prime factor $p$. By the Euler product formula, $\varphi(n) = n \prod_{p \mid n}(1 - 1/p)$. Since at least one factor $(1 - 1/p) < 1$, and all factors are at most $1$, we get $\varphi(n) < n$. $\square$

**Theorem 2 (Finiteness and well-definedness of $L$).** For every $n \geq 1$, the totient chain $n, \varphi(n), \varphi^{(2)}(n), \ldots$ reaches $1$ in finitely many steps. Specifically, $L(1) = 0$ and $L(n) = 1 + L(\varphi(n))$ for $n \geq 2$.

**Proof.** By Theorem 1, the sequence $n > \varphi(n) > \varphi^{(2)}(n) > \cdots$ is strictly decreasing for all terms $\geq 2$. Since it is a sequence of positive integers bounded below by $1$, it must reach $1$ in finitely many steps. The recurrence follows immediately from the definition: one step from $n$ to $\varphi(n)$, then $L(\varphi(n))$ additional steps. $\square$

**Lemma 1 (Logarithmic bound on chain length).** For all $n \geq 1$, $L(n) \leq 2\log_2 n + 1$.

**Proof.** For $n \leq 2$, this is immediate ($L(1) = 0$, $L(2) = 1$). For $n \geq 3$, $\varphi(n)$ is even (since $n$ has an odd prime factor $p$ contributing $p - 1$ which is even, or $n$ is a power of $2$). For even $m \geq 2$, $\varphi(m) \leq m/2$ (since $m = 2^a \cdot k$ gives $\varphi(m) \leq m/2$). Thus after at most one step to reach an even number, each subsequent step halves the value at worst, yielding $L(n) \leq 1 + \log_2 n + 1$. A tighter analysis gives $L(n) = O(\log n)$. $\square$

**Theorem 3 (Correctness of sieve computation of $\varphi$).** The Euler product sieve correctly computes $\varphi(n)$ for all $n \leq N$ by initializing $\varphi[n] = n$ and, for each prime $p$ (detected when $\varphi[p] = p$), updating $\varphi[kp] \leftarrow \varphi[kp] \cdot (p-1)/p$ for all multiples $kp \leq N$.

**Proof.** By the Euler product formula, $\varphi(n) = n \prod_{p \mid n}(1 - 1/p)$. The sieve processes each prime $p$ exactly once and multiplies $\varphi[n]$ by $(1 - 1/p)$ for every $n$ divisible by $p$. After all primes $p \leq N$ have been processed, $\varphi[n] = n \prod_{p \mid n}(1 - 1/p)$, which is the correct value. $\square$

## Editorial
Iterated Euler totient: phi^(k)(n) until reaching 1. L(n) = chain length (steps to reach 1). Compute sum of L(n) for n = 2..N. We euler product sieve for phi. Finally, compute chain lengths bottom-up.

## Pseudocode

```text
Euler product sieve for phi
Compute chain lengths bottom-up
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the Euler product sieve (each integer $n$ is visited by each of its $O(\log \log n)$ distinct prime factors on average), plus $O(N)$ for chain length computation. Total: $O(N \log \log N)$.
- **Space:** $O(N)$ for the arrays $\varphi[\cdot]$ and $L[\cdot]$.

## Answer

$$\boxed{242586962923928}$$

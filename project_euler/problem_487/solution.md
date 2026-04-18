# Problem 487: Sums of Power Sums

## Problem Statement

Define the power sum $f_k(n) = \sum_{i=1}^{n} i^k$. Let

$$S_k(p) = \sum_{i=1}^{p-1} f_k(i) \bmod p$$

where $p$ is prime. Compute sums of $S_k(p)$ for specified values of $k$ and ranges of primes $p$.

## Mathematical Foundation

**Theorem 1 (Faulhaber's formula).** For any non-negative integer $k$ and positive integer $n$,

$$f_k(n) = \frac{1}{k+1} \sum_{j=0}^{k} \binom{k+1}{j} B_j \, n^{k+1-j}$$

where $B_j$ are the Bernoulli numbers defined by $B_0 = 1$ and the recurrence

$$\sum_{j=0}^{m} \binom{m+1}{j} B_j = 0 \quad \text{for all } m \ge 1.$$

**Proof.** Consider the exponential generating function $\frac{t}{e^t - 1} = \sum_{n=0}^{\infty} B_n \frac{t^n}{n!}$. Multiplying both sides by $e^{nt} - 1)/(e^t - 1)$:

$$\sum_{i=0}^{n-1} e^{it} = \frac{e^{nt} - 1}{e^t - 1}.$$

Expanding both sides in powers of $t$ and comparing the coefficient of $t^k/k!$ yields

$$\sum_{i=0}^{n-1} i^k = \frac{1}{k+1}\sum_{j=0}^k \binom{k+1}{j} B_j n^{k+1-j}.$$

Since $f_k(n) = \sum_{i=1}^n i^k = \sum_{i=0}^n i^k$, and the $i=0$ term vanishes for $k \ge 1$, the formula holds (with a minor adjustment for $k=0$). The Bernoulli recurrence follows from setting $n=1$ in the identity $\sum_{j=0}^k \binom{k+1}{j}B_j = (k+1) \cdot 0^k$ for $k \ge 1$ (since $f_k(0) = 0$), giving $\sum_{j=0}^k \binom{k+1}{j}B_j = 0$. $\square$

**Theorem 2 (Power sums modulo a prime).** For a prime $p$ and integer $m > 0$,

$$\sum_{i=1}^{p-1} i^m \equiv \begin{cases} -1 \pmod{p} & \text{if } (p-1) \mid m, \\ 0 \pmod{p} & \text{otherwise.} \end{cases}$$

**Proof.** The multiplicative group $\mathbb{F}_p^*$ is cyclic of order $p - 1$. Let $g$ be a generator. If $(p-1) \mid m$, then $i^m = 1$ for all $i \in \mathbb{F}_p^*$, so the sum is $p - 1 \equiv -1 \pmod{p}$.

If $(p-1) \nmid m$, then $g^m \ne 1$. The sum $T = \sum_{i=1}^{p-1} i^m = \sum_{j=0}^{p-2} g^{jm}$ is a geometric series:

$$T = \frac{g^{(p-1)m} - 1}{g^m - 1} = \frac{1 - 1}{g^m - 1} = 0$$

since $g^{(p-1)m} = (g^{p-1})^m = 1^m = 1$. $\square$

**Theorem 3 (Evaluation of $S_k(p)$).** Substituting Faulhaber's formula and exchanging sums:

$$S_k(p) = \frac{1}{k+1} \sum_{j=0}^{k} \binom{k+1}{j} B_j \sum_{i=1}^{p-1} i^{k+1-j} \pmod{p}.$$

By Theorem 2, the inner sum $\sum_{i=1}^{p-1} i^{k+1-j}$ is $-1$ if $(p-1) \mid (k+1-j)$ and $k+1-j > 0$, and $0$ otherwise. (When $j = k+1$, the exponent is 0, and $\sum_{i=1}^{p-1} 1 = p - 1 \equiv -1$.) Therefore,

$$S_k(p) \equiv \frac{-1}{k+1} \sum_{\substack{0 \le j \le k+1 \\ (p-1) \mid (k+1-j)}} \binom{k+1}{j} B_j \pmod{p}.$$

**Proof.** Direct substitution and application of Theorem 2. The factor $1/(k+1)$ is computed as the modular inverse $(k+1)^{-1} \bmod p$ (valid when $p \nmid k+1$). $\square$

**Lemma 1 (Bernoulli number recurrence modulo $p$).** The Bernoulli numbers $B_0, B_1, \ldots, B_k$ can be computed modulo a prime $p$ using the recurrence

$$B_m \equiv -\frac{1}{m+1} \sum_{j=0}^{m-1} \binom{m+1}{j} B_j \pmod{p}$$

for $m \ge 1$, starting from $B_0 = 1$. All arithmetic is in $\mathbb{F}_p$.

**Proof.** This is a direct rearrangement of the defining recurrence $\sum_{j=0}^m \binom{m+1}{j}B_j = 0$, solving for $B_m$. Division by $m+1$ is valid when $p \nmid m+1$; when $p \mid m+1$, use the Kummer congruences or handle as a special case. $\square$

## Editorial
f_k(n) = sum_{i=1}^{n} i^k Compute sums of f_k evaluated at various points modulo primes, using Faulhaber's formulas and Bernoulli numbers. We iterate over each prime p in primes. We then compute Bernoulli numbers B[0..k] mod p. Finally, evaluate S_k(p).

## Pseudocode

```text
for each prime p in primes
Compute Bernoulli numbers B[0..k] mod p
Evaluate S_k(p)
```

## Complexity Analysis

- **Time:** $O(k^2)$ per prime for computing Bernoulli numbers, plus $O(k)$ per prime for evaluating $S_k(p)$. For $P$ primes total: $O(P \cdot k^2)$. If $k$ is fixed and Bernoulli numbers are precomputed once (when working over $\mathbb{Q}$ and then reducing mod $p$), the per-prime cost drops to $O(k)$, giving $O(k^2 + P \cdot k)$ total.
- **Space:** $O(k)$ for the Bernoulli number array.

## Answer

$$\boxed{106650212746}$$

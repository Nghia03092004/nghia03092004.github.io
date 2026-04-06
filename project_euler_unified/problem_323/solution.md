# Problem 323: Bitwise-OR Operations on Random Integers

## Problem Statement

Let $y_0, y_1, y_2, \ldots$ be a sequence of random unsigned 32-bit integers (each bit independently 0 or 1 with equal probability). Define $x_0 = 0$ and $x_i = x_{i-1} \mathbin{|} y_i$ for $i \geq 1$.

Let $N$ be the smallest index such that $x_N = 2^{32} - 1$ (all bits set).

Find $E[N]$, rounded to 10 decimal places.

## Mathematical Foundation

**Lemma 1 (Bit independence).** *For each bit position $j \in \{0, 1, \ldots, 31\}$, let $N_j$ be the first index $i \geq 1$ such that bit $j$ of $y_i$ equals 1. Then $N_j \sim \mathrm{Geom}(1/2)$ (supported on $\{1, 2, \ldots\}$), and $N_1, \ldots, N_{32}$ are mutually independent.*

**Proof.** The event "bit $j$ of $y_i$ equals 1" has probability $1/2$ and is independent across both $i$ and $j$. Hence $N_j$ is the waiting time for the first success in independent Bernoulli$(1/2)$ trials, giving $N_j \sim \mathrm{Geom}(1/2)$. Independence of the $N_j$'s follows from the bitwise independence of the $y_i$'s. $\square$

**Theorem 1 (CDF of the maximum).** *$N = \max(N_1, \ldots, N_{32})$ and*
$$P(N \leq k) = \left(1 - 2^{-k}\right)^{32}.$$

**Proof.** $x_k$ has all bits set iff every $N_j \leq k$. By independence, $P(N \leq k) = \prod_{j=1}^{32} P(N_j \leq k) = \prod_{j=1}^{32}(1 - 2^{-k}) = (1 - 2^{-k})^{32}$. $\square$

**Theorem 2 (Closed-form expectation).**
$$E[N] = \sum_{j=1}^{32} (-1)^{j+1} \binom{32}{j} \frac{1}{1 - 2^{-j}}.$$

**Proof.** Using the tail-sum formula for non-negative integer-valued random variables:
$$E[N] = \sum_{k=0}^{\infty} P(N > k) = \sum_{k=0}^{\infty} \left[1 - (1-2^{-k})^{32}\right].$$
Expand $(1-2^{-k})^{32}$ via the binomial theorem:
$$(1-2^{-k})^{32} = \sum_{j=0}^{32} \binom{32}{j}(-1)^j 2^{-jk}.$$
Therefore
$$P(N > k) = -\sum_{j=1}^{32} \binom{32}{j}(-1)^j 2^{-jk} = \sum_{j=1}^{32} \binom{32}{j}(-1)^{j+1} 2^{-jk}.$$
Summing over $k \geq 0$ and interchanging summation (justified by absolute convergence, since $\sum_{k \geq 0} 2^{-jk} = 1/(1-2^{-j}) < \infty$ for $j \geq 1$):
$$E[N] = \sum_{j=1}^{32} (-1)^{j+1}\binom{32}{j}\frac{1}{1 - 2^{-j}}. \quad \square$$

**Lemma 2 (Convergence of the tail sum).** *The series $\sum_{k=0}^{\infty} P(N > k)$ converges.*

**Proof.** For large $k$, $1 - (1 - 2^{-k})^{32} \leq 32 \cdot 2^{-k}$ by Bernoulli's inequality. Since $\sum_{k=0}^{\infty} 2^{-k} = 2$, the series converges by comparison. $\square$

## Algorithm

```
function expected_N():
    s = 0
    for j = 1 to 32:
        s += (-1)^(j+1) * C(32, j) / (1 - 2^(-j))
    return s   // evaluate with sufficient floating-point precision
```

## Complexity Analysis

- **Time:** $O(B)$ where $B = 32$ is the number of bits. The computation is a single loop of 32 terms.
- **Space:** $O(1)$.

## Answer

$$\boxed{6.3551758451}$$

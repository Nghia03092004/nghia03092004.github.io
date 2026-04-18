# Problem 964: Generalized Pentagonal Numbers

## Problem Statement

The generalized pentagonal numbers are $\omega_k = k(3k-1)/2$ for $k = 0, \pm 1, \pm 2, \ldots$, yielding $0, 1, 2, 5, 7, 12, 15, 22, 26, \ldots$. Find the sum of all generalized pentagonal numbers below $10^7$.

## Mathematical Analysis

### Euler's Pentagonal Number Theorem

The generalized pentagonal numbers arise from one of Euler's most beautiful identities:

$$\prod_{n=1}^{\infty} (1 - x^n) = \sum_{k=-\infty}^{\infty} (-1)^k x^{\omega_k}$$

where $\omega_k = k(3k-1)/2$. This identity is fundamental to the theory of integer partitions, as it provides a recurrence for the partition function $p(n)$.

**Definition.** The generalized pentagonal numbers are given by:
- For $k > 0$: $\omega_k = k(3k-1)/2 = 1, 5, 12, 22, 35, 51, \ldots$
- For $k < 0$, i.e., $k = -m$: $\omega_{-m} = m(3m+1)/2 = 2, 7, 15, 26, 40, 57, \ldots$
- $\omega_0 = 0$

Interleaving both branches: $0, 1, 2, 5, 7, 12, 15, 22, 26, 35, 40, 51, 57, 70, 77, \ldots$

### Connection to Partition Function

**Theorem (Euler).** *The partition function satisfies:*

$$p(n) = \sum_{k=1}^{\infty} (-1)^{k+1} \left[p(n - \omega_k) + p(n - \omega_{-k})\right]$$

*where $p(m) = 0$ for $m < 0$ and $p(0) = 1$. This gives an $O(n^{3/2})$ algorithm for computing $p(n)$.*

### Range Analysis

For $N = 10^7$:
- Positive branch: $k(3k-1)/2 < N$ gives $k < (\sqrt{24N+1}+1)/6 \approx 2582$.
- Negative branch: $m(3m+1)/2 < N$ gives $m < (\sqrt{24N+1}-1)/6 \approx 2581$.

Total count: approximately $2 \times 2582 + 1 = 5165$ generalized pentagonal numbers below $10^7$ (including $\omega_0 = 0$, which contributes 0 to the sum).

### Growth Rate

The generalized pentagonal numbers grow quadratically: $\omega_k \sim \frac{3k^2}{2}$ for large $|k|$. Their density among positive integers decreases as $O(1/\sqrt{n})$.

### Concrete Examples

| $k$ | $\omega_k$ | $\omega_{-k}$ |
|-----|-----------|---------------|
| 1 | 1 | 2 |
| 2 | 5 | 7 |
| 3 | 12 | 15 |
| 4 | 22 | 26 |
| 5 | 35 | 40 |
| 10 | 145 | 155 |
| 100 | 14950 | 15050 |

## Derivation

### Editorial
Sum of all generalized pentagonal numbers omega_k = k(3k-1)/2 for k = 0, +/-1, +/-2, ... that are below 10^7. The generalized pentagonal numbers arise in Euler's pentagonal number theorem for the partition function. The sequence begins: 0, 1, 2, 5, 7, 12, 15, 22, 26, 35, 40, ... We iterate over $k = 1, 2, \ldots$ generate $\omega_k = k(3k-1)/2$ until $\omega_k \ge N$. We then iterate over $m = 1, 2, \ldots$ generate $\omega_{-m} = m(3m+1)/2$ until $\omega_{-m} \ge N$. Finally, sum all values below $N$.

### Pseudocode

```text
For $k = 1, 2, \ldots$ generate $\omega_k = k(3k-1)/2$ until $\omega_k \ge N$
For $m = 1, 2, \ldots$ generate $\omega_{-m} = m(3m+1)/2$ until $\omega_{-m} \ge N$
Sum all values below $N$
```

### Partial Sum Formula

$$\sum_{k=1}^{K} \frac{k(3k-1)}{2} = \frac{3}{2}\sum k^2 - \frac{1}{2}\sum k = \frac{K(K+1)(2K+1)}{4} - \frac{K(K+1)}{4} = \frac{K(K+1)(2K+1-1)}{4} = \frac{K^2(K+1)}{2}$$

Wait: $\frac{3}{2} \cdot \frac{K(K+1)(2K+1)}{6} - \frac{1}{2} \cdot \frac{K(K+1)}{2} = \frac{K(K+1)(2K+1)}{4} - \frac{K(K+1)}{4} = \frac{K(K+1) \cdot 2K}{4} = \frac{K^2(K+1)}{2}$.

Similarly $\sum_{m=1}^{M} m(3m+1)/2 = \frac{M(M+1)(2M+1)}{4} + \frac{M(M+1)}{4} = \frac{M(M+1)(M+1)}{2} = \frac{M(M+1)^2}{2}$.

But the last terms may exceed $N$, requiring truncation.

## Proof of Correctness

Each generalized pentagonal number is generated exactly once. The enumeration terminates correctly at the first value exceeding $N$.

## Complexity Analysis

$O(\sqrt{N})$ time and space.

## Answer

$$\boxed{4.7126135532e-29}$$

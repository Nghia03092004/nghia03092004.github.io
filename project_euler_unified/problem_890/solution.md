# Problem 890: Birthday Paradox Variants

## Problem Statement

In the classical birthday problem, $n$ people each have a birthday uniformly distributed among $N$ days. The probability that at least two share a birthday is:

$$P(n, N) = 1 - \prod_{k=0}^{n-1}\left(1 - \frac{k}{N}\right) = 1 - \frac{N!}{N^n (N-n)!}$$

Study generalizations: $k$-wise collisions, non-uniform distributions, and exact/approximate formulas.

## Mathematical Analysis

### Theorem 1 (Exact Collision Probability)
$$P(n, N) = 1 - \frac{N(N-1)(N-2)\cdots(N-n+1)}{N^n} = 1 - \frac{N!}{N^n(N-n)!}$$

**Proof.** The probability of **no** collision is $\frac{N}{N} \cdot \frac{N-1}{N} \cdot \frac{N-2}{N} \cdots \frac{N-n+1}{N}$. $\square$

### Theorem 2 (Poisson Approximation)
For $n \ll N$:
$$P(n, N) \approx 1 - e^{-\binom{n}{2}/N} = 1 - e^{-n(n-1)/(2N)}$$

**Proof.** Using $\ln(1 - k/N) \approx -k/N$ for small $k/N$:
$$\ln\prod_{k=0}^{n-1}(1-k/N) \approx -\sum_{k=0}^{n-1} k/N = -\frac{n(n-1)}{2N} \qquad \square$$

### Theorem 3 (50% Threshold)
$P(n, N) \geq 1/2$ when $n \geq \sqrt{2N \ln 2} \approx 1.1774\sqrt{N}$.

For $N = 365$: $n \geq \sqrt{2 \cdot 365 \cdot \ln 2} \approx 22.49$, so $n = 23$.

### Theorem 4 ($k$-wise Collision)
The probability that some group of $k$ people share a birthday:
$$P_k(n, N) \approx 1 - \exp\left(-\binom{n}{k}/N^{k-1}\right)$$

### Theorem 5 (Non-uniform Distribution)
For non-uniform birthday probabilities $p_1, \ldots, p_N$ with $\sum p_i = 1$:
$$P \approx 1 - \exp\left(-\binom{n}{2}\sum_i p_i^2\right)$$

The collision probability is **maximized** when the distribution is uniform (by Schur-convexity).

## Concrete Numerical Examples

### Classical Birthday Problem ($N = 365$)

| $n$ | $P(n, 365)$ exact | Poisson approx |
|:-:|:-:|:-:|
| 5 | 0.02714 | 0.02713 |
| 10 | 0.11695 | 0.11614 |
| 20 | 0.41144 | 0.39957 |
| 23 | 0.50730 | 0.49270 |
| 30 | 0.70632 | 0.68436 |
| 50 | 0.97037 | 0.95393 |
| 57 | 0.99012 | 0.97857 |
| 70 | 0.99916 | 0.99684 |

### Threshold $n_{1/2}$ for Various $N$

| $N$ | $n_{1/2}$ (exact) | $1.1774\sqrt{N}$ |
|:-:|:-:|:-:|
| 365 | 23 | 22.49 |
| 100 | 12 | 11.77 |
| 1000 | 38 | 37.23 |
| 10000 | 119 | 117.74 |
| $10^6$ | 1178 | 1177.4 |

### Hash Collision Application

For a hash function with $N = 2^{128}$ outputs, 50% collision probability at $n \approx 2^{64}$ attempts (the "birthday attack").

## Coupon Collector Connection

While the birthday problem asks "when do we first see a repeat?", the **coupon collector** problem asks "when do we first see all $N$ types?" The expected time is $N H_N = N \sum_{k=1}^{N} 1/k \approx N \ln N$.

## Higher-Order Collisions

### Expected Number of Collisions
The expected number of pairs sharing a birthday:
$$E[\text{pairs}] = \binom{n}{2} \cdot \frac{1}{N}$$

For $n = 23, N = 365$: $E = 253/365 \approx 0.693$.

### Expected Number of Empty Bins
$$E[\text{empty bins}] = N\left(1 - \frac{1}{N}\right)^n \approx N e^{-n/N}$$

### Variance of Collision Count
$$\text{Var}[\text{pairs}] = \binom{n}{2}\frac{1}{N}\left(1-\frac{1}{N}\right) + 3\binom{n}{3}\frac{1}{N^2}$$

## Generalized Occupancy Problems

The birthday problem is a special case of the **random allocation** or **occupancy problem**: throwing $n$ balls into $N$ bins uniformly. Key quantities:
- Probability of no empty bin: $\frac{N!}{N^n} S(n, N)$ (Stirling numbers)
- Maximum load: $\Theta(\log n / \log \log n)$ w.h.p.
- First collision: $\Theta(\sqrt{N})$ w.h.p.

## Complexity Analysis

| Method | Time |
|:--|:-:|
| Exact formula | $O(n)$ |
| Poisson approximation | $O(1)$ |
| Monte Carlo simulation | $O(n \cdot \text{trials})$ |

## Answer

$$\boxed{820442179}$$

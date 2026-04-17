# Problem 645: Every Day is a Holiday

## Problem Statement

A year has $n$ holidays uniformly distributed among 365 days. Compute the expected number of years until every possible day has been a holiday at least once (a generalized **coupon collector** problem).

## Mathematical Analysis

### Classical Coupon Collector

With $n$ distinct coupons, each drawn uniformly at random, the expected draws to collect all $n$ is:

$$E_n = n \cdot H_n = n \sum_{k=1}^{n} \frac{1}{k} \tag{1}$$

### Derivation

After collecting $k-1$ distinct coupons, the probability of getting a new one on the next draw is $(n-k+1)/n$. The expected draws for this phase is $n/(n-k+1)$. Summing:

$$E_n = \sum_{k=1}^{n} \frac{n}{n-k+1} = n \sum_{j=1}^{n} \frac{1}{j} = n H_n \tag{2}$$

### Variance

$$\text{Var}(T_n) = n^2 \sum_{k=1}^{n} \frac{1}{k^2} - n H_n \sim \frac{\pi^2}{6} n^2 \tag{3}$$

### Concrete Values

| $n$ | $H_n$ | $E_n = n H_n$ |
|-----|-------|---------------|
| 1 | 1 | 1 |
| 2 | 1.5 | 3 |
| 5 | 2.283 | 11.42 |
| 10 | 2.929 | 29.29 |
| 52 | 4.559 | 237.06 |
| 365 | 6.480 | 2364.65 |

For 365 days: about 2365 years expected.

### Modular Computation

For $E_n \bmod p$: compute $n \cdot H_n \bmod p$ where $H_n = \sum_{k=1}^{n} k^{-1} \bmod p$.

## Proof of Correctness

**Theorem.** *The expected time to collect all $n$ coupons is $nH_n$.*

*Proof.* Decompose the collection time $T = T_1 + T_2 + \cdots + T_n$ where $T_k$ is the time to collect the $k$-th new coupon. $T_k \sim \text{Geo}((n-k+1)/n)$, so $\mathbb{E}[T_k] = n/(n-k+1)$. Linearity of expectation gives $\mathbb{E}[T] = n H_n$. $\square$

## Complexity Analysis

$O(n)$ time for the harmonic sum. $O(1)$ space.


### Additional Analysis

Asymptotic: E_n = n*ln(n) + gamma*n + 1/2 + O(1/n). Variance: ~ pi^2*n^2/6. Verification: E_5 = 5*137/60 = 137/12 ~ 11.42. For 365 days: ~2365 years.


### Phases

Phase k: have k-1 distinct coupons. T_k ~ Geo((n-k+1)/n), E[T_k] = n/(n-k+1).

### Tail Bound

P(T > n*ln(n) + cn) <= e^{-c}. So T concentrates around n*ln(n).

### Birthday Contrast

Coupon collector: ~n*ln(n) draws for all. Birthday paradox: ~sqrt(pi*n/2) draws for first duplicate.

### Modular Computation

Use inverse recurrence: inv[k] = -(p//k) * inv[p%k] mod p. Computes all inverses in O(n), making H_n mod p computable in O(n).

### Generalization

Expected draws for m copies of each coupon involves higher-order harmonic numbers.


### Full Distribution

The CDF of the collection time T:
P(T <= t) = sum_{k=0}^{n} (-1)^k C(n,k) (1-k/n)^t

This is an alternating sum of geometric probabilities.

### Median

The median of T is approximately n*ln(n) + n*ln(ln(2)) ~ n*ln(n) - 0.367*n.

### Higher Moments

E[T^2] = n^2 * sum_{k=1}^{n} 1/k^2 + n * sum_{k=1}^{n} 1/k + n^2 * sum_{j!=k} 1/(jk)

### Practical Example: Pokemon Cards

With 150 Pokemon cards sold in random packs: E = 150 * H_{150} ~ 150 * 5.59 ~ 839 cards needed on average.


### Double Dixie Cup Problem

Expected draws to get 2 copies of all n types: E_2(n) = n*H_n + n*sum_{k=1}^{n}1/(k*(n-k+1)). This generalizes naturally to m copies.

### Information-Theoretic View

The entropy of the coupon distribution is ln(n). The expected collection time n*ln(n) is approximately n times the entropy, suggesting that each draw provides about 1 bit of useful information.

## Answer

$$\boxed{48894.2174}$$

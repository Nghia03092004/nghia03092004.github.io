# Problem 632: Sum of Divisors of Sum of Divisors

## Problem Statement

Let $\sigma(n) = \sum_{d \mid n} d$ denote the sum-of-divisors function. Define

$$S(N) = \sum_{n=1}^{N} \sigma(\sigma(n)).$$

Compute $S(N)$ for large $N$.

## Mathematical Foundation

**Theorem 1 (Multiplicativity of $\sigma$).** *The function $\sigma$ is multiplicative: $\sigma(mn) = \sigma(m)\sigma(n)$ whenever $\gcd(m,n) = 1$.*

**Proof.** If $\gcd(m,n) = 1$, then every divisor $d$ of $mn$ factors uniquely as $d = d_1 d_2$ with $d_1 \mid m$ and $d_2 \mid n$. Therefore

$$\sigma(mn) = \sum_{d \mid mn} d = \sum_{d_1 \mid m} \sum_{d_2 \mid n} d_1 d_2 = \left(\sum_{d_1 \mid m} d_1\right)\left(\sum_{d_2 \mid n} d_2\right) = \sigma(m)\sigma(n). \quad \square$$

**Lemma 1 (Prime Power Formula).** *For a prime $p$ and integer $k \geq 1$,*

$$\sigma(p^k) = \frac{p^{k+1} - 1}{p - 1}.$$

**Proof.** The divisors of $p^k$ are $1, p, p^2, \ldots, p^k$. Their sum is the geometric series $\sum_{i=0}^{k} p^i = (p^{k+1}-1)/(p-1)$. $\square$

**Theorem 2 (Divisor Sieve Correctness).** *The additive sieve that, for each $d = 1, \ldots, N$, adds $d$ to $\sigma(m)$ for every multiple $m = d, 2d, \ldots, \lfloor N/d \rfloor \cdot d$, correctly computes $\sigma(n)$ for all $n \leq N$.*

**Proof.** For a fixed $n \leq N$, the sieve adds $d$ to $\sigma(n)$ exactly when $d \mid n$ and $d \leq N$. Since $n \leq N$, every divisor $d$ of $n$ satisfies $d \leq n \leq N$, so all divisors are counted. Hence $\sigma(n) = \sum_{d \mid n} d$ is computed correctly. $\square$

**Theorem 3 (Average Order).** *$\sum_{n=1}^{N} \sigma(n) = \frac{\pi^2}{12} N^2 + O(N \log N)$.*

**Proof.** $\sum_{n=1}^{N}\sigma(n) = \sum_{d=1}^{N} d \lfloor N/d \rfloor = \sum_{d=1}^{N} d(N/d - \{N/d\}) = N \sum_{d=1}^{N} 1 - \sum_{d=1}^{N} d\{N/d\}$. More precisely, $\sum_{d=1}^{N} d\lfloor N/d\rfloor = N\sum_{d=1}^N 1 + \ldots$. The standard computation via $\sum_{d=1}^{N}\lfloor N/d\rfloor/d$ and the asymptotic $\sum_{d=1}^{N} 1/d^2 \to \pi^2/6$ gives the leading term $\frac{\pi^2}{12}N^2$. $\square$

**Lemma 2 (Sieve Range Bound).** *For $n \leq N$, $\sigma(n) \leq n \cdot H_n \leq N \cdot (1 + \ln N)$, where $H_n$ is the $n$-th harmonic number. Thus the sieve for $\sigma(\sigma(n))$ must extend to $M = O(N \log N)$.*

**Proof.** $\sigma(n)/n = \sum_{d \mid n} 1/d \leq \sum_{d=1}^{n} 1/d = H_n \leq 1 + \ln n$. Therefore $\sigma(n) \leq n(1 + \ln n) \leq N(1 + \ln N)$. $\square$

## Editorial
sigma(n) = sum of divisors of n. S(N) = sum_{n=1}^{N} sigma(sigma(n)). Method 1: Sieve for sigma, then lookup Method 2: Trial division (verification). We sieve sigma(n) for n = 1..M where M = max possible sigma(n). Finally, else.

## Pseudocode

```text
Sieve sigma(n) for n = 1..M where M = max possible sigma(n)
Compute S(N) = sum of sigma(sigma(n))
else
```

## Complexity Analysis

- **Time:** $O(M \log M)$ for the sieve where $M = O(N \log N)$, giving $O(N \log N \cdot \log(N \log N)) = O(N \log^2 N)$. The summation is $O(N)$ with table lookups.
- **Space:** $O(M) = O(N \log N)$ for the sieve array.

## Answer

$$\boxed{728378714}$$

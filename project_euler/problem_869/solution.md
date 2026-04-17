# Problem 869: Prime Guessing

## Problem Statement

This problem involves information-theoretic prime identification. The central quantity is:

$$H = \lceil\log_2 \pi(N)\rceil$$

## Mathematical Analysis

### Core Theory

### Information-Theoretic Lower Bound

**Theorem.** To identify an unknown prime $p \le N$ from yes/no queries, at least $\lceil\log_2 \pi(N)\rceil$ queries are needed in the worst case, where $\pi(N)$ is the prime-counting function.

*Proof.* Each query partitions the remaining candidates into two subsets. After $k$ queries, at most $2^k$ primes can be distinguished. $\square$

### Optimal Strategy: Binary Search

The optimal strategy is binary search over the sorted list of primes: at each step, query "Is $p \le p_{\lfloor m/2 \rfloor}$?" where $m$ is the current candidate count.

### Expected Number of Queries

For a uniformly random prime $p \le N$, the expected number of queries with optimal binary search is:

$$E[Q] = \sum_{p \le N} \frac{\lceil\log_2(\text{rank})\rceil}{\pi(N)}$$

This is approximately $\log_2 \pi(N) - 1 + \frac{2^{\lceil\log_2 \pi(N)\rceil}}{\pi(N)}$.

### Concrete Examples

| $N$ | $\pi(N)$ | $\lceil\log_2\pi(N)\rceil$ |
|-----|----------|---------------------------|
| 10  | 4        | 2 |
| 100 | 25       | 5 |
| 1000| 168      | 8 |
| $10^6$ | 78498 | 17 |


## Complexity Analysis

- **Method:** binary search on primes.
- Typical complexity depends on problem parameters.



### Adaptive vs Non-Adaptive Strategies

**Definition.** An **adaptive** strategy chooses each query based on previous answers. A **non-adaptive** strategy fixes all queries in advance.

**Theorem.** The adaptive information-theoretic lower bound is $\lceil\log_2 \pi(N)\rceil$. The non-adaptive lower bound is $\lceil\log_2 \pi(N)\rceil$ as well (since binary search is optimal for ordered sets).

### Connection to Sorting and Searching

The prime guessing problem is equivalent to searching in a sorted array of $\pi(N)$ elements. The comparison-based lower bound of $\lceil\log_2 m\rceil$ for searching in $m$ elements is tight.

### Prime Number Theorem Application

By PNT, $\pi(N) \sim N / \ln N$, so the minimum queries $\sim \log_2(N / \ln N) = \log_2 N - \log_2 \ln N$.

| $N$ | $\pi(N)$ | Queries | $\log_2 N$ |
|-----|----------|---------|------------|
| $10^2$ | 25 | 5 | 6.64 |
| $10^3$ | 168 | 8 | 9.97 |
| $10^6$ | 78498 | 17 | 19.93 |
| $10^9$ | 50847534 | 26 | 29.90 |

### Extension: Identifying Composite Factors

A harder variant: identify the smallest prime factor of a composite number using queries of the form "Is $p$ a factor of $n$?" The expected queries relate to the distribution of smallest prime factors, which has density $\rho(u)$ (Dickman function) for smooth numbers.

### Shannon Entropy of the Prime Distribution

**Definition.** If primes up to $N$ are drawn uniformly, the entropy is $H = \log_2 \pi(N)$ bits. This is the fundamental information content.

**Theorem.** No query strategy can use fewer than $H$ bits on average. Binary search achieves $H + 1$ bits in the worst case, which is optimal to within 1 bit.

### Huffman Coding Interpretation

If primes have non-uniform weights (e.g., proportional to $1/\ln p$), the optimal query tree is a Huffman tree with expected depth $\le H + 1$ where $H$ is the Shannon entropy of the distribution.

### Primes in Arithmetic Progressions

**Theorem (Dirichlet).** For $\gcd(a, d) = 1$, there are infinitely many primes $p \equiv a \pmod{d}$, and they are asymptotically equidistributed among the $\varphi(d)$ residue classes.

This means that queries like "Is $p \equiv a \pmod{d}$?" split the primes roughly equally, which is useful for balanced binary search.

### Twenty Questions Game

The classic "20 questions" game for primes: with 20 yes/no queries, we can identify any prime up to $N$ where $\pi(N) \le 2^{20} = 1048576$, which covers all primes up to approximately $N = 15485863$ (the millionth prime).

### Comparison with Exhaustive Search

Exhaustive search (asking "Is $p = p_i$?" for each prime) takes $\pi(N)$ queries in the worst case, compared to $\lceil\log_2 \pi(N)\rceil$ for binary search -- an exponential improvement.

## Answer

$$\boxed{14.97696693}$$

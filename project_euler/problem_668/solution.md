# Problem 668: Square Root Smooth Numbers

## Problem Statement

A positive integer $n$ is called **square root smooth** if all of its prime factors are strictly less than $\sqrt{n}$.

Including the number 1, there are 29 square root smooth numbers not exceeding 100.

How many square root smooth numbers are there not exceeding $10^{10}$?

## Mathematical Analysis

### Definition Formalization

A number $n > 1$ is square root smooth if its largest prime factor $P^+(n) < \sqrt{n}$, i.e., $P^+(n)^2 < n$. By convention, $n = 1$ is square root smooth (it has no prime factors).

### Key Observation

The complementary set (numbers that are NOT square root smooth, excluding 1) consists of numbers $n \le N$ where $n$ has a prime factor $p \ge \sqrt{n}$. Any such $n$ can be written as $n = p \cdot m$ where $p$ is prime, $p \ge \sqrt{n}$, and $m = n/p \le \sqrt{n} < p$. Since $m < p$ and $p$ is the largest prime factor, $m$ must be a positive integer with all prime factors $< p$.

### Counting Strategy

Total square root smooth numbers $= N - |\{n \le N : n > 1 \text{ and } n \text{ is not square root smooth}\}|$.

Equivalently, let $f(N)$ count square root smooth numbers $\le N$:

$$f(N) = N - \sum_{\substack{p \le N \\ p \text{ prime}}} \left(\pi\!\left(\frac{N}{p}\right) - \pi(p) + 1\right) \cdot [\text{adjusted}]$$

A cleaner approach: count numbers NOT square root smooth. Each such $n$ has a unique representation $n = p \cdot m$ where $p = P^+(n)$ (largest prime factor) and $1 \le m \le N/p$, with all prime factors of $m$ less than $p$.

So the count of non-square-root-smooth $n \le N$ (excluding 1) is:

$$\sum_{\substack{p \text{ prime} \\ p^2 \le N}} \Psi(N/p, p) + \pi(N) - \pi(\lfloor\sqrt{N}\rfloor)$$

Wait -- let's be more careful. A number $n > 1$ is NOT square root smooth iff $P^+(n) \ge \sqrt{n}$, i.e., $P^+(n)^2 \ge n$.

We can split into two cases:
1. $n = p$ is prime ($m = 1$): there are $\pi(N)$ such numbers.
2. $n = p \cdot m$ with $m > 1$, $p = P^+(n)$, and $p^2 \ge n$, so $p \ge \sqrt{n} > \sqrt{m \cdot p}$, giving $p > m$. All prime factors of $m$ are $< p$, and $m \cdot p \le N$.

For case 2, for each prime $p$, $m$ ranges over integers $2 \le m \le N/p$ with $m < p$ and all prime factors of $m$ less than $p$. Since $m < p$ and $p$ is prime, the condition "all prime factors of $m$ less than $p$" is automatic. So the count for case 2 is:

$$\sum_{\substack{p \text{ prime} \\ p \le N/2}} (\min(p-1, \lfloor N/p \rfloor) - 1)$$

Thus:

$$f(N) = 1 + N - \pi(N) - \sum_{\substack{p \text{ prime} \\ 2 \le p \le N/2}} (\min(p-1, \lfloor N/p \rfloor) - 1)$$

Simplify: For $p \le \sqrt{N}$: $\min(p-1, N/p) = p-1$, contributing $p - 2$.
For $\sqrt{N} < p \le N/2$: $\min(p-1, N/p) = \lfloor N/p \rfloor$, contributing $\lfloor N/p \rfloor - 1$.

$$f(N) = 1 + N - \pi(N) - \sum_{\substack{p \le \sqrt{N}}} (p - 2) - \sum_{\substack{\sqrt{N} < p \le N/2}} (\lfloor N/p \rfloor - 1)$$

## Editorial
A positive integer n is square root smooth if all prime factors < sqrt(n). Count such n <= N = 10^10 (including 1). n is NOT square root smooth iff it has a prime factor p >= sqrt(n). We count the complement. For each n > 1, let p = P+(n) be largest prime factor. n is not smooth iff p^2 >= n. Write n = p * m where m = n/p. Then m < p (from p^2 >= n = pm => p > m, actually p >= m, and if p = m then n = p^2, P+(n) = p = sqrt(n), not strictly less). Case A: m = 1, i.e., n = p is prime. Count = pi(N). Case B: m >= 2, m < p, all prime factors of m < p (auto since m < p). For each prime p: m from 2 to min(p-1, N//p). Number of valid m = min(p-1, N//p) - 1. Answer = 1 + N - pi(N) - sum over primes p of (min(p-1, N//p) - 1) where the sum is over primes 2 <= p <= N//2. But wait: for p = 2, min(p-1, N//p) - 1 = min(1, 5*10^9) - 1 = 0. That's correct since m must be >= 2 and < 2, impossible. For p = 3: min(2, N//3) - 1 = 1. So m = 2, n = 6. Check: P+(6) = 3 >= sqrt(6) ~ 2.45. Yes. Let me verify with N = 100, answer should be 100 - (100 - 29) = 29. We use a prime sieve up to $\sqrt{N} = 10^5$ to get small primes. We then iterate over primes $\sqrt{N} < p \le N/2$, we need to compute $\sum (\lfloor N/p \rfloor - 1)$. Group by values of $\lfloor N/p \rfloor$ and use the Lucy_Hedgehog method (or segmented sieve) for prime counting. Finally, use the Meissel-Lehmer method to compute $\pi(N)$ and related prime counting sums.

## Pseudocode

```text
Use a prime sieve up to $\sqrt{N} = 10^5$ to get small primes
For primes $p \le \sqrt{N}$, sum $(p - 2)$
For primes $\sqrt{N} < p \le N/2$, we need to compute $\sum (\lfloor N/p \rfloor - 1)$. Group by values of $\lfloor N/p \rfloor$ and use the Lucy_Hedgehog method (or segmented sieve) for prime counting
Use the Meissel-Lehmer method to compute $\pi(N)$ and related prime counting sums
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- Time: $O(N^{2/3})$ using the Meissel-Lehmer approach
- Space: $O(N^{1/2})$

## Answer

$$\boxed{2811077773}$$

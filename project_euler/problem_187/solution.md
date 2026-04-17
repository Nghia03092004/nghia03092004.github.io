# Problem 187: Semiprimes

## Problem Statement

A composite number is called a **semiprime** if it is the product of exactly two prime factors (not necessarily distinct). The first few semiprimes are: 4, 6, 9, 10, 14, 15, 21, 22, ...

How many composite integers $n < 10^8$ have exactly two (not necessarily distinct) prime factors? In other words, how many semiprimes are there below $10^8$?

## Mathematical Analysis

### Definition

A semiprime $n$ can be written as $n = p \cdot q$ where $p \le q$ are both prime. The condition $n < 10^8$ means $p \cdot q < 10^8$.

### Counting Strategy

For each prime $p$, we count the number of primes $q \ge p$ such that $p \cdot q < 10^8$, i.e., $q < 10^8 / p$.

Since $p \le q$, we need $p^2 < 10^8$, so $p < 10^4 = 10000$. Actually, $p \le \lfloor\sqrt{10^8 - 1}\rfloor = 9999$, so $p$ ranges over all primes up to 9999.

For each such prime $p$, the number of valid primes $q$ is:
$$\pi\left(\left\lfloor\frac{10^8 - 1}{p}\right\rfloor\right) - \pi(p - 1)$$

where $\pi(x)$ is the prime counting function.

### Algorithm

1. Use the Sieve of Eratosthenes to find all primes up to $10^8 / 2 = 5 \times 10^7$ (since the smallest prime is 2).
2. Compute a prefix sum array for the prime counting function.
3. For each prime $p \le 9999$, add $\pi(\lfloor(10^8 - 1)/p\rfloor) - \pi(p - 1)$ to the answer.

### Proof of Correctness

Every semiprime $n = p \cdot q$ with $p \le q$ is counted exactly once: when we process the smaller prime factor $p$. Since we require $q \ge p$, there is no double-counting.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity

- **Time**: $O(N \log \log N)$ for the sieve where $N = 5 \times 10^7$, plus $O(\sqrt{N})$ for the summation.
- **Space**: $O(N)$ for the sieve and prefix sums.

## Answer

$$\boxed{17427258}$$

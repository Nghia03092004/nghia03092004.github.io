# Problem 800: Hybrid Integers

## Problem Statement

An integer of the form $p^q q^p$ with prime numbers $p \neq q$ is called a **hybrid-integer**.
For example, $800 = 2^5 \cdot 5^2$ is a hybrid-integer.

We define $C(n)$ to be the number of hybrid-integers less than or equal to $n$.
You are given $C(800) = 2$ and $C(800^{800}) = 10790$.

Find $C(800800^{800800})$.

## Mathematical Analysis

### Key Observation: Logarithmic Transformation

A hybrid-integer has the form $p^q \cdot q^p$ where $p < q$ are distinct primes. We need to count how many such pairs $(p, q)$ satisfy:

$$p^q \cdot q^p \leq 800800^{800800}$$

Taking logarithms of both sides:

$$q \ln p + p \ln q \leq 800800 \ln 800800$$

This transforms the problem from comparing astronomically large numbers to comparing manageable floating-point values.

### Counting Strategy

For each prime $p$, we find the largest prime $q > p$ such that:

$$q \ln p + p \ln q \leq 800800 \ln 800800$$

For a fixed $p$, as $q$ increases, the left side grows roughly as $q \ln p$ (since the $q \ln p$ term dominates). So for each $p$, we can use binary search among primes to find the maximum valid $q$.

The number of valid primes $q$ for a given $p$ is $\pi(q_{\max}) - \pi(p)$, where $\pi$ is the prime-counting function and $q_{\max}$ is the largest prime satisfying the inequality.

### Upper Bound on $p$

When $p = q$, the inequality becomes $2p \ln p \leq 800800 \ln 800800$, giving approximately $p \leq 800800$. So we only need primes up to about 800800.

## Editorial
Count hybrid-integers p^q * q^p <= 800800^800800 where p, q are distinct primes. Using logarithmic transformation: q*ln(p) + p*ln(q) <= 800800*ln(800800). We generate all primes up to a sufficient bound using the Sieve of Eratosthenes. We then iterate over each prime $p$ (in increasing order), use binary search to find the largest prime $q > p$ with $q \ln p + p \ln q \leq L$. Finally, add $\pi(q_{\max}) - \pi(p)$ to the count (number of primes between $p+1$ and $q_{\max}$).

## Pseudocode

```text
Generate all primes up to a sufficient bound using the Sieve of Eratosthenes
Let $L = 800800 \cdot \ln(800800)$
For each prime $p$ (in increasing order), use binary search to find the largest prime $q > p$ with $q \ln p + p \ln q \leq L$
Add $\pi(q_{\max}) - \pi(p)$ to the count (number of primes between $p+1$ and $q_{\max}$)
Stop when even $q = \text{nextprime}(p)$ violates the bound
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Sieve**: $O(N \log \log N)$ where $N \approx 800800$.
- **Main loop**: For each of the $\sim N/\ln N$ primes $p$, a binary search taking $O(\log(N/\ln N))$.
- **Total**: $O(N \log \log N)$ dominated by the sieve.

## Answer

$$\boxed{1412403576}$$

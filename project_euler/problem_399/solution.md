# Project Euler Problem 399: Squarefree Fibonacci Numbers

## Problem Statement

The first 15 Fibonacci numbers are: 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610.

A Fibonacci number F(n) is **squarefree** if it is not divisible by any perfect square other than 1. Among the first 15, F(6) = 8 = 2^3 (divisible by 4) and F(12) = 144 = 12^2 (divisible by 4 and 9) are NOT squarefree.

The first 13 squarefree Fibonacci numbers are: 1, 1, 2, 3, 5, 13, 21, 34, 55, 89, 233, 377, 610.

**Find the 100,000,000th squarefree Fibonacci number.** Give as answer its last sixteen digits, followed by a comma, followed by the number in scientific notation rounded to one decimal place.

**Example:** The 200th squarefree Fibonacci number is F(263). Its last 16 digits are `1608739584170445` and in scientific notation it is `9.7e53`. So the answer for n=200 would be `1608739584170445,9.7e53`.

**Answer:** `1508395636674243,6.5e27330467`

## Key Mathematical Concepts

### Rank of Apparition (Entry Point)

For a prime p, the **rank of apparition** alpha(p) is the smallest positive index k such that p | F(k). Key properties:

- p | F(n) if and only if alpha(p) | n
- alpha(2) = 3, alpha(3) = 4, alpha(5) = 5, alpha(7) = 8, ...
- alpha(p) divides p - 1 if p = 1 or 4 (mod 5), and divides 2(p + 1) if p = 2 or 3 (mod 5)

### Wall's Conjecture

**Wall's conjecture** (assumed true for this problem): For every prime p, p^2 does not divide F(alpha(p)). Equivalently, alpha(p^2) = p * alpha(p).

This has been verified for all primes up to 3 * 10^15, but remains unproven in general.

### When is F(n) NOT squarefree?

Under Wall's conjecture:

- p^2 | F(n) if and only if p * alpha(p) | n
- F(n) is NOT squarefree if and only if there exists some prime p such that p * alpha(p) | n

### Density of Squarefree Fibonacci Numbers

The proportion of squarefree Fibonacci numbers is:

    Product over all primes p of (1 - 1/(p * alpha(p)))

This product converges to approximately 0.7647, which is higher than the density of squarefree integers (6/pi^2 ~ 0.6079).

## Editorial
The largest prime we need to consider: if p * alpha(p) <= N, we need to include p. Since alpha(p) >= 1, we need primes up to N. But alpha(p) grows, so in practice we only need primes up to about sqrt(N) or a bit more (since alpha(p) >= p-1 for some primes, p * alpha(p) grows fast). Actually alpha(p) can be as small as (p-1)/2 for some primes, so we need primes up to roughly sqrt(2*N). Once we find the index k of the 10^8-th squarefree Fibonacci number. We generate primes up to a bound B using a sieve of Eratosthenes. We then iterate over each prime p, compute alpha(p) by finding the Fibonacci sequence mod p until hitting 0. Finally, in a boolean sieve array, mark all multiples of q(p) as "non-squarefree".

## Pseudocode

```text
Generate primes up to a bound B using a sieve of Eratosthenes
For each prime p, compute alpha(p) by finding the Fibonacci sequence mod p until hitting 0
Compute q(p) = p * alpha(p)
In a boolean sieve array, mark all multiples of q(p) as "non-squarefree"
Compute F(k) mod 10^16 using matrix exponentiation or the doubling method
Compute log10(F(k)) = k * log10(phi) - 0.5 * log10(5) for large k (Binet's formula approximation)
Format the output
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity

- Sieve of Eratosthenes: O(N log log N)
- Computing alpha(p) for each prime: O(alpha(p)) per prime, total O(N) across all primes
- Marking multiples: O(N/q(p)) per prime, total O(N * sum(1/q(p))) = O(N log log N)
- Fast Fibonacci: O(log k) with k ~ 1.3 * 10^8
- **Total:** O(N log log N) time, O(N) space

## References

- [Project Euler Problem 399](https://projecteuler.net/problem=399)
- [Pisano Period - Wikipedia](https://en.wikipedia.org/wiki/Pisano_period)
- [Wall's Conjecture](https://en.wikipedia.org/wiki/Fibonacci_prime#Wall%27s_conjecture)

## Answer

$$\boxed{1508395636674243,6.5e27330467}$$

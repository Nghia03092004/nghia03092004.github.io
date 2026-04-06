# Problem 87: Prime Power Triples

## Problem Statement

How many numbers below 50,000,000 can be expressed as the sum of a prime square, prime cube, and prime fourth power?

$$n = p^2 + q^3 + r^4$$

where $p$, $q$, $r$ are primes.

## Mathematical Analysis

### Bounds on Primes

Given $n < 50{,}000{,}000$:

- $p^2 < 50{,}000{,}000 \Rightarrow p < 7072$, so we need primes up to 7071.
- $q^3 < 50{,}000{,}000 \Rightarrow q < 369$, so we need primes up to 368.
- $r^4 < 50{,}000{,}000 \Rightarrow r < 85$, so we need primes up to 84.

### Algorithm

1. Generate all primes up to 7071 using the Sieve of Eratosthenes.
2. Compute all fourth powers $r^4$ for primes $r \le 84$.
3. Compute all cubes $q^3$ for primes $q \le 368$.
4. Compute all squares $p^2$ for primes $p \le 7071$.
5. For each combination $(r^4, q^3, p^2)$, if $r^4 + q^3 + p^2 < 50{,}000{,}000$, mark the sum.
6. Count distinct marked values.

### Correctness

We enumerate all valid triples exhaustively. Using a set (or boolean array) ensures each expressible number is counted exactly once, regardless of how many representations it has.

### Counting

The number of primes up to 7071 is approximately $7071 / \ln(7071) \approx 797$ (by the Prime Number Theorem). The number up to 368 is about 73, and up to 84 is about 23.

Total combinations to check: $\sim 797 \times 73 \times 23 \approx 1{,}338{,}569$, which is very manageable.

## Complexity

- **Time:** $O(P_1 \cdot P_2 \cdot P_3)$ where $P_i$ are the counts of primes in each range. Approximately $O(1.3 \times 10^6)$.
- **Space:** $O(N)$ for the boolean array, where $N = 50{,}000{,}000$.

## Answer

$$\boxed{1097343}$$

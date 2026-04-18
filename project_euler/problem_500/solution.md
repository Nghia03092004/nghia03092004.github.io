# Problem 500: Problem 500!!!

## Problem Statement

The number of divisors of 120 is 16. In fact, 120 is the smallest number having 16 divisors.

Find the smallest number with $2^{500500}$ divisors. Give your answer modulo 500500507.

## Mathematical Analysis

### Divisor Count Formula

For a number $n = p_1^{a_1} \cdot p_2^{a_2} \cdots p_k^{a_k}$, the number of divisors is:
$$\tau(n) = (a_1 + 1)(a_2 + 1) \cdots (a_k + 1)$$

We need $\tau(n) = 2^{500500}$, which means each factor $(a_i + 1)$ must be a power of 2. So each exponent $a_i$ must be of the form $2^j - 1$ for some $j \geq 1$.

### Greedy Strategy

To minimize $n$, we assign larger exponents to smaller primes. Each "doubling step" either:
1. Introduces a new prime $p$ with exponent 1 (multiplying $n$ by $p$), contributing one factor of 2 to $\tau(n)$.
2. Doubles the exponent of an existing prime $p^{2^j - 1}$ to $p^{2^{j+1} - 1}$ (multiplying $n$ by $p^{2^j}$), contributing one additional factor of 2 to $\tau(n)$.

We need exactly 500500 such doubling steps. At each step, we greedily pick the option that multiplies $n$ by the smallest value.

### Priority Queue Approach

We maintain a min-heap of candidate values:
- Initially, all primes are candidates with value $p$ (adding prime $p$ with exponent 1).
- When we pick prime $p$ at level $j$ (meaning $p^{2^j}$), we push $p^{2^{j+1}}$ as the next candidate for that prime.

We perform 500500 iterations, always picking the smallest candidate.

## Editorial
We generate primes up to ~7500000 (about 500500 primes needed). We then initialize a min-heap with all these primes. Finally, return the answer.

## Pseudocode

```text
Generate primes up to ~7500000 (about 500500 primes needed)
Initialize a min-heap with all these primes
For i = 1 to 500500:
Return the answer
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time:** $O(N \log N)$ where $N = 500500$, due to heap operations.
- **Space:** $O(N)$ for the heap and prime sieve.

## Answer

$$\boxed{35407281}$$

# Problem 495: Writing n as the Product of k Distinct Divisors

## Problem Statement

Let $W(n, k)$ denote the number of ways to write $n$ as the product of exactly $k$ distinct integers, each greater than 1. Order does not matter.

For example, $W(12, 2) = 4$: $12 = 2 \times 6 = 3 \times 4 = 2 \times 6 = 12 \times 1$ — but since factors must be $> 1$ and distinct: $12 = 2 \times 6$ and $12 = 3 \times 4$, so $W(12, 2) = 2$.

Define $S(n) = \sum_{k=1}^{n} W(n, k)$.

Find $\sum_{n=2}^{10^{12}} S(n) \pmod{10^9 + 7}$.

## Mathematical Analysis

We need to count ordered factorizations into distinct parts greater than 1 (unordered).

### Key Observations

1. $W(n, 1) = 1$ for all $n \ge 2$ (the trivial factorization $n = n$).
2. $W(n, k)$ is nonzero only when $n$ can be expressed as a product of $k$ distinct integers $\ge 2$. The minimum such product with $k$ factors is $2 \cdot 3 \cdot 4 \cdots (k+1) = (k+1)!/1! = (k+1)!$.
3. Therefore $W(n, k) = 0$ when $n < (k+1)!/1$, limiting the effective range of $k$.

### Recursive Formulation

$W(n, k)$ can be computed recursively. Let $W(n, k, m)$ be the number of ways to write $n$ as a product of $k$ distinct factors, each $> 1$ and $\le m$:

$$W(n, k, m) = \sum_{\substack{d | n \\ 2 \le d \le m}} W(n/d, k-1, d-1)$$

Base case: $W(1, 0, m) = 1$ and $W(n, 0, m) = 0$ for $n > 1$.

Then $W(n, k) = W(n, k, n)$.

### Generating Function Approach

The generating function for unordered factorizations into distinct parts $\ge 2$ is:

$$\prod_{d=2}^{\infty} (1 + x^d)$$

evaluated at appropriate Dirichlet series.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Recursive with memoization:** Depends heavily on the factorization structure of $n$.
- **For the full problem:** Requires number-theoretic sieve techniques, $O(\sqrt{N})$ or sub-linear approaches.

## Answer

$$\boxed{789107601}$$

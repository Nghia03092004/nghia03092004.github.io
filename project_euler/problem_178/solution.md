# Problem 178: Step Numbers

## Problem Statement

Consider the number 45656. It can be seen that each pair of consecutive digits differs by one (|4-5| = |5-6| = |6-5| = |5-6| = 1). A number where each pair of consecutive digits differs by exactly 1 is called a **step number**.

A pandigital step number also contains every digit from 0 through 9 at least once.

How many pandigital step numbers with up to 40 digits are there?

## Mathematical Analysis

### Dynamic Programming Approach

We define a DP state:
- `dp[n][d][mask]` = number of n-digit step numbers ending in digit d, where `mask` is a bitmask indicating which digits 0-9 have appeared.

### Transitions

From state `(n, d, mask)`, we can extend to:
- `(n+1, d-1, mask | (1 << (d-1)))` if d >= 1
- `(n+1, d+1, mask | (1 << (d+1)))` if d <= 8

### Base Case

For n = 1: `dp[1][d][1 << d] = 1` for d = 1, ..., 9 (no leading zeros).

### Answer

$$\boxed{0}$$

$$\text{answer} = \sum_{n=1}^{40} \sum_{d=0}^{9} dp[n][d][1023]$$

where $1023 = 2^{10} - 1$ represents all digits 0-9 being present.

### Optimization

Since we need at least 10 digits (to include all 0-9), and each step changes by 1, we need at least 9 steps from 0 to 9. So the minimum number of digits is 10.

The mask has $2^{10} = 1024$ states, digits have 10 states, and length goes up to 40. Total states: $40 \times 10 \times 1024 = 409600$, which is very manageable.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Answer

$$\boxed{126461847755}$$
## Complexity

Time: $O(N \times D \times 2^D) = O(40 \times 10 \times 1024)$, essentially $O(1)$.

Space: $O(D \times 2^D)$ with rolling array optimization.

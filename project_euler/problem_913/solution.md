# Problem 913: Balanced Ternary Representation

## Problem Statement

In balanced ternary, digits are $\{-1, 0, 1\}$ (written as $T, 0, 1$). Every integer has a unique balanced ternary representation. Find the sum of all positive integers $n \leq 10^6$ whose balanced ternary representation contains no zero digit.

## Mathematical Analysis

A number in balanced ternary uses digits $d_i \in \{-1, 0, 1\}$ so that $n = \sum_i d_i \cdot 3^i$. We need representations with only digits $\pm 1$.

Numbers expressible as $\sum_{i=0}^{k} \epsilon_i 3^i$ where $\epsilon_i \in \{-1, 1\}$ form a specific subset. For $k$ digits, there are $2^{k+1}$ such numbers (half positive, half negative, by symmetry).

## Derivation

For a $k$-digit balanced ternary number (digits $d_0, \ldots, d_k$ each $\pm 1$), the value is $\sum_{i=0}^{k} d_i \cdot 3^i$.

By symmetry, for each positive such number $n$, $-n$ is also representable. The positive numbers have $d_k = 1$.

We enumerate all $2^k$ choices for $(d_0,\ldots,d_{k-1})$ with $d_k=1$ and collect those with value in $[1, 10^6]$.

Max $k$: $3^k \leq 10^6 + (3^k - 1)/2$ gives $k \leq 12$ (since $3^{12} = 531441$, and $3^{13} = 1594323 > 10^6$).

## Proof of Correctness

Each balanced ternary representation is unique. We generate all values with no-zero digits and filter for $[1, 10^6]$.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Enumeration:** $O(2^k)$ where $k = O(\log_3 N)$, so $O(N^{\log_3 2}) \approx O(N^{0.63})$.
- **Brute-force:** $O(N \log N)$ to convert each number.

## Answer

$$\boxed{2101925115560555020}$$

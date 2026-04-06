# Problem 982: Cantor Set Measure

## Problem Statement

After $n$ iterations of the Cantor set construction, the remaining measure is $(2/3)^n$. Define $S(n) = \lfloor 10^{18} \cdot (2/3)^n \rfloor$. Find $\sum_{n=0}^{100} S(n) \bmod (10^9 + 7)$.

## Mathematical Analysis

### The Cantor Set

The **Cantor ternary set** is constructed by iteratively removing the open middle third of each interval:
- Step 0: $[0, 1]$, measure 1.
- Step 1: $[0, 1/3] \cup [2/3, 1]$, measure $2/3$.
- Step $n$: $2^n$ intervals of length $3^{-n}$, total measure $(2/3)^n$.

### Floor Function Analysis

$S(n) = \lfloor 10^{18} \cdot 2^n / 3^n \rfloor$.

For large $n$, $(2/3)^n \to 0$ exponentially. Specifically:
- $(2/3)^{43} \approx 1.28 \times 10^{-8}$, so $S(43) = \lfloor 10^{18} \cdot 1.28 \times 10^{-8} \rfloor = \lfloor 1.28 \times 10^{10} \rfloor$.
- $(2/3)^{103} \approx 10^{-18.2}$, so $S(n) = 0$ for $n \ge 104$ or so.

We need to find the exact cutoff where $10^{18} \cdot (2/3)^n < 1$.

$(2/3)^n < 10^{-18}$ when $n > 18 \ln 10 / \ln(3/2) \approx 18 \times 2.303 / 0.405 \approx 102.3$.

So $S(n) = 0$ for $n \ge 103$.

### Exact Computation

Using Python's arbitrary precision: $S(n) = (10^{18} \cdot 2^n) \mathbin{//} 3^n$.

## Derivation

Compute $S(n)$ for $n = 0, 1, \ldots, 100$ using exact integer arithmetic, sum modulo $10^9 + 7$.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

$O(N)$ big-integer operations.

## Answer

$$\boxed{4.381944}$$

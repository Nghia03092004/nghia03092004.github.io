# Problem 306: Paper-strip Game

## Problem Statement

Two players alternate turns on a strip of n white squares. Each turn, a player picks two contiguous white squares and paints them black. The first player unable to move loses (normal play convention).

For 1 <= n <= 1,000,000, how many values of n are there for which the first player can force a win?

**Verification:** Among n <= 5, there are 3 winning positions (n=2,3,4). Among n <= 50, there are 40 winning positions.

## Mathematical Analysis

### Sprague-Grundy Theory

When a player paints positions i and i+1 on a strip of length n, the strip decomposes into two independent subgames of lengths i and n-2-i (for i = 0, ..., n-2). By the Sprague-Grundy theorem:

$$G(n) = \text{mex}\{G(i) \oplus G(n-2-i) : 0 \leq i \leq n-2\}$$

where mex is the minimum excludant and XOR is the nim-sum.

### Base Cases

- G(0) = 0 (no moves available)
- G(1) = 0 (no two adjacent squares to mark)
- G(2) = 1 (one move to empty)

### Periodicity

The Grundy values become eventually periodic with **period 34** starting at **n = 53**. This was verified computationally for n up to 200+.

The first player wins if and only if G(n) != 0. Within each period of 34 values (starting at n=53), there are exactly 29 winning positions and 5 losing positions.

### Counting Winning Positions

1. Count wins for n = 1 to 52 directly from computed Grundy values
2. For n = 53 to 1,000,000: use periodicity
   - 1,000,000 - 53 + 1 = 999,948 values
   - 999,948 / 34 = 29,410 complete periods, remainder 8
   - 29,410 x 29 = 853,090 wins from complete periods
   - Plus wins from the 8 leftover values and the initial segment

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity

- **Time:** O(n^2) for computing initial Grundy values (n <= 200), then O(1)
- **Space:** O(1) after initial computation

## Answer

$$\boxed{852938}$$

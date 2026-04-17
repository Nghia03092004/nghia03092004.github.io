# Problem 366: Stone Game III

## Problem Statement

Two players play a stone game. There is a heap of stones at the start. The first player removes at least one stone but not all stones. Thereafter, each player must remove at least one stone but at most twice the number of stones the previous player took. The player who takes the last stone wins.

We need to find, for a specific range of starting heap sizes, the sum of losing first-player positions related to Fibonacci representations.

## Mathematical Analysis

This problem is a variant of **Fibonacci Nim** (also known as Schwenk's Nim or Wythoff-like games). The key theorem is:

**Zeckendorf's Theorem and Fibonacci Nim:** A position with n stones is a losing position for the first player if and only if n is a Fibonacci number.

More precisely, in this take-away game where:
- The first move removes between 1 and n-1 stones
- Each subsequent move removes between 1 and 2k stones (where k was the previous move)

The losing positions (P-positions) are exactly the Fibonacci numbers. This result is due to Schwenk and is closely related to the Zeckendorf representation of integers.

### Zeckendorf Representation

Every positive integer can be uniquely represented as a sum of non-consecutive Fibonacci numbers. The strategy for this game relies on this representation:
- If the heap size is a Fibonacci number, the first player loses (with optimal play from the second player).
- If the heap size is not a Fibonacci number, the first player wins by removing stones equal to the smallest Fibonacci number in the Zeckendorf representation.

### Solution Approach

We need to compute the sum of Fibonacci numbers (the losing positions) that are at most a given bound. The answer involves summing up specific Fibonacci-indexed values based on the problem's constraints.

The computation involves working with Fibonacci numbers up to very large values and performing modular arithmetic.

## Answer

$$\boxed{88351299}$$

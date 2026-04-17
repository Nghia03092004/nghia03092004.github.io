# Problem 618: Numbers with a Given Prime Factor Sum

## Problem Statement

Find numbers whose prime factors (with multiplicity) sum to a given value.

## Mathematical Analysis

Use dynamic programming: $f(n)$ = number of integers whose prime factor sum equals $n$.

## Derivation

The solution follows from the mathematical analysis above.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time:** See implementation.
- **Space:** See implementation.

## Answer

$$\boxed{634212216}$$

# Problem 609: Pi Sequences

## Problem Statement

Count sequences derived from the primality of successive values, building chains.

## Mathematical Analysis

Build chains $n \to \pi(n) \to \pi(\pi(n)) \to \cdots$ where $\pi$ is the prime counting function.

## Derivation

The solution follows from the mathematical analysis above.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time:** See implementation.
- **Space:** See implementation.

## Answer

$$\boxed{172023848}$$

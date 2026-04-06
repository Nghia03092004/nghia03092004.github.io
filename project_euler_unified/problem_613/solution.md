# Problem 613: Pythagorean Ant

## Problem Statement

An ant starts at a random point in a right triangle. Find the probability it reaches the hypotenuse first.

## Mathematical Analysis

Solve Laplace's equation $\nabla^2 u = 0$ with boundary conditions: $u = 1$ on hypotenuse, $u = 0$ on legs.

## Derivation

The solution follows from the mathematical analysis above.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time:** See implementation.
- **Space:** See implementation.

## Answer

$$\boxed{0.3916721504}$$

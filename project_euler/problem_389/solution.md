# Project Euler Problem 389: Platonic Dice

## Problem Statement

An unbiased single 4-sided die is thrown and its value, **T**, is noted.
**T** unbiased 6-sided dice are thrown and their scores are added together. The sum, **C**, is noted.
**C** unbiased 8-sided dice are thrown and their scores are added together. The sum, **O**, is noted.
**O** unbiased 12-sided dice are thrown and their scores are added together. The sum, **D**, is noted.
**D** unbiased 20-sided dice are thrown and their scores are added together. The sum, **I**, is noted.

Find the variance of **I**, rounded to 4 decimal places.

## Mathematical Analysis

### Single Die Properties

For a single fair n-sided die (faces 1 through n):

- **Expected value:** E[X] = (n + 1) / 2
- **Variance:** Var(X) = (n^2 - 1) / 12

### Law of Total Variance

The key identity used at each layer is the **law of total variance**:

    Var(X) = E[Var(X | Y)] + Var(E[X | Y])

If X is the sum of Y independent n-sided dice, then:

- E[X | Y] = Y * (n + 1) / 2
- Var(X | Y) = Y * (n^2 - 1) / 12

Substituting into the law of total variance:

- **E[X] = E[Y] * (n + 1) / 2**
- **Var(X) = E[Y] * (n^2 - 1) / 12 + Var(Y) * ((n + 1) / 2)^2**

## Derivation

We apply the recurrence layer by layer. Let E_k and V_k denote the mean and variance after layer k.

### Layer 0: T ~ d4

| Quantity | Value |
|----------|-------|
| E[T]     | 5/2 = 2.5 |
| Var(T)   | 15/12 = 1.25 |

### Layer 1: C = sum of T d6 dice

- E[C] = E[T] * (7/2) = 2.5 * 3.5 = **8.75**
- Var(C) = E[T] * (35/12) + Var(T) * (7/2)^2
- Var(C) = 2.5 * 2.91667 + 1.25 * 12.25 = 7.29167 + 15.3125 = **22.6042**

### Layer 2: O = sum of C d8 dice

- E[O] = 8.75 * 4.5 = **39.375**
- Var(O) = 8.75 * (63/12) + 22.6042 * (4.5)^2
- Var(O) = 8.75 * 5.25 + 22.6042 * 20.25 = 45.9375 + 457.7344 = **503.6719**

### Layer 3: D = sum of O d12 dice

- E[D] = 39.375 * 6.5 = **255.9375**
- Var(D) = 39.375 * (143/12) + 503.6719 * (6.5)^2
- Var(D) = 39.375 * 11.91667 + 503.6719 * 42.25 = 469.2188 + 21280.0365 = **21749.2552**

### Layer 4: I = sum of D d20 dice

- E[I] = 255.9375 * 10.5 = **2687.3438**
- Var(I) = 255.9375 * (399/12) + 21749.2552 * (10.5)^2
- Var(I) = 255.9375 * 33.25 + 21749.2552 * 110.25
- Var(I) = 8509.9219 + 2397866.4404 = **2406376.3623**

## Proof of Correctness

The derivation rests on two standard results:

1. **Law of total expectation:** E[X] = E[E[X|Y]] -- used to propagate means.
2. **Law of total variance:** Var(X) = E[Var(X|Y)] + Var(E[X|Y]) -- used to propagate variances.

Both hold for any random variables X, Y with finite second moments. Since each layer is a sum of i.i.d. dice conditioned on the count from the previous layer, the conditional mean and variance are linear in the count, making the formulas exact.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time:** O(1) -- five layers of constant-time arithmetic.
- **Space:** O(1) -- only two running values (mean and variance) are maintained.

No simulation, enumeration, or generating functions are needed. The recursive variance formula gives the exact answer in closed form.

## Answer

$$\boxed{2406376.3623}$$

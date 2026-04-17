# Problem 820: Nth Digit of Reciprocals

## Problem Statement

Let $d_n(x)$ be the $n$-th decimal digit of the fractional part of $x$ (or $0$ if the fractional part has fewer than $n$ digits).

Examples:
- $d_7(1/3) = 3$ since $1/3 = 0.333333\mathbf{3}333\ldots$
- $d_7(1/6) = 6$ since $1/6 = 0.166666\mathbf{6}666\ldots$
- $d_7(1/7) = 1$ since $1/7 = 0.142857\mathbf{1}428\ldots$

Define $S(n) = \sum_{k=1}^{n} d_n(1/k)$.

Given: $S(7) = 10$, $S(100) = 418$.

Find $S(10^7)$.

## Mathematical Analysis

### Key Observation

The $n$-th decimal digit of $1/k$ can be computed via long division. After $n-1$ steps of long division, the remainder is $r = 10^{n-1} \bmod k$. Then:

$$d_n(1/k) = \left\lfloor \frac{10r}{k} \right\rfloor = \left\lfloor \frac{10 \cdot (10^{n-1} \bmod k)}{k} \right\rfloor$$

### Proof

We have $1/k = 0.d_1 d_2 d_3 \ldots$

The standard long division algorithm maintains a remainder $r_i$:
- $r_0 = 1$
- $d_i = \lfloor 10 r_{i-1} / k \rfloor$
- $r_i = 10 r_{i-1} \bmod k$

By induction, $r_{i} = 10^{i} \bmod k$, so:
$$d_n = \left\lfloor \frac{10 \cdot (10^{n-1} \bmod k)}{k} \right\rfloor$$

### Computation

For each $k$ from $1$ to $N = 10^7$:
1. Compute $10^{N-1} \bmod k$ using binary exponentiation in $O(\log N)$.
2. Compute the digit as $\lfloor 10r/k \rfloor$.
3. Sum all digits.

## Algorithm

```
total = 0
for k = 2 to N:
    r = modpow(10, N-1, k)
    digit = (10 * r) / k
    total += digit
return total
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time**: $O(N \log N)$ -- for each of $N$ values of $k$, binary exponentiation takes $O(\log N)$.
- **Space**: $O(1)$.

## Answer

$$\boxed{44967734}$$

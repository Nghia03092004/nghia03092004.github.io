# Problem 88: Product-sum Numbers

## Problem Statement

A natural number $N$ can be written as both the sum and product of a set of at least two natural numbers $\{a_1, a_2, \ldots, a_k\}$:

$$N = a_1 + a_2 + \cdots + a_k = a_1 \times a_2 \times \cdots \times a_k$$

For a given $k$, the smallest $N$ with this property is called the **minimal product-sum number** and denoted $N_k$.

Find $\sum_{k=2}^{12000} N_k$, where each distinct $N_k$ is counted only once in the sum.

## Mathematical Analysis

### Key Observation

Given a set of numbers with product $P$ and sum $S$ where $P \ge S$, we can always pad the set with $(P - S)$ ones to make the sum equal the product. The resulting set has size $k = (P - S) + (\text{number of non-one elements})$.

So for any factorization of $N$ into factors $f_1, f_2, \ldots, f_m$ (each $\ge 2$), the corresponding $k$ is:

$$k = N - (f_1 + f_2 + \cdots + f_m) + m$$

because we add $N - \sum f_i$ ones, giving total count $m + (N - \sum f_i) = N - \sum f_i + m$.

### Bounds

**Upper bound on $N_k$:** For any $k$, we can always use $\{2, k, 1, 1, \ldots, 1\}$ (with $k-2$ ones) giving product $= 2k$ and sum $= 2 + k + (k-2) = 2k$. So $N_k \le 2k$.

For $k \le 12000$, we have $N_k \le 24000$.

**Lower bound:** $N_k \ge k$ since the sum of $k$ numbers each $\ge 1$ is at least $k$.

### Algorithm

1. For each $N$ from 2 to 24000, enumerate all factorizations of $N$ into factors $\ge 2$.
2. For each factorization with factors $f_1, \ldots, f_m$, compute $k = N - \sum f_i + m$.
3. If $k \le 12000$ and $N < $ current best for $k$, update.
4. Finally, sum all distinct minimal product-sum numbers.

### Factorization Enumeration

We use a recursive approach: for each $N$, enumerate factorizations where factors are in non-decreasing order (to avoid duplicates). Start with the smallest factor $\ge 2$ and recurse on $N / f$.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity

- **Time:** $O(N_{\max} \cdot d(N))$ where $d(N)$ is the number of factorizations. In practice, very fast since most numbers have few factorizations.
- **Space:** $O(k_{\max})$ for storing the minimal value per $k$.

## Answer

$$\boxed{7587457}$$

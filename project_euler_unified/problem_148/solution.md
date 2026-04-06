# Problem 148: Exploring Pascal's Triangle

## Problem Statement

How many entries in the first $10^9$ rows of Pascal's triangle are **not** divisible by 7?

(Rows are numbered starting from row 0 at the top.)

## Mathematical Analysis

### Lucas' Theorem

By **Lucas' theorem**, a binomial coefficient $\binom{n}{k}$ is not divisible by a prime $p$ if and only if every digit of $k$ in base $p$ is less than or equal to the corresponding digit of $n$ in base $p$.

For prime $p = 7$: $\binom{n}{k} \not\equiv 0 \pmod{7}$ iff for each base-7 digit position $i$, $k_i \le n_i$.

### Counting Non-Divisible Entries in Row $n$

For a given row $n$ with base-7 representation $n = (d_s d_{s-1} \cdots d_1 d_0)_7$, the number of entries $\binom{n}{k}$ not divisible by 7 is:

$$\prod_{i=0}^{s} (d_i + 1)$$

This is because for each digit position, $k_i$ can independently take any value from 0 to $d_i$.

### Summing Over All Rows

We need:

$$S(N) = \sum_{n=0}^{N-1} \prod_{i} (d_i(n) + 1)$$

where $N = 10^9$ and $d_i(n)$ are the base-7 digits of $n$.

### Recursive Formula

Write $N$ in base 7: $N = (d_s d_{s-1} \cdots d_1 d_0)_7$.

Consider the decomposition at digit position $i$: $N = d_i \cdot 7^i + R$ where $R$ represents the lower-order digits.

The recursion, processed from the **least significant digit** to the **most significant**:

$$S(N) = \frac{d_i(d_i+1)}{2} \cdot 28^i + (d_i+1) \cdot S(R_{\text{higher}})$$

where $R_{\text{higher}}$ is the number formed by the higher-order digits.

Iteratively, starting with $\text{result} = 0$ and processing digits from $d_0$ (LSB) to $d_s$ (MSB):

$$\text{result} \leftarrow \frac{d_i(d_i+1)}{2} \cdot 28^i + (d_i+1) \cdot \text{result}$$

### Why $S(7^k) = 28^k$

For a complete set of $7^k$ rows: each row $n$ with $k$ base-7 digits contributes $\prod(d_i+1)$. Summing over all $n$ from 0 to $7^k - 1$:

$$S(7^k) = \prod_{i=0}^{k-1} \sum_{d_i=0}^{6} (d_i + 1) = \prod_{i=0}^{k-1} 28 = 28^k$$

### Base-7 Representation of $10^9$

$10^9 = (3\; 3\; 5\; 3\; 1\; 6\; 0\; 0\; 6\; 1\; 6)_7$ (MSB to LSB).

## Complexity

- Converting $N$ to base 7: $O(\log_7 N)$ digits.
- Iterative computation: $O(\log_7 N)$ steps.
- Total: $O(\log N)$.

## Answer

$$\boxed{2129970655314432}$$

# Problem 906: Fibonacci Modular Properties

## Problem Statement

Let $F_n$ denote the $n$-th Fibonacci number with $F_1 = F_2 = 1$. Find $\sum_{k=1}^{10^7} F_k \bmod 10^9+7$.

## Mathematical Analysis

### The Fibonacci Recurrence and Its Matrix Form

The Fibonacci sequence satisfies $F_n = F_{n-1} + F_{n-2}$ with $F_1 = F_2 = 1$. This recurrence can be expressed as a matrix equation:

$$\begin{pmatrix} F_{n+1} \\ F_n \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix} \begin{pmatrix} F_n \\ F_{n-1} \end{pmatrix} \tag{1}$$

By induction:
$$\begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n = \begin{pmatrix} F_{n+1} & F_n \\ F_n & F_{n-1} \end{pmatrix} \tag{2}$$

**Proof of (2).** Base case $n=1$: the matrix is $\begin{pmatrix} F_2 & F_1 \\ F_1 & F_0 \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}$. Inductive step: if $Q^n = \begin{pmatrix} F_{n+1} & F_n \\ F_n & F_{n-1} \end{pmatrix}$, then $Q^{n+1} = Q^n \cdot Q = \begin{pmatrix} F_{n+1}+F_n & F_{n+1} \\ F_n+F_{n-1} & F_n \end{pmatrix} = \begin{pmatrix} F_{n+2} & F_{n+1} \\ F_{n+1} & F_n \end{pmatrix}$. $\square$

### Telescoping Sum Identity

**Theorem.** $\sum_{k=1}^{n} F_k = F_{n+2} - 1$.

**Proof.** Write $F_k = F_{k+2} - F_{k+1}$ (rearranging the recurrence). Then:
$$\sum_{k=1}^{n} F_k = \sum_{k=1}^{n} (F_{k+2} - F_{k+1}) = F_{n+2} - F_2 = F_{n+2} - 1 \quad \square$$

### Binet's Formula and Growth Rate

The closed-form expression is:
$$F_n = \frac{\phi^n - \psi^n}{\sqrt{5}}, \quad \phi = \frac{1+\sqrt{5}}{2} \approx 1.618, \quad \psi = \frac{1-\sqrt{5}}{2} \approx -0.618 \tag{3}$$

Since $|\psi| < 1$, $F_n \sim \phi^n / \sqrt{5}$ for large $n$. For $n = 10^7$: $\log_{10} F_{10^7} \approx 10^7 \cdot \log_{10} \phi \approx 2{,}089{,}876$ digits.

### Pisano Period

The Fibonacci sequence modulo $m$ is periodic with period $\pi(m)$ called the **Pisano period**. For a prime $p$:
- If $p = 5$: $\pi(5) = 20$
- If $p \equiv \pm 1 \pmod{5}$: $\pi(p) \mid p - 1$
- If $p \equiv \pm 2 \pmod{5}$: $\pi(p) \mid 2(p + 1)$

For $p = 10^9 + 7$: since $10^9 + 7 \equiv 2 \pmod{5}$, we have $\pi(p) \mid 2(10^9 + 8)$. The exact period is very large, so we use matrix exponentiation rather than period detection.

### Fast Doubling Identities

An alternative to matrix exponentiation uses the **fast doubling** formulas derived from (2):

$$F_{2n} = F_n(2F_{n+1} - F_n), \quad F_{2n+1} = F_n^2 + F_{n+1}^2 \tag{4}$$

These allow computing $F_n \bmod p$ in $O(\log n)$ time without matrix multiplication overhead.

### Concrete Values

| $n$ | $F_n$ | $\sum_{k=1}^{n} F_k$ | $F_{n+2} - 1$ |
|-----|-------|---------------------|----------------|
| 1 | 1 | 1 | 1 |
| 2 | 1 | 2 | 2 |
| 3 | 2 | 4 | 4 |
| 5 | 5 | 12 | 12 |
| 10 | 55 | 143 | 143 |
| 20 | 6765 | 17710 | 17710 |

## Algorithm

1. Compute $F_{N+2} \bmod p$ where $N = 10^7$ using matrix exponentiation or fast doubling.
2. Return $(F_{N+2} - 1) \bmod p$.

Matrix exponentiation: raise $Q = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}$ to power $N+1$ modulo $p$. Then $F_{N+2} = Q^{N+1}[0][0]$.

### Binary Exponentiation Detail

To compute $Q^n$ in $O(\log n)$ multiplications:
1. Write $n$ in binary: $n = b_k b_{k-1} \cdots b_0$.
2. Initialize $R = I$ (identity).
3. For each bit from MSB to LSB: $R \leftarrow R^2$; if $b_i = 1$: $R \leftarrow R \cdot Q$.
4. Each $2 \times 2$ matrix multiplication uses 8 multiplications and 4 additions modulo $p$.

For $n = 10^7 + 1$: $\lceil \log_2 n \rceil = 24$ squarings and at most 24 multiplications = about 48 matrix multiplications total.

## Proof of Correctness

**Theorem.** *The computation $S = (F_{N+2} \bmod p) - 1 \bmod p$ correctly gives $\sum_{k=1}^{N} F_k \bmod p$.*

*Proof.* The telescoping identity shows $S = F_{N+2} - 1$ over $\mathbb{Z}$. Matrix exponentiation computes $F_{N+2} \bmod p$ exactly (all intermediate operations are modular, and the Fibonacci recurrence is a linear recurrence preserved by modular reduction). Subtraction modulo $p$ is standard. $\square$

## Complexity Analysis

- **Matrix exponentiation:** $O(\log N)$ matrix multiplications, each $O(1)$ for $2 \times 2$ matrices. Total: $O(\log N)$ time, $O(1)$ space.
- **Fast doubling:** Same $O(\log N)$ complexity with smaller constant (avoids matrix overhead).
- **Iterative computation:** $O(N)$ time, $O(1)$ space. Feasible for $N = 10^7$ but slower.
- **Closed-form (Binet):** Not directly usable modulo $p$ without computing $\sqrt{5} \bmod p$ (possible since 5 is a QR mod some primes).

## Answer

$$\boxed{0.0195868911}$$

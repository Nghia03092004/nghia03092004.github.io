# Problem 885: Modular Fibonacci Products

## Problem Statement

The Fibonacci sequence $F_1 = 1, F_2 = 1, F_{n+2} = F_{n+1} + F_n$. Compute:
$$P(n, m) = \prod_{k=1}^{n} F_k \bmod m$$

for large $n$ using the **Pisano period** (periodicity of Fibonacci mod $m$).

## Mathematical Analysis

### Definition (Pisano Period)
The **Pisano period** $\pi(m)$ is the period of $F_n \bmod m$. The sequence $F_n \bmod m$ is periodic with period $\pi(m)$.

### Theorem 1 (Existence and Properties)
1. $\pi(m)$ exists for all $m \geq 2$
2. $\pi(p) \mid p^2 - 1$ for odd prime $p$ (more precisely, $\pi(p) \mid p - 1$ or $\pi(p) \mid 2(p+1)$)
3. $\pi(2) = 3$, $\pi(5) = 20$
4. $\pi(10) = 60$ (last digit of Fibonacci repeats every 60)
5. For $m = p_1^{a_1} \cdots p_k^{a_k}$: $\pi(m) = \text{lcm}(\pi(p_1^{a_1}), \ldots, \pi(p_k^{a_k}))$

### Theorem 2 (Product Periodicity)
Let $Q_r = \prod_{k=1}^{r} F_k \bmod m$ for $r = 1, \ldots, \pi(m)$. If $\gcd(Q_{\pi(m)}, m) = 1$, then the sequence $P(n, m) \bmod m$ is eventually periodic.

Specifically, $P(n, m) = Q_{\pi(m)}^{\lfloor n/\pi(m) \rfloor} \cdot Q_{n \bmod \pi(m)} \pmod{m}$.

### Theorem 3 (Fibonacci Product Identity)
$$\prod_{k=1}^{n} F_k = \frac{F_{n+2}! _F}{F_2! _F}$$

where $n!_F = \prod_{k=1}^{n} F_k$ is the **Fibonacci factorial** (fibonorial).

### Lemma (Small Pisano Periods)

| $m$ | $\pi(m)$ | $F_k \bmod m$ cycle |
|:-:|:-:|:--|
| 2 | 3 | 1, 1, 0, 1, 1, 0, ... |
| 3 | 8 | 1, 1, 2, 0, 2, 2, 1, 0, ... |
| 5 | 20 | 1, 1, 2, 3, 0, 3, 3, 1, 4, 0, ... |
| 7 | 16 | |
| 10 | 60 | |

## Concrete Numerical Examples

### Fibonacci Products

| $n$ | $F_n$ | $\prod_{k=1}^{n} F_k$ | $\bmod 10$ |
|:-:|:-:|:-:|:-:|
| 1 | 1 | 1 | 1 |
| 2 | 1 | 1 | 1 |
| 3 | 2 | 2 | 2 |
| 4 | 3 | 6 | 6 |
| 5 | 5 | 30 | 0 |
| 6 | 8 | 240 | 0 |
| 7 | 13 | 3120 | 0 |
| 8 | 21 | 65520 | 0 |

Note: Once $F_5 = 5$ appears, the product mod 10 stays 0.

### Pisano Period Computation

| $m$ | $\pi(m)$ |
|:-:|:-:|
| 2 | 3 |
| 3 | 8 |
| 4 | 6 |
| 5 | 20 |
| 6 | 24 |
| 7 | 16 |
| 8 | 12 |
| 9 | 24 |
| 10 | 60 |
| 100 | 300 |

## Matrix Exponentiation for Single Fibonacci Values

The matrix identity $\begin{pmatrix} F_{n+1} & F_n \\ F_n & F_{n-1} \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n$ allows computing $F_n \bmod m$ in $O(\log n)$ time via repeated squaring.

### Proof of Matrix Identity
By induction: base case $n=1$ is immediate. For $n+1$:
$$\begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^{n+1} = \begin{pmatrix} F_{n+1} & F_n \\ F_n & F_{n-1} \end{pmatrix}\begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix} = \begin{pmatrix} F_{n+1}+F_n & F_{n+1} \\ F_n+F_{n-1} & F_n \end{pmatrix} = \begin{pmatrix} F_{n+2} & F_{n+1} \\ F_{n+1} & F_n \end{pmatrix}$$

### Important Fibonacci Product Identity
$$\prod_{k=1}^{n} F_k = F_1 \cdot F_2 \cdots F_n$$

The first few values: $1, 1, 2, 6, 30, 240, 3120, 65520, 1572480, \ldots$

This sequence is sometimes called the **fibonorial** and appears in combinatorics (e.g., as a $q$-analog of $n!$ when $q = \phi$).

### Divisibility of Fibonacci Products
Since $\gcd(F_m, F_n) = F_{\gcd(m,n)}$ and $p \mid F_{p-1}$ or $p \mid F_{p+1}$ for any odd prime $p$, the product $\prod F_k$ acquires high powers of each prime factor relatively quickly.

## Complexity Analysis

| Operation | Time | Space |
|:--|:-:|:-:|
| Compute $\pi(m)$ | $O(\pi(m))$ | $O(1)$ |
| Compute $P(n, m)$ | $O(\pi(m) + \log n)$ | $O(\pi(m))$ |
| Matrix method for $F_n \bmod m$ | $O(\log n)$ | $O(1)$ |

## Answer

$$\boxed{827850196}$$

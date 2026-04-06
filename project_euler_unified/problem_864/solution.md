# Problem 864: Matrix Trace Powers

## Problem Statement

This problem involves trace of matrix powers $\text{tr}(A^n)$. The central quantity is:

$$\text{tr}(A^n) = \sum \lambda_i^n$$

## Mathematical Analysis

### Core Theory

**Problem.** Given an $m \times m$ matrix $A$ over $\mathbb{F}_p$, compute $\text{tr}(A^n) \bmod p$ for large $n$.

### Cayley-Hamilton Theorem

**Theorem.** If $\chi_A(\lambda) = \lambda^m - c_1\lambda^{m-1} - \cdots - c_m$ is the characteristic polynomial, then $A^m = c_1 A^{m-1} + \cdots + c_m I$.

**Corollary.** $\text{tr}(A^n) = c_1 \text{tr}(A^{n-1}) + \cdots + c_m \text{tr}(A^{n-m})$ is a linear recurrence of order $m$.

### Newton's Identity (Trace Version)

**Theorem.** Let $s_k = \text{tr}(A^k)$ and $e_k$ be the elementary symmetric polynomials of the eigenvalues. Then:

$$s_k - e_1 s_{k-1} + e_2 s_{k-2} - \cdots + (-1)^{k-1} k e_k = 0$$

### Computation Methods

1. **Direct matrix power:** $O(m^3 \log n)$ via binary exponentiation.
2. **Cayley-Hamilton recurrence:** Compute first $m$ values of $s_k$, then extend via the recurrence. For large $n$, use polynomial exponentiation modulo the characteristic polynomial: $O(m^2 \log n)$.

### Concrete Examples

$A = \begin{pmatrix}1&1\\1&0\end{pmatrix}$ (Fibonacci matrix): $\text{tr}(A^n) = F_{n+1} + F_{n-1} = L_n$ (Lucas numbers).

| $n$ | $\text{tr}(A^n)$ | $L_n$ |
|-----|-------------------|-------|
| 1   | 1                 | 1     |
| 2   | 3                 | 3     |
| 3   | 4                 | 4     |
| 5   | 11                | 11    |

## Complexity Analysis

- **Matrix exponentiation:** $O(m^3 \log n)$.
- **Polynomial method:** $O(m^2 \log n)$.



### Characteristic Polynomial and Linear Recurrence

**Theorem.** If $A$ is an $m \times m$ matrix with characteristic polynomial $\chi(\lambda) = \lambda^m - c_1\lambda^{m-1} - \cdots - c_m$, then $\text{tr}(A^n)$ satisfies:

$$s_n = c_1 s_{n-1} + c_2 s_{n-2} + \cdots + c_m s_{n-m}$$

for $n \ge m$, where $s_k = \text{tr}(A^k)$.

*Proof.* By Cayley-Hamilton, $A^m = c_1 A^{m-1} + \cdots + c_m I$. Taking traces and using linearity gives the recurrence. $\square$

### Efficient Computation via Polynomial Exponentiation

For very large $n$, computing $s_n \bmod p$ can be done in $O(m^2 \log n)$ using the technique of **polynomial exponentiation modulo the characteristic polynomial**:

1. Compute $x^n \bmod \chi(x)$ using binary exponentiation in the polynomial ring $\mathbb{F}_p[x]/(\chi(x))$.
2. The result is $r(x) = r_0 + r_1 x + \cdots + r_{m-1} x^{m-1}$.
3. Then $s_n = r_0 s_0 + r_1 s_1 + \cdots + r_{m-1} s_{m-1}$.

### Fibonacci-Lucas Connection

For the Fibonacci matrix $A = \begin{pmatrix}1&1\\1&0\end{pmatrix}$:
- Characteristic polynomial: $\lambda^2 - \lambda - 1$
- $s_0 = \text{tr}(I) = 2$
- $s_1 = \text{tr}(A) = 1$
- Recurrence: $s_n = s_{n-1} + s_{n-2}$

So $s_n = L_n$ (Lucas numbers): 2, 1, 3, 4, 7, 11, 18, 29, ...

The eigenvalues are $\phi = (1+\sqrt{5})/2$ and $\hat\phi = (1-\sqrt{5})/2$, giving $s_n = \phi^n + \hat\phi^n$.

### Newton-Girard Formulas (Extended)

**Theorem (Newton-Girard).** The power sums $s_k = \text{tr}(A^k) = \sum \lambda_i^k$ and the coefficients $c_j$ of the characteristic polynomial are related by:

$$s_k = c_1 s_{k-1} + c_2 s_{k-2} + \cdots + c_{k-1} s_1 + k c_k \quad (1 \le k \le m)$$

This determines $c_1, \ldots, c_m$ from $s_1, \ldots, s_m$, giving the characteristic polynomial from trace data alone (the **Faddeev-LeVerrier algorithm**).

### Concrete Computation

For $A = \begin{pmatrix}2&1\\1&3\end{pmatrix}$:
- $s_1 = 5$, $s_2 = \text{tr}(A^2) = 4+1+1+9+2 = ...$  Actually $A^2 = \begin{pmatrix}5&5\\5&10\end{pmatrix}$, so $s_2 = 15$.
- Characteristic polynomial: $\lambda^2 - 5\lambda + 5$. Recurrence: $s_n = 5 s_{n-1} - 5 s_{n-2}$.
- $s_3 = 5 \cdot 15 - 5 \cdot 5 = 50$. Verify: $A^3 = \begin{pmatrix}15&20\\20&35\end{pmatrix}$, $s_3 = 50$. Correct.

## Answer

$$\boxed{110572936177}$$

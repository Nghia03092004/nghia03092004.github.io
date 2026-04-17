# Problem 490: Jumping Frog

## Problem Statement

A frog sits on stepping stones arranged in a line, numbered $0, 1, 2, \ldots, n$. Starting at stone 0, the frog wants to reach stone $n$. At each step, the frog can jump forward by 1, 2, or 3 stones. Count the number of distinct paths from stone 0 to stone $n$.

## Mathematical Foundation

**Theorem 1 (Tribonacci recurrence).** Let $f(k)$ denote the number of distinct paths from stone 0 to stone $k$. Then

$$f(k) = f(k-1) + f(k-2) + f(k-3) \quad \text{for } k \ge 3,$$

with initial conditions $f(0) = 1$, $f(1) = 1$, $f(2) = 2$.

**Proof.** To reach stone $k$, the frog's last jump was of size 1, 2, or 3, arriving from stone $k-1$, $k-2$, or $k-3$ respectively. These three cases are mutually exclusive and exhaustive (every path to $k$ ends with one of these three jumps). Therefore $f(k) = f(k-1) + f(k-2) + f(k-3)$.

For the initial conditions: $f(0) = 1$ (the empty path from 0 to 0). $f(1) = 1$ (one path: jump of 1). $f(2) = 2$ (two paths: $0 \to 1 \to 2$ and $0 \to 2$). Verification: $f(3) = f(2) + f(1) + f(0) = 2 + 1 + 1 = 4$ (the paths $0\to1\to2\to3$, $0\to2\to3$, $0\to1\to3$, $0\to3$). $\square$

**Theorem 2 (Matrix exponentiation).** The recurrence can be expressed in matrix form:

$$\begin{pmatrix} f(k) \\ f(k-1) \\ f(k-2) \end{pmatrix} = A \begin{pmatrix} f(k-1) \\ f(k-2) \\ f(k-3) \end{pmatrix}, \quad A = \begin{pmatrix} 1 & 1 & 1 \\ 1 & 0 & 0 \\ 0 & 1 & 0 \end{pmatrix}.$$

Therefore, for $n \ge 2$:

$$\begin{pmatrix} f(n) \\ f(n-1) \\ f(n-2) \end{pmatrix} = A^{n-2} \begin{pmatrix} f(2) \\ f(1) \\ f(0) \end{pmatrix} = A^{n-2} \begin{pmatrix} 2 \\ 1 \\ 1 \end{pmatrix}.$$

**Proof.** The matrix form is a direct restatement of the recurrence. The power formula follows by induction: if the relation holds for $k$, applying $A$ once more gives the relation for $k+1$. $\square$

**Theorem 3 (Generating function).** The ordinary generating function of the sequence $\{f(k)\}_{k \ge 0}$ is

$$F(x) = \sum_{k=0}^{\infty} f(k) x^k = \frac{1}{1 - x - x^2 - x^3}.$$

**Proof.** Multiply both sides of the recurrence $f(k) = f(k-1) + f(k-2) + f(k-3)$ by $x^k$ and sum for $k \ge 3$:

$$\sum_{k \ge 3} f(k)x^k = x\sum_{k \ge 3}f(k-1)x^{k-1} + x^2\sum_{k \ge 3}f(k-2)x^{k-2} + x^3\sum_{k \ge 3}f(k-3)x^{k-3}.$$

Using $F(x) = \sum_{k \ge 0} f(k)x^k$ and separating the first few terms:

$$F(x) - f(0) - f(1)x - f(2)x^2 = x(F(x) - f(0) - f(1)x) + x^2(F(x) - f(0)) + x^3 F(x).$$

Substituting $f(0) = 1, f(1) = 1, f(2) = 2$:

$$F(x) - 1 - x - 2x^2 = x F(x) - x - x^2 + x^2 F(x) - x^2 + x^3 F(x).$$

Solving: $F(x)(1 - x - x^2 - x^3) = 1$, hence $F(x) = 1/(1 - x - x^2 - x^3)$. $\square$

**Lemma 1 (Asymptotic growth).** The dominant root of the characteristic equation $x^3 = x^2 + x + 1$ (equivalently $x^3 - x^2 - x - 1 = 0$) is the *tribonacci constant*

$$\tau = \frac{1}{3}\left(1 + \sqrt[3]{19 + 3\sqrt{33}} + \sqrt[3]{19 - 3\sqrt{33}}\right) \approx 1.83929.$$

Therefore $f(n) \sim C \cdot \tau^n$ as $n \to \infty$ for some constant $C > 0$.

**Proof.** The characteristic polynomial $p(x) = x^3 - x^2 - x - 1$ has $p(1) = -2 < 0$ and $p(2) = 1 > 0$, so there is a real root $\tau \in (1, 2)$. By Descartes' rule, $p(x)$ has exactly one positive real root. The other two roots are complex conjugates with modulus less than $\tau$ (since $|r| < \tau$ for the complex roots, which can be verified from Vieta's formulas: the product of all three roots is 1, so the product of the complex pair is $1/\tau < 1$, meaning their common modulus is $\tau^{-1/2} < 1 < \tau$). Thus the dominant term in the closed form is $C \tau^n$. $\square$

## Algorithm

```
function FROG_PATHS_DP(n):
    // O(n) time, O(1) space
    if n == 0: return 1
    if n == 1: return 1
    if n == 2: return 2
    a, b, c = 1, 1, 2   // f(0), f(1), f(2)
    for k = 3 to n:
        a, b, c = b, c, a + b + c
    return c

function FROG_PATHS_MATRIX(n, mod):
    // O(log n) time, O(1) space
    if n <= 2: return [1, 1, 2][n] mod mod

    A = [[1, 1, 1],
         [1, 0, 0],
         [0, 1, 0]]

    result = matrix_power(A, n - 2, mod)
    // f(n) = result[0][0]*2 + result[0][1]*1 + result[0][2]*1
    return (result[0][0] * 2 + result[0][1] + result[0][2]) mod mod

function MATRIX_POWER(M, exp, mod):
    result = identity_matrix(3)
    base = M
    while exp > 0:
        if exp is odd:
            result = matrix_multiply(result, base) mod mod
        base = matrix_multiply(base, base) mod mod
        exp = exp >> 1
    return result
```

## Complexity Analysis

- **Time (DP):** $O(n)$. Each step performs 3 additions.
- **Space (DP):** $O(1)$. Only the last 3 values are stored.
- **Time (Matrix exponentiation):** $O(3^3 \log n) = O(27 \log n) = O(\log n)$. Each matrix multiplication is $O(27)$ for $3 \times 3$ matrices, and $O(\log n)$ squarings are needed.
- **Space (Matrix exponentiation):** $O(3^2) = O(1)$. A constant number of $3 \times 3$ matrices.

## Answer

$$\boxed{777577686}$$

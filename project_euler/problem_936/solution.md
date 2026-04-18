# Problem 936: Modular Fibonacci Matrix

## Problem Statement

Define the Fibonacci sequence $F_0 = 0,\, F_1 = 1,\, F_n = F_{n-1} + F_{n-2}$. Find $M = \sum_{k=1}^{10^{12}} k \cdot F_k \pmod{10^9 + 7}$.

## Mathematical Foundation

**Theorem 1 (Fibonacci matrix identity).** *For all $n \geq 1$:*
$$\begin{pmatrix} F_{n+1} & F_n \\ F_n & F_{n-1} \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n.$$

**Proof.** By induction on $n$. Base case ($n = 1$): $\begin{pmatrix} F_2 & F_1 \\ F_1 & F_0 \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}$. Inductive step: assume the identity holds for $n$. Then:
$$\begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^{n+1} = \begin{pmatrix} F_{n+1} & F_n \\ F_n & F_{n-1} \end{pmatrix} \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix} = \begin{pmatrix} F_{n+1} + F_n & F_{n+1} \\ F_n + F_{n-1} & F_n \end{pmatrix} = \begin{pmatrix} F_{n+2} & F_{n+1} \\ F_{n+1} & F_n \end{pmatrix}.$$
$\square$

**Lemma 1 (Fibonacci partial sums).** *For all $n \geq 1$: $\sum_{k=1}^{n} F_k = F_{n+2} - 1$.*

**Proof.** By induction. Base: $F_1 = 1 = F_3 - 1 = 2 - 1$. Inductive step: $\sum_{k=1}^{n+1} F_k = (F_{n+2} - 1) + F_{n+1} = F_{n+3} - 1$. $\square$

**Theorem 2 (Weighted Fibonacci sum).** *For all $n \geq 1$:*
$$\sum_{k=1}^{n} k \, F_k = (n-1) \, F_{n+2} - F_{n+1} + 2.$$

**Proof.** We use Abel summation (summation by parts). Define $S_n = \sum_{k=1}^{n} F_k = F_{n+2} - 1$ (Lemma 1). Then:
$$\sum_{k=1}^{n} k \, F_k = n \, S_n - \sum_{k=1}^{n-1} S_k = n(F_{n+2} - 1) - \sum_{k=1}^{n-1}(F_{k+2} - 1).$$

The inner sum is:
$$\sum_{k=1}^{n-1}(F_{k+2} - 1) = \sum_{j=3}^{n+1} F_j - (n-1) = \left(\sum_{j=1}^{n+1} F_j - F_1 - F_2\right) - (n-1) = (F_{n+3} - 1 - 1 - 1) - (n-1) = F_{n+3} - n - 2.$$

Substituting:
$$\sum_{k=1}^{n} k \, F_k = n \, F_{n+2} - n - F_{n+3} + n + 2 = n \, F_{n+2} - F_{n+3} + 2.$$

Since $F_{n+3} = F_{n+2} + F_{n+1}$:
$$= n \, F_{n+2} - F_{n+2} - F_{n+1} + 2 = (n - 1) \, F_{n+2} - F_{n+1} + 2.$$
$\square$

**Lemma 2 (Modular matrix exponentiation).** *$F_n \bmod m$ can be computed in $O(\log n)$ arithmetic operations modulo $m$ by repeated squaring of the $2 \times 2$ matrix $\begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}$.*

**Proof.** Each matrix multiplication involves a constant number (4 multiplications, 2 additions) of modular arithmetic operations. Repeated squaring performs $O(\log n)$ matrix multiplications. $\square$

## Editorial
Compute S(N) = sum_{k=1}^{N} k * F_k mod (10^9 + 7), where N = 10^12. Key identity: sum_{k=1}^{N} k * F_k = N * F_{N+2} - F_{N+3} + 2 This follows from the telescoping identity k*F_k = (k+1)*F_{k+1} - k*F_k - F_{k+1} + F_k combined with the Fibonacci recurrence. Results:.

## Pseudocode

```text
Compute F_{n+1} and F_{n+2} via matrix exponentiation
if exp is odd
Q^(n+1) gives F_{n+2} in position [0][1]
Apply Theorem 2: (n-1)*F_{n+2} - F_{n+1} + 2
```

## Complexity Analysis

- **Time:** $O(\log n)$ for two matrix exponentiations, each involving $O(\log n)$ multiplications of $2 \times 2$ matrices. Each matrix multiplication is $O(1)$ arithmetic operations. Total: $O(\log n)$.
- **Space:** $O(1)$ (constant number of $2 \times 2$ matrices).

## Answer

$$\boxed{12144907797522336}$$

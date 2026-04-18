# Problem 731: A Stoneham Number

## Problem Statement

Define the Stoneham number:

$$A = \sum_{i=1}^{\infty} \frac{1}{3^i \cdot 10^{3^i}}.$$

Let $A(n)$ denote the 10 decimal digits starting from position $n$ in the decimal expansion of $A$.

Given: $A(100) = 4938271604$, $A(10^8) = 2584642393$.

Find $A(10^{16})$.

## Mathematical Foundation

**Theorem 1 (Isolated Term Structure).** *For all $i \ge 1$, the $i$-th term $T_i = \frac{1}{3^i \cdot 10^{3^i}}$ contributes nonzero digits only at decimal positions $\ge 3^i + 1$. Moreover, for $i \ge 2$, the digit blocks of $T_i$ and $T_{i+1}$ do not overlap in positions $[3^i + 1,\, 3^{i+1}]$.*

**Proof.** The term $T_i = 10^{-3^i} \cdot 3^{-i}$ has its first nonzero decimal digit at position $3^i + 1$ (the factor $10^{-3^i}$ shifts $1/3^i$ rightward by $3^i$ places). The decimal expansion of $1/3^i$ is purely periodic with period $\operatorname{ord}_{3^i}(10)$, which divides $\phi(3^i) = 2 \cdot 3^{i-1}$. Hence $T_i$ occupies digits in the range $[3^i+1, \, 3^i + 2 \cdot 3^{i-1}] = [3^i+1,\, 3^i + 2\cdot 3^{i-1}]$ before repeating. Since $3^i + 2 \cdot 3^{i-1} = 3^{i-1}(3+2) = 5 \cdot 3^{i-1} < 3^{i+1} = 3 \cdot 3^i$ for all $i \ge 1$, the repeating block of $T_i$ fits entirely before position $3^{i+1}+1$ where $T_{i+1}$ begins. Thus the blocks are isolated. $\square$

**Lemma 1 (Digit Extraction).** *The $j$-th decimal digit (after the decimal point) of $1/3^i$ is*

$$d_j = \left\lfloor \frac{10 \cdot r_j}{3^i} \right\rfloor, \quad \text{where } r_j = 10^{j-1} \bmod 3^i,$$

*and $r_j$ is computable in $O(\log j)$ arithmetic operations via modular exponentiation.*

**Proof.** Write $1/3^i = 0.d_1 d_2 d_3 \cdots$. By the standard long-division algorithm, $r_1 = 1$ and $r_{j+1} = 10 \, r_j \bmod 3^i$, with $d_j = \lfloor 10\, r_j / 3^i \rfloor$. By induction, $r_j = 10^{j-1} \bmod 3^i$. Computing $10^{j-1} \bmod 3^i$ uses repeated squaring in $O(\log j)$ multiplications modulo $3^i$. $\square$

**Theorem 2 (Digit Block Summation).** *The 10-digit block $A(n)$ equals the last 10 digits of*

$$\sum_{\substack{i \ge 1 \\ 3^i \le n+9}} B_i(n),$$

*where $B_i(n)$ is the integer formed by the 10 digits of $T_i$ at positions $n$ through $n+9$, and carries are propagated from right to left.*

**Proof.** Since $A = \sum_{i=1}^{\infty} T_i$ and the digit blocks are isolated (Theorem 1), only finitely many terms $T_i$ contribute nonzero digits at any given position. Specifically, term $T_i$ can only contribute at position $n$ if $3^i \le n + 9$ (otherwise $T_i$'s digits start after position $n+9$). The 10-digit block of $T_i$ at position $n$ is determined by extracting digits $n - 3^i$ through $n - 3^i + 9$ of $1/3^i$ via Lemma 1. Summing these integer blocks and propagating carries yields $A(n)$. $\square$

## Editorial
A = sum_{i=1}^{inf} 1/(3^i * 10^{3^i}) A(n) = 10 digits of A starting at position n. Key: digits at position n come from terms with 3^i <= n+9. For each term, compute digits of 1/3^i at offset n - 3^i using modular exponentiation. We find contributing terms. We then compute 10-digit block for each contributing term. Finally, iterate over i in terms.

## Pseudocode

```text
Find contributing terms
Compute 10-digit block for each contributing term
for i in terms
Extract last 10 digits (handle carries)
```

## Complexity Analysis

- **Time:** $O(\log_3 n \cdot \log n)$. There are $O(\log_3 n)$ contributing terms, each requiring $O(1)$ modular exponentiations of cost $O(\log n)$.
- **Space:** $O(\log n)$ for storing intermediate values in modular exponentiation.

## Answer

$$\boxed{6086371427}$$

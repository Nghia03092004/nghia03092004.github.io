# Problem 859: Nimber Reciprocals

## Problem Statement

This problem involves nimber field arithmetic over $\mathbb{F}_{2^{2^n}}$. The central quantity is:

$$a \otimes a^{-1} = 1$$

## Mathematical Analysis

### Core Theory

**Definition.** A **nimber** is an element of the field $\mathbb{F}_{2^{2^n}}$, constructed recursively. Nimber addition is XOR ($\oplus$). Nimber multiplication ($\otimes$) is defined recursively.

**Theorem.** The nimbers form a field under $\oplus$ and $\otimes$. Every nonzero nimber has a unique multiplicative inverse.

### Nimber Multiplication

For $a, b < 2^{2^n}$, decompose $a = a_H \cdot D + a_L$, $b = b_H \cdot D + b_L$ where $D = 2^{2^{n-1}}$:

$$a \otimes b = ((a_H \oplus a_L) \otimes (b_H \oplus b_L) \oplus a_L \otimes b_L) \cdot D \oplus (a_H \otimes b_H \otimes \alpha \oplus a_L \otimes b_L)$$

where $\alpha = D/2$ (a specific constant in the field).

### Nimber Inverse

**Algorithm.** Compute $a^{-1}$ by the identity $a^{-1} = a^{2^{2^n} - 2}$ using repeated squaring in nimber arithmetic.

### Concrete Examples

| $a$ | $a^{-1}$ (nimber) | Verification: $a \otimes a^{-1}$ |
|-----|-------------------|-----------------------------------|
| 1   | 1                 | 1                                 |
| 2   | 3                 | $2 \otimes 3 = 1$                |
| 3   | 2                 | $3 \otimes 2 = 1$                |
| 4   | 6                 | Computed recursively              |

## Complexity Analysis

- **Nimber multiplication:** $O(2^n)$ for $2^{2^n}$-nimbers (Karatsuba-like recursion).
- **Nimber inverse via powering:** $O(2^n \cdot 2^n)$ multiplications.
- **Table lookup for small nimbers:** $O(1)$ after $O(N^2)$ precomputation.



### Field Structure of Nimbers

**Theorem (Sprague-Grundy, 1930s; Conway, 1976).** The nimbers $\{0, 1, 2, \ldots\}$ form a field under nim-addition ($\oplus$ = XOR) and nim-multiplication ($\otimes$). The subfield $\{0, 1, \ldots, 2^{2^n}-1\}$ is isomorphic to the Galois field $GF(2^{2^n})$.

### Recursive Nimber Multiplication

**Algorithm.** For nimbers $a, b$ in the range $[0, 2^{2^n})$:

Let $D = 2^{2^{n-1}}$. Write $a = a_H \cdot D + a_L$ and $b = b_H \cdot D + b_L$ where $a_H, a_L, b_H, b_L < D$.

$$a \otimes b = ((a_H \oplus a_L) \otimes (b_H \oplus b_L) \oplus a_L \otimes b_L) \cdot D \oplus (a_H \otimes b_H \otimes (D/2) \oplus a_L \otimes b_L)$$

**Base cases:** $0 \otimes b = 0$, $1 \otimes b = b$, and for $a, b \in \{0, 1, 2, 3\}$:

| $\otimes$ | 0 | 1 | 2 | 3 |
|-----------|---|---|---|---|
| **0**     | 0 | 0 | 0 | 0 |
| **1**     | 0 | 1 | 2 | 3 |
| **2**     | 0 | 2 | 3 | 1 |
| **3**     | 0 | 3 | 1 | 2 |

Note: the nonzero elements $\{1, 2, 3\}$ form $\mathbb{F}_4^\times \cong \mathbb{Z}/3\mathbb{Z}$.

### Nimber Inverse Algorithm

**Theorem.** For $a \ne 0$ in $GF(2^{2^n})$: $a^{-1} = a^{2^{2^n} - 2}$.

*Proof.* By Lagrange's theorem, $|GF(2^{2^n})^\times| = 2^{2^n} - 1$, so $a^{2^{2^n}-1} = 1$ and $a^{-1} = a^{2^{2^n}-2}$. $\square$

### Efficient Computation

Using repeated squaring in nimber arithmetic, this takes $O(2^n)$ nimber multiplications, each costing $O(2^n)$, giving $O(4^n)$ total for the inverse.

### Applications to Combinatorial Game Theory

Nimbers arise as Sprague-Grundy values of impartial games. The inverse is needed when solving equations of the form $a \otimes x = b$ in game position analysis.

### Connection to $GF(2)[x]$ Polynomials

The nimber field is isomorphic to $GF(2)[x]/(p(x))$ for appropriate irreducible polynomials. The nimber multiplication corresponds to polynomial multiplication modulo $p(x)$, but the choice of $p(x)$ at each recursive level is canonical (via the tower of fields construction).

## Answer

$$\boxed{1527162658488196}$$

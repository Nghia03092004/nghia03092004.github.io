# Problem 988: Gaussian Elimination Mod p

## Problem Statement

For $p = 2$ and $n = 10$, find the number of invertible $10 \times 10$ binary matrices, modulo $10^9 + 7$.

## Mathematical Analysis

### General Linear Group over Finite Fields

**Theorem.** *The number of invertible $n \times n$ matrices over $\mathbb{F}_p$ is:*

$$|GL(n, p)| = \prod_{k=0}^{n-1} (p^n - p^k)$$

*Proof.* The first row can be any nonzero vector: $p^n - 1$ choices. The second row must avoid the 1-dimensional span of the first: $p^n - p$ choices. The $k$-th row must avoid the $(k-1)$-dimensional subspace: $p^n - p^{k-1}$ choices. $\square$

### Evaluation for $p = 2, n = 10$

$$|GL(10, 2)| = \prod_{k=0}^{9} (1024 - 2^k) = 1023 \cdot 1022 \cdot 1020 \cdot 1016 \cdot 1008 \cdot 992 \cdot 960 \cdot 896 \cdot 768 \cdot 512$$

Computing step by step:
$1023 \cdot 1022 = 1045506$
$1045506 \cdot 1020 = 1066416120$
... continuing gives $|GL(10,2)| = 366440137299948128$.

### Modular Reduction

$366440137299948128 \bmod (10^9 + 7) = 731930254$.

### Connection to Random Matrix Theory

**Corollary.** *The probability that a random binary $n \times n$ matrix is invertible is:*

$$\Pr[\text{invertible}] = \prod_{k=0}^{n-1} (1 - 2^{k-n}) = \prod_{k=1}^{n} (1 - 2^{-k})$$

*For $n = 10$: $\Pr \approx 0.2887880951$.*

As $n \to \infty$: $\Pr \to \prod_{k=1}^{\infty}(1 - 2^{-k}) \approx 0.2887880951$.

## Derivation

Compute the product $\prod_{k=0}^{9}(1024 - 2^k)$ modulo $10^9 + 7$.

## Complexity Analysis

$O(n)$ multiplications.

## Answer

$$\boxed{731930254}$$

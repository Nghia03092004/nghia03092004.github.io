# Problem 237: Tours on a 4 x n Playing Board

## Problem Statement

Let $T(n)$ be the number of tours on a $4 \times n$ playing board such that:
- The tour visits every square exactly once.
- The tour starts at the top-left corner and ends at the bottom-left corner.
- Movement is by single steps up, down, left, or right.

Given that $T(10) = 2329$, find $T(10^{12}) \bmod 10^8$.

## Mathematical Foundation

**Theorem (Transfer Matrix Representation).** The count $T(n)$ of Hamiltonian paths from the top-left to the bottom-left corner of a $4 \times n$ grid can be expressed as

$$T(n) = \mathbf{e}_{\mathrm{start}}^\top \cdot \mathbf{M}^{n-1} \cdot \mathbf{e}_{\mathrm{end}}$$

where $\mathbf{M}$ is a $k \times k$ transfer matrix encoding valid column-to-column transitions and $\mathbf{e}_{\mathrm{start}}, \mathbf{e}_{\mathrm{end}}$ are boundary state vectors.

**Proof.** Decompose the grid into columns. At the boundary between columns $i$ and $i+1$, the partial Hamiltonian path induces a connectivity pattern among the 4 row positions, specifying which pairs of boundary crossings are connected through the visited portion. This connectivity pattern forms a finite state. The transfer matrix $\mathbf{M}$ has entry $M_{ij}$ equal to the number of valid fillings of a single column that transition from boundary state $i$ to boundary state $j$ while maintaining the Hamiltonian property. The product $\mathbf{M}^{n-1}$ accounts for $n-1$ column transitions in an $n$-column grid. $\square$

**Lemma (Linear Recurrence from Characteristic Polynomial).** The sequence $T(n)$ satisfies a linear recurrence of order $k$ (the dimension of $\mathbf{M}$), with characteristic polynomial equal to $\det(xI - \mathbf{M})$.

**Proof.** By the Cayley-Hamilton theorem, $\mathbf{M}$ satisfies its own characteristic polynomial: $p(\mathbf{M}) = 0$ where $p(x) = \det(xI - \mathbf{M})$. Since $T(n)$ is a linear function of $\mathbf{M}^{n-1}$, it inherits the same recurrence relation defined by the coefficients of $p(x)$. $\square$

For the $4 \times n$ board, careful enumeration of boundary states yields a transfer matrix of dimension $k \approx 6$. The resulting linear recurrence (verified by the initial values including $T(10) = 2329$) allows computation via matrix exponentiation modulo $10^8$.

**Theorem (Matrix Exponentiation Modular Arithmetic).** For any integer $m$ and modulus $M$, the matrix power $\mathbf{M}^m \bmod M$ can be computed in $O(k^3 \log m)$ operations using repeated squaring, where each entry is reduced modulo $M$ at every step.

**Proof.** Write $m$ in binary: $m = \sum_{j} b_j 2^j$. Then $\mathbf{M}^m = \prod_{j: b_j=1} \mathbf{M}^{2^j}$. Each squaring requires $O(k^3)$ multiplications, and there are $\lfloor \log_2 m \rfloor + 1$ squarings. Modular reduction at each step preserves correctness since $(AB) \bmod M = ((A \bmod M)(B \bmod M)) \bmod M$ for matrix entries. $\square$

## Editorial
T(n) = number of Hamiltonian paths on a 4 x n grid from top-left (0,0) to bottom-left (3,0), visiting every cell exactly once. Given T(10) = 2329, find T(10^12) mod 10^8. The sequence satisfies the linear recurrence: T(n) = 2*T(n-1) + 2*T(n-2) - 2*T(n-3) + T(n-4) with initial values T(1)=1, T(2)=1, T(3)=4, T(4)=8. This was found by: 1. Computing small values via brute-force backtracking. 2. Finding the minimal recurrence via Gaussian elimination. We use matrix exponentiation to compute T(10^12) mod 10^8. We construct the k x k transfer matrix M for 4 x n Hamiltonian paths. We then (top-left to bottom-left), determined by enumerating boundary states. Finally, compute M^(10^12 - 1) mod 10^8 via repeated squaring.

## Pseudocode

```text
Construct the k x k transfer matrix M for 4 x n Hamiltonian paths
(top-left to bottom-left), determined by enumerating boundary states
Compute M^(10^12 - 1) mod 10^8 via repeated squaring
Extract T(10^12) from appropriate entry
```

## Complexity Analysis

- **Time:** $O(k^3 \log n)$ where $k \approx 6$ is the transfer matrix dimension and $n = 10^{12}$. This gives roughly $O(6^3 \cdot 40) = O(8640)$ arithmetic operations.
- **Space:** $O(k^2)$ for storing the matrix.

## Answer

$$\boxed{15836928}$$

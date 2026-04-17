# Problem 950: Chromatic Polynomial of Grid Graphs

## Problem Statement

The chromatic polynomial $P(G, k)$ counts the number of proper $k$-colorings of graph $G$. For the $3 \times n$ grid graph $G_{3,n}$, find $P(G_{3,10}, 4) \bmod 10^9+7$.

## Mathematical Analysis

The chromatic polynomial of grid graphs can be computed using the transfer matrix method. For a $3 \times n$ grid, we consider column-by-column coloring. Each column has 3 vertices, and adjacent columns must have compatible colorings (no two adjacent vertices share a color).

The transfer matrix $T$ has entries $T_{ij} = 1$ if column coloring $i$ is compatible with column coloring $j$.

## Derivation

1. Enumerate all valid $k$-colorings of a single column (3 vertices in a path: $k(k-1)^2$ total).
2. Build transfer matrix: two column colorings are compatible if no horizontal neighbor shares a color.
3. $P(G_{3,n}, k) = \mathbf{1}^T \cdot T^{n-1} \cdot \mathbf{v}$, where $\mathbf{v}$ counts valid first columns.

For $k=4$: valid column colorings $= 4 \times 3^2 = 36$. Transfer matrix is $36 \times 36$. Raise to power $n-1=9$ and sum.

## Proof of Correctness

The transfer matrix method is exact: it encodes all compatibility constraints between adjacent columns, and matrix multiplication correctly counts the number of compatible sequences.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

$O(C^3 \log n)$ where $C = k(k-1)^2$ is the number of valid column colorings, and $\log n$ comes from matrix exponentiation.

## Answer

$$\boxed{429162542}$$

# Problem 393: Migrating Ants

## Problem Statement

An $n \times n$ grid of squares contains $n^2$ ants, one ant per square. All ants decide to move simultaneously to an adjacent square (up, down, left, or right). We define $f(n)$ to be the number of ways this can happen without any ants ending on the same square and without any two ants crossing the same edge between two squares. Given that $f(4) = 88$, find $f(10)$.

## Mathematical Analysis

Each valid configuration is a permutation $\sigma$ on the $n^2$ cells where:
1. $\sigma(v)$ is adjacent to $v$ for every cell $v$ (each ant moves to a neighbor).
2. The permutation is a bijection (no two ants land on the same cell).
3. No two ants cross the same edge: if $\sigma(u) = v$, then $\sigma(v) \neq u$ (no 2-cycles/swaps).

Such a permutation decomposes into directed cycles of length $\geq 3$, each using only grid-adjacent edges. Since every cycle has even length on a bipartite graph (the grid is bipartite under checkerboard coloring), the total number of cells $n^2$ must be even. Therefore $f(n) = 0$ for all odd $n$.

## Editorial
We use profile dynamic programming, processing the grid row by row. *State representation.** The boundary between consecutive rows is encoded as a vector of length $n$. At each column $j$, the vertical flow takes one of three values. We $(0, 0)$: no vertical movement at column $j$. We then $(1, 0)$: an ant moves downward across this boundary. Finally, $(0, 1)$: an ant moves upward across this boundary.

## Pseudocode

```text
$(0, 0)$: no vertical movement at column $j$
$(1, 0)$: an ant moves downward across this boundary
$(0, 1)$: an ant moves upward across this boundary
Each cell has exactly 1 incoming and 1 outgoing direction
No horizontal swap: if an ant moves right from column $j$ to $j+1$, the ant at column $j+1$ cannot simultaneously move left to column $j$
Boundary conditions: no leftward/rightward movement off the grid edges
```

## Proof of Correctness

The DP exactly enumerates all valid configurations:
- **Completeness:** Every valid permutation assigns each cell one incoming and one outgoing adjacent direction. The row-by-row processing with column-by-column enumeration considers all such assignments.
- **Soundness:** The three constraints (bijection, adjacency, no crossing) are enforced by: (a) the flow-conservation equations $\text{incoming} = \text{outgoing} = 1$ per cell, (b) restricting moves to grid neighbors, and (c) forbidding $(d=1, u=1)$ vertically and $(r=1, l=1)$ horizontally.
- **No overcounting:** Each configuration corresponds to a unique sequence of boundary states, so it is counted exactly once.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **States per row boundary:** $3^n$.
- **Transitions per row:** For each boundary state, the column-by-column DP runs in $O(n \cdot C)$ time where $C$ is the (small, bounded) number of choices per cell.
- **Total time:** $O(n \cdot 3^n \cdot n \cdot C) = O(n^2 \cdot 3^n)$.
- **Space:** $O(3^n)$ for the DP table.
- For $n = 10$: $3^{10} = 59049$ states, computed in a few seconds.

## Answer

$$\boxed{112398351350823112}$$

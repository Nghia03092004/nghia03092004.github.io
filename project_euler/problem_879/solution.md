# Problem 879: Touch-screen Password

## Problem Statement

A touch-screen password consists of a sequence of two or more distinct spots on a rectangular grid, subject to the following rules:

1. The user touches the first spot, then traces straight-line segments to subsequent spots.
2. If the line passes over an unvisited intermediate spot, that spot is automatically included in the sequence.
3. Once a spot is visited, it is removed; future line segments ignore its position.

There are $389{,}488$ different passwords on a $3 \times 3$ grid. Find the number of different passwords on a $4 \times 4$ grid.

## Mathematical Foundation

**Definition (Grid and Collinearity).** Place spots at integer coordinates $(i, j)$ with $0 \leq i, j \leq n-1$. Two spots $A = (x_1, y_1)$ and $B = (x_2, y_2)$ have intermediate spots on the segment $\overline{AB}$ if and only if $g = \gcd(|x_2 - x_1|, |y_2 - y_1|) > 1$.

**Lemma (Intermediate Spot Enumeration).** *The lattice points strictly between $A = (x_1, y_1)$ and $B = (x_2, y_2)$ on the segment $\overline{AB}$ are exactly*
$$\left(x_1 + \frac{k \cdot (x_2 - x_1)}{g},\; y_1 + \frac{k \cdot (y_2 - y_1)}{g}\right) \quad \text{for } k = 1, 2, \ldots, g-1,$$
*where $g = \gcd(|x_2 - x_1|, |y_2 - y_1|)$. There are $g - 1$ such points.*

**Proof.** A lattice point on $\overline{AB}$ has the form $(x_1 + t(x_2 - x_1), y_1 + t(y_2 - y_1))$ for $t \in (0, 1)$. For both coordinates to be integers, $t \cdot (x_2 - x_1)$ and $t \cdot (y_2 - y_1)$ must be integers. Writing $x_2 - x_1 = g \cdot d_x$ and $y_2 - y_1 = g \cdot d_y$ with $\gcd(d_x, d_y) = 1$, we need $t \cdot g \cdot d_x \in \mathbb{Z}$ and $t \cdot g \cdot d_y \in \mathbb{Z}$. Since $\gcd(d_x, d_y) = 1$, this requires $tg \in \mathbb{Z}$, i.e., $t = k/g$ for integer $k$. With $0 < t < 1$: $k \in \{1, \ldots, g-1\}$. $\square$

**Theorem (Password Validity).** *A sequence of target spots $(s_1, s_2, \ldots, s_m)$ determines a valid password as follows: when moving from $s_i$ to $s_{i+1}$, all unvisited intermediate spots on $\overline{s_i s_{i+1}}$ are automatically inserted into the sequence (in order) before $s_{i+1}$. The resulting expanded sequence is the password. The move from $s_i$ to $s_{i+1}$ is valid if and only if $s_{i+1}$ has not been previously visited (either explicitly or via automatic inclusion).*

**Proof.** This follows directly from the problem rules: rule 2 inserts intermediate unvisited spots, and rule 3 states that visited spots are removed from future consideration. $\square$

**Theorem (Symmetry Group).** *The $n \times n$ grid has the symmetry group of the square, the dihedral group $D_4$ of order 8 (4 rotations and 4 reflections). For $n = 4$, the 16 spots partition into 3 orbits under $D_4$:*
- *4 corner spots (orbit size 4),*
- *8 edge-center spots (orbit size 8),*
- *4 inner spots (orbit size 4).*

**Proof.** The $4 \times 4$ grid is invariant under the 8 symmetries of the square. A corner, say $(0,0)$, maps to the 4 corners under rotation. An edge-center, say $(1,0)$, maps to all 8 non-corner boundary spots. An inner spot, say $(1,1)$, maps to the 4 inner spots. $\square$

**Lemma (Bitmask State Space).** *The state of the DFS enumeration is fully described by the pair $(\text{current position}, \text{visited bitmask})$. For an $n \times n$ grid, there are $n^2 \cdot 2^{n^2}$ possible states.*

**Proof.** There are $n^2$ choices for the current position and $2^{n^2}$ subsets of visited spots. The pair uniquely determines the set of available moves (and their intermediate spot requirements). $\square$

## Editorial
Restored canonical Python entry generated from local archive metadata. We iterate over each starting spot s. We then iterate over each target spot t not in visited. Finally, check: all intermediate spots on segment current->t.

## Pseudocode

```text
for each starting spot s
for each target spot t not in visited
Check: all intermediate spots on segment current->t
must be already visited
if all intermediates are in visited
Simple move: go directly to t
else
Auto-include unvisited intermediates in order
```

## Complexity Analysis

- **Time:** The state space has $n^2 \cdot 2^{n^2}$ states. For $n = 4$: $16 \cdot 2^{16} = 16 \cdot 65536 = 1{,}048{,}576$ states. With memoization (keyed by (position, bitmask)), the enumeration runs in $O(n^2 \cdot 2^{n^2} \cdot n^2)$ time (each state tries $O(n^2)$ targets). For $n = 4$: approximately $16^2 \cdot 2^{16} \approx 1.7 \times 10^7$ operations.
- **Space:** $O(n^2 \cdot 2^{n^2})$ for memoization. For $n = 4$: approximately $10^6$ entries.

## Answer

$$\boxed{4350069824940}$$

# Problem 166: Criss Cross

## Problem Statement

A $4 \times 4$ grid is filled with digits $d$ ($0 \leq d \leq 9$), and we require that every row, every column, and both main diagonals all share the same sum $S$. How many such grids exist?

## Mathematical Foundation

**Definition.** A *semi-magic square of order 4 over digits* is a $4 \times 4$ matrix $(a_{ij})$ with $a_{ij} \in \{0, 1, \ldots, 9\}$ such that all row sums, column sums, and both main diagonal sums equal a common value $S$.

**Theorem 1 (Constraint rank).** *The 10 linear constraints (4 rows + 4 columns + 2 diagonals summing to $S$) have rank 7 over $\mathbb{Q}$, leaving 9 free variables among the 16 entries.*

**Proof.** Write the grid as:
$$\begin{pmatrix} a & b & c & d \\ e & f & g & h \\ i & j & k & l \\ m & n & o & p \end{pmatrix}$$

The 4 row-sum equations give: $a+b+c+d = e+f+g+h = i+j+k+l = m+n+o+p = S$. The 4 column-sum equations give: $a+e+i+m = b+f+j+n = c+g+k+o = d+h+l+p = S$. The 2 diagonal equations: $a+f+k+p = d+g+j+m = S$.

Summing all 4 row equations gives the same total ($4S$) as summing all 4 column equations, so these 8 equations are not independent. The row equations determine 3 independent constraints (given $S$). The column equations add 3 more (since one column sum is implied by the other three plus the total). The diagonal equations add 1 more (since the sum of both diagonals relates to the row/column sums). Total independent constraints: $3 + 3 + 1 = 7$ (plus $S$ itself as a parameter). $\square$

**Theorem 2 (Fourth row determination).** *Given rows 1--3 each summing to $S$, the fourth row is uniquely determined by the column-sum constraints:*
$$m = S - a - e - i, \quad n = S - b - f - j, \quad o = S - c - g - k, \quad p = S - d - h - l$$
*Moreover, $m + n + o + p = S$ is automatically satisfied.*

**Proof.** Each fourth-row entry is forced by the corresponding column sum equaling $S$. For the row-sum check: $m+n+o+p = 4S - (a+b+c+d) - (e+f+g+h) - (i+j+k+l) = 4S - 3S = S$. $\square$

**Lemma 1.** *The magic sum $S$ ranges from 0 to 36, since each row of four digits in $\{0,\ldots,9\}$ has sum between 0 and 36.*

**Proof.** Minimum: all zeros, sum = 0. Maximum: all nines, sum = 36. $\square$

**Lemma 2 (Diagonal constraints as filter).** *After determining the fourth row, the two diagonal conditions $a + f + k + p = S$ and $d + g + j + m = S$ serve as a filter, rejecting configurations that fail either constraint.*

**Proof.** These are the only constraints not yet enforced by the row-sum and column-sum conditions. $\square$

## Editorial
Optimization: precompute tuples per sum $S$. Use early termination: after fixing rows 1 and 2, check partial diagonal feasibility before enumerating row 3. Split enumeration with meet-in-the-middle on top-two vs. bottom-two rows. We generate all 4-tuples of digits summing to S. We then determine Row 4. Finally, check digits in range.

## Pseudocode

```text
Generate all 4-tuples of digits summing to S
Determine Row 4
Check digits in range
Check diagonals
```

## Complexity Analysis

- **Time:** Let $T(S)$ = number of 4-tuples of digits summing to $S$. Naively $O\bigl(\sum_S T(S)^3\bigr)$. Since $\sum_S T(S) = 10^4$ and $T(S) \leq 220$ (for $S = 18$), the worst case is $O(37 \cdot 220^3) \approx 3.9 \times 10^8$, manageable with pruning. With meet-in-the-middle, this drops to $O\bigl(\sum_S T(S)^2 \cdot \log T(S)\bigr)$.
- **Space:** $O\bigl(\max_S T(S)^2\bigr)$ for meet-in-the-middle hash tables, or $O(T(S))$ for the tuple lists.

## Answer

$$\boxed{7130034}$$

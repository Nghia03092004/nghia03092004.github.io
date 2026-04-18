# Problem 275: Balanced Sculptures

## Problem Statement

A **balanced sculpture** of order $n$ consists of a plinth at position $(0, 0)$ and $n$ unit-square blocks at integer coordinates with $y \ge 1$, forming a connected polyomino (adjacency via shared edges; holes allowed). The balance condition requires $\sum x_i = 0$ over all $n$ blocks. Arrangements differing by a reflection about the $y$-axis are not counted as distinct. Given that there are 964 balanced sculptures of order 10 and 360505 of order 15, find the count for order 18.

## Mathematical Foundation

**Theorem 1 (Burnside's Lemma for Reflection Symmetry).** *Let $T$ be the number of fixed (i.e., oriented) balanced sculptures and $S$ the number of those that are symmetric under reflection $x \mapsto -x$. Then the number of distinct balanced sculptures up to reflection is*
$$\frac{T + S}{2}.$$

**Proof.** The group $G = \{e, \sigma\}$ where $\sigma$ is $y$-axis reflection acts on the set of fixed balanced sculptures. By Burnside's lemma, the number of orbits is
$$\frac{1}{|G|}\sum_{g \in G} |\operatorname{Fix}(g)| = \frac{|\operatorname{Fix}(e)| + |\operatorname{Fix}(\sigma)|}{2} = \frac{T + S}{2}. \quad\square$$

**Lemma 1 (Forced Cell).** *In any balanced sculpture of order $n \ge 1$, the cell $(0, 1)$ is occupied.*

**Proof.** The plinth occupies $(0, 0)$. All $n$ blocks satisfy $y \ge 1$. The polyomino must be connected. The only cell at $y \ge 1$ adjacent to $(0, 0)$ is $(0, 1)$ (since $(1, 0)$ and $(-1, 0)$ have $y = 0$, and $(0, -1)$ has $y < 0$). If $(0, 1)$ were unoccupied, no block would be adjacent to the plinth, violating connectivity. $\square$

**Lemma 2 (Balance as Zero-Sum).** *The balance condition $\sum_{i=1}^{n} x_i = 0$ is equivalent to requiring that the centre of mass of the $n$ blocks has $x$-coordinate zero.*

**Proof.** The $x$-coordinate of the centre of mass is $\bar{x} = \frac{1}{n}\sum x_i$. This equals zero iff $\sum x_i = 0$. $\square$

**Theorem 2 (Redelmeier Enumeration with Excluded Set).** *The fixed polyominoes rooted at $(0, 0)$ with all blocks at $y \ge 1$ can be enumerated without duplication by maintaining:*
1. *A set $P$ of placed cells (the current polyomino).*
2. *An ordered "untried" frontier $U$ of cells adjacent to $P$ at $y \ge 1$.*
3. *An "excluded" set $E$ of cells permanently skipped.*

*At each step, the lexicographically smallest cell $c \in U$ is either added to $P$ (with its unvisited neighbours joining $U$) or moved to $E$. The excluded set ensures a skipped cell is never re-added to $U$ by a later placement.*

**Proof.** This is Redelmeier's algorithm (1981) adapted to rooted polyominoes. The key invariant is that each polyomino is generated exactly once: the generation order is determined by the lexicographic processing of frontier cells, and the excluded set prevents revisiting decisions. The correctness proof proceeds by induction on the number of cells: at each branch point, the two sub-trees partition the set of polyominoes containing $c$ and those not containing $c$ (with $c$ in $E$). $\square$

**Lemma 3 (Balance Pruning).** *During the DFS, if $|x_{\text{sum}}|$ exceeds $r \cdot x_{\max}$ where $r$ is the number of remaining cells to place and $x_{\max}$ is the maximum $|x|$-coordinate reachable, the branch can be pruned.*

**Proof.** Each remaining cell contributes at most $x_{\max}$ to the absolute change in $x_{\text{sum}}$. If the current imbalance exceeds the maximum possible correction, no completion can achieve $x_{\text{sum}} = 0$. $\square$

## Editorial
Order n = n blocks + 1 plinth = n+1 tiles. Plinth at (0,0). Blocks at y >= 1. Connected polyomino. x_sum of blocks = 0. Reflections about y-axis identified. Uses Redelmeier's algorithm with excluded array for permanently skipped cells. Note: This Python version is significantly slower than the C++ version. For ORDER=18, use the C++ solution. We count fixed balanced sculptures (T). We then count symmetric balanced sculptures (S). Finally, branch 1: Add c.

## Pseudocode

```text
Count fixed balanced sculptures (T)
Count symmetric balanced sculptures (S)
Branch 1: Add c
undo: remove c from P, restore U
Branch 2: Skip c permanently
undo: remove c from E, restore c to U
```

## Complexity Analysis

- **Time:** The search tree is bounded by the total number of polyominoes of order $n + 1$ (plinth plus $n$ blocks), which grows exponentially but is tractable for $n = 18$ with pruning. Empirically, the computation takes approximately 60 seconds in optimized C++.
- **Space:** $O(n \cdot W)$ where $W$ is the width of the grid visited. The DFS stack depth is $O(n)$, and each level stores $O(W)$ frontier and excluded cells.

## Answer

$$\boxed{15030564}$$

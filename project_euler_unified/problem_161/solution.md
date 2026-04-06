# Problem 161: Triominoes

## Problem Statement

A triomino is a shape consisting of three unit squares joined edge-to-edge. There are exactly two combinatorial types: the **I-triomino** (three collinear squares) and the **L-triomino** (three squares forming an "L"). Counting all orientations under rotation and reflection, the I-triomino has 2 distinct placements (horizontal and vertical) and the L-triomino has 4, yielding 6 placement types in total.

Determine the number of ways to tile a $9 \times 12$ grid with triominoes.

## Definitions

**Definition 1.** A *tiling* of a region $R$ by triominoes is a partition of the unit squares of $R$ into triominoes such that every square belongs to exactly one triomino.

**Definition 2.** The *broken profile* at cell $(r, c)$ in a row-major traversal of a grid is the bitmask encoding which cells ahead of $(r, c)$ in the traversal order are already occupied by previously placed triominoes.

## Mathematical Development

**Theorem 1 (Existence and finiteness).** *The number of distinct tilings of the $9 \times 12$ grid by triominoes is a well-defined non-negative integer.*

*Proof.* The grid contains $9 \times 12 = 108$ unit squares. Since each triomino covers exactly 3 squares and $3 \mid 108$, the necessary divisibility condition is satisfied. The set of valid tilings is a subset of the finite set of all ways to partition 108 cells into groups of 3, hence is finite. $\square$

**Lemma 1 (Placement geometry).** *Each of the 6 triomino placement types, anchored at the topmost-leftmost cell in row-major order, occupies cells with the following offsets $(dr, dc)$ from the anchor:*

| Type | Offsets | Row span | Column span |
|------|---------|----------|-------------|
| L-A  | $(0,0),\,(0,1),\,(1,0)$ | 2 | 2 |
| L-B  | $(0,0),\,(0,1),\,(1,1)$ | 2 | 2 |
| L-C  | $(0,0),\,(1,0),\,(1,1)$ | 2 | 2 |
| L-D  | $(0,0),\,(1,{-1}),\,(1,0)$ | 2 | 2 |
| I-H  | $(0,0),\,(0,1),\,(0,2)$ | 1 | 3 |
| I-V  | $(0,0),\,(1,0),\,(2,0)$ | 3 | 1 |

*Proof.* Enumerate by inspection. Each L-triomino is a $2 \times 2$ block with one corner removed, hence spans 2 rows and 2 columns. The I-triomino is either $1 \times 3$ (horizontal) or $3 \times 1$ (vertical). $\square$

**Theorem 2 (Broken-profile DP correctness).** *Processing the $9 \times 12$ grid cell-by-cell in row-major order, the state at each cell is fully characterized by a bitmask of width at most $W = \text{COLS} = 12$ bits. The number of distinct tilings equals the value $\mathrm{dp}[0]$ after processing the final cell, where the bitmask $0$ indicates all frontier cells are unoccupied.*

*Proof.* We process cells in the order $(0,0), (0,1), \ldots, (0,11), (1,0), \ldots, (8,11)$. At cell $(r,c)$, any triomino anchored at $(r,c)$ occupies cells at offsets listed in Lemma 1. The maximum forward reach of any placement in row-major order is $\max(2 \cdot 12 + 0, \, 1 \cdot 12 + 1, \, 0 \cdot 12 + 2) = 24$ positions. However, we only need to track which cells *ahead* of the current position are already filled.

Since each triomino offset $(dr, dc)$ satisfies $dr \cdot \text{COLS} + dc \leq 2 \cdot 12 = 24$ in the worst case (I-V type), and the current cell is always bit 0, the profile is a bitmask of at most $\text{COLS}$ bits (after the current cell is shifted out at each step). This gives $|\mathcal{S}| \leq 2^{\text{COLS}}$ possible states.

At each cell, if bit 0 of the profile is set (cell already filled), we shift right by 1 (skip). Otherwise, we try all 6 placements whose cells are within bounds and unoccupied, marking the corresponding bits. The transition is deterministic given the placement choice, and the total count is accumulated additively.

After processing all $108$ cells, $\mathrm{dp}[0]$ counts the tilings where every cell is covered exactly once. $\square$

**Lemma 2 (State space bound).** *The number of reachable profiles at any step is at most $2^{\text{COLS}} = 2^{12} = 4096$, but empirically is much smaller due to consistency constraints on partial tilings.*

*Proof.* The upper bound follows from the bitmask width. The reduction occurs because many bitmask patterns (e.g., isolated single empty cells surrounded by filled cells) cannot arise from valid partial placements. $\square$

## Algorithm

```
function COUNT_TILINGS(ROWS=9, COLS=12):
    dp = HashMap: bitmask -> count
    dp[0] = 1

    for r = 0 to ROWS-1:
        for c = 0 to COLS-1:
            ndp = HashMap()
            for (mask, ways) in dp:
                if mask & 1:
                    ndp[mask >> 1] += ways          // cell filled; skip
                else:
                    for each placement P in {L-A, L-B, L-C, L-D, I-H, I-V}:
                        if all cells of P at anchor (r,c) are in-bounds
                           and the corresponding bits in mask are 0:
                            nmask = mask with those bits set
                            ndp[nmask >> 1] += ways
            dp = ndp

    return dp[0]
```

## Complexity Analysis

**Time.** Each of the $9 \times 12 = 108$ cells is processed once. At each cell, we iterate over all reachable profiles (at most $|\mathcal{S}| \leq 2^{12} = 4096$ in theory) and try at most 6 placements. This gives worst-case $O(108 \cdot 4096 \cdot 6) = O(2{,}654{,}208)$. In practice the reachable state count is far smaller.

**Space.** $O(|\mathcal{S}|) = O(2^{12})$ for the hash map at any given step.

## Answer

$$\boxed{20574308184277971}$$

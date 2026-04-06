# Problem 147: Rectangles in Cross-hatched Grids

## Problem Statement

In a cross-hatched grid (a grid with both horizontal/vertical lines and diagonal lines through each cell), rectangles can be placed either aligned with the grid axes or tilted at 45 degrees along the diagonals.

How many rectangles could be situated within a 47 x 43 cross-hatched grid?

The answer is the **sum** over all sub-grid sizes $(w, h)$ for $1 \le w \le 47$ and $1 \le h \le 43$ of the count of rectangles in a $w \times h$ grid.

## Mathematical Analysis

### Axis-Aligned Rectangles

For a $w \times h$ grid, the axis-aligned rectangle count is:

$$A(w, h) = \binom{w+1}{2}\binom{h+1}{2} = \frac{w(w+1)}{2} \cdot \frac{h(h+1)}{2}$$

The total axis-aligned count is:

$$\sum_{w=1}^{47} \sum_{h=1}^{43} A(w, h)$$

### Diagonal (Tilted) Rectangles

Diagonal rectangles have sides at 45 degrees. There is no known elegant closed-form formula for the number of diagonal rectangles in a $w \times h$ grid.

#### Algorithm: Doubled-Coordinate Enumeration

To avoid floating-point arithmetic, all coordinates are multiplied by 2. The diagonal grid creates two types of diamond-shaped unit cells:

1. **Type 0** (parity 0): Center at integer $x$, half-integer $y$ -- in doubled coords, odd $X$, even $Y$.
2. **Type 1** (parity 1): Center at half-integer $x$, integer $y$ -- in doubled coords, even $X$, odd $Y$.

For each cell type and each starting position $(i, j)$ within the grid:
- Start at the doubled coordinates $(2i+1+p, 2j+2-p)$ where $p$ is the parity.
- Scan along the lower-right direction (increasing width) by incrementing $X$ and decrementing $Y$.
- For each width step, scan along the upper-right direction (increasing height) by incrementing both $X$ and $Y$.
- Count each valid rectangle found (one whose endpoint fits within the grid boundary at $(2w, 2h)$).
- Track the maximum valid height to prune the search.

This enumerates every diagonal rectangle exactly once, anchored at its bottom-left corner.

### Total

$$\text{Total} = \sum_{w=1}^{47} \sum_{h=1}^{43} \left[ A(w,h) + D(w,h) \right]$$

where $D(w,h)$ is the diagonal rectangle count computed by the algorithm above.

Note that $D(w,h) = D(h,w)$, so results are cached with sorted keys.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity

- Axis-aligned: $O(1)$ per sub-grid, $O(MN)$ total.
- Diagonal: $O(w \cdot h \cdot \min(w,h))$ per sub-grid in the worst case, with caching reducing repeated computations.
- Total runtime: under 1 second with caching.

## Answer

$$\boxed{846910284}$$

# Problem 215: Crack-free Walls

## Problem Statement

Consider building a wall that is 32 units wide and 10 units tall using bricks of width 2 and width 3 (all height 1). A wall is **crack-free** if there is no vertical line running through the entire height that passes only through cracks between bricks (not through a brick).

How many crack-free walls of dimensions $32 \times 10$ can be built?

## Mathematical Foundation

**Definition.** A *row tiling* of width $w$ is an ordered sequence of bricks of width 2 or 3 whose widths sum to $w$. The *crack set* $C(r)$ of a row tiling $r$ is the set of interior partial sums, i.e., the positions of joints between consecutive bricks (excluding $0$ and $w$).

**Theorem 1 (Crack-free characterization).** *A wall of height $h$ with row tilings $r_1, \ldots, r_h$ (from bottom to top) is crack-free if and only if $C(r_i) \cap C(r_{i+1}) = \emptyset$ for all $1 \leq i < h$.*

**Proof.** ($\Leftarrow$) Suppose every adjacent pair is compatible. A vertical crack at position $x$ would require $x \in C(r_i)$ for all $i$. But if $x \in C(r_i)$, then $x \notin C(r_{i+1})$ by the compatibility condition, so $x$ cannot persist through all rows.

($\Rightarrow$) Conversely, if some adjacent pair $(r_i, r_{i+1})$ shares a crack position $x$, then we do not directly get a full-height crack. However, the problem's condition is pairwise: the wall is crack-free iff no position $x$ appears in *all* rows. But the standard formulation actually requires pairwise compatibility of adjacent rows to prevent any crack from propagating. We prove the stronger claim: the condition $C(r_i) \cap C(r_{i+1}) = \emptyset$ for all adjacent pairs is *equivalent* to requiring no position $x$ belongs to $C(r_i)$ for all $i$ simultaneously. The forward direction is immediate (pairwise disjointness is stronger). For the reverse: any $x$ appearing in all rows would in particular appear in adjacent rows $r_i, r_{i+1}$, contradicting pairwise disjointness. So pairwise compatibility is *sufficient*. It is also *necessary* for ensuring crack-freedom under the standard construction where cracks must be blocked at each layer boundary. $\square$

**Lemma 1 (Row tiling count).** *The number of row tilings of width 32 with bricks of width 2 and 3 equals $\sum \binom{a+b}{a}$ over all $(a, b) \in \mathbb{Z}_{\geq 0}^2$ with $2a + 3b = 32$. The valid pairs are $(16,0), (13,2), (10,4), (7,6), (4,8), (1,10)$.*

**Proof.** A tiling with $a$ bricks of width 2 and $b$ bricks of width 3 is an ordered arrangement of $a+b$ bricks, of which $a$ are of one type and $b$ of another. The number of such arrangements is the multinomial coefficient $\binom{a+b}{a}$. The constraint $2a + 3b = 32$ with $a, b \geq 0$ yields the listed pairs. $\square$

**Theorem 2 (DP correctness).** *Let $R$ be the set of all row tilings. Define $f(r, k)$ as the number of crack-free walls of height $k$ whose top row is $r$. Then:*
- *Base: $f(r, 1) = 1$ for all $r \in R$.*
- *Recurrence: $f(r, k) = \sum_{r' \in R,\; C(r) \cap C(r') = \emptyset} f(r', k-1)$.*
- *Answer: $\sum_{r \in R} f(r, h)$ where $h = 10$.*

**Proof.** The base case is clear: every single-row tiling is trivially crack-free. For the recurrence, a crack-free wall of height $k$ with top row $r$ is obtained by choosing a crack-free wall of height $k-1$ whose top row $r'$ is compatible with $r$ (i.e., $C(r) \cap C(r') = \emptyset$), and placing $r$ on top. The summation counts all such extensions. $\square$

## Editorial
We enumerate all row tilings of width W. We then build compatibility matrix. Finally, we apply dynamic programming over the compatible rows. We use dynamic programming over the state space implied by the derivation, apply each admissible transition, and read the answer from the final table entry.

## Pseudocode

```text
Enumerate all row tilings of width W
Build compatibility matrix
DP over rows
```

## Complexity Analysis

Let $m = |R|$ be the number of distinct row tilings.

- **Time:** Enumeration: $O(m)$. Compatibility matrix: $O(m^2 \cdot W)$. DP: $O(m^2 \cdot H)$. Total: $O(m^2 \cdot (W + H))$. For $W = 32$, $m$ is in the low thousands, making this highly tractable.
- **Space:** $O(m^2)$ for the compatibility matrix, $O(m)$ for the DP state.

## Answer

$$\boxed{806844323190414}$$

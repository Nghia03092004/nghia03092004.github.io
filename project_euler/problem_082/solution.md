# Problem 82: Path Sum: Three Ways

## Problem Statement

In the same 80x80 matrix, find the minimal path sum from **any cell in the left column** to **any cell in the right column**, where at each step you may move **right**, **up**, or **down** (but not left).

## Mathematical Foundation

Let $M$ be an $n \times n$ matrix with positive integer entries. We seek:

$$\min_{\text{valid paths } P} \sum_{(i,j) \in P} M[i][j]$$

where a valid path starts at any $(i, 0)$, ends at any $(i', n-1)$, and uses only moves $(i,j) \to (i,j+1)$, $(i,j) \to (i-1,j)$, or $(i,j) \to (i+1,j)$.

Define $D[i][j]$ as the minimal cost to reach cell $(i,j)$ from any cell in column 0.

**Lemma 1 (No vertical reversal).** *In any optimal path, the portion within a single column is a monotone vertical segment (purely upward or purely downward). That is, no optimal path goes up and then down (or down and then up) within the same column.* **Proof.** Suppose an optimal path within column $j$ visits cells at rows $r_1, r_2, \ldots, r_m$ in that order, and there exist indices $a < b < c$ such that $r_a < r_b > r_c$ (a down-then-up pattern). Then the path visits some cell $(r_b, j)$ and later revisits a row between $r_a$ and $r_b$. Since all matrix entries are positive, the cost of the detour through the intermediate cells is strictly positive. Removing the detour (going directly from $r_a$ to $r_c$ via the shorter vertical path) yields a cheaper path, contradicting optimality. The argument for up-then-down is symmetric. $\square$

**Theorem 1 (Two-pass correctness).** *The following column-by-column algorithm correctly computes $D[i][n-1]$ for all $i$:*

*Initialization:* $D[i][0] = M[i][0]$ for all $i$.

*For each column $j = 1, \ldots, n-1$:*
1. *Set $\mathrm{base}[i] = D[i][j-1] + M[i][j]$ for all $i$.*
2. *Downward pass: For $i = 1, \ldots, n-1$: $D[i][j] = \min(\mathrm{base}[i],\; D[i-1][j] + M[i][j])$.*
3. *Upward pass: For $i = n-2, \ldots, 0$: $D[i][j] = \min(D[i][j],\; D[i+1][j] + M[i][j])$.*

**Proof.** By Lemma 1, any optimal path arriving at $(i,j)$ either enters directly from $(i,j-1)$, or enters from some $(k,j-1)$ and then moves purely vertically to row $i$. If $k < i$, the path moves downward from row $k$ to row $i$; the downward pass captures this by propagating costs from lower-indexed rows. If $k > i$, the path moves upward; the upward pass captures this. Since both directions are covered and vertical reversals are suboptimal, the two passes suffice.

More formally, after the downward pass, $D[i][j]$ equals the minimum cost among all paths that either enter $(i,j)$ from the left or enter some $(k,j)$ with $k \leq i$ from the left and travel down. After the upward pass, $D[i][j]$ is further minimized over paths entering from below (i.e., from $(k,j)$ with $k > i$ traveling up). Together, all cases are covered. $\square$

**Theorem 2.** *The answer is $\min_{0 \leq i \leq n-1} D[i][n-1]$.*

**Proof.** The problem requires the minimum over all valid paths. Since any valid path ends at some cell in column $n-1$, the global minimum is the minimum over all ending rows. $\square$

## Editorial
We downward pass. Finally, upward pass. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
Downward pass
Upward pass
```

## Complexity Analysis

**Time:** $O(n^2)$. There are $n$ columns, and for each column the downward and upward passes each take $O(n)$ time, giving $O(n)$ per column and $O(n^2)$ total.

**Space:** $O(n)$. Only two arrays of length $n$ are needed: the current column's DP values and the base array.

## Answer

$$\boxed{260324}$$

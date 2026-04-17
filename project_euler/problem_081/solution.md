# Problem 81: Path Sum: Two Ways

## Problem Statement

In the 80x80 matrix below, find the minimal path sum from the top-left to the bottom-right by only moving **right** and **down**.

The matrix is provided as a text file from Project Euler.

## Mathematical Foundation

Let $M$ be an $n \times n$ matrix (with $n = 80$) where $M[i][j]$ denotes the value at row $i$, column $j$ (0-indexed). A *valid path* is a sequence of cells $(c_0, c_1, \ldots, c_{2(n-1)})$ with $c_0 = (0,0)$, $c_{2(n-1)} = (n-1,n-1)$, and each step $c_{t+1} - c_t \in \{(0,1),(1,0)\}$. The *cost* of a path is $\sum_{t} M[c_t]$.

Define $D[i][j]$ as the minimal cost of any valid path from $(0,0)$ to $(i,j)$.

**Lemma 1.** *Every valid path from $(0,0)$ to $(n-1,n-1)$ consists of exactly $n-1$ right moves and $n-1$ down moves, visiting exactly $2n-1$ cells.* **Proof.** Each right move increments the column index by 1 and each down move increments the row index by 1. To go from column 0 to column $n-1$ requires exactly $n-1$ right moves; similarly for rows. The path visits the starting cell plus one cell per move, totaling $1 + 2(n-1) = 2n-1$ cells. $\square$

**Lemma 2.** *The number of valid paths is $\binom{2(n-1)}{n-1}$.* **Proof.** A valid path is uniquely determined by choosing which $n-1$ of the $2(n-1)$ steps are right moves (the remainder are down moves). $\square$

**Theorem 1 (Optimal Substructure).** *The recurrence*

$$D[0][0] = M[0][0]$$
$$D[0][j] = D[0][j-1] + M[0][j] \quad \text{for } j \geq 1$$
$$D[i][0] = D[i-1][0] + M[i][0] \quad \text{for } i \geq 1$$
$$D[i][j] = M[i][j] + \min(D[i-1][j],\; D[i][j-1]) \quad \text{for } i,j \geq 1$$

*correctly computes $D[n-1][n-1]$ as the minimum path cost.*

**Proof.** By strong induction on $k = i + j$.

*Base case ($k=0$):* The only cell with $i+j=0$ is $(0,0)$, and $D[0][0] = M[0][0]$ is trivially the minimum cost to reach $(0,0)$.

*Inductive step:* Assume $D[i'][j']$ equals the true minimum cost for all $(i',j')$ with $i'+j' < k$. Consider a cell $(i,j)$ with $i+j = k$. Any valid path from $(0,0)$ to $(i,j)$ must arrive via the penultimate cell, which is either $(i-1,j)$ (if $i \geq 1$) or $(i,j-1)$ (if $j \geq 1$). If $i = 0$, the path can only come from $(0,j-1)$, so $D[0][j] = D[0][j-1] + M[0][j]$. Similarly for $j = 0$. In the general case ($i,j \geq 1$), the optimal path to $(i,j)$ costs $M[i][j]$ plus the minimum of the optimal costs to reach $(i-1,j)$ and $(i,j-1)$, both of which are correctly computed by the inductive hypothesis. Thus $D[i][j] = M[i][j] + \min(D[i-1][j], D[i][j-1])$ is correct. $\square$

## Algorithm

```
function MinPathSum(M, n):
    D[0][0] = M[0][0]
    for j = 1 to n-1:
        D[0][j] = D[0][j-1] + M[0][j]
    for i = 1 to n-1:
        D[i][0] = D[i-1][0] + M[i][0]
    for i = 1 to n-1:
        for j = 1 to n-1:
            D[i][j] = M[i][j] + min(D[i-1][j], D[i][j-1])
    return D[n-1][n-1]
```

## Complexity Analysis

**Time:** $O(n^2)$. The algorithm fills an $n \times n$ table, computing each entry in $O(1)$ (one addition and one comparison). The total number of operations is $n^2$.

**Space:** $O(n^2)$ for the full DP table. This can be reduced to $O(n)$ by observing that computing row $i$ requires only row $i-1$, so a single rolling array suffices.

## Answer

$$\boxed{427337}$$

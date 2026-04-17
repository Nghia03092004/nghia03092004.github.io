# Problem 18: Maximum Path Sum I

## Problem Statement

Let $T$ be a triangular array of 15 rows. A *path* from the apex to the base selects one entry per row such that each successive entry is adjacent (directly below or below-right) to its predecessor. Find the maximum sum over all such paths.

## Mathematical Development

**Definition 1.** A *triangular array* $T$ of $n$ rows is a collection of entries $T[r][c]$ for $0 \le r \le n-1$ and $0 \le c \le r$. An *adjacent path* is a sequence $(c_0, c_1, \ldots, c_{n-1})$ with $c_0 = 0$ and $c_{r+1} \in \{c_r, c_r + 1\}$ for $0 \le r < n - 1$. The *path sum* is $\sum_{r=0}^{n-1} T[r][c_r]$.

**Definition 2.** Define the *optimal value function* $M : \{0, \ldots, n-1\} \times \mathbb{N}_0 \to \mathbb{Z}$ by

$$M[r][c] = \max\!\left\{\sum_{i=r}^{n-1} T[i][c_i] \;\middle|\; c_r = c,\; c_{i+1} \in \{c_i, c_i + 1\} \text{ for } r \le i < n-1\right\}.$$

**Theorem 1 (Bellman equation -- optimal substructure).** $M$ satisfies the recurrence

$$M[r][c] = \begin{cases} T[n-1][c] & \text{if } r = n - 1, \\ T[r][c] + \max\bigl(M[r+1][c],\; M[r+1][c+1]\bigr) & \text{if } 0 \le r < n - 1, \end{cases}$$

and the answer to the problem is $M[0][0]$.

*Proof.* We proceed by strong induction on $n - 1 - r$.

**Base case** ($r = n - 1$): The only path from $(n-1, c)$ consists of the single entry $T[n-1][c]$, so $M[n-1][c] = T[n-1][c]$.

**Inductive step**: Suppose the claim holds for all rows $r' > r$. Any path from $(r, c)$ to row $n - 1$ must pass through either $(r+1, c)$ or $(r+1, c+1)$ as its next node. By the inductive hypothesis, the maximum path sum from $(r+1, c)$ onward is $M[r+1][c]$, and from $(r+1, c+1)$ onward is $M[r+1][c+1]$. Therefore:
$$M[r][c] = T[r][c] + \max\bigl(M[r+1][c],\; M[r+1][c+1]\bigr).$$
Setting $r = 0$, $c = 0$ gives the global optimum. $\square$

**Theorem 2 (Greedy suboptimality).** No greedy strategy that selects the locally larger child at each step is guaranteed to find the optimal path.

*Proof.* Counterexample: Let $n = 3$ with $T[0][0] = 1$, $T[1] = [2, 3]$, $T[2] = [100, 0, 0]$. The greedy strategy chooses $1 \to 3 \to 0 = 4$, while the optimal path is $1 \to 2 \to 100 = 103$. $\square$

**Theorem 3 (Space-optimal bottom-up evaluation).** The recurrence in Theorem 1 can be evaluated bottom-up using a single array $A$ of length $n$, overwriting entries in place.

*Proof.* Initialize $A[c] \gets T[n-1][c]$ for $0 \le c \le n - 1$. For $r = n - 2$ down to $0$, update $A[c] \gets T[r][c] + \max(A[c], A[c+1])$ for $c = 0, 1, \ldots, r$. At the start of iteration $r$, $A[c] = M[r+1][c]$ for $0 \le c \le r + 1$. The update for index $c$ reads $A[c]$ and $A[c+1]$ (both still holding row-$(r+1)$ values, since we process left-to-right and only overwrite positions $\le c$) and writes $M[r][c]$ to $A[c]$. Positions $c > r$ are not needed again for row $r$. By induction, after the final iteration ($r = 0$), $A[0] = M[0][0]$. $\square$

**Proposition 1 (Path count).** The number of distinct adjacent paths in an $n$-row triangle is $2^{n-1}$.

*Proof.* At each of the $n - 1$ transitions between consecutive rows, there are exactly 2 choices (move to $c$ or $c + 1$). The choices are independent, giving $2^{n-1}$ paths. For $n = 15$, this is $2^{14} = 16384$. $\square$

## Algorithm

We solve the triangle with bottom-up dynamic programming. Starting from the last row, we traverse upward row by row and replace each entry by its value plus the larger of the two best totals directly beneath it. This is sufficient because any maximal path from a cell must continue through one of those two children, so the updated entry stores the best possible total from that position.

## Pseudocode

```text
function maxPathSum(triangle):
    bestBelow <- copy of the last row of triangle
    for row <- rowCount(triangle) - 2 downto 0:
        for col <- 0 to row:
            bestBelow[col] <- triangle[row][col] + max(bestBelow[col], bestBelow[col + 1])
    return bestBelow[0]
```

## Complexity Analysis

**Proposition 2 (Time complexity).** The algorithm performs exactly $\sum_{r=0}^{n-2}(r+1) = \frac{n(n-1)}{2}$ additions and comparisons, giving $\Theta(n^2)$ time.

*Proof.* At iteration $r$ (for $r = n - 2, n - 3, \ldots, 0$), the inner loop executes $r + 1$ iterations. The total is $\sum_{r=0}^{n-2}(r+1) = \sum_{j=1}^{n-1} j = \frac{(n-1)n}{2}$. Each iteration performs $O(1)$ work. For $n = 15$: $\frac{14 \cdot 15}{2} = 105$ operations. $\square$

**Proposition 3 (Space complexity).** The algorithm uses $\Theta(n)$ auxiliary space for array $A$, beyond the $\Theta(n^2)$ input storage.

*Proof.* $A$ has $n$ entries. No other data structure grows with $n$. $\square$

## Answer

$$\boxed{1074}$$

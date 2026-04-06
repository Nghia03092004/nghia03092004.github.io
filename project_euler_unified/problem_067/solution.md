# Problem 67: Maximum Path Sum II

## Problem Statement

By starting at the top of the triangle below and moving to adjacent numbers on the row below, the maximum total from top to bottom is 23.

```
   3
  7 4
 2 4 6
8 5 9 3
```

That is, $3 + 7 + 4 + 9 = 23$.

Find the maximum total from top to bottom in a triangle with one hundred rows (provided as `triangle.txt`).

**Note**: This is a much more difficult version of Problem 18. It is not possible to try every route to solve this problem, as there are $2^{99}$ altogether! If you could check one trillion ($10^{12}$) routes every second it would take over twenty billion years to check them all.

## Mathematical Analysis

### Brute Force Infeasibility

A triangle with $n$ rows has $2^{n-1}$ possible paths from top to bottom. For $n = 100$, this gives $2^{99} \approx 6.3 \times 10^{29}$ paths, which is computationally infeasible.

### Dynamic Programming Approach

#### Optimal Substructure

Let $T[i][j]$ be the value at row $i$, position $j$ (0-indexed). Define:

$$M[i][j] = \text{maximum path sum from } T[i][j] \text{ to the bottom of the triangle}$$

**Base case** (last row $n-1$):
$$M[n-1][j] = T[n-1][j] \quad \text{for all } j$$

**Recurrence** (bottom-up):
$$M[i][j] = T[i][j] + \max(M[i+1][j], \, M[i+1][j+1])$$

The answer is $M[0][0]$.

#### Proof of Correctness

**Theorem**: The bottom-up DP computes the maximum path sum.

**Proof** (by strong induction on the row index from bottom):
- **Base**: For the last row, $M[n-1][j] = T[n-1][j]$, which is trivially the maximum path sum (the path has length 1).
- **Inductive step**: Assume $M[k][j]$ correctly stores the maximum path sum starting from position $(k, j)$ for all $k > i$. At position $(i, j)$, any path must go through either $(i+1, j)$ or $(i+1, j+1)$. By the inductive hypothesis, the best continuation from those positions is $M[i+1][j]$ and $M[i+1][j+1]$ respectively. Therefore:
$$M[i][j] = T[i][j] + \max(M[i+1][j], M[i+1][j+1])$$
is the optimal path sum from $(i, j)$. $\square$

### In-Place Optimization

We can modify the triangle array in place, working from the bottom row upward, so that $T[i][j]$ is replaced by $M[i][j]$. This requires no extra space.

## Algorithm

```
1. Load the triangle T with n rows
2. For i = n-2 down to 0:
     For j = 0 to i:
       T[i][j] += max(T[i+1][j], T[i+1][j+1])
3. Return T[0][0]
```

## Complexity

- **Time**: $O(n^2)$ where $n = 100$ is the number of rows. Total operations: $\sum_{i=0}^{98}(i+1) = \frac{99 \times 100}{2} = 4950$.
- **Space**: $O(1)$ extra (in-place modification) or $O(n^2)$ for the triangle itself.

## Answer

$$\boxed{7273}$$

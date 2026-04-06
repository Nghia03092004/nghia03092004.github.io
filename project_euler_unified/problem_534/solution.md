# Problem 534: Weak Queens

## Problem Statement

A **weak queen** on a chessboard attacks only the 8 adjacent squares (like a king). Let $f(n)$ be the number of ways to place $n$ non-attacking weak queens on an $n \times n$ board with exactly one queen per row. Find $\sum_{n=1}^{12} f(n)$.

## Mathematical Foundation

**Theorem 1 (Reduction to Constrained Permutations).** *A placement of $n$ non-attacking weak queens, one per row, on an $n \times n$ board is valid if and only if the column assignment $\sigma \in S_n$ satisfies $|\sigma(i) - \sigma(i+1)| \ge 2$ for all $1 \le i \le n - 1$.*

**Proof.** Since there is exactly one queen per row, the column assignments $\sigma(1), \ldots, \sigma(n)$ must be distinct, forming a permutation. A weak queen in row $i$, column $\sigma(i)$ attacks the 8 cells at Chebyshev distance 1. Two queens in rows $i$ and $j$ can attack each other only if $|i - j| \le 1$ and $|\sigma(i) - \sigma(j)| \le 1$. Since queens are in distinct rows, the only potentially attacking pairs are in consecutive rows $i$ and $i+1$, requiring $|\sigma(i) - \sigma(i+1)| \le 1$. The non-attacking condition is therefore $|\sigma(i) - \sigma(i+1)| \ge 2$ for all consecutive $i$. $\square$

**Theorem 2 (Bitmask DP Correctness).** *Define $\mathrm{dp}[\mathrm{mask}][c]$ as the number of ways to fill $|\mathrm{mask}|$ rows using column set $\mathrm{mask}$, with the last queen in column $c \in \mathrm{mask}$. Then*

$$f(n) = \sum_{c=0}^{n-1} \mathrm{dp}[2^n - 1][c]$$

*where the recurrence is*

$$\mathrm{dp}[\mathrm{mask} \cup \{c\}][c] \mathrel{+}= \mathrm{dp}[\mathrm{mask}][c'] \quad \text{for all } c \notin \mathrm{mask},\; |c - c'| \ge 2.$$

**Proof.** The DP state captures exactly the information needed: which columns have been used (the bitmask ensures the permutation property) and which column was most recently placed (needed to enforce the $|\sigma(i) - \sigma(i+1)| \ge 2$ constraint). The base case assigns $\mathrm{dp}[\{c\}][c] = 1$ for each column $c$. Each transition extends a valid partial placement by one row. By induction on $|\mathrm{mask}|$, $\mathrm{dp}[\mathrm{mask}][c]$ counts all valid partial placements of $|\mathrm{mask}|$ queens using columns $\mathrm{mask}$ with the last in column $c$. Summing over all $c$ when $\mathrm{mask} = 2^n - 1$ gives $f(n)$. $\square$

## Algorithm

```
function F(n):
    if n == 1: return 1
    if n <= 3: return 0

    // dp[mask][last_col] = count of valid placements
    dp = map from (mask, col) -> integer, initially 0
    for c = 0 to n-1:
        dp[(1 << c, c)] = 1

    for row = 1 to n-1:
        new_dp = empty map
        for each (mask, prev_c) in dp with popcount(mask) == row:
            for c = 0 to n-1:
                if mask & (1 << c) != 0: continue
                if |c - prev_c| < 2: continue
                new_dp[(mask | (1 << c), c)] += dp[(mask, prev_c)]
        merge new_dp into dp

    return sum of dp[(2^n - 1, c)] for all c

function SOLVE():
    return sum of F(n) for n = 1 to 12
```

## Complexity Analysis

- **Time:** $O(n^2 \cdot 2^n)$ per value of $n$. For each of the $O(n \cdot 2^n)$ states, we try $O(n)$ transitions. For $n = 12$: $12^2 \cdot 2^{12} = 589{,}824$ operations. Summing over $n = 1, \ldots, 12$ is dominated by the $n = 12$ term.
- **Space:** $O(n \cdot 2^n)$ for the DP table. For $n = 12$: $12 \cdot 4096 = 49{,}152$ entries.

## Answer

$$\boxed{11726115562784664}$$

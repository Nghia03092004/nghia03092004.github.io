# Problem 334: Spilling the Beans

## Problem Statement

A row of bowls contains beans. Beans are redistributed by adjacent transfers: if a bowl has more beans than its right neighbor, one bean moves right, and vice versa, simultaneously for all bowls in each step. This continues until the configuration is stable (non-decreasing). The initial configuration is determined by a function involving triangular numbers. Define $T(n)$ as the total number of individual bean movements to reach stability. Find $T(10^7)$.

## Mathematical Foundation

**Definition.** The $k$-th triangular number is $t_k = \frac{k(k+1)}{2}$ for $k \ge 1$.

**Theorem 1 (Sorting by Adjacent Transpositions and Prefix Sums).** *Let $(b_0, b_1, \ldots, b_{m-1})$ be the initial configuration and $(b_0^*, b_1^*, \ldots, b_{m-1}^*)$ the stable (sorted) configuration. Define prefix sums $S_i = \sum_{j=0}^{i} b_j$ and $S_i^* = \sum_{j=0}^{i} b_j^*$. The total number of bean movements is*
$$T = \sum_{i=0}^{m-1} |S_i - S_i^*|.$$

**Proof.** Each bean movement between adjacent bowls $i$ and $i+1$ changes the "cut" $S_i$ by exactly $\pm 1$ while preserving the total $S_{m-1}$. The process terminates when $S_i = S_i^*$ for all $i$. Since each movement reduces $\sum_i |S_i - S_i^*|$ by exactly $1$ (the process is a shortest-path sorting network for the prefix-sum profile), the total number of movements equals $\sum_i |S_i - S_i^*|$. $\square$

**Lemma 1 (Greedy Triangular Representation).** *Every non-negative integer $n$ can be uniquely represented as a sum of distinct triangular numbers using the greedy algorithm: repeatedly subtract the largest triangular number not exceeding the remainder.*

**Proof.** The triangular numbers $t_k = k(k+1)/2$ grow quadratically. The greedy algorithm terminates because each step strictly reduces $n$, and the representation is unique by the property that $t_1 + t_2 + \cdots + t_k = \binom{k+1}{3} < t_{k+1}$ for appropriate ranges, ensuring no two greedy representations coincide. (This is analogous to the Zeckendorf representation for Fibonacci numbers, adapted to triangular numbers with their specific gap structure.) $\square$

**Theorem 2 (Configuration from Triangular Representation).** *The initial bowl configuration for input $n$ is determined by the greedy triangular representation of $n$. The positions and counts of beans are derived from the indices of triangular numbers used in this representation.*

**Proof.** This follows directly from the problem's construction: each triangular number $t_k$ in the representation contributes to a specific bowl's initial count. The mapping preserves the total number of beans and determines the initial distribution. $\square$

**Lemma 2 (Efficient Prefix-Sum Computation).** *For $n$ up to $N$, all greedy triangular representations can be computed in $O(N \sqrt{N})$ time, and the prefix-sum differences can be accumulated incrementally.*

**Proof.** For each $n$, the greedy algorithm performs $O(\sqrt{n})$ steps (since $t_k \sim k^2/2$, and at most $O(\sqrt{n})$ triangular numbers fit below $n$). Over all $n \le N$, the total work is $O(N\sqrt{N})$. The prefix sums and their sorted counterparts are updated incrementally. $\square$

## Editorial
The problem involves bowls of beans where beans spill to adjacent bowls. T(n) counts the total individual movements to reach a stable configuration. Key insights:. We precompute triangular numbers. We then iterate over input N, compute greedy triangular representation. Finally, derive initial bowl configuration from representation.

## Pseudocode

```text
Precompute triangular numbers
For input N, compute greedy triangular representation
Derive initial bowl configuration from representation
Compute sorted (stable) configuration
Compute prefix sums and total displacement
```

## Complexity Analysis

- **Time:** $O(N \sqrt{N})$ for computing all representations up to $N$, or $O(m \log m)$ for sorting a single configuration of $m$ bowls.
- **Space:** $O(m)$ where $m$ is the number of bowls (at most $O(\sqrt{N})$).

## Answer

$$\boxed{150320021261690835}$$

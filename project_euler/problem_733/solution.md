# Problem 733: Ascending Subsequences

## Problem Statement

Define $a_i = 153^i \bmod 10\,000\,019$ for $i \ge 1$. An **ascending subsequence** of length 4 within the first $n$ terms is a tuple $(a_{i_1}, a_{i_2}, a_{i_3}, a_{i_4})$ with $i_1 < i_2 < i_3 < i_4$ and $a_{i_1} < a_{i_2} < a_{i_3} < a_{i_4}$.

Let $S(n)$ be the sum of all terms in all such subsequences. Given: $S(6) = 94\,513\,710$, $S(100) = 4\,465\,488\,724\,217$.

Find $S(10^6) \bmod 10^9 + 7$.

## Mathematical Analysis

### Counting with Weighted Sums

For each ascending subsequence $(a_{i_1}, a_{i_2}, a_{i_3}, a_{i_4})$, its contribution to $S$ is $a_{i_1} + a_{i_2} + a_{i_3} + a_{i_4}$.

We can decompose: the contribution of $a_j$ at position $k$ (1st, 2nd, 3rd, or 4th element) equals $a_j$ times the number of ascending subsequences where $a_j$ occupies position $k$.

### BIT/Fenwick Tree Approach

Define arrays for $j = 1, \ldots, n$:
- $c_k(j)$ = number of ascending subsequences of length $k$ ending at $j$.
- $s_k(j)$ = sum of all elements in all ascending subsequences of length $k$ ending at $j$.

Recurrence:
$$c_k(j) = \sum_{\substack{i < j \\ a_i < a_j}} c_{k-1}(i), \quad s_k(j) = \sum_{\substack{i < j \\ a_i < a_j}} s_{k-1}(i) + a_j \cdot c_k(j)$$

These prefix sums over $a_i < a_j$ can be computed efficiently using a **Fenwick tree** (BIT) indexed by the value $a_i$ (after coordinate compression).

### Editorial
Sum of all elements in all ascending subsequences of length 4. Uses Fenwick trees for efficient prefix sum queries. We coordinate-compress the values to $[1, n]$. We then maintain 4 Fenwick trees for $c_1, c_2, c_3, c_4$ (counts) and 4 for $s_1, s_2, s_3, s_4$ (sums). Finally, process elements left to right. For each $a_j$.

### Pseudocode

```text
Generate all $a_i$ for $i = 1, \ldots, n$
Coordinate-compress the values to $[1, n]$
Maintain 4 Fenwick trees for $c_1, c_2, c_3, c_4$ (counts) and 4 for $s_1, s_2, s_3, s_4$ (sums)
Process elements left to right. For each $a_j$:
Query prefix sum of $c_{k-1}$ and $s_{k-1}$ for values $< a_j$
Update $c_k$ and $s_k$ at position $a_j$
Final answer: $S = \sum_j s_4(j)$
```

## Verification

| $n$ | $S(n)$ |
|-----|--------|
| 6 | 94,513,710 |
| 100 | 4,465,488,724,217 |

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time:** $O(n \log n)$ — four passes with Fenwick tree queries/updates.
- **Space:** $O(n)$ for the Fenwick trees.
- For $n = 10^6$: runs in seconds.

## Answer

$$\boxed{574368578}$$

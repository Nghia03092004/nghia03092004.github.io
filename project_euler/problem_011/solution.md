# Problem 11: Largest Product in a Grid

## Problem Statement

Let $G = (g_{i,j})_{0 \le i,j \le 19}$ be a $20 \times 20$ matrix of non-negative integers (given below). Find the greatest product of four adjacent entries along any horizontal, vertical, or diagonal line.

## Definitions

**Definition 1.** For a direction vector $(d_r, d_c) \in \{(0,1),(1,0),(1,1),(1,-1)\}$, define the *$(d_r,d_c)$-product of length $\ell$ starting at $(i,j)$* as

$$P_{d_r,d_c}(i,j) = \prod_{k=0}^{\ell-1} g_{i + k d_r,\, j + k d_c},$$

provided all indices $i + kd_r, j + kd_c \in [0, n-1]$ for $k = 0, \ldots, \ell-1$.

**Definition 2.** The *feasible set* for direction $(d_r,d_c)$ is

$$\mathcal{F}_{d_r,d_c} = \bigl\{(i,j) \in [0,n-1]^2 : i+(\ell-1)d_r \in [0,n-1] \text{ and } j+(\ell-1)d_c \in [0,n-1]\bigr\}.$$

## Theorems

**Theorem 1 (Completeness of four directions).** The four direction vectors $(0,1)$, $(1,0)$, $(1,1)$, $(1,-1)$ suffice to enumerate every set of $\ell$ collinear adjacent entries in the grid. No products are missed and none are double-counted.

*Proof.* An arbitrary direction vector for collinear adjacency has the form $(d_r, d_c)$ with $(d_r, d_c) \in \{(\pm 1, 0), (0, \pm 1), (\pm 1, \pm 1)\}$. Consider any line segment of $\ell$ cells in direction $(d_r, d_c)$ starting at $(i,j)$. Its reverse, in direction $(-d_r, -d_c)$, starts at $(i + (\ell-1)d_r,\, j + (\ell-1)d_c)$ and traverses the same multiset of cells. Since multiplication is commutative, the product is identical. We pair directions:

- $(0,1) \leftrightarrow (0,-1)$ (horizontal),
- $(1,0) \leftrightarrow (-1,0)$ (vertical),
- $(1,1) \leftrightarrow (-1,-1)$ (main diagonal),
- $(1,-1) \leftrightarrow (-1,1)$ (anti-diagonal).

Each pair is represented by exactly one of our four chosen directions. Thus every product appears exactly once in $\bigcup_{(d_r,d_c)} \{P_{d_r,d_c}(i,j) : (i,j) \in \mathcal{F}_{d_r,d_c}\}$. $\square$

**Lemma 1 (Product count).** For $n = 20$ and $\ell = 4$, the total number of products is $1258$.

*Proof.* We compute $|\mathcal{F}_{d_r,d_c}|$ for each direction:

| Direction $(d_r,d_c)$ | Row constraint | Column constraint | Count |
|---|---|---|---|
| $(0,1)$ | $0 \le i \le 19$ | $0 \le j \le 16$ | $20 \times 17 = 340$ |
| $(1,0)$ | $0 \le i \le 16$ | $0 \le j \le 19$ | $17 \times 20 = 340$ |
| $(1,1)$ | $0 \le i \le 16$ | $0 \le j \le 16$ | $17 \times 17 = 289$ |
| $(1,-1)$ | $0 \le i \le 16$ | $3 \le j \le 19$ | $17 \times 17 = 289$ |

Total: $340 + 340 + 289 + 289 = 1258$. $\square$

**Theorem 2 (Correctness of brute-force search).** The algorithm below computes $\max_{(d_r,d_c)}\, \max_{(i,j) \in \mathcal{F}_{d_r,d_c}} P_{d_r,d_c}(i,j)$ by exhaustive enumeration over all $1258$ products.

*Proof.* The nested loops iterate over every $(i,j) \in \mathcal{F}_{d_r,d_c}$ for each of the four directions, computing each product exactly once and tracking the running maximum. By Theorem 1, this covers all possible products. $\square$

## Algorithm

```
LARGEST-GRID-PRODUCT(G, n, l):
    best <- 0
    for each (dr, dc) in {(0,1), (1,0), (1,1), (1,-1)}:
        for each (i, j) in F_{dr,dc}:
            p <- 1
            for k <- 0 to l-1:
                p <- p * G[i + k*dr][j + k*dc]
            best <- max(best, p)
    return best
```

## Complexity Analysis

**Proposition.** The algorithm runs in $\Theta(n^2 \ell)$ time and $\Theta(n^2)$ space.

*Proof.* There are $|\mathcal{F}| = O(n^2)$ starting positions across all directions (precisely $4n(n-\ell+1) - 2(n-\ell+1)^2$ by Lemma 1, which is $\Theta(n^2)$ for fixed $\ell$). Each product requires $\ell - 1$ multiplications, so total work is $\Theta(n^2 \ell)$. The grid occupies $\Theta(n^2)$ space, and the algorithm uses $O(1)$ auxiliary variables. $\square$

For $n = 20$, $\ell = 4$: exactly $1258 \times 3 = 3774$ multiplications and $1258$ comparisons.

## Answer

$$\boxed{70600674}$$

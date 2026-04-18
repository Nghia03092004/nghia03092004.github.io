# Problem 345: Matrix Sum

## Problem Statement

Given the $15 \times 15$ matrix:

```
  7  53 183 439 863 497 383 563  79 973 287  63 343 169 583
627 343 773 959 943 767 473 103 699 303 957 703 583 639 913
447 283 463  29  23 487 463 993 119 883 327 493 423 159 743
217 623   3 399 853 407 103 983  89 463 290 516 212 462 350
960 376 682 962 300 780 486 502 912 800 250 346 172 812 350
870 456 192 162 593 473 915  45 989 873 823 965 425 329 803
973 965 905 919 133 673 665 235 509 613 673 815 165 992 326
322 148 972 962 286 255 941 541 265 323 925 281 601  95 973
445 721  11 525 473  65 511 164 138 672  18 428 154 448 848
414 456 310 312 798 104 566 520 302 248 694 976 430 392 198
184 829 373 181 631 101 969 613 840 740 778 458 284 760 390
821 461 843 513  17 901 711 993 293 157 274  94 192 156 574
 34 124   4 878 450 476 712 914 838 669 875 299 823 329 699
815 559 813 459 522 788 168 586 966 232 308 833 251 631 107
813 883 451 509 615  77 281 613 459 205 380 274 302  35 805
```

Find $\max_{\sigma \in S_{15}} \sum_{i=1}^{15} M_{i,\sigma(i)}$, the maximum sum obtainable by selecting exactly one element from each row and each column.

## Mathematical Foundation

**Theorem 1 (Assignment Problem Duality -- Konig--Egervary).** The maximum weight matching in a bipartite graph equals the minimum weight cover. For the assignment problem with cost matrix $C$, the optimum of
$$\max_{\sigma \in S_n} \sum_{i=1}^{n} C_{i,\sigma(i)}$$
equals
$$\min \left\{ \sum_{i=1}^{n} u_i + \sum_{j=1}^{n} v_j \;\middle|\; u_i + v_j \ge C_{ij} \;\forall\, i,j \right\}.$$

**Proof.** This is LP duality applied to the assignment problem. The primal is a linear program over the Birkhoff polytope (doubly stochastic matrices), which has integer vertices corresponding to permutation matrices. The dual introduces row potentials $u_i$ and column potentials $v_j$ with constraints $u_i + v_j \ge C_{ij}$. Strong duality holds since the primal is feasible and bounded. $\square$

**Theorem 2 (Hungarian Algorithm Correctness).** The Hungarian algorithm (Kuhn 1955, Munkres 1957) finds an optimal assignment in $O(n^3)$ time by maintaining dual-feasible potentials and iteratively augmenting the matching.

**Proof.** The algorithm maintains potentials $(u, v)$ satisfying $u_i + v_j \ge C_{ij}$ with equality on matched edges. At each step, it either augments the matching (increasing the primal objective) or adjusts potentials (tightening the dual bound) while preserving feasibility. After $n$ augmentations, a perfect matching is achieved with complementary slackness satisfied, guaranteeing optimality by LP duality. $\square$

**Lemma 1 (Bitmask DP Alternative).** For small $n$, the problem admits a bitmask DP solution: define $\text{dp}[S]$ as the maximum sum achievable by assigning columns in set $S$ to the first $|S|$ rows. Then:
$$\text{dp}[S] = \max_{j \in S} \left( \text{dp}[S \setminus \{j\}] + M_{|S|, j} \right), \quad \text{dp}[\emptyset] = 0.$$

**Proof.** By induction on $|S|$. The recurrence considers all possible column assignments $j$ for row $|S|$, taking the best over all choices. The base case $\text{dp}[\emptyset] = 0$ is trivial. Correctness follows from the principle of optimality (Bellman). $\square$

## Editorial
Approach: Bitmask DP over columns. dp[mask] = maximum sum using first popcount(mask) rows, with columns in mask used. Time: O(n * 2^n) = O(15 * 32768) ~ 500K operations. We bitmask DP approach (feasible for n=15).

## Pseudocode

```text
Bitmask DP approach (feasible for n=15)
if mask has bit j set
```

## Complexity Analysis

- **Time:** $O(n \cdot 2^n) = O(15 \cdot 2^{15}) = O(491{,}520)$ operations. (Hungarian algorithm would give $O(n^3) = O(3375)$.)
- **Space:** $O(2^n) = O(32{,}768)$ for the DP table.

## Answer

$$\boxed{13938}$$

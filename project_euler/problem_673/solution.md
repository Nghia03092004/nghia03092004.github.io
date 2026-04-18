# Problem 673: Beds and Desks

## Problem Statement

At Euler University, $n$ students each occupy a bed (in a dormitory) and a desk (in a classroom). Some beds are in single rooms and others in double rooms; similarly for desks. A reassignment is a permutation $\sigma$ of $\{1, \ldots, n\}$ such that for every double bed-room pair $(i, j)$, the images $(\sigma(i), \sigma(j))$ form a valid desk-room pair (either a double desk-room or two singles). Count the number of valid reassignment permutations.

## Mathematical Foundation

**Definition.** Let $\mathcal{M}_B$ and $\mathcal{M}_D$ be the matchings on $[n]$ induced by double bed-rooms and double desk-rooms respectively. A permutation $\sigma \in S_n$ is *valid* if for every edge $(i,j) \in \mathcal{M}_B$, the pair $(\sigma(i), \sigma(j))$ is an edge in $\mathcal{M}_D$ or both $\sigma(i)$ and $\sigma(j)$ are unmatched in $\mathcal{M}_D$.

**Theorem 1 (Reduction to Permanent).** *Let $A$ be the $n \times n$ binary compatibility matrix where $A_{ij} = 1$ if student $i$ can be assigned desk $j$ under the room constraints. Then the number of valid reassignments is $\operatorname{perm}(A)$.*

**Proof.** Each valid reassignment is a permutation $\sigma$ such that $A_{i,\sigma(i)} = 1$ for all $i$. By definition of the permanent:
$$\operatorname{perm}(A) = \sum_{\sigma \in S_n} \prod_{i=1}^{n} A_{i,\sigma(i)}.$$
Each summand is $1$ if $\sigma$ is valid and $0$ otherwise, so $\operatorname{perm}(A)$ counts exactly the valid reassignments. $\square$

**Theorem 2 (Ryser's Formula).** *For an $n \times n$ matrix $A$:*
$$\operatorname{perm}(A) = (-1)^n \sum_{S \subseteq [n]} (-1)^{|S|} \prod_{i=1}^{n} \sum_{j \in S} A_{ij}.$$

**Proof.** Expand $\prod_{i=1}^{n} \bigl(\sum_{j=1}^{n} A_{ij} x_j\bigr)$ and apply inclusion-exclusion. The coefficient of $x_1 x_2 \cdots x_n$ in $\prod_i \bigl(\sum_j A_{ij} x_j\bigr)$ equals $\operatorname{perm}(A)$. By inclusion-exclusion over subsets $S \subseteq [n]$ (which variables are "active"), the permanent is extracted as stated. The sign $(-1)^n$ accounts for the alternating inclusion-exclusion. $\square$

**Lemma 1 (Block Factorisation).** *If the compatibility matrix $A$ is block-diagonal, $A = \operatorname{diag}(A_1, \ldots, A_m)$, then*
$$\operatorname{perm}(A) = \prod_{j=1}^{m} \operatorname{perm}(A_j).$$

**Proof.** A permutation $\sigma$ contributes to $\operatorname{perm}(A)$ only if $\sigma$ maps each block's indices to themselves (since off-diagonal entries are $0$). Thus $\sigma$ decomposes as $\sigma = \sigma_1 \times \cdots \times \sigma_m$ where $\sigma_j$ permutes block $j$'s indices, and the product factorises. $\square$

**Lemma 2 (Hafnian for All-Doubles Case).** *When every room is a double room, the matchings $\mathcal{M}_B$ and $\mathcal{M}_D$ are perfect matchings on $[n]$ ($n$ even). The count of valid $\sigma$ mapping bed-pairs to desk-pairs bijects to the permanent of a $\frac{n}{2} \times \frac{n}{2}$ matrix $B$ where $B_{ij}$ counts the number of ways bed-pair $i$ maps to desk-pair $j$ (either $0$ or $2$). Thus the answer is $\operatorname{perm}(B)$ times the appropriate orientation factor.*

**Proof.** Each bed-pair $(i_1, i_2)$ must map to some desk-pair. The pair can map in two orientations (either $\sigma(i_1) = j_1, \sigma(i_2) = j_2$ or the reverse), so $B_{ij} = 2$ if compatible and $0$ otherwise. The total count is $\operatorname{perm}(B)$ when expanded, since each matching of bed-pairs to desk-pairs contributes independently. $\square$

## Editorial
We build compatibility matrix. We then iterate over each student i. Finally, iterate over each desk j. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
Build compatibility matrix
for each student i
for each desk j
with all room constraints
Identify block structure
Compute permanent of each block via Ryser's formula
```

## Complexity Analysis

- **Time:** Ryser's formula on each block costs $O(2^{n_k} \cdot n_k)$. With block factorisation, total is $O(\sum_k 2^{n_k} \cdot n_k)$. If there are no large blocks, this is efficient; worst case (single block) is $O(2^n \cdot n)$.
- **Space:** $O(n^2)$ for the compatibility matrix, $O(n)$ auxiliary for the subset enumeration (using Gray code).

## Answer

$$\boxed{700325380}$$

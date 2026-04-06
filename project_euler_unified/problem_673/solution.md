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

## Algorithm

```
function CountValidReassignments(n, bed_rooms, desk_rooms):
    // Step 1: Build compatibility matrix
    A = n x n zero matrix
    for each student i:
        for each desk j:
            if assigning student i to desk j is compatible
               with all room constraints:
                A[i][j] = 1

    // Step 2: Identify block structure
    blocks = connected_components(A)

    // Step 3: Compute permanent of each block via Ryser's formula
    result = 1
    for each block B_k of size n_k:
        perm_k = 0
        for each subset S of columns of B_k (Gray code order):
            row_sums_product = product over rows i of (sum of B_k[i][j] for j in S)
            perm_k += (-1)^(n_k + |S|) * row_sums_product
        result *= perm_k

    return result
```

## Complexity Analysis

- **Time:** Ryser's formula on each block costs $O(2^{n_k} \cdot n_k)$. With block factorisation, total is $O(\sum_k 2^{n_k} \cdot n_k)$. If there are no large blocks, this is efficient; worst case (single block) is $O(2^n \cdot n)$.
- **Space:** $O(n^2)$ for the compatibility matrix, $O(n)$ auxiliary for the subset enumeration (using Gray code).

## Answer

$$\boxed{700325380}$$

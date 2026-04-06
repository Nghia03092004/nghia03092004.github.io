# Problem 631: Constrained Permutations

## Problem Statement

Let $A$ be an $n \times n$ matrix with entries in $\{0, 1\}$, where $A_{ij} = 1$ indicates that element $j$ is permitted in position $i$. Count the number of permutations $\sigma \in S_n$ such that $A_{i,\sigma(i)} = 1$ for all $i$. This count equals the **permanent** of $A$:

$$\operatorname{perm}(A) = \sum_{\sigma \in S_n} \prod_{i=1}^{n} A_{i,\sigma(i)}.$$

## Mathematical Foundation

**Theorem 1 (Ryser, 1963).** *For any $n \times n$ matrix $A$,*

$$\operatorname{perm}(A) = (-1)^n \sum_{S \subseteq [n]} (-1)^{|S|} \prod_{i=1}^{n} \sum_{j \in S} A_{ij}.$$

**Proof.** For each subset $S \subseteq [n]$, expand the product $\prod_{i=1}^{n} \bigl(\sum_{j \in S} A_{ij}\bigr)$ as a sum over functions $f : [n] \to S$:

$$\prod_{i=1}^{n} \sum_{j \in S} A_{ij} = \sum_{f : [n] \to S} \prod_{i=1}^{n} A_{i,f(i)}.$$

Now consider a fixed bijection $\sigma \in S_n$. The term $\prod_{i} A_{i,\sigma(i)}$ appears in the inner sum whenever $\operatorname{range}(\sigma) \subseteq S$, i.e., $S = [n]$ since $\sigma$ is a bijection. So its contribution to the alternating sum is

$$(-1)^n \cdot (-1)^{n} \cdot \prod_i A_{i,\sigma(i)} = \prod_i A_{i,\sigma(i)}.$$

For a non-injective function $f : [n] \to S$ with $|\operatorname{range}(f)| = r < n$, the function appears in every $S \supseteq \operatorname{range}(f)$. By inclusion-exclusion, the contribution over all $S$ is

$$\sum_{S \supseteq \operatorname{range}(f)} (-1)^{|S|} = (-1)^r \sum_{k=0}^{n-r} \binom{n-r}{k}(-1)^k = (-1)^r \cdot 0 = 0$$

since $n - r \geq 1$. Therefore only bijections survive, yielding $(-1)^n \cdot (-1)^n \operatorname{perm}(A) = \operatorname{perm}(A)$. $\square$

**Lemma 1 (Gray Code Update).** *If subsets of $[n]$ are enumerated in Gray code order, consecutive subsets differ by exactly one element. The row sums $r_i(S) = \sum_{j \in S} A_{ij}$ can therefore be updated in $O(n)$ per transition.*

**Proof.** Let $S' = S \oplus \{j_0\}$ for some $j_0$. Then $r_i(S') = r_i(S) \pm A_{i,j_0}$ for each $i \in [n]$, requiring $n$ additions. The product $\prod_i r_i(S')$ is then computed in $O(n)$. $\square$

**Lemma 2 (Bipartite Matching Equivalence).** *$\operatorname{perm}(A)$ equals the number of perfect matchings in the bipartite graph $G = (U \cup V, E)$ where $U = \{u_1, \ldots, u_n\}$ (positions), $V = \{v_1, \ldots, v_n\}$ (elements), and $(u_i, v_j) \in E$ iff $A_{ij} = 1$.*

**Proof.** A perfect matching $M$ in $G$ is a bijection $\sigma$ with $(u_i, v_{\sigma(i)}) \in E$ for all $i$, i.e., $A_{i,\sigma(i)} = 1$ for all $i$. Summing over all such bijections gives $\operatorname{perm}(A)$. $\square$

## Algorithm

```
function PERMANENT(A, n):
    // Ryser's formula with Gray code enumeration
    r[1..n] <- 0          // row sums
    perm <- 0
    sign <- (-1)^n
    for each subset S of [n] in Gray code order:
        let j0 be the element toggled
        if j0 was added to S:
            for i = 1 to n: r[i] <- r[i] + A[i][j0]
        else:
            for i = 1 to n: r[i] <- r[i] - A[i][j0]
        product <- r[1] * r[2] * ... * r[n]
        perm <- perm + sign * product
        sign <- -sign
    return (-1)^n * perm
```

## Complexity Analysis

- **Time:** $O(2^n \cdot n)$. There are $2^n$ subsets; each transition updates $n$ row sums and computes an $n$-fold product.
- **Space:** $O(n)$ for storing the row sums and the matrix $A$.

For comparison, the naive summation over all $n!$ permutations costs $O(n! \cdot n)$.

## Answer

$$\boxed{869588692}$$

# Problem 106: Special Subset Sums: Meta-testing

## Problem Statement

Let $S(A)$ represent the sum of elements in set $A$ of size $n$. We shall call it a special sum set if for any two non-empty disjoint subsets, $B$ and $C$, the following properties are true:

1. $S(B) \neq S(C)$; that is, sums of subsets are not equal.
2. If $|B| > |C|$ then $S(B) > S(C)$.

We assume Property 2 is already satisfied, which automatically ensures $S(B) \neq S(C)$ when $|B| \neq |C|$. Therefore, we only need to verify Property 1 for pairs $(B, C)$ with $|B| = |C|$.

For $n = 12$, how many subset pairs need to be tested for equality?

## Mathematical Foundation

**Theorem 1.** *(Reduction to Equal-Size Pairs)* If Property 2 holds for a set $A$, then $S(B) \neq S(C)$ for all non-empty disjoint subsets $B, C$ with $|B| \neq |C|$.

**Proof.** Without loss of generality, assume $|B| > |C|$. Property 2 directly gives $S(B) > S(C)$, hence $S(B) \neq S(C)$. $\square$

**Theorem 2.** *(Counting Equal-Size Disjoint Pairs)* The number of unordered pairs $\{B, C\}$ of disjoint subsets of an $n$-element set with $|B| = |C| = k$ is:

$$N_k = \frac{1}{2}\binom{n}{k}\binom{n-k}{k} = \binom{n}{2k} \cdot \frac{1}{2}\binom{2k}{k}$$

**Proof.** To choose an ordered pair $(B, C)$: first choose $B$ as any $k$-subset of $[n]$ in $\binom{n}{k}$ ways, then choose $C$ as any $k$-subset of the remaining $n - k$ elements in $\binom{n-k}{k}$ ways. This counts each unordered pair $\{B, C\}$ twice (once as $(B, C)$ and once as $(C, B)$), so $N_k = \frac{1}{2}\binom{n}{k}\binom{n-k}{k}$.

For the alternative form: first choose the $2k$ elements that will participate in $\binom{n}{2k}$ ways, then partition them into two groups of $k$ in $\frac{1}{2}\binom{2k}{k}$ ways (dividing by 2 for the unordered pair). $\square$

**Theorem 3.** *(Non-Crossing Pairs are Automatically Verified)* Let $A = \{a_1 < a_2 < \cdots < a_n\}$ satisfy Property 2. Consider two disjoint subsets $B, C \subset A$ with $|B| = |C| = k$. Sort the elements of $B$ as $b_1 < \cdots < b_k$ and of $C$ as $c_1 < \cdots < c_k$. If $b_i < c_i$ for all $i$ (or $b_i > c_i$ for all $i$), then $S(B) \neq S(C)$ is automatically guaranteed.

**Proof.** Suppose $b_i < c_i$ for all $i = 1, \ldots, k$. Then $S(B) = \sum b_i < \sum c_i = S(C)$, since $b_i < c_i$ for every index. Hence $S(B) \neq S(C)$ without any additional verification. The case $b_i > c_i$ for all $i$ is symmetric. $\square$

**Theorem 4.** *(Connection to Catalan Numbers)* Given $2k$ elements in sorted order, the number of ways to partition them into two groups of $k$ such that the element-wise comparison is non-crossing (i.e., $b_i < c_i$ for all $i$, or $b_i > c_i$ for all $i$, when both sequences are sorted) equals $2 \cdot C_k$ for ordered pairs or $C_k$ for unordered pairs, where $C_k = \frac{1}{k+1}\binom{2k}{k}$ is the $k$-th Catalan number.

**Proof.** Label the $2k$ sorted elements by their group membership: each element gets label $B$ or $C$, with exactly $k$ of each. We claim that the partition has the non-crossing property (say $b_i < c_i$ for all $i$) if and only if, when we read the labels in sorted order of the elements, we never have more $C$-labels than $B$-labels in any prefix.

To see this: sort all $2k$ elements as $e_1 < e_2 < \cdots < e_{2k}$. Assign $B$-labels and $C$-labels. The sorted $B$-sequence is $b_1 < \cdots < b_k$ (the elements labeled $B$ in order) and similarly for $C$. The condition $b_i < c_i$ for all $i$ means that the $i$-th $B$-element appears before the $i$-th $C$-element in the sorted order. This is equivalent to requiring that in every prefix of the label sequence, the count of $B$'s is at least the count of $C$'s.

The number of such sequences is exactly the $k$-th Catalan number $C_k = \frac{1}{k+1}\binom{2k}{k}$, by the ballot problem / Bertrand ballot theorem. (The total number of label sequences with $k$ $B$'s and $k$ $C$'s is $\binom{2k}{k}$, and the fraction with all prefixes having at least as many $B$'s as $C$'s is $\frac{1}{k+1}$.)

For unordered pairs, the $C_k$ non-crossing partitions with $b_i < c_i$ are in bijection with the $C_k$ partitions with $b_i > c_i$ (swap labels). Each unordered pair is counted once, so the number of unordered non-crossing pairs is $C_k$. $\square$

**Theorem 5.** *(Main Formula)* The number of equal-size disjoint pairs that need testing is:

$$T = \sum_{k=2}^{\lfloor n/2 \rfloor} \binom{n}{2k}\left[\frac{1}{2}\binom{2k}{k} - C_k\right]$$

where $C_k = \frac{1}{k+1}\binom{2k}{k}$.

**Proof.** For each subset size $k$, the total number of unordered equal-size disjoint pairs is $\binom{n}{2k} \cdot \frac{1}{2}\binom{2k}{k}$ (Theorem 2). Of these, $\binom{n}{2k} \cdot C_k$ are non-crossing and hence automatically verified (Theorems 3-4). The difference gives the number needing testing.

For $k = 1$: $\frac{1}{2}\binom{2}{1} - C_1 = 1 - 1 = 0$, so no singleton pairs need testing (if $b_1 < c_1$ then trivially $S(B) < S(C)$). Hence the sum starts at $k = 2$. $\square$

## Editorial
|-----|-------------------|-----------------------------|--------|------------|--------------| | 2   | 495               | 3                           | 2      | 1          | 495          | | 3   | 924               | 10                          | 5      | 5          | 4620         | | 4   | 495               | 35                          | 14     | 21         | 10395        | | 5   | 66                | 126                         | 42     | 84         | 5544         | | 6   | 1                 | 462                         | 132    | 330        | 330          |.

## Pseudocode

```text
    total = 0
    For k from 2 to floor(n/2):
        choose_2k = binomial(n, 2*k)
        half_middle = binomial(2*k, k) / 2
        catalan_k = binomial(2*k, k) / (k + 1)
        total += choose_2k * (half_middle - catalan_k)
    Return total
```

**Computation for $n = 12$:**

| $k$ | $\binom{12}{2k}$ | $\frac{1}{2}\binom{2k}{k}$ | $C_k$ | Difference | Contribution |
|-----|-------------------|-----------------------------|--------|------------|--------------|
| 2 | 495 | 3 | 2 | 1 | 495 |
| 3 | 924 | 10 | 5 | 5 | 4620 |
| 4 | 495 | 35 | 14 | 21 | 10395 |
| 5 | 66 | 126 | 42 | 84 | 5544 |
| 6 | 1 | 462 | 132 | 330 | 330 |

$$T = 495 + 4620 + 10395 + 5544 + 330 = 21384$$
```

## Complexity Analysis

- **Time**: $O(n)$ to iterate over $k$ values from 2 to $\lfloor n/2 \rfloor$, assuming $O(1)$ computation of binomial coefficients (precomputed or via closed-form). For $n = 12$, this is 5 iterations.
- **Space**: $O(1)$, storing only a running total and temporary values.

## Answer

$$\boxed{21384}$$

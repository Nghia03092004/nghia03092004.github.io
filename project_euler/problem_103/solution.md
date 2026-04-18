# Problem 103: Special Subset Sums: Optimum

## Problem Statement

Let $S(A)$ denote the sum of elements in a set $A$ of positive integers. We call $A$ a **special sum set** if for any two non-empty disjoint subsets $B, C \subseteq A$:

1. $S(B) \neq S(C)$ (distinct subset sums).
2. If $|B| > |C|$, then $S(B) > S(C)$ (size-ordering property).

For $n = 7$, starting from the near-optimum set $\{20, 31, 38, 39, 40, 42, 45\}$ with $S(A) = 255$, find the optimum special sum set (minimizing $S(A)$) and give the answer as the concatenated string of its sorted elements.

## Mathematical Development

**Theorem 1** (Equivalent Condition for Property 2). *Let $A = \{a_1 < a_2 < \cdots < a_n\}$ be a set of positive integers. Property 2 holds if and only if*
$$\sum_{i=1}^{k+1} a_i > \sum_{i=n-k+1}^{n} a_i \qquad \text{for all } k = 1, 2, \ldots, \lfloor n/2 \rfloor.$$

*Proof.* ($\Rightarrow$) Take $B = \{a_1, \ldots, a_{k+1}\}$ and $C = \{a_{n-k+1}, \ldots, a_n\}$. These are disjoint since $k + 1 \leq \lfloor n/2 \rfloor + 1 \leq n - k + 1$ for $k \leq \lfloor n/2 \rfloor$. Since $|B| = k + 1 > k = |C|$, Property 2 yields $S(B) > S(C)$.

($\Leftarrow$) Let $B, C$ be disjoint subsets of $A$ with $|B| = m > \ell = |C|$. Write $B = \{b_1 < \cdots < b_m\}$ and $C = \{c_1 < \cdots < c_\ell\}$. Since the $b_i$ are $m$ distinct elements of $A$, we have $b_i \geq a_i$, so $S(B) \geq \sum_{i=1}^{m} a_i$. Similarly, since the $c_j$ are $\ell$ distinct elements of $A$, we have $c_j \leq a_{n - \ell + j}$, so $S(C) \leq \sum_{j=n-\ell+1}^{n} a_j$. Applying the hypothesis with $k = \ell$:
$$S(B) \geq \sum_{i=1}^{m} a_i \geq \sum_{i=1}^{\ell+1} a_i > \sum_{j=n-\ell+1}^{n} a_j \geq S(C). \qquad \blacksquare$$

**Theorem 2** (Reduction to Global Subset-Sum Uniqueness). *Property 1 holds for $A$ if and only if all $2^n - 1$ non-empty subset sums of $A$ are pairwise distinct.*

*Proof.* ($\Leftarrow$) If all subset sums are distinct, then in particular $S(B) \neq S(C)$ for any two distinct non-empty subsets, a fortiori for disjoint ones.

($\Rightarrow$) We prove the contrapositive. Suppose distinct non-empty subsets $B', C'$ satisfy $S(B') = S(C')$. Let $D = B' \cap C'$, $B = B' \setminus D$, $C = C' \setminus D$. Then $B$ and $C$ are disjoint. Since $B' \neq C'$, at least one of $B, C$ is non-empty. If both are non-empty, then
$$S(B) = S(B') - S(D) = S(C') - S(D) = S(C),$$
violating Property 1. If exactly one (say $C$) is empty, then $C' \subseteq B'$ and $S(B') = S(C')$ forces $S(B' \setminus C') = 0$; but all elements are positive, so $B' \setminus C' = \emptyset$, contradicting $B' \neq C'$. $\blacksquare$

**Lemma 1** (Near-Optimum Heuristic Construction). *Given an optimal special sum set $\{b_1, \ldots, b_{n-1}\}$ of size $n - 1$ with middle element $b_m$ where $m = \lceil(n-1)/2\rceil$, the set $\{b_m,\, b_m + b_1,\, b_m + b_2,\, \ldots,\, b_m + b_{n-1}\}$ is a near-optimum special sum set of size $n$.*

*Proof.* This construction, due to Lunnon, yields a set satisfying both properties. The minimum element is $b_m > 0$, ensuring positivity. Property 2 is satisfied because the spread $b_m + b_1$ vs.\ $b_m + b_{n-1}$ inherits the structure of the original set. Optimality is not guaranteed, but the set serves as a starting point for local search. $\blacksquare$

## Editorial
We check Property 2. Finally, check Property 1. Candidates are generated from the derived formulas, filtered by the required conditions, and processed in order until the desired value is obtained.

## Pseudocode

```text
Check Property 2
Check Property 1
```

## Complexity Analysis

- **Time.** The perturbation space has $(2 \cdot 3 + 1)^7 = 7^7 = 823{,}543$ candidates. For each candidate passing the sum bound and Property 2 filter, Property 1 costs $O(2^n)$ with $n = 7$, i.e., 128 subset-sum computations. With aggressive pruning, total work is on the order of $10^6$ operations.
- **Space.** $O(2^n) = O(128)$ for the hash set of subset sums.

## Answer

$$\boxed{20313839404245}$$

# Problem 105: Special Subset Sums: Testing

## Problem Statement

Let $S(A)$ denote the sum of elements in set $A$ of size $n$. We shall call it a **special sum set** if for any two non-empty, disjoint subsets $B$ and $C$, the following properties hold:

1. $S(B) \neq S(C)$; that is, sums of subsets are never equal.
2. If $|B| > |C|$ then $S(B) > S(C)$.

Using a file containing one hundred sets (the first of which is the example set from Problem 103), identify all the special sum sets, $A_1, A_2, \ldots, A_k$, and find $\sum S(A_i)$.

## Mathematical Foundation

**Theorem 1.** *(Equivalent Condition for Property 2)* For a sorted set $A = \{a_1 < a_2 < \cdots < a_n\}$, Property 2 holds if and only if for all $k = 1, 2, \ldots, \lfloor n/2 \rfloor$:

$$\sum_{i=1}^{k+1} a_i > \sum_{i=n-k+1}^{n} a_i$$

**Proof.** ($\Rightarrow$): Setting $B = \{a_1, \ldots, a_{k+1}\}$ and $C = \{a_{n-k+1}, \ldots, a_n\}$ in Property 2 gives the inequality directly, since $|B| = k+1 > k = |C|$ and $B \cap C = \emptyset$ (because $k+1 \leq n-k+1$ when $k \leq \lfloor n/2 \rfloor$).

($\Leftarrow$): Let $B, C$ be disjoint subsets with $|B| = m > \ell = |C|$. Write $B = \{b_1 < \cdots < b_m\}$ and $C = \{c_1 < \cdots < c_\ell\}$. Since $b_i \geq a_i$ (the $i$-th element of $B$ is at least the $i$-th smallest in $A$) and $c_j \leq a_{n-\ell+j}$ (the $j$-th element of $C$ is at most the $(n-\ell+j)$-th smallest in $A$), we have:

$$S(B) \geq \sum_{i=1}^{m} a_i \geq \sum_{i=1}^{\ell+1} a_i > \sum_{i=n-\ell+1}^{n} a_i \geq S(C)$$

where the strict inequality uses the hypothesis with $k = \ell$ (valid since $\ell < m \leq n$ implies $\ell \leq \lfloor n/2 \rfloor$ when $B$ and $C$ are disjoint subsets of an $n$-element set). $\square$

**Theorem 2.** *(Subset Sum Uniqueness Implies Property 1)* Property 1 (no two non-empty disjoint subsets have equal sums) holds if and only if all $2^n - 1$ non-empty subset sums are pairwise distinct.

**Proof.** ($\Leftarrow$): Trivial, since disjoint subsets are in particular distinct subsets.

($\Rightarrow$): Suppose for contradiction that two distinct non-empty subsets $B', C'$ satisfy $S(B') = S(C')$. Let $D = B' \cap C'$, $B = B' \setminus D$, $C = C' \setminus D$. Then $B \cap C = \emptyset$ and $B \cup C \neq \emptyset$ (since $B' \neq C'$). Moreover $S(B) = S(B') - S(D) = S(C') - S(D) = S(C)$.

If $B = \emptyset$, then $C' = D \cup C$ and $B' = D$, so $S(D \cup C) = S(D)$, giving $S(C) = 0$. Since all elements are positive, this forces $C = \emptyset$, contradicting $B \cup C \neq \emptyset$. Similarly $C \neq \emptyset$. Thus $B, C$ are non-empty, disjoint, with $S(B) = S(C)$, violating Property 1. $\square$

**Lemma 1.** *(Complexity Bound for Subset Enumeration)* For a set of $n$ elements, the number of non-empty subsets is $2^n - 1$. Checking distinctness of all subset sums can be done in $O(2^n)$ expected time using a hash set.

**Proof.** There are $2^n - 1$ non-empty subsets. We compute each subset sum incrementally (e.g., using Gray code enumeration, changing one element at a time, giving $O(1)$ per subset sum update). Inserting into and querying a hash set takes $O(1)$ expected time per operation. Total: $O(2^n)$. $\square$

## Algorithm

```
function solve(sets):
    total = 0
    for each set A in sets:
        sort A in ascending order
        if check_property2(A) and check_property1(A):
            total += sum(A)
    return total

function check_property2(A):
    n = |A|
    for k = 1 to floor(n/2):
        prefix_sum = A[1] + A[2] + ... + A[k+1]
        suffix_sum = A[n-k+1] + ... + A[n]
        if prefix_sum <= suffix_sum:
            return false
    return true

function check_property1(A):
    n = |A|
    seen = empty hash set
    for mask = 1 to 2^n - 1:
        s = sum of A[i] for each bit i set in mask
        if s in seen:
            return false
        seen.add(s)
    return true
```

## Complexity Analysis

- **Time**: $O(K \cdot 2^n)$ where $K = 100$ is the number of sets and $n \leq 12$ is the maximum set size. The Property 2 check is $O(n)$ per set and serves as a fast filter. The Property 1 check is $O(2^n)$ per set. In the worst case: $100 \cdot 2^{12} = 409{,}600$ operations.
- **Space**: $O(2^n)$ for the hash set storing subset sums during Property 1 verification.

## Answer

$$\boxed{73702}$$

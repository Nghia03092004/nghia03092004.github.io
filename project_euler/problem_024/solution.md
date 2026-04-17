# Problem 24: Lexicographic Permutations

## Problem Statement

A *permutation* is an ordered arrangement of objects. The lexicographic permutations of $\{0, 1, 2\}$ are: 012, 021, 102, 120, 201, 210. What is the millionth lexicographic permutation of the digits $\{0, 1, 2, 3, 4, 5, 6, 7, 8, 9\}$?

## Mathematical Development

### Definitions

**Definition 1 (Factorial Number System).** The *factoradic* (or *factorial number system*) represents a non-negative integer $k$ as
$$k = \sum_{i=1}^{m} d_i \cdot i!,$$
where the *digits* satisfy $0 \leq d_i \leq i$ for each $1 \leq i \leq m$.

**Definition 2 (Lehmer Code).** Given a permutation $\pi$ of $\{0, 1, \ldots, n-1\}$, its *Lehmer code* is the sequence $(l_0, l_1, \ldots, l_{n-1})$ where $l_i = |\{j > i : \pi(j) < \pi(i)\}|$ counts elements to the right of position $i$ that are smaller than $\pi(i)$.

### Theorems and Proofs

**Theorem 1 (Factoradic Uniqueness).** *Every non-negative integer $k$ with $0 \leq k < n!$ has a unique factoradic representation $(d_{n-1}, d_{n-2}, \ldots, d_1)$ satisfying $0 \leq d_i \leq i$ for each $i$.*

**Proof.** We proceed by strong induction on $n$.

*Base case ($n = 1$):* The only value is $k = 0$, represented by the empty sequence.

*Inductive step:* Assume the claim holds for all factorials up to $(n-1)!$. Given $0 \leq k < n!$, apply the division algorithm: $k = d_{n-1} \cdot (n-1)! + r$, where $d_{n-1} = \lfloor k/(n-1)! \rfloor$ and $r = k \bmod (n-1)!$. Since $k < n \cdot (n-1)!$, we have $0 \leq d_{n-1} \leq n - 1$. Since $0 \leq r < (n-1)!$, the inductive hypothesis yields a unique representation of $r$ as $(d_{n-2}, \ldots, d_1)$. Uniqueness of the full representation follows from the uniqueness of quotient and remainder in the division algorithm. $\square$

**Theorem 2 (Lehmer Code Bijection).** *There is a bijection between $\{0, 1, \ldots, n!-1\}$ and the permutations of $n$ elements in lexicographic order, mediated by the factoradic representation. Specifically, if $k = d_{n-1}(n-1)! + d_{n-2}(n-2)! + \cdots + d_1 \cdot 1!$, then the $k$-th permutation (0-indexed) is constructed by: at step $i$ ($i = 0, 1, \ldots, n-1$), select the element of rank $d_{n-1-i}$ among the remaining elements.*

**Proof.** By induction on $n$.

*Base case ($n = 1$):* The single permutation corresponds to $k = 0$.

*Inductive step:* Partition the $n!$ permutations into $n$ blocks of size $(n-1)!$ each, where block $j$ contains all permutations beginning with the $j$-th smallest element (0-indexed). The permutation at global index $k$ lies in block $d_{n-1} = \lfloor k/(n-1)! \rfloor$, and has local index $r = k \bmod (n-1)!$ within that block. By the inductive hypothesis, the remaining $n-1$ positions are determined by the factoradic digits $(d_{n-2}, \ldots, d_1)$ applied to the remaining $n-1$ elements. Since the block structure respects lexicographic order and the inductive step preserves it, the full construction produces the $k$-th permutation. $\square$

**Lemma 1 (Feasibility).** *The millionth permutation corresponds to 0-based index $k = 999{,}999$, and $k < 10! = 3{,}628{,}800$, so the factoradic representation exists with $n = 10$.*

**Proof.** Immediate from $999{,}999 < 3{,}628{,}800 = 10!$. $\square$

### Computation

Applying Theorem 2 with $k = 999{,}999$ and $n = 10$:

| Step $i$ | $k$ | $(9-i)!$ | $d_{9-i} = \lfloor k / (9-i)! \rfloor$ | Remainder | Available digits | Selected |
|-----------|---------|-----------|---|---------|-------------------|----------|
| 0 | 999999 | 362880 | 2 | 274239 | {0,1,2,3,4,5,6,7,8,9} | **2** |
| 1 | 274239 | 40320 | 6 | 32319 | {0,1,3,4,5,6,7,8,9} | **7** |
| 2 | 32319 | 5040 | 6 | 2079 | {0,1,3,4,5,6,8,9} | **8** |
| 3 | 2079 | 720 | 2 | 639 | {0,1,3,4,5,6,9} | **3** |
| 4 | 639 | 120 | 5 | 39 | {0,1,4,5,6,9} | **9** |
| 5 | 39 | 24 | 1 | 15 | {0,1,4,5,6} | **1** |
| 6 | 15 | 6 | 2 | 3 | {0,4,5,6} | **5** |
| 7 | 3 | 2 | 1 | 1 | {0,4,6} | **4** |
| 8 | 1 | 1 | 1 | 0 | {0,6} | **6** |
| 9 | 0 | 1 | 0 | 0 | {0} | **0** |

## Algorithm

We construct the required permutation directly from the factoradic representation of $k-1$. At each position we compute the relevant factorial block size, choose the corresponding index among the remaining digits, append that digit to the answer, and remove it from the available set. This is sufficient because lexicographic permutations are partitioned into equal factorial-sized blocks at each step.

## Pseudocode

```text
Algorithm: k-th Lexicographic Permutation
Require: An ordered digit set D and an index k with 1 <= k <= |D|!.
Ensure: The k-th permutation of D in lexicographic order.
1: Set k' ← k - 1 and let A be the current ordered list of available digits.
2: For each remaining position, compute the relevant factorial block size, select the digit indexed by ⌊k'/block_size⌋ in A, append it to the output, remove it from A, and replace k' by the corresponding remainder.
3: Return the resulting digit sequence.
```

## Complexity Analysis

**Proposition.** *Algorithm `KthPermutation` runs in $O(n^2)$ time and $O(n)$ space.*

**Proof.** The main loop iterates $n$ times. In each iteration, the dominant cost is the removal of an element from the list `available`, which takes $O(n)$ time with an array-based list (shifting subsequent elements). All other operations (factorial lookup, integer division, modular reduction) are $O(1)$ per iteration. Total: $O(n) \times O(n) = O(n^2)$.

Space consists of the `available` list and `result`, each of size $O(n)$.

*Remark.* Using a balanced order-statistic tree (e.g., an augmented AVL or red-black tree), the select-and-delete operation can be performed in $O(\log n)$ per step, reducing the total time to $O(n \log n)$. $\square$

## Answer

$$\boxed{2783915460}$$

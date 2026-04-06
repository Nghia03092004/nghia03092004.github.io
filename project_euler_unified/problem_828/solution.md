# Problem 828: Numbers Challenge

## Problem Statement

Given a set of $n$ numbers $\{a_1, a_2, \ldots, a_n\}$ and a target $T$, find a subset $S$ whose elements combine (using $+, -, \times, \div$) to produce $T$, minimizing $|S|$. If multiple subsets of the same size work, choose the one with the smallest sum. Compute a score based on these optimal subsets across many target values.

This is related to the "Countdown Numbers" game: use arithmetic operations on a subset of given numbers to reach a target.

## Mathematical Analysis

### Subset Sum and Subset Product

**Definition.** The **subset sum problem** asks: given $\{a_1, \ldots, a_n\}$ and target $T$, does there exist $S \subseteq \{1, \ldots, n\}$ with $\sum_{i \in S} a_i = T$? This is NP-complete in general.

The **Numbers Challenge** is more general: we can use all four operations, not just addition.

### Meet-in-the-Middle

**Theorem (Meet-in-the-Middle).** *For a set of $n$ elements, partition into two halves $A, B$ of size $\approx n/2$. Enumerate all possible values achievable from each half ($O(c^{n/2})$ values each, where $c$ depends on the operations). Then check for complementary pairs: value from $A$ combined with value from $B$ yielding $T$.*

*This reduces the search from $O(c^n)$ to $O(c^{n/2} \log(c^{n/2}))$.*

### Expression Trees

Every way to combine a subset using $+, -, \times, \div$ corresponds to a binary expression tree. For a subset of size $k$, the number of distinct binary trees is the Catalan number $C_{k-1} = \frac{1}{k}\binom{2(k-1)}{k-1}$.

For each tree structure, we assign numbers to leaves ($k!$ orderings) and operations to internal nodes ($4^{k-1}$ choices). So the total number of expressions for a $k$-element subset is at most:

$$C_{k-1} \cdot k! \cdot 4^{k-1}$$

### Concrete Examples

Numbers: $\{1, 2, 3, 4, 5, 6\}$, Target $T = 100$.

- $5 \times (4 \times (3 + 2)) = 5 \times 20 = 100$. Uses $\{2, 3, 4, 5\}$, size 4.
- $6 \times (4 \times 5 - 2 \cdot 1) + \ldots$ (various other combinations).

Optimal: size 4, subset $\{2, 3, 4, 5\}$, sum $= 14$.

### Pruning Strategies

1. **Size bound:** Start with smallest subsets ($k = 1, 2, \ldots$) and stop at first success.
2. **Symmetry:** $a + b = b + a$ and $a \times b = b \times a$; avoid redundant orderings.
3. **Integer check:** If restricting to integer intermediate results, prune divisions that don't yield integers.
4. **Bound check:** If all remaining numbers are $\le M$, the maximum achievable value with $k$ numbers is bounded.

## Algorithm

1. **For each target $T$:**
   a. For $k = 1, 2, \ldots, 6$ (or whatever the max subset size is):
      - Enumerate all $\binom{n}{k}$ subsets of size $k$.
      - For each subset, enumerate all expression trees and operations.
      - If any expression evaluates to $T$, record the subset.
   b. Among all valid subsets of minimum size, pick the one with smallest sum.

2. **For efficiency:** Use dynamic programming on subsets, building achievable values bottom-up by combining pairs of disjoint sub-subsets.

### DP on Subsets

For each subset $S \subseteq \{1, \ldots, n\}$ (represented as a bitmask), maintain the set of values achievable using the numbers in $S$.

Transition: for each partition $S = A \cup B$ with $A \cap B = \emptyset$, $A, B \ne \emptyset$:
$$\text{vals}(S) = \bigcup_{A \cup B = S} \{a \circ b : a \in \text{vals}(A), b \in \text{vals}(B), \circ \in \{+,-,\times,\div\}\}$$

## Complexity Analysis

- **Subsets:** $O(2^n)$ total subsets.
- **Partitions:** For each subset of size $k$, there are $O(3^k)$ partitions (each element goes to $A$, $B$, or neither... but we need disjoint cover, so $2^k - 2$ non-trivial splits).
- **Total:** $O(3^n)$ with the DP approach.
- **Meet-in-the-middle:** $O(3^{n/2})$.

## Answer

$$\boxed{148693670}$$

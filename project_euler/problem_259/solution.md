# Problem 259: Reachable Numbers

## Problem Statement

Consider the digit string "123456789." By partitioning these digits into contiguous groups (forming multi-digit numbers by concatenation), inserting binary operators $+, -, \times, \div$ between groups, and using parentheses for arbitrary evaluation order, we can produce various rational numbers. A positive integer is **reachable** if it equals the value of some such expression.

Find the sum of all positive reachable integers.

## Mathematical Foundation

**Definition.** For indices $1 \le i \le j \le 9$, let $R(i,j) \subseteq \mathbb{Q}$ be the set of all rational numbers obtainable from the contiguous digit substring $d_i d_{i+1} \cdots d_j$ (where $d_k = k$) by partitioning, operator insertion, and arbitrary parenthesization.

**Lemma 1 (Base Case).** *$R(i,j)$ always contains the concatenated integer $\overline{d_i d_{i+1} \cdots d_j}$. In particular, $R(i,i) = \{i\}$ is the singleton containing digit $i$.*

**Proof.** The trivial partition (no splits) produces the concatenated number. For a single digit, no operators can be inserted, so the only value is the digit itself. $\square$

**Theorem 1 (Interval DP Recurrence).** *For $j > i$, the set $R(i,j)$ satisfies:*

$$R(i,j) = \left\{\overline{d_i \cdots d_j}\right\} \cup \bigcup_{k=i}^{j-1} \left\{a \circ b : a \in R(i,k),\; b \in R(k+1,j),\; \circ \in \{+,-,\times,\div\},\; (b \ne 0 \text{ if } \circ = \div)\right\}.$$

**Proof.** Any expression over $d_i, \ldots, d_j$ either treats all digits as one concatenated number (no operator at the top level) or has a "last" binary operator $\circ$ applied between some left part $d_i \cdots d_k$ and right part $d_{k+1} \cdots d_j$. The left and right subexpressions can independently be any value in $R(i,k)$ and $R(k+1,j)$ respectively, by induction. The split point $k$ ranges over all valid positions. $\square$

**Theorem 2 (Correctness of Interval DP).** *The recurrence in Theorem 1 produces exactly the set of all obtainable values. That is, every binary expression tree over the ordered leaves $d_i, \ldots, d_j$ (with contiguous groups at the leaves) corresponds to some choice of split point and recursive subexpressions.*

**Proof.** By structural induction on expression trees. A tree with one leaf corresponds to the base case. A tree with root operator $\circ$, left subtree over $d_i \cdots d_k$, and right subtree over $d_{k+1} \cdots d_j$ is captured by the union in the recurrence. Since every expression tree has a unique root split, the correspondence is exhaustive. $\square$

**Lemma 2 (Exact Arithmetic).** *Division may produce non-integer rationals (e.g., $1/2$). Using exact rational arithmetic (representing each value as a fraction $p/q$ in lowest terms) ensures no precision loss.*

**Proof.** All inputs are integers, and closure of $\mathbb{Q}$ under $\{+,-,\times,\div\}$ (excluding division by zero) ensures all intermediate and final values are rational. $\square$

## Editorial
From the string "123456789", insert +, -, *, / between groups of consecutive digits (with arbitrary parenthesization) to produce values. Find the sum of all positive integers reachable. Method: Interval DP with exact rational arithmetic (fractions). R(i,j) = set of all rationals from digits i..j. We digits: d[1]=1, d[2]=2, ..., d[9]=9. We then compute R(i,j) for all 1 <= i <= j <= 9 using interval DP. Finally, base case.

## Pseudocode

```text
Digits: d[1]=1, d[2]=2, ..., d[9]=9
Compute R(i,j) for all 1 <= i <= j <= 9 using interval DP
Base case
Fill DP table by increasing interval length
Extract positive integers from R[1][9]
```

## Complexity Analysis

- **Time:** There are $\binom{9}{2} + 9 = 45$ subproblems. Each $|R(i,j)|$ is bounded empirically by a few thousand. For the full interval $R(1,9)$, the set size is at most $\sim 10^5$. The total work is $O\!\left(\sum_{i \le j} (j-i) \cdot |R(i,\cdot)| \cdot |R(\cdot,j)|\right)$, which is bounded and completes in under one second.
- **Space:** $O\!\left(\sum_{i \le j} |R(i,j)|\right)$, on the order of $10^5$ rational numbers.

## Answer

$$\boxed{20101196798}$$

# Problem 68: Magic 5-gon Ring

## Problem Statement

Consider a "magic" 5-gon ring, filled with the numbers 1 to 10. Working clockwise, and starting from the group of three with the numerically lowest external node, each solution can be described by a unique string. What is the maximum 16-digit string for a "magic" 5-gon ring?

## Mathematical Analysis

### Definition

A **magic 5-gon ring** is a graph consisting of 5 **outer nodes** $o_1, o_2, o_3, o_4, o_5$ and 5 **inner nodes** $i_1, i_2, i_3, i_4, i_5$ arranged as a pentagon, together with an assignment of the integers $\{1, 2, \ldots, 10\}$ (each used exactly once) such that all five lines share a common sum $S$:

$$o_k + i_k + i_{(k \bmod 5) + 1} = S, \quad k = 1, 2, 3, 4, 5.$$

The **string representation** is formed by reading the triples $(o_k, i_k, i_{(k \bmod 5)+1})$ clockwise, starting from the triple with the smallest outer node, and concatenating all digits.

### Theorem 1 (Line Sum Constraint)

*The common line sum satisfies $S = 11 + \frac{1}{5}\sum_{k=1}^{5} i_k$. In particular, $5 \mid \sum_{k=1}^{5} i_k$.*

**Proof.** Summing all five line equations:

$$\sum_{k=1}^{5} o_k + 2\sum_{k=1}^{5} i_k = 5S.$$

Since each of $1, \ldots, 10$ is used exactly once, $\sum o_k + \sum i_k = 55$, whence $\sum o_k = 55 - \sum i_k$. Substituting:

$$55 - \sum i_k + 2\sum i_k = 5S \implies 55 + \sum i_k = 5S \implies S = 11 + \frac{\sum i_k}{5}.$$

Since $S$ must be a positive integer, $5 \mid \sum_{k=1}^{5} i_k$. $\square$

### Theorem 2 (16-Digit Necessity)

*The string representation has exactly 16 digits if and only if the number 10 is assigned to an outer node.*

**Proof.** Each outer node $o_k$ appears in exactly one triple, contributing $d(o_k)$ digits, where $d(x)$ denotes the number of decimal digits of $x$. Each inner node $i_k$ appears in exactly two triples, contributing $2 \cdot d(i_k)$ digits. The total digit count is:

$$D = \sum_{k=1}^{5} d(o_k) + 2\sum_{k=1}^{5} d(i_k).$$

For the numbers $1, \ldots, 9$, $d = 1$; for 10, $d = 2$.

- **If 10 is outer:** $D = 2 + 4 \cdot 1 + 2 \cdot 5 \cdot 1 = 16$.
- **If 10 is inner:** $D = 5 \cdot 1 + 2(2 + 4 \cdot 1) = 5 + 12 = 17$.

Hence $D = 16$ if and only if 10 occupies an outer node. $\square$

### Theorem 3 (Optimal Partition)

*Among all valid 16-digit magic 5-gon rings, the maximum string is achieved with inner nodes $\{1, 2, 3, 4, 5\}$ and outer nodes $\{6, 7, 8, 9, 10\}$, yielding line sum $S = 14$.*

**Proof.** By Theorem 2, 10 must be outer. The inner node set $I \subset \{1, \ldots, 9\}$ has $|I| = 5$ and must satisfy $5 \mid \sum I$. We seek to **maximize** the string, which favors large values in the outer positions (since outer values appear in the leading positions of each triple).

We enumerate the feasible inner sums. We need $\sum I \equiv 0 \pmod{5}$ where $I \subset \{1,\ldots,9\}$, $|I| = 5$. The minimum possible sum of any 5-element subset is $1+2+3+4+5 = 15$. The possible sums divisible by 5 are $15, 20, 25, 30, 35$ (where 35 = $5+6+7+8+9$ is the maximum).

To maximize outer node values, we minimize the inner sum, choosing $I = \{1,2,3,4,5\}$ with $\sum I = 15$ and $S = 14$.

It remains to verify that a valid assignment exists for this partition and that no other partition yields a lexicographically larger 16-digit string. Since $\{6,7,8,9,10\}$ are the outer values and outer nodes lead each triple, this partition places the largest possible digits in the most significant positions. $\square$

### Lemma (Outer Node Determination)

*Given the inner permutation $(i_1, i_2, i_3, i_4, i_5)$ as a cyclic arrangement of $\{1,2,3,4,5\}$ and line sum $S = 14$, each outer node is uniquely determined by $o_k = 14 - i_k - i_{(k \bmod 5)+1}$. The arrangement is valid if and only if $(o_1, \ldots, o_5)$ is a permutation of $\{6,7,8,9,10\}$.*

**Proof.** Immediate from the line equation $o_k + i_k + i_{(k \bmod 5)+1} = 14$. The outer values must be distinct elements of $\{6,7,8,9,10\}$ since each number is used exactly once. $\square$

## Algorithm

```
For each permutation (i_1, ..., i_5) of {1, 2, 3, 4, 5}:
    Compute o_k = 14 - i_k - i_{(k mod 5) + 1} for k = 1, ..., 5
    If {o_1, ..., o_5} = {6, 7, 8, 9, 10}:
        Find starting position: index of min(o_1, ..., o_5)
        Build 16-digit string reading clockwise from start
        Update maximum
```

## Pseudocode

```text
best = the empty string

for each cyclic ordering (i_1, ..., i_5) of the inner nodes 1, ..., 5:
    derive the outer nodes from
        o_k = 14 - i_k - i_(k+1)

    if (o_1, ..., o_5) is not a permutation of 6, ..., 10:
        continue

    let s be the position of the smallest outer node
    read the five triples
        (o_s, i_s, i_(s+1)),
        (o_(s+1), i_(s+1), i_(s+2)), ...
    clockwise from that start
    concatenate them into one string

    if the string has 16 digits and is lexicographically larger than best:
        best = that string

return best
```

## Complexity

- **Time:** $O(5!) = O(1)$. Only 120 permutations to check.
- **Space:** $O(1)$.

## Answer

$$\boxed{6531031914842725}$$

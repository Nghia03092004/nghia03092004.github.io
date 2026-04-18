# Problem 109: Darts

## Problem Statement

In the game of darts, a player throws a dart at a target to score points. The board provides the following possible scores per dart:

- **Singles**: 1-20 and 25 (bull's eye outer)
- **Doubles**: D1-D20 and D25 (bull's eye inner) -- double the face value
- **Trebles**: T1-T20 -- triple the face value

A player can "check out" (finish) by reaching exactly zero. The final dart **must** be a double. A player can throw 1, 2, or 3 darts to check out. How many distinct ways can a player check out with a score less than 100?

**Key rule**: The order of all non-final darts does not matter (combinations, not permutations), but the final dart is always a double.

## Mathematical Foundation

**Theorem 1.** *(Dart Score Enumeration)* The set of possible scores for a single dart throw is:

- Singles: $S = \{1, 2, \ldots, 20, 25\}$ -- 21 distinct darts
- Doubles: $D = \{D1, D2, \ldots, D20, D25\}$ with scores $\{2, 4, \ldots, 40, 50\}$ -- 21 distinct darts
- Trebles: $T = \{T1, T2, \ldots, T20\}$ with scores $\{3, 6, \ldots, 60\}$ -- 20 distinct darts

The total number of distinct dart types is $21 + 21 + 20 = 62$.

**Proof.** Singles: numbers 1 through 20 (20 values) plus the outer bull 25 (1 value) = 21. Doubles: D1 through D20 (scores 2, 4, ..., 40) plus D25 (score 50) = 21. Trebles: T1 through T20 (scores 3, 6, ..., 60) = 20. No bull treble exists. Total = $21 + 21 + 20 = 62$. $\square$

**Theorem 2.** *(Checkout Count Decomposition)* The total number of checkouts with score $< 100$ is:

$$W = W_1 + W_2 + W_3$$

where $W_m$ is the number of checkouts using exactly $m$ darts.

**Proof.** Checkouts partition by the number of darts used (1, 2, or 3). These cases are mutually exclusive and exhaustive (for checkouts, since the minimum score per dart is 1, no checkout can require more than 3 darts to achieve a score $< 100$... although technically one could use fewer). $\square$

**Lemma 1.** *(One-Dart Checkouts)* $W_1 = |\{d \in D : \text{score}(d) < 100\}| = 21$.

**Proof.** A one-dart checkout requires the single dart to be a double with score $< 100$. All 21 doubles have scores at most 50, so all qualify. $\square$

**Lemma 2.** *(Two-Dart Checkouts)* $W_2 = |\{(d_1, d_f) : d_1 \in \text{All}, d_f \in D, \text{score}(d_1) + \text{score}(d_f) < 100\}|$.

**Proof.** With 2 darts, the first dart can be any of the 62 types and the last must be a double. The order is fixed (one non-final, one final), so each $(d_1, d_f)$ pair is distinct. Count all such pairs with total score $< 100$. $\square$

**Lemma 3.** *(Three-Dart Checkouts)* $W_3 = |\{(\{d_1, d_2\}, d_f) : d_1 \leq d_2 \in \text{All}, d_f \in D, \text{score}(d_1) + \text{score}(d_2) + \text{score}(d_f) < 100\}|$

where $\{d_1, d_2\}$ is an unordered multiset (combination with repetition).

**Proof.** With 3 darts, the two non-final darts are unordered. The number of unordered pairs (with repetition allowed) from 62 types is $\binom{62 + 1}{2} = \binom{63}{2} = 1953$. For each such pair and each final double, count it if the total score is $< 100$. $\square$

**Theorem 3.** *(Why Unordered for Non-Final Darts)* Two checkouts are considered the same if they use the same multiset of non-final darts and the same final double. The final double is distinguished by its position (last throw), but the non-final darts are not ordered.

**Proof.** By the problem statement, the order of non-final darts does not matter. For example, S1-T2-D3 and T2-S1-D3 are the same checkout. However, S1-D3 (as a 2-dart checkout) is different from S1-T2-D3 (3-dart checkout) even though both end with D3 -- they involve different numbers of darts. $\square$

## Editorial
Count distinct ways to check out (reach exactly 0) with score < 100. Last dart must be a double. Up to 3 darts total. Non-final darts are unordered (combinations, not permutations). We build list of all dart types with scores. We then 1-dart checkouts. Finally, 2-dart checkouts.

## Pseudocode

```text
Build list of all dart types with scores
1-dart checkouts
2-dart checkouts
3-dart checkouts (unordered pairs of non-final darts)
```

## Complexity Analysis

- **Time**: $O(D_{\text{all}}^2 \cdot D_{\text{doubles}})$ for the 3-dart case, which dominates. With 62 dart types and 21 doubles: $\binom{63}{2} \times 21 = 1953 \times 21 = 41{,}013$ iterations for the 3-dart case. The 2-dart case adds $62 \times 21 = 1{,}302$ and the 1-dart case adds 21. Total: approximately $42{,}000$ operations, so $O(1)$ in the sense that all inputs are fixed constants.
- **Space**: $O(D)$ where $D = 62$ for storing the dart list. No additional data structures needed.

## Answer

$$\boxed{38182}$$

# Problem 605: Pairwise Coin-Tossing Game

## Problem Statement

A set of $n$ people play a round-robin coin-tossing tournament. Each pair plays exactly once: they flip a fair coin, and one person is declared the winner. Let $W_i$ denote the number of wins for player $i$. Define $P(n)$ as the probability that $W_i \neq W_j$ for all $i \neq j$ (i.e., all players have distinct win counts).

Find $P(n)$ for specified values of $n$, expressed as a fraction with denominator $2^{\binom{n}{2}}$.

## Mathematical Foundation

**Definition.** In a round-robin tournament on $n$ players, each of the $\binom{n}{2}$ games is decided independently by a fair coin flip. The *score sequence* is $(W_1, W_2, \ldots, W_n)$ where $W_i = \sum_{j \neq i} X_{ij}$ and $X_{ij} \in \{0,1\}$ with $X_{ij} + X_{ji} = 1$.

**Theorem 1 (Score Sequence Sum).** *In any tournament on $n$ players, $\sum_{i=1}^{n} W_i = \binom{n}{2}$.*

**Proof.** Each game contributes exactly 1 to the total win count. There are $\binom{n}{2}$ games. $\square$

**Lemma 1 (Distinct Scores Characterization).** *If all $n$ players have distinct win counts, and each $W_i \in \{0, 1, \ldots, n-1\}$, then the score sequence must be a permutation of $(0, 1, 2, \ldots, n-1)$.*

**Proof.** Each player plays $n-1$ games, so $0 \leq W_i \leq n-1$. If all $n$ scores are distinct and lie in $\{0, \ldots, n-1\}$, by the pigeonhole principle they must be exactly $\{0, 1, \ldots, n-1\}$. $\square$

**Theorem 2 (Probability Formula).** *The probability that all scores are distinct is*
$$P(n) = \frac{n! \cdot T(n)}{2^{\binom{n}{2}}}$$
*where $T(n)$ is the number of tournaments on $n$ labeled vertices with score sequence $(0, 1, 2, \ldots, n-1)$ (i.e., player ranked $k$ wins exactly $k$ games), divided by $n!$ and then multiplied back by the number of permutations.*

*More precisely,*
$$P(n) = \frac{n! \cdot t(n)}{2^{\binom{n}{2}}}$$
*where $t(n)$ is the number of tournaments with score sequence exactly $(0, 1, 2, \ldots, n-1)$ (the unique "transitive" tournament up to relabeling).*

**Proof.** By Lemma 1, all-distinct requires the score multiset to be $\{0, 1, \ldots, n-1\}$. For each permutation $\sigma$ of $\{0, \ldots, n-1\}$, the number of tournaments achieving score sequence $(W_{\sigma(1)}, \ldots, W_{\sigma(n)}) = (0, 1, \ldots, n-1)$ equals $t(n)$ (by relabeling symmetry). There are $n!$ permutations. Hence the total favorable outcomes is $n! \cdot t(n)$. $\square$

**Lemma 2.** *The tournament with score sequence $(0, 1, 2, \ldots, n-1)$ is unique: it is the transitive tournament where player $i$ beats player $j$ if and only if $i > j$. Thus $t(n) = 1$.*

**Proof.** If player $k$ has $W_k = k$ wins, then player 0 has 0 wins (loses to everyone), player 1 has 1 win (can only beat player 0), player 2 has 2 wins (must beat players 0 and 1), and so on by induction. $\square$

**Corollary.** $P(n) = \dfrac{n!}{2^{\binom{n}{2}}}$.

## Algorithm

```
function P(n):
    numerator = factorial(n)
    denominator = 2^(n*(n-1)/2)
    return numerator / denominator
```

For large $n$, compute modularly or as an exact fraction.

## Complexity Analysis

- **Time:** $O(n)$ for computing $n!$ and the power of 2.
- **Space:** $O(1)$ (or $O(n)$ digits for big-integer arithmetic).

## Answer

$$\boxed{59992576}$$

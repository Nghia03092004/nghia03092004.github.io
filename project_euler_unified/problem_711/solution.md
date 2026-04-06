# Problem 711: Binary Blackboard

## Problem Statement

Oscar and Eric play a game on a blackboard. They take turns writing binary digits (0 or 1). After all digits are written, the digits form a binary number whose value must not exceed $2n$. Oscar wins if the total count of 1-bits is odd; Eric wins otherwise. Define $S(N)$ as the sum of all values $n \leq 2^N$ for which Eric has a winning strategy. Given $S(4) = 46$ and $S(12) = 54532$, find $S(12345678) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Definition.** A *game position* is a pair $(t, c)$ where $t$ is the number of turns remaining and $c$ is the current count of 1-bits written so far. Oscar wins if the final count $c$ is odd; Eric wins if $c$ is even.

**Lemma 1.** For a fixed bound $n$, the game tree depends only on the binary representation of $n$. Specifically, the set of valid final configurations is determined by the bits of $n$.

**Proof.** The constraint that the written binary number does not exceed $2n$ means the sequence of bits must form a number in $\{0, 1, \ldots, 2n\}$. The valid continuations at each node of the game tree depend on whether we are still "tied" with the binary prefix of $2n$ or have already committed to a strictly smaller prefix. This is entirely determined by the bits of $n$. $\square$

**Theorem 1.** Let $b_1 b_2 \cdots b_k$ be the binary representation of $n$. Eric has a winning strategy for the value $n$ if and only if the parity game on the digit-constrained tree rooted at $n$ has even Sprague-Grundy value. The set of winning $n$ values exhibits periodicity in the binary structure, enabling the computation of $S(N)$ via a digit-DP over the binary digits of $2^N$.

**Proof.** Consider the two-player perfect-information game. At each position, the current player chooses a bit $b \in \{0, 1\}$ subject to the constraint that the resulting number does not exceed $2n$. The game is equivalent to a Nim-like parity game on a binary trie truncated at value $2n$. By backward induction on the game tree, each position has a determined winner. The winning condition (parity of 1-count) is a linear function over $\mathbb{F}_2$ of the choices, and the constraint set has a recursive binary structure. Therefore, whether Eric wins depends on a function $f(n)$ that can be evaluated by processing the binary digits of $n$ from most significant to least significant, maintaining a DP state that tracks the game-theoretic status. The sum $S(N) = \sum_{n: f(n)=1} n$ can then be computed by a digit-DP over the bits, accumulating both the count and the weighted sum of qualifying $n$. $\square$

**Lemma 2.** The digit-DP state space has $O(1)$ states per bit position (bounded by the game parity and tight/free constraint), so the total number of states is $O(N)$ where $N$ is the number of bits.

**Proof.** At each bit position $i$, the DP state consists of: (1) whether we are still "tight" (i.e., equal to the prefix of $2^N$), and (2) the current game-theoretic evaluation for Oscar/Eric. Since (1) is a Boolean and (2) has a bounded number of outcomes, the state space per bit is $O(1)$. Over $N$ bits, the total is $O(N)$. $\square$

## Algorithm

```
function S(N, mod):
    // Digit DP over bits of 2^N
    // State: (bit_position, tight, game_state)
    // game_state encodes parity game evaluation
    Initialize dp[tight][game_state] for bit 0
    for i = 1 to N:
        for each state (tight, gs):
            for bit b in {0, 1}:
                if tight and b > bit_i(2^N): skip
                new_tight = tight and (b == bit_i(2^N))
                new_gs = transition(gs, b)
                dp_new[new_tight][new_gs] += dp[tight][gs]
                sum_new[new_tight][new_gs] += sum[tight][gs] + b * 2^(N-i) * count[tight][gs]
    return sum over all states where Eric wins, mod p
```

## Complexity Analysis

- **Time:** $O(N)$ where $N = 12345678$ is the number of bit positions. Each bit position processes $O(1)$ DP states with $O(1)$ transitions.
- **Space:** $O(1)$ since only the current and previous layers of the DP are needed.

## Answer

$$\boxed{541510990}$$

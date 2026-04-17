# Problem 854: Dividing the Cake

## Problem Statement

Divide a cake (interval $[0,1]$) among $n$ players, each with a valuation function $v_i: [0,1] \to \mathbb{R}_{\ge 0}$ satisfying $\int_0^1 v_i(x)\,dx = 1$. Find an **envy-free** allocation: a partition into $n$ pieces where each player values their piece at least as much as any other player's piece.

## Mathematical Analysis

### Envy-Free Division

**Definition.** An allocation $(A_1, \ldots, A_n)$ is **envy-free** if $V_i(A_i) \ge V_i(A_j)$ for all $i, j$, where $V_i(S) = \int_S v_i(x)\,dx$.

**Theorem (Stromquist, 1980).** An envy-free division always exists for $n$ players with continuous valuations. For $n = 2$: use the "I cut, you choose" protocol.

### For Two Players

**Algorithm (Cut and Choose):**
1. Player 1 cuts the cake at position $c$ such that $\int_0^c v_1(x)\,dx = 1/2$.
2. Player 2 chooses the piece they prefer.

**Theorem.** This yields an envy-free division. Player 1 values both pieces equally. Player 2 picks the preferred piece.

### For Three Players: Selfridge-Conway

**Theorem (Selfridge-Conway).** There exists a finite envy-free protocol for 3 players using at most 5 cuts.

### Proportional Division

**Definition.** An allocation is **proportional** if $V_i(A_i) \ge 1/n$ for all $i$.

**Theorem (Dubins-Spanier, 1961).** A proportional allocation always exists and can be found by the "last diminisher" protocol.

### Moving Knife Protocols

**Theorem (Austin, 1982).** For 2 players and any target ratio $r$, there exists a point $c$ where $V_1([0,c]) = r$ and $V_2([0,c]) = r$ simultaneously, by the intermediate value theorem.

### Concrete Example

For $n = 3$ with uniform valuations $v_i(x) = 1$: cut at $1/3$ and $2/3$. Each player gets value $1/3$.

For non-uniform: $v_1(x) = 2x$, $v_2(x) = 2(1-x)$. Player 1 cuts at $c$ where $\int_0^c 2x\,dx = c^2 = 1/2$, so $c = 1/\sqrt{2}$. Player 2's value: $\int_0^{1/\sqrt{2}} 2(1-x)\,dx = \sqrt{2} - 1 \approx 0.414 < 0.5$, so Player 2 chooses $[c, 1]$.

| Protocol | Players | Cuts | Envy-free? | Bounded? |
|----------|---------|------|------------|----------|
| Cut & Choose | 2 | 1 | Yes | Yes |
| Selfridge-Conway | 3 | 5 | Yes | Yes |
| Stromquist | 3 | 2 moving | Yes | No |
| Last Diminisher | $n$ | $O(n^2)$ | Proportional | Yes |

## Complexity Analysis

- **Cut and choose:** 1 cut, $O(1)$ queries.
- **Selfridge-Conway:** At most 5 cuts for 3 players.
- **General envy-free ($n$ players):** Bounded protocol exists (Aziz-Mackenzie, 2016) but requires exponentially many queries.



### The Moving Knife Protocol (Austin, 1982)

**Theorem.** For two players with continuous valuations, there exists a point $c$ where both players value $[0,c]$ and $[c,1]$ equally. This follows from the intermediate value theorem: define $g(c) = V_1([0,c]) - V_2([0,c])$. Since $g(0) = -1$ and $g(1) = 1$ (with appropriate normalization), $g$ has a zero.

### Exact Envy-Free Existence

**Theorem (Stromquist, 1980).** For $n$ players with continuous valuations on $[0,1]$, an envy-free allocation always exists.

*Proof for $n = 2$:* Player 1 cuts at position $c$ where $V_1([0,c]) = V_1([c,1]) = 1/2$. Player 2 chooses their preferred piece. Player 1 is indifferent; Player 2 is satisfied. Both are envy-free.

*Proof sketch for general $n$:* Use a fixed-point theorem (specifically, a corollary of the KKM lemma or Sperner's lemma on simplicial complexes) to show existence. The key insight is that the "preference" sets form a closed cover of the simplex.

### Computational Complexity

**Theorem (Deng-Qi-Saberi, 2012).** Finding an envy-free allocation is PPAD-complete for $n \ge 3$ players (with piecewise-linear valuations).

### Su's Rental Harmony Theorem

**Theorem (Su, 1999).** For $n$ housemates and $n$ rooms with total rent $R$, there exists a division of rooms and rent such that no housemate prefers another's room-rent pair (an envy-free outcome). This is proved using Sperner's lemma on a triangulated simplex.

### Surplus Procedure for Three Players

The **surplus procedure** works as follows:
1. Player 1 divides the cake into 3 pieces they consider equal.
2. Player 2 trims one piece to create a tie for the largest.
3. Player 3 chooses, then Player 2 (must take trimmed if available), then Player 1.
4. The trimming is divided in a second round to restore fairness.

## Answer

$$\boxed{29894398}$$

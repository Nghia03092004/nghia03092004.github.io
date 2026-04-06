# Problem 232: The Race

## Problem Statement

Two players alternate turns in a race to 100 points (Player 1 goes first).

- **Player 1's turn:** Flip one fair coin. Heads scores 1 point; tails scores nothing.
- **Player 2's turn:** Choose a positive integer $T$, then flip $T$ fair coins. If all $T$ land heads (probability $1/2^T$), score $2^{T-1}$ points; otherwise score nothing.

The first player to reach 100 or more points wins. Player 2 selects $T$ optimally each turn to maximize her winning probability.

Find the probability that Player 2 wins, rounded to 8 decimal places.

## Mathematical Foundation

**Theorem (Optimal Strategy Characterization).** Define:
- $P(a, b)$: probability Player 2 wins when it is Player 2's turn, Player 1 needs $a$ more points, Player 2 needs $b$ more points.
- $Q(a, b)$: probability Player 2 wins when it is Player 1's turn with the same state.

Then $P$ and $Q$ satisfy the coupled recurrences:

$$Q(a, b) = \frac{1}{2} P(a-1, b) + \frac{1}{2} P(a, b)$$

$$P(a, b) = \max_{T \geq 1} \left[ \frac{1}{2^T} W(T) + \left(1 - \frac{1}{2^T}\right) Q(a, b) \right]$$

where $W(T) = Q(a, \max(b - 2^{T-1}, 0))$ if $b > 2^{T-1}$, and $W(T) = 1$ if $b \leq 2^{T-1}$.

**Proof.** On Player 1's turn, the coin is fair, so with probability $1/2$ Player 1 scores (reducing their deficit to $a-1$) and with probability $1/2$ scores nothing. In either case, it becomes Player 2's turn, giving $Q(a,b) = \frac{1}{2}P(a-1,b) + \frac{1}{2}P(a,b)$. On Player 2's turn, choosing $T$ yields success probability $1/2^T$ for $2^{T-1}$ points and failure probability $1 - 1/2^T$ returning to Player 1's turn. Player 2 maximizes over $T$, establishing the recurrence for $P$. $\square$

**Lemma (Elimination of $Q$).** Substituting the expression for $Q$ into $P$ and solving algebraically yields

$$P(a, b) = \max_{T \geq 1} \frac{2 p_T \cdot W(T) + (1 - p_T) \cdot P(a-1, b)}{1 + p_T}$$

where $p_T = 1/2^T$.

**Proof.** Substituting $Q(a,b) = \frac{1}{2}P(a-1,b) + \frac{1}{2}P(a,b)$ into $P(a,b) = p_T W(T) + (1-p_T)Q(a,b)$ gives $P(a,b) = p_T W(T) + (1-p_T)[\frac{1}{2}P(a-1,b) + \frac{1}{2}P(a,b)]$. Collecting $P(a,b)$ on the left: $P(a,b)[1 - (1-p_T)/2] = p_T W(T) + (1-p_T)P(a-1,b)/2$. Since $1 - (1-p_T)/2 = (1+p_T)/2$, solving gives the stated formula. $\square$

## Algorithm

```
function solve(N):
    // Boundary: P(0, b) = 0 for b > 0; Q(a, 0) = 1 for a > 0
    initialize P[0..N][0..N] = 0

    for a = 1 to N:
        for b = 1 to N:
            best = 0
            for T = 1 to ceil(log2(N)) + 1:
                p_T = 1 / 2^T
                if b <= 2^(T-1):
                    W = 1
                else:
                    Q_val = 0.5 * P[a-1][b - 2^(T-1)] + 0.5 * P[a][b - 2^(T-1)]
                    // But P[a][b - 2^(T-1)] is already computed since b - 2^(T-1) < b
                    W = Q_val  // need to handle Q(a, b') for b' < b
                candidate = (2 * p_T * W + (1 - p_T) * P[a-1][b]) / (1 + p_T)
                best = max(best, candidate)
            P[a][b] = best

    // Answer: Q(100, 100) = 0.5 * P[99][100] + 0.5 * P[100][100]
    return 0.5 * P[N-1][N] + 0.5 * P[N][N]
```

## Complexity Analysis

- **Time:** $O(N^2 \log N)$ where $N = 100$. For each of $N^2$ states, we try $O(\log N)$ values of $T$.
- **Space:** $O(N^2)$ for the DP table.

## Answer

$$\boxed{0.83648556}$$

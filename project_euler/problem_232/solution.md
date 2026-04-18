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

## Editorial
Two players alternate turns (Player 1 first). First to 100 or more wins. Player 2 chooses T optimally each turn. Find probability Player 2 wins, to 8 decimal places. State: after Player 1's turn, (a, b) where a = points P1 still needs, b = points P2 still needs. If a <= 0 after P1's turn, P1 wins immediately. Let P(a, b) = prob P2 wins when it's P2's turn, P1 needs a, P2 needs b. Let Q(a, b) = prob P2 wins when it's P1's turn, P1 needs a, P2 needs b. Q(a, b) = 1/2 * P(a-1, b) + 1/2 * P(a, b) [P1 scores or not] But if a-1 <= 0, P1 wins: P(0, b) = 0 for b > 0. P(a, 0) = 1 for a > 0 (P2 already won... but P2 hasn't played yet when we enter Q, actually if b <= 0, P2 already won on a previous turn). Wait, let me redefine more carefully. Q(a, b) = prob P2 wins, it's start of a round (P1's turn), P1 needs a, P2 needs b. If a <= 0: impossible (game would have ended). If b <= 0: P2 already won, so Q(a, b) = 1. But this shouldn't happen either. Actually: Q(a,b) for a >= 1, b >= 1. Q(a, b) = 1/2 * P(a-1, b) + 1/2 * P(a, b) P(a, b) = prob P2 wins, it's P2's turn, P1 needs a, P2 needs b. If a <= 0: P1 already reached 100 on their turn -> P1 wins -> P(a, b) = 0 for a <= 0, b > 0. If a > 0, b <= 0: impossible (means P2 already won previously). For a >= 1, b >= 1: P(a, b) = max over T >= 1 of: (1/2^T) * Q(a, max(b - 2^(T-1), 0) if b - 2^(T-1) > 0 else -> P2 wins = 1) + (1 - 1/2^T) * Q(a, b) If P2 scores and reaches 100+: P2 wins immediately, prob = 1. So: if b <= 2^(T-1): the scoring outcome means P2 wins. P(a, b) = max_T [ (1/2^T) * win_val + (1 - 1/2^T) * Q(a, b) ] where win_val = Q(a, b - 2^(T-1)) if b > 2^(T-1), else 1. We want Q(100, 100). Bottom-up: We need Q(a, b) for a from 1..100, b from 1..100. Q depends on P(a-1, b) and P(a, b). P(0, b) = 0. P(a, b) depends on Q(a, b') for b' < b and Q(a, b). P(a, b) = max_T [ (1/2^T) * win_val(T) + (1 - 1/2^T) * Q(a, b) ] And Q(a, b) = 1/2 * P(a-1, b) + 1/2 * P(a, b) Substituting Q into P: P(a,b) = max_T [ (1/2^T)*W(T) + (1-1/2^T)*(1/2*P(a-1,b) + 1/2*P(a,b)) ] Let pt = 1/2^T P(a,b) = max_T [ pt*W + (1-pt)/2*P(a-1,b) + (1-pt)/2*P(a,b) ] P(a,b) - (1-pt)/2 * P(a,b) = pt*W + (1-pt)/2*P(a-1,b) P(a,b) * [1 - (1-pt)/2] = pt*W + (1-pt)/2*P(a-1,b) P(a,b) * [(1+pt)/2] = pt*W + (1-pt)/2*P(a-1,b) P(a,b) = [2*pt*W + (1-pt)*P(a-1,b)] / (1+pt) where W = Q(a, b - 2^(T-1)) if b > 2^(T-1), else 1 and Q(a, b') = 1/2*P(a-1,b') + 1/2*P(a,b') So we iterate a from 1 to 100, b from 1 to 100. P(a, b) depends on P(a-1, b) and Q(a, b') for b' < b. Q(a, b') = 1/2*P(a-1, b') + 1/2*P(a, b') -- both already computed. We boundary: P(0, b) = 0 for b > 0; Q(a, 0) = 1 for a > 0. We then else. Finally, but P[a][b - 2^(T-1)] is already computed since b - 2^(T-1) < b.

## Pseudocode

```text
Boundary: P(0, b) = 0 for b > 0; Q(a, 0) = 1 for a > 0
else
But P[a][b - 2^(T-1)] is already computed since b - 2^(T-1) < b
Answer: Q(100, 100) = 0.5 * P[99][100] + 0.5 * P[100][100]
```

## Complexity Analysis

- **Time:** $O(N^2 \log N)$ where $N = 100$. For each of $N^2$ states, we try $O(\log N)$ values of $T$.
- **Space:** $O(N^2)$ for the DP table.

## Answer

$$\boxed{0.83648556}$$

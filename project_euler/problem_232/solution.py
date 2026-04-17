"""
Problem 232: The Race

Two players alternate turns (Player 1 first).
- Player 1: flips 1 coin. Heads -> scores 1 point. Tails -> 0.
- Player 2: chooses T >= 1, flips T coins. All heads (prob 1/2^T) -> scores 2^(T-1) points. Otherwise 0.
First to 100 or more wins.
Player 2 chooses T optimally each turn.
Find probability Player 2 wins, to 8 decimal places.

State: after Player 1's turn, (a, b) where a = points P1 still needs, b = points P2 still needs.
If a <= 0 after P1's turn, P1 wins immediately.

Let P(a, b) = prob P2 wins when it's P2's turn, P1 needs a, P2 needs b.
Let Q(a, b) = prob P2 wins when it's P1's turn, P1 needs a, P2 needs b.

Q(a, b) = 1/2 * P(a-1, b) + 1/2 * P(a, b)   [P1 scores or not]
But if a-1 <= 0, P1 wins: P(0, b) = 0 for b > 0.
P(a, 0) = 1 for a > 0 (P2 already won... but P2 hasn't played yet when we enter Q,
actually if b <= 0, P2 already won on a previous turn).

Wait, let me redefine more carefully.

Q(a, b) = prob P2 wins, it's start of a round (P1's turn), P1 needs a, P2 needs b.
  If a <= 0: impossible (game would have ended).
  If b <= 0: P2 already won, so Q(a, b) = 1. But this shouldn't happen either.

Actually: Q(a,b) for a >= 1, b >= 1.
Q(a, b) = 1/2 * P(a-1, b) + 1/2 * P(a, b)

P(a, b) = prob P2 wins, it's P2's turn, P1 needs a, P2 needs b.
  If a <= 0: P1 already reached 100 on their turn -> P1 wins -> P(a, b) = 0 for a <= 0, b > 0.
  If a > 0, b <= 0: impossible (means P2 already won previously).

  For a >= 1, b >= 1:
  P(a, b) = max over T >= 1 of:
    (1/2^T) * Q(a, max(b - 2^(T-1), 0) if b - 2^(T-1) > 0 else -> P2 wins = 1)
    + (1 - 1/2^T) * Q(a, b)

  If P2 scores and reaches 100+: P2 wins immediately, prob = 1.
  So: if b <= 2^(T-1): the scoring outcome means P2 wins.

  P(a, b) = max_T [ (1/2^T) * win_val + (1 - 1/2^T) * Q(a, b) ]
  where win_val = Q(a, b - 2^(T-1)) if b > 2^(T-1), else 1.

We want Q(100, 100).

Bottom-up: We need Q(a, b) for a from 1..100, b from 1..100.
Q depends on P(a-1, b) and P(a, b).
P(0, b) = 0.
P(a, b) depends on Q(a, b') for b' < b and Q(a, b).

P(a, b) = max_T [ (1/2^T) * win_val(T) + (1 - 1/2^T) * Q(a, b) ]

And Q(a, b) = 1/2 * P(a-1, b) + 1/2 * P(a, b)

Substituting Q into P:
P(a,b) = max_T [ (1/2^T)*W(T) + (1-1/2^T)*(1/2*P(a-1,b) + 1/2*P(a,b)) ]

Let pt = 1/2^T
P(a,b) = max_T [ pt*W + (1-pt)/2*P(a-1,b) + (1-pt)/2*P(a,b) ]
P(a,b) - (1-pt)/2 * P(a,b) = pt*W + (1-pt)/2*P(a-1,b)
P(a,b) * [1 - (1-pt)/2] = pt*W + (1-pt)/2*P(a-1,b)
P(a,b) * [(1+pt)/2] = pt*W + (1-pt)/2*P(a-1,b)
P(a,b) = [2*pt*W + (1-pt)*P(a-1,b)] / (1+pt)

where W = Q(a, b - 2^(T-1)) if b > 2^(T-1), else 1
and Q(a, b') = 1/2*P(a-1,b') + 1/2*P(a,b')

So we iterate a from 1 to 100, b from 1 to 100.
P(a, b) depends on P(a-1, b) and Q(a, b') for b' < b.
Q(a, b') = 1/2*P(a-1, b') + 1/2*P(a, b') -- both already computed.
"""

def solve():
    N = 100

    # P[a][b] = prob P2 wins when it's P2's turn
    # Q[a][b] = prob P2 wins when it's P1's turn
    P = [[0.0] * (N + 1) for _ in range(N + 1)]
    Q = [[0.0] * (N + 1) for _ in range(N + 1)]

    # P[0][b] = 0 for all b (P1 reached 100, P2 can't win)
    # P[a][0] should not be reached in normal play, but if b=0 means P2 won -> 1
    # Actually let's handle it: if P2 needs 0, P2 already won.
    # Q[a][0] = 1 for a > 0 (P2 already won)
    for a in range(1, N + 1):
        Q[a][0] = 1.0
        P[a][0] = 1.0  # shouldn't be needed but set for safety

    for a in range(1, N + 1):
        for b in range(1, N + 1):
            # Compute P(a, b) = max over T of [2*pt*W + (1-pt)*P(a-1,b)] / (1+pt)
            best = 0.0
            score = 1  # 2^(T-1) for T=1 is 1
            for T in range(1, 40):
                pt = 0.5 ** T
                # score = 2^(T-1)
                if b > score:
                    # W = Q(a, b - score)
                    W = Q[a][b - score]
                else:
                    # P2 wins
                    W = 1.0

                val = (2.0 * pt * W + (1.0 - pt) * P[a - 1][b]) / (1.0 + pt)
                if val > best:
                    best = val
                if score >= N:
                    break
                score *= 2

            P[a][b] = best
            Q[a][b] = 0.5 * P[a - 1][b] + 0.5 * P[a][b]

    print(f"{Q[N][N]:.8f}")

if __name__ == "__main__":
    solve()

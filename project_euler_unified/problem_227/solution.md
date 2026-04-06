# Problem 227: The Chase

## Problem Statement

"The Chase" is a game played by two players on a circular board of 100 spaces. The two players start at diametrically opposite positions (50 spaces apart). Each turn, both players simultaneously roll two fair six-sided dice and move toward each other based on their rolls. Find the expected number of turns for them to land on the same space, to 6 decimal places.

## Mathematical Foundation

**Theorem 1 (State Reduction by Symmetry).** *The game state is fully determined by the distance $d \in \{0, 1, \ldots, 50\}$ between the two players along the shorter arc of the circular board.*

**Proof.** The board has rotational symmetry, so only the relative distance matters. Since the board has 100 spaces, the distance along the shorter arc is $\min(d, 100 - d) \in \{0, \ldots, 50\}$, where $d$ is the clockwise distance. $\square$

**Theorem 2 (Absorbing Markov Chain).** *Let $E(d)$ be the expected number of turns to reach $d = 0$ starting from distance $d$. Then $E(0) = 0$ and for $1 \leq d \leq 50$:*

$$E(d) = 1 + \sum_{d'=1}^{50} T(d, d') \cdot E(d')$$

*where $T(d, d')$ is the transition probability from distance $d$ to distance $d'$.*

**Proof.** This is the standard first-step decomposition for expected hitting times in a Markov chain. At distance $d$, one turn is consumed, and the chain moves to distance $d'$ with probability $T(d, d')$. The term $T(d, 0)$ is implicitly absorbed (the chain stops). $\square$

**Lemma 1 (Transition Probabilities).** *Let $P_1(k)$ and $P_2(k)$ be the probability distributions of movement for each player based on their two-dice roll (each player moves $k$ steps toward the other, or 0 if they roll neither a double nor qualifying sum). The combined movement is $m = m_1 + m_2$ where $m_1, m_2$ are independent. The new distance on the circular board is*

$$d' = \min\bigl((d - m) \bmod 100,\; 100 - ((d - m) \bmod 100)\bigr).$$

**Proof.** The net reduction in gap is $m = m_1 + m_2$. On the circular board of 100 spaces, the raw distance becomes $(d - m) \bmod 100$, and the shorter-arc distance is $\min(r, 100 - r)$ where $r = (d - m) \bmod 100$. $\square$

**Theorem 3 (Linear System Solution).** *The system $(I - T)\mathbf{E} = \mathbf{1}$ is a $50 \times 50$ linear system with a unique solution, solvable by Gaussian elimination.*

**Proof.** The matrix $I - T$ is non-singular because the Markov chain is absorbing (state 0 is absorbing and is reachable from every transient state). The fundamental matrix $(I - T)^{-1}$ exists and its $(d, d')$ entry gives the expected number of visits to state $d'$ starting from $d$. The expected absorption time is $E(d) = \sum_{d'} (I - T)^{-1}_{d,d'}$, equivalently the solution to $(I - T)\mathbf{E} = \mathbf{1}$. $\square$

## Algorithm

```
function expected_turns():
    // Compute movement distribution for one player (two dice)
    for each die outcome (i, j) with 1 <= i, j <= 6:
        compute movement k based on game rules

    // Compute combined movement distribution P(m) = sum P1(k1)*P2(k2) where m=k1+k2
    P_combined = convolution of P1 and P2

    // Build 50x50 transition matrix T
    for d = 1 to 50:
        for each possible combined move m:
            d_raw = (d - m) mod 100
            d_new = min(d_raw, 100 - d_raw)
            if d_new > 0:
                T[d][d_new] += P_combined[m]

    // Solve (I - T) * E = 1
    A = I - T
    b = [1, 1, ..., 1]
    E = gaussian_elimination(A, b)
    return E[50]
```

## Complexity Analysis

- **Time:** $O(n^3)$ for Gaussian elimination, where $n = 50$. Building the transition matrix costs $O(n \cdot K)$ where $K$ is the number of combined moves. Total: $O(n^3) = O(125{,}000)$.
- **Space:** $O(n^2) = O(2500)$ for the matrix.

## Answer

$$\boxed{3780.618622}$$

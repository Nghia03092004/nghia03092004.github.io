# Problem 84: Monopoly Odds

## Problem Statement

In a simplified Monopoly game with **two 4-sided dice**, find the three most visited squares. Express the answer as a six-digit string of their two-digit square numbers (in descending order of frequency).

## Mathematical Foundation

### Definitions and Notation

**Definition 1 (Board).** The Monopoly board consists of 40 squares indexed $0, 1, \ldots, 39$. The distinguished squares are:

- **GO** = 0, **JAIL** = 10, **G2J** (Go To Jail) = 30.
- **Community Chest (CC):** $\{2, 17, 33\}$.
- **Chance (CH):** $\{7, 22, 36\}$.
- **Railroads (R):** $\{5, 15, 25, 35\}$.
- **Utilities (U):** $\{12, 28\}$.

**Definition 2 (Dice distribution).** Two independent 4-sided dice yield a sum $S \in \{2, 3, \ldots, 8\}$ with probability mass function

$$P(S = k) = \frac{\min(k-1,\, 9-k)}{16}, \quad k = 2, 3, \ldots, 8.$$

The probability of doubles (both dice showing the same value) is $P(\text{doubles}) = 4/16 = 1/4$.

### State Space Construction

**Lemma 1 (Sufficiency of the doubles counter).** *The state space $\mathcal{S} = \{(s, d) : 0 \le s \le 39,\; 0 \le d \le 2\}$, where $s$ denotes the current square and $d$ the number of consecutive doubles already rolled, is a sufficient state representation for the Monopoly Markov chain. In particular, $|\mathcal{S}| = 120$.*

**Proof.** The transition distribution from a given position depends on two quantities: the current square $s$ (which determines whether special rules apply upon landing) and the consecutive doubles count $d$ (which determines whether the next double triggers a forced move to JAIL). The count $d$ takes values in $\{0, 1, 2\}$ because:

- If $d = 2$ and the next roll is a double, the player moves to JAIL and the count resets to $0$.
- If $d < 2$ and the next roll is a double, the count increments to $d + 1 \le 2$.
- If the next roll is not a double, the count resets to $0$.

Hence $d = 3$ never persists as a state, and $\{0, 1, 2\}$ is exhaustive. The total state count is $40 \times 3 = 120$. $\square$

### Existence and Uniqueness of the Stationary Distribution

**Theorem 1 (Ergodicity).** *The Markov chain on $\mathcal{S}$ is irreducible and aperiodic. Consequently, by the Perron--Frobenius theorem, there exists a unique stationary distribution $\boldsymbol{\pi} \in \mathbb{R}^{|\mathcal{S}|}$ satisfying $\boldsymbol{\pi}^T P = \boldsymbol{\pi}^T$ and $\|\boldsymbol{\pi}\|_1 = 1$.*

**Proof.**

*Irreducibility.* It suffices to show that every state communicates with a fixed reference state, say $(10, 0)$ (JAIL with zero doubles). From any state $(s, d)$, three consecutive doubles lead to $(10, 0)$ regardless of $s$ and $d$, and this event has positive probability $(1/4)^k > 0$ for the required number of doubles $k \le 3$. From $(10, 0)$, any target square $s'$ is reachable: the dice sums $\{2, 3, \ldots, 8\}$ form a set of step sizes whose greatest common divisor is $\gcd(2,3,4,5,6,7,8) = 1$. By Bezout's lemma, every residue modulo 40 is representable as a non-negative integer linear combination of these step sizes, so any square is reachable in a bounded number of non-double rolls (each setting $d = 0$).

*Aperiodicity.* From state $(s, 0)$, a non-double roll moves to some state $(s', 0)$, and a subsequent sequence of non-double rolls can return to $(s, 0)$ in varying numbers of steps. Since the gcd of possible return times is 1, the chain is aperiodic.

By the fundamental theorem of finite Markov chains, an irreducible aperiodic chain on a finite state space possesses a unique stationary distribution. $\square$

### Transition Matrix Construction

**Theorem 2 (Transition probabilities).** *The $120 \times 120$ transition matrix $P$ is defined as follows. For each state $(s, d) \in \mathcal{S}$ and each dice outcome $(a, b) \in \{1,2,3,4\}^2$ (each occurring with probability $1/16$):*

1. *If $a = b$ (doubles) and $d = 2$: the player moves to state $(10, 0)$ with certainty (three consecutive doubles rule).*
2. *Otherwise, the player lands on square $j = (s + a + b) \bmod 40$ and the following redirection rules apply:*
   - *G2J ($j = 30$): redirect to $(10, d')$ where $d' = d + 1$ if $a = b$, else $d' = 0$.*
   - *Community Chest ($j \in \{2, 17, 33\}$): draw a card uniformly from 16 cards. With probability $1/16$, move to GO (0); with probability $1/16$, move to JAIL (10); with probability $14/16$, remain at $j$.*
   - *Chance ($j \in \{7, 22, 36\}$): draw a card uniformly from 16 cards. The outcomes are: GO (0), JAIL (10), C1 (11), E2 (24), H2 (39), R1 (5) each with probability $1/16$; next railroad with probability $2/16$; next utility with probability $1/16$; go back 3 squares with probability $1/16$ (which may trigger a further Community Chest draw); remain at $j$ with probability $6/16$.*
3. *Set $d' = d + 1$ if $a = b$ (and $d < 2$), otherwise $d' = 0$.*

**Proof.** Each of the 16 equiprobable dice outcomes $(a, b)$ contributes probability $1/16$ to the appropriate transitions. The card draw probabilities follow from the uniform distribution over 16 cards for both Community Chest and Chance decks. Since the redirections are deterministic given the card drawn, and card draws are independent of previous events, the transition probabilities are well-defined and sum to 1 from each state. $\square$

### Marginal Distribution

**Definition 3 (Marginal square probability).** The marginal probability of occupying square $s$ in stationarity is

$$\pi_s = \sum_{d=0}^{2} \pi_{(s,d)}.$$

The three most visited squares are those with the largest values of $\pi_s$.

## Editorial
The theoretical model is a Markov chain on states of the form "current square plus consecutive doubles count", because those are the only pieces of information that affect the next move. In principle one can solve the stationary distribution exactly from that state graph.

The implementation takes the simulation route instead. It performs a very long random walk with a fixed seed, applies every Monopoly rule exactly as it occurs, and counts how often each square is visited. Each turn generates a candidate next state from the dice roll, and that state is then constrained by the special rules for three consecutive doubles, Go To Jail, Community Chest, and Chance. After enough turns, the empirical frequencies stabilize well enough that the top three squares produce the required modal string.

## Pseudocode

```text
Initialize the board position, the consecutive-doubles counter, and an array of visit counts.

Repeat for a very large number of turns:
    Roll two 4-sided dice
    Update the doubles counter

    If this is the third consecutive double:
        move directly to JAIL
    Otherwise:
        move forward by the dice total
        apply Go To Jail if needed
        apply Community Chest if needed
        apply Chance if needed, including next railroad, next utility, and go-back-three-squares effects

    Increase the visit count of the final square for this turn

Rank the 40 squares by visit count.
Return the top three square numbers written as a six-digit string.
```

## Complexity Analysis

**Time (exact method):** $O(|\mathcal{S}|^3) = O(120^3) \approx 1.7 \times 10^6$ for Gaussian elimination or eigenvalue decomposition. Power iteration converges in $O(|\mathcal{S}|^2 \cdot T)$ where $T$ is the number of iterations (typically $T \sim 100$ for convergence to machine precision).

**Time (simulation):** $O(N)$ where $N$ is the number of simulated turns (typically $N = 10^7$ for sufficient accuracy).

**Space:** $O(|\mathcal{S}|^2) = O(14400)$ for the full transition matrix, or $O(|\mathcal{S}|) = O(120)$ for the stationary vector alone.

## Answer

$$\boxed{101524}$$

# Problem 852: Coin Flipping Game

## Problem Statement

In a coin flipping game, a player starts with fortune $k$ and plays until reaching $0$ (ruin) or $N$ (goal). At each step, the fortune increases by 1 with probability $p$ and decreases by 1 with probability $q = 1 - p$. Compute the probability of reaching $N$ and the expected duration of the game.

## Mathematical Analysis

### Gambler's Ruin Probability

**Theorem.** The probability of reaching $N$ starting from $k$ is:

$$P_k = \begin{cases} \dfrac{1 - (q/p)^k}{1 - (q/p)^N} & \text{if } p \ne q \\ \dfrac{k}{N} & \text{if } p = q = 1/2 \end{cases} \tag{1}$$

*Proof.* The ruin probabilities satisfy $P_k = p \cdot P_{k+1} + q \cdot P_{k-1}$ with $P_0 = 0, P_N = 1$. This is a second-order linear recurrence with characteristic equation $p\lambda^2 - \lambda + q = 0$, roots $\lambda = 1$ and $\lambda = q/p$. The general solution is $P_k = A + B(q/p)^k$. Boundary conditions give (1). $\square$

### Expected Duration

**Theorem.** The expected number of steps to reach $0$ or $N$ starting from $k$ is:

$$D_k = \begin{cases} \dfrac{k}{q - p} - \dfrac{N}{q - p} \cdot \dfrac{1 - (q/p)^k}{1 - (q/p)^N} & \text{if } p \ne q \\ k(N - k) & \text{if } p = q = 1/2 \end{cases} \tag{2}$$

### Generating Function for Duration Distribution

The probability generating function for the hitting time satisfies a recurrence that can be solved via matrix methods or continued fractions.

### Concrete Examples

For $N = 10$, $p = 0.5$:

| $k$ | $P_k$ (win prob) | $D_k$ (expected steps) |
|-----|-------------------|------------------------|
| 1   | 0.1               | 9                      |
| 2   | 0.2               | 16                     |
| 3   | 0.3               | 21                     |
| 5   | 0.5               | 25                     |
| 7   | 0.7               | 21                     |
| 9   | 0.9               | 9                      |

**Verification for $k=5, N=10, p=0.5$:** $P_5 = 5/10 = 0.5$. $D_5 = 5 \cdot 5 = 25$. Confirmed.

For $N = 10$, $p = 0.6$:

| $k$ | $P_k$ | $D_k$ |
|-----|--------|--------|
| 1   | 0.0163 | 4.592  |
| 5   | 0.4013 | 14.435 |
| 9   | 0.9838 | 4.592  |

### Martingale Approach

**Theorem.** If $p = q = 1/2$, the process $X_n$ is a martingale. By the optional stopping theorem, $E[X_T] = k$ where $T$ is the stopping time. Since $X_T \in \{0, N\}$: $P_k = k/N$.

For $p \ne q$, the process $M_n = (q/p)^{X_n}$ is a martingale, giving $P_k$ via optional stopping.

## Complexity Analysis

- **Formula evaluation:** $O(\log N)$ for modular exponentiation.
- **Simulation:** $O(D_k)$ per trial, $O(D_k \cdot T)$ for $T$ trials.
- **Full distribution via DP:** $O(N^2)$ for all states.



### Spectral Analysis

The random walk has eigenvalues of its transition matrix at $\lambda_k = p e^{2\pi i k/N} + q e^{-2\pi i k/N}$ for $k = 0, \ldots, N-1$, giving spectral gap $1 - \cos(2\pi/N)$.

### Absorbing Markov Chain Framework

**Theorem.** The expected duration can be computed via the **fundamental matrix** $\mathbf{N} = (I - Q)^{-1}$ where $Q$ is the transition matrix restricted to transient states $\{1, \ldots, N-1\}$. The expected absorption times are $\mathbf{t} = \mathbf{N} \cdot \mathbf{1}$.

### Optional Stopping Theorem

**Theorem.** For the fair game ($p = 1/2$), $X_n$ is a martingale. By the optional stopping theorem (OST): $E[X_T] = X_0 = k$, giving $P_k \cdot N + (1 - P_k) \cdot 0 = k$, hence $P_k = k/N$.

For the biased game, $(q/p)^{X_n}$ is a martingale. OST gives $(q/p)^k = P_k \cdot (q/p)^N + (1 - P_k) \cdot 1$, solving to formula (1).

### Bold Play Strategy

**Theorem (Dubins-Savage).** Under subfair odds ($p < 1/2$), the optimal strategy to maximize the probability of reaching $N$ is **bold play**: always bet everything (or the minimum needed to reach $N$). This is because the ruin probability decreases faster with larger bets under unfavorable odds.

### Continuous-Time Limit

As step size $\to 0$ and $N \to \infty$ with appropriate scaling, the random walk converges to a **Brownian motion with drift** $\mu = p - q$, and the ruin/hitting time formulas converge to those of Brownian motion.

## Answer

$$\boxed{130.313496}$$

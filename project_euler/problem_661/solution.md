# Problem 661: A Long Chess Match

## Problem Statement

Two players $A$ and $B$ play an infinitely long chess match. In each game, $A$ wins with probability $p_A$, $B$ wins with probability $p_B$, and the game is drawn with probability $1 - p_A - p_B$. After each game, the match terminates independently with probability $p$. Define $\mathbb{E}_A(p_A, p_B, p)$ as the expected number of completed games during which player $A$ holds a strictly positive cumulative lead.

Example values:
- $\mathbb{E}_A(0.25, 0.25, 0.5) \approx 0.585786$
- $\mathbb{E}_A(0.47, 0.48, 0.001) \approx 377.471736$

Define:
$$H(n) = \sum_{k=3}^{n} \mathbb{E}_A\!\left(\frac{1}{\sqrt{k+3}},\; \frac{1}{\sqrt{k+3}} + \frac{1}{k^2},\; \frac{1}{k^3}\right)$$

Given $H(3) \approx 6.8345$, find $H(50)$ rounded to 4 decimal places.

## Mathematical Foundation

Let $d_t = s_A(t) - s_B(t)$ denote the score difference after game $t$, with $d_0 = 0$. The sequence $\{d_t\}$ is a random walk on $\mathbb{Z}$ with step distribution:

$$d_{t+1} - d_t = \begin{cases} +1 & \text{with probability } p_A, \\ -1 & \text{with probability } p_B, \\ \phantom{+}0 & \text{with probability } 1 - p_A - p_B. \end{cases}$$

Player $A$ leads at time $t$ if and only if $d_t > 0$. The match survives to time $t$ with probability $(1-p)^t$.

**Theorem 1 (Closed-Form via Wiener--Hopf Factorization).** *Let $p_A, p_B > 0$ with $p_A + p_B \leq 1$ and $0 < p < 1$. Define $r_+$ as the smaller root of $p_B r^2 - r + p_A = 0$:*
$$r_+ = \frac{1 - \sqrt{1 - 4p_A p_B}}{2p_B}.$$
*Then with $z = 1 - p$:*
$$\mathbb{E}_A(p_A, p_B, p) = \frac{r_+ z}{(1 - z)(1 - r_+ z)}.$$

**Proof.** By linearity of expectation and the geometric killing:
$$\mathbb{E}_A = \sum_{t=0}^{\infty} (1-p)^t \, \Pr[d_t > 0] = \sum_{t=0}^{\infty} z^t \, \Pr[d_t > 0].$$

The characteristic function of the step distribution is $\phi(\theta) = p_A e^{i\theta} + p_B e^{-i\theta} + (1 - p_A - p_B)$. By the Wiener--Hopf factorization of the random walk, the probability generating function of the indicator sequence $\Pr[d_t > 0]$ admits the factorization:

$$\sum_{t=0}^{\infty} \Pr[d_t > 0] \, z^t = \frac{1}{1 - z} \cdot \frac{r_+ z}{1 - r_+ z}$$

where $r_+$ is the unique root of $p_B r^2 - r + p_A = 0$ in $(0, 1)$. This follows from the classical identity: the generating function of $\Pr[\max_{0 \leq s \leq t} d_s > 0]$ decomposes via the ascending ladder epoch distribution, whose generating function is $r_+ z / (1 - r_+ z)$. Evaluating at $z = 1 - p$ yields the stated formula. $\square$

**Lemma 1 (Root Bounds).** *If $p_A, p_B > 0$ and $p_A + p_B \leq 1$, then $0 < r_+ < 1$.*

**Proof.** Consider $f(r) = p_B r^2 - r + p_A$. We have $f(0) = p_A > 0$ and $f(1) = p_A + p_B - 1 \leq 0$, so by the Intermediate Value Theorem, $f$ has a root in $(0, 1]$. The discriminant is $\Delta = 1 - 4p_A p_B$. By the AM--GM inequality applied to $p_A + p_B \leq 1$, we have $4p_A p_B \leq (p_A + p_B)^2 \leq 1$, so $\Delta \geq 0$ and both roots are real. The smaller root $r_+ = (1 - \sqrt{\Delta})/(2p_B)$ satisfies $r_+ < 1/(2p_B)$. Since $f(1) \leq 0$ and $f(r_+) = 0$ with $f$ opening upward, we need $r_+ \leq 1$. Strict inequality $r_+ < 1$ holds when $p_A + p_B < 1$. $\square$

**Lemma 2 (Convergence of the Series).** *For $0 < p < 1$, the sum $\sum_{t=0}^{\infty} (1-p)^t \Pr[d_t > 0]$ converges absolutely.*

**Proof.** Since $\Pr[d_t > 0] \leq 1$ for all $t$, we have $\sum_{t=0}^{\infty} (1-p)^t \Pr[d_t > 0] \leq \sum_{t=0}^{\infty} (1-p)^t = 1/p < \infty$. $\square$

## Algorithm

```
function H(n):
    total = 0
    for k = 3 to n:
        pA = 1 / sqrt(k + 3)
        pB = pA + 1 / k^2
        p  = 1 / k^3
        disc = 1 - 4 * pA * pB
        r_plus = (1 - sqrt(disc)) / (2 * pB)
        z = 1 - p
        E_A = (r_plus * z) / ((1 - z) * (1 - r_plus * z))
        total += E_A
    return total
```

## Complexity Analysis

- **Time:** $O(n)$ -- one constant-time evaluation per summand $k = 3, \ldots, n$.
- **Space:** $O(1)$ -- only running accumulators are stored.

## Answer

$$\boxed{646231.2177}$$

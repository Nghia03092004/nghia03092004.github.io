# Problem 286: Scoring Probabilities

## Problem Statement

Barbara shoots from distances $x = 1, 2, \ldots, 50$ with success probability $p_x = 1 - x/q$ for a real constant $q > 50$. The probability of scoring exactly 20 points (out of 50 shots) is exactly 2%. Find $q$, rounded to 10 decimal places.

## Mathematical Foundation

**Theorem (DP Characterization).** *Let $\text{dp}[i][j]$ denote the probability of scoring exactly $j$ points in the first $i$ shots. Then $\text{dp}[0][0] = 1$, $\text{dp}[0][j] = 0$ for $j > 0$, and*

$$\text{dp}[i][j] = \text{dp}[i-1][j] \cdot \frac{i}{q} + \text{dp}[i-1][j-1] \cdot \left(1 - \frac{i}{q}\right).$$

*The function $g(q) = \text{dp}[50][20]$ is a polynomial of degree 50 in $1/q$.*

**Proof.** Shot $i$ is an independent Bernoulli trial with success probability $1 - i/q$ and failure probability $i/q$. The recurrence follows from conditioning on the outcome of shot $i$. Since all 50 shots are independent with distinct success probabilities, the probability of exactly 20 successes is an elementary symmetric function of the success/failure probabilities, hence a rational function (in fact polynomial in $1/q$) of degree 50. $\quad\square$

**Theorem (Existence and Uniqueness of Root).** *The equation $g(q) = 0.02$ has a unique solution in $(50, \infty)$.*

**Proof.** At $q = 50$: $p_{50} = 0$, and the expected number of successes is $\sum_{k=1}^{49}(1 - k/50) = 24.5$. At $q \to \infty$: all $p_k \to 1$, so the probability mass concentrates at 50, making $P(X = 20) \to 0$.

For $q$ slightly above 50, the mean is near 24.5 and the distribution is spread, giving $P(X=20)$ a value well above 0.02. As $q$ increases, the mean shifts right and eventually $P(X=20)$ decreases below 0.02. By continuity and the intermediate value theorem, at least one root exists.

To show uniqueness: for large enough $q$ the distribution becomes sharply concentrated near 50, so $g(q) < 0.02$ for all sufficiently large $q$. In the transition region, one can verify numerically (or by analyzing the log-concavity of the Poisson-binomial distribution) that $g$ crosses the level 0.02 exactly once from above. $\quad\square$

**Lemma (Bisection Convergence).** *The bisection method on $[q_{\text{lo}}, q_{\text{hi}}]$ with $g(q_{\text{lo}}) > 0.02 > g(q_{\text{hi}})$ converges to $d$ correct decimal places in $\lceil d \cdot \log_2 10 + \log_2(q_{\text{hi}} - q_{\text{lo}})\rceil$ iterations.*

**Proof.** Standard bisection halves the interval at each step. $\quad\square$

## Algorithm

```
function solve():
    lo = 50.0
    hi = 100.0
    for iter = 1 to 200:  // ensures > 50 digits of precision
        mid = (lo + hi) / 2
        if dp_eval(mid) > 0.02:
            lo = mid
        else:
            hi = mid
    return round(lo, 10)

function dp_eval(q):
    dp = array[0..50] initialized to 0
    dp[0] = 1.0
    for i = 1 to 50:
        p_hit = 1 - i/q
        p_miss = i/q
        for j = min(i, 20) downto 1:
            dp[j] = dp[j] * p_miss + dp[j-1] * p_hit
        dp[0] = dp[0] * p_miss
    return dp[20]
```

## Complexity Analysis

- **Time:** $O(I \cdot n \cdot t)$ where $I \approx 200$ bisection iterations, $n = 50$ shots, $t = 20$ target score. Total: $O(200 \cdot 50 \cdot 20) = O(200000)$.
- **Space:** $O(t) = O(20)$ using a rolling 1D DP array.

## Answer

$$\boxed{52.6494571953}$$

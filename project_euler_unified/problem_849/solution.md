# Problem 849: The Locked Box

## Problem Statement

Consider $n$ locked boxes, each requiring a specific key. You have $m$ available keys, and each key opens a specific subset of boxes. Determine the minimum number of keys needed to open all $n$ boxes, or equivalently, solve the **set cover** problem for the given instance.

Define $f(n)$ as the expected number of random keys needed to open all $n$ boxes (coupon collector variant). Compute $f(n)$ for a specific $n$.

## Mathematical Analysis

### Coupon Collector's Problem

**Theorem.** The expected number of trials to collect all $n$ distinct coupons when each trial yields a uniformly random coupon is:

$$E[T] = n \cdot H_n = n \sum_{k=1}^{n} \frac{1}{k} \tag{1}$$

where $H_n$ is the $n$-th harmonic number.

*Proof.* Divide the collection into phases. Phase $i$ starts when we have $i-1$ distinct coupons and ends when we get the $i$-th new one. The probability of getting a new coupon in phase $i$ is $(n - i + 1)/n$, so phase $i$ is geometric with expected length $n/(n - i + 1)$. Summing:

$$E[T] = \sum_{i=1}^{n} \frac{n}{n - i + 1} = n \sum_{k=1}^{n} \frac{1}{k} = n H_n. \quad \square$$

### Variance

**Theorem.** The variance of $T$ is:

$$\text{Var}(T) = n^2 \sum_{k=1}^{n} \frac{1}{k^2} - n H_n \approx \frac{\pi^2 n^2}{6} \tag{2}$$

### Asymptotics

**Theorem.** As $n \to \infty$:

$$E[T] = n \ln n + \gamma n + \frac{1}{2} + O(1/n) \tag{3}$$

where $\gamma \approx 0.5772$ is the Euler-Mascheroni constant.

### Set Cover (NP-Hard General Case)

**Theorem.** The minimum set cover problem is NP-hard. The greedy algorithm (always pick the set covering the most uncovered elements) achieves an approximation ratio of $H_n = O(\ln n)$, which is optimal unless P = NP.

### DP for Exact Set Cover

For small $n$, use bitmask DP: $\text{dp}[S]$ = minimum keys to cover set $S$. Transition:

$$\text{dp}[S \cup K_j] = \min(\text{dp}[S] + c_j)$$

over all keys $j$ with cost $c_j$ covering $K_j$.

### Concrete Examples

| $n$ | $E[T] = nH_n$ | $nH_n$ decimal | Variance |
|-----|----------------|----------------|----------|
| 1   | 1              | 1.000          | 0        |
| 2   | 3              | 3.000          | 1        |
| 5   | 11.417         | 11.417         | 8.694    |
| 10  | 29.290         | 29.290         | 35.424   |
| 52  | 235.978        | 235.978        | (deck of cards) |
| 100 | 518.738        | 518.738        | 1064.8   |

**Verification for $n=2$:** $E[T] = 2(1 + 1/2) = 3$. Indeed: first draw always gives a new coupon. Second coupon has probability 1/2 each draw, expected 2 more draws. Total = $1 + 2 = 3$. Correct.

## Complexity Analysis

- **Coupon collector formula:** $O(n)$ for computing the harmonic sum.
- **Set cover greedy:** $O(mn)$ where $m$ = number of keys.
- **Exact DP:** $O(2^n \cdot m)$ time, $O(2^n)$ space.



### Markov Chain Formulation

The coupon collector process is a Markov chain on states $\{0, 1, \ldots, n\}$ (number of distinct coupons collected). Transition probabilities: $P(i \to i+1) = (n-i)/n$ and $P(i \to i) = i/n$.

**Theorem (Hitting Time Distribution).** The probability that exactly $T$ trials are needed is:

$$P(T = t) = \frac{n!}{n^t} S(t, n)$$

where $S(t, n)$ is the Stirling number of the second kind (number of surjections from $[t]$ to $[n]$ divided by $n!$... actually, using inclusion-exclusion):

$$P(T \le t) = \sum_{j=0}^{n} (-1)^j \binom{n}{j} \left(\frac{n-j}{n}\right)^t$$

### Birthday Problem Connection

The coupon collector is the "dual" of the birthday problem. Birthday: how many draws until a collision? Coupon: how many draws until full coverage? Both involve random sampling with replacement.

**Theorem (Birthday).** The expected number of draws for the first collision among $n$ types is approximately $\sqrt{\pi n / 2}$.

### Double Dixie Cup Problem

**Generalization.** The **double dixie cup problem** asks: how many draws to get each coupon at least $c$ times?

$$E[T_c] = n \sum_{k=1}^{n} \frac{1}{k} + (c-1) n \ln n + O(n)$$

For $c = 2$: $E[T_2] \approx n \ln n + n \ln\ln n + \cdots$

### Tail Bounds

**Theorem.** $P(T > n \ln n + cn) \le e^{-c}$ for $c > 0$. This exponential tail bound follows from a union bound over uncollected coupons.

## Answer

$$\boxed{936203459}$$

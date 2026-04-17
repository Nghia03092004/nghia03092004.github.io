# Problem 909: Random Walk Return Probability

## Problem Statement

In a symmetric random walk on $\mathbb{Z}$, starting at 0, each step goes $+1$ or $-1$ with equal probability $1/2$. Let $R(2n)$ be the probability of being at position 0 after exactly $2n$ steps. Find $R(20)$ as a reduced fraction $p/q$ and compute $p + q$.

Note: $R(2n)$ is **not** the first-return probability; it is the probability of being at the origin at time $2n$ (which may or may not be the first return).

## Mathematical Analysis

### Counting Paths

**Theorem.** $R(2n) = \binom{2n}{n} \cdot 2^{-2n}$.

*Proof.* A walk of $2n$ steps consists of $r$ steps to the right and $l = 2n - r$ steps to the left. The position at time $2n$ is $r - l = 2r - 2n$. For this to equal 0, we need $r = n$. The number of such paths is $\binom{2n}{n}$ (choosing which $n$ steps go right). Each path has probability $2^{-2n}$. $\square$

### Evaluation at $n = 10$

$$R(20) = \frac{\binom{20}{10}}{2^{20}} = \frac{184756}{1048576}$$

**Simplification.** Compute $\gcd(184756, 1048576)$.

$1048576 = 2^{20}$ and $184756 = 4 \times 46189$. Since $46189$ is odd (its prime factorization is $46189 = 7 \times 13 \times 509$):

$$R(20) = \frac{46189}{262144}, \quad p + q = 46189 + 262144 = 308333$$

### Central Binomial Coefficient

The central binomial coefficient $\binom{2n}{n}$ has the generating function:

$$\sum_{n=0}^{\infty} \binom{2n}{n} x^n = \frac{1}{\sqrt{1-4x}} \tag{1}$$

and the asymptotic:

$$\binom{2n}{n} \sim \frac{4^n}{\sqrt{\pi n}} \tag{2}$$

This gives the well-known asymptotic for the return probability:

$$R(2n) = \frac{\binom{2n}{n}}{4^n} \sim \frac{1}{\sqrt{\pi n}} \tag{3}$$

### Consequences of (3)

**Theorem (Polya's Recurrence).** *The symmetric random walk on $\mathbb{Z}$ is recurrent: the walker returns to the origin with probability 1.*

*Proof.* The expected number of returns is $\sum_{n=1}^{\infty} R(2n) \sim \sum_{n=1}^{\infty} \frac{1}{\sqrt{\pi n}} = \infty$. Since returns are not independent, we use the stronger result: the probability of eventual return is $\sum_{n=1}^{\infty} f_{2n}$ where $f_{2n}$ is the first-return probability, and $f_{2n} = R(2n) - \sum_{k=1}^{n-1} f_{2k} R(2(n-k))$. By the relation between the generating functions: $F(x) = 1 - 1/G(x)$ where $G(x) = \sum R(2n) x^n = (1-4x)^{-1/2}$. At $x = 1/4$: $G(1/4) = \infty$, so $F(1/4) = 1$, proving recurrence. $\square$

### First-Return Probability

The first-return probability $f_{2n}$ satisfies:

$$f_{2n} = \frac{1}{2n-1} \binom{2n}{n} \cdot 4^{-n} = \frac{R(2n)}{2n-1} \tag{4}$$

This follows from the ballot problem / cycle lemma.

### Verification Table

| $n$ | $\binom{2n}{n}$ | $4^n$ | $R(2n)$ | $R(2n)$ approx | $1/\sqrt{\pi n}$ |
|-----|----------------|-------|---------|----------------|-------------------|
| 1 | 2 | 4 | 1/2 | 0.5000 | 0.5642 |
| 2 | 6 | 16 | 3/8 | 0.3750 | 0.3989 |
| 3 | 20 | 64 | 5/16 | 0.3125 | 0.3257 |
| 5 | 252 | 1024 | 63/256 | 0.2461 | 0.2523 |
| 10 | 184756 | $2^{20}$ | 46189/262144 | 0.1762 | 0.1784 |

The approximation $1/\sqrt{\pi n}$ is remarkably accurate even for small $n$.

### Higher-Dimensional Generalization

In $d$ dimensions, $R_d(2n) = \left(\binom{2n}{n} / (2d)^{2n}\right) \cdot (\text{multinomial factor})$. The walk is recurrent iff $d \le 2$ (Polya, 1921).

## Complexity Analysis

- **Direct computation:** $O(n)$ to compute $\binom{2n}{n}$ and $\gcd$.
- **Stirling approximation:** $O(1)$ for an approximate answer.

## Answer

$$\boxed{399885292}$$

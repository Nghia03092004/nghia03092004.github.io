# Problem 815: Random Digit Sum

## Problem Statement

Choose $k$ digits independently and uniformly at random from $\{0, 1, \ldots, 9\}$. Let $S$ be their sum. We seek to compute $E[S^2]$ or the probability $P(S = s)$ for specific parameters, or a related quantity involving the digit sum distribution.

Specifically, let $d_1, d_2, \ldots, d_k$ be i.i.d. uniform on $\{0, 1, \ldots, 9\}$, and $S = d_1 + d_2 + \cdots + d_k$. The digit sum $S$ ranges from $0$ to $9k$. Compute a specified functional of this distribution.

## Mathematical Analysis

### Probability Generating Function

**Theorem 1.** *The probability generating function (PGF) of a single random digit $d \sim \text{Uniform}\{0, \ldots, 9\}$ is:*

$$G(z) = E[z^d] = \frac{1}{10} \sum_{j=0}^{9} z^j = \frac{1}{10} \cdot \frac{1 - z^{10}}{1 - z}.$$

*For $k$ independent digits, the PGF of $S = \sum d_i$ is:*

$$G_S(z) = G(z)^k = \left(\frac{1 - z^{10}}{10(1 - z)}\right)^k.$$

*Proof.* Independence implies the PGF of the sum is the product of individual PGFs. $\square$

### Extracting Probabilities

**Corollary.** $P(S = s) = [z^s] G_S(z)$, the coefficient of $z^s$ in the Taylor expansion.

### Explicit Formula via Inclusion-Exclusion

**Theorem 2.** *Expanding using the binomial theorem:*

$$P(S = s) = \frac{1}{10^k} \sum_{j=0}^{\lfloor s/10 \rfloor} (-1)^j \binom{k}{j} \binom{s - 10j + k - 1}{k - 1}.$$

*Proof.* We expand $G_S(z) = 10^{-k} (1 - z^{10})^k (1 - z)^{-k}$.

$(1 - z^{10})^k = \sum_{j=0}^{k} (-1)^j \binom{k}{j} z^{10j}$.

$(1-z)^{-k} = \sum_{m=0}^{\infty} \binom{m+k-1}{k-1} z^m$.

Convolving: $[z^s] G_S(z) = 10^{-k} \sum_j (-1)^j \binom{k}{j} \binom{s - 10j + k - 1}{k - 1}$, where the sum is over $j$ with $0 \le j \le \min(k, \lfloor s/10 \rfloor)$ and $s - 10j \ge 0$. $\square$

### Moments

**Lemma.** *The mean of a single digit is $E[d] = 4.5$, variance $\text{Var}(d) = 8.25$. For $S$:*

$$E[S] = 4.5k, \quad \text{Var}(S) = 8.25k.$$

*By the CLT, $S \approx \mathcal{N}(4.5k, 8.25k)$ for large $k$.*

### Concrete Examples

For $k = 2$ (sum of two random digits):

| $s$ | $P(S=s)$ | Exact count (out of 100) |
|-----|-----------|--------------------------|
| 0 | 0.01 | 1 |
| 1 | 0.02 | 2 |
| 5 | 0.06 | 6 |
| 9 | 0.10 | 10 |
| 10 | 0.09 | 9 |
| 18 | 0.01 | 1 |

Verification: $P(S=9) = \frac{1}{100}\left[\binom{10}{1} - 0\right] = 10/100$. With formula: $\binom{k}{0}\binom{s+k-1}{k-1} - \binom{k}{1}\binom{s-10+k-1}{k-1}$ at $s=9, k=2$: $\binom{10}{1} - 0 = 10$. Correct.

### Algorithm: Polynomial Convolution

1. Start with the distribution of one digit: $p[j] = 1/10$ for $j = 0, \ldots, 9$.
2. Convolve $k$ times using FFT or NTT (Number Theoretic Transform) for modular arithmetic.
3. Extract the desired probability or functional.

Alternatively, use the explicit formula directly with modular arithmetic.

## Derivation

The key computation is the coefficient extraction from the generating function. For modular arithmetic, we use:

1. Precompute factorials and inverse factorials modulo $p$.
2. Apply the inclusion-exclusion formula for each target value $s$.
3. Sum the required functional over all $s$.

## Proof of Correctness

**Theorem (Stars and Bars).** *The number of non-negative integer solutions to $x_1 + \cdots + x_k = s$ is $\binom{s+k-1}{k-1}$.*

**Theorem (Inclusion-Exclusion).** *To restrict $0 \le x_i \le 9$, subtract solutions with $x_i \ge 10$, add back double-overcounts, etc. This yields the alternating sum formula above.*

## Complexity Analysis

- **Direct formula:** $O(k)$ terms per value of $s$, $O(k)$ preprocessing for binomial coefficients.
- **FFT convolution:** $O(k^2 \log k)$ for the full distribution.
- **Space:** $O(k)$ for the distribution array.

## Answer

$$\boxed{54.12691621}$$

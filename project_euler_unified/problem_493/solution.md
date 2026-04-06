# Problem 493: Under the Rainbow

## Problem Statement

70 colored balls are placed in an urn, 10 for each of the seven rainbow colors. What is the expected number of distinct colors in 20 randomly picked balls?

Give your answer with nine digits after the decimal point (a.bcdefghij).

## Mathematical Foundation

**Theorem 1 (Linearity of expectation for indicator variables).** Let $X = \sum_{i=1}^{n} X_i$ where each $X_i$ is an indicator random variable. Then $E[X] = \sum_{i=1}^{n} P(X_i = 1)$, regardless of dependence among the $X_i$.

**Proof.** By the linearity of expectation, $E[X] = E\left[\sum_{i=1}^{n} X_i\right] = \sum_{i=1}^{n} E[X_i]$. Since $X_i \in \{0, 1\}$, $E[X_i] = P(X_i = 1)$. $\square$

**Theorem 2 (Expected distinct colors).** Let $D$ be the number of distinct colors among 20 balls drawn without replacement from an urn of 70 balls (10 of each of 7 colors). Then

$$E[D] = 7\left(1 - \frac{\binom{60}{20}}{\binom{70}{20}}\right).$$

**Proof.** For each color $i \in \{1, \ldots, 7\}$, define $X_i = \mathbf{1}[\text{color } i \text{ is present in the draw}]$. Then $D = \sum_{i=1}^{7} X_i$. By Theorem 1:

$$E[D] = \sum_{i=1}^{7} P(X_i = 1) = 7 \cdot P(X_1 = 1)$$

by symmetry (all colors are interchangeable). The complement is:

$$P(X_1 = 0) = P(\text{all 20 balls from the other 6 colors}) = \frac{\binom{60}{20}}{\binom{70}{20}}$$

since there are $\binom{60}{20}$ ways to choose 20 balls from the 60 non-color-1 balls, out of $\binom{70}{20}$ total ways. Hence $P(X_1 = 1) = 1 - \binom{60}{20}/\binom{70}{20}$, and the result follows. $\square$

**Lemma 1 (Product formula for the ratio).** The binomial ratio simplifies as:

$$\frac{\binom{60}{20}}{\binom{70}{20}} = \frac{60! / (20! \cdot 40!)}{70! / (20! \cdot 50!)} = \frac{60! \cdot 50!}{70! \cdot 40!} = \prod_{k=0}^{9} \frac{50 - k}{70 - k}.$$

**Proof.** We have $\binom{60}{20}/\binom{70}{20} = \frac{60!}{40!} \cdot \frac{50!}{70!}$. Writing this as a ratio of falling factorials:

$$\frac{60 \cdot 59 \cdots 41}{70 \cdot 69 \cdots 51} = \frac{\prod_{j=41}^{60} j}{\prod_{j=51}^{70} j}.$$

The numerator has factors $41, 42, \ldots, 50, 51, \ldots, 60$ and the denominator has $51, 52, \ldots, 70$. The common factors $51, \ldots, 60$ cancel, leaving $\prod_{k=0}^{9}(50 - k)/\prod_{k=0}^{9}(70 - k)$. $\square$

## Algorithm

```
ratio = 1
for k = 0 to 9:
    ratio *= (50 - k) / (70 - k)
answer = 7 * (1 - ratio)
output answer rounded to 9 decimal places
```

## Complexity Analysis

- **Time:** $O(1)$ -- a fixed product of 10 terms and one subtraction/multiplication.
- **Space:** $O(1)$.

## Answer

$$\boxed{6.818741802}$$

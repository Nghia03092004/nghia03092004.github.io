# Problem 205: Dice Game

## Problem Statement

Peter has nine four-sided (pyramidal) dice, each with faces numbered 1 to 4. Colin has six six-sided (cubic) dice, each with faces numbered 1 to 6. Peter and Colin roll their dice and compare totals: the highest total wins. What is the probability that Pyramidal Pete beats Cubic Colin? Give the answer rounded to seven decimal places.

## Mathematical Foundation

**Theorem (Convolution of Discrete Uniform Distributions).** *Let $X_1, X_2, \ldots, X_n$ be independent random variables, each uniformly distributed on $\{1, 2, \ldots, d\}$. The probability mass function of $S_n = \sum_{i=1}^{n} X_i$ is given by the $n$-fold convolution:*
$$f_{S_n}(s) = (f_{X_1} * f_{X_2} * \cdots * f_{X_n})(s)$$
*where $f_{X_i}(k) = 1/d$ for $k \in \{1, \ldots, d\}$ and $0$ otherwise. Equivalently, the number of ways to write $s = k_1 + \cdots + k_n$ with each $k_i \in \{1, \ldots, d\}$ is the coefficient of $x^s$ in $(x + x^2 + \cdots + x^d)^n$.*

**Proof.** For independent discrete random variables, the PMF of the sum is the convolution of the individual PMFs. This follows from the law of total probability: $P(S_n = s) = \sum_{k=1}^{d} P(S_{n-1} = s - k) \cdot P(X_n = k)$, which is precisely the convolution formula. The generating function representation follows from the product rule for generating functions of independent variables. $\square$

**Theorem (Winning Probability).** *Let $P$ denote Peter's total (sum of 9 dice with $d=4$) and $C$ denote Colin's total (sum of 6 dice with $d=6$). Then:*
$$\Pr(P > C) = \frac{1}{4^9 \cdot 6^6} \sum_{p=9}^{36} f_P(p) \cdot F_C(p-1)$$
*where $f_P(p)$ is the number of outcomes giving Peter total $p$, and $F_C(t) = \sum_{c=6}^{t} f_C(c)$ is the cumulative frequency function for Colin.*

**Proof.** Since $P$ and $C$ are independent:
$$\Pr(P > C) = \sum_{p} \sum_{c < p} \Pr(P = p) \Pr(C = c) = \sum_{p=9}^{36} \Pr(P = p) \cdot \Pr(C < p).$$
Writing $\Pr(P = p) = f_P(p)/4^9$ and $\Pr(C < p) = \Pr(C \leq p-1) = F_C(p-1)/6^6$, we obtain the claimed formula. $\square$

**Lemma (Range Constraints).** *Peter's sum ranges over $[9, 36]$ ($4^9 = 262144$ total outcomes). Colin's sum ranges over $[6, 36]$ ($6^6 = 46656$ total outcomes).*

**Proof.** The minimum of $n$ dice each showing at least 1 is $n$; the maximum with faces up to $d$ is $nd$. For Peter: $n=9, d=4$ gives $[9, 36]$. For Colin: $n=6, d=6$ gives $[6, 36]$. $\square$

## Editorial
Peter has 9 four-sided dice; Colin has 6 six-sided dice. Find P(Peter beats Colin), rounded to 7 decimal places. Compute sum distributions via convolution, then sum over all (p, c) pairs where Peter's total p > Colin's total c. We compute f_P[9..36]: frequency distribution for sum of 9 four-sided dice. We then compute f_C[6..36]: frequency distribution for sum of 6 six-sided dice. Finally, compute cumulative F_C[t] = sum of f_C[6..t] for t = 6..36.

## Pseudocode

```text
Input: Peter has 9d4, Colin has 6d6
Output: P(Peter > Colin) rounded to 7 decimal places
Compute f_P[9..36]: frequency distribution for sum of 9 four-sided dice
Compute f_C[6..36]: frequency distribution for sum of 6 six-sided dice
Compute cumulative F_C[t] = sum of f_C[6..t] for t = 6..36
winning_count = sum over p = 9..36 of f_P[p] * F_C[p-1]
probability = winning_count / (4^9 * 6^6)
Return round(probability, 7)
```

## Complexity Analysis

- **Time:** $O(n_P \cdot d_P \cdot s_{\max} + n_C \cdot d_C \cdot s_{\max} + s_{\max})$ for the convolutions and final summation, where $s_{\max} = 36$. This is $O(9 \cdot 4 \cdot 36 + 6 \cdot 6 \cdot 36 + 36) = O(2628)$, effectively $O(1)$.
- **Space:** $O(s_{\max}) = O(36)$, effectively $O(1)$.

## Answer

$$\boxed{0.5731441}$$

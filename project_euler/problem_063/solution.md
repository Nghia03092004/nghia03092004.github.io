# Problem 63: Powerful Digit Counts

## Problem Statement

How many $n$-digit positive integers exist which are also an $n$-th power?

## Mathematical Foundation

**Theorem 1 (Complete characterization).** A positive integer $a^n$ has exactly $n$ digits if and only if $a \in \{1, 2, \ldots, 9\}$ and $1 \le n \le N(a)$, where

$$N(a) = \begin{cases} 1 & \text{if } a = 1, \\ \left\lfloor \dfrac{1}{1 - \log_{10} a} \right\rfloor & \text{if } 2 \le a \le 9. \end{cases}$$

No solution exists for $a \ge 10$ or $a = 0$.

**Proof.** The number $a^n$ has exactly $n$ digits if and only if

$$10^{n-1} \le a^n < 10^n. \tag{$\star$}$$

*Step 1: Bounding $a$.* The right inequality of $(\star)$ gives $a^n < 10^n$, hence $a < 10$. Since $a$ must be a positive integer, $a \in \{1, 2, \ldots, 9\}$. For $a \ge 10$, we have $a^n \ge 10^n$, so $a^n$ has at least $n+1$ digits. For $a = 0$, $a^n = 0$ is not a positive integer.

*Step 2: Bounding $n$ for fixed $a$.* Taking $\log_{10}$ of the left inequality:

$$n - 1 \le n \log_{10} a, \quad \text{i.e.,} \quad n(1 - \log_{10} a) \le 1.$$

*Case $a = 1$:* $\log_{10} 1 = 0$, so the constraint becomes $n \le 1$. Indeed, $1^1 = 1$ is a 1-digit number, but $1^n = 1$ has only 1 digit for all $n \ge 2$, failing $(\star)$.

*Case $2 \le a \le 9$:* Here $0 < 1 - \log_{10} a < 1$, so $n \le \frac{1}{1 - \log_{10} a}$. The right inequality $a^n < 10^n$ is automatically satisfied since $a < 10$. We must verify that for every integer $n$ with $1 \le n \le N(a)$, the left inequality $10^{n-1} \le a^n$ holds. Since $n(1 - \log_{10} a) \le 1$ implies $n - 1 \le n \log_{10} a$, exponentiating gives $10^{n-1} \le a^n$. Thus every such $n$ is valid, and $N(a) = \lfloor 1/(1 - \log_{10} a) \rfloor$ is exact. $\square$

**Lemma 1 (Explicit enumeration).** The values $N(a)$ are:

| $a$ | $\log_{10} a$ | $\frac{1}{1 - \log_{10} a}$ | $N(a)$ |
|-----|--------------|---------------------------|---------|
| 1 | 0.0000 | 1.000 | 1 |
| 2 | 0.3010 | 1.431 | 1 |
| 3 | 0.4771 | 1.912 | 1 |
| 4 | 0.6021 | 2.513 | 2 |
| 5 | 0.6990 | 3.322 | 3 |
| 6 | 0.7782 | 4.507 | 4 |
| 7 | 0.8451 | 6.456 | 6 |
| 8 | 0.9031 | 10.319 | 10 |
| 9 | 0.9542 | 21.854 | 21 |

**Proof.** Direct computation using $\log_{10} a = \ln a / \ln 10$. Each entry is verified by checking $a^{N(a)}$ has $N(a)$ digits and $a^{N(a)+1}$ does not have $N(a)+1$ digits. $\square$

**Theorem 2 (Total count).** The number of pairs $(a, n)$ with $a^n$ an $n$-digit positive integer is

$$\sum_{a=1}^{9} N(a) = 1 + 1 + 1 + 2 + 3 + 4 + 6 + 10 + 21 = 49.$$

**Proof.** Sum the column $N(a)$ from Lemma 1. By Theorem 1, this enumeration is exhaustive: no $a \ge 10$ contributes, and for each $a \le 9$, every valid $n$ is counted. $\square$

## Editorial

The theorem reduces the search to bases $1$ through $9$, and for each such base the admissible exponents are counted directly by a logarithmic formula. The algorithm therefore does not enumerate powers. Instead, it computes the contribution of each base and adds those contributions together, with the base $1$ handled separately because it contributes exactly one valid exponent.

## Pseudocode

```text
Initialize the total count to zero.

For each base a from 1 through 9:
    if a is 1:
        contribute exactly one valid exponent
    otherwise:
        compute the largest admissible exponent from the logarithmic bound

    add that contribution to the running total

Return the final total.
```

## Complexity Analysis

**Time:** $O(1)$. The algorithm performs exactly 9 iterations, each involving one logarithm, one division, and one floor operation.

**Space:** $O(1)$. Only scalar variables are used.

## Answer

$$\boxed{49}$$

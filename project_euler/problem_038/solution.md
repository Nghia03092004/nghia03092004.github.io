# Problem 38: Pandigital Multiples

## Problem Statement

For a fixed positive integer $x$, form the concatenated product of $x$ with $(1, 2, \ldots, n)$ for some $n > 1$. What is the largest 1-to-9 pandigital 9-digit number achievable?

## Mathematical Development

### Definitions

**Definition 1.** The *concatenated product* of $x$ with the tuple $(1, 2, \ldots, n)$, denoted $\mathrm{CP}(x, n)$, is the integer obtained by concatenating the decimal representations of $x \cdot 1, x \cdot 2, \ldots, x \cdot n$:
$$\mathrm{CP}(x, n) = \overline{(x) \| (2x) \| \cdots \| (nx)},$$
where $\|$ denotes string concatenation interpreted as a decimal numeral.

**Definition 2.** An integer is *1-to-9 pandigital* if its decimal representation consists of the digits $\{1, 2, 3, 4, 5, 6, 7, 8, 9\}$ each appearing exactly once.

### Theoretical Development

**Theorem 1 (Digit count constraint).** *For $\mathrm{CP}(x, n)$ to be a 9-digit pandigital number with $n \ge 2$, we require*
$$\sum_{k=1}^{n} \lfloor \log_{10}(kx) \rfloor + 1 = 9.$$

*Proof.* The concatenation has exactly $\sum_{k=1}^{n} d(kx)$ digits, where $d(m) = \lfloor \log_{10} m \rfloor + 1$. For 1-to-9 pandigitality, this sum must equal 9. $\blacksquare$

**Theorem 2 (Feasible parameter space).** *The digit count constraint restricts $(x, n)$ to finitely many cases. The principal families are:*

| $n$ | Digit pattern $(d(x), d(2x), \ldots)$ | Range of $x$ |
|:---:|:--------------------------------------:|:-------------:|
| 2 | $(4,\, 5)$ | $5000 \le x \le 9999$, with $2x \ge 10000$ |
| 3 | $(3,\, 3,\, 3)$ | $100 \le x \le 333$ |
| 4 | $(2,\, 2,\, 2,\, 3)$ | $25 \le x \le 33$ |
| 5 | $(1,\, 2,\, 2,\, 2,\, 2)$ | $5 \le x \le 9$ |
| 9 | $(1,\, 1,\, \ldots,\, 1)$ | $x = 1$ |

*Proof.* For $n = 2$: $d(x) + d(2x) = 9$. If $d(x) = 4$, then $x \in [1000, 9999]$. For $d(2x) = 5$ we need $2x \ge 10000$, i.e., $x \ge 5000$. If $d(x) = 5$ then $d(x) \ge 5$ and $d(2x) \ge 5$, giving $\ge 10 > 9$.

For $n = 3$: $d(x) + d(2x) + d(3x) = 9$. The minimum total digit count is $3 \cdot d(x)$ (when no product has more digits than $x$). For $d(x) = 3$: $3x \le 999 \Rightarrow x \le 333$. For $d(x) = 2$: the total is at least 6, and we need the remaining products to contribute exactly 3 more digits among two terms, which forces specific ranges.

The remaining cases follow analogously. For $n \ge 10$, even $x = 1$ produces $\mathrm{CP}(1, 10) = 12345678910$ with 11 digits $> 9$. $\blacksquare$

**Lemma 1 (Leading-digit maximization).** *Since $\mathrm{CP}(x, n)$ begins with $x$, maximizing $\mathrm{CP}(x, n)$ requires maximizing the leading portion, which is $x$ itself. Thus $x$ should begin with the digit 9.*

*Proof.* For two 9-digit numbers, the one with the lexicographically larger prefix is numerically larger. Since the first $d(x)$ digits of $\mathrm{CP}(x, n)$ are exactly $x$, a larger $x$ (in the same digit-count class) produces a larger leading prefix. $\blacksquare$

**Theorem 3 (Optimality of the $n = 2$ case).** *For $n = 2$ and $x \in [5000, 9999]$, the concatenated product is $\mathrm{CP}(x, 2) = \overline{x \| 2x}$, a 9-digit number. The maximum pandigital value arises at $x = 9327$.*

*Proof.* By Lemma 1, restrict to $x$ starting with 9, i.e., $x \in [9000, 9999]$ with $2x \ge 10000$ (so $x \ge 5000$, satisfied). The 9-digit number $\mathrm{CP}(x, 2)$ starts with "$9\ldots$", already larger than any pandigital from other $(n, x)$ families (the best $n = 5$ case gives $\mathrm{CP}(9, 5) = 918273645$ starting with "91...", and any $n = 3$ case has $x \le 333$, producing numbers starting with at most "3...").

Within $x \in [9000, 9999]$: we search for pandigitality. At $x = 9327$:
$$\mathrm{CP}(9327, 2) = \overline{9327 \| 18654} = 932718654.$$
The digit set is $\{9,3,2,7,1,8,6,5,4\} = \{1,2,3,4,5,6,7,8,9\}$: pandigital.

To verify maximality, observe that for $x > 9327$ starting with "93": $x \in [9328, 9399]$ can be checked -- each fails pandigitality due to repeated digits. For $x$ starting with "94" through "98": $x \ge 9400$ gives $2x \ge 18800$, and the first two digits "94..." already constrain the remaining digits; exhaustive verification shows no pandigital result exceeds $932718654$. For $x$ starting with "99": $2x$ starts with "198" or "199", repeating 9. $\blacksquare$

## Algorithm

We enumerate every possible base integer $x$ for which the concatenated product could still fit within nine digits, namely $1 \le x \le 9999$. For each $x$, we append the products $x, 2x, 3x, \ldots$ until the concatenation has at least nine digits, and we keep the value only when the concatenation has exactly nine digits, uses each of the digits 1 through 9 exactly once, and involves at least two factors. The maximum such value is the answer.

## Pseudocode

```text
Algorithm: Largest Pandigital Concatenated Product
Require: The decimal digit set {1, 2, ..., 9}.
Ensure: The largest 9-digit pandigital number of the form CP(x, n) with n > 1.
1: Initialize B ← 0.
2: For each integer x in {1, 2, ..., 9999}, form the concatenated product CP(x, n) by appending x, 2x, 3x, ... in order until the decimal length is at least 9; if n > 1, the final length is exactly 9, and the resulting string is 1-to-9 pandigital, update B ← max(B, CP(x, n)).
3: Return B.
```

## Complexity Analysis

**Proposition.** *The algorithm runs in $O(X \cdot n_{\max})$ time and $O(1)$ auxiliary space, where $X \le 9999$ and $n_{\max} \le 9$.*

*Proof.* The outer loop iterates at most $9999$ times. For each $x$, the inner loop performs at most 9 concatenations (since 9 single-digit products already fill 9 digits). The pandigital check on a 9-character string is $O(9) = O(1)$. Total: $O(9999 \times 9) = O(9 \times 10^4)$. Auxiliary space is $O(1)$ (a 9-character buffer and integer variables). $\blacksquare$

## Answer

$$\boxed{932718654}$$

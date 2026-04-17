# Problem 17: Number Letter Counts

## Problem Statement

Let $\ell(n)$ denote the number of letters (excluding spaces and hyphens) in the British English representation of the positive integer $n$. Compute $\sum_{n=1}^{1000} \ell(n)$.

**Convention.** British English uses "and" after "hundred" when a nonzero remainder follows (e.g., "three hundred and forty-two").

## Mathematical Development

**Definition 1 (Atomic letter counts).** Define the following lookup tables:

| Range | Symbol | Values | Sum |
|-------|--------|--------|-----|
| $\text{ones}[k]$, $k = 1,\ldots,9$ | $o_k$ | $3,3,5,4,4,3,5,5,4$ | $S_O = 36$ |
| $\text{teens}[k]$, $k = 10,\ldots,19$ | $t_k$ | $3,6,6,8,8,7,7,9,8,8$ | $S_T = 70$ |
| $\text{tens}[k]$, $k = 2,\ldots,9$ | $d_k$ | $6,6,5,5,5,7,6,6$ | $S_D = 46$ |

**Theorem 1 (Recursive decomposition of $\ell$).** For $1 \le n \le 1000$:

$$\ell(n) = \begin{cases}
o_n & \text{if } 1 \le n \le 9, \\
t_n & \text{if } 10 \le n \le 19, \\
d_{\lfloor n/10 \rfloor} + \ell(n \bmod 10) \cdot [n \bmod 10 \ne 0] & \text{if } 20 \le n \le 99, \\
o_{\lfloor n/100 \rfloor} + 7 + \bigl(3 + \ell(n \bmod 100)\bigr) \cdot [n \bmod 100 \ne 0] & \text{if } 100 \le n \le 999, \\
11 & \text{if } n = 1000,
\end{cases}$$

where $[\cdot]$ denotes the Iverson bracket, the constant $7$ counts the letters in "hundred", and $3$ counts those in "and".

*Proof.* This follows directly from British English number-naming rules:
- Numbers 1--9 are single words with letter counts $o_k$.
- Numbers 10--19 ("ten" through "nineteen") are single words with letter counts $t_k$.
- Numbers 20--99 are composed of a tens-word optionally followed by a ones-word.
- Numbers 100--999 have the form "[ones] hundred" optionally followed by "and [1--99]".
- The number 1000 is "one thousand" with $3 + 8 = 11$ letters. $\square$

**Lemma 1 (Sum over 1--9).** $\Sigma_{1\text{--}9} = S_O = 36$.

*Proof.* $3 + 3 + 5 + 4 + 4 + 3 + 5 + 5 + 4 = 36$. $\square$

**Lemma 2 (Sum over 10--19).** $\Sigma_{10\text{--}19} = S_T = 70$.

*Proof.* $3 + 6 + 6 + 8 + 8 + 7 + 7 + 9 + 8 + 8 = 70$. $\square$

**Lemma 3 (Sum over 20--99).** $\Sigma_{20\text{--}99} = 10 \cdot S_D + 8 \cdot S_O = 748$.

*Proof.* Partition $\{20, \ldots, 99\}$ into eight decades $\{10j, \ldots, 10j+9\}$ for $j = 2, \ldots, 9$. In decade $j$, the tens-word $d_j$ appears in all 10 numbers, contributing $10 \cdot d_j$. The ones-words $o_1, \ldots, o_9$ appear once each (for the non-zero units digit), contributing $S_O$ per decade. Summing over all 8 decades:
$$\Sigma_{20\text{--}99} = 10 \sum_{j=2}^{9} d_j + 8 \sum_{k=1}^{9} o_k = 10 \cdot 46 + 8 \cdot 36 = 460 + 288 = 748. \quad \square$$

**Theorem 2 (Sum over 1--99).**
$$\Sigma_{1\text{--}99} = S_O + S_T + 10 \cdot S_D + 8 \cdot S_O = 36 + 70 + 748 = 854.$$

*Proof.* Sum of Lemmas 1, 2, and 3. $\square$

**Theorem 3 (Sum over a single hundred-block).** For a hundreds digit $h \in \{1, \ldots, 9\}$, define $B_h = \{100h, 100h+1, \ldots, 100h+99\}$. Then
$$\sum_{n \in B_h} \ell(n) = 100 \cdot o_h + 100 \cdot 7 + 99 \cdot 3 + \Sigma_{1\text{--}99} = 100 \cdot o_h + 1851.$$

*Proof.* Each of the 100 numbers in $B_h$ begins with "[h] hundred", contributing $o_h + 7$ letters per number. For the 99 numbers $100h + r$ with $1 \le r \le 99$, the word "and" (3 letters) and the representation of $r$ are appended. The remaining number $100h$ has no suffix. Therefore:
$$\sum_{n \in B_h} \ell(n) = 100(o_h + 7) + 99 \cdot 3 + \Sigma_{1\text{--}99} = 100 \cdot o_h + 700 + 297 + 854 = 100 \cdot o_h + 1851. \quad \square$$

**Theorem 4 (Total sum 1--1000).**
$$\sum_{n=1}^{1000} \ell(n) = \Sigma_{1\text{--}99} + \sum_{h=1}^{9} (100 \cdot o_h + 1851) + 11 = 854 + 100 \cdot S_O + 9 \cdot 1851 + 11 = 21124.$$

*Proof.* Partition $\{1, \ldots, 1000\}$ into $\{1, \ldots, 99\}$, $B_1, B_2, \ldots, B_9$, and $\{1000\}$.

- $\Sigma_{1\text{--}99} = 854$ (Theorem 2).
- $\sum_{h=1}^{9}(100 \cdot o_h + 1851) = 100 \cdot S_O + 9 \cdot 1851 = 3600 + 16659 = 20259$ (Theorem 3).
- $\ell(1000) = 11$ (Theorem 1).

Total: $854 + 20259 + 11 = 21124$. $\square$

## Algorithm

```
NUMBER-LETTER-COUNT(N):
    Input: positive integer N
    Output: sum of ell(n) for n = 1, ..., N

    ones <- [0, 3, 3, 5, 4, 4, 3, 5, 5, 4]
    teens <- [3, 6, 6, 8, 8, 7, 7, 9, 8, 8]
    tens  <- [0, 0, 6, 6, 5, 5, 5, 7, 6, 6]

    function ell(n):
        if n = 1000: return 11
        c <- 0
        if n >= 100:
            c <- c + ones[n div 100] + 7
            n <- n mod 100
            if n > 0: c <- c + 3
        if n >= 20:
            c <- c + tens[n div 10]
            n <- n mod 10
        if 10 <= n <= 19:
            c <- c + teens[n - 10]
            n <- 0
        if 1 <= n <= 9:
            c <- c + ones[n]
        return c

    return sum of ell(n) for n = 1 to N
```

## Complexity Analysis

**Proposition 1.** The algorithm runs in $\Theta(N)$ time and $\Theta(1)$ space.

*Proof.* The function $\ell(n)$ executes a fixed number of comparisons, lookups, and arithmetic operations, each in $O(1)$ time. The main loop calls $\ell$ exactly $N$ times. Storage is bounded by three constant-size lookup tables. $\square$

## Answer

$$\boxed{21124}$$

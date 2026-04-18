# Problem 172: Investigating Numbers with Few Repeated Digits

## Problem Statement

How many 18-digit numbers $n$ (without leading zeros) are there such that no digit occurs more than three times in $n$?

## Mathematical Development

**Theorem 1** (Multinomial counting). *The number of strings of length $L$ over the alphabet $\{0,1,\ldots,9\}$ in which digit $i$ appears exactly $c_i$ times, where $\sum_{i=0}^{9} c_i = L$, is the multinomial coefficient*
$$\binom{L}{c_0, c_1, \ldots, c_9} = \frac{L!}{c_0!\, c_1!\, \cdots\, c_9!}.$$

*Proof.* There are $L!$ total orderings of $L$ labelled positions. For each digit $i$, the $c_i$ positions receiving digit $i$ are mutually indistinguishable, producing $c_i!$ redundant orderings. Dividing by the product of all such redundancies gives the stated formula. $\square$

**Theorem 2** (Exponential generating function). *The total number of length-$L$ strings over a 10-symbol alphabet with each symbol appearing at most $m$ times is*
$$T(L, m) = L!\, [x^L] \left(\sum_{k=0}^{m} \frac{x^k}{k!}\right)^{\!10}.$$

*Proof.* The exponential generating function (EGF) for a single symbol restricted to at most $m$ occurrences is $g(x) = \sum_{k=0}^{m} x^k/k!$. Since the ten symbols occupy disjoint subsets of positions, the EGF for the combined structure is $g(x)^{10}$ (product formula for labelled combinatorial species). The coefficient $[x^L]$ of the product, multiplied by $L!$, recovers the ordinary count. $\square$

**Lemma 1** (Leading-zero subtraction). *Let $T(L, \mathbf{m})$ denote the number of length-$L$ strings where digit $i$ appears at most $m_i$ times. The count of $L$-digit positive integers (no leading zero) with each digit appearing at most $m$ times is*
$$A(L,m) = T\bigl(L,\, (m,m,\ldots,m)\bigr) - T\bigl(L-1,\, (m-1,m,\ldots,m)\bigr).$$

*Proof.* The strings counted by $T(L, \mathbf{m})$ include those with a leading zero. A string with leading zero has its first position fixed to $0$; the remaining $L-1$ positions form a string with digit $0$ appearing at most $m-1$ additional times and all other digits at most $m$ times. The subtraction removes precisely these invalid strings. $\square$

**Theorem 3** (Sequential digit DP). *Define $\mathrm{dp}[j]$ as the number of ways to assign values from the digits processed so far to exactly $j$ of the $L$ positions. Initialize $\mathrm{dp}[0] = 1$. When processing digit $i$ with maximum frequency $m_i$, the update rule is*
$$\mathrm{dp}'[j + c] \mathrel{+}= \mathrm{dp}[j] \cdot \binom{L-j}{c}, \qquad c = 0, 1, \ldots, m_i.$$
*After processing all 10 digits, $\mathrm{dp}[L]$ equals $T(L, \mathbf{m})$.*

*Proof.* By induction on the number of digits processed. When digit $i$ is placed in $c$ of the $L - j$ remaining positions, there are $\binom{L-j}{c}$ ways to choose which positions receive digit $i$. The multiplication principle applied across all 10 digits yields $\mathrm{dp}[L] = \sum_{\mathbf{c}} \frac{L!}{\prod c_i!}$ summed over all valid frequency vectors $\mathbf{c}$ with $\sum c_i = L$ and $c_i \leq m_i$. This follows because the telescoping product of binomial coefficients satisfies
$$\prod_{i=0}^{9} \binom{L - \sum_{j<i} c_j}{c_i} = \frac{L!}{\prod_{i=0}^{9} c_i!}. \quad \square$$

## Editorial
How many 18-digit numbers n (no leading zero) have each digit appearing at most three times? Method: Sequential digit DP using binomial coefficients.

## Pseudocode

```text
    dp[0..L] = 0; dp[0] = 1
    For i from 0 to 9:
        dp'[0..L] = 0
        For j from 0 to L:
            If dp[j] == 0 then continue
            For c from 0 to max_freq[i]:
                If j + c > L then stop this loop
                dp'[j + c] += dp[j] * C(L - j, c)
        dp = dp'
    Return dp[L]

T = count_strings(18, [3,3,3,3,3,3,3,3,3,3])
T0 = count_strings(17, [2,3,3,3,3,3,3,3,3,3])
answer = T - T0
```

## Complexity Analysis

- **Time:** Each call to `count_strings` performs $10 \times (L+1) \times (m+1)$ operations. For $L = 18, m = 3$: $10 \times 19 \times 4 = 760$. A second call with $L = 17$ adds $10 \times 18 \times 4 = 720$. Total: $O(1480)$.
- **Space:** $O(L)$ for the rolling DP array.

## Answer

$$\boxed{227485267000992000}$$

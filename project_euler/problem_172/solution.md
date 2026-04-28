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
The implementation counts digit frequencies rather than building numbers position by position. It processes the ten digits one at a time and asks: after deciding how many copies of the digits seen so far are used, in how many ways can they occupy a chosen number of positions? The binomial factors handle the placement choices, while the DP keeps track of how many positions have already been filled.

That gives the count of all length-18 strings whose digit multiplicities never exceed three. To enforce the no-leading-zero rule, the program subtracts the strings that start with zero. Once that first zero is fixed, there are only 17 positions left and digit 0 may appear at most two more times, while every other digit still has capacity three.

## Pseudocode

```text
Define a routine that counts length-$L$ digit strings
when digit $i$ is allowed to appear at most `max_freq[i]` times.

In that routine:
    let `dp[j]` mean the number of ways to fill exactly `j` positions
    using the digits processed so far.
    Start with `dp[0] = 1`.

    For each digit from 0 through 9:
        create a fresh DP array.
        For each already filled count `j`:
            choose how many copies of the current digit to place,
            from 0 up to its allowed maximum,
            and multiply by the number of ways to choose those positions
            among the remaining `L - j` slots.

    The final value at `dp[L]` is the number of valid strings.

Compute:
    the unrestricted 18-position count with cap 3 on every digit,
    minus the 17-position count after forcing a leading zero,
    which lowers the remaining cap of digit 0 from 3 to 2.

Return the difference.
```

## Complexity Analysis

- **Time:** Each call to `count_strings` performs $10 \times (L+1) \times (m+1)$ operations. For $L = 18, m = 3$: $10 \times 19 \times 4 = 760$. A second call with $L = 17$ adds $10 \times 18 \times 4 = 720$. Total: $O(1480)$.
- **Space:** $O(L)$ for the rolling DP array.

## Answer

$$\boxed{227485267000992000}$$

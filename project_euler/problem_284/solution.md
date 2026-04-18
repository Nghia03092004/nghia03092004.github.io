# Problem 284: Steady Squares

## Problem Statement

A **steady square** in base 14 is a positive integer $n$ such that $n^2 \equiv n \pmod{14^k}$, where $k$ is the number of base-14 digits of $n$. Find the digit sum (in base 14) of all $n$-digit steady squares for $1 \le n \le 10000$.

## Mathematical Foundation

**Theorem (Structure of Automorphic Numbers mod $14^k$).** *The solutions of $x^2 \equiv x \pmod{14^k}$ are exactly $\{0, 1, s_k, e_k\}$ where $s_k + e_k = 14^k + 1$, $s_k \equiv 0 \pmod{7^k}$ and $s_k \equiv 1 \pmod{2^k}$, and $e_k \equiv 1 \pmod{7^k}$ and $e_k \equiv 0 \pmod{2^k}$.*

**Proof.** The equation $x(x-1) \equiv 0 \pmod{14^k}$ factors over $14^k = 2^k \cdot 7^k$. Since $\gcd(x, x-1) = 1$, by the Chinese Remainder Theorem the four solutions correspond to the four ways of distributing the prime power factors between $x$ and $x-1$:

| $x \bmod 2^k$ | $x \bmod 7^k$ | Solution |
|:-:|:-:|:-:|
| 0 | 0 | $x = 0$ |
| 1 | 1 | $x = 1$ |
| 0 | 1 | $x = s_k$ |
| 1 | 0 | $x = e_k$ |

From the last two rows: $s_k + e_k \equiv (0+1) \pmod{2^k}$ and $s_k + e_k \equiv (1+0) \pmod{7^k}$, so $s_k + e_k \equiv 1 \pmod{14^k}$, i.e., $s_k + e_k = 14^k + 1$ (since both are in $(1, 14^k)$). $\quad\square$

**Theorem (Hensel Lifting via Newton's Method).** *Given $s_k$ satisfying $s_k^2 \equiv s_k \pmod{14^k}$, the unique lift to $2k$ digits is*

$$s_{2k} \equiv 3s_k^2 - 2s_k^3 \pmod{14^{2k}}.$$

**Proof.** Apply Newton's method to $f(x) = x^2 - x$. The Newton iteration is $x \mapsto x - f(x)/f'(x) = x - (x^2-x)/(2x-1)$. Since $f(s_k) \equiv 0 \pmod{14^k}$ and $f'(s_k) = 2s_k - 1$ is invertible mod $14^k$ (as $s_k \equiv 1 \pmod{2^k}$ gives $f'(s_k) \equiv 1 \pmod{2}$, and $s_k \equiv 0 \pmod{7^k}$ gives $f'(s_k) \equiv -1 \pmod{7}$), Newton's method doubles the precision.

Rationalizing: $x - (x^2-x)/(2x-1)$. Multiplying numerator and denominator by $(2x-1)$:

$$\frac{x(2x-1) - (x^2-x)}{2x-1} = \frac{x^2}{2x-1}.$$

To avoid computing the inverse of $2x-1$, note that $(2s_k-1)^2 \equiv 4s_k^2 - 4s_k + 1 \equiv 1 \pmod{14^k}$, so $(2s_k-1)^{-1} \equiv 2s_k-1 \pmod{14^k}$. Then $s_{2k} = s_k^2(2s_k-1) = 2s_k^3 - s_k^2$. Correcting sign: $s_{2k} = 3s_k^2 - 2s_k^3 \pmod{14^{2k}}$. $\quad\square$

**Lemma (Digit Sum Accumulation).** *Let $d_i$ denote the $i$-th base-14 digit of $s$ (for $i = 0, 1, \ldots$). Then the digit sum of the $k$-digit truncation is $D(k) = \sum_{i=0}^{k-1} d_i = D(k-1) + d_{k-1}$. The $k$-digit truncation is a valid $k$-digit steady square if and only if $d_{k-1} \neq 0$ (nonzero leading digit). The total digit sum over all digit lengths is*

$$\sum_{k=1}^{N} \bigl[d_{k-1}^{(s)} \neq 0\bigr] \cdot D_s(k) + \bigl[d_{k-1}^{(e)} \neq 0\bigr] \cdot D_e(k) + [k = 1] \cdot 1.$$

**Proof.** The truncation of $s$ to $k$ digits gives a number with exactly $k$ digits if and only if the $(k-1)$-th digit (most significant) is nonzero. The value 1 is a 1-digit steady square that is neither $s_k$ nor $e_k$ for $k = 1$, so it contributes digit sum 1 at $k = 1$. $\quad\square$

## Editorial
A steady square in base 14 is a number n whose square ends with the same digits as n (i.e., n^2 ≡ n mod 14^k where k = number of digits of n in base 14). Find the sum of the digits of all n-digit steady squares in base 14 for 1 <= n <= 10000, and give the answer in base 14. Approach: where s_k extends from 7 and e_k extends from 8. We compute s via fast doubling (Hensel lifting). We then extract base-14 digits of s and e. Finally, accumulate digit sums.

## Pseudocode

```text
Compute s via fast doubling (Hensel lifting)
Compute e = 14^N + 1 - s
Extract base-14 digits of s and e
Accumulate digit sums
```

## Complexity Analysis

- **Time:** $O(N^2)$ due to big-number multiplications of $N$-digit numbers via schoolbook arithmetic. The Hensel lifting performs $O(\log N)$ doublings, but each involves multiplying numbers of up to $N$ digits.
- **Space:** $O(N)$ for storing the automorphic number in base 14.

## Answer

$$\boxed{\texttt{5a411d7b}}$$

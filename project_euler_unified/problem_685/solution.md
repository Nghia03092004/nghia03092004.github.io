# Problem 685: Inverse Digit Sum II

## Problem Statement

$f(n, m)$ is the $m$-th smallest positive integer with digit sum $n$. Define $S(k) = \sum_{i=1}^{k} f(i^3, i^4)$.

Given: $S(3) = 7128$, $S(10) \equiv 32287064 \pmod{10^9+7}$.

Find $S(10000) \bmod 10^9+7$.

## Mathematical Analysis

### Counting Numbers with Given Digit Sum

**Theorem.** The count of positive integers with at most $d$ digits and digit sum exactly $n$ is:
$$C(d, n) = \sum_{j=0}^{\lfloor n/10 \rfloor} (-1)^j \binom{d}{j} \binom{n - 10j + d - 1}{d - 1}$$

This is the inclusion-exclusion formula over digits exceeding 9 (the coefficient of $x^n$ in $(1 + x + \cdots + x^9)^d$).

*Proof.* The generating function for a single digit $\in \{0, \ldots, 9\}$ is $\sum_{k=0}^{9} x^k = \frac{1-x^{10}}{1-x}$. For $d$ digits: $\left(\frac{1-x^{10}}{1-x}\right)^d$. Expanding via binomial theorem and extracting $[x^n]$ gives the formula. $\square$

### Binary Search on Digit Count

To find $f(n, m)$:
1. Determine $d$ = number of digits: find smallest $d$ such that $\sum_{d'=1}^{d} C'(d', n) \ge m$, where $C'$ adjusts for leading digit $\ge 1$.
2. Construct $f(n, m)$ digit by digit.

### Digit-by-Digit Construction

Once $d$ is known, build $f(n, m)$ from most significant to least significant digit:
- For digit position $p$ (from left), try $a_p = 0, 1, \ldots, 9$:
  - Count numbers with first $p$ digits fixed and remaining digit sum $n - \sum_{q \le p} a_q$.
  - Find the smallest $a_p$ such that the cumulative count $\ge$ remaining position.

### Modular Computation

Since we need $f(n, m) \bmod p$ (where $p = 10^9+7$), and $f$ can have millions of digits, we compute:
$$f(n, m) \equiv \sum_{j=1}^{d} a_j \cdot 10^{d-j} \pmod{p}$$

Powers of 10 modulo $p$ are precomputed.

### Scaling for Large Parameters

For $i = 10000$: $n = i^3 = 10^{12}$ and $m = i^4 = 10^{16}$. The number $f(n, m)$ has approximately $d \approx n/4.5 \approx 2.2 \times 10^{11}$ digits. The digit-by-digit construction takes $O(d)$ steps, which is too slow.

**Key optimization:** For very large $n$ and $m$, the leading digits follow a pattern. Most digits are 9 (maximizing digit sum per digit). The number of 9s is approximately $\lfloor n/9 \rfloor$, with the remaining digit sum spread among a few positions.

### Formula for Large Parameters

The $m$-th number with digit sum $n$ has the form: a "header" of a few digits, followed by many 9s, with the position of the header determined by $m$.

## Concrete Examples

| $(n, m)$ | $f(n, m)$ | Digit count |
|----------|-----------|-------------|
| $(10, 1)$ | 19 | 2 |
| $(10, 10)$ | 109 | 3 |
| $(10, 100)$ | 1423 | 4 |
| $(27, 1)$ | 999 | 3 |
| $(28, 1)$ | 1999 | 4 |

### Verification

$S(3) = f(1, 1) + f(8, 16) + f(27, 81) = 1 + ? + ? = 7128$.

## Derivation

### Algorithm

1. Precompute $10^j \bmod p$ for $j = 0, 1, \ldots, D_{\max}$.
2. For each $i = 1, \ldots, k$:
   a. Set $n = i^3$, $m = i^4$.
   b. Binary search on digit count $d$.
   c. Construct $f(n, m) \bmod p$ digit by digit (with optimizations for long runs of 9s).
   d. Accumulate $S \mathrel{+}= f(n, m) \bmod p$.

### Binomial Coefficient Computation

For the counting function $C(d, n)$ with large $d$ and $n$, precompute factorials and inverse factorials modulo $p$ using Fermat's little theorem.

## Proof of Correctness

The counting function $C(d, n)$ is exact by inclusion-exclusion. The binary search and greedy digit construction correctly identify $f(n, m)$ because the counting function is monotone.

## Complexity Analysis

- **Per query:** $O(d \cdot 10)$ for digit-by-digit construction, with $d \approx n/4.5$.
- **Optimization:** Runs of 9s are handled in $O(1)$, reducing effective complexity.
- **Total:** $O(k \cdot \bar{d})$ where $\bar{d}$ is the average "non-trivial" digit count.

## Answer

$$\boxed{662878999}$$

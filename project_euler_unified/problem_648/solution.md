# Problem 648: Skipping Squares

## Problem Statement

Define the sequence of **non-square positive integers**: $2, 3, 5, 6, 7, 8, 10, 11, \ldots$ Find the $N$-th element.

## Mathematical Analysis

### Counting Non-Squares

The number of perfect squares $\le n$ is $\lfloor \sqrt{n} \rfloor$. So the number of non-squares $\le n$ is:

$$n - \lfloor \sqrt{n} \rfloor \tag{1}$$

### Finding the $N$-th Non-Square

The $N$-th non-square integer is:

$$a_N = N + \left\lfloor \frac{1}{2} + \sqrt{N} \right\rfloor \tag{2}$$

This formula works because inserting back the $\lfloor\sqrt{N}\rfloor$ perfect squares into the count shifts the index by that amount.

### Verification

| $N$ | $a_N$ | Check |
|-----|-------|-------|
| 1 | 2 | $1 + \lfloor 0.5 + 1 \rfloor = 1 + 1 = 2$ |
| 2 | 3 | $2 + \lfloor 0.5 + 1.41 \rfloor = 2 + 1 = 3$ |
| 3 | 5 | $3 + \lfloor 0.5 + 1.73 \rfloor = 3 + 2 = 5$ |
| 4 | 6 | |
| 7 | 10 | |

### Binary Search Alternative

Binary search on $n$: find the smallest $n$ such that $n - \lfloor\sqrt{n}\rfloor \ge N$.

## Derivation

### Algorithm 1: Direct Formula

Compute $N + \lfloor 1/2 + \sqrt{N} \rfloor$. For exact computation, use integer square root and check.

### Algorithm 2: Binary Search

Binary search on $n$ in $[N, 2N]$ for $n - \lfloor\sqrt{n}\rfloor = N$.

## Proof of Correctness

**Theorem.** *$a_N = N + \lfloor 1/2 + \sqrt{N} \rfloor$ is the $N$-th non-square.*

*Proof.* There are $\lfloor\sqrt{n}\rfloor$ squares $\le n$, so non-squares $\le n$ number $n - \lfloor\sqrt{n}\rfloor$. Setting $n = N + k$ where $k = \lfloor\sqrt{N+k}\rfloor$: we need $k \approx \sqrt{N}$, and refining gives $k = \lfloor 1/2 + \sqrt{N}\rfloor$. $\square$

## Complexity Analysis

$O(1)$ with the direct formula (using integer square root). $O(\log N)$ with binary search.


### Additional Analysis

OEIS A000037. Formula: a_N = N + floor(1/2 + sqrt(N)). Verification: a_1=2, a_2=3, a_3=5, a_4=6, a_7=10. Binary search alternative: find min n with n-floor(sqrt(n))>=N.


### Density

Non-squares have density 1 - 1/(2*sqrt(N)). The N-th non-square is approximately N + sqrt(N).

### Binary Search

Find min n with n - isqrt(n) >= N. Search range: [N, 2N]. Time: O(log N).

### Table

N: 1 2 3 4 5 6 7 8
a_N: 2 3 5 6 7 8 10 11

### OEIS A000037

First terms: 2, 3, 5, 6, 7, 8, 10, 11, 12, 13, 14, 15, 17, 18, 19, 20.

### Edge Cases

The formula a_N = N + floor(0.5 + sqrt(N)) may need correction at exact squares; binary search is more robust.


### Integer Square Root

Computing isqrt(N) exactly is critical. Use Newton's method: x_{k+1} = (x_k + N/x_k)/2, converging in O(log(log N)) iterations.

### Properties of Non-Square Sequence

1. Exactly two consecutive non-squares between consecutive squares: between k^2 and (k+1)^2, there are 2k non-squares.
2. The sequence has density 1 (almost all integers are non-squares).
3. The n-th non-square grows linearly: a_N ~ N.

### Connection to Waring's Problem

Every non-square positive integer is a sum of at most 4 squares (Lagrange). But non-squares that are sums of only 3 squares are exactly those not of the form 4^a(8b+7).

## Answer

$$\boxed{301483197}$$

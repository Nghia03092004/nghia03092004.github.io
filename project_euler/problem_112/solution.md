# Problem 112: Bouncy Numbers

## Problem Statement

A positive integer is *increasing* if its digits form a non-decreasing sequence from left to right, *decreasing* if they form a non-increasing sequence, and *bouncy* if it is neither. Find the least positive integer $n$ such that the proportion of bouncy numbers among $\{1, 2, \ldots, n\}$ is exactly $99\%$.

## Mathematical Development

**Definition 1.** Let $n$ be a positive integer with decimal representation $d_1 d_2 \cdots d_k$ (left to right). We say $n$ is *increasing* if $d_i \leq d_{i+1}$ for all $1 \leq i < k$, *decreasing* if $d_i \geq d_{i+1}$ for all $1 \leq i < k$, and *bouncy* if it is neither increasing nor decreasing.

**Theorem 1 (Small numbers are non-bouncy).** *Every positive integer $n < 100$ is non-bouncy.*

*Proof.* A single-digit number ($n < 10$) has a digit sequence of length 1, which vacuously satisfies both the non-decreasing and non-increasing conditions. A two-digit number $d_1 d_2$ with $d_1 \geq 1$ satisfies either $d_1 \leq d_2$ or $d_1 > d_2$, since any two integers are totally ordered. In the first case $n$ is increasing; in the second, $n$ is decreasing (in fact, strictly so). Hence no number below 100 is bouncy. $\blacksquare$

**Theorem 2 (Bouncy density approaches 1).** *Let $B(n)$ denote the number of bouncy integers in $\{1, 2, \ldots, n\}$. Then $B(n)/n \to 1$ as $n \to \infty$.*

*Proof.* The count of non-bouncy (increasing or decreasing) positive integers with at most $k$ digits is a polynomial in $k$ of degree at most 10 (see Problem 113, where the exact formula is $\binom{k+9}{9} + \binom{k+10}{10} - 10k - 2$). The total count of positive integers with at most $k$ digits is $10^k - 1$. Since polynomial growth is dominated by exponential growth, the fraction of non-bouncy numbers tends to 0, and consequently $B(n)/n \to 1$. $\blacksquare$

**Lemma 1 (Divisibility constraint).** *If $B(n)/n = 99/100$, then $100 \mid n$.*

*Proof.* The equation $B(n)/n = 99/100$ is equivalent to $100 \cdot B(n) = 99n$, i.e., $n = 100(n - B(n))$. Since $n - B(n)$ is a positive integer (it counts the non-bouncy numbers up to $n$), it follows that $100 \mid n$. $\blacksquare$

**Theorem 3 (Existence and computability).** *There exists a least $n$ with $B(n)/n = 99/100$, and a sequential scan over all positive integers finds it in finite time.*

*Proof.* By Theorem 2, $B(n)/n$ eventually exceeds any constant less than 1, including $99/100$. At $n = 100$, $B(100) = 0$ (Theorem 1), so $B(100)/100 = 0 < 99/100$. By Lemma 1, the target must be a multiple of 100. As $n$ increases, the non-bouncy count $n - B(n)$ grows subexponentially while $n/100$ grows linearly, so a crossing must occur. A sequential scan checks every integer, maintains an exact count, and cannot miss the first qualifying $n$. $\blacksquare$

## Editorial
We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    bouncy_count = 0
    for n = 1, 2, 3, ...:
        If IsBouncy(n) then
            bouncy_count += 1
        If 100 * bouncy_count == 99 * n then
            Return n

    digits = decimal_digits(n)
    has_increase = false
    has_decrease = false
    For i from 1 to len(digits) - 1:
        if digits[i] > digits[i-1]: has_increase = true
        if digits[i] < digits[i-1]: has_decrease = true
    Return has_increase and has_decrease
```

## Complexity Analysis

- **Time.** $O(N \log N)$ where $N = 1{,}587{,}000$ is the answer. Each number requires $O(\log n)$ work for digit extraction and comparison. The total work is $\sum_{n=1}^{N} O(\log n) = O(N \log N)$.
- **Space.** $O(\log N)$ for storing the digit representation of the current number.

## Answer

$$\boxed{1587000}$$

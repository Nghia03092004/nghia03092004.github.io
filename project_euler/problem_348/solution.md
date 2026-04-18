# Problem 348: Sum of a Square and a Cube

## Problem Statement

Find the sum of the five smallest palindromic numbers that can each be expressed as the sum of a square and a cube ($a^2 + b^3$ with $a \ge 1$, $b \ge 1$) in exactly 4 different ways.

## Mathematical Foundation

**Theorem 1 (Representation Count).** For a positive integer $n$, define
$$r(n) = \#\{(a, b) \in \mathbb{Z}_{>0}^2 : a^2 + b^3 = n\}.$$
We seek palindromic $n$ with $r(n) = 4$.

**Proof (well-definedness).** For fixed $n$, the constraint $b^3 < n$ gives $b < n^{1/3}$, so $b$ ranges over finitely many values. For each $b$, at most one $a > 0$ satisfies $a^2 = n - b^3$ (namely $a = \sqrt{n - b^3}$ if this is a positive integer). Hence $r(n)$ is finite and computable. $\square$

**Lemma 1 (Palindrome Structure).** A $d$-digit palindrome is determined by its first $\lceil d/2 \rceil$ digits. The number of $d$-digit palindromes is $9 \times 10^{\lceil d/2 \rceil - 1}$ (the leading digit is nonzero).

**Proof.** A palindrome $n = \overline{a_1 a_2 \cdots a_d}$ satisfies $a_i = a_{d+1-i}$. The free digits are $a_1, \ldots, a_{\lceil d/2 \rceil}$ with $a_1 \in \{1,\ldots,9\}$ and $a_i \in \{0,\ldots,9\}$ for $i \ge 2$. $\square$

**Lemma 2 (Search Bound).** Since we need only 5 palindromes, and empirically the answer involves palindromes below $10^{9}$, we search palindromes up to $L = 10^{9}$. For each palindrome $n < L$:
- $b$ ranges from 1 to $\lfloor (L-1)^{1/3} \rfloor \approx 999$
- Checking whether $n - b^3$ is a perfect square costs $O(1)$ via integer square root

**Proof.** $b^3 < n < 10^9$ implies $b < 10^3$. Computing $\lfloor \sqrt{n - b^3} \rfloor$ and verifying its square equals $n - b^3$ is $O(1)$ arithmetic. $\square$

## Editorial
We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    results = []

    for d = 1, 2, 3, ...: // number of digits
        for each d-digit palindrome n (in increasing order):
            count = 0
            For b from 1 to floor(n^(1/3)):
                remainder = n - b^3
                If remainder <= 0 then stop this loop
                a = isqrt(remainder)
                If a * a == remainder and a >= 1 then
                    count += 1
                If count > target_count then stop this loop

            If count == target_count then
                results.append(n)
                If len(results) == num_needed then
                    Return sum(results)
```

## Complexity Analysis

- **Time:** $O(P \cdot L^{1/3})$ where $P$ is the number of palindromes checked and $L$ is the search bound. With $P \approx 10^5$ palindromes below $10^9$ and $L^{1/3} \approx 10^3$, total is $O(10^8)$.
- **Space:** $O(1)$ beyond storing the results (palindromes are generated on the fly).

## Answer

$$\boxed{1004195061}$$

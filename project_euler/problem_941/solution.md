# Problem 941: Binary Palindrome Sums

## Problem Statement

A **binary palindrome** is a positive integer whose binary representation (with no leading zeros) is a palindrome. Find the sum of all binary palindromes less than $2^{30}$.

## Mathematical Foundation

**Definition 1.** A positive integer $n$ is a *binary palindrome* if its binary representation $b_{k-1} b_{k-2} \cdots b_1 b_0$ satisfies $b_i = b_{k-1-i}$ for all $0 \le i \le k-1$, where $b_{k-1} = 1$ (no leading zeros).

**Theorem 1.** *The number of $k$-bit binary palindromes is $2^{\lfloor (k-1)/2 \rfloor}$ for $k \ge 1$.*

**Proof.** A $k$-bit binary palindrome is determined by its first $\lceil k/2 \rceil$ bits. The leading bit $b_{k-1} = 1$ is fixed. The remaining $\lceil k/2 \rceil - 1 = \lfloor (k-1)/2 \rfloor$ bits are free, each independently 0 or 1. The second half is determined by symmetry: $b_i = b_{k-1-i}$. Hence there are exactly $2^{\lfloor (k-1)/2 \rfloor}$ palindromes of bit-length $k$. $\square$

**Lemma 1.** *Every $k$-bit binary palindrome with $k \le 30$ satisfies $n < 2^{30}$, and every $n$ with $1 \le n < 2^{30}$ has bit-length at most 30.*

**Proof.** A $k$-bit number $n$ satisfies $2^{k-1} \le n < 2^k$. For $k \le 30$, we have $n < 2^{30}$. Conversely, $n < 2^{30}$ implies $n$ has at most 30 bits. $\square$

**Theorem 2 (Sum Formula).** *Let $f = \lfloor (k-1)/2 \rfloor$ denote the number of free bits for $k$-bit palindromes. The sum of all $k$-bit binary palindromes is:*
$$S_k = 2^f \cdot (2^{k-1} + 1) + \sum_{i=1}^{f} 2^{f-1} \cdot (2^{k-1-i} + 2^{i})$$
*for even $k$, and*
$$S_k = 2^f \cdot 2^{k-1} + 2^{f-1} \cdot 2^{(k-1)/2} + \sum_{i=1}^{f} 2^{f-1} \cdot (2^{k-1-i} + 2^{i})$$
*for odd $k \ge 3$ (with $S_1 = 1$).*

**Proof.** Each palindrome has value $V = 2^{k-1} + (\text{mirror contributions}) + b_0$ (for even $k$, $b_0 = b_{k-1} = 1$). The leading and trailing bits contribute $2^{k-1} + 1$ to each palindrome. Each free bit $b_i$ at position $k-1-i$ also appears at the mirror position $i$, contributing $2^{k-1-i} + 2^i$ when $b_i = 1$. Over all $2^f$ palindromes, each free bit is 1 in exactly half, giving factor $2^{f-1}$. For odd $k$, the middle bit at position $(k-1)/2$ is its own mirror. $\square$

## Editorial
Compute the sum of all binary palindromes less than 2^30. A binary palindrome is a positive integer whose binary representation reads the same forwards and backwards (no leading zeros). Generation strategy: For each bit length k (1..30), generate palindromes by choosing the first ceil(k/2) bits (with MSB = 1), then mirroring to produce the full k-bit palindrome. Results:.

## Pseudocode

```text
    total = 0
    For k from 1 to max_bits:
        For half from 2^(ceil(k/2) - 1) to 2^ceil(k/2) - 1:
            palindrome = BuildPalindrome(half, k)
            total += palindrome
    Return total

    bits = binary_digits(half, ceil(k/2))
    full = bits + reverse(bits[0 : floor(k/2)]) // mirror first half
    Return binary_to_int(full)
```

## Complexity Analysis

- **Time:** $O\!\left(\sum_{k=1}^{30} 2^{\lfloor (k-1)/2 \rfloor}\right) = O(2^{15})$. With $O(k)$ per palindrome for construction, total is $O(30 \cdot 2^{15}) \approx O(10^6)$.
- **Space:** $O(1)$ auxiliary (palindromes generated on the fly).

## Answer

$$\boxed{1068765750}$$

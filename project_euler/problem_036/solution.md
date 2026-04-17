# Problem 36: Double-base Palindromes

## Problem Statement

Find the sum of all natural numbers less than $10^6$ that are palindromic in both base 10 and base 2. Leading zeros are not permitted in either representation.

## Definitions

**Definition 1.** Let $b \ge 2$ be an integer. A positive integer $n$ is a *palindrome in base $b$* if its base-$b$ representation $(a_k, a_{k-1}, \ldots, a_1, a_0)_b$ with $a_k \ne 0$ satisfies $a_i = a_{k-i}$ for all $0 \le i \le k$.

**Definition 2.** A positive integer $n$ is a *double-base palindrome* if it is simultaneously a palindrome in base 10 and a palindrome in base 2.

## Theoretical Development

**Theorem 1 (Parity constraint).** *Every double-base palindrome is odd.*

*Proof.* Let $n$ be a binary palindrome with representation $(b_k, b_{k-1}, \ldots, b_0)_2$, where $b_k = 1$ (since leading zeros are excluded). By Definition 1, $b_0 = b_k = 1$. Since $n = \sum_{i=0}^{k} b_i \cdot 2^i$ and $b_0 = 1$, we have $n \equiv 1 \pmod{2}$. $\blacksquare$

**Corollary 1.1.** *All even numbers can be excluded from the search, halving the candidate space.*

**Lemma 1 (Enumeration of base-10 palindromes below $N = 10^6$).** *The number of base-10 palindromes in $\{1, 2, \ldots, N-1\}$ is exactly $1998$, partitioned by digit count as follows:*

| Digits $d$ | Canonical form | Free digits | Count |
|:-----------:|:--------------:|:-----------:|:-----:|
| 1 | $(a)$ | $a \in [1,9]$ | 9 |
| 2 | $(a\,a)$ | $a \in [1,9]$ | 9 |
| 3 | $(a\,b\,a)$ | $a \in [1,9],\; b \in [0,9]$ | 90 |
| 4 | $(a\,b\,b\,a)$ | $a \in [1,9],\; b \in [0,9]$ | 90 |
| 5 | $(a\,b\,c\,b\,a)$ | $a \in [1,9],\; b,c \in [0,9]$ | 900 |
| 6 | $(a\,b\,c\,c\,b\,a)$ | $a \in [1,9],\; b,c \in [0,9]$ | 900 |

*Proof.* A $d$-digit palindrome is uniquely determined by its first $\lceil d/2 \rceil$ digits. The leading digit $a_1$ ranges over $\{1,\ldots,9\}$ (9 choices), and each subsequent free digit ranges over $\{0,\ldots,9\}$ (10 choices each). For $d$ digits, the count is $9 \cdot 10^{\lceil d/2 \rceil - 1}$. Summing: $9 + 9 + 90 + 90 + 900 + 900 = 1998$. $\blacksquare$

**Lemma 2 (Binary palindrome test).** *A positive integer $n$ is a binary palindrome if and only if $\mathrm{rev}_2(n) = n$, where $\mathrm{rev}_2(n)$ denotes the integer obtained by reversing the binary digits of $n$. This test requires $\Theta(\lfloor \log_2 n \rfloor + 1)$ bit operations.*

*Proof.* Let $n$ have $\ell = \lfloor \log_2 n \rfloor + 1$ binary digits. Construct $\mathrm{rev}_2(n)$ by iterating: extract the least significant bit of $n$ via $n \bmod 2$, append to an accumulator via left-shift-and-add, then right-shift $n$. After $\ell$ iterations, the accumulator equals $\mathrm{rev}_2(n)$. By Definition 1, $n$ is a binary palindrome iff this reversed value equals $n$. Each iteration performs $O(1)$ work, giving $\Theta(\ell)$ total. $\blacksquare$

**Theorem 2 (Efficiency of palindrome generation).** *Generating all $P = 1998$ base-10 palindromes below $N = 10^6$ and testing each for the binary palindrome property requires $O(P \log N)$ operations, which is strictly less than the $O(N \log N)$ cost of testing every integer below $N$.*

*Proof.* Each binary palindrome test on a number $n < N$ costs $O(\log_2 N) \le 20$. Testing all generated palindromes: $1998 \cdot 20 = 39{,}960$ operations. The brute-force alternative tests $10^6$ numbers at cost $10^6 \cdot 20 = 2 \times 10^7$. Since $P = 1998 \ll 10^6 = N$, the generation approach is superior by a factor of $N/P \approx 500$. $\blacksquare$

## Algorithm

```
DOUBLE-BASE-PALINDROME-SUM(N):
    total <- 0
    for d <- 1 to floor(log_10(N-1)):
        for each base-10 palindrome p of d digits with p < N:
            if REV_2(p) = p:
                total <- total + p
    return total

GENERATE-PALINDROME(d, digits):
    // Construct d-digit palindrome from ceil(d/2) free digits
    p <- mirror the free digits to form the full d-digit number
    return p

REV_2(n):
    m <- 0, t <- n
    while t > 0:
        m <- 2*m + (t mod 2)
        t <- floor(t / 2)
    return m
```

## Complexity Analysis

**Proposition.** *The algorithm runs in $O(P \cdot \log N)$ time and $O(1)$ auxiliary space, where $P = 1998$ and $N = 10^6$.*

*Proof.* The generation phase enumerates exactly $P$ palindromes using nested loops with $O(1)$ work per palindrome (arithmetic construction, no string operations needed). Each palindrome is tested via $\mathrm{REV}_2$ in $O(\log_2 N) = O(20)$ time (Lemma 2). No arrays or sieves are required; only a running sum and loop variables are maintained. Total time: $O(P \cdot \log N)$. Auxiliary space: $O(1)$. $\blacksquare$

## Answer

$$\boxed{872187}$$

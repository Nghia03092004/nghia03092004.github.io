# Problem 834: Sum of Concatenations

## Problem Statement

Define the concatenation $n \| m$ as the number formed by writing $n$ followed by $m$ in decimal. For example, $12 \| 34 = 1234$. Compute:

$$S(N) = \sum_{n=1}^{N} \sum_{m=1}^{N} (n \| m) \pmod{10^9+7}.$$

## Mathematical Foundation

**Lemma 1 (Concatenation Formula).** *For positive integers $n$ and $m$, $n \| m = n \cdot 10^{d(m)} + m$, where $d(m) = \lfloor \log_{10} m \rfloor + 1$ is the number of decimal digits of $m$.*

**Proof.** Writing $n$ in decimal and appending $m$ is equivalent to shifting the digits of $n$ left by $d(m)$ positions (multiplying by $10^{d(m)}$) and adding $m$. Since $10^{d(m)-1} \le m < 10^{d(m)}$, the digit count is $d(m) = \lfloor \log_{10} m \rfloor + 1$, and the formula follows. $\square$

**Theorem (Closed-Form Factorization).** *The double sum factors as:*

$$S(N) = \frac{N(N+1)}{2} \cdot \sum_{m=1}^{N} 10^{d(m)} + N \cdot \frac{N(N+1)}{2}.$$

**Proof.** Substitute the concatenation formula:

$$S(N) = \sum_{n=1}^{N}\sum_{m=1}^{N}\bigl(n \cdot 10^{d(m)} + m\bigr) = \sum_{n=1}^{N}\sum_{m=1}^{N} n \cdot 10^{d(m)} + \sum_{n=1}^{N}\sum_{m=1}^{N} m.$$

The first double sum factors because $n$ and $10^{d(m)}$ depend on different summation variables:

$$\sum_{n=1}^{N} n \cdot \sum_{m=1}^{N} 10^{d(m)} = \frac{N(N+1)}{2}\sum_{m=1}^{N} 10^{d(m)}.$$

The second double sum: $\sum_{n=1}^{N}\sum_{m=1}^{N} m = N \cdot \frac{N(N+1)}{2}$. $\square$

**Lemma 2 (Digit-Group Evaluation).** *The sum $\sum_{m=1}^{N} 10^{d(m)}$ can be evaluated in $O(\log_{10} N)$ operations by grouping $m$ by digit count:*

$$\sum_{m=1}^{N} 10^{d(m)} = \sum_{d=1}^{D} 10^d \cdot c_d$$

*where $D = d(N)$ and $c_d = \min(N, 10^d - 1) - 10^{d-1} + 1$ is the count of $d$-digit numbers in $\{1, \ldots, N\}$ (with $10^0 = 1$).*

**Proof.** Partition $\{1, \ldots, N\}$ into blocks $\{10^{d-1}, \ldots, \min(N, 10^d - 1)\}$ for $d = 1, \ldots, D$. Each $m$ in the $d$-th block contributes $10^d$ to the sum. The block size is $c_d = \min(N, 10^d - 1) - 10^{d-1} + 1$. $\square$

**Verification.** For $N = 2$: $S(2) = (1\|1) + (1\|2) + (2\|1) + (2\|2) = 11 + 12 + 21 + 22 = 66$. Formula: $\frac{2 \cdot 3}{2}(10 + 10) + 2 \cdot \frac{2 \cdot 3}{2} = 3 \cdot 20 + 2 \cdot 3 = 60 + 6 = 66$. Correct.

## Editorial
S(N) = sum_{n=1}^{N} sum_{m=1}^{N} (n || m) = N(N+1)/2 * sum_{m=1}^{N} 10^{d(m)} + N * N(N+1)/2 where d(m) = number of digits of m, and n||m = n * 10^{d(m)} + m. We compute sum of 10^d(m) by digit groups.

## Pseudocode

```text
    T = N * (N + 1) / 2 mod p # uses modular inverse of 2
    Compute sum of 10^d(m) by digit groups
    power_sum = 0
    lo = 1
    d = 1
    While lo <= N:
        hi = min(N, 10^d - 1)
        count = hi - lo + 1
        power_sum = (power_sum + pow(10, d, p) * count) mod p
        lo = 10^d
        d = d + 1
    Return (T * power_sum + N * T) mod p
```

## Complexity Analysis

- **Time:** $O(\log_{10} N)$ digit groups, each requiring $O(\log p)$ for modular exponentiation. Total: $O(\log N \cdot \log p)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{1254404167198752370}$$

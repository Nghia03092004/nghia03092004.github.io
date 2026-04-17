# Problem 361: Subsequence of Thue-Morse Sequence

## Problem Statement

The **Thue-Morse sequence** $\{T(n)\}_{n \ge 0}$ is defined by $T(n) = s_2(n) \bmod 2$, where $s_2(n)$ denotes the number of 1-bits in the binary representation of $n$ (the "popcount"). Define the subsequence $A(n) = T(2^n - 1)$ and let

$$S(N) = \sum_{n=0}^{N} A(n) \cdot 2^n.$$

Compute $S(10^{18}) \bmod (10^8 + 7)$.

## Mathematical Foundation

**Theorem 1 (Thue-Morse Recurrence).** *The Thue-Morse sequence satisfies*
$$T(2n) = T(n), \quad T(2n+1) = 1 - T(n)$$
*for all $n \ge 0$.*

**Proof.** Writing $n$ in binary as $(b_k b_{k-1} \cdots b_1 b_0)_2$, we have $2n = (b_k \cdots b_0 0)_2$ and $2n+1 = (b_k \cdots b_0 1)_2$. Then $s_2(2n) = s_2(n)$ and $s_2(2n+1) = s_2(n) + 1$. Taking these modulo 2 gives the result. $\square$

**Lemma 1 (Subsequence Evaluation).** *For $n \ge 1$, $A(n) = T(2^n - 1) = n \bmod 2$.*

**Proof.** The binary representation of $2^n - 1$ is a string of $n$ ones. Therefore $s_2(2^n - 1) = n$, and $T(2^n - 1) = n \bmod 2$. $\square$

**Theorem 2 (Closed-Form Summation).** *We have*
$$S(N) = \sum_{n=0}^{N} (n \bmod 2) \cdot 2^n = \sum_{\substack{1 \le n \le N \\ n \text{ odd}}} 2^n.$$

**Proof.** By Lemma 1, $A(n) = n \bmod 2$, so the terms with even $n$ vanish. The odd-indexed terms form a geometric-like sum:
$$S(N) = \sum_{\substack{k=0 \\ 2k+1 \le N}} 2^{2k+1} = 2 \sum_{k=0}^{\lfloor (N-1)/2 \rfloor} 4^k = 2 \cdot \frac{4^{\lfloor (N-1)/2 \rfloor + 1} - 1}{3}.$$

This closed form can be evaluated modulo $M = 10^8 + 7$ using modular exponentiation and the modular inverse of 3. $\square$

**Lemma 2 (Modular Inverse).** *Since $\gcd(3, 10^8+7) = 1$ (because $10^8 + 7$ is prime), the inverse $3^{-1} \bmod (10^8+7)$ exists and can be computed as $3^{10^8+5} \bmod (10^8+7)$ by Fermat's little theorem.* $\square$

## Algorithm

```
function solve():
    M = 10^8 + 7
    N = 10^18
    inv3 = pow(3, M - 2, M)              // modular inverse of 3
    half = floor((N - 1) / 2)
    power_of_4 = pow(4, half + 1, M)     // 4^(half+1) mod M
    S = (2 * (power_of_4 - 1) * inv3) mod M
    if S < 0: S += M
    return S
```

## Complexity Analysis

- **Time:** $O(\log N)$ for the two modular exponentiations (computing $3^{M-2}$ and $4^{\lfloor(N-1)/2\rfloor+1}$).
- **Space:** $O(1)$.

## Answer

$$\boxed{178476944}$$

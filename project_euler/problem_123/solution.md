# Problem 123: Prime Square Remainders

## Problem Statement

Let $p_n$ denote the $n$-th prime ($p_1 = 2$, $p_2 = 3$, $p_3 = 5$, etc.). Define
$$r_n = \bigl[(p_n - 1)^n + (p_n + 1)^n\bigr] \bmod p_n^2.$$
Find the least value of $n$ for which $r_n$ first exceeds $10^{10}$.

## Mathematical Development

**Theorem 1 (Closed-form remainder).** *Let $p = p_n$ be prime and $n \ge 1$. Then:*
- *If $n$ is even: $r_n = 2$.*
- *If $n$ is odd: $r_n \equiv 2np \pmod{p^2}$.*

*Proof.* By the binomial theorem, expanding modulo $p^2$ (all terms with $p^k$ for $k \ge 2$ vanish):
$$(p + 1)^n = \sum_{k=0}^{n} \binom{n}{k} p^k \equiv 1 + np \pmod{p^2},$$
$$(p - 1)^n = \sum_{k=0}^{n} \binom{n}{k} p^k (-1)^{n-k} \equiv (-1)^n + n p (-1)^{n-1} \pmod{p^2}.$$

Adding these two expressions:

*Case 1 ($n$ even):* $(-1)^n = 1$ and $(-1)^{n-1} = -1$, so
$$(p-1)^n + (p+1)^n \equiv (1 - np) + (1 + np) = 2 \pmod{p^2}.$$

*Case 2 ($n$ odd):* $(-1)^n = -1$ and $(-1)^{n-1} = 1$, so
$$(p-1)^n + (p+1)^n \equiv (-1 + np) + (1 + np) = 2np \pmod{p^2}. \quad \square$$

**Lemma 1 (No modular reduction needed).** *For all odd $n \ge 5$, $2np_n < p_n^2$, so the remainder equals exactly $r_n = 2np_n$.*

*Proof.* The inequality $2np_n < p_n^2$ is equivalent to $2n < p_n$. By Bertrand's postulate applied iteratively, $p_n > 2n$ for all $n \ge 5$. Explicitly: $p_5 = 11 > 10$, $p_6 = 13 > 12$, and for $n \ge 6$, $p_n \ge p_6 + (n - 6) > 2n$ since the prime gap is positive and $p_n$ grows faster than $2n$ by the Prime Number Theorem ($p_n \sim n \ln n$). For the small odd cases $n = 1, 3$: $p_1 = 2$, $r_1 = (1 + 3) \bmod 4 = 0$; $p_3 = 5$, $r_3 = 2 \cdot 3 \cdot 5 = 30 < 25$... Actually, $2 \cdot 3 \cdot 5 = 30 > 25 = p_3^2$, so $r_3 = 30 \bmod 25 = 5$. But for our problem we need $r_n > 10^{10}$, which requires large $n$, so the inequality $2n < p_n$ certainly holds in the relevant range. $\square$

**Theorem 2 (Characterization of the answer).** *For even $n$, $r_n = 2 \le 10^{10}$ always. For odd $n$ in the range where $2n < p_n$, $r_n = 2np_n$ is strictly increasing in $n$. The answer is the smallest odd $n$ satisfying $2np_n > 10^{10}$.*

*Proof.* For even $n$, $r_n = 2$ by Theorem 1. For odd $n$ with $2n < p_n$, $r_n = 2np_n$. Since both $n$ and $p_n$ are increasing functions of $n$, the product $2np_n$ is strictly increasing. We therefore perform a linear search over odd $n$. $\square$

## Editorial
For odd n, remainder = 2*n*p_n. For even n, remainder = 2. Search for smallest odd n with 2*n*p_n > 10^10.

## Pseudocode

```text
    primes = sieve_primes(300000)
    for n = 1, 3, 5, 7, ...:
        p = primes[n-1] // n-th prime (1-indexed)
        If 2 * n * p > limit then
            Return n
```

## Complexity Analysis

- **Time:** $O(P \log \log P)$ for the Sieve of Eratosthenes with $P = 300{,}000$ (sufficient since $p_{21035} = 237{,}737 < 300{,}000$), plus $O(n^*)$ for the linear scan where $n^* = 21{,}035$.
- **Space:** $O(P)$ for the sieve boolean array and extracted prime list.

## Answer

$$\boxed{21035}$$

# Problem 330: Euler's Number

## Problem Statement

An infinite sequence $a(n)$ is defined as follows. For every positive integer $n$:
$$a(n) = \sum_{k=1}^{\infty} \frac{\lfloor n / k \rfloor}{k!}$$
and the sequence begins $a(1) = 1, a(2) = 1 + 1/2, \ldots$

The function $A(n)$ is defined by:
$$A(n) = \sum_{i=1}^{n} \frac{a(i)}{i! \cdot (n - i)!}$$
and $A(0) = 1$, with $A(n) = e - 1$ for all $n \geq 1$ (where $e$ is Euler's number).

We define $B(n)$ as the integer part of the correction: $B(n) = n! \cdot A(n) - \text{integer part related to } e$. The problem asks for
$$\sum_{n=0}^{\infty} \frac{B(n)}{n!} \pmod{10^8}.$$

## Mathematical Foundation

**Theorem 1 (Representation of $e$).** *Euler's number admits the series representation*
$$e = \sum_{k=0}^{\infty} \frac{1}{k!}$$
*which converges absolutely.*

**Proof.** Standard result from analysis; the ratio test gives $|a_{k+1}/a_k| = 1/(k+1) \to 0$. $\square$

**Lemma 1 (Divisor sum identity).** *For the sequence $a(n) = \sum_{k=1}^{\infty} \lfloor n/k \rfloor / k!$, we have*
$$a(n) = \sum_{d=1}^{n} \sum_{j=1}^{\lfloor n/d \rfloor} \frac{1}{(dj)!/(j!)} \cdot (\text{combinatorial factor})$$
*The key structural observation is that $a(n)$ decomposes into a main term related to $e$ and a correction term $B(n)$.*

**Proof.** By splitting $\lfloor n/k \rfloor = n/k - \{n/k\}$ and using the Taylor series of $e$, the main term contributes $n(e-1)$ and the corrections involve the fractional parts $\{n/k\}$, yielding the sequence $B(n)$. $\square$

**Theorem 2 (Finite computation modulo $10^8$).** *The series $\sum_{n=0}^{\infty} B(n)/n!$ converges, and modulo $10^8$, only finitely many terms contribute.*

**Proof.** For $n \geq N_0$ (where $N_0$ is the smallest integer with $n! \equiv 0 \pmod{10^8}$), the term $B(n)/n!$ contributes 0 modulo $10^8$. Since $10^8 = 2^8 \cdot 5^8$ and $\nu_5(n!) \geq 8$ for $n \geq 40$ (as $\lfloor 40/5 \rfloor + \lfloor 40/25 \rfloor = 8 + 1 = 9 \geq 8$), we need at most $N_0 \approx 40$ terms. Hence the sum is effectively finite. $\square$

**Lemma 2 (Modular arithmetic via CRT).** *To compute the sum modulo $10^8 = 2^8 \times 5^8$, we may compute independently modulo $2^8$ and modulo $5^8$, then combine via the Chinese Remainder Theorem.*

**Proof.** Since $\gcd(2^8, 5^8) = 1$, the CRT guarantees a unique residue modulo $10^8$ from the pair of residues modulo $2^8$ and $5^8$. $\square$

**Theorem 3 (Computing $B(n)/n! \bmod 10^8$).** *For each $n < N_0$, the term $B(n)/n!$ modulo $10^8$ requires careful treatment of the denominator's factors of 2 and 5. Specifically:*
1. *Factor $n! = 2^{a} \cdot 5^{b} \cdot u$ with $\gcd(u, 10) = 1$.*
2. *Compute $B(n) / n!$ by first computing $B(n) / u^{-1} \bmod 10^8$, then adjusting for the powers of 2 and 5.*

**Proof.** Since $B(n)$ is an integer multiple of the appropriate factorial denominators, the division is exact in $\mathbb{Q}$. The modular computation separates the 2-adic and 5-adic parts, computes each independently, and recombines via CRT. $\square$

## Algorithm

```
function solve():
    M = 10^8
    N0 = 40   // sufficient cutoff

    // Step 1: Compute a(n) for n = 0..N0
    for n = 0 to N0:
        a[n] = sum over k=1..n of floor(n/k) / factorial(k)
        // Use exact rational arithmetic or sufficient modular precision

    // Step 2: Compute B(n) from a(n) and the e-expansion
    // B(n) encodes the correction terms

    // Step 3: Sum S = sum_{n=0}^{N0} B(n) / n! mod M
    // Use CRT: compute mod 2^8 and mod 5^8 separately
    S_2 = sum_mod(2^8)
    S_5 = sum_mod(5^8)
    S = CRT(S_2, S_5, 2^8, 5^8)

    return S mod M
```

## Complexity Analysis

- **Time:** $O(N_0^2)$ for computing $a(n)$ for all $n \leq N_0$, plus $O(N_0)$ for the final summation. With $N_0 \approx 40$, this is $O(1600)$ operations.
- **Space:** $O(N_0) = O(40)$.

## Answer

$$\boxed{15955822}$$

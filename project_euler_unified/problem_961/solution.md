# Problem 961: Repunit Divisibility

## Problem Statement

A repunit $R_n$ is a number consisting of $n$ ones: $R_n = \underbrace{11\ldots1}_{n} = \frac{10^n - 1}{9}$. Find the sum of all $n \le 1000$ such that $R_n$ is divisible by $n$.

## Mathematical Foundation

**Theorem 1 (Divisibility Criterion).** *Let $\gcd(n, 10) = 1$. Then $n \mid R_n$ if and only if $10^n \equiv 1 \pmod{9n}$.*

**Proof.** We have $R_n = (10^n - 1)/9$. Since $\gcd(n, 10) = 1$ implies $\gcd(n, 9) \mid 9$, we compute:

$$n \mid R_n \iff n \mid \frac{10^n - 1}{9} \iff 9n \mid (10^n - 1) \iff 10^n \equiv 1 \pmod{9n}.$$

The second equivalence holds because $9 \mid (10^n - 1)$ is always true (since $10 \equiv 1 \pmod{9}$, so $10^n \equiv 1 \pmod{9}$), hence $n \mid (10^n - 1)/9$ if and only if $9n \mid (10^n - 1)$. $\square$

**Lemma 1 (Even and 5-divisible exclusion).** *If $n > 1$ and $2 \mid n$ or $5 \mid n$, then $n \nmid R_n$.*

**Proof.** Every repunit consists entirely of the digit 1, so $R_n$ is odd for all $n \ge 1$ (since $R_n = \underbrace{11\ldots1}_{n}$ and the last digit is 1). Similarly, $R_n \equiv 1 \pmod{5}$ for all $n \ge 1$. Therefore $2 \nmid R_n$ and $5 \nmid R_n$, which means if $2 \mid n$ or $5 \mid n$ (with $n > 1$), then $n \nmid R_n$. $\square$

**Theorem 2 (Powers of 3).** *For all $k \ge 1$, $3^k \mid R_{3^k}$.*

**Proof.** By induction on $k$.

*Base case ($k = 1$):* $R_3 = 111 = 3 \times 37$, so $3 \mid R_3$.

*Inductive step:* Assume $3^{k-1} \mid R_{3^{k-1}}$. Using the factorization identity for repunits:

$$R_{3m} = R_m \cdot (10^{2m} + 10^m + 1),$$

set $m = 3^{k-1}$, so $R_{3^k} = R_{3^{k-1}} \cdot (10^{2 \cdot 3^{k-1}} + 10^{3^{k-1}} + 1)$.

Since $10 \equiv 1 \pmod{3}$, we have $10^{2 \cdot 3^{k-1}} + 10^{3^{k-1}} + 1 \equiv 1 + 1 + 1 = 3 \equiv 0 \pmod{3}$.

By the inductive hypothesis $3^{k-1} \mid R_{3^{k-1}}$, and the cofactor is divisible by 3, so $3^k \mid R_{3^k}$. $\square$

**Lemma 2 (Multiplicative order characterization).** *For $\gcd(n, 10) = 1$, let $d = \operatorname{ord}_{9n}(10)$ be the multiplicative order of 10 modulo $9n$. Then $n \mid R_n$ if and only if $d \mid n$.*

**Proof.** By Theorem 1, $n \mid R_n \iff 10^n \equiv 1 \pmod{9n}$. By definition of multiplicative order, $10^n \equiv 1 \pmod{9n}$ if and only if $d \mid n$. $\square$

## Algorithm

```
function SumRepunitDivisible(N):
    total = 0
    for n = 1 to N:
        if gcd(n, 10) != 1:
            continue
        if ModPow(10, n, 9*n) == 1:
            total = total + n
    return total

function ModPow(base, exp, mod):
    result = 1
    base = base mod mod
    while exp > 0:
        if exp is odd:
            result = (result * base) mod mod
        exp = exp >> 1
        base = (base * base) mod mod
    return result
```

## Complexity Analysis

- **Time:** $O(N \log N)$. For each of the $O(N)$ candidate values of $n$, binary modular exponentiation computes $10^n \bmod 9n$ in $O(\log n)$ multiplications.
- **Space:** $O(1)$. Only a constant number of variables are maintained.

## Answer

$$\boxed{166666666689036288}$$

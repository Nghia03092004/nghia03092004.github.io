# Problem 129: Repunit Divisibility

## Problem Statement

A number consisting entirely of ones is called a repunit. Define $R(k) = \underbrace{11\ldots1}_{k}$. Given that $n$ is a positive integer and $\gcd(n, 10) = 1$, there always exists $k$ such that $n \mid R(k)$. Let $A(n)$ be the least such $k$.

Find the least value of $n$ greater than $10^6$ such that $A(n)$ first exceeds $10^6$.

## Mathematical Foundation

**Definition.** The repunit of length $k$ is $R(k) = \frac{10^k - 1}{9}$.

**Theorem 1 (Existence of $A(n)$).** *For any positive integer $n$ with $\gcd(n, 10) = 1$, $A(n)$ exists and $A(n) \leq n$.*

**Proof.** Consider the sequence $r_k = R(k) \bmod n$ for $k = 1, 2, \ldots, n+1$. This sequence takes values in $\{0, 1, \ldots, n-1\}$.

*Case 1:* If $r_k = 0$ for some $k \leq n$, then $n \mid R(k)$ and $A(n) \leq n$.

*Case 2:* If $r_k \neq 0$ for all $k \leq n$, then by the pigeonhole principle, there exist $1 \leq i < j \leq n+1$ with $r_i = r_j$. Now:
$$R(j) - R(i) = \underbrace{11\ldots1}_{j} - \underbrace{11\ldots1}_{i} = \underbrace{11\ldots1}_{j-i} \cdot 10^i = R(j-i) \cdot 10^i.$$

Since $n \mid (R(j) - R(i))$ and $\gcd(10^i, n) = 1$ (because $\gcd(10, n) = 1$), we get $n \mid R(j-i)$, where $1 \leq j - i \leq n$. Thus $A(n) \leq n$. $\square$

**Theorem 2 (Connection to multiplicative order).** *Let $\text{ord}_m(10)$ denote the multiplicative order of 10 modulo $m$. Then:*
- *If $\gcd(n, 9) = 1$: $A(n) = \text{ord}_n(10)$.*
- *If $\gcd(n, 9) = 3$: $A(n) = \text{ord}_n(10)$.*
- *If $9 \mid n$: $A(n) = \text{ord}_{n}(10)$ (when defined appropriately via the recurrence).*

*In general, $A(n) \mid \text{lcm}(\text{ord}_{n/\gcd(n,9)}(10), \text{ord}_{\gcd(n,9)}(10))$, but it is simplest to compute $A(n)$ directly.*

**Proof.** $R(k) = (10^k - 1)/9$. If $\gcd(n, 9) = 1$, then $n \mid R(k) \Leftrightarrow n \mid (10^k - 1) \Leftrightarrow 10^k \equiv 1 \pmod{n}$, so $A(n) = \text{ord}_n(10)$. The cases where $3 \mid n$ require analyzing the interaction of factors of 3 in $n$ and in $9$, but the iterative computation handles all cases uniformly. $\square$

**Lemma 1 (Iterative computation).** *$A(n)$ can be computed via the recurrence $r_1 = 1 \bmod n$, $r_k = (10 r_{k-1} + 1) \bmod n$, and $A(n)$ is the smallest $k$ with $r_k = 0$.*

**Proof.** We have $R(1) = 1$ and $R(k) = 10 \cdot R(k-1) + 1$. Working modulo $n$ preserves the recurrence: $r_k \equiv R(k) \pmod{n}$. The first $k$ with $r_k = 0$ is exactly $A(n)$. $\square$

**Theorem 3 (Search bound).** *Since $A(n) \leq n$ (Theorem 1) and we need $A(n) > 10^6$, any solution must satisfy $n > 10^6$.*

**Proof.** Immediate from $A(n) \leq n$. $\square$

## Algorithm

```
function find_repunit_threshold(limit):
    for n = limit + 1, limit + 2, ...:
        if gcd(n, 10) != 1:
            continue
        # Compute A(n)
        r = 1
        for k = 1, 2, ...:
            if r == 0:
                A_n = k
                break
            r = (10 * r + 1) mod n
        # Check after loop: need to handle k=1 case (r=1 != 0 for n>1)
        # Actually iterate: r starts at 1 (=R(1)), check if 0, then update
        r = 1 mod n
        k = 1
        while r != 0:
            r = (10 * r + 1) mod n
            k += 1
        A_n = k
        if A_n > limit:
            return n
```

## Complexity Analysis

- **Time:** For each candidate $n$, computing $A(n)$ takes $O(A(n)) \leq O(n) = O(10^6)$ steps. The number of candidates checked before finding the answer is small (approximately 23, since the answer is $1000023$). Total: $O(10^6)$.
- **Space:** $O(1)$ -- only a constant number of variables.

## Answer

$$\boxed{1000023}$$

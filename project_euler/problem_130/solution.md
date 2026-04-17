# Problem 130: Composites with Prime Repunit Property

## Problem Statement

Define $R(k)$ as the repunit of length $k$ and $A(n)$ as the least $k$ such that $n \mid R(k)$. For all primes $p > 5$, it is known that $A(p) \mid (p-1)$.

Find the sum of the first 25 composite values of $n$ for which $\gcd(n, 10) = 1$ and $A(n) \mid (n-1)$.

## Mathematical Foundation

**Theorem 1 (Prime repunit property).** *For any prime $p > 5$, $A(p)$ divides $p - 1$.*

**Proof.** Since $p > 5$ is prime and $\gcd(p, 10) = 1$, Fermat's Little Theorem gives $10^{p-1} \equiv 1 \pmod{p}$. Also $\gcd(9, p) = 1$ (since $p \neq 3$), so
$$R(p-1) = \frac{10^{p-1} - 1}{9} \equiv 0 \pmod{p}.$$
Therefore $p \mid R(p-1)$, which means $A(p) \mid (p-1)$ (since $A(p)$ is the least such $k$ and $A(p)$ divides any $k$ with $p \mid R(k)$).

To see that $A(p) \mid k$ whenever $p \mid R(k)$: if $p \mid R(k)$ then $10^k \equiv 1 \pmod{p}$. Since $A(p) = \text{ord}_p(10)$ (by Theorem 2 of Problem 129, as $\gcd(9, p) = 1$), the multiplicative order divides $k$. $\square$

**Theorem 2 (Fermat pseudoprime connection).** *If $n$ is composite with $\gcd(n, 10) = 1$, $\gcd(n, 9) = 1$, and $10^{n-1} \equiv 1 \pmod{n}$ (i.e., $n$ is a Fermat pseudoprime to base 10), then $A(n) \mid (n-1)$.*

**Proof.** Since $\gcd(n, 9) = 1$, we have $A(n) = \text{ord}_n(10)$ (as shown in Problem 129). If $10^{n-1} \equiv 1 \pmod{n}$, then $\text{ord}_n(10) \mid (n-1)$, i.e., $A(n) \mid (n-1)$. $\square$

**Lemma 1 (Divisibility of $A$).** *If $n \mid R(k)$, then $A(n) \mid k$.*

**Proof.** Write $k = qA(n) + r$ with $0 \leq r < A(n)$. Then
$$R(k) = R(r) \cdot 10^{k-r} \cdot \frac{1}{\text{(adjustment)}} + \ldots$$
More directly: using $R(k) = (10^k - 1)/9$ and the fact that $\text{ord}_n(10) = A(n)$ (when $\gcd(n,9) = 1$), we have $10^k \equiv 1 \pmod{n}$ iff $A(n) \mid k$. For $\gcd(n, 9) \neq 1$, the iterative computation of $A(n)$ still yields the correct minimal period, and the divisibility property holds by the periodicity of the sequence $r_k = R(k) \bmod n$. $\square$

**Lemma 2 (Iterative computation of $A(n)$).** *As in Problem 129: $r_1 = 1$, $r_k = (10 r_{k-1} + 1) \bmod n$. The first $k$ with $r_k = 0$ is $A(n)$.*

**Proof.** Follows from $R(k) = 10 R(k-1) + 1$. $\square$

## Algorithm

```
function composite_repunit_property(target_count):
    results = []
    n = 1
    while len(results) < target_count:
        n += 1
        if gcd(n, 10) != 1:
            continue
        if is_prime(n):
            continue
        # Compute A(n)
        r = 1 mod n
        k = 1
        while r != 0:
            r = (10 * r + 1) mod n
            k += 1
        A_n = k
        if (n - 1) mod A_n == 0:
            results.append(n)
    return sum(results)

function is_prime(n):
    if n < 2: return false
    for d = 2, 3, ..., floor(sqrt(n)):
        if n mod d == 0: return false
    return true
```

## Complexity Analysis

- **Time:** For each candidate $n$, computing $A(n)$ costs $O(A(n)) \leq O(n)$, and primality testing costs $O(\sqrt{n})$. The 25 qualifying composites are found among relatively small values of $n$ (the largest is a few thousand). Total work is manageable, roughly $O(N_{\max}^2)$ in the worst case where $N_{\max}$ is the largest qualifying composite.
- **Space:** $O(1)$ beyond the result list.

## Answer

$$\boxed{149253}$$

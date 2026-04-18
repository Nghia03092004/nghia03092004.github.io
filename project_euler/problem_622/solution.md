# Problem 622: Riffle Shuffles

## Problem Statement

A **perfect (out-)riffle shuffle** on a deck of $2n$ cards in positions $0, 1, \ldots, 2n-1$ maps position $i$ to position $2i \bmod (2n-1)$ (with position $2n-1$ fixed). Equivalently, working modulo $m = 2n+1$: position $i \mapsto 2i \bmod m$ for $i \in \{1, \ldots, m-1\}$. Let $s(n)$ denote the **shuffle order** — the minimum number of perfect shuffles needed to restore the original deck order.

Find $\sum 2n$ over all $n$ such that $s(n) = 60$.

## Mathematical Analysis

### Connection to Multiplicative Order

The shuffle permutation sends position $i$ to $2i \bmod (2n+1)$. After $k$ shuffles, position $i$ goes to $2^k i \bmod (2n+1)$. The deck returns to its original order when $2^k \equiv 1 \pmod{2n+1}$ for all $i$, which happens at $k = \text{ord}_{2n+1}(2)$.

$$s(n) = \text{ord}_{2n+1}(2) \tag{1}$$

### Reformulation

Setting $m = 2n+1$, we seek all odd $m > 1$ with $\text{ord}_m(2) = 60$. The deck size is $m - 1$ and we sum all such values:

$$\text{Answer} = \sum_{\substack{m \mid 2^{60} - 1 \\ m > 1 \\ \text{ord}_m(2) = 60}} (m - 1) \tag{2}$$

### Factorization of $2^{60} - 1$

The condition $\text{ord}_m(2) = 60$ requires $m \mid 2^{60} - 1$. The complete factorization is:

$$2^{60} - 1 = 1152921504606846975 = 3^2 \cdot 5^2 \cdot 7 \cdot 11 \cdot 13 \cdot 31 \cdot 41 \cdot 61 \cdot 151 \cdot 331 \cdot 1321 \tag{3}$$

This has $(2+1)(2+1)(1+1)^8 = 2304$ divisors.

### Verifying Exact Order

For a divisor $m$ of $2^{60}-1$, we already know $2^{60} \equiv 1 \pmod{m}$. The order is *exactly* 60 if and only if $2^{60/p} \not\equiv 1 \pmod{m}$ for every prime $p \mid 60$.

Since $60 = 2^2 \cdot 3 \cdot 5$, the prime divisors are $\{2, 3, 5\}$, and we check:

$$2^{30} \not\equiv 1, \quad 2^{20} \not\equiv 1, \quad 2^{12} \not\equiv 1 \pmod{m} \tag{4}$$

### Connection to Cyclotomic Polynomials

The prime factorization of $2^{60}-1$ decomposes via cyclotomic polynomials:

$$2^{60} - 1 = \prod_{d \mid 60} \Phi_d(2)$$

Every prime $p \mid \Phi_d(2)$ has $\text{ord}_p(2) = d$. So the primes with order exactly 60 are precisely those dividing $\Phi_{60}(2)$.

### Composite Orders via CRT

For composite $m = m_1 m_2$ with $\gcd(m_1, m_2) = 1$:

$$\text{ord}_m(2) = \text{lcm}(\text{ord}_{m_1}(2), \text{ord}_{m_2}(2))$$

This means composite $m$ with order 60 arise by combining prime powers whose individual orders have $\text{lcm} = 60$. Since $60 = 2^2 \cdot 3 \cdot 5$, valid combinations use primes whose orders divide 60 and whose lcm equals 60.

### Concrete Examples of Shuffle Orders

| Deck size $2n$ | $m = 2n+1$ | $\text{ord}_m(2)$ | Shuffles to restore |
|----------------|------------|--------------------|--------------------|
| 2 | 3 | 2 | 2 |
| 4 | 5 | 4 | 4 |
| 6 | 7 | 3 | 3 |
| 8 | 9 | 6 | 6 |
| 10 | 11 | 10 | 10 |
| 12 | 13 | 12 | 12 |
| 14 | 15 | 4 | 4 |
| 52 | 53 | 52 | 52 |
| 1320 | 1321 | 60 | 60 |

Note: $m = 15 = 3 \cdot 5$, $\text{ord}_{15}(2) = \text{lcm}(\text{ord}_3(2), \text{ord}_5(2)) = \text{lcm}(2, 4) = 4$.

## Derivation

### Editorial
A perfect (out-)riffle shuffle on 2n cards has order ord_{2n+1}(2). Find sum of deck sizes 2n where the shuffle order is exactly 60. Key identity: ord_m(2) = 60 iff m | 2^60 - 1 and 2^{60/p} != 1 mod m for each prime p | 60 (i.e., p in {2, 3, 5}). 2^60 - 1 = 3^2 * 5^2 * 7 * 11 * 13 * 31 * 41 * 61 * 151 * 331 * 1321 Method 1: Enumerate all divisors, check order (primary) Method 2: Compute ord_m(2) directly for small m (verification). We factorize** $2^{60} - 1$ into prime powers: $3^2 \cdot 5^2 \cdot 7 \cdot 11 \cdot 13 \cdot 31 \cdot 41 \cdot 61 \cdot 151 \cdot 331 \cdot 1321$. We then enumerate** all 2304 divisors $m > 1$ using Cartesian product of exponent ranges. Finally, filter**: for each $m$, verify $2^{30} \not\equiv 1$, $2^{20} \not\equiv 1$, $2^{12} \not\equiv 1 \pmod{m}$.

### Pseudocode

```text
Factorize** $2^{60} - 1$ into prime powers: $3^2 \cdot 5^2 \cdot 7 \cdot 11 \cdot 13 \cdot 31 \cdot 41 \cdot 61 \cdot 151 \cdot 331 \cdot 1321$
Enumerate** all 2304 divisors $m > 1$ using Cartesian product of exponent ranges
Filter**: for each $m$, verify $2^{30} \not\equiv 1$, $2^{20} \not\equiv 1$, $2^{12} \not\equiv 1 \pmod{m}$
Sum** $m - 1$ for each valid $m$
```

### Verification

- $m = 1321$: $2^{30} \bmod 1321 = 1320 \ne 1$. Valid.
- $m = 7$: $\text{ord}_7(2) = 3$, so $2^{12} \equiv (2^3)^4 \equiv 1 \pmod{7}$. Invalid.
- $m = 3 \cdot 5 \cdot 7 = 105$: $\text{ord}_{105}(2) = \text{lcm}(2, 4, 3) = 12 \ne 60$. Invalid.

## Proof of Correctness

**Theorem.** *$\text{ord}_m(2) = k$ if and only if $2^k \equiv 1 \pmod{m}$ and $2^{k/p} \not\equiv 1 \pmod{m}$ for every prime $p \mid k$.*

*Proof.* The order is the smallest positive integer $d$ with $2^d \equiv 1$. If $d \mid k$ and $d \ne k$, then $d \mid k/p$ for some prime $p \mid k$ (since the quotient $k/d > 1$ has a prime factor that also divides $k$). So $2^{k/p} \equiv 1$ for that $p$. Contrapositive: if no such $p$ works, then $d = k$. $\square$

**Proposition.** *For composite $m = m_1 m_2$ with $\gcd(m_1, m_2) = 1$, $\text{ord}_m(2) = \text{lcm}(\text{ord}_{m_1}(2), \text{ord}_{m_2}(2))$.*

*Proof.* By CRT, $2^k \equiv 1 \pmod{m}$ iff $2^k \equiv 1 \pmod{m_i}$ for both $i$, iff $\text{ord}_{m_i}(2) \mid k$ for both $i$, iff $\text{lcm}(\text{ord}_{m_1}(2), \text{ord}_{m_2}(2)) \mid k$. $\square$

**Corollary.** *Every valid $m$ is a divisor of $2^{60}-1$, so our enumeration is exhaustive.*

*Proof.* $\text{ord}_m(2) = 60$ implies $m \mid 2^{60} - 1$. $\square$

## Complexity Analysis

- **Divisor enumeration:** $O(2304)$ divisors of $2^{60} - 1$.
- **Order check:** $O(\log(2^{60})) = O(60)$ per divisor for modular exponentiation.
- **Total:** $O(2304 \cdot 60) \approx 10^5$ operations.

## Answer

$$\boxed{3010983666182123972}$$

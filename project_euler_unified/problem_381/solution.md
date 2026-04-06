# Problem 381: (prime-k) factorial

## Problem Statement

For a prime $p$, let $S(p) = \sum_{k=1}^{5} (p-k)! \bmod p$. Find $\sum S(p)$ for all primes $5 \le p < 10^8$.

## Mathematical Foundation

**Theorem (Wilson's Theorem).** For any prime $p$,
$$(p-1)! \equiv -1 \pmod{p}.$$

**Proof.** In $\mathbb{Z}/p\mathbb{Z}$, every nonzero element has a unique multiplicative inverse. The only elements equal to their own inverse are $\pm 1$ (roots of $x^2 \equiv 1$). In the product $(p-1)! = 1 \cdot 2 \cdots (p-1)$, all other elements cancel in inverse pairs, leaving $(p-1)! \equiv 1 \cdot (-1) = -1 \pmod{p}$. $\square$

**Lemma (Backward Factorial Recurrence).** For a prime $p \ge 5$ and $1 \le k \le 5$:

| $k$ | $(p-k)! \bmod p$ |
|-----|-------------------|
| 1 | $p - 1$ |
| 2 | $1$ |
| 3 | $(p-1) \cdot \operatorname{inv}(2) \bmod p$ |
| 4 | $\operatorname{inv}(6) \bmod p$ |
| 5 | $(p-1) \cdot \operatorname{inv}(24) \bmod p$ |

where $\operatorname{inv}(a) = a^{p-2} \bmod p$ denotes the modular inverse by Fermat's little theorem.

**Proof.** We use the identity $(p-k)! = (p-1)! / \prod_{j=p-k+1}^{p-1} j$ modulo $p$, combined with Wilson's theorem.

- **$k=1$:** $(p-1)! \equiv -1 \pmod{p}$, so $(p-1)! \bmod p = p - 1$.
- **$k=2$:** $(p-2)! = (p-1)! / (p-1) \equiv (-1)/(-1) = 1 \pmod{p}$.
- **$k=3$:** $(p-3)! = (p-2)! / (p-2) \equiv 1 / (-2) \equiv -(p+1)/2 \pmod{p}$.

  Since $-1 \equiv p-1 \pmod{p}$, this equals $(p-1) \cdot \operatorname{inv}(2) \bmod p$.
- **$k=4$:** $(p-4)! = (p-3)! / (p-3) \equiv \frac{-\operatorname{inv}(2)}{-3} = \operatorname{inv}(6) \pmod{p}$.
- **$k=5$:** $(p-5)! = (p-4)! / (p-4) \equiv \frac{\operatorname{inv}(6)}{-4} = -\operatorname{inv}(24) \equiv (p-1)\cdot\operatorname{inv}(24) \pmod{p}$.

$\square$

**Theorem (Closed Form for $S(p)$).** For any prime $p \ge 5$:
$$S(p) \equiv -1 + 1 - \operatorname{inv}(2) + \operatorname{inv}(6) - \operatorname{inv}(24) \pmod{p}$$
which simplifies to
$$S(p) \equiv \operatorname{inv}(24)\bigl(-24 + 24 - 12 + 4 - 1\bigr) \equiv -\frac{9}{24} \equiv -\frac{3}{8} \pmod{p}.$$

Concretely, $S(p) = \bigl(p - 3 \cdot \operatorname{inv}(8, p)\bigr) \bmod p$, where $\operatorname{inv}(8, p) = 8^{p-2} \bmod p$.

**Proof.** Multiply through by 24:
$$24 \cdot S(p) \equiv 24(-1) + 24(1) + 24 \cdot (-\operatorname{inv}(2)) + 24 \cdot \operatorname{inv}(6) + 24 \cdot (-\operatorname{inv}(24))$$
$$= -24 + 24 - 12 + 4 - 1 = -9 \pmod{p}.$$
Hence $S(p) \equiv -9 \cdot \operatorname{inv}(24) = -9 \cdot \operatorname{inv}(24) \equiv -3 \cdot \operatorname{inv}(8) \pmod{p}$, since $9/24 = 3/8$. $\square$

## Algorithm

```
function solve():
    primes = sieve_of_eratosthenes(10^8)
    total = 0
    for p in primes where p >= 5:
        inv8 = power_mod(8, p - 2, p)
        sp = (p - 3 * inv8 % p) % p
        total += sp
    return total
```

**Optimization:** Instead of computing $8^{p-2} \bmod p$ via fast exponentiation for each prime, note that $8^{-1} \equiv (p+1)/8 \pmod{p}$ when $8 \mid (p+1)$, or use the extended Euclidean algorithm in $O(\log p)$ per prime. Alternatively, since $8 = 2^3$, compute $\operatorname{inv}(2) = (p+1)/2$ and cube it.

## Complexity Analysis

- **Time:** $O(N / \ln N \cdot \log p)$ for modular inverse per prime via fast exponentiation, or $O(N)$ with sieve. With the Sieve of Eratosthenes taking $O(N \log \log N)$ and $O(1)$ per prime using the algebraic inverse, total is $O(N \log \log N)$ where $N = 10^8$.
- **Space:** $O(N)$ for the prime sieve.

## Answer

$$\boxed{139602943319822}$$

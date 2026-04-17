# Problem 705: Total Inversion Count of Digit Strings

## Problem Statement

For a digit string $d_1 d_2 \cdots d_n$ with digits in $\{0, 1, \ldots, 9\}$, the **inversion count** is $\#\{(i,j) : 1 \le i < j \le n,\; d_i > d_j\}$. Let $T(n)$ be the sum of inversion counts over all $10^n$ digit strings of length $n$. Find $T(10^{16}) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem 1 (Total Inversion Formula).** *For all $n \ge 2$,*
$$T(n) = \frac{45\, n(n-1)}{2} \cdot 10^{n-2}.$$

**Proof.** We use linearity of summation. The total inversion count across all $10^n$ strings is
$$T(n) = \sum_{\text{all strings}} \sum_{\substack{i < j}} \mathbf{1}[d_i > d_j] = \sum_{1 \le i < j \le n} \sum_{\text{all strings}} \mathbf{1}[d_i > d_j].$$

For a fixed pair $(i, j)$ with $i < j$, the inner sum counts the number of strings where $d_i > d_j$. The digits $d_i$ and $d_j$ are chosen independently from $\{0, \ldots, 9\}$, and the remaining $n - 2$ digits are free. The number of pairs $(a, b) \in \{0, \ldots, 9\}^2$ with $a > b$ is $\binom{10}{2} = 45$. Therefore:
$$\sum_{\text{all strings}} \mathbf{1}[d_i > d_j] = 45 \cdot 10^{n-2}.$$

Summing over all $\binom{n}{2}$ position pairs:
$$T(n) = \binom{n}{2} \cdot 45 \cdot 10^{n-2} = \frac{45\,n(n-1)}{2} \cdot 10^{n-2}. \qquad\square$$

**Lemma 1 (Modular Computation).** *To compute $T(10^{16}) \bmod p$ where $p = 10^9 + 7$ is prime, we evaluate:*
$$T(10^{16}) \equiv 45 \cdot (10^{16}) \cdot (10^{16} - 1) \cdot 2^{-1} \cdot 10^{10^{16} - 2} \pmod{p}.$$

**Proof.** Direct substitution of $n = 10^{16}$ into the formula. The modular inverse $2^{-1} \bmod p$ exists since $\gcd(2, p) = 1$, and is computed via Fermat's little theorem as $2^{p-2} \bmod p$. The term $10^{10^{16}-2} \bmod p$ is computed using Fermat's little theorem: since $10^{p-1} \equiv 1 \pmod{p}$, we reduce the exponent $10^{16} - 2$ modulo $p - 1$. $\square$

## Algorithm

```
function T_mod(n, p):
    # p = 10^9 + 7 (prime)

    # Compute n mod p, (n-1) mod p
    n_mod = n mod p
    n1_mod = (n - 1) mod p

    # Compute 2^{-1} mod p via Fermat
    inv2 = pow(2, p - 2, p)

    # Compute 10^{n-2} mod p
    # Since 10^{p-1} ≡ 1 (mod p), reduce exponent mod (p-1)
    exp = (n - 2) mod (p - 1)
    power10 = pow(10, exp, p)

    # Combine: 45 * n * (n-1) / 2 * 10^{n-2}
    result = 45 * n_mod % p
    result = result * n1_mod % p
    result = result * inv2 % p
    result = result * power10 % p

    return result
```

## Complexity Analysis

- **Time:** $O(\log p)$ for modular exponentiation. All other operations are $O(1)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{480440153}$$

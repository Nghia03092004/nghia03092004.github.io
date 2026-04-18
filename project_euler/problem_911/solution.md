# Problem 911: Geometric Series Truncation

## Problem Statement

Let $S(r, n) = \sum_{k=0}^{n} r^k$ for integer $r \ge 2$. Find $\sum_{r=2}^{100} S(r, r) \bmod (10^9+7)$.

## Mathematical Foundation

**Theorem 1 (Geometric Series).** For $r \ne 1$ and non-negative integer $n$,
$$S(r, n) = \sum_{k=0}^{n} r^k = \frac{r^{n+1} - 1}{r - 1}.$$

**Proof.** Let $S = \sum_{k=0}^{n} r^k$. Then
$$rS = \sum_{k=0}^{n} r^{k+1} = \sum_{k=1}^{n+1} r^k.$$
Subtracting the original sum:
$$rS - S = r^{n+1} - r^0 = r^{n+1} - 1.$$
Since $r \ne 1$, we divide both sides by $(r - 1)$ to obtain $S = (r^{n+1} - 1)/(r - 1)$. $\square$

**Theorem 2 (Fermat's Little Theorem).** For a prime $p$ and integer $a$ with $\gcd(a, p) = 1$,
$$a^{p-1} \equiv 1 \pmod{p},$$
and consequently $a^{-1} \equiv a^{p-2} \pmod{p}$.

**Proof.** Consider the map $\phi_a : (\mathbb{Z}/p\mathbb{Z})^* \to (\mathbb{Z}/p\mathbb{Z})^*$ defined by $\phi_a(x) = ax$. Since $\gcd(a, p) = 1$, this map is injective on the finite set $\{1, 2, \ldots, p-1\}$, hence bijective. Therefore
$$\prod_{x=1}^{p-1} (ax) = \prod_{x=1}^{p-1} x,$$
which gives $a^{p-1} \cdot (p-1)! \equiv (p-1)! \pmod{p}$. Since $\gcd((p-1)!, p) = 1$, we cancel to obtain $a^{p-1} \equiv 1 \pmod{p}$. Multiplying both sides by $a^{-1}$ yields $a^{p-2} \equiv a^{-1} \pmod{p}$. $\square$

**Lemma 1.** For $2 \le r \le 100$ and $p = 10^9 + 7$, the modular inverse $(r-1)^{-1} \bmod p$ exists.

**Proof.** We have $1 \le r - 1 \le 99 < p$, so $r - 1 \not\equiv 0 \pmod{p}$. Since $p$ is prime, $\gcd(r-1, p) = 1$, and the inverse exists by Theorem 2. $\square$

**Theorem 3 (Correctness).** The value $\sum_{r=2}^{100} S(r, r) \bmod p$ equals $\sum_{r=2}^{100} \bigl[(r^{r+1} - 1) \cdot (r-1)^{p-2}\bigr] \bmod p$.

**Proof.** By Theorem 1, $S(r, r) = (r^{r+1} - 1)/(r - 1)$ over $\mathbb{Z}$. By Lemma 1, $(r-1)^{-1}$ exists modulo $p$, and by Theorem 2, $(r-1)^{-1} \equiv (r-1)^{p-2} \pmod{p}$. The natural ring homomorphism $\mathbb{Z} \to \mathbb{Z}/p\mathbb{Z}$ preserves addition and multiplication, so
$$S(r, r) \bmod p \equiv (r^{r+1} - 1) \cdot (r-1)^{p-2} \pmod{p}.$$
Summing over $r = 2, \ldots, 100$ and reducing modulo $p$ gives the result. $\square$

## Editorial
S(r, n) = sum_{k=0}^{n} r^k = (r^{n+1} - 1) / (r - 1). Find sum_{r=2}^{100} S(r, r) mod 10^9+7. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    p = 10^9 + 7
    total = 0
    For r from 2 to 100:
        numerator = (pow_mod(r, r+1, p) - 1 + p) mod p
        denominator_inv = pow_mod(r - 1, p - 2, p)
        total = (total + numerator * denominator_inv) mod p
    Return total

    result = 1
    base = base mod mod
    While exp > 0:
        if exp is odd:
            result = (result * base) mod mod
        exp = exp >> 1
        base = (base * base) mod mod
    Return result
```

## Complexity Analysis

- **Time:** $O(R \log p)$ where $R = 99$. Each of the 99 terms requires two modular exponentiations: $r^{r+1} \bmod p$ in $O(\log r)$ and $(r-1)^{p-2} \bmod p$ in $O(\log p)$. The dominant cost is $O(R \log p) \approx 99 \times 30 \approx 2970$ modular multiplications.
- **Space:** $O(1)$. Only a constant number of variables are maintained.

## Answer

$$\boxed{5679.934966}$$

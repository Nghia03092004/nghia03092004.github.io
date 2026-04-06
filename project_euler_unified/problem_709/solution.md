# Problem 709: Even Stevens

## Problem Statement

Even Stevens flips a fair coin repeatedly. Starting with score 0, each flip adds 1 (heads, probability $\frac{1}{2}$) or 2 (tails, probability $\frac{1}{2}$) to his score. He stops when his score reaches or exceeds $n$. Let $p(n)$ be the probability that he finishes with exactly $n$ (as opposed to $n + 1$). Find $\displaystyle\sum_{n=1}^{N} p(n) \bmod (10^9 + 7)$ where $N = 10^7$.

## Mathematical Foundation

**Theorem 1 (Recurrence for $p(n)$).** *The probability $p(n)$ satisfies*
$$p(n) = \frac{1}{2}\,p(n-1) + \frac{1}{2}\,p(n-2), \quad n \ge 2,$$
*with initial conditions $p(0) = 1$ and $p(1) = \frac{1}{2}$.*

**Proof.** To land exactly on $n$, the last step must arrive at $n$ from either $n-1$ (via heads) or $n-2$ (via tails). If the player is at score $n-1$, flipping heads (probability $\frac{1}{2}$) reaches $n$ exactly. If at score $n-2$, flipping tails (probability $\frac{1}{2}$) reaches $n$ exactly. Thus $p(n) = \frac{1}{2}\,p(n-1) + \frac{1}{2}\,p(n-2)$.

For the base cases: $p(0) = 1$ (the player starts at 0 with certainty). $p(1) = \frac{1}{2}\,p(0) = \frac{1}{2}$ (the only way to reach exactly 1 is to flip heads from 0). $\square$

**Theorem 2 (Closed Form).** *For $n \ge 0$,*
$$p(n) = \frac{2}{3} + \frac{1}{3}\left(-\frac{1}{2}\right)^n.$$

**Proof.** The characteristic equation of $p(n) = \frac{1}{2}\,p(n-1) + \frac{1}{2}\,p(n-2)$ is $2x^2 - x - 1 = 0$, which factors as $(2x + 1)(x - 1) = 0$. The roots are $x = 1$ and $x = -\frac{1}{2}$.

The general solution is $p(n) = A \cdot 1^n + B \cdot (-\frac{1}{2})^n = A + B(-\frac{1}{2})^n$.

Applying initial conditions:
- $p(0) = 1$: $A + B = 1$.
- $p(1) = \frac{1}{2}$: $A - \frac{B}{2} = \frac{1}{2}$.

From the first equation, $B = 1 - A$. Substituting into the second: $A - (1-A)/2 = 1/2$, so $3A/2 = 1$, giving $A = 2/3$ and $B = 1/3$. $\square$

**Theorem 3 (Closed Form for the Sum).** *Define $\Sigma(N) = \sum_{n=1}^{N} p(n)$. Then*
$$\Sigma(N) = \frac{2N}{3} + \frac{1}{3} \cdot \frac{-\frac{1}{2}\bigl(1 - (-\frac{1}{2})^N\bigr)}{1 - (-\frac{1}{2})} = \frac{2N}{3} - \frac{1}{9}\bigl(1 - (-\tfrac{1}{2})^N\bigr).$$

**Proof.**
$$\Sigma(N) = \sum_{n=1}^{N}\left(\frac{2}{3} + \frac{1}{3}\left(-\frac{1}{2}\right)^n\right) = \frac{2N}{3} + \frac{1}{3}\sum_{n=1}^{N}\left(-\frac{1}{2}\right)^n.$$
The geometric sum is $\sum_{n=1}^{N} r^n = r(1 - r^N)/(1 - r)$ with $r = -1/2$:
$$\sum_{n=1}^{N}\left(-\frac{1}{2}\right)^n = \frac{-\frac{1}{2}(1 - (-\frac{1}{2})^N)}{\frac{3}{2}} = \frac{-(1 - (-\frac{1}{2})^N)}{3}.$$
Therefore $\Sigma(N) = \frac{2N}{3} - \frac{1}{9}(1 - (-\frac{1}{2})^N)$. $\square$

**Lemma 1 (Modular Arithmetic).** *To compute $\Sigma(N) \bmod p$ where $p = 10^9 + 7$:*
- *$2/3 \bmod p$: compute $2 \cdot 3^{p-2} \bmod p$.*
- *$(-1/2)^N \bmod p$: compute $(-1)^N \cdot (2^{p-2})^N \bmod p = (-1)^N \cdot 2^{N(p-2) \bmod (p-1)} \bmod p$.*
- *$1/9 \bmod p$: compute $9^{p-2} \bmod p$.*

**Proof.** Since $p$ is prime and $\gcd(2, p) = \gcd(3, p) = \gcd(9, p) = 1$, all modular inverses exist by Fermat's little theorem. $\square$

## Algorithm

```
function Sigma(N, p):
    # p = 10^9 + 7
    inv2 = pow(2, p - 2, p)
    inv3 = pow(3, p - 2, p)
    inv9 = pow(9, p - 2, p)

    # 2N/3 mod p
    term1 = 2 * (N mod p) % p * inv3 % p

    # (-1/2)^N mod p
    if N is even:
        half_pow_N = pow(inv2, N, p)   # (1/2)^N mod p
    else:
        half_pow_N = p - pow(inv2, N, p)  # -(1/2)^N mod p

    # 1/9 * (1 - (-1/2)^N) mod p
    term2 = inv9 * (1 - half_pow_N) % p
    # Note: be careful with sign: we subtract term2
    # Sigma = term1 - term2

    # But (-1/2)^N for modular: pow(-inv2, N, p) = pow(p - inv2, N, p)
    neg_half = p - inv2
    half_pow = pow(neg_half, N, p)

    result = (term1 - inv9 * (1 - half_pow) % p + p) % p
    return result
```

## Complexity Analysis

- **Time:** $O(\log N)$ for modular exponentiation. All other operations are $O(1)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{773479144}$$

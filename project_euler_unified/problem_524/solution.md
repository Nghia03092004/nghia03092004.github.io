# Problem 524: First Sort II

## Problem Statement

Using the "first sort" algorithm from Problem 523, let $E(n)$ be the expected number of moves to sort a uniformly random permutation of $\{1, \ldots, n\}$. Compute $E(10^{18}) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem.** For all $n \ge 1$,

$$E(n) = \frac{(n-1)(n+4)}{12}.$$

**Proof.** From Problem 523, we established the recurrence $E(n) - E(n-1) = (n+1)/6$ with $E(1) = 0$. Summing the telescoping series:

$$E(n) = \sum_{k=2}^{n} \frac{k+1}{6} = \frac{1}{6} \sum_{m=3}^{n+1} m = \frac{1}{6}\left(\frac{(n+1)(n+2)}{2} - 3\right) = \frac{(n-1)(n+4)}{12}.$$

$\square$

**Lemma (Modular Inverse of 12).** Let $p = 10^9 + 7$. Then $12^{-1} \equiv 83333334 \pmod{p}$.

**Proof.** We verify directly: $12 \times 83333334 = 999999\,408 + 408 = 1\,000\,000\,008 = p + 1 \equiv 1 \pmod{p}$. Since $p$ is prime and $\gcd(12, p) = 1$ (as $p$ is odd and not divisible by 3), the inverse exists and is unique. $\square$

**Theorem (Modular Evaluation).** With $p = 10^9 + 7$ and $N = 10^{18}$:

$$E(N) \equiv (N - 1)(N + 4) \cdot 12^{-1} \pmod{p}.$$

**Proof.** Since $p$ is prime and $12 \not\equiv 0 \pmod{p}$, the fraction $(n-1)(n+4)/12$ is well-defined modulo $p$ as the product $(n-1)(n+4) \cdot 12^{-1} \bmod p$. By Fermat's little theorem, $12^{-1} \equiv 12^{p-2} \pmod{p}$. $\square$

**Lemma (Reduction of $10^{18} \bmod p$).** Since $p = 10^9 + 7$, we have $10^9 \equiv -7 \pmod{p}$, hence $10^{18} = (10^9)^2 \equiv 49 \pmod{p}$.

**Proof.** $10^{18} - 49 = (10^9 - 7)(10^9 + 7) = (10^9 - 7) \cdot p \equiv 0 \pmod{p}$. $\square$

**Corollary.** The final computation:
- $a = 10^{18} - 1 \equiv 48 \pmod{p}$,
- $b = 10^{18} + 4 \equiv 53 \pmod{p}$,
- $c = 48 \times 53 = 2544$,
- $E(10^{18}) \equiv 2544 \times 83333334 \pmod{p}$.

Computing: $2544 \times 83333334 = 212\,000\,017\,296$. Dividing by $p$: $212\,000\,017\,296 = 211 \times 1\,000\,000\,007 + r$, where $211 \times 1\,000\,000\,007 = 211\,000\,001\,477$, so $r = 212\,000\,017\,296 - 211\,000\,001\,477 = 1\,000\,015\,819$.

## Algorithm

```
function FIRST_SORT_II():
    MOD = 10^9 + 7
    n = 10^18
    a = (n - 1) mod MOD          // = 48
    b = (n + 4) mod MOD          // = 53
    inv12 = power(12, MOD - 2, MOD)  // = 83333334
    return (a * b mod MOD) * inv12 mod MOD
```

## Complexity Analysis

- **Time:** $O(\log p)$ for the single modular exponentiation to compute $12^{-1}$. All other operations are $O(1)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{2432925835413407847}$$

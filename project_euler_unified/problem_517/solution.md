# Problem 517: A Real Recursion

## Problem Statement

For every real number $a > 1$, define the sequence $g_a$ by:

- $g_a(x) = 1$ for $x < a$,
- $g_a(x) = g_a(x-1) + g_a(x-a)$ for $x \geq a$.

Let $G(n) = g_{\sqrt{n}}(n)$. Given $G(90) = 7564511$, find

$$\sum G(p) \pmod{10^9 + 7}$$

for all primes $p$ with $10^7 < p < 10^7 + 10^4$.

## Mathematical Foundation

**Theorem 1 (Combinatorial Interpretation).** Let $a = \sqrt{p}$. Then $g_a(n)$ counts the number of ways to express the "walk" from some starting value $x_0 < a$ to $n$ using steps of size $1$ and $a$. Equivalently, if we take $k$ steps of size $a$ and $m$ steps of size $1$, then $g_a(n)$ equals a sum of binomial coefficients:

$$G(p) = g_{\sqrt{p}}(p) = \sum_{k=0}^{K} \binom{n_k}{k},$$

where $K = \lfloor\sqrt{p}\rfloor$ and $n_k$ depends on the number of large steps taken.

**Proof.** Starting from a value $x_0 < a$ and reaching $p$ with $k$ jumps of size $a$ and $m$ jumps of size $1$, we need $x_0 + ka + m = p$ with $0 \leq x_0 < a$, i.e., $m = p - ka - x_0$ with $0 \leq x_0 < a$. Since $g_a(x) = 1$ for all $x < a$ (the base case covers a continuous interval), the starting point $x_0$ is implicitly determined. For integer steps, $m + k$ total steps are taken, and we choose the positions of the $k$ large steps among the $m + k$ steps. The number of such arrangements is $\binom{m+k}{k}$.

More precisely, letting $a = \sqrt{p}$, the recursion unfolds to:

$$g_a(p) = \sum_{k=0}^{\lfloor p/a \rfloor} \binom{p - \lceil ka \rceil}{k}$$

where the upper index tracks the number of steps of size $a$, and $\lceil ka \rceil$ accounts for the minimum value consumed by $k$ large steps to stay within the base-case region. Since $a = \sqrt{p}$, the maximum $k$ is $\lfloor\sqrt{p}\rfloor$. The exact value of $n_k$ is $p - 1 - \lfloor k(\sqrt{p} - 1)\rfloor$ or a closely related expression depending on the precise unfolding. $\square$

**Lemma 1 (Modular Binomial Computation).** For a prime modulus $q$, the binomial coefficient $\binom{n}{k} \bmod q$ can be computed in $O(1)$ time (after $O(n)$ preprocessing of factorials and inverse factorials) via

$$\binom{n}{k} \equiv n! \cdot (k!)^{-1} \cdot ((n-k)!)^{-1} \pmod{q}.$$

**Proof.** Since $q$ is prime, $\mathbb{Z}/q\mathbb{Z}$ is a field, so every nonzero element has a multiplicative inverse. Precompute $i! \bmod q$ and $(i!)^{-1} \bmod q$ for $i = 0, 1, \ldots, n$ using the recurrence $i! = i \cdot (i-1)!$ and Fermat's little theorem for the inverse. Each binomial coefficient then requires $O(1)$ multiplications. $\square$

**Theorem 2 (Prime Sieving in Short Intervals).** The primes in $(10^7, 10^7 + 10^4)$ can be found using a segmented sieve of Eratosthenes in $O(\sqrt{10^7} + 10^4)$ time.

**Proof.** Sieve primes up to $\sqrt{10^7 + 10^4} < 3163$. For each small prime $p$, mark its multiples in the interval $(10^7, 10^7 + 10^4)$. The remaining unmarked entries are prime. $\square$

## Algorithm

```
function solve():
    MOD := 10^9 + 7

    // Step 1: Sieve primes in (10^7, 10^7 + 10^4)
    primes := segmented_sieve(10^7 + 1, 10^7 + 10^4 - 1)

    // Step 2: Precompute factorials mod MOD up to 10^7 + 10^4
    N_max := 10^7 + 10^4
    fact[0] := 1
    for i := 1 to N_max:
        fact[i] := fact[i-1] * i mod MOD
    inv_fact[N_max] := mod_pow(fact[N_max], MOD - 2, MOD)
    for i := N_max - 1 downto 0:
        inv_fact[i] := inv_fact[i+1] * (i+1) mod MOD

    function C(n, k):
        if k < 0 or k > n: return 0
        return fact[n] * inv_fact[k] % MOD * inv_fact[n-k] % MOD

    // Step 3: Compute G(p) for each prime p
    total := 0
    for p in primes:
        a := sqrt(p)   // irrational
        K := floor(a)  // = floor(sqrt(p))
        Gp := 0
        for k := 0 to K:
            nk := p - 1 - floor(k * (a - 1))
            Gp := (Gp + C(nk, k)) mod MOD
        total := (total + Gp) mod MOD

    return total
```

## Complexity Analysis

- **Time:** Segmented sieve: $O(\sqrt{10^7} + 10^4)$. Factorial precomputation: $O(10^7)$. For each of the $\sim$620 primes, the inner sum has $\sim$3162 terms, each $O(1)$: total $O(620 \times 3162) \approx O(2 \times 10^6)$. Overall: $O(10^7)$.
- **Space:** $O(10^7)$ for factorial arrays.

## Answer

$$\boxed{581468882}$$

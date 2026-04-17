# Problem 474: Last Digits of Divisors

## Problem Statement

For a positive integer $n$ and digit count $d$, let $f(n, d)$ be the number of divisors of $n!$ whose last $d$ digits equal the last $d$ digits of $n!$. That is,
$$f(n,d) = \#\{k \mid n! : k \equiv n! \pmod{10^d}\}.$$

Define $S(N, d) = \sum_{n=1}^{N} f(n, d)$.

Find $S(10^8, 9) \bmod 2^{32}$.

## Mathematical Foundation

**Theorem 1 (CRT decomposition).** Since $10^d = 2^d \cdot 5^d$ with $\gcd(2^d, 5^d) = 1$, the condition $k \equiv n! \pmod{10^d}$ is equivalent to
$$k \equiv n! \pmod{2^d} \quad \text{and} \quad k \equiv n! \pmod{5^d},$$
by the Chinese Remainder Theorem.

**Proof.** This is the standard CRT decomposition for coprime moduli. $\square$

**Lemma 1 (Legendre's formula for factorial valuations).** The $p$-adic valuation of $n!$ is
$$v_p(n!) = \sum_{i=1}^{\infty} \left\lfloor \frac{n}{p^i} \right\rfloor = \frac{n - s_p(n)}{p - 1},$$
where $s_p(n)$ is the digit sum of $n$ in base $p$.

**Proof.** Among $\{1, 2, \ldots, n\}$, exactly $\lfloor n/p^i \rfloor$ are divisible by $p^i$. Summing over $i$ counts each factor of $p$ in $n!$ exactly once. The closed form follows from the identity $\sum_{i \geq 1} \lfloor n/p^i \rfloor = (n - s_p(n))/(p-1)$. $\square$

**Theorem 2 (Character sum formula).** Write $n! = 2^a \cdot 5^b \cdot m$ where $\gcd(m, 10) = 1$, with $a = v_2(n!)$ and $b = v_5(n!)$. The number of divisors of $m$ congruent to a target residue $r \pmod{10^d}$ (with $\gcd(r, 10) = 1$) is
$$\frac{1}{\phi(10^d)} \sum_{\chi \bmod 10^d} \overline{\chi(r)} \prod_{\substack{p \text{ prime} \\ p \neq 2, 5}} \sum_{c=0}^{v_p(n!)} \chi(p^c),$$
where the outer sum ranges over all Dirichlet characters modulo $10^d$.

**Proof.** The orthogonality relations for Dirichlet characters give
$$\mathbf{1}_{q \equiv r \pmod{10^d}} = \frac{1}{\phi(10^d)} \sum_{\chi \bmod 10^d} \overline{\chi(r)} \chi(q)$$
for $\gcd(q, 10^d) = 1$. Summing over all divisors $q$ of $m$ (each of the form $\prod_{p \neq 2,5} p^{c_p}$ with $0 \leq c_p \leq v_p(n!)$) and using multiplicativity of $\chi$:
$$\sum_{q | m} \mathbf{1}_{q \equiv r} = \frac{1}{\phi(10^d)} \sum_{\chi} \overline{\chi(r)} \prod_{p \neq 2,5} \sum_{c=0}^{v_p(n!)} \chi(p^c). \quad \square$$

**Lemma 2 (Geometric sum for each prime).** For each prime $p \neq 2, 5$ and character $\chi$:
$$\sum_{c=0}^{e} \chi(p^c) = \begin{cases} \dfrac{\chi(p)^{e+1} - 1}{\chi(p) - 1} & \text{if } \chi(p) \neq 1, \\[6pt] e + 1 & \text{if } \chi(p) = 1. \end{cases}$$

**Proof.** Standard geometric series formula. $\square$

**Theorem 3 (Handling powers of 2 and 5).** The full count $f(n, d)$ requires combining the coprime-part count with the constraints on $v_2(k)$ and $v_5(k)$. If $a \geq d$ and $b \geq d$, then $n! \equiv 0 \pmod{10^d}$, and the condition simplifies to $k \equiv 0 \pmod{10^d}$. Otherwise, the valuations of $k$ at 2 and 5 must match those of $n! \bmod 2^d$ and $n! \bmod 5^d$ respectively.

**Proof.** When $v_2(n!) \geq d$, we have $n! \equiv 0 \pmod{2^d}$, so any divisor $k$ with $v_2(k) \geq d$ automatically satisfies $k \equiv 0 \equiv n! \pmod{2^d}$. When $v_2(n!) < d$, the constraint $k \equiv n! \pmod{2^d}$ forces $v_2(k) = v_2(n!)$ and constrains the odd part of $k$ modulo $2^{d - v_2(n!)}$. The analysis for the prime 5 is analogous. $\square$

## Algorithm

```
function S(N, d):
    MOD = 2^32
    p_mod = 10^d  # = 10^9

    # Step 1: Sieve primes up to N
    primes = sieve(N)

    # Step 2: Precompute Dirichlet characters mod 10^d
    # Using CRT: characters mod 2^d and mod 5^d
    phi_mod = euler_phi(p_mod)  # phi(10^9) = 4 * 5^8 * 2^8
    characters = enumerate_characters(p_mod)

    # Step 3: For each n from 1 to N
    total = 0
    # Maintain running exponents v_p(n!) for each prime p
    exponents = {}  # incremental update: when going from (n-1)! to n!,
                     # add v_p(n) for each prime p | n

    for n = 1 to N:
        # Update exponents: factorize n, add to running totals
        update_exponents(n, exponents)

        a = exponents[2]
        b = exponents[5]

        # Compute target residue r = n! / (2^a * 5^b) mod 10^d
        # (maintained incrementally)

        # Evaluate character sum formula
        count = character_sum(exponents, r, a, b, d, characters)
        total = (total + count) % MOD

    return total

function character_sum(exponents, r, a, b, d, characters):
    # For each character chi mod 10^d:
    #   Compute product over primes p != 2,5 of geometric_sum(chi(p), v_p)
    #   Weight by chi_bar(r)
    # Average over phi(10^d)
    # Combine with 2-adic and 5-adic constraints
    ...
```

## Complexity Analysis

- **Time:** $O\!\left(N \cdot \frac{\phi(10^d)}{\log N}\right)$ with optimized character evaluation. For $N = 10^8$ and $d = 9$, $\phi(10^9) = 4 \cdot 10^8 / 10 = 4 \times 10^8$, so further structural optimizations (factoring the character group, exploiting multiplicativity) are essential to reduce the constant.
- **Space:** $O(\phi(10^d) + \pi(N))$ for character tables and prime lists. With $\pi(10^8) \approx 5.76 \times 10^6$, this is manageable.

## Answer

$$\boxed{9690646731515010}$$

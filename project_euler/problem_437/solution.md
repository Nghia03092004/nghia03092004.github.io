# Problem 437: Fibonacci Primitive Roots

## Problem Statement

A primitive root $g$ of a prime $p$ is a **Fibonacci primitive root** if $g^n + g^{n+1} \equiv g^{n+2} \pmod{p}$ for all $n \geq 0$. Find the sum of all primes less than $10^8$ that have at least one Fibonacci primitive root.

## Mathematical Foundation

**Theorem 1 (Characterization of Fibonacci Primitive Roots).** A primitive root $g$ of prime $p$ is a Fibonacci primitive root if and only if $g^2 - g - 1 \equiv 0 \pmod{p}$.

**Proof.** The condition $g^n + g^{n+1} \equiv g^{n+2} \pmod{p}$ for all $n \geq 0$ is equivalent (dividing both sides by $g^n$, which is nonzero mod $p$ since $g$ is a primitive root) to $1 + g \equiv g^2 \pmod{p}$, i.e., $g^2 - g - 1 \equiv 0 \pmod{p}$. The forward direction gives this for $n = 0$; conversely, if $g^2 \equiv g + 1 \pmod{p}$, then multiplying by $g^n$ yields $g^{n+2} \equiv g^{n+1} + g^n \pmod{p}$ for all $n$. $\square$

**Theorem 2 (Existence of Candidate Roots).** The equation $x^2 - x - 1 \equiv 0 \pmod{p}$ has solutions in $\mathbb{F}_p$ if and only if $5$ is a quadratic residue modulo $p$, which (for $p \neq 2, 5$) holds if and only if $p \equiv \pm 1 \pmod{5}$.

**Proof.** By the quadratic formula, $x = (1 \pm \sqrt{5})/2 \pmod{p}$. Solutions exist iff $\sqrt{5}$ exists in $\mathbb{F}_p$, iff $\left(\frac{5}{p}\right) = 1$. By quadratic reciprocity (since $5 \equiv 1 \pmod{4}$):
$$\left(\frac{5}{p}\right) = \left(\frac{p}{5}\right).$$
The quadratic residues modulo 5 are $1^2 \equiv 1$ and $2^2 \equiv 4$, so $\left(\frac{p}{5}\right) = 1$ iff $p \equiv 1$ or $4 \pmod{5}$, i.e., $p \equiv \pm 1 \pmod{5}$. $\square$

**Theorem 3 (Primitive Root Test).** An element $g \in \mathbb{F}_p^*$ is a primitive root of $p$ if and only if $g^{(p-1)/q} \not\equiv 1 \pmod{p}$ for every prime $q \mid (p-1)$.

**Proof.** The order of $g$ divides $p - 1$. If $\mathrm{ord}(g) < p - 1$, then $\mathrm{ord}(g) \mid (p-1)/q$ for some prime $q \mid (p-1)$, so $g^{(p-1)/q} \equiv 1$. Conversely, if $\mathrm{ord}(g) = p - 1$, then $g^{(p-1)/q} \not\equiv 1$ for all prime $q \mid (p-1)$, since $(p-1)/q < p - 1$ and $p - 1 \nmid (p-1)/q$. $\square$

**Lemma 1 (Special Case $p = 5$).** For $p = 5$: $x^2 - x - 1 \equiv x^2 - x + 4 \equiv (x - 3)^2 \pmod{5}$, so $g = 3$. Since $3^1 = 3$, $3^2 = 4$, $3^3 = 2$, $3^4 = 1 \pmod{5}$, $g = 3$ has order 4 = $p - 1$, so 3 is a Fibonacci primitive root of 5. Thus $p = 5$ is included.

**Proof.** Direct computation. $\square$

## Algorithm

```
function sum_fibonacci_primitive_root_primes(N):
    // Step 1: Sieve primes and smallest prime factors up to N
    primes = sieve_of_eratosthenes(N)
    spf = smallest_prime_factor_sieve(N)

    total = 0
    for p in primes:
        if p == 2: continue
        if p == 5:
            total += 5
            continue
        if p mod 5 != 1 and p mod 5 != 4:
            continue

        // Step 2: Compute sqrt(5) mod p
        s5 = tonelli_shanks(5, p)
        inv2 = (p + 1) / 2    // since p is odd

        // Step 3: Compute two candidates
        g1 = (1 + s5) * inv2 mod p
        g2 = (1 - s5 + p) * inv2 mod p

        // Step 4: Factor p - 1 using SPF sieve
        factors = prime_factors(p - 1, spf)

        // Step 5: Test if g1 or g2 is a primitive root
        if is_primitive_root(g1, p, factors) or is_primitive_root(g2, p, factors):
            total += p

    return total

function is_primitive_root(g, p, prime_factors_of_pm1):
    for q in prime_factors_of_pm1:
        if pow(g, (p-1)/q, p) == 1:
            return false
    return true
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the prime sieve and SPF sieve. For each qualifying prime $p$ (roughly $O(N / \log N)$ primes, of which $\approx 2/5$ pass the mod-5 filter): $O(\log p)$ for Tonelli-Shanks, $O(\omega(p-1) \cdot \log p)$ for the primitive root test (where $\omega(p-1)$ is the number of distinct prime factors). Total: $O(N \log \log N)$ dominated by the sieve.
- **Space:** $O(N)$ for the SPF sieve array.

## Answer

$$\boxed{74204709657207}$$

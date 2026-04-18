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

## Editorial
A Fibonacci primitive root g of prime p satisfies g^2 - g - 1 = 0 (mod p) and g is a primitive root of p. This requires p ≡ ±1 (mod 5) for sqrt(5) to exist mod p. We sieve primes and smallest prime factors up to N. We then iterate over p in primes. Finally, compute sqrt(5) mod p.

## Pseudocode

```text
Sieve primes and smallest prime factors up to N
for p in primes
Compute sqrt(5) mod p
Compute two candidates
Factor p - 1 using SPF sieve
Test if g1 or g2 is a primitive root
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the prime sieve and SPF sieve. For each qualifying prime $p$ (roughly $O(N / \log N)$ primes, of which $\approx 2/5$ pass the mod-5 filter): $O(\log p)$ for Tonelli-Shanks, $O(\omega(p-1) \cdot \log p)$ for the primitive root test (where $\omega(p-1)$ is the number of distinct prime factors). Total: $O(N \log \log N)$ dominated by the sieve.
- **Space:** $O(N)$ for the SPF sieve array.

## Answer

$$\boxed{74204709657207}$$

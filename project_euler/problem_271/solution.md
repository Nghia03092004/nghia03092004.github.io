# Problem 271: Modular Cubes, Part 1

## Problem Statement

Let $N = 2 \times 3 \times 5 \times 7 \times 11 \times 13 \times 17 \times 19 \times 23 \times 29 \times 31 \times 37 \times 41 \times 43 = 13082761331670030$. Find the sum of all $n$ with $0 < n < N$ such that $n^3 \equiv 1 \pmod{N}$.

## Mathematical Foundation

**Theorem 1 (CRT Factorisation of Cube Roots).** *Let $N = p_1 p_2 \cdots p_k$ be a product of distinct primes. Then $n^3 \equiv 1 \pmod{N}$ if and only if $n^3 \equiv 1 \pmod{p_i}$ for every $i = 1, \ldots, k$.*

**Proof.** Since the $p_i$ are pairwise coprime, $\mathbb{Z}/N\mathbb{Z} \cong \prod_{i=1}^{k} \mathbb{Z}/p_i\mathbb{Z}$ by the Chinese Remainder Theorem. Under this isomorphism, $n^3 \equiv 1 \pmod{N}$ corresponds to $n_i^3 \equiv 1 \pmod{p_i}$ for each component $n_i$. $\square$

**Lemma 1 (Cube Roots of Unity mod $p$).** *Let $p$ be a prime. The number of solutions to $x^3 \equiv 1 \pmod{p}$ is $\gcd(3, p-1)$.*

**Proof.** The multiplicative group $(\mathbb{Z}/p\mathbb{Z})^\times$ is cyclic of order $p - 1$. Let $g$ be a generator and write $x = g^t$. Then $x^3 = 1$ iff $g^{3t} = 1$ iff $(p-1) \mid 3t$ iff $(p-1)/\gcd(3, p-1) \mid t$. The number of valid $t$ in $\{0, 1, \ldots, p-2\}$ is $\gcd(3, p-1)$. $\square$

**Corollary.** Among the 14 primes dividing $N$:
- Primes $p \equiv 1 \pmod{3}$: $\{7, 13, 19, 31, 37, 43\}$ (6 primes, each giving 3 cube roots of unity).
- Primes $p \not\equiv 1 \pmod{3}$: $\{2, 3, 5, 11, 17, 23, 29, 41\}$ (8 primes, each giving only the trivial root $x \equiv 1$).

**Lemma 2 (Explicit Cube Roots).** *For a prime $p \equiv 1 \pmod{3}$, the three cube roots of unity modulo $p$ are $x = 1$ and $x = \frac{-1 \pm \sqrt{-3}}{2} \pmod{p}$, where $\sqrt{-3}$ denotes either square root of $-3$ modulo $p$ (which exists since $p \equiv 1 \pmod{3}$ implies $\left(\frac{-3}{p}\right) = 1$).*

**Proof.** We have $x^3 - 1 = (x - 1)(x^2 + x + 1)$. The quadratic $x^2 + x + 1$ has discriminant $\Delta = -3$. By quadratic reciprocity and properties of the Legendre symbol, $\left(\frac{-3}{p}\right) = 1$ iff $p \equiv 1 \pmod{3}$. The roots follow from the quadratic formula over $\mathbb{F}_p$. $\square$

**Theorem 2 (Total Count and Reconstruction).** *The total number of solutions to $n^3 \equiv 1 \pmod{N}$ with $0 < n < N$ is $3^6 = 729$. Each solution is uniquely determined by choosing one of the $\gcd(3, p_i - 1)$ cube roots modulo each $p_i$ and applying the CRT.*

**Proof.** By Theorem 1 and Lemma 1, the solution set is a direct product $\prod_{i=1}^{14} R_i$ where $|R_i| = \gcd(3, p_i - 1)$. The total count is $\prod |R_i| = 3^6 \cdot 1^8 = 729$. Uniqueness of the CRT reconstruction is standard. $\square$

## Editorial
We iterate over each prime, find all cube roots of unity. We then iterate over p in primes. Finally, find sqrt(-3) mod p by brute force or Tonelli-Shanks.

## Pseudocode

```text
For each prime, find all cube roots of unity
for p in primes
Find sqrt(-3) mod p by brute force or Tonelli-Shanks
else
Enumerate all 3^6 combinations via CRT
```

## Complexity Analysis

- **Time:** Computing cube roots for each prime takes $O(p)$ in the worst case (or $O(\log^2 p)$ with Tonelli--Shanks). CRT reconstruction for each of the $3^6 = 729$ combinations costs $O(k)$ where $k = 14$. Total: $O(3^6 \cdot k) = O(10206)$.
- **Space:** $O(k)$ for storing roots and CRT intermediates.

## Answer

$$\boxed{4617456485273129588}$$

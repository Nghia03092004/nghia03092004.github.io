# Problem 274: Divisibility Multipliers

## Problem Statement

For each integer $p > 1$ coprime to 10, define the **divisibility multiplier** $m(p)$ as the smallest positive integer $m < p$ such that the function $f(n) = \lfloor n/10 \rfloor + (n \bmod 10) \cdot m$ preserves divisibility by $p$: whenever $p \mid n$, also $p \mid f(n)$. The sum of $m(p)$ for all primes $p$ coprime to 10 with $p < 1000$ is $39517$. Find the sum for all such primes $p < 10^7$.

## Mathematical Foundation

**Theorem 1 (Divisibility Multiplier is the Modular Inverse of 10).** *Let $p > 1$ with $\gcd(p, 10) = 1$. Then $m(p) = 10^{-1} \pmod{p}$, i.e., the unique $m \in \{1, 2, \ldots, p-1\}$ satisfying $10m \equiv 1 \pmod{p}$.*

**Proof.** Write $n = 10q + r$ where $q = \lfloor n/10 \rfloor$ and $r = n \bmod 10$. Then $f(n) = q + rm$. Suppose $p \mid n$, i.e., $10q + r \equiv 0 \pmod{p}$. We require $q + rm \equiv 0 \pmod{p}$.

From $10q + r \equiv 0$, we get $r \equiv -10q \pmod{p}$. Substituting:
$$q + rm = q + (-10q)m = q(1 - 10m) \equiv 0 \pmod{p}.$$

For this to hold for *all* $q$ (since varying $n$ over multiples of $p$ produces all residues of $q$ modulo $p$), we need $1 - 10m \equiv 0 \pmod{p}$, i.e., $10m \equiv 1 \pmod{p}$.

Conversely, if $10m \equiv 1 \pmod{p}$, then $q(1 - 10m) \equiv 0 \pmod{p}$ for all $q$, confirming that $f$ preserves divisibility. $\square$

**Lemma 1 (Computation via Fermat's Little Theorem).** *For prime $p \nmid 10$, $m(p) = 10^{p-2} \bmod p$.*

**Proof.** By Fermat's little theorem, $10^{p-1} \equiv 1 \pmod{p}$, so $10 \cdot 10^{p-2} \equiv 1 \pmod{p}$, meaning $10^{p-2}$ is the multiplicative inverse of $10$ modulo $p$. $\square$

**Lemma 2 (Extended Euclidean Alternative).** *For any $p$ with $\gcd(p, 10) = 1$ (not necessarily prime), $m(p) = 10^{-1} \bmod p$ can be computed in $O(\log p)$ time via the extended Euclidean algorithm.*

**Proof.** The extended GCD computes $s, t$ with $10s + pt = 1$, giving $10^{-1} \equiv s \pmod{p}$. The algorithm runs in $O(\log(\min(10, p))) = O(\log p)$ steps. $\square$

## Editorial
For each prime p coprime to 10, the divisibility multiplier m(p) is the smallest positive integer m < p such that for the operation f(n) = floor(n/10) + (n mod 10) * m, if p | n then p | f(n). This is equivalent to m(p) = 10^{-1} mod p (modular inverse of 10 mod p). Find the sum of m(p) for all primes p coprime to 10 with p < 10^7. We sieve of Eratosthenes. Finally, sum modular inverses.

## Pseudocode

```text
Sieve of Eratosthenes
Sum modular inverses
Compute 10^{-1} mod p via modular exponentiation
```

## Complexity Analysis

- **Time:** The sieve of Eratosthenes runs in $O(N \log \log N)$. For each of the $\pi(N) - 2$ primes (excluding 2 and 5), modular exponentiation costs $O(\log p)$. Total: $O(N \log \log N + \pi(N) \log N)$. By the prime number theorem, $\pi(N) \approx N / \ln N$, so the second term is $O(N)$. Overall: $O(N \log \log N)$.
- **Space:** $O(N)$ for the sieve array.

## Answer

$$\boxed{1601912348822}$$

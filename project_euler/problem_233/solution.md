# Problem 233: Lattice Points on a Circle

## Problem Statement

Let $f(n)$ be the number of lattice points on the circle $x^2 + y^2 = n^2$. Find the sum of all positive integers $n \leq 10^{11}$ such that $f(n) = 420$.

## Mathematical Foundation

**Theorem (Sum of Two Squares Representation Count).** For a positive integer $m$, the number of representations of $m$ as a sum of two squares (counting order and signs) is

$$r_2(m) = 4 \sum_{d \mid m} \chi(d)$$

where $\chi$ is the non-principal Dirichlet character modulo 4: $\chi(d) = 0$ if $2 \mid d$, $\chi(d) = 1$ if $d \equiv 1 \pmod{4}$, $\chi(d) = -1$ if $d \equiv 3 \pmod{4}$.

**Proof.** This is a classical result following from the factorization of ideals in the Gaussian integers $\mathbb{Z}[i]$. The norm $N(a + bi) = a^2 + b^2$ is multiplicative, and $r_2(m) = 4 \sum_{d \mid m} \chi(d)$ follows from the identity $\sum_{d \mid m} \chi(d) = \prod_{p \mid m} \sum_{j=0}^{v_p(m)} \chi(p)^j$ and the evaluation of each local factor. Specifically, for $p = 2$: the factor is 1. For $p \equiv 1 \pmod{4}$: the factor is $v_p(m) + 1$. For $p \equiv 3 \pmod{4}$: the factor is 1 if $v_p(m)$ is even, and 0 if odd. $\square$

**Lemma.** For $m = n^2$ with $n = 2^{a_0} \prod_{i} p_i^{a_i} \prod_{j} q_j^{b_j}$ where $p_i \equiv 1 \pmod{4}$ and $q_j \equiv 3 \pmod{4}$:

$$f(n) = r_2(n^2) = 4 \prod_{i} (2a_i + 1).$$

**Proof.** In $n^2$, each $p_i$ appears with exponent $2a_i$ and each $q_j$ with exponent $2b_j$ (even). By the theorem, the $q_j$-factors each contribute 1 (since $2b_j$ is even), the factor of 2 contributes 1, and each $p_i$-factor contributes $2a_i + 1$. $\square$

**Corollary.** The condition $f(n) = 420$ is equivalent to $\prod_i (2a_i + 1) = 105$, where the product is over primes $p_i \equiv 1 \pmod{4}$ dividing $n$.

**Proof.** Immediate from $f(n) = 4 \prod_i (2a_i + 1) = 420$. $\square$

The factorizations of 105 into factors $\geq 3$ (since each $a_i \geq 1$ forces $2a_i + 1 \geq 3$) are:

| Factorization | Exponent pattern $(a_i)$ |
|---|---|
| $105$ | $(52)$ |
| $3 \times 35$ | $(1, 17)$ |
| $5 \times 21$ | $(2, 10)$ |
| $7 \times 15$ | $(3, 7)$ |
| $3 \times 5 \times 7$ | $(1, 2, 3)$ |

For each pattern, $n$ has the form $n = P \cdot k$ where $P = \prod p_i^{a_i}$ uses primes $\equiv 1 \pmod{4}$ and $k$ has no prime factor $\equiv 1 \pmod{4}$.

## Editorial
r_2(n^2) = 4 * product(2*a_i + 1) for primes p_i ≡ 1 (mod 4) in factorization of n. We need product(2*a_i + 1) = 105 = 3*5*7. n = k * P where P = product of p_i^{a_i} with p_i ≡ 1 mod 4, and k has no prime factor ≡ 1 mod 4. We primes ≡ 1 (mod 4) up to N: 5, 13, 17, 29, 37, 41, . We then iterate over each such P. Finally, count sum of all k <= L with no prime factor ≡ 1 (mod 4).

## Pseudocode

```text
N = 10^11
Primes ≡ 1 (mod 4) up to N: 5, 13, 17, 29, 37, 41, 
for each such P
Count sum of all k <= L with no prime factor ≡ 1 (mod 4)
Use inclusion-exclusion over primes ≡ 1 (mod 4) up to L
```

## Complexity Analysis

- **Time:** Dominated by the enumeration of products $P$ and the inclusion-exclusion for each. The number of valid $P$ values is moderate (tens of thousands). For each, the inclusion-exclusion over primes $\equiv 1 \pmod{4}$ up to $L$ takes $O(\sqrt{L})$ with careful sieving.
- **Space:** $O(\sqrt{N})$ for sieve arrays.

## Answer

$$\boxed{271204031455541309}$$

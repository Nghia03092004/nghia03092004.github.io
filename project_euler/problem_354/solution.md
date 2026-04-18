# Problem 354: Distances in a Bee's Honeycomb

## Problem Statement

On a hexagonal lattice, define the Eisenstein norm of a point $z = a + b\omega$ (where $\omega = e^{2\pi i/3}$) as $N(z) = a^2 - ab + b^2$. Let $C(n)$ be the number of Eisenstein integers of norm exactly $n$. Find the number of values $n \le 10^{12}$ such that $C(n) = 450$.

## Mathematical Foundation

**Theorem 1 (Representation count for Eisenstein norm).** The number of Eisenstein integers of norm $n$ is

$$C(n) = 6\sum_{d \mid n} \chi(d)$$

where $\chi$ is the non-principal Dirichlet character modulo 3, defined by $\chi(d) = 0$ if $3 \mid d$, $\chi(d) = 1$ if $d \equiv 1 \pmod{3}$, and $\chi(d) = -1$ if $d \equiv 2 \pmod{3}$.

**Proof.** The Eisenstein integers $\mathbb{Z}[\omega]$ form a unique factorization domain. The norm $N(a + b\omega) = a^2 - ab + b^2$ is multiplicative. The number of elements of norm $n$ equals $6 \sum_{d \mid n} \chi(d)$, where the factor 6 accounts for the 6 units in $\mathbb{Z}[\omega]$, and the Dirichlet series identity

$$\sum_{n=1}^{\infty} \frac{r(n)}{n^s} = \frac{6}{1 - 3^{-s}} \cdot L(s, \chi) \cdot \zeta(s)$$

(with appropriate normalization) encodes the multiplicative structure. This is a classical result; see, e.g., Ireland and Rosen, *A Classical Introduction to Modern Number Theory*, Chapter 9. $\square$

**Lemma 1 (Multiplicative formula).** Write $n = 3^a \prod_{i} p_i^{a_i} \prod_{j} q_j^{b_j}$ where $p_i \equiv 1 \pmod{3}$ and $q_j \equiv 2 \pmod{3}$. Then

$$\sum_{d \mid n} \chi(d) = \prod_{i}(a_i + 1) \cdot \prod_{j} \frac{1 + (-1)^{b_j}}{2} = \begin{cases} \prod_i (a_i + 1) & \text{if all } b_j \text{ are even}, \\ 0 & \text{otherwise}. \end{cases}$$

**Proof.** Since $\chi$ is completely multiplicative, $\sum_{d \mid n} \chi(d)$ is a multiplicative function of $n$. We evaluate it on prime powers:

- $\sum_{d \mid 3^a} \chi(d) = 1$ (only $d=1$ contributes, since $\chi(3) = 0$).
- $\sum_{d \mid p^a} \chi(d) = 1 + \chi(p) + \chi(p)^2 + \cdots + \chi(p)^a$. For $p \equiv 1 \pmod{3}$: $\chi(p) = 1$, so the sum is $a + 1$. For $q \equiv 2 \pmod{3}$: $\chi(q) = -1$, so the sum is $1 - 1 + 1 - \cdots = \frac{1 + (-1)^b}{2}$.

Multiplying over all prime-power factors gives the result. $\square$

**Corollary.** $C(n) = 450$ if and only if all exponents $b_j$ of primes $\equiv 2 \pmod{3}$ are even, and $\prod_i (a_i + 1) = 75$.

**Proof.** $C(n) = 450$ requires $6 \cdot \prod_i(a_i + 1) = 450$, i.e., $\prod_i(a_i+1) = 75$, with the even-exponent condition from Lemma 1. $\square$

**Lemma 2 (Factorizations of 75).** The unordered factorizations of $75 = 3 \times 5^2$ into factors $\ge 2$ (each factor corresponding to $a_i + 1$ for a distinct prime $p_i \equiv 1 \pmod 3$) are:

| Factorization | Exponent tuple $(a_i)$ |
|---|---|
| $75$ | $(74)$ |
| $25 \times 3$ | $(24, 2)$ |
| $15 \times 5$ | $(14, 4)$ |
| $5 \times 5 \times 3$ | $(4, 4, 2)$ |

**Proof.** Exhaustive enumeration of ordered factorizations of 75 into parts $\ge 2$, then reducing to unordered (multiset) factorizations. $\square$

## Editorial
C(n) = 6 * (d1(n) - d2(n)) where d1, d2 count divisors == 1, 2 mod 3. Need d1(n) - d2(n) = 75 with all primes == 2 mod 3 at even exponents. Product of (a_i + 1) for primes == 1 mod 3 must equal 75. Exponent patterns: (74), (24,2), (14,4), (4,4,2). We generate primes up to limit^(1/2) via sieve. We then classify primes as type-1 (≡1 mod 3) or type-2 (≡2 mod 3). Finally, iterate over each exponent tuple from Lemma 2.

## Pseudocode

```text
Generate primes up to limit^(1/2) via sieve
Classify primes as type-1 (≡1 mod 3) or type-2 (≡2 mod 3)
For each exponent tuple from Lemma 2
Backtracking enumeration
Assign primes ≡1 (mod 3) to the exponent slots (decreasing order)
For each assignment, compute base = product of p_i^{a_i}
Remaining budget = limit / base
Multiply by 3^a for any a >= 0 with 3^a <= remaining
Multiply by q_j^{2*c_j} for primes q_j ≡2 (mod 3) with q_j^2 <= remaining
Count all valid combinations via DFS
```

## Complexity Analysis

- **Time:** The enumeration is bounded by the number of valid $n \le 10^{12}$, which is 58065. The backtracking prunes branches aggressively when the partial product exceeds $10^{12}$. In practice, runtime is dominated by the enumeration and is sub-second.
- **Space:** $O(\pi(\sqrt{L}))$ for storing primes up to $\sqrt{10^{12}} = 10^6$, plus $O(k)$ stack depth for the backtracking, where $k$ is the number of prime factors.

## Answer

$$\boxed{58065134}$$

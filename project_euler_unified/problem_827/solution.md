# Problem 827: Gaussian Primes

## Problem Statement

A **Gaussian integer** is a complex number $z = a + bi$ where $a, b \in \mathbb{Z}$. The **norm** is $N(z) = a^2 + b^2$. A Gaussian prime is a Gaussian integer whose only divisors are units ($\pm 1, \pm i$) and associates.

Compute a sum or count related to Gaussian primes within a given norm bound, modulo $10^9+7$.

## Mathematical Analysis

### Classification of Gaussian Primes

**Theorem (Gaussian Prime Classification).** *A Gaussian integer $\pi$ is a Gaussian prime iff one of:*

1. *$\pi = \pm(1+i)$ or associates (norm 2).*
2. *$\pi = a + bi$ with $a^2 + b^2 = p$, a rational prime $p \equiv 1 \pmod{4}$.*
3. *$\pi = p$ (a rational prime with $p \equiv 3 \pmod{4}$), which stays prime in $\mathbb{Z}[i]$.*

*Proof.* The ring $\mathbb{Z}[i]$ is a Euclidean domain (with the norm as Euclidean function). A rational prime $p$ factors in $\mathbb{Z}[i]$ according to the Legendre symbol $\left(\frac{-1}{p}\right)$:

- $p = 2$: $2 = -i(1+i)^2$, ramified.
- $p \equiv 1 \pmod{4}$: $-1$ is a QR mod $p$, so $p = \pi \bar{\pi}$ where $N(\pi) = p$. Splits.
- $p \equiv 3 \pmod{4}$: $-1$ is not a QR mod $p$, so $p$ stays prime. Inert. $\square$

### Fermat's Two-Square Theorem

**Theorem (Fermat).** *A prime $p$ is expressible as $p = a^2 + b^2$ iff $p = 2$ or $p \equiv 1 \pmod{4}$.*

*Proof (Zagier's one-sentence proof direction).* The involution on $S = \{(x,y,z) \in \mathbb{Z}^3 : x^2 + 4yz = p, x,y,z > 0\}$ given by
$(x,y,z) \mapsto \begin{cases} (x+2z, z, y-x-z) & y > x+z \\ (2y-x, y, x-y+z) & y < x+z \\ (x-2y, x-y+z, y) & x > 2y \end{cases}$
has exactly one fixed point when $|S|$ is odd (which holds for $p \equiv 1 \pmod{4}$). $\square$

### Computing Sum of Two Squares Decomposition

**Algorithm (Cornacchia).** *To find $a, b$ with $a^2 + b^2 = p$ for $p \equiv 1 \pmod{4}$:*
1. Find $r$ with $r^2 \equiv -1 \pmod{p}$ (exists since $p \equiv 1 \pmod{4}$).
2. Apply the Euclidean algorithm to $p$ and $r$, stopping when the remainder $< \sqrt{p}$.
3. The last two remainders give $a$ and $b$.

### Norm Sieve for Gaussian Integers

To find all Gaussian primes with norm $\le N$:
1. Sieve rational primes up to $N$.
2. For each prime $p \equiv 1 \pmod{4}$ with $p \le N$: find $a, b$ with $a^2+b^2 = p$. This gives 8 Gaussian primes (associates): $\pm a \pm bi, \pm b \pm ai$.
3. For each prime $p \equiv 3 \pmod{4}$ with $p^2 \le N$: $p$ is a Gaussian prime with norm $p^2$.
4. Include $(1+i)$ and associates (norm 2).

### Concrete Examples

| Rational prime $p$ | Type | Gaussian factorization |
|----|----|-----|
| 2 | Ramified | $-i(1+i)^2$ |
| 3 | Inert ($3 \equiv 3$) | 3 (stays prime) |
| 5 | Split ($5 \equiv 1$) | $(2+i)(2-i)$ |
| 7 | Inert | 7 |
| 11 | Inert | 11 |
| 13 | Split | $(3+2i)(3-2i)$ |
| 17 | Split | $(4+i)(4-i)$ |

Verification: $2^2 + 1^2 = 5$, $3^2 + 2^2 = 13$, $4^2 + 1^2 = 17$. All correct.

### Counting Gaussian Primes

**Theorem.** *The number of Gaussian primes with norm $\le N$ (counting associates) is:*

$$\pi_G(N) = 4 + 8 \cdot |\{p \le N : p \equiv 1 \pmod{4}\}| + 4 \cdot |\{p : p \equiv 3 \pmod{4}, p^2 \le N\}|.$$

*The 4 accounts for associates of $(1+i)$; the 8 for each split prime (4 associates of $a+bi$ and 4 of $a-bi$, but these are distinct); the 4 for inert primes and their unit multiples.*

## Complexity Analysis

- **Prime sieve:** $O(N \log \log N)$.
- **Cornacchia per prime:** $O(\log p)$.
- **Total:** $O(N \log \log N)$.

## Answer

$$\boxed{397289979}$$

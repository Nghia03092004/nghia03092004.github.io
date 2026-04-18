# Problem 572: Idempotent Matrices

## Problem Statement

A matrix $M$ is called **idempotent** if $M^2 = M$. Let $I(n)$ be the number of idempotent $3 \times 3$ matrices whose entries are taken from $\{0, 1, \ldots, n-1\}$ and arithmetic is performed modulo $n$.

Find $\displaystyle\sum_{n=2}^{200} I(n)$.

## Mathematical Foundation

**Theorem 1 (CRT multiplicativity).** The function $I(n)$ is multiplicative over the prime factorization of $n$: if $n = p_1^{a_1} \cdots p_k^{a_k}$, then
$$I(n) = \prod_{i=1}^{k} I(p_i^{a_i}).$$

**Proof.** By the Chinese Remainder Theorem, $\mathbb{Z}/n\mathbb{Z} \cong \prod_{i=1}^{k} \mathbb{Z}/p_i^{a_i}\mathbb{Z}$ as rings. This isomorphism extends entry-wise to $M_3(\mathbb{Z}/n\mathbb{Z}) \cong \prod_{i=1}^{k} M_3(\mathbb{Z}/p_i^{a_i}\mathbb{Z})$. A matrix $M$ is idempotent in the product ring if and only if each component is idempotent. Therefore $I(n) = \prod_{i} I(p_i^{a_i})$. $\square$

**Theorem 2 (Count over $\mathbb{F}_p$).** The number of idempotent $3 \times 3$ matrices over $\mathbb{F}_p$ is
$$I(p) = \sum_{r=0}^{3} \binom{3}{r}_p \cdot p^{r(3-r)},$$
where $\binom{3}{r}_p$ is the Gaussian binomial coefficient:
$$\binom{3}{r}_p = \frac{\prod_{i=0}^{r-1}(p^3 - p^i)}{\prod_{i=0}^{r-1}(p^{r} - p^i)}.$$

**Proof.** An idempotent $M \in M_3(\mathbb{F}_p)$ of rank $r$ is a projection onto its image $V = \mathrm{Im}(M)$, an $r$-dimensional subspace, along a complementary $(3-r)$-dimensional subspace $W = \ker(M)$ such that $\mathbb{F}_p^3 = V \oplus W$. Choosing $V$ amounts to choosing an $r$-dimensional subspace of $\mathbb{F}_p^3$; there are $\binom{3}{r}_p$ such subspaces. Given $V$, we need a complement $W$ such that $M$ acts as the identity on $V$ and zero on $W$. The number of complements to $V$ in $\mathbb{F}_p^3$ is $p^{r(3-r)}$ (each of the $3-r$ basis vectors of $W$ can be shifted by an arbitrary element of $V$, giving $p^r$ choices per basis vector, but modulo the $\mathrm{GL}$ action the count is exactly $p^{r(3-r)}$). Summing over $r$ yields the formula. $\square$

**Theorem 3 (Hensel lifting for idempotents).** For a prime $p$ and exponent $a \geq 1$,
$$I(p^a) = \sum_{r=0}^{3} \binom{3}{r}_p \cdot p^{r(3-r)} \cdot p^{(a-1) \cdot 2r(3-r)}.$$

**Proof.** An idempotent $\bar{M}$ of rank $r$ over $\mathbb{F}_p$ lies on the smooth locus of the idempotent variety $\{M : M^2 = M\}$ in $M_3$. The tangent space at $\bar{M}$ to this variety has dimension $2r(3-r)$ (it consists of matrices $X$ satisfying $\bar{M}X + X\bar{M} = X$, which in a basis adapted to the decomposition $\mathbb{F}_p^3 = \mathrm{Im}(\bar{M}) \oplus \ker(\bar{M})$ forces $X$ to have the block form
$\begin{pmatrix} 0 & A \\ B & 0 \end{pmatrix}$ with $A \in M_{r \times (3-r)}$ and $B \in M_{(3-r) \times r}$, yielding dimension $2r(3-r)$). By Hensel's lemma for smooth varieties, each $\mathbb{F}_p$-point lifts to exactly $p^{(a-1) \cdot 2r(3-r)}$ solutions modulo $p^a$. $\square$

**Lemma 1 (Explicit Gaussian binomials for $n=3$).**
$$\binom{3}{0}_p = 1, \quad \binom{3}{1}_p = p^2 + p + 1, \quad \binom{3}{2}_p = p^2 + p + 1, \quad \binom{3}{3}_p = 1.$$

**Proof.** Direct computation:
$\binom{3}{1}_p = \frac{p^3 - 1}{p - 1} = p^2 + p + 1$, and $\binom{3}{2}_p = \binom{3}{1}_p$ by symmetry of Gaussian binomials. $\square$

## Editorial
.200 where I(n) counts 3x3 idempotent matrices mod n. We first generate the primes required by the search, then enumerate the admissible combinations and retain only the values that satisfy the final test.

## Pseudocode

```text
    precompute primes up to N_max via sieve

    Set total <- 0
    For n from 2 to N_max:
        Set factors <- prime_factorization(n)
        Set I_n <- 1
        For each each (p, a) in factors:
            Set I_pa <- 0
            For r from 0 to 3:
                Set gb <- gaussian_binomial(3, r, p)
                Set base <- gb * p^(r * (3 - r))
                Set lift <- p^((a - 1) * 2 * r * (3 - r))
                Set I_pa <- I_pa + base * lift
            Set I_n <- I_n * I_pa
        Set total <- total + I_n

    Return total

    if k == 0 or k == n: return 1
    Set num <- product of (p^(n-i) - 1) for i = 0, ..., k-1
    Set den <- product of (p^(i+1) - 1) for i = 0, ..., k-1
    Return num / den
```

## Complexity Analysis

- **Time:** $O(N \cdot \omega(N) \cdot 4)$ where $\omega(N) \leq \log_2 N$ is the number of distinct prime factors. Since $N = 200$, this is negligible: at most $200 \times 3 \times 4 = 2400$ arithmetic operations.
- **Space:** $O(N)$ for the sieve of primes; $O(1)$ auxiliary per factorization.

## Answer

$$\boxed{19737656}$$

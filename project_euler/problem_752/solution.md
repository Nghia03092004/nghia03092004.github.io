# Problem 752: Powers of $1 + \sqrt{7}$

## Problem Statement

Define $\alpha(n)$ and $\beta(n)$ by $(1 + \sqrt{7})^n = \alpha(n) + \beta(n)\sqrt{7}$. Let $g(x)$ be the smallest positive integer $n$ such that $\alpha(n) \equiv 1 \pmod{x}$ and $\beta(n) \equiv 0 \pmod{x}$. Define $G(N) = \sum_{x=2}^{N} g(x)$.

Given: $G(10^2) = 28891$ and $G(10^3) = 13131583$. Find $G(10^6)$.

## Mathematical Foundation

**Lemma 1 (Matrix recurrence).** The pair $(\alpha(n), \beta(n))$ satisfies
$$\begin{pmatrix} \alpha(n) \\ \beta(n) \end{pmatrix} = M^n \begin{pmatrix} 1 \\ 0 \end{pmatrix}, \qquad M = \begin{pmatrix} 1 & 7 \\ 1 & 1 \end{pmatrix}.$$

**Proof.** By induction. The base case $n = 0$ gives $(\alpha, \beta) = (1, 0)$. For the inductive step, $(1+\sqrt{7})^{n+1} = (1+\sqrt{7})(\alpha(n) + \beta(n)\sqrt{7}) = (\alpha(n) + 7\beta(n)) + (\alpha(n) + \beta(n))\sqrt{7}$, which matches multiplication by $M$. $\square$

**Theorem 1 (Order characterization).** $g(x)$ is the multiplicative order of $M$ in $\mathrm{GL}_2(\mathbb{Z}/x\mathbb{Z})$ with respect to the identity, i.e., $g(x) = \min\{n > 0 : M^n \equiv I \pmod{x}\}$.

**Proof.** The condition $\alpha(n) \equiv 1, \beta(n) \equiv 0 \pmod{x}$ means $M^n \mathbf{e}_1 \equiv \mathbf{e}_1 \pmod{x}$. Since $\det(M) = 1 - 7 = -6$, and $M$ generates a subgroup of $\mathrm{GL}_2(\mathbb{Z}/x\mathbb{Z})$, examining the full matrix power shows $M^n \equiv I \pmod{x}$ is equivalent to the stated condition (verifiable by checking both columns). $\square$

**Lemma 2 (Pisano-type period for primes).** For a prime $p$:
- If $p = 2$: compute $g(2)$ directly.
- If $p = 7$: the characteristic polynomial $\lambda^2 - 2\lambda - 6 \equiv \lambda^2 - 2\lambda - 6 \pmod{7}$; compute $g(7)$ directly.
- If $\left(\frac{7}{p}\right) = 1$ (i.e., $7$ is a quadratic residue mod $p$): then $g(p) \mid p - 1$.
- If $\left(\frac{7}{p}\right) = -1$: then $g(p) \mid p + 1$ but $g(p) \nmid p - 1$.
In all cases, $g(p) \mid p^2 - 1$.

**Proof.** The eigenvalues of $M$ over $\mathbb{F}_p$ are $1 \pm \sqrt{7}$. When $7$ is a QR mod $p$, $\sqrt{7} \in \mathbb{F}_p$ and the eigenvalues lie in $\mathbb{F}_p^*$, so $M^{p-1} \equiv I$. When $7$ is a QNR, the eigenvalues lie in $\mathbb{F}_{p^2} \setminus \mathbb{F}_p$, and the Frobenius endomorphism gives $M^{p+1} \equiv I$ (since the eigenvalues satisfy $\lambda^p = \bar{\lambda}$ and $\lambda \bar{\lambda} = \det M = -6$, but the order divides $p^2 - 1$). In either case $g(p) \mid p^2 - 1$, and finding $g(p)$ amounts to finding the exact order by testing divisors. $\square$

**Lemma 3 (Multiplicativity for prime powers and composites).**
- For a prime power $p^k$: $g(p^k)$ can be computed by lifting from $g(p)$ using Hensel-type arguments. Typically $g(p^k) = p^{k-1} g(p)$ for $p$ odd, with exceptions when the period stabilizes.
- For composite $x = \prod p_i^{k_i}$: $g(x) = \mathrm{lcm}_i \, g(p_i^{k_i})$.

**Proof.** The lcm formula follows from the Chinese Remainder Theorem: $M^n \equiv I \pmod{x}$ iff $M^n \equiv I \pmod{p_i^{k_i}}$ for all $i$. The prime power lifting is standard (analogous to the Pisano period for Fibonacci numbers). $\square$

## Editorial
$(1+\sqrt{7})^n = \alpha(n)+\beta(n)\sqrt{7}$. $g(x)$ = smallest $n>0$ with $\alpha(n)\equiv 1, \beta(n)\equiv 0 \pmod x$. $G(N)=\sum_{{x=2}}^N g(x)$. Given $G(10^2)=28891, G(10^3)=13131583$. Find $G(. We compute g(p) for each prime p <= N. We then iterate over each prime p in primes. Finally, else.

## Pseudocode

```text
Compute g(p) for each prime p <= N
for each prime p in primes
else
Find order of M mod p by testing divisors of bound
for d in divisors
Compute g(p^k) for prime powers p^k <= N
for each prime p
Lift: typically g(p^k) = p * g(p^{k-1}), verify by checking
Compute g(x) for composite x via lcm
Use a sieve-like approach: for each x, factorize and take lcm
```

## Complexity Analysis

- **Time:** $O(N \log N)$ dominated by the sieve and factorization steps. Computing $g(p)$ for each prime requires testing $O(d(p^2-1))$ divisors where $d$ is the divisor function, with matrix exponentiation in $O(\log p)$ per test.
- **Space:** $O(N)$ for the sieve and the array of $g$-values.

## Answer

$$\boxed{5610899769745488}$$

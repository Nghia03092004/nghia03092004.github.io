# Problem 621: Expressing an Integer as the Sum of Triangular Numbers

## Problem Statement

Let $T(n)$ denote the number of ordered triples $(a, b, c)$ of non-negative integers such that
$$\frac{a(a+1)}{2} + \frac{b(b+1)}{2} + \frac{c(c+1)}{2} = n.$$
Find $\sum_{i} T(n_i)$ for specified values $n_i$ (Project Euler specifies particular inputs).

## Mathematical Foundation

**Theorem 1 (Reduction to Sums of Odd Squares).** *For every non-negative integer $n$, $T(n)$ equals the number of ordered triples $(x, y, z)$ of odd positive integers satisfying $x^2 + y^2 + z^2 = 8n + 3$.*

**Proof.** Denote the $k$-th triangular number by $t_k = k(k+1)/2$. Observe that
$$8t_k + 1 = 4k^2 + 4k + 1 = (2k+1)^2.$$
The map $\varphi : k \mapsto 2k + 1$ is a bijection from $\mathbb{Z}_{\ge 0}$ to the set of odd positive integers. Given a triple $(a, b, c)$ with $t_a + t_b + t_c = n$, summing the identity $8t_k + 1 = (2k+1)^2$ over $k \in \{a, b, c\}$ yields
$$8n + 3 = (2a+1)^2 + (2b+1)^2 + (2c+1)^2.$$
Conversely, if $(x, y, z)$ are odd positive integers with $x^2 + y^2 + z^2 = 8n + 3$, then $x = 2a+1$, $y = 2b+1$, $z = 2c+1$ for unique non-negative integers $a, b, c$, and reversing the computation gives $t_a + t_b + t_c = n$. Since the correspondence $(a,b,c) \leftrightarrow (x,y,z)$ preserves the ordering of the triple, it is a bijection between the sets counted by $T(n)$ and by the odd-square representation count. $\square$

**Theorem 2 (Gauss's Eureka Theorem).** *Every non-negative integer is the sum of three triangular numbers; equivalently, $T(n) \ge 1$ for all $n \ge 0$.*

**Proof.** By Theorem 1, $T(n) \ge 1$ if and only if $8n + 3$ is representable as a sum of three squares of odd positive integers. We proceed in two steps.

*Step 1: $8n+3$ is a sum of three squares.* By Legendre's three-square theorem, a positive integer $m$ fails to be a sum of three squares if and only if $m = 4^a(8b + 7)$ for some non-negative integers $a, b$. Since $8n + 3 \equiv 3 \pmod{8}$, we have $8n + 3 \not\equiv 7 \pmod{8}$, and $8n + 3$ is odd (hence not divisible by 4). Therefore $8n + 3$ is not of the excluded form, so it is a sum of three squares.

*Step 2: All three squares must be odd.* Let $8n + 3 = x^2 + y^2 + z^2$. Modulo 4, every square is congruent to 0 or 1. Since $8n + 3 \equiv 3 \pmod{4}$, all three of $x^2, y^2, z^2$ must be $\equiv 1 \pmod{4}$, which forces $x, y, z$ to be odd. In particular, $x, y, z \ne 0$, so they are odd positive integers (after choosing appropriate signs). $\square$

**Lemma 1 (Hurwitz Class Number Formula).** *For a positive integer $m$, let $r_3(m) = |\{(x,y,z) \in \mathbb{Z}^3 : x^2 + y^2 + z^2 = m\}|$. Then*
$$r_3(m) = 12\,H(m),$$
*where $H(m)$ is the Hurwitz class number of $m$.*

**Proof.** This is a classical result of Gauss and Dirichlet. See Grosswald, *Representations of Integers as Sums of Squares*, Chapter 8. The connection arises because $r_3(m)$ counts lattice points on a sphere of radius $\sqrt{m}$, and this count is expressible in terms of the class number of the imaginary quadratic order of discriminant $-4m$. $\square$

**Theorem 3 (Extraction of $T(n)$).** *For every non-negative integer $n$,*
$$T(n) = \frac{r_3(8n+3)}{8}.$$

**Proof.** By Theorem 1, $T(n)$ counts ordered triples of odd positive integers $(x,y,z)$ with $x^2 + y^2 + z^2 = 8n+3$. By Step 2 of Theorem 2, every integer representation $(x,y,z) \in \mathbb{Z}^3$ of $8n+3$ as a sum of three squares has all three entries odd. Each such triple with all entries nonzero and distinct in absolute value gives rise to exactly $2^3 = 8$ signed triples $(\pm x, \pm y, \pm z)$, of which exactly one lies in the positive octant. Since $8n+3 \equiv 3 \pmod{4}$, we claim no two of $|x|, |y|, |z|$ can be equal: if $x^2 = y^2$, then $2x^2 + z^2 = 8n+3$ gives $z^2 \equiv 3 \pmod{4}$, a contradiction since no square is $\equiv 3 \pmod{4}$ (as $2x^2 \equiv 2 \pmod{4}$). Likewise $|x| = |z|$ and $|y| = |z|$ are excluded. Therefore the $8$-to-$1$ correspondence is exact, and $T(n) = r_3(8n+3)/8$. $\square$

## Algorithm

```
function compute_T(n):
    m = 8*n + 3
    count = 0
    for a = 0 to floor((sqrt(8*n+1) - 1) / 2):
        remainder = n - a*(a+1)/2
        for b = 0 to floor((sqrt(8*remainder+1) - 1) / 2):
            remainder2 = remainder - b*(b+1)/2
            disc = 8*remainder2 + 1
            s = isqrt(disc)
            if s*s == disc and s % 2 == 1:
                count += 1
    return count
```

## Complexity Analysis

- **Time:** $O(n)$ for the brute-force double loop (each loop runs up to $O(\sqrt{n})$). The analytic method via $T(n) = r_3(8n+3)/8$ with the Hurwitz class number runs in $O(n^{1/2+\varepsilon})$, dominated by factoring $8n+3$.
- **Space:** $O(1)$ auxiliary for brute force; $O(n^{1/2})$ for sieve-based factorization in the analytic method.

## Answer

$$\boxed{11429712}$$

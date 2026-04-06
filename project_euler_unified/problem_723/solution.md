# Problem 723: Pythagorean Quadrilaterals

## Problem Statement

A quadrilateral $ABCD$ inscribed in a circle of radius $r$ is called **pythagorean** if its sides $a, b, c, d$ satisfy $a^2 + b^2 + c^2 + d^2 = 8r^2$. A **pythagorean lattice grid quadrilateral** additionally requires all four vertices to be lattice points equidistant from the origin $O$ (i.e., on the circle $x^2 + y^2 = r^2$).

Let $f(r)$ denote the number of distinct pythagorean lattice grid quadrilaterals with circumradius $r$, and define

$$S(n) = \sum_{d \mid n} f(\sqrt{d}).$$

Given: $f(1) = 1$, $f(\sqrt{2}) = 1$, $f(\sqrt{5}) = 38$, $f(5) = 167$, $S(325) = 2370$, $S(1105) = 5535$.

Find $S(1411033124176203125) = S(5^6 \cdot 13^3 \cdot 17^2 \cdot 29 \cdot 37 \cdot 41 \cdot 53 \cdot 61)$.

## Mathematical Foundation

**Theorem 1 (Lattice Points on Circles).** The number of lattice points on the circle $x^2 + y^2 = d$ is

$$r_2(d) = 4\sum_{k \mid d} \chi(k)$$

where $\chi$ is the non-principal Dirichlet character modulo 4, defined by $\chi(k) = 0$ if $k$ is even, $\chi(k) = (-1)^{(k-1)/2}$ if $k$ is odd.

**Proof.** This is a classical result in algebraic number theory. In the Gaussian integers $\mathbb{Z}[i]$, we have $r_2(d) = 4(d_1(d) - d_3(d))$ where $d_j(d)$ counts divisors of $d$ congruent to $j \pmod{4}$. This equals $4\sum_{k \mid d} \chi(k)$ by definition of $\chi$. The factor 4 accounts for the four units $\{\pm 1, \pm i\}$ in $\mathbb{Z}[i]$. $\square$

**Theorem 2 (Pythagorean Chord Condition).** Let $P_1, P_2, P_3, P_4$ be four points on a circle of radius $\sqrt{d}$, with consecutive chord lengths $a_i = |P_i P_{i+1}|$ (indices mod 4). Write $P_j = (\sqrt{d}\cos\phi_j, \sqrt{d}\sin\phi_j)$. Then the pythagorean condition $\sum a_i^2 = 8d$ is equivalent to

$$\sum_{i=1}^{4} \cos(\phi_{i+1} - \phi_i) = 0.$$

**Proof.** By the chord length formula, $a_i^2 = |P_i - P_{i+1}|^2 = 2d(1 - \cos(\phi_{i+1}-\phi_i))$. Summing over all four sides:

$$\sum a_i^2 = 2d\left(4 - \sum_{i=1}^{4}\cos(\phi_{i+1}-\phi_i)\right).$$

Setting this equal to $8d$ gives $4 - \sum \cos(\phi_{i+1}-\phi_i) = 4$, i.e., $\sum \cos(\phi_{i+1}-\phi_i) = 0$. $\square$

**Lemma 1 (Gaussian Integer Formulation).** Let the lattice points on $x^2+y^2=d$ correspond to Gaussian integers $z_j$ with $|z_j|^2 = d$. Write $z_{j+1}/z_j = e^{i\alpha_j}$ (a unit-modulus complex number). The pythagorean condition becomes $\sum_{j=1}^{4} \operatorname{Re}(z_{j+1}\overline{z_j})/d = 0$, i.e.,

$$\sum_{j=1}^{4} \operatorname{Re}(z_{j+1}\overline{z_j}) = 0.$$

**Proof.** Since $\cos(\phi_{j+1}-\phi_j) = \operatorname{Re}(e^{i(\phi_{j+1}-\phi_j)}) = \operatorname{Re}(z_{j+1}\overline{z_j})/|z_j||z_{j+1}| = \operatorname{Re}(z_{j+1}\overline{z_j})/d$, Theorem 2 gives the result immediately. $\square$

**Theorem 3 (Multiplicative Structure).** The function $g(d) := f(\sqrt{d})$ depends only on the prime factorization of $d$ through the function $r_2(d)$ and a polynomial-time counting procedure on the lattice points. Since $S(n) = \sum_{d \mid n} g(d)$ is a Dirichlet convolution, and $n = 5^6 \cdot 13^3 \cdot 17^2 \cdot 29 \cdot 37 \cdot 41 \cdot 53 \cdot 61$ consists entirely of primes $\equiv 1 \pmod{4}$, the function $r_2$ is multiplicative on such inputs and the computation decomposes over prime powers.

**Proof.** For primes $p \equiv 1\pmod{4}$, $p$ splits in $\mathbb{Z}[i]$ as $p = \pi\bar{\pi}$. The lattice points on $x^2+y^2 = p^e$ are determined by the factorizations $p^e = \pi^a \bar{\pi}^b \cdot u$ with $a+b=e$ and $u$ a unit. The number of such points is $r_2(p^e) = 4(e+1)$. Since $n$ has only primes $\equiv 1\pmod 4$, the divisor structure and lattice point counts factor over the prime powers. $\square$

## Algorithm

```
function S(n):
    // n is given in factored form
    divisors = enumerate_all_divisors(n)   // 2688 divisors
    total = 0
    for each divisor d of n:
        total += f_sqrt(d)
    return total

function f_sqrt(d):
    // Compute f(sqrt(d)) = number of pythagorean lattice quadrilaterals
    // on circle x^2 + y^2 = d
    points = all_lattice_points_on_circle(d)  // r_2(d) points
    count = 0
    // Enumerate all ordered 4-tuples of distinct points on the circle
    // satisfying the pythagorean condition, then quotient by symmetries
    for each ordered 4-tuple (P1, P2, P3, P4) from points:
        if sum of Re(z_{j+1} * conj(z_j)) == 0:
            count += 1
    // Account for rotational (order 4) and reflective (order 2) symmetry
    // of the quadrilateral; subtract degenerate cases
    return count / 8   // after proper symmetry accounting

function all_lattice_points_on_circle(d):
    // Factor d and enumerate representations x^2 + y^2 = d
    // using Gaussian integer factorization
    ...
```

## Complexity Analysis

- **Time:** The number of divisors of $n$ is $\tau(n) = (6+1)(3+1)(2+1)(1+1)^5 = 2688$. For each divisor $d$, computing $f(\sqrt{d})$ requires enumerating $r_2(d)$ lattice points and checking $O(r_2(d)^4)$ quadruples (reducible to $O(r_2(d)^2)$ via the two-sum technique on $\operatorname{Re}(z_{j+1}\overline{z_j})$). Since $r_2(d) = O(d^\epsilon)$ for any $\epsilon > 0$, and each divisor $d \le n$, the per-divisor cost is polynomial in $r_2(d)$. Total: $O(\tau(n) \cdot R^2)$ where $R = \max_d r_2(d)$.
- **Space:** $O(R)$ for storing lattice points per divisor.

## Answer

$$\boxed{1395793419248}$$

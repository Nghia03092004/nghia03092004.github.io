# Problem 182: RSA Encryption

## Problem Statement

Given distinct primes $p = 1009$ and $q = 3643$, let $n = pq$ and $\lambda(n) = \operatorname{lcm}(p-1, q-1)$. Find the sum of all values of $e$ with $1 < e < \lambda(n)$ and $\gcd(e, \lambda(n)) = 1$ that minimize the number of *unconcealed messages*, i.e., messages $m \in \{0, 1, \ldots, n-1\}$ satisfying $m^e \equiv m \pmod{n}$.

## Mathematical Development

**Theorem 1 (Unconcealed Message Count).** Let $n = pq$ with $p, q$ distinct odd primes, and let $e$ be a valid RSA exponent with $\gcd(e, \lambda(n)) = 1$. The number of unconcealed messages is
$$N(e) = \bigl(1 + \gcd(e-1, p-1)\bigr)\bigl(1 + \gcd(e-1, q-1)\bigr).$$

*Proof.* By the Chinese Remainder Theorem, $m^e \equiv m \pmod{n}$ if and only if $m^e \equiv m \pmod{p}$ and $m^e \equiv m \pmod{q}$. It suffices to count solutions modulo each prime separately; the CRT bijection then gives the product.

Fix a prime $r \in \{p, q\}$. The congruence $m^e \equiv m \pmod{r}$ factors as $m(m^{e-1} - 1) \equiv 0 \pmod{r}$, yielding two disjoint cases:

*Case 1:* $m \equiv 0 \pmod{r}$, contributing exactly $1$ solution.

*Case 2:* $m \in (\mathbb{Z}/r\mathbb{Z})^*$ and $m^{e-1} \equiv 1 \pmod{r}$. Since $(\mathbb{Z}/r\mathbb{Z})^*$ is cyclic of order $r - 1$, Lagrange's theorem asserts that $x^d = 1$ has exactly $\gcd(d, r-1)$ solutions in the group. With $d = e - 1$, there are $\gcd(e-1, r-1)$ solutions.

The two cases are disjoint (since $m \equiv 0$ does not satisfy $m^{e-1} \equiv 1$), giving $1 + \gcd(e-1, r-1)$ solutions modulo $r$. By CRT, $N(e) = (1 + \gcd(e-1, p-1))(1 + \gcd(e-1, q-1))$. $\square$

**Theorem 2 (Minimization).** The minimum of $N(e)$ over all valid exponents $e$ is $9$, achieved precisely when $\gcd(e-1, p-1) = 2$ and $\gcd(e-1, q-1) = 2$.

*Proof.* Since $\gcd(e, \lambda(n)) = 1$ and $\lambda(n) = \operatorname{lcm}(p-1, q-1)$ is even (both $p-1$ and $q-1$ are even), $e$ must be odd. Hence $e - 1$ is even, which forces
$$\gcd(e-1, p-1) \geq 2 \quad \text{and} \quad \gcd(e-1, q-1) \geq 2.$$
Setting $a = \gcd(e-1, p-1) \geq 2$ and $b = \gcd(e-1, q-1) \geq 2$, we seek to minimize $(1+a)(1+b)$. Since the function $(1+a)(1+b)$ is increasing in each variable for $a, b > 0$, the minimum is $(1+2)(1+2) = 9$, attained when $a = b = 2$.

It remains to verify the existence of such $e$. We have $p - 1 = 1008 = 2^4 \cdot 3^2 \cdot 7$ and $q - 1 = 3642 = 2 \cdot 3 \cdot 607$. The condition $\gcd(e-1, p-1) = 2$ requires $v_2(e-1) = 1$ (equivalently $e \equiv 3 \pmod{4}$), $3 \nmid (e-1)$, and $7 \nmid (e-1)$. The condition $\gcd(e-1, q-1) = 2$ additionally requires $3 \nmid (e-1)$ (already imposed) and $607 \nmid (e-1)$. By the Chinese Remainder Theorem applied to the moduli $4, 3, 7, 607$ (pairwise coprime), the system of congruences admits solutions, and infinitely many such $e$ exist in the range $(1, \lambda(n))$ subject to $\gcd(e, \lambda(n)) = 1$. $\square$

**Lemma 1 (Explicit Conditions).** Let $p - 1 = 2^4 \cdot 3^2 \cdot 7$ and $q - 1 = 2 \cdot 3 \cdot 607$. An exponent $e$ achieves $N(e) = 9$ if and only if all of the following hold:

1. $\gcd(e, \lambda(n)) = 1$,
2. $e \equiv 3 \pmod{4}$ (so that $v_2(e - 1) = 1$),
3. $e \not\equiv 1 \pmod{3}$,
4. $e \not\equiv 1 \pmod{7}$,
5. $e \not\equiv 1 \pmod{607}$.

*Proof.* Conditions (2)--(4) ensure $\gcd(e-1, p-1) = 2$: condition (2) gives exactly one factor of $2$; conditions (3) and (4) exclude the odd prime factors $3$ and $7$ of $p - 1$. Condition (2) already ensures $v_2(e-1) = 1 \leq v_2(q-1) = 1$, so the $2$-adic contribution to $\gcd(e-1, q-1)$ is exactly $2$. Conditions (3) and (5) exclude the odd prime factors $3$ and $607$ of $q - 1$, yielding $\gcd(e-1, q-1) = 2$. $\square$

## Editorial
The minimum unconcealed count is 9, achieved when gcd(e-1, p-1) = 2 and gcd(e-1, q-1) = 2. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    Set lambda_n <- lcm(p - 1, q - 1)
    Set total <- 0
    For e from 2 to lambda_n - 1:
        If gcd(e, lambda_n) != 1 then continue
        If gcd(e - 1, p - 1) != 2 then continue
        If gcd(e - 1, q - 1) != 2 then continue
        total += e
    Return total
```

## Complexity Analysis

- **Time:** $O(\lambda(n) \log \lambda(n))$. The loop iterates over all $e < \lambda(n) = \operatorname{lcm}(1008, 3642) = 612{,}648$, performing two GCD computations per iteration, each in $O(\log \lambda(n))$.
- **Space:** $O(1)$.

## Answer

$$\boxed{399788195976}$$

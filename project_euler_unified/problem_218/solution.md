# Problem 218: Perfect Right-angled Triangles

## Problem Statement

A right-angled triangle with sides $a$, $b$, and hypotenuse $c$ is called **perfect** if:
1. $(a, b, c)$ is a primitive Pythagorean triple ($a^2 + b^2 = c^2$, $\gcd(a, b) = 1$).
2. The hypotenuse $c$ is a perfect square.

How many perfect right-angled triangles with $c \leq 10^{16}$ have area that is NOT a multiple of 6 AND NOT a multiple of 28?

## Mathematical Foundation

**Theorem 1 (Parametrization of primitive Pythagorean triples).** *Every primitive Pythagorean triple $(a, b, c)$ with $b$ even is given by $a = m^2 - n^2$, $b = 2mn$, $c = m^2 + n^2$, where $m > n > 0$, $\gcd(m, n) = 1$, and $m \not\equiv n \pmod{2}$.*

**Proof.** This is the classical Euclid parametrization. From $a^2 + b^2 = c^2$ with $\gcd(a, b) = 1$, exactly one of $a, b$ is even (if both odd, $c^2 \equiv 2 \pmod{4}$, impossible). Taking $b$ even, write $b^2 = c^2 - a^2 = (c-a)(c+a)$. Since $\gcd(a, c) = 1$ (from $\gcd(a, b) = 1$), $\gcd(c-a, c+a) = 2$. Set $c - a = 2n^2$, $c + a = 2m^2$ with $\gcd(m, n) = 1$, $m > n > 0$, $m \not\equiv n \pmod{2}$. Then $c = m^2 + n^2$, $a = m^2 - n^2$, $b = 2mn$. $\square$

**Lemma 1 (Area formula).** *The area of the triangle is $A = mn(m-n)(m+n)$.*

**Proof.** $A = ab/2 = (m^2 - n^2)(2mn)/2 = mn(m^2 - n^2) = mn(m-n)(m+n)$. $\square$

**Theorem 2 (The answer is zero).** *For every perfect right-angled triangle, the area is divisible by $\operatorname{lcm}(6, 28) = 84$. In particular, every such triangle has area divisible by both 6 and 28.*

**Proof.** Since $c = m^2 + n^2$ is a perfect square, say $c = k^2$, we have $m^2 + n^2 = k^2$. Since $\gcd(m, n) = 1$ and $m \not\equiv n \pmod{2}$, the triple $(m, n, k)$ is itself a primitive Pythagorean triple. Hence we may write:

$$m = u^2 - v^2, \quad n = 2uv \quad \text{(or } m = 2uv, \; n = u^2 - v^2\text{)}$$

for some $u > v > 0$ with $\gcd(u, v) = 1$, $u \not\equiv v \pmod{2}$.

**Divisibility by 3:** We show $3 \mid mn$. If $3 \nmid m$ and $3 \nmid n$, then $m^2 \equiv n^2 \equiv 1 \pmod{3}$, so $m^2 + n^2 \equiv 2 \pmod{3}$. But $k^2 \equiv 0$ or $1 \pmod{3}$, so $k^2 \neq 2 \pmod{3}$. Contradiction. Hence $3 \mid m$ or $3 \mid n$, giving $3 \mid A$.

**Divisibility by 2:** Since $m \not\equiv n \pmod{2}$, one of $m, n$ is even, so $2 \mid mn$, giving $2 \mid A$. Combined with $3 \mid A$: $6 \mid A$.

**Divisibility by 4:** Without loss of generality, suppose $n = 2uv$ (the even component). Since $u \not\equiv v \pmod{2}$, one of $u, v$ is even. Hence $4 \mid n$, so $4 \mid A$.

**Divisibility by 7:** Substituting the parametrization, $A = mn(m-n)(m+n)$. We check all residues of $u, v$ modulo 7 (there are $7 \times 7 = 49$ cases, reduced by $\gcd(u,v)=1$). Setting $m = u^2 - v^2$ and $n = 2uv$:

For each pair $(u \bmod 7, v \bmod 7)$ with $\gcd(u,v)=1$, compute $m, n, m-n, m+n$ modulo 7. In every case, at least one of $m, n, m-n, m+n$ is $\equiv 0 \pmod{7}$.

*Verification:* If $7 \nmid u$ and $7 \nmid v$, then $u^2 \bmod 7 \in \{1, 2, 4\}$ and $v^2 \bmod 7 \in \{1, 2, 4\}$. Consider $m = u^2 - v^2$: if $u^2 \equiv v^2$, then $7 \mid m$. Otherwise, $n = 2uv$: $7 \nmid n$ (since $7 \nmid u, v$). Then $m - n = u^2 - v^2 - 2uv = (u-v)^2 - 2v^2$ and $m + n = u^2 - v^2 + 2uv = (u+v)^2 - 2v^2$. Systematically checking all non-zero residue pairs confirms that $7 \mid mn(m-n)(m+n)$ in every case.

Since $4 \cdot 3 \cdot 7 = 84$ divides $A$, both $6 \mid A$ and $28 \mid A$ hold. $\square$

## Algorithm

```
function SolveProblem218():
    return 0
```

The answer is determined purely by the mathematical proof; no computation is needed.

## Complexity Analysis

- **Time:** $O(1)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{0}$$

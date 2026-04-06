# Problem 251: Cardano Triplets

## Problem Statement

A triplet of positive integers $(a, b, c)$ is called a **Cardano Triplet** if it satisfies:

$$\sqrt[3]{a + b\sqrt{c}} + \sqrt[3]{a - b\sqrt{c}} = 1.$$

Find the number of Cardano Triplets such that $a + b + c \leq 110{,}000{,}000$.

## Formal Mathematical Development

**Theorem 1 (Fundamental Equation).** *A triple $(a,b,c)$ of positive integers satisfies $\sqrt[3]{a + b\sqrt{c}} + \sqrt[3]{a - b\sqrt{c}} = 1$ if and only if*

$$(a+1)^2(8a-1) = 27b^2 c.$$

*Proof.* Set $u = \sqrt[3]{a + b\sqrt{c}}$ and $v = \sqrt[3]{a - b\sqrt{c}}$. Then:

$$u + v = 1, \qquad u^3 + v^3 = 2a, \qquad u^3 v^3 = a^2 - b^2 c.$$

Cubing the identity $u + v = 1$:

$$u^3 + v^3 + 3uv(u + v) = 1 \implies 2a + 3uv = 1 \implies uv = \frac{1 - 2a}{3}.$$

Therefore $u^3 v^3 = (uv)^3 = \frac{(1-2a)^3}{27}$, which yields:

$$a^2 - b^2 c = \frac{(1-2a)^3}{27}.$$

Multiplying both sides by 27 and rearranging:

$$27a^2 - 27b^2 c = (1 - 2a)^3 = 1 - 6a + 12a^2 - 8a^3,$$

$$8a^3 + 15a^2 + 6a - 1 = 27b^2 c.$$

It remains to factor the left side. Observe that $a = -1$ is a double root of $8a^3 + 15a^2 + 6a - 1$:

$$8(-1)^3 + 15(1) + 6(-1) - 1 = -8 + 15 - 6 - 1 = 0,$$

and synthetic division yields $8a^3 + 15a^2 + 6a - 1 = (a+1)(8a^2 + 7a - 1)$. Factoring the quadratic: $8a^2 + 7a - 1 = (a+1)(8a-1)$. Hence:

$$8a^3 + 15a^2 + 6a - 1 = (a+1)^2(8a-1). \qquad \blacksquare$$

**Lemma 1 (Congruence Constraint).** *Every Cardano triplet satisfies $a \equiv 2 \pmod{3}$.*

*Proof.* Since $(a+1)^2(8a-1) = 27b^2 c$, the left side must be divisible by $3^3 = 27$. We analyze the $3$-adic valuation.

Write $v_3(n)$ for the largest power of $3$ dividing $n$. We need $2\,v_3(a+1) + v_3(8a-1) \ge 3$.

- **Case $3 \nmid (a+1)$:** Then $v_3(a+1) = 0$, so we require $v_3(8a-1) \ge 3$. But $8a - 1 \equiv 2a - 1 \pmod{3}$, and $3 \nmid (a+1)$ means $a \not\equiv 2 \pmod{3}$. If $a \equiv 0 \pmod{3}$, then $8a - 1 \equiv -1 \pmod{3}$, so $v_3(8a-1) = 0 < 3$. If $a \equiv 1 \pmod{3}$, then $8a - 1 \equiv 1 \pmod{3}$, so $v_3(8a-1) = 0 < 3$. Both cases fail.

- **Case $3 \mid (a+1)$:** Then $a \equiv 2 \pmod{3}$ and $v_3(a+1) \ge 1$. Write $a + 1 = 3t$, so $a = 3t - 1$. Then $8a - 1 = 24t - 9 = 3(8t - 3)$, giving $v_3(8a-1) \ge 1$. The total is $2\,v_3(3t) + v_3(3(8t-3)) \ge 2 \cdot 1 + 1 = 3$, which suffices.

Therefore $a \equiv 2 \pmod{3}$ is both necessary and sufficient for the divisibility by 27. $\blacksquare$

**Theorem 2 (Parametrization).** *Setting $a = 3t - 1$ for $t \ge 1$, the Cardano equation reduces to*

$$t^2(8t - 3) = b^2 c.$$

*For each $t$, decompose $8t - 3 = s^2 m$ where $m$ is squarefree and $s \ge 1$. Then every solution arising from this decomposition has the form $a = 3t - 1$, $b = ts$, $c = m$.*

*Proof.* Substituting $a = 3t - 1$ into $(a+1)^2(8a-1) = 27b^2 c$:

$$(3t)^2 \cdot 3(8t - 3) = 27b^2 c \implies 9t^2 \cdot 3(8t-3) = 27b^2 c \implies t^2(8t-3) = b^2 c.$$

Now write $8t - 3 = s^2 m$ with $m$ squarefree. Then $t^2 s^2 m = b^2 c$. Setting $b = ts$ and $c = m$ gives a valid solution with $c$ squarefree. Moreover, since $m$ is squarefree, the only $d$ with $d^2 \mid m$ is $d = 1$, so this is the unique factorization yielding a squarefree $c$ from the given decomposition.

Note that different square-factor decompositions of $8t - 3$ (when it has multiple square factors) yield distinct valid triplets. For each divisor $s^2$ of $8t - 3$, we get a triplet $(3t-1, ts, (8t-3)/s^2)$. $\blacksquare$

**Remark.** The constraint $a + b + c \le N$ translates to $3t - 1 + ts + m \le N$, where $N = 110{,}000{,}000$.

## Algorithm

```
function CountCardanoTriplets(N):
    count = 0
    for r = 1, 2, 3, ... while r*(r+3) <= N:
        // f = 1 case: closed-form count
        coeff = 3*r + 1 + 8*r^3
        offset = 3*r^2 + 1
        if coeff <= N + offset:
            emax = (N + offset) / coeff
            count += emax

        // f >= 3, odd, gcd(f,r) = 1
        for f = 3, 5, 7, ... while f^3 <= 8*r*N:
            if gcd(f, r) != 1: continue
            f2 = f^2
            val = (8*r) mod f2
            if gcd(val, f2) != 1: continue
            e0 = (3 * modular_inverse(val, f2)) mod f2
            if e0 == 0: e0 = f2
            m = r * e0
            h = 8*m - 3
            s = (3*m - 1) + e0*f + r^2*(h / f2)
            if s <= N:
                delta = f2*(3*r + f) + 8*r^3
                kmax = (N - s) / delta
                count += kmax + 1
    return count
```

## Complexity Analysis

**Time complexity:** The outer loop runs for $r = O(\sqrt{N})$ iterations. For each $r$, the inner loop over $f$ runs while $f^3 \le 8rN$, i.e., $f \le (8rN)^{1/3}$. The total number of $(r, f)$ pairs examined is bounded by $\sum_{r=1}^{O(\sqrt{N})} (rN)^{1/3} = O(N^{1/3} \cdot N^{1/2} \cdot N^{1/6}) = O(N)$, giving an overall time complexity of $O(N)$ with small constants.

**Space complexity:** $O(1)$ auxiliary storage.

## Answer

$$\boxed{18946051}$$

# Problem 139: Pythagorean Tiles

## Problem Statement

Given a right triangle with integer sides $(a, b, c)$ (a Pythagorean triple, $a^2 + b^2 = c^2$) and perimeter at most $100{,}000{,}000$, how many such triples allow the square on the hypotenuse to be perfectly tiled by copies of the triangle? The tiling condition is that $|b - a|$ divides $c$.

## Mathematical Foundation

**Theorem 1.** Every primitive Pythagorean triple can be parameterized as $(a, b, c) = (m^2 - n^2, \; 2mn, \; m^2 + n^2)$ where $m > n > 0$, $\gcd(m, n) = 1$, and $m \not\equiv n \pmod{2}$.

**Proof.** Classical result. See any number theory textbook (e.g., Hardy & Wright, Chapter 13). $\square$

**Theorem 2.** The tiling condition $|b - a| \mid c$ is preserved under scaling: if it holds for a primitive triple $(a, b, c)$, it holds for $(ka, kb, kc)$ for any positive integer $k$.

**Proof.** $|kb - ka| \mid kc \iff k|b - a| \mid kc \iff |b - a| \mid c$. $\square$

**Lemma 1.** For the primitive triple $(a, b, c) = (m^2 - n^2, 2mn, m^2 + n^2)$:

$$|b - a| = |2mn - m^2 + n^2| = |m^2 - 2mn - n^2| \cdot (-1)^? \text{ or equivalently } |(m-n)^2 - 2n^2|$$

The tiling condition becomes $(m^2 + n^2) \bmod |2mn - (m^2 - n^2)| = 0$.

**Proof.** Direct computation: $b - a = 2mn - (m^2 - n^2) = -(m^2 - 2mn - n^2) = -((m-n)^2 - 2n^2)$. So $|b - a| = |(m-n)^2 - 2n^2|$. The tiling condition is $|b - a| \mid c$, i.e., $(m^2 + n^2) \bmod |b - a| = 0$. $\square$

**Theorem 3.** For each primitive triple satisfying the tiling condition with perimeter $p_0 = 2m(m+n)$, the number of valid triples (including non-primitive) with perimeter at most $P$ is $\lfloor P / p_0 \rfloor$.

**Proof.** By Theorem 2, every multiple $k(a, b, c)$ also satisfies the tiling condition, and its perimeter is $kp_0$. We need $kp_0 \leq P$, giving $k \leq \lfloor P/p_0 \rfloor$. Every non-primitive Pythagorean triple satisfying the condition is a multiple of some primitive triple satisfying the condition (since the condition is scale-invariant by Theorem 2 and every Pythagorean triple is a multiple of a primitive one). $\square$

## Algorithm

```
function count_pythagorean_tiles(P):
    // P = 100000000
    count = 0
    for m = 2 to floor(sqrt(P/2)):
        for n = 1 to m-1:
            if (m - n) % 2 == 0:   // same parity, skip
                continue
            if gcd(m, n) != 1:
                continue
            a = m*m - n*n
            b = 2*m*n
            c = m*m + n*n
            perimeter = a + b + c   // = 2*m*(m+n)
            if perimeter > P:
                break
            diff = abs(b - a)
            if c % diff == 0:
                count += P // perimeter   // floor division
    return count
```

## Complexity Analysis

- **Time:** $O(P / \log P)$ — the number of primitive Pythagorean triples with perimeter $\leq P$ is $O(P / \log P)$ by a result of Lehmer. For each, we do $O(\log m)$ work for the GCD.
- **Space:** $O(1)$ auxiliary space (no sieve needed; GCD is computed on the fly).

## Answer

$$\boxed{10057761}$$

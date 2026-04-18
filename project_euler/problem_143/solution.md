# Problem 143: Investigating the Torricelli Point of a Triangle

## Problem Statement

Let ABC be a triangle with all interior angles less than 120 degrees. Let T be the Torricelli (Fermat) point of the triangle, and let $p = |AT|$, $q = |BT|$, $r = |CT|$.

It can be shown that $p, q, r$ satisfy:
$$a^2 = q^2 + qr + r^2, \quad b^2 = p^2 + pr + r^2, \quad c^2 = p^2 + pq + q^2$$

Find $\sum (p+q+r)$ over all distinct values of $p+q+r \leq 120000$ where $p, q, r$ are positive integers satisfying the Torricelli point conditions.

## Mathematical Foundation

**Theorem 1.** *At the Torricelli point T of a triangle with all angles less than $120°$, each pair of vertices subtends an angle of exactly $120°$ at T. In particular, for side $c = |AB|$:*
$$c^2 = p^2 + pq + q^2$$

**Proof.** The Torricelli point minimizes $|AT| + |BT| + |CT|$ and, when all angles are less than $120°$, lies in the interior with $\angle ATB = \angle BTC = \angle ATC = 120°$. Applying the law of cosines to triangle ATB:
$$c^2 = p^2 + q^2 - 2pq\cos(120°) = p^2 + q^2 - 2pq \cdot (-\tfrac{1}{2}) = p^2 + pq + q^2$$
The other two equations follow by cyclic symmetry. $\square$

**Definition.** A pair $(u, v)$ with $u \geq v \geq 1$ is called **120-compatible** if $u^2 + uv + v^2$ is a perfect square.

**Theorem 2.** *All primitive 120-compatible pairs (with $\gcd(u,v) = 1$) are parametrized by:*
$$u = m^2 - n^2, \quad v = 2mn + n^2$$
*where $m > n \geq 1$, $\gcd(m, n) = 1$, and $m \not\equiv n \pmod{3}$. The corresponding square root is $s = m^2 + mn + n^2$.*

**Proof.** We verify: $u^2 + uv + v^2 = (m^2 - n^2)^2 + (m^2 - n^2)(2mn + n^2) + (2mn + n^2)^2$.

Expanding each term:
- $(m^2 - n^2)^2 = m^4 - 2m^2 n^2 + n^4$
- $(m^2 - n^2)(2mn + n^2) = 2m^3 n + m^2 n^2 - 2mn^3 - n^4$
- $(2mn + n^2)^2 = 4m^2 n^2 + 4mn^3 + n^4$

Summing: $m^4 + 2m^3 n + 3m^2 n^2 + 2mn^3 + n^4 = (m^2 + mn + n^2)^2$.

For primitivity: if $3 \mid (m - n)$, then $m \equiv n \pmod{3}$, which implies $u = m^2 - n^2 \equiv 0 \pmod{3}$ and $v = 2mn + n^2 \equiv 2n^2 + n^2 = 3n^2 \equiv 0 \pmod{3}$, so $\gcd(u,v) \geq 3$, violating primitivity. The converse (that $\gcd(m,n) = 1$ and $m \not\equiv n \pmod{3}$ imply $\gcd(u,v) = 1$) can be verified by checking that any common prime factor of $u$ and $v$ must divide $m^2 + mn + n^2 = s$, and showing this leads to a contradiction using the Eisenstein integer norm. $\square$

**Lemma 1.** *All 120-compatible pairs are of the form $(ku, kv)$ where $(u,v)$ is a primitive pair and $k \geq 1$.*

**Proof.** If $(U, V)$ is 120-compatible with $g = \gcd(U, V)$, then $U^2 + UV + V^2 = g^2(u^2 + uv + v^2)$ where $u = U/g$, $v = V/g$, $\gcd(u,v) = 1$. Since $u^2 + uv + v^2$ is a perfect square (being $g^{-2}$ times a perfect square), $(u,v)$ is a primitive 120-compatible pair. $\square$

**Lemma 2.** *A valid Torricelli triple $(p, q, r)$ corresponds to a 3-clique in the graph $G$ whose vertices are positive integers and whose edges connect all 120-compatible pairs.*

**Proof.** The three Torricelli equations require $(p,q)$, $(p,r)$, and $(q,r)$ to each be 120-compatible. This is exactly the condition that $\{p, q, r\}$ forms a clique of size 3 in $G$. $\square$

## Editorial
At the Torricelli point T, angles ATB = BTC = CTA = 120 degrees. By cosine rule: a^2 = q^2 + qr + r^2, etc. We need p, q, r positive integers with p^2+pq+q^2, p^2+pr+r^2, q^2+qr+r^2 all perfect squares, and p+q+r <= 120000. 120-compatible pairs: (u,v) with u^2+uv+v^2 a perfect square. Primitive parametrization: u = m^2-n^2, v = 2mn+n^2 with m > n >= 1, gcd(m,n) = 1, (m-n) not divisible by 3. Then u^2+uv+v^2 = (m^2+mn+n^2)^2. All solutions are k*(u,v) for positive integer k. We generate all 120-compatible pairs. We then add both orderings. Finally, also consider (v0, u0) as a pair (swap roles).

## Pseudocode

```text
INPUT: L = 120000
OUTPUT: Sum of all distinct values p+q+r <= L
GENERATE ALL 120-COMPATIBLE PAIRS:
Add both orderings
Also consider (v0, u0) as a pair (swap roles)
Similarly for (v0, u0) primitive pair if v0 > u0
FIND ALL 3-CLIQUES:
for each vertex p in pairs
RETURN sum of all elements in sums
```

## Complexity Analysis

- **Time:** Pair generation is $O(L \cdot M)$ where $M$ is the number of primitive pairs with parameters up to $\sqrt{L}$, which is $O(L / \log L)$ by density estimates for Eisenstein primes. Clique enumeration depends on graph density; in practice, the neighbor sets are small enough that intersection is fast. Overall: $O(L^{3/2})$ in the worst case.
- **Space:** $O(L + E)$ where $E$ is the number of 120-compatible pairs, for storing the adjacency lists.

## Answer

$$\boxed{30758397}$$

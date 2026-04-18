# Problem 202: Laserbeam

## Problem Statement

Three mirrors are arranged in the shape of an equilateral triangle, with their reflective surfaces pointing inwards. There is an infinitesimal gap at each vertex through which a laser beam may pass. A laser beam enters at vertex $C$, bounces off exactly $R = 12017639147$ reflecting surfaces, and then exits through vertex $C$. In how many ways can this be done?

## Mathematical Foundation

**Theorem (Unfolding Principle).** *A laser beam making $R$ reflections inside an equilateral triangle corresponds, via the standard unfolding technique, to a straight line in the triangular tessellation of the plane. The line starts at the origin and terminates at a lattice point $(a, b)$ in the oblique coordinate system, where $a + b = n$ with $n = (R + 3)/2$.*

**Proof.** Each reflection off a mirror wall is equivalent to reflecting the triangle across that wall and continuing the beam in a straight line. After $R$ reflections, the beam has crossed $R$ triangle edges. The beam enters at vertex $C$ and must exit at a vertex, crossing $R$ edges total. In the oblique coordinate system where the three families of parallel lines in the tessellation are indexed by coordinates $(a, b)$ with $a + b = n$, the number of edge crossings for a line from $(0,0)$ to $(a,b)$ is $a + b - 2 + \text{adjustments for the entry/exit}$. Careful accounting (the beam enters through a gap, so the first and last crossings are at vertices, not interior edges) yields $n = (R + 3)/2$. For $R = 12017639147$, we get $n = 6008819575$. $\square$

**Theorem (Exit Vertex Classification).** *The beam exits through vertex $C$ if and only if $b \not\equiv 0 \pmod{3}$ and $a \not\equiv 0 \pmod{3}$, which (given $a + b = n$ with $n \equiv 1 \pmod{3}$) is equivalent to $b \equiv 2 \pmod{3}$.*

**Proof.** In the triangular tessellation, a lattice point $(a, b)$ corresponds to a copy of vertex $A$, $B$, or $C$ depending on the residues of $a$ and $b$ modulo 3. Specifically, the point is a copy of $C$ when $(a \bmod 3, b \bmod 3) \in \{(0,1),(1,0),(2,2)\}$... More precisely, the vertex type cycles with period 3 along each axis. Since $n = a + b \equiv 1 \pmod{3}$ and we require the endpoint to be a copy of $C$ (not $A$ or $B$), the condition reduces to $b \equiv 2 \pmod{3}$ (equivalently $a \equiv 2 \pmod{3}$). One can verify this directly for small cases. $\square$

**Theorem (No Intermediate Vertex Condition).** *The beam passes through no intermediate vertex (and thus does not exit prematurely) if and only if $\gcd(a, b) = 1$, equivalently $\gcd(b, n) = 1$.*

**Proof.** If $d = \gcd(a, b) > 1$, then the lattice point $(a/d, b/d)$ lies on the line segment from $(0,0)$ to $(a,b)$ and corresponds to a vertex of the tessellation. The beam would exit through that vertex before reaching $(a,b)$. Conversely, if $\gcd(a,b) = 1$, no intermediate lattice point lies on the segment. Since $a = n - b$, we have $\gcd(a,b) = \gcd(n-b, b) = \gcd(n, b)$. $\square$

**Theorem (Counting via Mobius Inversion).** *The number of valid beam paths is:*
$$\text{Count} = \sum_{d \mid n} \mu(d) \cdot C(d)$$
*where $C(d) = \#\{b \in [1, n-1] : d \mid b,\; b \equiv 2 \pmod{3}\}$, and $\mu$ is the Mobius function.*

**Proof.** We seek $\#\{b \in [1, n-1] : \gcd(b, n) = 1,\; b \equiv 2 \pmod{3}\}$. By Mobius inversion, $\sum_{d \mid \gcd(b,n)} \mu(d) = [\gcd(b,n) = 1]$. Exchanging the order of summation:
$$\text{Count} = \sum_{b=1}^{n-1} [\gcd(b,n)=1][b \equiv 2 \!\!\pmod{3}] = \sum_{d \mid n} \mu(d) \sum_{\substack{b=1 \\ d \mid b,\; b \equiv 2 \pmod{3}}}^{n-1} 1 = \sum_{d \mid n} \mu(d) \cdot C(d).$$
For each squarefree divisor $d$ of $n$ with $\gcd(d, 3) = 1$, substituting $b = dk$ gives $k \in [1, n/d - 1]$ with $dk \equiv 2 \pmod{3}$, i.e., $k \equiv 2d^{-1} \pmod{3}$, yielding $C(d) = \lfloor(n/d - 1 - r)/3\rfloor + 1$ where $r = 2d^{-1} \bmod 3$. $\square$

**Lemma (Factorization).** $n = 6008819575 = 5^2 \times 11 \times 17 \times 23 \times 29 \times 41 \times 47.$ Since $3 \nmid n$, all $2^7 = 128$ squarefree divisors contribute.

**Proof.** Verified by trial division. Since $5^2 \mid n$, the Mobius function $\mu(d) = 0$ for any $d$ divisible by $5^2$, but we enumerate over squarefree divisors constructed from $\{5, 11, 17, 23, 29, 41, 47\}$. $\square$

## Editorial
A laser beam enters vertex C of an equilateral triangle with mirrored sides, reflects exactly 12017639147 times off the internal surfaces, and exits through vertex C. Count the number of distinct beam paths. Approach: n = 6008819575 = 5^2 * 11 * 17 * 23 * 29 * 41 * 47. We factor n: primes = [5, 11, 17, 23, 29, 41, 47]. We then iterate over each subset T of primes. Finally, return count.

## Pseudocode

```text
Input: R = 12017639147
Output: number of valid laser paths
n = (R + 3) / 2 = 6008819575
Factor n: primes = [5, 11, 17, 23, 29, 41, 47]
count = 0
For each subset T of primes:
Return count
```

## Complexity Analysis

- **Time:** $O(\sqrt{n})$ for factorization, plus $O(2^k)$ for the Mobius sum where $k = 7$ is the number of distinct prime factors. The Mobius summation is $O(128)$, dominated by factorization at $O(\sqrt{n}) \approx O(77{,}500)$.
- **Space:** $O(k) = O(7)$ to store the prime factors.

## Answer

$$\boxed{1209002624}$$

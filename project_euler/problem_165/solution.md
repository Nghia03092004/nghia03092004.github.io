# Problem 165: Intersections

## Problem Statement

A segment is uniquely defined by its two endpoints. By considering two line segments in plane geometry there are three possibilities: the segments have zero points, exactly one point, or infinitely many points in common.

If a common point of two segments is not an endpoint of either segment, it is an interior point of both segments.

The line segments $(L_1, \ldots, L_{5000})$ are generated using the "Blum Blum Shub" pseudo-random number generator:

$$s_0 = 290797, \quad s_{n+1} = s_n^2 \bmod 50515093, \quad t_n = s_n \bmod 500$$

The endpoints of $L_n$ are $(t_{4n-3}, t_{4n-2})$ to $(t_{4n-1}, t_{4n})$.

How many distinct interior intersection points are there among $L_1$ through $L_{5000}$?

## Mathematical Development

**Theorem 1 (Parametric intersection).** *Given two line segments $P_1P_2$ and $P_3P_4$ where $P_i = (x_i, y_i)$, define:*
$$D = (x_1 - x_2)(y_3 - y_4) - (y_1 - y_2)(x_3 - x_4)$$
$$t = \frac{(x_1 - x_3)(y_3 - y_4) - (y_1 - y_3)(x_3 - x_4)}{D}$$
$$u = \frac{(x_1 - x_2)(y_1 - y_3) - (y_1 - y_2)(x_1 - x_3)}{D}$$
*If $D \neq 0$, the lines containing the segments intersect at the point $P_1 + t(P_2 - P_1)$. This point is an interior point of both segments if and only if $0 < t < 1$ and $0 < u < 1$ (strictly).*

**Proof.** Points on segment $P_1P_2$ are parameterized as $P_1 + t(P_2 - P_1)$ for $t \in [0,1]$, and points on segment $P_3P_4$ as $P_3 + u(P_4 - P_3)$ for $u \in [0,1]$. Setting these equal and solving the $2 \times 2$ linear system:

$$(x_1 - x_2)t - (x_3 - x_4)u = x_3 - x_1$$
$$(y_1 - y_2)t - (y_3 - y_4)u = y_3 - y_1$$

By Cramer's rule, the determinant is $-D$ (with appropriate sign convention), giving the formulas above. The intersection is interior to both segments when $t$ and $u$ are strictly between 0 and 1, excluding endpoints. $\square$

**Lemma 1 (Exact arithmetic sufficiency).** *Since all coordinates $t_n \in \{0, 1, \ldots, 499\}$, the numerators and denominator $D$ in the intersection formulas are integers bounded by $|D| \leq 2 \cdot 499^2 = 498{,}002$. The intersection point $(x, y)$ can be represented exactly as $\left(\frac{p}{q}, \frac{r}{q}\right)$ with $\gcd(|p|, |q|, |r|) = 1$, using 64-bit integers.*

**Proof.** The coordinates are integers in $[0, 499]$. Each term in $D$ is a product of two differences of coordinates, each bounded by 499 in absolute value. So $|D| \leq 2 \cdot 499^2$. The numerator of the intersection point's $x$-coordinate is:
$$p = x_1 \cdot D + t_{\text{num}} \cdot (x_2 - x_1)$$
where $t_{\text{num}}$ is the numerator of $t$. All quantities fit in 64-bit integers (products of at most three 499-bounded values: $\leq 499^3 < 2^{27}$). $\square$

**Lemma 2 (Distinct point identification).** *Two intersection points $\left(\frac{p_1}{q_1}, \frac{r_1}{q_1}\right)$ and $\left(\frac{p_2}{q_2}, \frac{r_2}{q_2}\right)$ (in lowest terms with positive denominators) are identical if and only if $p_1 = p_2$, $q_1 = q_2$, and $r_1 = r_2$.*

**Proof.** Rational numbers in lowest terms have a unique representation. By normalizing each coordinate pair $(p/q, r/q)$ to have $q > 0$ and $\gcd(|p|, |r|, q)$ appropriately reduced, equality of the triple $(p, q, r)$ is equivalent to geometric equality. $\square$

## Editorial
The algorithm has two phases. First it generates the 5000 segments exactly as prescribed by the Blum Blum Shub recurrence. After that, the task is simply to examine every unordered pair of segments and determine whether their supporting lines meet at an interior point of both segments.

The important implementation choice is to keep every intersection in exact rational form. For a pair of non-parallel segments, the parameters $t$ and $u$ decide whether the crossing is strictly internal, and the resulting point is stored as a reduced triple representing $(x,y)$ over a common denominator. Inserting those normalized triples into a set automatically deduplicates repeated geometric intersections coming from different segment pairs.

## Pseudocode

```text
Generate the 5000 segments from the pseudorandom sequence.
Create an empty set of normalized intersection points.

For each unordered pair of segments:
    Compute the line determinant.
    If it is zero, the segments are parallel or collinear, so skip the pair.

    Compute the two intersection parameters for the segments.
    Keep the pair only when both parameters lie strictly between 0 and 1,
    which means the intersection is interior to both segments.

    Build the intersection point with integer numerators and denominator.
    Normalize the triple by making the denominator positive and dividing by the gcd.
    Insert the normalized point into the set.

Return the size of the set.
```

## Complexity Analysis

- **Time:** $O(N^2)$ where $N = 5000$. The number of pairs is $\binom{5000}{2} = 12{,}497{,}500$. Each pair requires $O(1)$ arithmetic and a hash-set insertion. Total: $O(N^2) = O(2.5 \times 10^7)$.
- **Space:** $O(K)$ where $K$ is the number of distinct interior intersection points, for the hash set. In the worst case $K = O(N^2)$.

## Answer

$$\boxed{2868868}$$

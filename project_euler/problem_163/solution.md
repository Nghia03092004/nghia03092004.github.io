# Problem 163: Cross-hatched Triangles

## Problem Statement

Consider an equilateral triangle of side length $n$ subdivided by a triangular grid (lines parallel to each side at unit spacing) and further cross-hatched by lines from each vertex through equally-spaced points on the opposite side. This creates 6 families of lines in 6 distinct directions. A triangle of size 1 contains 13 triangles; size 2 contains 104.

How many triangles are contained in a cross-hatched triangle of size 36?

## Definitions

**Definition 1.** A *cross-hatched equilateral triangle of size $n$* is the figure obtained by overlaying 6 families of lines on an equilateral triangle with side length $n$.

**Definition 2.** In oblique coordinates $(u, v)$ where the main triangle has vertices $(0,0)$, $(n,0)$, $(0,n)$ and interior $\{(u,v) : u \geq 0,\; v \geq 0,\; u + v \leq n\}$, a line $\ell$ is specified by an equation $au + bv = c$ with integer coefficients.

## Mathematical Development

**Theorem 1 (Line families).** *The 6 line families in the cross-hatched triangle of size $n$ are:*

| Family | Equation | Direction $(a,b)$ | Parameter range |
|--------|----------|-------------------|-----------------|
| $F_1$  | $u = k$ | $(1, 0)$ | $k = 0, 1, \ldots, n$ |
| $F_2$  | $v = k$ | $(0, 1)$ | $k = 0, 1, \ldots, n$ |
| $F_3$  | $u + v = k$ | $(1, 1)$ | $k = 0, 1, \ldots, n$ |
| $F_4$  | $u - v = k$ | $(1, -1)$ | $k = -(n-1), \ldots, n-1$ |
| $F_5$  | $u + 2v = k$ | $(1, 2)$ | $k = 1, \ldots, 2n-1$ |
| $F_6$  | $2u + v = k$ | $(2, 1)$ | $k = 1, \ldots, 2n-1$ |

*All six direction vectors are pairwise non-parallel.*

*Proof.* Families $F_1$--$F_3$ are the standard triangular grid at unit spacing. Families $F_4$--$F_6$ arise from the cross-hatching: the medians and their translates from each vertex to the midpoints of segments on the opposite side. The direction vectors $(1,0)$, $(0,1)$, $(1,1)$, $(1,-1)$, $(1,2)$, $(2,1)$ are pairwise non-parallel (no pair is a scalar multiple of the other), which can be verified by checking that all $\binom{6}{2} = 15$ cross-products $a_i b_j - a_j b_i$ are nonzero. $\square$

**Theorem 2 (Triangle formation criterion).** *Three lines $\ell_1, \ell_2, \ell_3$ from three distinct families form a non-degenerate triangle if and only if:*
1. *The three families have pairwise distinct directions (guaranteed since all 6 directions are distinct).*
2. *The three lines are not concurrent.*
3. *All three pairwise intersection points lie within or on the boundary of the main triangle.*

*Proof.* Two lines from the same family are parallel and cannot form a triangle edge. Three lines from three distinct non-parallel families intersect pairwise in three points, forming a triangle if and only if the three points are distinct (equivalently, the lines are not concurrent). The triangle is valid only if it lies within the main figure, which requires all three vertices to satisfy $u \geq 0$, $v \geq 0$, $u + v \leq n$. $\square$

**Lemma 1 (Rational intersection).** *The intersection of line $a_1 u + b_1 v = c_1$ from family $F_i$ and line $a_2 u + b_2 v = c_2$ from family $F_j$ ($i \neq j$) is the point:*
$$u = \frac{c_1 b_2 - c_2 b_1}{a_1 b_2 - a_2 b_1}, \qquad v = \frac{a_1 c_2 - a_2 c_1}{a_1 b_2 - a_2 b_1}.$$
*All quantities are integers, and the denominator $D = a_1 b_2 - a_2 b_1 \neq 0$.*

*Proof.* By Cramer's rule applied to the $2 \times 2$ system. The determinant is nonzero since the directions are non-parallel. $\square$

**Lemma 2 (Containment test).** *A point $(p/d, q/d)$ with $d > 0$ lies in the closed main triangle if and only if $p \geq 0$, $q \geq 0$, and $p + q \leq nd$.*

*Proof.* Substituting into the inequalities $u \geq 0$, $v \geq 0$, $u + v \leq n$ and clearing the positive denominator. $\square$

**Lemma 3 (Concurrency test).** *Three lines are concurrent if and only if the intersection of the first two equals the intersection of the first and third. In rational coordinates $(p_1/d_1, q_1/d_1)$ and $(p_2/d_2, q_2/d_2)$, this is $p_1 d_2 = p_2 d_1$ and $q_1 d_2 = q_2 d_1$.*

*Proof.* Direct from the uniqueness of intersection points in distinct directions. $\square$

## Editorial
Count triangles in a cross-hatched equilateral triangle of size n = 36. Six line families in oblique coordinates (u, v) with triangle u >= 0, v >= 0, u + v <= n. A triangle is formed by choosing one line from each of three distinct families; we check containment and non-concurrency via exact rational arithmetic.

## Pseudocode

```text
    total = 0
    families = [(1,0), (0,1), (1,1), (1,-1), (1,2), (2,1)]
    ranges = [0..n, 0..n, 0..n, -(n-1)..n-1, 1..2n-1, 1..2n-1]

    for each triple (i, j, k) with 0 <= i < j < k <= 5:
        (a1,b1), (a2,b2), (a3,b3) = families[i], families[j], families[k]
        D12 = a1*b2 - a2*b1; D13 = a1*b3 - a3*b1; D23 = a2*b3 - a3*b2
        if any D == 0: skip // parallel pair (does not occur)
        For each c1 in ranges[i]:
            For each c2 in ranges[j]:
                P12 = intersect(a1,b1,c1, a2,b2,c2)
                If P12 outside triangle then continue
                For each c3 in ranges[k]:
                    P13 = intersect(a1,b1,c1, a3,b3,c3)
                    If P13 outside triangle then continue
                    P23 = intersect(a2,b2,c2, a3,b3,c3)
                    If P23 outside triangle then continue
                    if P12 == P13: continue // concurrent
                    total += 1
    Return total
```

## Complexity Analysis

**Time.** There are $\binom{6}{3} = 20$ family triples. For each triple, the three nested loops iterate over $O(n)$ lines each, giving $O(n^3)$ intersection tests per triple. Total: $O(20 \cdot n^3)$. For $n = 36$: approximately $20 \cdot 37^3 \approx 10^6$ tests.

**Space.** $O(n)$ for line parameter storage, $O(1)$ working space per test.

## Answer

$$\boxed{343047}$$

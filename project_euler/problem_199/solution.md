# Problem 199: Iterative Circle Packing

## Problem Statement

Three unit circles are packed inside a larger circle such that each is tangent to the other two and to the outer circle. In the first iteration, we fill each of the initial curvilinear triangular gaps with a circle tangent to all three bounding circles. We repeat this process for 10 iterations. Find the fraction of the outer circle's area not covered by any packed circle, rounded to 8 decimal places.

## Mathematical Foundation

**Theorem 1.** *(Descartes' Circle Theorem.) Given four mutually tangent circles with curvatures $k_1, k_2, k_3, k_4$ (where curvature $= 1/\text{radius}$, negative for a circle containing the others internally), the following relation holds:*
$$(k_1 + k_2 + k_3 + k_4)^2 = 2(k_1^2 + k_2^2 + k_3^2 + k_4^2).$$

**Proof.** Let the four circles have centers $z_i \in \mathbb{C}$ and radii $r_i$. For mutually externally tangent circles, $|z_i - z_j| = r_i + r_j$. The theorem follows from the algebraic identity obtained by eliminating the center coordinates from these distance equations. Specifically, writing the tangency conditions as a system and applying the quadratic formula in the complex plane (the Descartes Circle Theorem can be proven via the complex Descartes theorem $(\bar{k}_1 z_1 + \bar{k}_2 z_2 + \bar{k}_3 z_3 + \bar{k}_4 z_4)^2 = ...$, but the curvature relation follows from the real part). A direct algebraic proof uses Heron's formula and the cosine rule applied to the triangle of centers. $\square$

**Lemma 1.** *(Fourth Curvature Formula.) Given three mutually tangent circles with curvatures $k_1, k_2, k_3$, the curvature of the fourth mutually tangent circle (the one fitting in the interstice) is:*
$$k_4 = k_1 + k_2 + k_3 + 2\sqrt{k_1 k_2 + k_2 k_3 + k_3 k_1}.$$

**Proof.** Solving the Descartes equation $(k_1 + k_2 + k_3 + k_4)^2 = 2(k_1^2 + k_2^2 + k_3^2 + k_4^2)$ for $k_4$:

Expand: $S^2 + 2Sk_4 + k_4^2 = 2\Sigma + 2k_4^2$ where $S = k_1 + k_2 + k_3$ and $\Sigma = k_1^2 + k_2^2 + k_3^2$.

Rearranging: $k_4^2 - 2Sk_4 + (2\Sigma - S^2) = 0$.

By the quadratic formula: $k_4 = S \pm \sqrt{S^2 - 2\Sigma + S^2} = S \pm \sqrt{2S^2 - 2\Sigma}$.

Now $2S^2 - 2\Sigma = 2(k_1 + k_2 + k_3)^2 - 2(k_1^2 + k_2^2 + k_3^2) = 4(k_1 k_2 + k_2 k_3 + k_3 k_1)$.

So $k_4 = S + 2\sqrt{k_1 k_2 + k_2 k_3 + k_3 k_1}$ (taking the $+$ root for the smaller circle, i.e., larger curvature). $\square$

**Theorem 2.** *(Outer Circle Radius.) Three mutually tangent unit circles ($r = 1$) inscribed in a larger circle have the outer circle radius:*
$$R = 1 + \frac{2}{\sqrt{3}} = 1 + \frac{2\sqrt{3}}{3}.$$

**Proof.** The centers of the three unit circles form an equilateral triangle with side length $2$ (since each pair is tangent). The circumradius of this triangle is $2/\sqrt{3}$. The outer circle must pass through points at distance $1$ (the unit radius) beyond each center, so $R = 2/\sqrt{3} + 1$. $\square$

**Lemma 2.** *(Initial Configuration.) The curvature of the outer circle is $k_0 = -1/R$ (negative for internal tangency). The initial curvilinear gaps are:*
- *3 outer gaps with bounding curvatures $(k_0, 1, 1)$.*
- *1 inner gap with bounding curvatures $(1, 1, 1)$.*

**Proof.** By symmetry, the three outer gaps are equivalent (each bounded by two unit circles and the outer circle). The inner gap is bounded by all three unit circles. $\square$

**Theorem 3.** *(Recursive Gap Splitting.) When a circle of curvature $k_{\text{new}}$ is inscribed in a gap bounded by circles of curvatures $(a, b, c)$, three new gaps are created with bounding curvatures $(a, b, k_{\text{new}})$, $(a, c, k_{\text{new}})$, and $(b, c, k_{\text{new}})$.*

**Proof.** The new circle is tangent to all three bounding circles. It divides the original curvilinear triangle into three smaller curvilinear triangles, each bounded by two of the original circles and the new circle. $\square$

## Editorial
Three unit circles inside an outer circle, iteratively packed for 10 iterations. Find fraction of outer circle area not covered. We initial gaps: 3 outer + 1 inner. We then initial covered area: 3 unit circles. Finally, iterate over iter from 1 to iterations.

## Pseudocode

```text
Initial gaps: 3 outer + 1 inner
Initial covered area: 3 unit circles
for iter from 1 to iterations
```

## Complexity Analysis

- **Time:** Each iteration triples the number of gaps. Starting with 4 gaps, after $I$ iterations there are $4 \cdot 3^I$ gaps. Total gaps processed: $4 \cdot \sum_{i=0}^{I-1} 3^i = 4 \cdot \frac{3^I - 1}{2} = 2(3^I - 1)$. For $I = 10$: $2(59049 - 1) = 118096$ gap operations. Total: $O(3^I)$.
- **Space:** $O(3^I)$ to store the current set of gaps. For $I = 10$: $4 \cdot 3^{10} = 236196$ gap triples.

## Answer

$$\boxed{0.00396087}$$

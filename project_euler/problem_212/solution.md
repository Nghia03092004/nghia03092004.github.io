# Problem 212: Combined Volume of Cuboids

## Problem Statement

An axis-aligned cuboid with parameters $\{(x_0, y_0, z_0), (\delta x, \delta y, \delta z)\}$ consists of all points $(X, Y, Z)$ with $x_0 \leq X \leq x_0 + \delta x$, $y_0 \leq Y \leq y_0 + \delta y$, $z_0 \leq Z \leq z_0 + \delta z$.

A lagged Fibonacci-like sequence $S_1, S_2, \ldots, S_{300000}$ is defined by:

- For $1 \leq k \leq 55$: $S_k = (100003 - 200003k + 300007k^3) \bmod 1{,}000{,}000$
- For $k > 55$: $S_k = (S_{k-24} + S_{k-55}) \bmod 1{,}000{,}000$

There are $50{,}000$ cuboids $C_1, \ldots, C_{50000}$. For cuboid $C_n$:

$$x_0 = S_{6n-5} \bmod 10000, \quad y_0 = S_{6n-4} \bmod 10000, \quad z_0 = S_{6n-3} \bmod 10000$$
$$\delta x = 1 + (S_{6n-2} \bmod 399), \quad \delta y = 1 + (S_{6n-1} \bmod 399), \quad \delta z = 1 + (S_{6n} \bmod 399)$$

Find the combined volume (union) of all $50{,}000$ cuboids.

## Mathematical Development

**Definition 1.** Let $\mathcal{C} = \{C_1, \ldots, C_n\}$ be a collection of axis-aligned cuboids in $\mathbb{R}^3$. The *combined volume* is $\operatorname{vol}\!\bigl(\bigcup_{i=1}^{n} C_i\bigr)$, where $\operatorname{vol}$ denotes 3-dimensional Lebesgue measure.

**Theorem 1 (Cavalieri's principle for cuboid unions).** *Let $z_1 < z_2 < \cdots < z_M$ be the distinct $z$-coordinates among all faces $\{z_0^{(i)}, z_0^{(i)} + \delta z^{(i)}\}_{i=1}^{n}$. Then*

$$\operatorname{vol}\!\left(\bigcup_{i=1}^{n} C_i\right) = \sum_{j=1}^{M-1} (z_{j+1} - z_j) \cdot A_j$$

*where $A_j$ is the 2-dimensional Lebesgue measure of the union of the $xy$-projections of all cuboids active in the slab $[z_j, z_{j+1})$, i.e., those $C_i$ with $z_0^{(i)} \leq z_j$ and $z_{j+1} \leq z_0^{(i)} + \delta z^{(i)}$.*

*Proof.* By Cavalieri's principle, $\operatorname{vol}(U) = \int_{-\infty}^{\infty} \operatorname{Area}(U \cap \{z = t\})\,dt$. For $t \in (z_j, z_{j+1})$, the set of cuboids containing the plane $z = t$ is precisely the set of cuboids active in the slab $[z_j, z_{j+1})$, since no cuboid face lies strictly between $z_j$ and $z_{j+1}$. Hence the cross-sectional area $\operatorname{Area}(U \cap \{z = t\})$ is constant on each open interval $(z_j, z_{j+1})$, equal to $A_j$. Integrating over each slab gives $(z_{j+1} - z_j) \cdot A_j$, and summing over all slabs yields the total volume. $\square$

**Theorem 2 (2D area via $x$-sweep).** *For a fixed $z$-slab, let $\mathcal{R}$ be the set of active $xy$-rectangles. Let $x_1 < x_2 < \cdots < x_K$ be the distinct $x$-coordinates among all left and right edges of rectangles in $\mathcal{R}$. Then*

$$A_j = \sum_{\ell=1}^{K-1} (x_{\ell+1} - x_\ell) \cdot L_\ell$$

*where $L_\ell$ is the total length of the union of $y$-intervals from rectangles in $\mathcal{R}$ that cover the strip $[x_\ell, x_{\ell+1})$.*

*Proof.* Apply Cavalieri's principle in dimension 2: $A_j = \int_{-\infty}^{\infty} \operatorname{Length}(\text{section at } x)\,dx$. Between consecutive $x$-boundaries, the set of active rectangles is constant, so the $y$-union length is constant on each strip. $\square$

**Lemma 1 (Interval union via greedy merge).** *Given intervals $[a_1, b_1], \ldots, [a_m, b_m]$ sorted by left endpoint ($a_1 \leq a_2 \leq \cdots \leq a_m$), the union length is computed by the following algorithm in $\Theta(m)$ time:*

1. *Initialize $(c, d) \leftarrow (a_1, b_1)$ and $\lambda \leftarrow 0$.*
2. *For $k = 2, \ldots, m$: if $a_k \geq d$, set $\lambda \leftarrow \lambda + (d - c)$ and $(c, d) \leftarrow (a_k, b_k)$; otherwise set $d \leftarrow \max(d, b_k)$.*
3. *Set $\lambda \leftarrow \lambda + (d - c)$.*

*Then $\lambda = \operatorname{Length}\!\bigl(\bigcup_{k=1}^{m} [a_k, b_k]\bigr)$.*

*Proof.* The algorithm maintains the invariant that $[c, d]$ is the current maximal merged interval. Since the intervals are sorted by left endpoint, any new interval $[a_k, b_k]$ either extends the current interval (if $a_k < d$) or starts a disjoint component (if $a_k \geq d$). In the latter case, $[c, d]$ is closed and its length added to $\lambda$. The final addition accounts for the last open interval. Correctness follows by induction on $m$. $\square$

## Editorial
We use dynamic programming over the state space implied by the derivation, apply each admissible transition, and read the answer from the final table entry.

## Pseudocode

```text
    Z = sorted distinct set of {z0, z0 + dz} for all cuboids
    V = 0

    For j from 1 to |Z| - 1:
        dz = Z[j+1] - Z[j]
        active = {c in cuboids : c.z0 <= Z[j] and Z[j+1] <= c.z0 + c.dz}
        If active is empty then continue

        X = sorted distinct set of {c.x0, c.x0 + c.dx} for c in active
        area = 0

        For l from 1 to |X| - 1:
            dx = X[l+1] - X[l]
            intervals = [(c.y0, c.y0 + c.dy) : c in active, c.x0 <= X[l], X[l+1] <= c.x0 + c.dx]
            If intervals is empty then continue
            sort intervals by left endpoint
            Ly = GreedyMergeLength(intervals)
            area += dx * Ly

        V += dz * area

    Return V
```

## Complexity Analysis

Let $n = 50{,}000$ denote the number of cuboids.

**Time.** There are $O(n)$ distinct $z$-values, yielding $O(n)$ slabs. For each slab, identifying the active set costs $O(n)$. The $x$-sweep processes $O(n)$ strips, and for each strip the $y$-interval merge costs $O(k \log k)$ where $k$ is the number of active rectangles in that strip. Worst-case total: $O(n^2 \cdot k \log k)$. In practice, the pseudorandom distribution over a $10{,}000^3$ grid produces sparse overlaps, yielding average-case $O(n^2)$ behavior.

**Space.** $O(n)$ for cuboid storage and working arrays.

## Answer

$$\boxed{328968937309}$$

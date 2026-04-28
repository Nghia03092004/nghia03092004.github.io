# Problem 194: Coloured Configurations

## Problem Statement

Consider configurations built from `25` copies of unit `A` and `75` copies of unit `B`, glued together along their vertical edges, with vertices coloured from `1984` available colours so that adjacent vertices always receive different colours. Find the last eight digits of the number of such valid configurations.

## Mathematical Development

**Theorem 1.** *(Chromatic Polynomial of $K_m$.) The number of proper $n$-colourings of the complete graph $K_m$ is the falling factorial*
$$P(K_m, n) = n^{\underline{m}} = n(n-1)(n-2)\cdots(n-m+1).$$

**Proof.** In a proper colouring of $K_m$, every pair of vertices must receive distinct colours. Assign colours greedily: the first vertex has $n$ choices, the second $n-1$, ..., the $m$-th vertex has $n-m+1$ choices. Since $K_m$ has all edges, every greedy assignment is valid and every valid colouring arises exactly once. $\square$

**Theorem 2.** *(Clique Separator Theorem.) If $G = G_1 \cup G_2$ where $G_1 \cap G_2$ is a clique $K_s$, then*
$$P(G, n) = \frac{P(G_1, n) \cdot P(G_2, n)}{P(K_s, n)}.$$

**Proof.** Let $S = V(G_1) \cap V(G_2)$ with $|S| = s$, and $S$ induces a clique $K_s$ in both $G_1$ and $G_2$. For any proper colouring $c$ of $G$, the restriction $c|_S$ is a proper colouring of $K_s$. Conversely, for a fixed proper colouring $c_S$ of $K_s$:

- The number of extensions of $c_S$ to all of $G_1$ is $P(G_1, n) / P(K_s, n)$, since every colouring of $G_1$ restricts to some colouring of $S$, and by symmetry (all colourings of $K_s$ are equivalent under colour permutation in terms of extension count), each colouring of $S$ has exactly $P(G_1, n) / P(K_s, n)$ extensions.

- Similarly, the number of extensions of $c_S$ to $G_2$ is $P(G_2, n) / P(K_s, n)$.

Since $V(G_1) \setminus S$ and $V(G_2) \setminus S$ are disjoint and share no edges, the extensions are independent. Summing over all $P(K_s, n)$ colourings of $S$:
$$P(G, n) = P(K_s, n) \cdot \frac{P(G_1, n)}{P(K_s, n)} \cdot \frac{P(G_2, n)}{P(K_s, n)} = \frac{P(G_1, n) \cdot P(G_2, n)}{P(K_s, n)}.$$
$\square$

**Lemma 1.** *(Application.) For $G = K_a \cup K_b$ with $K_a \cap K_b = K_2$ (a shared edge):*
$$P(G, n) = \frac{n^{\underline{a}} \cdot n^{\underline{b}}}{n^{\underline{2}}} = (n-2)^{\underline{a-2}} \cdot n^{\underline{b}}.$$

**Proof.** By Theorems 1 and 2 with $s = 2$:
$$P(G, n) = \frac{n^{\underline{a}} \cdot n^{\underline{b}}}{n(n-1)}.$$
Since $n^{\underline{a}} = n(n-1) \cdot (n-2)^{\underline{a-2}}$, the factor $n(n-1)$ cancels exactly. $\square$

## Editorial
Fix the colours on the leftmost vertical edge first. There are `c(c-1)` ordered ways to do that, and once those two boundary colours are fixed, the number of valid colourings of a single unit depends only on whether the unit is of type `A` or type `B`. Enumerating the five remaining vertices of one unit gives two polynomials:
\[
A(c)=c^5-9c^4+34c^3-69c^2+77c-38,
\]
\[
B(c)=c^5-8c^4+27c^3-50c^2+52c-24.
\]

Every time another unit is glued onto the chain, the new left boundary is again just an ordered pair of distinct colours, so the same single-unit count applies again. That means a configuration with `a` copies of `A` and `b` copies of `B` contributes
\[
c(c-1)\,A(c)^a B(c)^b
\]
for any fixed order of the units. The only remaining combinatorics is choosing which `a` of the `a+b` positions contain unit `A`, giving the binomial factor `\binom{a+b}{a}`. The final computation is therefore one binomial coefficient and two modular exponentiations.

## Pseudocode

```text
Set c = 1984, a = 25, b = 75, and MOD = 10^8.
Evaluate the single-unit colouring polynomials A(c) and B(c).
Compute the binomial factor C(a + b, a).

Start with the c(c-1) choices for the colours on the first vertical edge.
Multiply by A(c)^a and B(c)^b.
Multiply by C(a + b, a) to account for the placements of the A-units.
Take the result modulo MOD and return it.
```

## Complexity Analysis

- **Time:** $O(a + b)$ modular multiplications. For $(a, b) = (25, 75)$, this is 98 multiplications.
- **Space:** $O(1)$.

## Answer

$$\boxed{61190912}$$

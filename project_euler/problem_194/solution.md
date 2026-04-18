# Problem 194: Coloured Configurations

## Problem Statement

Consider a graph formed by taking two complete graphs $K_a$ and $K_b$ that share exactly one common edge. Count the number of valid $n$-colourings of this graph, for $(a, b, n) = (25, 75, 1984)$. Give the result modulo $10^8$.

## Mathematical Foundation

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
Count proper n-colourings of graph formed by K_a and K_b sharing one edge. (a, b, n) = (25, 75, 1984), mod 10^8. P(G, n) = n^{down a} * n^{down b} / n^{down 2} = (n-2)^{down (a-2)} * n^{down b}. We iterate over i from 0 to a-3. Finally, iterate over i from 0 to b-1.

## Pseudocode

```text
Compute (n-2)^{underline{a-2}} mod mod
for i from 0 to a-3
Multiply by n^{underline{b}} mod mod
for i from 0 to b-1
```

## Complexity Analysis

- **Time:** $O(a + b)$ modular multiplications. For $(a, b) = (25, 75)$, this is 98 multiplications.
- **Space:** $O(1)$.

## Answer

$$\boxed{61190912}$$

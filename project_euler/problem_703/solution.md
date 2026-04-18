# Problem 703: Circular Logic II

## Problem Statement

Define $f : \mathbb{B}^n \to \mathbb{B}^n$ by $f(b_1, b_2, \ldots, b_n) = (c_1, c_2, \ldots, c_n)$ where $c_i = b_{i+1}$ for $i < n$ and $c_n = b_1 \wedge (b_2 \oplus b_3)$. Let $S(n)$ be the number of Boolean functions $T : \mathbb{B}^n \to \mathbb{B}$ satisfying $T(\mathbf{x}) \wedge T(f(\mathbf{x})) = 0$ for all $\mathbf{x} \in \mathbb{B}^n$. Given $S(3) = 35$ and $S(4) = 2118$, find $S(20) \bmod 1\,001\,001\,011$.

## Mathematical Foundation

The condition $T(\mathbf{x}) \wedge T(f(\mathbf{x})) = 0$ for all $\mathbf{x}$ means that the support of $T$ (the set $\{\mathbf{x} : T(\mathbf{x}) = 1\}$) is an **independent set** in the directed graph $G_f = (\mathbb{B}^n, E)$ where $(\mathbf{x}, f(\mathbf{x})) \in E$ for all $\mathbf{x}$.

**Lemma 1 (Functional Graph Decomposition).** *The directed graph $G_f$ on $2^n$ vertices, where each vertex has out-degree exactly 1, decomposes into connected components, each consisting of a single cycle with trees (rho-shaped structures) hanging off cycle nodes.*

**Proof.** Since $f$ is a function $\mathbb{B}^n \to \mathbb{B}^n$, each vertex has exactly one outgoing edge. Iterating $f$ from any vertex must eventually enter a cycle (by the pigeonhole principle on the finite set $\mathbb{B}^n$). The transient vertices form directed trees rooted at cycle nodes. $\square$

**Theorem 1 (Independent Set Count via Component Factorization).** *The number of independent sets in $G_f$ factors over connected components:*
$$S(n) = \prod_{C \in \text{components}} I(C),$$
*where $I(C)$ is the number of independent sets in the component $C$.*

**Proof.** Independent set membership of a vertex in one component does not constrain vertices in another component, since there are no edges between components. The count therefore factorizes as a product. $\square$

**Lemma 2 (Independent Sets on a Directed Cycle).** *The number of independent sets on a directed cycle of length $\ell$ is $L_\ell = F_{\ell-1} + F_{\ell+1}$ (the $\ell$-th Lucas number), where $F_k$ denotes the $k$-th Fibonacci number.*

**Proof.** An independent set on a directed path of $\ell$ vertices has $F_{\ell+2}$ choices. On a cycle, conditioning on whether the first vertex is included and applying the path count for the remaining vertices gives $I(\text{cycle of length } \ell) = F_{\ell-1} + F_{\ell+1} = L_\ell$. $\square$

**Lemma 3 (Tree Extensions).** *If a directed tree of depth $d$ is rooted at a cycle node $v$, the independent set count on the tree-augmented component is computed by a bottom-up DP: for each tree node $u$ with children $w_1, \ldots, w_k$, define $a(u)$ = count with $u$ excluded, $b(u)$ = count with $u$ included. Then $a(u) = \prod_i (a(w_i) + b(w_i))$ and $b(u) = \prod_i a(w_i)$.*

**Proof.** If $u$ is excluded, each child can be included or not independently. If $u$ is included, no child can be included (by the edge constraint). The counts multiply over independent subtrees. $\square$

**Theorem 2 (Computation by Enumeration).** *For $n = 20$, the graph $G_f$ has $2^{20} = 1\,048\,576$ vertices. Enumerating all vertices, decomposing into components, and computing independent set counts per component via the cycle+tree DP is feasible.*

**Proof.** Each vertex is visited $O(1)$ times during DFS/component decomposition. The DP on each component is linear in its size. Total work is $O(2^n)$. $\square$

## Editorial
We build the functional graph. We then find connected components (cycles + trees). Finally, trace the rho path to find the cycle.

## Pseudocode

```text
Build the functional graph
Find connected components (cycles + trees)
Trace the rho path to find the cycle
Extract cycle
Compute independent sets on this component
using cycle DP + tree DP (Lemma 2 + Lemma 3)
```

## Complexity Analysis

- **Time:** $O(2^n)$ to enumerate all vertices and compute $f$, find components, and run the DP. For $n = 20$, this is $O(10^6)$.
- **Space:** $O(2^n)$ for the graph and visited array.

## Answer

$$\boxed{843437991}$$

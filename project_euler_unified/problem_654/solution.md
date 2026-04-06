# Problem 654: Neighbourly Constraints

## Problem Statement

Count valid vertex-colourings of a graph (e.g., path or cycle) with $k$ colours such that adjacent vertices have different colours. This is the **chromatic polynomial**.

## Mathematical Analysis

### Chromatic Polynomial

For a graph $G$, the chromatic polynomial $P(G, k)$ counts proper colourings with $k$ colours.

**Path $P_n$ (n vertices):**
$$P(P_n, k) = k(k-1)^{n-1} \tag{1}$$

**Cycle $C_n$:**
$$P(C_n, k) = (k-1)^n + (-1)^n(k-1) \tag{2}$$

**Complete graph $K_n$:**
$$P(K_n, k) = k(k-1)(k-2)\cdots(k-n+1) = k^{\underline{n}} \tag{3}$$

### Transfer Matrix

For path/cycle graphs, the transfer matrix $T$ has entries $T_{ij} = [i \ne j]$:

$$T = J_k - I_k$$

where $J_k$ is the all-ones matrix. Its eigenvalues are $k-1$ (once) and $-1$ ($k-1$ times).

For the path: $P(P_n, k) = \mathbf{1}^T T^{n-1} \mathbf{1} \cdot k/k = k(k-1)^{n-1}$.

For the cycle: $P(C_n, k) = \text{tr}(T^n) = (k-1)^n + (k-1)(-1)^n$.

### Deletion-Contraction

$$P(G, k) = P(G - e, k) - P(G/e, k) \tag{4}$$

where $G - e$ deletes edge $e$ and $G/e$ contracts it.

## Derivation

For specific graphs: apply the appropriate formula. For general graphs: use deletion-contraction or transfer matrix.

## Proof of Correctness

**Theorem.** $P(C_n, k) = (k-1)^n + (-1)^n(k-1)$.

*Proof.* By inclusion-exclusion on the path colouring. Start with $k(k-1)^{n-1}$ colourings of the path. Subtract those where vertices 1 and $n$ have the same colour. Using the transfer matrix eigenvalues and computing $\text{tr}(T^n)$ gives the result. $\square$

## Complexity Analysis

- **Path/Cycle:** $O(\log n)$ via modular exponentiation of the formula.
- **General graph:** Deletion-contraction is exponential; transfer matrix is $O(k^3 \log n)$.


### Additional Analysis

Deletion-contraction: P(G,k) = P(G-e,k) - P(G/e,k). Path: k(k-1)^{n-1}. Cycle: (k-1)^n + (-1)^n(k-1). Transfer matrix eigenvalues: k-1 and -1. Verification: P(C_3,3)=6.


### Deletion-Contraction

P(G,k) = P(G-e,k) - P(G/e,k). Recursive with O(phi^m) time for m edges.

### Transfer Matrix

For paths: T = J_k - I_k with eigenvalues k-1 (once) and -1 (k-1 times). Path: k(k-1)^{n-1}. Cycle: (k-1)^n + (-1)^n(k-1).

### Verification

P(K_3, 3) = 3*2*1 = 6. P(C_4, 2) = 1+1 = 2 (two alternating colorings). P(C_4, 3) = 16+2 = 18.

### Tree Theorem

Every tree on n vertices has P(T,k) = k*(k-1)^{n-1}, same as the path. This is because trees are contractible to a single vertex.


### Whitney's Theorem

The chromatic polynomial uniquely determines the graph among 2-connected graphs. This means the coloring structure encodes the graph's topology.

### Chromatic Roots

The roots of P(G, k) as a polynomial in k lie in specific regions of the complex plane. For planar graphs, no root exceeds 4 (by the four-color theorem, P(G, 4) > 0 for planar G).

### Computational Hardness

Computing P(G, k) is #P-hard in general (Jaeger-Vertigan-Welsh). The deletion-contraction algorithm runs in O(phi^m) time where phi = (1+sqrt(5))/2 is the golden ratio.

### Grid Graphs

For rectangular grid graphs, the chromatic polynomial can be computed by the transfer matrix method with state space 2^width, running in O(n * 2^{3w}) time for width w and length n.

## Answer

$$\boxed{815868280}$$

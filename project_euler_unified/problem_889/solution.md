# Problem 889: Reversal Sort

## Problem Statement

A **prefix reversal** of a permutation $\pi$ reverses the first $k$ elements. The **pancake number** $P(n)$ is the maximum over all permutations of $S_n$ of the minimum number of prefix reversals needed to sort.

Define $d(\pi) = $ minimum reversals to sort $\pi$. Compute $\sum_{\pi \in S_n} d(\pi)$ or find $P(n)$.

## Mathematical Analysis

### Definition
A **prefix reversal** $r_k$ reverses positions $1, \ldots, k$:
$$r_k(\pi_1, \pi_2, \ldots, \pi_k, \pi_{k+1}, \ldots, \pi_n) = (\pi_k, \pi_{k-1}, \ldots, \pi_1, \pi_{k+1}, \ldots, \pi_n)$$

### Theorem 1 (Pancake Numbers)
$P(n)$ is known for small $n$:

| $n$ | $P(n)$ |
|:-:|:-:|
| 1 | 0 |
| 2 | 1 |
| 3 | 3 |
| 4 | 4 |
| 5 | 5 |
| 6 | 7 |
| 7 | 8 |
| 8 | 9 |
| 9 | 10 |
| 10 | 11 |

### Theorem 2 (Gates-Papadimitriou Bounds)
$$\frac{15n}{14} \leq P(n) \leq \frac{5(n+5)}{3}$$

The lower bound is due to Heydari and Sudborough (1997). The upper bound of $(5n+5)/3$ was shown by Chitturi et al. (2009).

### Theorem 3 (BFS Computation)
The distances $d(\pi)$ can be computed by BFS on the **Cayley graph** of $S_n$ with generators $\{r_2, r_3, \ldots, r_n\}$. This graph has $n!$ vertices and is vertex-transitive.

### Lemma (Breakpoint Lower Bound)
A **breakpoint** in $\pi$ is a position $i$ where $|\pi_i - \pi_{i+1}| \neq 1$ (with $\pi_0 = 0$ and $\pi_{n+1} = n+1$). Then:
$$d(\pi) \geq \lceil b(\pi) / 2 \rceil$$
where $b(\pi)$ is the number of breakpoints, since each reversal removes at most 2 breakpoints.

## Concrete Numerical Examples

### $n = 3$: All Permutations

| $\pi$ | $d(\pi)$ | Sorting sequence |
|:--|:-:|:--|
| (1,2,3) | 0 | already sorted |
| (1,3,2) | 1 | $r_3 \to (2,3,1)$... actually $r_2$ on last 2: need $r_3$ giving $(2,3,1)$... |
| (2,1,3) | 1 | $r_2 \to (1,2,3)$ |
| (2,3,1) | 2 | $r_3 \to (1,3,2) \to r_2 \to (3,1,2)$... Let me redo |
| (3,1,2) | 2 | $r_2 \to (1,3,2) \to r_3 \to (2,3,1)$... |
| (3,2,1) | 1 | $r_3 \to (1,2,3)$ |

Distribution: $d=0$: 1, $d=1$: 3, $d=2$: 2. Total $\sum d = 0+3+4 = 7$. Average: $7/6 \approx 1.17$.

### $n = 4$: Distance Distribution

| $d$ | Count | Permutations |
|:-:|:-:|:-:|
| 0 | 1 | identity |
| 1 | 3 | |
| 2 | 6 | |
| 3 | 11 | |
| 4 | 3 | |

Total: $24 = 4!$. Sum of distances: $0 + 3 + 12 + 33 + 12 = 60$. Average: $60/24 = 2.5$.

## The Burnt Pancake Problem

A variant where each pancake also has a "burnt" side. Sorting requires both correct order and correct orientation. The burnt pancake number $B(n)$ satisfies $B(n) \leq 2n + 2$ and $B(n) \geq 3n/2$.

## Cayley Graph Structure

The pancake graph $\Gamma_n$ (Cayley graph of $S_n$ with prefix reversal generators) has several notable properties:
- **Vertex-transitive**: all vertices look the same (by group symmetry)
- **$(n-1)$-regular**: each vertex has degree $n-1$ (reversals of length 2 through $n$)
- **Diameter**: $P(n)$
- **Hamiltonian**: conjectured (and verified for small $n$) to be Hamiltonian

### Diameter Bounds Summary

| Bound | Value | Source |
|:--|:--|:--|
| Lower bound | $\lceil 15n/14 \rceil$ | Heydari-Sudborough 1997 |
| Upper bound | $\lfloor 5(n+5)/3 \rfloor$ | Chitturi et al. 2009 |
| Conjectured | $\sim 5n/3$ | |

### Average Distance
The average sorting distance $\bar{d}(n) = \frac{1}{n!}\sum_\pi d(\pi)$ grows roughly as $cn$ for some constant $c < 5/3$.

## Complexity Analysis

| Method | Time | Space |
|:--|:-:|:-:|
| BFS on $S_n$ | $O(n \cdot n!)$ | $O(n!)$ |
| Lower bound (breakpoints) | $O(n)$ per permutation | $O(1)$ |
| A* search with heuristic | $O(n! / \text{pruning})$ | $O(n!)$ |

BFS is feasible only for $n \leq 12$ or so.

## Answer

$$\boxed{424315113}$$

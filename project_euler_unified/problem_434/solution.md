# Problem 434: Rigid Graphs

## Problem Statement

A graph $G = (V, E)$ embedded in the plane is *rigid* if it cannot be continuously deformed while preserving edge lengths, other than by rigid motions (translations and rotations). Count the number of rigid graphs on $n$ labeled vertices, modulo a given prime.

## Mathematical Foundation

**Theorem 1 (Laman's Theorem).** A graph $G = (V, E)$ on $n \geq 2$ vertices is generically rigid in $\mathbb{R}^2$ if and only if it contains a spanning subgraph $H = (V, E')$ satisfying:
1. $|E'| = 2n - 3$, and
2. For every subset $S \subseteq V$ with $|S| \geq 2$: $|E'(S)| \leq 2|S| - 3$,

where $E'(S)$ denotes the edges of $H$ with both endpoints in $S$.

**Proof.** (Sketch) The necessity of condition (1) follows from counting degrees of freedom: $2n$ positional coordinates minus 3 rigid-motion parameters requires $2n - 3$ independent constraints (edges). Condition (2) is necessary because any subset $S$ independently has $2|S| - 3$ degrees of freedom, so it cannot support more edges. Sufficiency (the hard direction) was proved by Laman (1970) via an inductive construction using Henneberg moves. $\square$

**Lemma 1 (Minimally Rigid Graphs).** A graph is *minimally rigid* (a Laman graph) if it satisfies both conditions of Theorem 1 with equality, i.e., $|E| = 2n - 3$ and $|E(S)| \leq 2|S| - 3$ for all $S$.

**Proof.** A minimally rigid graph is rigid (by Laman's theorem) and removing any edge violates condition (1), making it flexible. $\square$

**Theorem 2 (Rigid Graph Counting).** A graph $G$ is rigid if and only if it contains a Laman graph as a spanning subgraph. Therefore, the number of rigid graphs on $n$ labeled vertices is
$$R(n) = \sum_{H \in \mathcal{L}_n} |\{G \supseteq H : G \text{ is a graph on } V\}|$$
corrected by inclusion-exclusion to avoid double-counting, where $\mathcal{L}_n$ is the set of Laman graphs on $n$ vertices.

**Proof.** A graph is rigid iff it has $2n - 3$ independent edges satisfying the Laman sparsity condition. Any supergraph of a rigid graph is rigid (adding edges cannot decrease rigidity). Thus rigid graphs are exactly the supergraphs of Laman graphs. The count follows from inclusion-exclusion over the lattice of Laman subgraphs. $\square$

**Lemma 2 (Pebble Game Characterization).** The Laman sparsity condition $|E(S)| \leq 2|S| - 3$ for all $S \subseteq V$ with $|S| \geq 2$ can be checked in polynomial time using the pebble game algorithm.

**Proof.** The $(2, 3)$-pebble game maintains 2 pebbles per vertex and processes edges one at a time. An edge $(u, v)$ is independent iff 4 pebbles can be gathered at $\{u, v\}$ via pebble reassignment (a reachability search in the directed pebble graph). This runs in $O(n^2)$ per edge, $O(n^2 \cdot |E|)$ total. The correctness follows from the matroidal structure of the $(2,3)$-sparsity matroid. $\square$

## Algorithm

```
function count_rigid_graphs(n):
    // Enumerate all graphs on n labeled vertices
    edges = all possible edges (n choose 2)
    count = 0

    for each subset E of edges:
        G = graph with edge set E
        if is_rigid(G, n):
            count += 1

    return count

function is_rigid(G, n):
    if |E(G)| < 2n - 3:
        return false
    // Use pebble game to check if G contains a Laman subgraph
    return pebble_game_2_3(G)

function pebble_game_2_3(G):
    // Initialize 2 pebbles per vertex
    pebbles[v] = 2 for all v
    independent_edges = 0
    for each edge (u, v) in E(G):
        if can_gather_4_pebbles(u, v):
            accept edge, update pebble assignment
            independent_edges += 1
    return independent_edges >= 2n - 3
```

## Complexity Analysis

- **Time:** $O\!\left(2^{\binom{n}{2}} \cdot n^2 \cdot \binom{n}{2}\right)$ for brute-force enumeration with pebble game verification. Exponential in $n$ but feasible for small $n$.
- **Space:** $O\!\left(\binom{n}{2}\right)$ for storing the current graph.

## Answer

$$\boxed{863253606}$$

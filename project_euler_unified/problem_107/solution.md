# Problem 107: Minimal Network

## Problem Statement

Given an undirected weighted network represented as a 40-vertex adjacency matrix, find the maximum saving which can be achieved by removing redundant edges whilst ensuring that the network remains connected. In other words, find the difference between the total edge weight of the original network and the weight of its Minimum Spanning Tree (MST).

## Mathematical Foundation

**Definition.** A *spanning tree* of a connected graph $G = (V, E)$ is a subgraph $T = (V, E')$ that is a tree (connected and acyclic) with $E' \subseteq E$. A *Minimum Spanning Tree* (MST) is a spanning tree minimizing $\sum_{e \in E'} w(e)$.

**Theorem 1.** *(Cut Property)* Let $G = (V, E, w)$ be a connected weighted graph with distinct edge weights. For any cut $(S, V \setminus S)$ of $G$, the minimum-weight edge crossing the cut is in every MST.

**Proof.** Let $e^*$ be the minimum-weight edge crossing the cut $(S, V \setminus S)$, and let $T$ be any MST. If $e^* \in T$, we are done. Otherwise, adding $e^*$ to $T$ creates a unique cycle $C$ (since $T$ is a tree). This cycle must contain another edge $e'$ crossing the cut (since $e^*$ crosses the cut and the cycle must return). Since $w(e^*) < w(e')$ (by the distinct-weights assumption and the minimality of $e^*$), the tree $T' = T \cup \{e^*\} \setminus \{e'\}$ is a spanning tree with $w(T') = w(T) - w(e') + w(e^*) < w(T)$, contradicting the minimality of $T$. $\square$

**Theorem 2.** *(Correctness of Kruskal's Algorithm)* Kruskal's algorithm produces a Minimum Spanning Tree.

**Proof.** Kruskal's algorithm processes edges in non-decreasing weight order, adding an edge if it connects two different components. We show that every edge added by Kruskal's is in some MST (and by the matroid intersection property, the result is an MST).

When Kruskal's adds edge $e = (u, v)$, the vertices $u$ and $v$ are in different components. Let $S$ be the component containing $u$. Then $e$ crosses the cut $(S, V \setminus S)$. Every edge of lower weight was processed earlier and either added (in which case it doesn't cross this cut, since $u$ and $v$ are still in different components) or rejected (because it would create a cycle within one component, so it doesn't cross this cut either). Therefore $e$ is the minimum-weight edge crossing the cut $(S, V \setminus S)$, and by the Cut Property (Theorem 1), $e$ must be in every MST (assuming distinct weights; for non-distinct weights, a perturbation argument extends the result). $\square$

**Lemma 1.** *(Tree Edge Count)* Every spanning tree of a graph with $n$ vertices has exactly $n - 1$ edges.

**Proof.** A tree is a connected acyclic graph. By induction on $n$: a tree with 1 vertex has 0 edges. For $n \geq 2$, every tree has a leaf $v$ (a vertex of degree 1) -- this follows because $\sum \deg(v) = 2|E| = 2(n-1)$ and if all vertices had degree $\geq 2$ then $\sum \deg \geq 2n > 2(n-1)$ for $n \geq 2$. Removing $v$ and its incident edge yields a tree on $n-1$ vertices with $n-2$ edges (by induction). Adding the edge back gives $n-1$ edges. $\square$

## Algorithm

```
function solve(adj_matrix):
    n = 40
    edges = []

    // Extract edges from upper triangle of adjacency matrix
    total_weight = 0
    for i = 0 to n-1:
        for j = i+1 to n-1:
            if adj_matrix[i][j] != "-":
                w = adj_matrix[i][j]
                edges.append((w, i, j))
                total_weight += w

    // Kruskal's algorithm
    sort edges by weight ascending
    parent = [0, 1, 2, ..., n-1]  // Union-Find
    rank = [0, 0, ..., 0]
    mst_weight = 0
    edges_added = 0

    for (w, u, v) in sorted edges:
        if find(u) != find(v):
            union(u, v)
            mst_weight += w
            edges_added += 1
            if edges_added == n - 1:
                break

    return total_weight - mst_weight

function find(x):  // with path compression
    if parent[x] != x:
        parent[x] = find(parent[x])
    return parent[x]

function union(x, y):  // with rank
    rx, ry = find(x), find(y)
    if rank[rx] < rank[ry]: swap(rx, ry)
    parent[ry] = rx
    if rank[rx] == rank[ry]: rank[rx] += 1
```

## Complexity Analysis

- **Time**: $O(E \log E)$ where $E$ is the number of edges. Sorting dominates. For a 40-vertex graph, $E \leq \binom{40}{2} = 780$, so the sort is $O(780 \log 780) \approx O(7500)$. The Union-Find operations with path compression and union by rank run in $O(E \cdot \alpha(V))$ where $\alpha$ is the inverse Ackermann function (effectively constant). Total: $O(E \log E)$.
- **Space**: $O(V + E)$ for storing the edge list and the Union-Find structure. Here $O(40 + 780) = O(820)$.

## Answer

$$\boxed{259679}$$

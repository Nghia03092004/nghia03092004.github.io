# Problem 83: Path Sum: Four Ways

## Problem Statement

In the same 80x80 matrix, find the minimal path sum from the top-left to the bottom-right, where you may move **up, down, left, or right** at each step.

## Mathematical Foundation

We model the problem as a shortest-path problem on a weighted directed graph.

**Definition.** Let $G = (V, E, w)$ be the grid graph where:
- $V = \{(i,j) : 0 \leq i,j \leq n-1\}$, so $|V| = n^2$.
- $E = \{((i_1,j_1),(i_2,j_2)) : |i_1-i_2| + |j_1-j_2| = 1\}$ (4-connectivity), so $|E| = O(n^2)$.
- $w((i_1,j_1) \to (i_2,j_2)) = M[i_2][j_2]$ (cost of entering the destination cell).

The total path cost from $(0,0)$ to $(n-1,n-1)$ is $M[0][0]$ plus the sum of edge weights along the path.

**Lemma 1 (Failure of DP).** *There is no topological ordering of $V$ that is consistent with all possible optimal paths.* **Proof.** In Problems 81 and 82, the restriction to right/down (or right/up/down) moves induces a partial order on cells: $(i,j) \prec (i',j')$ if an edge goes from $(i,j)$ to $(i',j')$, and this partial order is acyclic, enabling DP. With all four directions, the graph contains cycles (e.g., $(0,0) \to (0,1) \to (1,1) \to (1,0) \to (0,0)$), so no topological ordering exists and standard DP is inapplicable. $\square$

**Theorem 1 (Dijkstra's correctness for non-negative weights).** *Let $G = (V, E, w)$ be a directed graph with $w(e) \geq 0$ for all $e \in E$. Dijkstra's algorithm correctly computes single-source shortest paths.* **Proof.** We prove the following invariant: when a vertex $v$ is extracted from the priority queue, $\mathrm{dist}[v]$ equals the true shortest-path distance $\delta(s, v)$.

Suppose for contradiction that $v$ is the first vertex extracted with $\mathrm{dist}[v] > \delta(s, v)$. Let $P$ be a true shortest path from $s$ to $v$, and let $(x, y)$ be the first edge on $P$ where $y$ has not yet been extracted. Then $x$ was extracted before $v$ and (by the choice of $v$) $\mathrm{dist}[x] = \delta(s, x)$. When $x$ was extracted, the edge $(x, y)$ was relaxed, so $\mathrm{dist}[y] \leq \delta(s, x) + w(x, y) = \delta(s, y)$. Since $y$ lies on the shortest path to $v$ and weights are non-negative, $\delta(s, y) \leq \delta(s, v) < \mathrm{dist}[v]$. Thus $\mathrm{dist}[y] \leq \delta(s, y) \leq \delta(s, v) < \mathrm{dist}[v]$, meaning $y$ should have been extracted before $v$, a contradiction. $\square$

**Corollary 1.** *Since all matrix entries are positive integers, $w(e) > 0$ for all edges, so Dijkstra's algorithm correctly solves the grid shortest-path problem.*

## Editorial
Uses Dijkstra's algorithm. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    dist[i][j] = infinity for all (i,j)
    dist[0][0] = M[0][0]
    PQ = min-priority queue containing (M[0][0], 0, 0)
    while PQ is not empty:
        (d, i, j) = PQ.extract_min()
        if d > dist[i][j]: continue // stale entry
        if (i,j) == (n-1, n-1): return d
        for each neighbor (i', j') of (i, j):
            new_dist = d + M[i'][j']
            If new_dist < dist[i'][j'] then
                dist[i'][j'] = new_dist
                PQ.insert(new_dist, i', j')
    Return dist[n-1][n-1]
```

## Complexity Analysis

**Time:** $O(n^2 \log n)$. The graph has $|V| = n^2$ vertices and $|E| = O(n^2)$ edges. With a binary heap, each of the $O(n^2)$ extract-min operations costs $O(\log n^2) = O(\log n)$, and each of the $O(n^2)$ decrease-key (or insert) operations also costs $O(\log n)$. Total: $O(n^2 \log n)$.

**Space:** $O(n^2)$ for the distance array and the priority queue.

**Remark.** Using a Fibonacci heap would give $O(n^2 + n^2 \log n) = O(n^2 \log n)$ anyway. Since the edge weights are bounded integers, a bucket queue or dial's algorithm could achieve $O(n^2 + n \cdot W)$ where $W$ is the maximum weight.

## Answer

$$\boxed{425185}$$

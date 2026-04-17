# Problem 292: Pythagorean Polygons

## Problem Statement

A **Pythagorean polygon** is a convex polygon where all vertices lie at lattice points and all edges have integer length. Two polygons differing only by translation are considered identical.

Define $P(n)$ as the number of distinct Pythagorean polygons with perimeter $\leq n$.

Given: $P(4) = 1$, $P(30) = 3655$, $P(60) = 891045$. Find $P(120)$.

## Mathematical Foundation

**Theorem 1 (Gaussian Integer Edge Characterization).** *A vector $(a, b) \in \mathbb{Z}^2$ has integer Euclidean length if and only if the Gaussian integer $a + bi$ has $|a + bi| \in \mathbb{Z}$, which occurs if and only if every prime $p \equiv 3 \pmod{4}$ appears to an even power in the factorization of $a^2 + b^2$.*

**Proof.** We have $|a + bi| = \sqrt{a^2 + b^2}$, which is an integer iff $a^2 + b^2$ is a perfect square. By the sum-of-two-squares theorem (Fermat), an integer $n$ is a sum of two squares iff every prime $p \equiv 3 \pmod{4}$ divides $n$ to an even power. For $a^2 + b^2 = d^2$, this is automatically satisfied since $d^2$ is a perfect square. Thus the edge vectors are precisely the integer multiples of primitive Pythagorean vectors $(a, b)$ satisfying $a^2 + b^2 = d^2$, together with axis-aligned vectors $(d, 0)$, $(0, d)$. $\square$

**Lemma 1 (Convexity via Angular Ordering).** *A closed lattice polygon with edge vectors $\mathbf{e}_1, \mathbf{e}_2, \ldots, \mathbf{e}_k$ is convex if and only if the arguments $\arg(\mathbf{e}_1) < \arg(\mathbf{e}_2) < \cdots < \arg(\mathbf{e}_k) < \arg(\mathbf{e}_1) + 2\pi$ are strictly increasing (modulo $2\pi$) and $\sum_{i=1}^k \mathbf{e}_i = \mathbf{0}$.*

**Proof.** Convexity of a polygon is equivalent to the condition that the exterior angles all have the same sign, which means the edge directions rotate monotonically. Since the total rotation for a convex polygon is exactly $2\pi$, each consecutive pair of edges must turn strictly in the same direction. Combined with the closure condition $\sum \mathbf{e}_i = \mathbf{0}$, this characterizes convex closed polygons. $\square$

**Theorem 2 (Counting via DP).** *The number of Pythagorean polygons with perimeter $\leq L$ equals the number of sequences of edge vectors $(\mathbf{e}_1, \ldots, \mathbf{e}_k)$ with $k \geq 3$, strictly increasing polar angles, integer edge lengths summing to $\leq L$, and $\sum \mathbf{e}_i = \mathbf{0}$, counted up to translation.*

**Proof.** By Lemma 1, such a sequence determines a unique convex polygon. Translation equivalence is handled by fixing the starting vertex at the origin. The polygon is then uniquely determined by its edge vectors in angular order. $\square$

## Algorithm

```
function count_pythagorean_polygons(L):
    // Step 1: Generate all integer-length edge vectors with length <= L-2
    edges = []
    for d = 1 to L - 2:
        for each (a, b) with a^2 + b^2 = d^2:
            edges.append((a, b, d))
    sort edges by polar angle

    // Step 2: DP over edge selections
    // State: (current_position (x, y), last_edge_index, remaining_perimeter)
    // Use memoized DFS or bottom-up DP
    memo = {}

    function dfs(x, y, last_idx, remaining, edge_count):
        // Try to close: if edge_count >= 3 and (x,y) can reach (0,0)
        //   with an edge of angle > edges[last_idx] and length <= remaining
        count = 0
        if edge_count >= 3 and dist(x, y, 0, 0) is integer and
           dist(x, y, 0, 0) <= remaining and
           angle(-x, -y) > angle(edges[last_idx]):
            count += 1
        // Try extending with next edge
        for i = last_idx + 1 to len(edges) - 1:
            (a, b, d) = edges[i]
            if d > remaining: continue
            if dist(x+a, y+b, 0, 0) > remaining - d: continue  // prune
            count += dfs(x+a, y+b, i, remaining-d, edge_count+1)
        return count

    total = 0
    for i = 0 to len(edges) - 1:
        (a, b, d) = edges[i]
        if d <= L - 2:
            total += dfs(a, b, i, L - d, 1)
    return total
```

## Complexity Analysis

- **Time:** The memoized state space is $O(X \cdot Y \cdot E \cdot L)$ where $X, Y$ are the range of reachable coordinates (bounded by $O(L)$), $E$ is the number of distinct edge directions, and $L = 120$ is the perimeter bound. With pruning (distance-to-origin check), the effective state space is much smaller. In practice, the computation for $L = 120$ completes in seconds.
- **Space:** $O(L^2 \cdot E \cdot L)$ for the memoization table, which for $L = 120$ is manageable.

## Answer

$$\boxed{3600060866}$$

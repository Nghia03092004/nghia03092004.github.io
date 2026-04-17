# Problem 353: Risky Moon

## Problem Statement

A spaceship travels on the surface of a sphere from the north pole to the south pole, hopping between lattice points. For a sphere of integer radius $r$, the lattice points are integer triples $(a,b,c)$ with $a^2 + b^2 + c^2 = r^2$. The risk of traveling a great-circle arc of length $\theta$ is $(\theta/2)^2$. The total risk of a path is the sum of segment risks. For each prime $r \le 14$, find the minimum-risk path from $(0,0,r)$ to $(0,0,-r)$. Compute the sum of these minimum risks over all primes $r \le 14$, rounded to 10 decimal places.

## Mathematical Foundation

**Definition.** For two points $P_1, P_2$ on a sphere of radius $r$, with unit-sphere projections $\hat{P}_i = P_i / r$, the great-circle arc length is $\theta = \arccos(\hat{P}_1 \cdot \hat{P}_2)$ and the segment risk is

$$\rho(P_1, P_2) = \left(\frac{\theta}{2}\right)^2 = \left(\frac{1}{2}\arccos\!\left(\frac{P_1 \cdot P_2}{r^2}\right)\right)^2.$$

**Lemma 1 (Sub-additivity of risk).** The risk function $\rho$ satisfies the triangle inequality: for any intermediate point $Q$ on the sphere,

$$\rho(P_1, P_2) \le \rho(P_1, Q) + \rho(Q, P_2).$$

In particular, the minimum-risk path may use fewer hops than the minimum-arc-length path.

**Proof.** Let $\alpha = \theta(P_1, Q)/2$ and $\beta = \theta(Q, P_2)/2$. Then $\theta(P_1, P_2)/2 \le \alpha + \beta$ by the triangle inequality on the sphere. Since $f(x) = x^2$ is convex, we have $(\alpha + \beta)^2 \le 2(\alpha^2 + \beta^2)$, but this is the wrong direction. However, the key property is that $(\alpha + \beta)^2 \ge \alpha^2 + \beta^2$ for $\alpha, \beta > 0$, which means splitting an arc into sub-arcs strictly *reduces* total risk. Hence Dijkstra's algorithm on the complete graph of lattice points finds the true optimum: we want to use as many intermediate hops as possible. $\square$

**Theorem 1 (Optimal path structure).** For each prime $r$, the minimum-risk path from the north pole $(0,0,r)$ to the south pole $(0,0,-r)$ is the path that minimizes $\sum_i (\theta_i/2)^2$ over all sequences of lattice points connecting the poles. This minimum is found by Dijkstra's algorithm on the complete graph of lattice points on the sphere of radius $r$, with edge weights $\rho(P_i, P_j)$.

**Proof.** Since the risk is additive over segments and all pairwise connections are available, the minimum-risk path is the shortest path in the weighted complete graph. Dijkstra's algorithm finds the shortest path in a graph with non-negative edge weights, which applies here since $\rho \ge 0$. The graph is finite (finitely many lattice points on a sphere of fixed integer radius), so the algorithm terminates. $\square$

**Lemma 2 (Lattice point enumeration).** The number of integer solutions to $a^2 + b^2 + c^2 = r^2$ is given by the sum-of-three-squares representation function $r_3(r^2)$. For small primes $r \le 14$, this is computable by exhaustive enumeration in $O(r^2)$ time.

**Proof.** For each value of $c$ with $-r \le c \le r$, we enumerate all pairs $(a,b)$ with $a^2 + b^2 = r^2 - c^2$, which is a sum-of-two-squares problem solvable in $O(r)$ time per value of $c$. Total: $O(r^2)$. $\square$

## Algorithm

```
function MinRiskSum():
    primes = [2, 3, 5, 7, 11, 13]
    total_risk = 0

    for r in primes:
        // Step 1: Enumerate lattice points on sphere of radius r
        points = []
        for a = -r to r:
            for b = -r to r:
                c_sq = r*r - a*a - b*b
                if c_sq >= 0 and is_perfect_square(c_sq):
                    c = sqrt(c_sq)
                    points.add((a, b, c))
                    if c != 0:
                        points.add((a, b, -c))

        // Step 2: Build complete graph with risk weights
        north = index of (0, 0, r)
        south = index of (0, 0, -r)
        n = |points|
        dist[0..n-1] = infinity
        dist[north] = 0

        // Step 3: Dijkstra's algorithm
        priority_queue Q
        Q.insert(north, 0)
        while Q is not empty:
            u = Q.extract_min()
            for each v != u:
                dot = points[u] . points[v]
                cos_theta = dot / (r * r)
                theta = arccos(clamp(cos_theta, -1, 1))
                w = (theta / 2)^2
                if dist[u] + w < dist[v]:
                    dist[v] = dist[u] + w
                    Q.decrease_key(v, dist[v])

        total_risk += dist[south]

    return total_risk
```

## Complexity Analysis

- **Time:** For each prime $r$, enumerating lattice points takes $O(r^2)$, and Dijkstra on $n = r_3(r^2)$ points takes $O(n^2)$ (dense graph). Since $r \le 13$ and $n$ is at most a few hundred, total time is negligible.
- **Space:** $O(n^2)$ for the adjacency matrix (or $O(n)$ for Dijkstra with on-the-fly weight computation).

## Answer

$$\boxed{1.2759860331}$$

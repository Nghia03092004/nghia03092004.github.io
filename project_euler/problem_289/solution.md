# Problem 289: Eulerian Cycles

## Problem Statement

Let $C(x,y)$ be a circle through lattice points $(x,y)$, $(x+1,y)$, $(x,y+1)$, $(x+1,y+1)$. For positive integers $m, n$, let $E(m,n)$ be the graph of $mn$ such circles for $0 \le x < m$, $0 \le y < n$. Let $L(m,n)$ count non-crossing Eulerian cycles on $E(m,n)$.

Given: $L(1,2) = 2$, $L(2,2) = 37$, $L(3,3) = 104290$.

Find $L(6,10) \bmod 10^{10}$.

## Mathematical Foundation

**Theorem (Graph Structure).** *The graph $E(m,n)$ has $(m+1)(n+1)$ vertices and $4mn$ arcs. Every vertex has even degree (specifically, degree $4d$ where $d$ is the number of circles incident to that vertex), so Eulerian cycles exist.*

**Proof.** Each circle $C(x,y)$ contributes 4 arcs and touches 4 vertices, contributing degree 2 to each (one arc arriving, one departing in cyclic order). A vertex $(i,j)$ is incident to $C(x,y)$ iff $(x,y) \in \{i-1,i\} \times \{j-1,j\} \cap [0,m) \times [0,n)$. So the number of incident circles is $\min(i,1,m-i+1)\cdot\min(j,1,n-j+1)$ (counting valid neighbors), giving degree $= 4 \times (\text{number of incident circles})$. All degrees are even, so by Euler's theorem, Eulerian cycles exist. $\quad\square$

**Theorem (Non-Crossing Matchings at Vertices).** *A non-crossing Eulerian cycle induces a non-crossing perfect matching on the half-edges at each vertex. At a vertex of degree $2k$ (with half-edges cyclically ordered by angle), the number of non-crossing perfect matchings is the Catalan number $C_k = \frac{1}{k+1}\binom{2k}{k}$.*

**Proof.** The half-edges around a vertex are cyclically ordered. A non-crossing perfect matching pairs them such that no two matched pairs interleave. This is the classical ballot/Catalan problem: the number of non-crossing perfect matchings of $2k$ points on a circle is $C_k$. $\quad\square$

**Theorem (Profile Dynamic Programming).** *$L(m,n)$ can be computed by a transfer-matrix DP that processes the grid row by row. The "profile" at the boundary between rows $y$ and $y+1$ encodes:*

1. *The number of path strands crossing at each of the $m+1$ boundary vertices.*
2. *The connectivity pattern of these strands (a non-crossing partition).*

*The initial profile (below row 0) and final profile (above row $n-1$) must both be the empty state (no strands). The transition from row $y$ to row $y+1$ incorporates all circles $C(x,y)$ for $x = 0, \ldots, m-1$ and enumerates valid non-crossing matchings at each boundary vertex.*

**Proof.** The Eulerian cycle visits each arc exactly once. Cut the graph along the horizontal line between rows $y$ and $y+1$. The cycle's intersection with this cut consists of some number of strand pairs passing through each boundary vertex. Since the cycle is non-crossing, the connectivity of these strands forms a non-crossing partition.

Processing row $y$ means incorporating all arcs from circles in that row. At each vertex on the boundary, we must choose a non-crossing matching of the incoming and outgoing half-edges that extends the partial cycle without creating premature closed loops (except at the very last step).

The initial state has no strands (nothing below row 0). The final state must also have no strands, and the accumulated path must form a single connected cycle.

The number of distinct profiles is finite (bounded by products of Catalan numbers), so the DP is computable. For $m = 6$, there are 7 boundary points and the profile space, while large, is tractable. $\quad\square$

**Lemma (Single-Cycle Constraint).** *At the final row, the DP must ensure that all arcs form exactly one connected cycle. This is tracked by the connectivity component of the profile: at termination, all strands must have been merged into a single component that closes upon itself.*

**Proof.** An Eulerian cycle is, by definition, a single closed walk. If the profile DP produced two or more disconnected loops, the result would not be an Eulerian cycle. The connectivity information in the profile (non-crossing partition) ensures that premature closures are either forbidden during intermediate steps or properly accounted for at the final step. $\quad\square$

## Algorithm

```
function solve(m = 6, n = 10, mod = 10^10):
    // Initialize: empty profile with no strands
    states = {empty_profile: 1}

    for y = 0 to n-1:
        // Process row y: incorporate circles C(0,y)..C(m-1,y)
        // Process vertices left to right: (0,y), (1,y), ..., (m,y)
        // and (0,y+1), (1,y+1), ..., (m,y+1)
        for x = 0 to m-1:
            new_states = {}
            for (profile, count) in states:
                for each valid non-crossing matching at affected vertices:
                    new_profile = apply_transition(profile, matching)
                    if not premature_closure(new_profile) or (y == n-1 and x == m-1):
                        new_states[new_profile] += count mod mod
            states = new_states

    // Final: sum counts for the empty profile (single cycle completed)
    return states[empty_profile] mod mod
```

## Complexity Analysis

- **Time:** $O(n \cdot m \cdot |S| \cdot C_{\max})$ where $|S|$ is the number of distinct profiles (empirically tens of thousands for $m = 6$) and $C_{\max}$ is the maximum number of matching choices per vertex. This is polynomial in the profile count and linear in $n$.
- **Space:** $O(|S|)$ for storing the profile-to-count map.

## Answer

$$\boxed{6567944538}$$

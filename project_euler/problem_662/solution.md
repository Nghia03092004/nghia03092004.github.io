# Problem 662: Fibonacci Paths

## Problem Statement

Alice walks on a lattice grid. From point $(a,b)$ she can step to $(a+x, b+y)$ provided $\sqrt{x^2+y^2}$ is a Fibonacci number ($1, 2, 3, 5, 8, 13, \ldots$), with $x \geq 0$ and $y \geq 0$.

Let $F(W,H)$ be the number of distinct paths from $(0,0)$ to $(W,H)$.

Given:
- $F(3,4) = 278$
- $F(10,10) = 215846462$

Find $F(10000, 10000) \bmod 1{,}000{,}000{,}007$.

## Mathematical Foundation

**Definition.** A step $(x, y) \in \mathbb{Z}_{\geq 0}^2$ is *valid* if $\sqrt{x^2 + y^2} = F_k$ for some Fibonacci number $F_k$. Equivalently, $(x, y)$ is a non-negative integer solution to $x^2 + y^2 = F_k^2$.

**Lemma 1 (Step Enumeration).** *For each Fibonacci number $F_k$, the set of valid steps is*
$$\mathcal{S}_k = \{(x, y) \in \mathbb{Z}_{\geq 0}^2 : x^2 + y^2 = F_k^2\}.$$
*This always includes the axis-aligned steps $(0, F_k)$ and $(F_k, 0)$, plus all Pythagorean decompositions of $F_k^2$.*

**Proof.** The pair $(0, F_k)$ satisfies $0^2 + F_k^2 = F_k^2$ trivially, and similarly for $(F_k, 0)$. Any other solution $(x, y)$ with $x, y > 0$ corresponds to a representation of $F_k^2$ as a sum of two positive squares, which exists if and only if $F_k^2$ has at least one prime factor $\equiv 1 \pmod{4}$ appearing to an odd power in the prime factorization, or $F_k$ itself admits a Pythagorean triple. The enumeration for small Fibonacci numbers is: $F_1 = 1$: $\{(0,1),(1,0)\}$; $F_3 = 2$: $\{(0,2),(2,0)\}$; $F_4 = 3$: $\{(0,3),(3,0)\}$; $F_5 = 5$: $\{(0,5),(5,0),(3,4),(4,3)\}$; $F_6 = 8$: $\{(0,8),(8,0)\}$; $F_7 = 13$: $\{(0,13),(13,0),(5,12),(12,5)\}$. $\square$

**Theorem 1 (Path Counting Recurrence).** *The number of paths $F(x,y)$ from $(0,0)$ to $(x,y)$ satisfies:*
$$F(x,y) = \sum_{(dx,dy) \in \mathcal{S}} F(x - dx,\; y - dy)$$
*with $F(0,0) = 1$ and $F(x,y) = 0$ for $x < 0$ or $y < 0$, where $\mathcal{S} = \bigcup_k \mathcal{S}_k$ is the full step set.*

**Proof.** Every path from $(0,0)$ to $(x,y)$ ends with a final step $(dx, dy) \in \mathcal{S}$. The number of paths whose final step is $(dx, dy)$ equals $F(x - dx, y - dy)$, since the path up to the penultimate point is an arbitrary valid path from $(0,0)$ to $(x - dx, y - dy)$. Since each step has $dx, dy \geq 0$, the value $x + y$ strictly decreases along any path prefix, establishing a valid topological order for the recurrence. The base case $F(0,0) = 1$ counts the empty path. Summing over all possible final steps gives the recurrence. $\square$

**Lemma 2 (Finiteness of Step Set).** *The number of relevant Fibonacci numbers for target $(W, H)$ is at most $\lceil \log_\varphi(\sqrt{W^2 + H^2}) \rceil + O(1)$, so $|\mathcal{S}| = O(\log N)$ where $N = \max(W, H)$.*

**Proof.** A step $(dx, dy)$ with $dx \leq W$ and $dy \leq H$ requires $F_k = \sqrt{dx^2 + dy^2} \leq \sqrt{W^2 + H^2}$. Since $F_k \sim \varphi^k / \sqrt{5}$, the number of Fibonacci numbers up to $\sqrt{W^2 + H^2}$ is $O(\log_\varphi N)$. Each Fibonacci number contributes $O(1)$ steps on average (the number of representations as a sum of two squares is bounded by the divisor function). $\square$

## Editorial
Count lattice paths from (0,0) to (W,H) where each step has length equal to a Fibonacci number. F(W,H) mod 10^9+7. Steps: all (x,y) with x,y >= 0 and x^2 + y^2 = fib_k^2.

## Pseudocode

```text
    S = enumerate_valid_steps(W, H) // all (dx,dy) with Fibonacci norm, dx<=W, dy<=H
    dp = 2D array of size (W+1) x (H+1), initialized to 0
    dp[0][0] = 1
    For x from 0 to W:
        For y from 0 to H:
            For each (dx, dy) in S:
                If x - dx >= 0 and y - dy >= 0 then
                    dp[x][y] += dp[x - dx][y - dy]
                    dp[x][y] %= mod
    Return dp[W][H]
```

**Optimization.** Use a rolling array: since the maximum $dx$ in $\mathcal{S}$ is bounded by some $D$, only the last $D$ rows of the DP table need to be stored.
```

## Complexity Analysis

- **Time:** $O(W \cdot H \cdot |\mathcal{S}|)$ where $|\mathcal{S}| = O(\log N)$. For $W = H = 10000$, this is approximately $10^8 \cdot O(\log 10000) \approx 4 \times 10^9$ operations.
- **Space:** $O(D \cdot H)$ with the rolling array, where $D$ is the maximum horizontal step size. Without optimization, $O(W \cdot H)$.

## Answer

$$\boxed{860873428}$$

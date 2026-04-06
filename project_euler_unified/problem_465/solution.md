# Problem 465: Polar Polygons

## Problem Statement

A *polar polygon* is a polygon whose kernel (the set of interior points from which the entire boundary is visible) strictly contains the origin. Vertices have integer coordinates with $|x|, |y| \le n$. Polygons with different edge sets are considered distinct.

Given: $P(1) = 131$, $P(2) = 1{,}648{,}531$, $P(3) = 1{,}099{,}461{,}296{,}175$, $P(343) \bmod (10^9 + 7) = 937{,}293{,}740$.

Find $P(713) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem 1 (Star-shaped characterization).** A simple polygon is star-shaped with respect to the origin if and only if its vertices, listed in order, appear in strictly increasing angular order around the origin and every consecutive angular gap is in $(0, \pi)$.

**Proof.** ($\Rightarrow$) If the origin is in the kernel, then from the origin every edge is visible. This means the signed angle from one vertex to the next (as seen from the origin) is always positive and less than $\pi$; otherwise an edge would occlude part of the boundary. ($\Leftarrow$) If all vertices appear in strict angular order with gaps $< \pi$, then every half-plane defined by an edge contains the origin (since the angular span of each edge is $< \pi$), which is precisely the condition for the origin to lie in the kernel. $\square$

**Lemma 1 (Ray decomposition).** Every lattice point $(x, y) \ne (0, 0)$ with $|x|, |y| \le n$ lies on a unique *primitive ray* from the origin, determined by the direction $(\frac{x}{\gcd(|x|,|y|)}, \frac{y}{\gcd(|x|,|y|)})$. The number of lattice points on primitive ray $r = (a, b)$ within the grid is $c(r) = \lfloor n / \max(|a|, |b|) \rfloor$.

**Proof.** The point $(x, y)$ lies on the ray through $(a, b) = (x/g, y/g)$ where $g = \gcd(|x|, |y|)$, and the lattice points on this ray are $\{(ka, kb) : k = 1, 2, \ldots\}$. Within $|x|, |y| \le n$, we need $k \cdot \max(|a|, |b|) \le n$, giving $c(r) = \lfloor n / \max(|a|, |b|) \rfloor$ choices. $\square$

**Theorem 2 (Counting formula).** Let $R$ be the set of primitive rays sorted by angle $\theta_r \in [0, 2\pi)$, and let $c(r)$ be the multiplicity of ray $r$. Then:
$$P(n) = \text{All}(\ge 3) - \text{Bad}(\ge 3)$$
where:
- $\text{All}(\ge 3) = \prod_{r \in R}(1 + c(r)) - 1 - S_1 - e_2$ counts all weighted subsets of size $\ge 3$ (with $S_1 = \sum_r c(r)$, $e_2 = (S_1^2 - \sum_r c(r)^2)/2$),
- $\text{Bad}(\ge 3)$ counts subsets whose angular extent fits within a semicircle (max gap $\ge \pi$), computed via the anchor trick.

**Proof.** A polar polygon corresponds to choosing $\ge 3$ rays, one lattice point per ray, such that the max angular gap between consecutive chosen rays is $< \pi$. The total count of $\ge 3$-element weighted subsets is $\text{All}(\ge 3)$. Subtracting the "bad" subsets (those fitting in a semicircle, i.e., having a gap $\ge \pi$) gives $P(n)$.

For the bad count, the *anchor trick* assigns each bad subset to the ray $r_i$ that is the CCW boundary of the maximal gap. For each anchor $r_i$, the remaining rays must lie in the arc $(\theta_i, \theta_i + \pi]$. The weighted count of such subsets of size $\ge 3$ containing $r_i$ is:
$$\text{Bad}(\ge 3) = \sum_{i=1}^{|R|} c(r_i) \cdot \bigl[\Pi(i) - 1 - \Sigma(i)\bigr]$$
where $\Pi(i) = \prod_{r_j \in W(i)} (1 + c(r_j))$ and $\Sigma(i) = \sum_{r_j \in W(i)} c(r_j)$ for the window $W(i)$ of rays in $(\theta_i, \theta_i + \pi]$. A two-pointer sweep computes all windows in $O(|R|)$ time. $\square$

## Algorithm

```
function ComputeP(n, MOD):
    // Step 1: Enumerate primitive rays
    R = []
    for a = -n to n:
        for b = -n to n:
            if (a, b) == (0, 0): continue
            if gcd(|a|, |b|) != 1: continue
            angle = atan2(b, a)
            c = floor(n / max(|a|, |b|))
            R.append((angle, c))
    sort R by angle

    // Step 2: Compute All(>=3)
    prod_all = 1
    S1 = 0, S2 = 0
    for (theta, c) in R:
        prod_all = prod_all * (1 + c) mod MOD
        S1 += c
        S2 += c * c
    e2 = (S1^2 - S2) / 2 mod MOD
    All_ge3 = (prod_all - 1 - S1 - e2) mod MOD

    // Step 3: Compute Bad(>=3) via anchor trick + two-pointer
    // For each anchor i, find window W(i) = rays in (theta_i, theta_i + pi]
    Bad = 0
    j = 0, window_prod = 1, window_sum = 0
    for i = 0 to |R| - 1:
        // Advance j to include rays in window
        while angle[j % |R|] - angle[i] < pi (mod 2pi):
            window_prod *= (1 + c[j % |R|])
            window_sum += c[j % |R|]
            j++
        // Subtract size-0 and size-1 subsets from window
        Bad += c[i] * (window_prod - 1 - window_sum) mod MOD
        // Remove anchor i from window for next iteration
        window_prod /= (1 + c[i])
        window_sum -= c[i]

    return (All_ge3 - Bad) mod MOD
```

## Complexity Analysis

- **Time:** $O(n^2)$ to enumerate primitive rays (there are $\sim 12n^2/\pi^2$ of them). $O(|R|)$ for the two-pointer sweep. Total: $O(n^2)$.
- **Space:** $O(|R|) = O(n^2)$.

## Answer

$$\boxed{585965659}$$

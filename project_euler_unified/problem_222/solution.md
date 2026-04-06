# Problem 222: Sphere Packing

## Problem Statement

What is the minimum length of a tube of internal radius $R = 50$ mm that can contain 21 spheres of radii $r = 30, 31, \ldots, 50$ mm? Give the answer in micrometers ($\mu$m), rounded to the nearest integer.

## Mathematical Foundation

**Theorem 1 (Sphere Center Offset).** *A sphere of radius $r$ placed inside a cylinder of internal radius $R$ (with $r < R$) and touching the cylinder wall has its center at distance $R - r$ from the cylinder axis.*

**Proof.** The sphere touches the cylinder wall at a point where the cylinder's inner surface has distance $R$ from the axis. The center of the sphere is at distance $R - r$ from the axis (the sphere radius measured inward from the contact point). $\square$

**Theorem 2 (Vertical Gap Formula).** *Two tangent spheres of radii $r_i$ and $r_j$, both touching the cylinder wall of radius $R$, placed on opposite sides of the cylinder axis, have a vertical separation of*

$$\Delta z(r_i, r_j) = 2\sqrt{R(r_i + r_j - R)}$$

*provided $r_i + r_j > R$.*

**Proof.** The centers are at distances $R - r_i$ and $R - r_j$ from the axis, on opposite sides ($\theta = \pi$). The horizontal distance between centers is

$$d_h = (R - r_i) + (R - r_j) = 2R - r_i - r_j.$$

Since the spheres are tangent, the center-to-center distance is $r_i + r_j$. By the Pythagorean theorem:

$$\Delta z^2 = (r_i + r_j)^2 - d_h^2 = (r_i + r_j)^2 - (2R - r_i - r_j)^2.$$

Let $s = r_i + r_j$:

$$\Delta z^2 = s^2 - (2R - s)^2 = s^2 - 4R^2 + 4Rs - s^2 = 4R(s - R).$$

Hence $\Delta z = 2\sqrt{R(r_i + r_j - R)}$, valid when $s > R$. $\square$

**Lemma 1 (Validity).** *For our problem, $r_i + r_j \geq 30 + 31 = 61 > 50 = R$, so the formula is always valid.*

**Proof.** Immediate from the minimum sphere radii. $\square$

**Theorem 3 (Total Tube Length).** *For a permutation $\sigma$ of the 21 spheres, the minimum tube length is*

$$L(\sigma) = r_{\sigma(1)} + r_{\sigma(21)} + \sum_{k=1}^{20} \Delta z(r_{\sigma(k)}, r_{\sigma(k+1)}).$$

**Proof.** The first sphere requires $r_{\sigma(1)}$ clearance from one end, the last requires $r_{\sigma(21)}$ from the other end. Between consecutive spheres, the vertical space consumed is $\Delta z$. $\square$

**Theorem 4 (Optimality via Bitmask DP).** *The minimum of $L(\sigma)$ over all $21!$ permutations can be computed exactly by dynamic programming on subsets.*

**Proof.** Define $\text{dp}[S][j]$ as the minimum partial tube length when the set $S$ of spheres has been placed and $j \in S$ is the last sphere. Initialize $\text{dp}[\{j\}][j] = r_j$. The recurrence

$$\text{dp}[S \cup \{k\}][k] = \min_{j \in S}\bigl(\text{dp}[S][j] + \Delta z(r_j, r_k)\bigr)$$

correctly computes the optimum by the principle of optimality (Bellman). The final answer is $L^* = \min_j(\text{dp}[\text{all}][j] + r_j)$. $\square$

## Algorithm

```
function min_tube_length(radii[1..21], R):
    n = 21
    precompute dz[i][j] = 2 * sqrt(R * (radii[i] + radii[j] - R)) for all i, j

    dp[1 << i][i] = radii[i]    for each i in 0..n-1

    for S = 1 to (1 << n) - 1:
        for j in S:
            if dp[S][j] is undefined: continue
            for k not in S:
                val = dp[S][j] + dz[j][k]
                dp[S | (1 << k)][k] = min(dp[S | (1 << k)][k], val)

    full = (1 << n) - 1
    L_star = min over j of (dp[full][j] + radii[j])
    return round(L_star * 1000)   // convert mm to micrometers
```

## Complexity Analysis

- **Time:** $O(2^n \cdot n^2)$ where $n = 21$. This gives $2^{21} \times 21^2 \approx 9.2 \times 10^8$ operations.
- **Space:** $O(2^n \cdot n)$ for the DP table, i.e., $2^{21} \times 21 \approx 4.4 \times 10^7$ entries.

## Answer

$$\boxed{1590933}$$

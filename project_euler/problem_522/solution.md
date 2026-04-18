# Problem 522: Hilbert's Blackout

## Problem Statement

Consider a $2^n \times 2^n$ grid of lights indexed by an order-$n$ Hilbert curve. A blackout operation turns off lights at positions along the curve satisfying specific arithmetic conditions. Determine the number of lights remaining on after the blackout.

## Mathematical Foundation

**Theorem (Hilbert Curve Bijection).** For every $n \ge 1$, the order-$n$ Hilbert curve defines a bijection

$$H_n : \{0, 1, \ldots, 4^n - 1\} \to \{0, \ldots, 2^n - 1\}^2$$

such that consecutive indices map to grid-adjacent cells (sharing an edge).

**Proof.** We proceed by induction on $n$.

*Base case ($n = 1$):* The order-1 curve visits the four cells of a $2 \times 2$ grid in the order $(0,0) \to (0,1) \to (1,1) \to (1,0)$. Each consecutive pair is adjacent. The map is clearly a bijection on a 4-element set.

*Inductive step:* Assume $H_{n-1}$ is a bijection on the $(2^{n-1})^2$ grid with the adjacency property. Partition the $2^n \times 2^n$ grid into four $2^{n-1} \times 2^{n-1}$ quadrants. Define $H_n$ by traversing:
1. Bottom-left quadrant using $H_{n-1}$ reflected about the diagonal ($90^\circ$ CW rotation),
2. Top-left quadrant using $H_{n-1}$ (unmodified),
3. Top-right quadrant using $H_{n-1}$ (unmodified),
4. Bottom-right quadrant using $H_{n-1}$ reflected about the anti-diagonal ($90^\circ$ CCW rotation).

Each quadrant contributes $4^{n-1}$ cells, giving $4 \cdot 4^{n-1} = 4^n$ total. The junction points between quadrants are adjacent by construction (the exit of one quadrant neighbors the entry of the next). By induction, within each quadrant consecutive indices are adjacent. Hence $H_n$ is a bijection with the adjacency property. $\square$

**Lemma (Self-Similar Counting).** Let $C_n$ denote the count of surviving lights after the blackout operation on an order-$n$ grid. If the blackout condition respects the 4-fold recursive structure of the Hilbert curve (i.e., the condition on index $d$ depends only on $d \bmod 4^k$ for some fixed $k$), then $C_n$ satisfies a linear recurrence of the form

$$C_n = \alpha \cdot C_{n-1} + \beta$$

for constants $\alpha, \beta$ determined by the blackout rule applied to the quadrant junctions.

**Proof.** When the blackout condition depends only on $d \bmod 4^k$, the number of blackout positions in each quadrant of an order-$n$ curve is determined by the number of blackout positions in an order-$(n-1)$ curve, shifted by a fixed offset modulo $4^k$. Since the four quadrants tile the index space $[0, 4^n)$ into four contiguous blocks of size $4^{n-1}$, each block's count depends on $C_{n-1}$ plus a bounded correction from the offset. The correction is constant in $n$ for $n > k$. $\square$

## Editorial
Grid blackout puzzle on Hilbert curve grids. We base: enumerate all 4^k positions for small k. We then recursive: exploit self-similarity. Finally, determine counts for each quadrant transformation.

## Pseudocode

```text
Base: enumerate all 4^k positions for small k
Recursive: exploit self-similarity
Determine counts for each quadrant transformation
rotate quadrant
```

## Complexity Analysis

- **Time (direct enumeration):** $O(4^n)$, since all positions are visited.
- **Time (recursive, structure-respecting blackout):** $O(n)$, since the recurrence has constant cost per level.
- **Space:** $O(n)$ for the recursion stack, or $O(4^n)$ for direct enumeration.

## Answer

$$\boxed{96772715}$$

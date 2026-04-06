# Problem 181: Investigating in How Many Ways Objects of Two Different Colours Can Be Grouped

## Problem Statement

How many ways can 60 black objects and 40 white objects be grouped into non-empty groups, where the order of groups does not matter and each group is characterized by the pair $(b, w)$ denoting the number of black and white objects it contains?

## Mathematical Development

**Definition 1.** A *bicolour partition* of the pair $(B, W) \in \mathbb{Z}_{\geq 0}^2$ is a multiset $\{(b_1, w_1), (b_2, w_2), \ldots, (b_k, w_k)\}$ of elements from $\mathbb{Z}_{\geq 0}^2 \setminus \{(0,0)\}$ satisfying
$$\sum_{i=1}^{k} b_i = B \quad \text{and} \quad \sum_{i=1}^{k} w_i = W.$$

Let $P(B, W)$ denote the number of such bicolour partitions. We seek $P(60, 40)$.

**Theorem 1 (Generating Function).** The bicolour partition function satisfies
$$\sum_{B=0}^{\infty} \sum_{W=0}^{\infty} P(B, W)\, x^B y^W = \prod_{\substack{(b, w) \in \mathbb{Z}_{\geq 0}^2 \\ (b,w) \neq (0,0)}} \frac{1}{1 - x^b y^w}.$$

*Proof.* The set of admissible part types is $\mathcal{P} = \mathbb{Z}_{\geq 0}^2 \setminus \{(0,0)\}$. A bicolour partition is a function $\mu: \mathcal{P} \to \mathbb{Z}_{\geq 0}$ assigning a multiplicity to each part type, subject to $\sum_{(b,w)} \mu(b,w) \cdot b = B$ and $\sum_{(b,w)} \mu(b,w) \cdot w = W$. The generating function for the multiplicity of a single part type $(b,w)$ is the geometric series
$$\sum_{k=0}^{\infty} (x^b y^w)^k = \frac{1}{1 - x^b y^w}.$$
Since the part types are independent, the total generating function is the product over all part types. The coefficient $[x^B y^W]$ of this product counts the number of multisets with the prescribed totals, which is precisely $P(B, W)$. $\square$

**Theorem 2 (Unbounded Knapsack Reduction).** Let the part types be enumerated in an arbitrary fixed total order as $p_1, p_2, \ldots, p_M$ where $M = (B+1)(W+1) - 1$. Define $f_j(b, w)$ as the number of bicolour partitions of $(b, w)$ using only part types from $\{p_1, \ldots, p_j\}$. Then $f_0(0,0) = 1$, $f_0(b, w) = 0$ for $(b,w) \neq (0,0)$, and for each $j \geq 1$ with $p_j = (\delta_b, \delta_w)$:
$$f_j(b, w) = \sum_{t=0}^{\lfloor \min(b/\delta_b,\, w/\delta_w) \rfloor} f_{j-1}(b - t\delta_b,\, w - t\delta_w).$$

*Proof.* By the principle of sequential decision: for each part type $p_j$, choose its multiplicity $t \geq 0$, then count partitions of the residual $(b - t\delta_b, w - t\delta_w)$ using only the preceding part types. The base case $f_0$ reflects the empty partition of $(0,0)$. $\square$

**Corollary 1 (In-Place Computation).** If part types are processed sequentially and, for each part type $(\delta_b, \delta_w)$, the table is updated via
$$\mathrm{dp}[b][w] \mathrel{+}= \mathrm{dp}[b - \delta_b][w - \delta_w]$$
with $b$ ranging from $\delta_b$ to $B$ and $w$ from $\delta_w$ to $W$ in increasing order, then after processing all part types, $\mathrm{dp}[B][W] = P(B, W)$.

*Proof.* This is the standard unbounded knapsack transformation. Iterating in increasing order permits each part type to be used with unlimited multiplicity. The in-place update telescopes the summation in Theorem 2, since after processing part $p_j$, the entry $\mathrm{dp}[b][w]$ equals $f_j(b, w)$. The proof proceeds by induction on $j$: the base case $j = 0$ holds by initialization; for the inductive step, the forward-sweep update accumulates exactly the sum $\sum_{t \geq 0} f_{j-1}(b - t\delta_b, w - t\delta_w)$. $\square$

## Algorithm

```
function CountBicolourPartitions(B, W):
    dp[0..B][0..W] := 0
    dp[0][0] := 1

    for db := 0 to B:
        for dw := 0 to W:
            if db = 0 and dw = 0: continue
            for b := db to B:
                for w := dw to W:
                    dp[b][w] += dp[b - db][w - dw]

    return dp[B][W]
```

## Complexity Analysis

- **Time:** The outer double loop iterates over $M = (B+1)(W+1) - 1$ part types. For each part type $(\delta_b, \delta_w)$, the inner loop performs $(B - \delta_b + 1)(W - \delta_w + 1) \leq B \cdot W$ updates. Hence the total work is $O(B^2 W^2)$. For $(B, W) = (60, 40)$, this yields approximately $60^2 \cdot 40^2 = 5{,}760{,}000$ operations.
- **Space:** $O(B \cdot W)$ for the two-dimensional DP table.

## Answer

$$\boxed{83735848679360680}$$

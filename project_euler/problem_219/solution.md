# Problem 219: Skew-cost Coding

## Problem Statement

Build a prefix-free binary code for $N = 10^9$ symbols. Each codeword is a binary string. A `0` bit costs 1 and a `1` bit costs 4. The **cost** of a codeword is the sum of costs of its bits. The **total cost** is the sum of costs of all $N$ codewords.

Find the minimum total cost.

## Mathematical Foundation

**Definition.** A *prefix-free code* for $N$ symbols is a set of $N$ binary strings such that no string is a prefix of another. Such a code corresponds to a binary tree with $N$ leaves.

**Theorem 1 (Greedy optimality).** *The minimum total cost is achieved by the greedy algorithm: starting from a single leaf (cost 0), repeatedly split the minimum-cost leaf. Splitting a leaf of cost $c$ removes it and creates two leaves of costs $c + 1$ (append 0) and $c + 4$ (append 1).*

**Proof.** We prove that at optimality, the two leaves created by the last split should come from the minimum-cost leaf. Suppose for contradiction that an optimal tree has a leaf $\ell_1$ of cost $c_1$ that is a leaf (never split) and a leaf $\ell_2$ of cost $c_2 > c_1$ that was split from some internal node. Consider the tree obtained by swapping: make $\ell_2$ a leaf and split $\ell_1$ instead. The cost change is $(c_1 + 1) + (c_1 + 4) - c_1 - [(c_2 + 1) + (c_2 + 4) - c_2] = c_1 - c_2 < 0$, a strict improvement. This contradicts optimality unless we always split the minimum-cost leaf. By induction on the number of splits, the greedy strategy is optimal. $\square$

**Lemma 1 (Cost accounting).** *Each split of a leaf with cost $c$ increases the total cost by $c + 5$ and the number of leaves by 1.*

**Proof.** Splitting removes one leaf of cost $c$ and adds two leaves of costs $c + 1$ and $c + 4$. Net change in total cost: $(c+1) + (c+4) - c = c + 5$. Net change in leaf count: $2 - 1 = 1$. $\square$

**Theorem 2 (Total cost formula).** *Starting with one leaf of cost 0, after $N - 1$ splits the total cost is*

$$\text{Total} = 5(N-1) + \sum_{i=0}^{N-2} c_i$$

*where $c_i$ is the cost of the leaf split at step $i$.*

**Proof.** The initial total cost is 0. By Lemma 1, each split $i$ adds $c_i + 5$. Summing: $\text{Total} = \sum_{i=0}^{N-2}(c_i + 5) = 5(N-1) + \sum c_i$. $\square$

**Theorem 3 (Bulk simulation correctness).** *At any point, if all $k$ leaves of the current minimum cost $c$ are split simultaneously, this is equivalent to $k$ sequential greedy splits (each splitting the minimum-cost leaf). The total cost increases by $k(c + 5)$, the leaf count increases by $k$, and the cost histogram updates as: remove $k$ entries at cost $c$; add $k$ entries at cost $c + 1$; add $k$ entries at cost $c + 4$.*

**Proof.** Since all $k$ leaves have the same minimum cost $c$, they would be the first $k$ leaves chosen by the sequential greedy algorithm (ties broken arbitrarily). Splitting them in any order produces the same result: each split removes one leaf of cost $c$ and adds leaves of costs $c+1$ and $c+4$. The $k$ splits are independent (they operate on distinct leaves), so the bulk operation is correct. $\square$

## Algorithm

```
function SolveProblem219(N):
    // Histogram: cost -> count
    hist = {0: 1}    // one leaf of cost 0
    leaves = 1
    total = 0

    while leaves < N:
        c = min key in hist
        k = hist[c]

        // Don't overshoot: we need N - leaves more splits
        needed = N - leaves
        if k > needed:
            k = needed

        // Bulk split k leaves of cost c
        total += k * (c + 5)
        leaves += k

        // Update histogram
        hist[c] -= k
        if hist[c] == 0:
            delete hist[c]
        hist[c + 1] = hist.get(c + 1, 0) + k
        hist[c + 4] = hist.get(c + 4, 0) + k

    return total
```

## Complexity Analysis

- **Time:** Each iteration processes all leaves of the current minimum cost, advancing the minimum cost by at least 1. The maximum cost reached is $O(\sqrt{N})$ (since the number of leaves grows with each bulk split, and the cost advances linearly while the leaf count grows geometrically). Hence $O(\sqrt{N})$ iterations, each $O(\log(\text{histogram size}))$. Total: $O(\sqrt{N} \log N)$.
- **Space:** $O(\sqrt{N})$ for the histogram (at most $O(\sqrt{N})$ distinct cost levels active at once).

## Answer

$$\boxed{64564225042}$$

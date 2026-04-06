# Problem 240: Top Dice

## Problem Statement

In how many ways can 20 twelve-sided dice (sides numbered 1 to 12) be rolled so that the top 10 values sum to 70?

For reference: there are 1111 ways for 5 six-sided dice where the top 3 sum to 15.

## Mathematical Foundation

**Theorem (Threshold Decomposition).** Let $D = 12$, $N = 20$, $H = 10$ (number of "top" dice), $S = 70$ (target sum). The number of ordered $N$-tuples $(d_1, \ldots, d_N) \in \{1, \ldots, D\}^N$ whose $H$ largest values sum to $S$ is

$$\text{Answer} = \sum_{t=1}^{D} \sum_{j=1}^{H} \sum_{m=0}^{N-H} [j + m \geq 1] \cdot R(H-j, t+1, D, S - jt) \cdot B(N-H-m, 1, t-1) \cdot \frac{N!}{\text{(frequency product)}}$$

decomposed over the threshold value $t = d_{(H)}$ (the $H$-th largest value), $j$ = number of top dice equal to $t$, and $m$ = number of bottom dice equal to $t$.

**Proof.** Sort the values: $d_{(1)} \geq d_{(2)} \geq \cdots \geq d_{(N)}$. The $H$-th largest value $t = d_{(H)}$ satisfies $1 \leq t \leq D$. Among the top $H$ dice, exactly $j \geq 1$ have value $t$, and the remaining $H - j$ have values strictly greater than $t$, summing to $S - jt$. Among the bottom $N - H$ dice, exactly $m \geq 0$ have value $t$, and the remaining $N - H - m$ have values strictly less than $t$.

For each valid $(t, j, m)$:
- The $H - j$ "above-threshold" top dice contribute values from $\{t+1, \ldots, D\}$ summing to $S - jt$. Let $R(h, a, b, s)$ denote the number of multisets of $h$ values from $\{a, \ldots, b\}$ summing to $s$.
- The $N - H - m$ "below-threshold" bottom dice contribute values from $\{1, \ldots, t-1\}$.
- There are $j + m$ dice with value exactly $t$.

The number of ordered tuples for a given frequency vector $(n_1, \ldots, n_D)$ is the multinomial coefficient $N! / \prod_{v=1}^{D} n_v!$. Summing over all valid frequency vectors gives the total count. $\square$

**Lemma (Stars-and-Bars with Bounds).** The count $R(h, a, b, s)$ of multisets of $h$ values from $\{a, \ldots, b\}$ summing to $s$ can be computed via dynamic programming on the recurrence

$$R(h, a, b, s) = \sum_{v=a}^{\min(b, s)} R(h-1, v, b, s-v)$$

with base case $R(0, a, b, 0) = 1$ and $R(0, a, b, s) = 0$ for $s > 0$.

**Proof.** Fix the smallest value in the multiset as $v \in \{a, \ldots, b\}$, subtract it from the sum, and recurse on the remaining $h-1$ values (each $\geq v$). The base case handles the empty multiset. $\square$

## Algorithm

```
function solve(D, N, H, S):
    total = 0
    for t = 1 to D:
        for j = 1 to H:
            remaining_sum = S - j * t
            if remaining_sum < 0: break
            top_count = H - j  // dice with values in {t+1, ..., D}

            // Enumerate frequency vectors for top dice above threshold
            for each multiset of top_count values from {t+1,...,D} summing to remaining_sum:
                freq = frequency vector of this multiset
                freq[t] += j

                // Enumerate bottom dice: (N-H) dice with values in {1,...,t}
                for m = 0 to N - H:
                    bottom_remaining = N - H - m  // dice with values in {1,...,t-1}
                    freq[t] += m  // temporarily

                    for each multiset of bottom_remaining values from {1,...,t-1}:
                        combined_freq = freq + bottom_freq
                        combined_freq[t] += m
                        multinomial = N! / product(combined_freq[v]! for v=1..D)
                        total += multinomial

                    freq[t] -= m  // restore

    return total
```

## Complexity Analysis

- **Time:** $O(D^2 \cdot N \cdot S)$ for the DP-based enumeration of frequency vectors. In practice, the number of valid frequency vectors is bounded and the computation is tractable.
- **Space:** $O(N \cdot S)$ for the DP tables used in computing multiset counts.

## Answer

$$\boxed{7448717393364181966}$$

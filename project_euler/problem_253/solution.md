# Problem 253: Tidying Up

## Problem Statement

A caterpillar of length $n = 40$ is a row of $n$ cells. Cells are filled one at a time in uniformly random order (each of the $n!$ permutations equally likely). After each placement, count the number of **segments** (maximal contiguous blocks of filled cells). Find the expected value of the maximum number of segments attained during the entire process, to six decimal places.

## Mathematical Foundation

**Definition.** Let $\sigma$ be a uniformly random permutation of $\{1, \ldots, n\}$. At step $k$, cell $\sigma(k)$ is filled. Let $S_k$ denote the number of segments after $k$ cells are filled. The quantity of interest is $E\!\left[\max_{1 \le k \le n} S_k\right]$.

**Lemma 1 (Segment Increment Rule).** *When cell $\sigma(k)$ is placed at step $k$, the change in the segment count is:*

$$S_k - S_{k-1} = 1 - (\text{number of already-filled neighbors of } \sigma(k)).$$

*That is, $S_k = S_{k-1} + 1$ if neither neighbor is filled, $S_k = S_{k-1}$ if exactly one neighbor is filled, and $S_k = S_{k-1} - 1$ if both neighbors are filled.*

**Proof.** A cell with no filled neighbor creates a new isolated segment (net $+1$). A cell adjacent to exactly one filled segment extends it (net $0$). A cell flanked by two distinct segments merges them (net $-1$). These are the only cases. $\square$

**Lemma 2 (Join Representation).** *Define $J_k = \#\{i \le k : \sigma(i)\text{ is adjacent to an already-filled cell at step }i\}$. Then $S_k = k - 2J_k$ and*

$$\max_{1 \le k \le n} S_k = \max_{1 \le k \le n}(k - 2J_k).$$

**Proof.** Each placement increments $k$ by 1. Each adjacency to a filled neighbor contributes a $-1$ instead of $+1$ to the segment count (relative to the "no neighbor" case). Since $S_0 = 0$ and each step $i$ contributes $+1 - (\text{filled-neighbor count at step }i)$, we have $S_k = \sum_{i=1}^{k}(1 - j_i)$ where $j_i \in \{0,1,2\}$ is the filled-neighbor count. Splitting $j_i$ as "number of joins," noting that a cell with 2 filled neighbors contributes 2 joins, the exact accounting gives $S_k = k - 2J_k$. $\square$

**Theorem 1 (Gap-Profile DP).** *The expected maximum segment count can be computed exactly via dynamic programming over gap profiles. After $k$ cells are placed forming $s$ segments, the remaining $n - k$ empty cells are partitioned into $s + 1$ gaps (some possibly of size 0 at the boundaries). The multiset of gap sizes, together with the running maximum $m$, determines the transition probabilities.*

**Proof.** The gap profile is a sufficient statistic: the number of empty cells in each gap determines how many cells in each gap can increase, maintain, or decrease the segment count, and the probability of filling any particular empty cell is $1/(n-k)$. The gap profile evolves deterministically given which gap is chosen and which position within the gap. Thus a DP on (gap profile, running max $m$) correctly computes $E[\max_k S_k]$ by summing $m \cdot P(\max = m)$ over terminal states (when $k = n$, necessarily $S_n = 1$, $m = $ recorded max). $\square$

## Editorial
Expected maximum number of segments when randomly filling a caterpillar of length 40. State-based DP: track gap configurations (end gaps + internal gaps) and max segments seen. At each step, a random empty cell is filled, which either:. We state: (gap_profile, max_segments_so_far). We then initial state: one gap of size n, 0 segments, max = 0. Finally, we run a memoized search over the reachable states.

## Pseudocode

```text
State: (gap_profile, max_segments_so_far)
gap_profile = sorted tuple of gap sizes (multiset)
Initial state: one gap of size n, 0 segments, max = 0
BFS/memoized recursion over states
Place in interior of gap g: splits into (j-1, g-j) for j=1..g
If j=1 or j=g (endpoint touching a segment): extends segment
If 1 < j < g: creates new segment, splitting gap
Endpoint cases depend on boundary
Handle boundary gaps (leftmost/rightmost) specially
```

## Complexity Analysis

- **Time:** The number of distinct states is bounded by $\sum_{k=0}^{n} p(n-k) \cdot \lceil n/2 \rceil$, where $p(j)$ is the number of integer partitions of $j$. For $n = 40$, this is approximately $10^6$ states, each with $O(n)$ transitions. Total: $O(n \cdot |\text{states}|) \approx O(n \cdot p(n) \cdot n) = O(n^2 \cdot p(n))$.
- **Space:** $O(|\text{states}|) = O(p(n) \cdot n)$.

## Answer

$$\boxed{11.492847}$$

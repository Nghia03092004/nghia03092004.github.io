# Problem 663: Sums of Subarrays

## Problem Statement

An array $A_n$ of length $n$ is initialized to all zeros. A pseudo-random sequence $\{t_i\}$ generates updates: at step $i$, set $A_n[t_{2i-2} \bmod n] \mathrel{+}= 2(t_{2i-1} \bmod n) - n + 1$.

After each step $i$, define $M_n(i) = \max\!\left\{\sum_{j=p}^{q} A_n[j] : 0 \leq p \leq q < n\right\}$, the maximum contiguous subarray sum.

Let $S(n, l) = \sum_{i=1}^{l} M_n(i)$.

Given:
- $S(5, 6) = 32$, $S(5, 100) = 2416$, $S(14, 100) = 3881$, $S(107, 1000) = 1618572$

Find $S(10\,000\,003,\; 10\,200\,000) - S(10\,000\,003,\; 10\,000\,000)$.

## Mathematical Foundation

**Theorem 1 (Kadane, 1984).** *The maximum contiguous subarray sum of an array $A[0..n-1]$ equals*
$$M = \max\!\left(0,\; \max_{1 \leq q \leq n}\left(P_q - \min_{0 \leq p < q} P_p\right)\right)$$
*where $P_k = \sum_{j=0}^{k-1} A[j]$ is the $k$-th prefix sum, with $P_0 = 0$.*

**Proof.** Any contiguous subarray sum $\sum_{j=p}^{q} A[j] = P_{q+1} - P_p$. Maximizing over $0 \leq p \leq q < n$ is equivalent to maximizing $P_b - P_a$ over $0 \leq a < b \leq n$. Since we also allow the empty subarray (sum 0), the result follows. $\square$

**Theorem 2 (Segment Tree for Maximum Subarray Sum).** *A segment tree on the prefix sum array $P_0, P_1, \ldots, P_n$ supports:*
- *Point update of $A[k]$ (which is a suffix update on $P$) in $O(\log n)$ time.*
- *Global maximum subarray sum query in $O(1)$ time.*

*Each node for interval $[l, r]$ stores the tuple $(\max P, \min P, \text{max\_sub}, \text{total})$.*

**Proof.** We prove correctness of the merge operation. Consider a node $v$ with children $L = [l, m]$ and $R = [m+1, r]$. The maximum subarray sum within $v$'s interval is $\max_{l \leq a < b \leq r}(P_b - P_a)$, which equals the maximum of three cases:

1. Both $a, b \in [l, m]$: this is $\text{max\_sub}(L)$.
2. Both $a, b \in [m+1, r]$: this is $\text{max\_sub}(R)$.
3. $a \in [l, m]$ and $b \in [m+1, r]$: this is $\max_{b \in R} P_b - \min_{a \in L} P_a = \text{max\_prefix}(R) - \text{min\_prefix}(L)$.

Taking the maximum of these three cases gives $\text{max\_sub}(v)$.

For the prefix aggregates:
$$\text{max\_prefix}(v) = \max(\text{max\_prefix}(L),\; \text{max\_prefix}(R))$$
$$\text{min\_prefix}(v) = \min(\text{min\_prefix}(L),\; \text{min\_prefix}(R))$$

A point update $A[k] \mathrel{+}= \delta$ changes $P_j$ by $\delta$ for all $j > k$. This is a suffix update on the prefix sum array, handled by updating the segment tree along the path from leaf $k+1$ to the root. With lazy propagation for the suffix-add, each update costs $O(\log n)$. $\square$

**Lemma 1 (Non-negativity).** *If $M_n(i) < 0$ is possible only when all $A[j] < 0$, but by convention the maximum subarray sum is at least 0 (empty subarray).*

**Proof.** The empty subarray has sum 0, so $M_n(i) \geq 0$ always. $\square$

## Editorial
Maintain maximum subarray sum under point updates using a segment tree. S(n, l) = sum of max subarray sums after each of l updates. We suffix-update P[k+1..n] by delta.

## Pseudocode

```text
    A = array of n zeros
    P = prefix sum array of n+1 zeros
    build segment tree T on P[0..n]
    total = 0
    For i from 1 to l:
        k = t[2i-2] mod n
        delta = 2 * (t[2i-1] mod n) - n + 1
        A[k] += delta
        suffix-update P[k+1..n] by delta
        update T at positions k+1 through n by +delta
        M = T.root.max_sub
        total += max(0, M)
    Return total
```

## Complexity Analysis

- **Time:** $O(n + l \log n)$ -- building the segment tree takes $O(n)$; each of the $l$ updates and queries takes $O(\log n)$.
- **Space:** $O(n)$ for the segment tree.

For the target parameters: $n \approx 10^7$, $l = 200{,}000$ updates, giving approximately $200{,}000 \times 23 \approx 4.6 \times 10^6$ operations.

## Answer

$$\boxed{1884138010064752}$$

# Problem 593: Fleeting Medians

## Problem Statement

Define the sequence $S_n$ for $n \geq 1$ by:

$$S_n = s_n \bmod 10007, \quad \text{where } s_n = \text{(prime}(n))^2 \bmod 10007$$

(Here $\text{prime}(n)$ is the $n$-th prime.) Let $M(i, j)$ be the median of the subsequence $S_i, S_{i+1}, \ldots, S_j$. Compute:

$$F(N, K) = \sum_{i=1}^{N-K+1} M(i, i+K-1)$$

for specified $N$ and $K$.

## Mathematical Foundation

**Theorem 1 (Sliding Window Median via Order Statistics).** Given a sequence $(x_1, \ldots, x_n)$ and window size $K$, the median of each window $\{x_i, \ldots, x_{i+K-1}\}$ can be maintained in $O(\log K)$ amortized time per window step using balanced order-statistic data structures.

**Proof.** We maintain two multisets (or heaps): a max-heap $L$ containing the $\lfloor K/2 \rfloor$ smallest elements of the current window, and a min-heap $R$ containing the remaining $\lceil K/2 \rceil$ elements. The median is the top of $R$ (for odd $K$) or the average of the tops (for even $K$).

When the window slides from $[i, i+K-1]$ to $[i+1, i+K]$:
1. Remove $x_i$: locate it in $L$ or $R$ and remove it ($O(\log K)$).
2. Insert $x_{i+K}$: compare with tops of $L$ and $R$ to determine placement ($O(\log K)$).
3. Rebalance: if $|L|$ and $|R|$ differ by more than the required amount, transfer the top element ($O(\log K)$).

Each step is $O(\log K)$, so the total over $N - K + 1$ windows is $O(N \log K)$. $\square$

**Lemma 1 (Fenwick Tree / BIT for Order Statistics).** Since $S_n \in \{0, 1, \ldots, 10006\}$, we can maintain a frequency array $\text{freq}[0 \ldots 10006]$ with a Fenwick tree supporting:
- $\text{update}(v, \delta)$: increment $\text{freq}[v]$ by $\delta$ in $O(\log M)$ time, where $M = 10007$.
- $\text{query}(k)$: find the $k$-th smallest element in $O(\log^2 M)$ time via binary search on prefix sums.

**Proof.** The Fenwick tree stores prefix sums of the frequency array. To find the $k$-th order statistic, perform a binary search on the index $v$ such that $\sum_{j=0}^{v} \text{freq}[j] \geq k$ and $\sum_{j=0}^{v-1} \text{freq}[j] < k$. Each prefix sum query costs $O(\log M)$, and the binary search has $O(\log M)$ steps, giving $O(\log^2 M)$ per median query. With a Fenwick tree that supports walking down the tree, this improves to $O(\log M)$. $\square$

**Theorem 2 (Correctness of Median Computation).** For a window of size $K$, the median is the $\lceil K/2 \rceil$-th smallest element. When $K$ is even, the median is the average of the $(K/2)$-th and $(K/2+1)$-th smallest.

**Proof.** By definition of the median for a finite set. $\square$

## Algorithm

```
function F(N, K):
    // Step 1: Generate sequence S
    primes = sieve_first_N_primes(N)
    S[1..N] where S[n] = (primes[n]^2) mod 10007

    // Step 2: Initialize Fenwick tree over [0, 10006]
    BIT = FenwickTree(10007)
    for j = 1 to K:
        BIT.update(S[j], +1)

    total = 0
    // Step 3: Compute median for first window
    if K is odd:
        med = BIT.kth_order_statistic((K+1)/2)
    else:
        lo = BIT.kth_order_statistic(K/2)
        hi = BIT.kth_order_statistic(K/2 + 1)
        med = (lo + hi) / 2.0
    total += med

    // Step 4: Slide window
    for i = 2 to N - K + 1:
        BIT.update(S[i-1], -1)       // remove leftmost
        BIT.update(S[i+K-1], +1)     // add rightmost
        if K is odd:
            med = BIT.kth_order_statistic((K+1)/2)
        else:
            lo = BIT.kth_order_statistic(K/2)
            hi = BIT.kth_order_statistic(K/2 + 1)
            med = (lo + hi) / 2.0
        total += med

    return total
```

## Complexity Analysis

- **Time:** $O(N \log M)$ where $M = 10007$. Generating primes takes $O(N \log \log N)$ via sieve. Each window slide involves one removal, one insertion, and one or two order-statistic queries, each $O(\log M)$. Total: $O(N \log M) = O(N \cdot 14) \approx O(N)$.
- **Space:** $O(N)$ for the prime sieve and sequence, plus $O(M) = O(10007)$ for the Fenwick tree.

## Answer

$$\boxed{96632320042.0}$$

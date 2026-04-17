# Problem 464: Mobius Function and Intervals

## Problem Statement

The Mobius function $\mu(n)$ equals $(-1)^k$ if $n$ is squarefree with $k$ distinct prime factors, and $0$ otherwise. Define:
- $P(a,b) = |\{n \in [a,b] : \mu(n) = 1\}|$
- $N(a,b) = |\{n \in [a,b] : \mu(n) = -1\}|$

Let $C(n)$ count pairs $(a,b)$ with $1 \le a \le b \le n$ such that $99 \cdot N(a,b) \le 100 \cdot P(a,b)$ and $99 \cdot P(a,b) \le 100 \cdot N(a,b)$.

Given: $C(10) = 13$, $C(500) = 16676$, $C(10000) = 20155319$. Find $C(20\,000\,000)$.

## Mathematical Foundation

**Theorem 1 (Reduction to 2D dominance).** Define prefix sums $\mathrm{prefP}(i) = |\{n \le i : \mu(n) = 1\}|$ and $\mathrm{prefN}(i) = |\{n \le i : \mu(n) = -1\}|$, with $\mathrm{prefP}(0) = \mathrm{prefN}(0) = 0$. Set
$$X(i) = 100 \cdot \mathrm{prefP}(i) - 99 \cdot \mathrm{prefN}(i), \qquad Y(i) = 100 \cdot \mathrm{prefN}(i) - 99 \cdot \mathrm{prefP}(i).$$
Then $C(n) = |\{(j, b) : 0 \le j < b \le n,\; X(b) \ge X(j),\; Y(b) \ge Y(j)\}|$.

**Proof.** For a pair $(a, b)$, write $P(a,b) = \mathrm{prefP}(b) - \mathrm{prefP}(a-1)$ and $N(a,b) = \mathrm{prefN}(b) - \mathrm{prefN}(a-1)$. The first inequality $99 \cdot N(a,b) \le 100 \cdot P(a,b)$ becomes
$$99(\mathrm{prefN}(b) - \mathrm{prefN}(a-1)) \le 100(\mathrm{prefP}(b) - \mathrm{prefP}(a-1)),$$
which rearranges to $X(b) \ge X(a-1)$. Similarly, the second inequality yields $Y(b) \ge Y(a-1)$. Setting $j = a - 1$ (so $j$ ranges over $0, 1, \ldots, n-1$ and $b$ over $j+1, \ldots, n$) completes the reduction. $\square$

**Lemma 1 (Monotonicity observation).** $X(i) + Y(i) = \mathrm{prefP}(i) + \mathrm{prefN}(i)$, which counts squarefree numbers up to $i$ and is non-decreasing.

**Proof.** Direct computation: $X(i) + Y(i) = 100\,\mathrm{prefP}(i) - 99\,\mathrm{prefN}(i) + 100\,\mathrm{prefN}(i) - 99\,\mathrm{prefP}(i) = \mathrm{prefP}(i) + \mathrm{prefN}(i)$. This is the count of squarefree integers $\le i$, which is non-decreasing. $\square$

**Theorem 2 (CDQ divide-and-conquer).** The 2D dominance count with the ordering constraint $j < b$ can be computed in $O(n \log^2 n)$ time.

**Proof.** We use the CDQ (Chen, Deng, Qin) offline divide-and-conquer framework. Divide the index range $[0, n]$ at midpoint $m$. Recursively count pairs where both indices lie in $[0, m]$ or both in $(m, n]$. For cross-pairs ($j \le m < b$):

1. Sort the left half $L = \{0, \ldots, m\}$ and right half $R = \{m+1, \ldots, n\}$ independently by $X$-value.
2. Two-pointer sweep: process elements of $R$ in increasing $X$ order. For each $b \in R$, insert all $j \in L$ with $X(j) \le X(b)$ into a Fenwick tree keyed by coordinate-compressed $Y(j)$.
3. Query the Fenwick tree for the prefix sum up to $Y(b)$, counting all $j$ with $Y(j) \le Y(b)$.

The merge step takes $O(k \log k)$ for a subproblem of size $k$. With $O(\log n)$ levels, the total is $O(n \log^2 n)$. Correctness follows because every valid pair $(j, b)$ with $j < b$ is counted exactly once: either both lie in the same half (handled recursively) or they straddle the midpoint (handled by the merge step). $\square$

## Algorithm

```
function CountC(n):
    // Step 1: Sieve Mobius function
    mu[1..n] = linear_sieve_mobius(n)

    // Step 2: Compute prefix sums and coordinates
    prefP[0] = 0, prefN[0] = 0
    for i = 1 to n:
        prefP[i] = prefP[i-1] + (1 if mu[i] == 1 else 0)
        prefN[i] = prefN[i-1] + (1 if mu[i] == -1 else 0)
    for i = 0 to n:
        X[i] = 100 * prefP[i] - 99 * prefN[i]
        Y[i] = 100 * prefN[i] - 99 * prefP[i]

    // Step 3: CDQ divide-and-conquer
    return CDQ(0, n, X, Y)

function CDQ(lo, hi, X, Y):
    if lo == hi: return 0
    mid = (lo + hi) / 2
    count = CDQ(lo, mid) + CDQ(mid + 1, hi)

    // Cross-pairs: j in [lo, mid], b in [mid+1, hi]
    L = [(X[j], Y[j]) for j in lo..mid], sorted by X
    R = [(X[b], Y[b]) for b in mid+1..hi], sorted by X

    // Coordinate-compress Y values of L
    // Two-pointer + Fenwick tree on Y
    ptr = 0
    BIT = new FenwickTree(compressed_Y_size)
    for each (xb, yb) in R (in sorted X order):
        while ptr < |L| and L[ptr].x <= xb:
            BIT.update(compress(L[ptr].y), +1)
            ptr++
        count += BIT.query(compress(yb))

    return count
```

## Complexity Analysis

- **Time:** $O(n)$ for the Mobius sieve. $O(n \log^2 n)$ for the CDQ divide-and-conquer ($O(\log n)$ levels, each doing $O(n \log n)$ work for sorting and Fenwick tree operations).
- **Space:** $O(n)$ for the sieve, prefix sums, and auxiliary arrays.

## Answer

$$\boxed{198775297232878}$$

# Problem 411: Uphill Paths

## Problem Statement

Let $n$ be a positive integer. Stations are placed at coordinates
$$
(x, y) = (2^i \bmod n,\; 3^i \bmod n), \quad 0 \le i \le 2n.
$$
Stations at identical coordinates are identified. An **uphill path** from $(0,0)$ to $(n,n)$ is a path along which both coordinates never decrease. Let $S(n)$ denote the maximum number of stations an uphill path can pass through.

**Given:** $S(22) = 5$, $S(123) = 14$, $S(10000) = 48$.

**Find:** $\displaystyle\sum_{k=1}^{30} S(k^5)$.

## Mathematical Foundation

**Theorem 1 (Reduction to LNDS).** Let $\mathcal{P} = \{(x_1,y_1),\dots,(x_M,y_M)\}$ be the set of distinct stations, sorted lexicographically by $(x,y)$. Then $S(n)$ equals the length of the longest non-decreasing subsequence (LNDS) of the sequence $(y_1,y_2,\dots,y_M)$.

**Proof.** An uphill path visits stations in order of non-decreasing $x$-coordinate. Among all subsequences of stations with non-decreasing $x$, the maximum-length subsequence that also has non-decreasing $y$ is precisely the LNDS of the $y$-values when stations are sorted by $(x,y)$ lexicographically. The lexicographic sort with $y$ ascending as tiebreaker ensures that when two stations share the same $x$-coordinate, both may appear on a non-decreasing path (since their $y$-values are in ascending order). Formally, a subsequence $(x_{i_1},y_{i_1}),\dots,(x_{i_\ell},y_{i_\ell})$ with $i_1 < i_2 < \cdots < i_\ell$ satisfies $x_{i_1} \le x_{i_2} \le \cdots \le x_{i_\ell}$ (by sort order) and $y_{i_1} \le y_{i_2} \le \cdots \le y_{i_\ell}$ (by the LNDS property), which is exactly the uphill condition. Conversely, every uphill subsequence corresponds to a non-decreasing subsequence of $y$-values in the sorted order. $\square$

**Lemma 1 (Periodicity of Station Generation).** The sequence $2^i \bmod n$ is periodic with period $\operatorname{ord}_n(2) \mid \varphi(n)$, and similarly $3^i \bmod n$ has period $\operatorname{ord}_n(3) \mid \varphi(n)$. The joint sequence of stations $(2^i \bmod n, 3^i \bmod n)$ is periodic with period $\operatorname{lcm}(\operatorname{ord}_n(2), \operatorname{ord}_n(3))$.

**Proof.** By Euler's theorem, $a^{\varphi(n)} \equiv 1 \pmod{n}$ for $\gcd(a,n)=1$, so the multiplicative order $\operatorname{ord}_n(a)$ divides $\varphi(n)$. The sequence $a^i \bmod n$ is periodic with this order (after the pre-period, which is at most $v_p(n)$ for each prime $p$ dividing both $a$ and $n$). The joint periodicity follows from the Chinese Remainder Theorem structure. Since we generate $2n+1$ terms and the period divides $\varphi(n) \le n$, all distinct stations appear within the first period. $\square$

**Theorem 2 (Patience Sorting for LNDS).** The LNDS of a sequence of length $M$ can be computed in $O(M \log M)$ time using the patience sorting algorithm with binary search (upper bound variant).

**Proof.** Maintain an array $\mathrm{tails}[]$ where $\mathrm{tails}[j]$ is the smallest possible last element of a non-decreasing subsequence of length $j+1$ found so far. For each new element $v$, use binary search to find the first position $p$ where $\mathrm{tails}[p] > v$ (i.e., $\operatorname{upper\_bound}$). If $p$ is past the end, append $v$; otherwise set $\mathrm{tails}[p] = v$. The invariant that $\mathrm{tails}[]$ is non-decreasing is maintained at each step, and each operation costs $O(\log M)$ via binary search. The final length of $\mathrm{tails}[]$ equals the LNDS length. The correctness follows from the standard patience sorting theory (see Aldous and Diaconis, 1999). $\square$

## Algorithm

```
function S(n):
    // Step 1: Generate stations
    stations = empty set
    x = 1, y = 1
    for i = 0 to 2n:
        stations.add((x mod n, y mod n))
        x = (x * 2) mod n
        y = (y * 3) mod n

    // Step 2: Sort lexicographically by (x, y)
    sorted_stations = sort(stations) by (x ascending, y ascending)

    // Step 3: Extract y-values
    Y = [s.y for s in sorted_stations]

    // Step 4: Compute LNDS via patience sorting
    tails = []
    for v in Y:
        p = upper_bound(tails, v)
        if p == len(tails):
            tails.append(v)
        else:
            tails[p] = v
    return len(tails)

function solve():
    total = 0
    for k = 1 to 30:
        total += S(k^5)
    return total
```

## Complexity Analysis

- **Station generation:** $O(n)$ per value of $n$ (with early termination via cycle detection, potentially $O(\operatorname{lcm}(\operatorname{ord}_n(2), \operatorname{ord}_n(3)))$).
- **Sorting:** $O(M \log M)$ where $M \le \min(2n+1, n^2)$ is the number of distinct stations.
- **LNDS computation:** $O(M \log M)$.
- **Per $k$:** $O(k^5 \log k^5) = O(k^5 \cdot 5\log k)$ in the worst case.
- **Total time:** $O\!\left(\sum_{k=1}^{30} k^5 \log k\right)$. Dominated by $k=30$: $n = 30^5 = 24\,300\,000$.
- **Space:** $O(M)$ for storing stations and the tails array.

## Answer

$$\boxed{9936352}$$

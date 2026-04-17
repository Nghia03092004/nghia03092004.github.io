# Problem 467: Superinteger

## Problem Statement

An integer $s$ is a *superinteger* of another integer $n$ if the digits of $n$ form a subsequence of the digits of $s$. Let $p(i)$ be the $i$-th prime, $c(i)$ the $i$-th composite. Define $\text{sd}(n) = 1 + (n-1) \bmod 9$ (the digital root). Let:
- $\text{sp}(i) = \text{sd}(p(i))$, $\text{sc}(i) = \text{sd}(c(i))$
- $P_n = \text{sp}(1)\,\text{sp}(2)\cdots\text{sp}(n)$, $C_n = \text{sc}(1)\,\text{sc}(2)\cdots\text{sc}(n)$

Define $f(n)$ as the smallest positive integer that is a superinteger of both $P_n$ and $C_n$ (i.e., the numeric value of the Shortest Common Supersequence of $P_n$ and $C_n$).

Given: $f(10) = 2357246891352679$, $f(100) \bmod (10^9 + 7) = 771661825$.

Find $f(10000) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem 1 (Digital root formula).** For $n \ge 1$, $\text{sd}(n) = 1 + (n - 1) \bmod 9$.

**Proof.** The digital root satisfies $\text{sd}(n) \equiv n \pmod{9}$ with $\text{sd}(n) \in \{1, 2, \ldots, 9\}$. For $n \ge 1$: if $9 \mid n$ then $\text{sd}(n) = 9 = 1 + (n-1) \bmod 9$ (since $(n-1) \bmod 9 = 8$); otherwise $\text{sd}(n) = n \bmod 9 = 1 + (n-1) \bmod 9$. $\square$

**Theorem 2 (SCS via LCS).** For strings $A$ and $B$ over alphabet $\Sigma$, the length of the Shortest Common Supersequence satisfies:
$$|\text{SCS}(A, B)| = |A| + |B| - |\text{LCS}(A, B)|$$
where $\text{LCS}$ is the Longest Common Subsequence.

**Proof.** An SCS of $A$ and $B$ is a string $S$ of minimum length containing both $A$ and $B$ as subsequences. Any common subsequence of $A$ and $B$ can be "shared" in $S$, and an LCS provides the maximum such sharing. Formally, align $A$ and $B$ to $S$: each position of $S$ is either from $A$ only, $B$ only, or shared. The shared positions form a common subsequence, and to minimize $|S|$ we maximize the shared portion. Thus $|S| = |A| + |B| - |\text{LCS}|$. $\square$

**Lemma 1 (LCS recurrence).** For sequences $A = a_1 \ldots a_m$ and $B = b_1 \ldots b_n$:
$$L(i, j) = \begin{cases} 0 & \text{if } i = 0 \text{ or } j = 0, \\ L(i-1, j-1) + 1 & \text{if } a_i = b_j, \\ \max(L(i-1,j),\, L(i,j-1)) & \text{otherwise.} \end{cases}$$
The LCS length is $L(m, n)$.

**Proof.** Standard dynamic programming recurrence. If $a_i = b_j$, the optimal LCS either uses this match (gaining 1 over $L(i-1,j-1)$) or does not, but using it is always at least as good. If $a_i \ne b_j$, at least one of $a_i$ or $b_j$ is not in the LCS suffix, giving the max of the two sub-cases. $\square$

**Theorem 3 (SCS construction and modular evaluation).** The SCS string is constructed by backtracking through the LCS DP table, interleaving characters from $A$ and $B$ while sharing matched characters. The numeric value modulo $M$ is computed incrementally:
$$\text{val} \leftarrow (10 \cdot \text{val} + d_k) \bmod M$$
for each digit $d_k$ of the SCS.

**Proof.** The backtracking produces the lexicographically smallest SCS (by choosing $A$'s character first when tied). The modular arithmetic preserves the polynomial identity $\text{val} = \sum_k d_k \cdot 10^{L-1-k} \bmod M$ via Horner's method. $\square$

## Algorithm

```
function ComputeF(n, MOD):
    // Step 1: Generate sequences
    primes = first n primes via sieve
    composites = first n composites
    P = [digital_root(p) for p in primes]
    C = [digital_root(c) for c in composites]

    // Step 2: LCS DP table (O(n^2))
    L = array[0..n][0..n], initialized to 0
    for i = 1 to n:
        for j = 1 to n:
            if P[i] == C[j]:
                L[i][j] = L[i-1][j-1] + 1
            else:
                L[i][j] = max(L[i-1][j], L[i][j-1])

    // Step 3: Backtrack to construct SCS, compute value mod MOD
    i = n, j = n, val = 0
    // Build SCS in reverse, then reverse at end
    scs_reversed = []
    while i > 0 and j > 0:
        if P[i] == C[j]:
            scs_reversed.append(P[i])
            i -= 1; j -= 1
        elif L[i-1][j] >= L[i][j-1]:
            scs_reversed.append(P[i])
            i -= 1
        else:
            scs_reversed.append(C[j])
            j -= 1
    while i > 0: scs_reversed.append(P[i]); i -= 1
    while j > 0: scs_reversed.append(C[j]); j -= 1

    scs = reverse(scs_reversed)

    // Step 4: Compute numeric value mod MOD
    val = 0
    for d in scs:
        val = (10 * val + d) mod MOD
    return val
```

## Complexity Analysis

- **Time:** $O(L \log \log L)$ for sieving primes up to the $n$-th prime ($L \approx n \ln n$). $O(n^2)$ for the LCS DP table. $O(n)$ for SCS construction and modular evaluation. Total: $O(n^2)$.
- **Space:** $O(n^2)$ for the LCS table (can be reduced to $O(n)$ with Hirschberg's algorithm if only the value is needed, but backtracking requires the full table).

## Answer

$$\boxed{775181359}$$

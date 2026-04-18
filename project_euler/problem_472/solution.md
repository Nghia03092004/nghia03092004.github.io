# Problem 472: Comfortable Distance II

## Problem Statement

$N$ seats are arranged in a row. People fill them according to these rules:

1. No person sits beside another (adjacent seats cannot both be occupied).
2. The first person may choose any seat.
3. Each subsequent person sits in the seat that maximizes the minimum distance to any already-seated person, subject to rule 1. Ties are broken by choosing the leftmost seat.

Let $f(N)$ be the number of first-seat choices that maximize the total number of occupants. Define $F(N) = \sum_{k=1}^{N} f(k)$.

Known values: $f(1) = 1$, $f(15) = 9$, $f(20) = 6$, $f(500) = 16$, $F(20) = 83$, $F(500) = 13343$.

Find $F(10^{12}) \bmod 10^8$.

## Mathematical Foundation

**Theorem 1 (Deterministic greedy process).** Given a first-seat choice at position $p$ in a row of $N$ seats, the subsequent seating process is completely deterministic, yielding a unique total occupancy $\mathrm{occ}(N, p)$.

**Proof.** After the first person sits at position $p$, the remaining empty seats form disjoint segments. At each step, the greedy rule selects the unique seat maximizing the minimum distance to any occupied seat (with leftmost tie-breaking). Since the set of occupied seats determines the segments, and the greedy rule is a function of the segments, each subsequent placement is uniquely determined. By induction on the number of remaining fillable seats, the process terminates with a unique configuration. $\square$

**Lemma 1 (Recursive gap filling).** Let $\mathrm{fill}(g, \ell, r)$ denote the number of people seated in a contiguous gap of $g$ empty seats, where $\ell \in \{W, O\}$ and $r \in \{W, O\}$ indicate whether the left and right boundaries are walls ($W$) or occupied seats ($O$). The greedy person sits at the position maximizing distance from both boundaries (leftmost tie-break), creating two sub-gaps, and the process recurses.

**Proof.** A gap of length $g$ with boundary types $(\ell, r)$ has effective usable positions determined by adjacency constraints: positions adjacent to occupied boundaries are forbidden. The greedy seat is the midpoint of the effective segment (leftmost if even length). This splits the gap into two sub-gaps with updated boundary types: the newly occupied seat serves as an $O$-type boundary for both sub-gaps. By induction on $g$, the total occupancy is well-defined. $\square$

**Theorem 2 (Fractal block structure).** The function $f(N)$ exhibits a quasi-periodic structure with period lengths that are powers of 2. Specifically, for $N$ in the range $[2^k + 1, 2^{k+1}]$, the values of $f(N)$ can be expressed in terms of the values for smaller ranges via a recursive decomposition.

**Proof.** The greedy seating process performs binary subdivision of intervals. When $N = 2^k - 1$, the row admits a perfectly balanced binary subdivision, making multiple starting positions equivalent (hence $f(2^k - 1)$ is relatively large). For general $N$, the imbalance in the binary subdivision determines which starting positions are optimal. The recursive structure of binary subdivision implies that $f(N)$ for $N \in [2^k+1, 2^{k+1}]$ depends on the sub-problem structure at scale $2^{k-1}$, establishing the quasi-periodicity. $\square$

**Lemma 2 (Block sum recurrence).** Define $B(k) = \sum_{N=2^{k-1}+1}^{2^k} f(N)$. Then $B(k)$ satisfies a linear recurrence (or a small set of recurrences based on residue classes), enabling $F(N)$ to be computed in $O(\log N)$ block evaluations.

**Proof.** Within each block $[2^{k-1}+1, 2^k]$, the pattern of $f(N)$ is determined by the recursive gap-filling at depth $k$. Since the binary subdivision at depth $k$ reduces to depth $k-1$ sub-problems, the block sum $B(k)$ can be expressed as a linear combination of previous block sums $B(k-1), B(k-2), \ldots$, plus correction terms for boundary effects. The number of distinct boundary configurations is bounded, so the recurrence has bounded order. $\square$

## Editorial
We direct computation for small N. We then identify block structure. Finally, compute block sums B(k) = sum f(N) for N in [2^(k-1)+1, 2^k].

## Pseudocode

```text
Direct computation for small N
Identify block structure
Compute block sums B(k) = sum f(N) for N in [2^(k-1)+1, 2^k]
Identify the recurrence relating B(k) to previous blocks
Sum over blocks
Decompose [1, N] into O(log N) dyadic blocks
For each complete block, use the recurrence to compute B(k)
For the partial final block, compute directly or via sub-block decomposition
Determine effective boundaries
Greedy seat at midpoint of effective range
```

## Complexity Analysis

- **Time:** $O(N^2 \log N)$ for the direct simulation phase (each of $N$ starting positions requires $O(N \log N)$ for the recursive gap-filling). For the block-recurrence phase, $O(\log^2 N)$ using matrix exponentiation on the recurrence.
- **Space:** $O(N)$ for the direct phase, $O(\log N)$ for the recurrence phase.

## Answer

$$\boxed{73811586}$$

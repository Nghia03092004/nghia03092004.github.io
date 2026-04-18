# Problem 122: Efficient Exponentiation

## Problem Statement

The most efficient way to compute $x^n$ uses the minimum number of multiplications, where each multiplication computes $x^{a_i + a_j}$ from previously computed powers $x^{a_i}$ and $x^{a_j}$. This minimum equals the length of the shortest addition chain for $n$. Find $\sum_{n=1}^{200} m(n)$, where $m(n)$ is the shortest addition chain length for $n$.

## Mathematical Development

**Definition 1 (Addition chain).** An *addition chain* for a positive integer $n$ is a finite sequence $1 = a_0 < a_1 < \cdots < a_r = n$ such that for each $1 \le k \le r$, there exist indices $0 \le i \le j < k$ with $a_k = a_i + a_j$. The *length* of the chain is $r$.

**Definition 2.** Let $m(n)$ denote the length of a shortest addition chain for $n$. By convention, $m(1) = 0$.

**Theorem 1 (Lower bound).** *For all $n \ge 1$, $m(n) \ge \lceil \log_2 n \rceil$.*

*Proof.* At each step $k$, since $a_k = a_i + a_j \le 2a_{k-1}$ (the largest element can at most double), we have by induction $a_k \le 2^k a_0 = 2^k$. In particular, $n = a_r \le 2^r$, so $r \ge \log_2 n$. Since $r$ is an integer, $r \ge \lceil \log_2 n \rceil$. $\square$

**Theorem 2 (Upper bound via binary method).** *For all $n \ge 1$, $m(n) \le \lfloor \log_2 n \rfloor + \nu(n) - 1$, where $\nu(n)$ denotes the number of $1$-bits (Hamming weight) of $n$.*

*Proof.* The binary method scans the binary representation $n = (b_s b_{s-1} \cdots b_1 b_0)_2$ (with $b_s = 1$) from the most significant bit to the least. For each bit position $i$ from $s-1$ down to $0$: perform a doubling (add the current value to itself). If $b_i = 1$, additionally add $a_0 = 1$. This produces a valid addition chain with $\lfloor \log_2 n \rfloor$ doublings and $\nu(n) - 1$ small additions (one per 1-bit excluding the leading bit). $\square$

**Proposition 1 (Binary method is not always optimal).** *$m(15) = 5$, but the binary method uses $\lfloor \log_2 15 \rfloor + \nu(15) - 1 = 3 + 4 - 1 = 6$ steps.*

*Proof.* The chain $1, 2, 3, 6, 12, 15$ has length 5 and is valid: $2 = 1+1$, $3 = 1+2$, $6 = 3+3$, $12 = 6+6$, $15 = 3+12$. By exhaustive search (or by noting that $m(15) \ge \lceil \log_2 15 \rceil = 4$ and verifying that no chain of length 4 reaches 15), $m(15) = 5$. $\square$

**Theorem 3 (IDDFS optimality).** *Iterative deepening depth-first search (IDDFS) with depth limit starting at $\lceil \log_2 n \rceil$ finds a shortest addition chain for any $n$.*

*Proof.* IDDFS is complete (it explores all chains of each length $r$ before incrementing $r$) and optimal for uniform-cost search (the first solution found is of minimum length). The search space is finite since all chain elements lie in $\{1, \ldots, n\}$, and the chain is strictly increasing, bounding the depth by $n - 1$. $\square$

**Lemma 1 (Pruning criterion).** *Let the current chain be $[a_0, \ldots, a_d]$ with depth limit $r$. If $a_d \cdot 2^{r-d} < n$, no extension of this chain can reach $n$, and the branch may be pruned.*

*Proof.* Each of the remaining $r - d$ steps can at most double the current maximum. After $r - d$ doublings the maximum attainable value is $a_d \cdot 2^{r-d}$. If this is strictly less than $n$, the target is unreachable from this state. $\square$

## Editorial
Uses iterative deepening DFS with doubling-based pruning. We perform a recursive search over the admissible choices, prune branches that violate the derived constraints, and keep only the candidates that satisfy the final condition.

## Pseudocode

```text
    m[1] = 0
    For n from 2 to N:
        For r from ceil(log2(n)) to infinity:
            If IDDFS(chain=[1], target=n, depth_limit=r) then
                m[n] = r
                break
    Return sum(m[1..N])

    d = len(chain) - 1
    a_d = chain[d]
    If d == depth_limit then
        Return (a_d == target)
    If a_d << (depth_limit - d) < target then
        Return false // pruning
    For i from d down to 0:
        next_val = a_d + chain[i]
        If next_val <= a_d or next_val > target then
            continue
        chain.append(next_val)
        If IDDFS(chain, target, depth_limit) then
            Return true
        chain.pop()
    Return false
```

## Complexity Analysis

- **Time:** The branching factor at depth $d$ is at most $d + 1$, and the maximum depth is $r \le 11$ for $n \le 200$. Worst-case time per $n$ is $O(r^r)$, but the pruning criterion drastically reduces the effective search tree. Empirically, the total time for all $n \le 200$ is very modest.
- **Space:** $O(r)$ for the DFS stack and current chain, where $r \le 11$.

## Answer

$$\boxed{1582}$$

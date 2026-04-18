# Problem 375: Minimum of Subsequences

## Problem Statement

Define the pseudo-random sequence $\{S_n\}$ by $S_0 = 290797$ and $S_{n+1} = S_n^2 \bmod 50515093$. Let $A(i,j) = \min(S_i, S_{i+1}, \ldots, S_j)$ for $1 \le i \le j$. Compute:

$$M(N) = \sum_{1 \le i \le j \le N} A(i,j)$$

for $N = 2{,}000{,}000{,}000$. Verification: $M(10) = 432256955$, $M(10000) = 3264567774119$.

## Mathematical Foundation

**Theorem (Incremental Decomposition).** Define $R(j) = \sum_{i=1}^{j} A(i,j)$. Then $M(N) = \sum_{j=1}^{N} R(j)$, and the sequence $\{R(j)\}$ satisfies the recurrence:

$$R(j+1) = R(j) - \sum_{\substack{(v,c) \text{ popped}}} v \cdot c + S_{j+1} \cdot (c_{\text{total}} + 1)$$

where the "popped" elements are those removed from a monotone stack when inserting $S_{j+1}$, and $c_{\text{total}}$ is the total count of starting indices they collectively represented.

**Proof.** Consider extending all subsequences ending at position $j$ by one element $S_{j+1}$, and adding the new singleton $(j+1, j+1)$. For a subsequence starting at position $i$:

$$A(i, j+1) = \min(A(i,j),\; S_{j+1}).$$

If $A(i,j) \ge S_{j+1}$, then the new minimum becomes $S_{j+1}$, reducing the contribution by $A(i,j) - S_{j+1}$. If $A(i,j) < S_{j+1}$, the minimum is unchanged. Plus we add the singleton contribution $S_{j+1}$.

The monotone stack tracks contiguous groups of starting indices sharing the same current minimum. Popping entries with value $\ge S_{j+1}$ replaces their contributions with $S_{j+1} \cdot c$ for each popped group of count $c$. $\square$

**Lemma (Monotone Stack Invariant).** Maintain a stack of pairs $(v, c)$ sorted in strictly increasing order of $v$ from bottom to top, where $c$ is the number of starting indices whose current minimum is $v$. The invariant is:

$$R(j) = \sum_{(v,c) \in \text{stack}} v \cdot c.$$

**Proof.** By induction on $j$. Base: $j = 1$, stack = $\{(S_1, 1)\}$, $R(1) = S_1$. Inductive step: when processing $S_{j+1}$, we pop all entries $(v, c)$ with $v \ge S_{j+1}$. Their combined count $c_{\text{total}}$ is accumulated, and a single new entry $(S_{j+1}, c_{\text{total}} + 1)$ is pushed (the $+1$ accounts for the new singleton). The value $R(j+1)$ equals the previous $R(j)$ minus the popped contributions plus $S_{j+1} \cdot (c_{\text{total}} + 1)$, maintaining the invariant. $\square$

**Lemma (Amortized Complexity).** Each element is pushed exactly once and popped at most once, so the total number of stack operations over $N$ steps is at most $2N$.

**Proof.** Each of the $N$ insertions pushes exactly one entry. An entry can only be popped once (after which it is gone). Since at most $N$ entries are ever pushed, at most $N$ pops occur. Total operations $\le 2N$. $\square$

## Editorial
S_0 = 290797, S_{n+1} = S_n^2 mod 50515093 A(i,j) = min(S_i, ..., S_j) M(N) = sum of A(i,j) for all 1 <= i <= j <= N N = 2,000,000,000 The pseudo-random sequence has a cycle of length 6,308,948. We use a monotone stack for O(N) computation. Verification: M(10) = 432256955, M(10000) = 3264567774119 Note: Running this in Python for N=2e9 is very slow (~hours). Use the C++ solution for the full computation.

## Pseudocode

```text
    S = 290797
    stack = [] // list of (value, count) pairs
    R = 0 // current R(j)
    T = 0 // running total M(N)
    For j from 1 to N:
        S = S * S mod 50515093
        c = 0
        While stack is not empty and stack.top().value >= S:
            (v, cnt) = stack.pop()
            R -= v * cnt
            c += cnt
        stack.push((S, c + 1))
        R += S * (c + 1)
        T += R
    Return T
```

## Complexity Analysis

- **Time:** $O(N)$ amortized. Each element is pushed and popped at most once; generating $S_n$ is $O(1)$ per step.
- **Space:** $O(N)$ worst case for the stack (if the sequence is strictly increasing). In practice, $O(\sqrt{N})$ expected for random-like sequences.

Note: $R$ fits in a 64-bit integer ($\sim 5 \times 10^{16}$), but $T$ can reach $\sim 10^{26}$, requiring 128-bit or arbitrary-precision integers.

## Answer

$$\boxed{7435327983715286168}$$

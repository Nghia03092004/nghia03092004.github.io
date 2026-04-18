# Problem 294: Sum of Digits -- Experience #23

## Problem Statement

For a positive integer $k$, let $d(k)$ denote the digit sum of $k$. Define

$$S(n) = \#\{k : 0 < k < 10^n,\; 23 \mid k,\; d(k) = 23\}.$$

Given: $S(9) = 263626$, $S(42) = 6{,}377{,}168{,}878{,}570{,}056$.

Find $S(11^{12}) \bmod 10^9$.

## Mathematical Foundation

**Theorem 1 (Digit DP State Space).** *The number of integers $0 \leq k < 10^n$ satisfying $d(k) = 23$ and $23 \mid k$ equals the $(s=23, r=0)$ entry of the vector $T^n \mathbf{v}_0$, where $T$ is a $552 \times 552$ transition matrix and $\mathbf{v}_0$ is the initial state vector with a $1$ at position $(s=0, r=0)$.*

**Proof.** We encode each partial number after processing $i$ digits (from most significant to least significant) by the state $(s, r)$ where $s \in \{0, 1, \ldots, 23\}$ is the running digit sum and $r \in \{0, 1, \ldots, 22\}$ is the running value modulo 23. The state space has $24 \times 23 = 552$ elements.

When we append digit $d \in \{0, 1, \ldots, 9\}$, the state transitions as:
$$(s, r) \xrightarrow{d} (s + d,\; (10r + d) \bmod 23) \quad \text{provided } s + d \leq 23.$$

The transition matrix $T$ has entry $T_{(s', r'), (s, r)} = \#\{d \in \{0,\ldots,9\} : s' = s + d,\; r' = (10r + d) \bmod 23\}$. Starting from state $(0, 0)$ and applying $T$ exactly $n$ times, the count of valid $k$ is the entry at state $(23, 0)$. Since $d(0) = 0 \neq 23$, the case $k = 0$ is automatically excluded. $\square$

**Theorem 2 (Matrix Exponentiation).** *For any $n \times n$ matrix $T$ and positive integer $N$, the matrix power $T^N$ can be computed in $O(n^3 \log N)$ arithmetic operations via repeated squaring.*

**Proof.** Write $N$ in binary: $N = \sum_{i=0}^{\lfloor \log_2 N \rfloor} b_i 2^i$. Compute $T, T^2, T^4, \ldots, T^{2^{\lfloor \log_2 N \rfloor}}$ by successive squaring ($\lfloor \log_2 N \rfloor$ matrix multiplications). Then $T^N = \prod_{i : b_i = 1} T^{2^i}$, requiring at most $\lfloor \log_2 N \rfloor$ additional matrix multiplications. Each matrix multiplication costs $O(n^3)$. $\square$

**Lemma 1 (Modular Computation).** *Since all entries of $T$ are non-negative integers and we compute modulo $10^9$, all intermediate results remain in $[0, 10^9)$ and the final result equals $S(11^{12}) \bmod 10^9$.*

**Proof.** The entries of $T^n$ count paths in the state graph, which are non-negative integers. By properties of modular arithmetic, $(A \cdot B) \bmod m = ((A \bmod m) \cdot (B \bmod m)) \bmod m$, so reducing modulo $10^9$ at each multiplication step yields the correct result modulo $10^9$. $\square$

## Editorial
S(n) = count of positive integers k < 10^n with 23 | k and digit sum d(k) = 23. Find S(11^12) mod 10^9. Approach: Matrix exponentiation on the digit DP transition matrix. State: (digit_sum, value mod 23), dimension 24 * 23 = 552. We build 552 x 552 transition matrix T. We then states indexed as (s, r) -> s * 23 + r. Finally, compute T^n mod MOD via repeated squaring.

## Pseudocode

```text
Build 552 x 552 transition matrix T
States indexed as (s, r) -> s * 23 + r
Compute T^n mod MOD via repeated squaring
Answer is entry (23*23 + 0, 0*23 + 0) = (529, 0)
```

## Complexity Analysis

- **Time:** $O(552^3 \cdot \log_2(11^{12}))$. We have $\log_2(11^{12}) = 12 \log_2(11) \approx 41.5$, so approximately 42 matrix squarings. Each matrix multiplication costs $552^3 \approx 1.68 \times 10^8$ operations. Total: $\approx 42 \times 1.68 \times 10^8 \approx 7.1 \times 10^9$ modular multiplications.
- **Space:** $O(552^2) = O(304{,}704)$ for storing the matrix, i.e., a few megabytes.

## Answer

$$\boxed{789184709}$$

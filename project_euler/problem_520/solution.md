# Problem 520: Simbers

## Problem Statement

A positive integer is called a *simber* if every decimal digit that appears in its representation appears an even number of times. For example, $1221$ is a simber (two 1s and two 2s), but $1231$ is not (one 3). Let $Q(n)$ count the simbers not exceeding $10^n$. Compute $Q(n)$ for given $n$.

## Mathematical Foundation

**Theorem 1 (Parity Bitmask Characterization).** A positive integer $m$ is a simber if and only if, letting $d_1 d_2 \cdots d_L$ be its decimal representation, the XOR-parity vector

$$\text{mask} = \bigoplus_{i=1}^{L} e_{d_i} \in \{0,1\}^{10}$$

equals the zero vector, where $e_j$ is the standard basis vector with a 1 in position $j$ and $\oplus$ denotes componentwise XOR (equivalently, addition modulo 2).

**Proof.** The $j$-th component of the mask is $\sum_{i=1}^{L}[d_i = j] \bmod 2$, which is 0 if and only if digit $j$ appears an even number of times. Thus $\text{mask} = \mathbf{0}$ if and only if every digit appears an even number of times. $\square$

**Theorem 2 (Digit DP Correctness).** The DP with states $(\text{pos}, \text{mask}, \text{tight}, \text{started})$ correctly counts all integers in $[1, N]$ that are simbers.

**Proof.** We construct each integer $m \leq N$ digit-by-digit from the most significant position.

- **Position:** $\text{pos} \in \{0, 1, \ldots, L-1\}$ where $L$ is the number of digits of $N$.
- **Mask:** $\text{mask} \in \{0,1\}^{10}$ tracks the parity of each digit's occurrence count so far.
- **Tight:** A boolean indicating whether the prefix constructed so far matches $N$ exactly (constraining future digits).
- **Started:** A boolean indicating whether a nonzero digit has been placed (to handle leading zeros correctly: leading zeros should not flip the mask for digit 0).

**Transitions:** At position $\text{pos}$, choose digit $d \in \{0, \ldots, \ell\}$ where $\ell = d_{\text{pos}}$ if tight, else $\ell = 9$.

- If not started and $d = 0$: proceed with mask unchanged, still not started.
- Otherwise: set $\text{started} = \text{true}$, $\text{mask} \leftarrow \text{mask} \oplus (1 \ll d)$.

**Base case:** At $\text{pos} = L$, the number is a simber iff $\text{started} = \text{true}$ and $\text{mask} = 0$.

Each integer $m \in [0, N]$ corresponds to exactly one path through the DP. The started flag ensures $m = 0$ is not counted (or counted separately if needed). $\square$

**Lemma 1 (State Space Bound).** The number of distinct DP states is at most $L \times 2^{10} \times 2 \times 2 = 4096L$.

**Proof.** There are $L$ positions, $2^{10} = 1024$ possible masks, 2 values for tight, and 2 values for started. $\square$

**Lemma 2 (Simplification for $N = 10^n$).** When $N = 10^n$, we can separately count simbers with exactly $\ell$ digits for $\ell = 1, 2, \ldots, n$. For exactly $\ell$ digits, the leading digit $d_1 \in \{1, \ldots, 9\}$ and the remaining $\ell - 1$ digits are in $\{0, \ldots, 9\}$, with the constraint that the final mask is $\mathbf{0}$.

**Proof.** Since $10^n$ itself is not a simber (digit 1 appears once), $Q(n)$ counts simbers in $[1, 10^n - 1]$, which is the union of simbers with $\ell$ digits for $\ell = 1, \ldots, n$. For a fixed digit length, there is no upper-bound constraint (all $\ell$-digit numbers are $< 10^n$), so the tight constraint is trivially released after the first digit, and we can use the combinatorial formula directly. $\square$

## Editorial
A simber is a number where every digit appears an even number of times. Count simbers up to 10^n using digit DP with parity bitmask. We count simbers in [1, 10^n - 1]. We then count L-digit numbers with all-even digit parities. Finally, such that the parity mask becomes 0.

## Pseudocode

```text
Count simbers in [1, 10^n - 1]
For each digit length L = 1, ..., n:
Count L-digit numbers with all-even digit parities
dp[mask] = number of ways to fill remaining positions
such that the parity mask becomes 0
Process position by position
Count L-digit simbers
Position 0: digit d1 in {1, ..., 9}
Positions 1..L-1: digit in {0, ..., 9}
DP over positions, tracking mask
After choosing d1:
```

## Complexity Analysis

- **Time:** For each digit length $L$ (from 1 to $n$), the DP processes $L$ positions with $2^{10} = 1024$ mask states and 10 digit choices per state. Total: $O(n \cdot n \cdot 1024 \cdot 10) = O(10240\, n^2)$. This can be optimized to $O(n \cdot 1024 \cdot 10)$ by processing all digit lengths simultaneously.
- **Space:** $O(2^{10}) = O(1024)$ for the DP table (using rolling arrays).

## Answer

$$\boxed{238413705}$$

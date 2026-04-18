# Problem 250: 250250

## Problem Statement

Find the number of non-empty subsets of $\{1^1, 2^2, 3^3, \ldots, 250250^{250250}\}$ whose element sum is divisible by $250$. Give the answer modulo $10^{16}$.

## Mathematical Foundation

**Theorem 1 (Residue reduction).** For divisibility of a sum by $250$, only the residues $r_i = i^i \bmod 250$ matter. That is, a subset $T \subseteq \{1, \ldots, 250250\}$ has $\sum_{i \in T} i^i \equiv 0 \pmod{250}$ if and only if $\sum_{i \in T} r_i \equiv 0 \pmod{250}$.

**Proof.** For any integers $a_i$, $\sum a_i \equiv \sum (a_i \bmod m) \pmod{m}$. Apply with $m = 250$ and $a_i = i^i$. $\square$

**Theorem 2 (DP on residue classes).** Let $dp[j]$ denote the number of subsets (including the empty set) of $\{1^1, \ldots, N^N\}$ whose element sum is congruent to $j \pmod{250}$, reduced modulo $M = 10^{16}$. The recurrence
$$dp'[j] = dp[j] + dp[(j - r_i) \bmod 250]$$
processed for $i = 1, 2, \ldots, N$ correctly computes $dp[j]$ for all $0 \le j < 250$.

**Proof.** Induction on the number of elements processed. Initially, $dp[0] = 1$ (empty set), $dp[j] = 0$ for $j \ne 0$.

For element $i$ with residue $r_i$: every subset of $\{1, \ldots, i\}$ either excludes $i$ (counted by old $dp[j]$) or includes $i$ (counted by old $dp[(j - r_i) \bmod 250]$, since including $i$ adds $r_i$ to the residue). We must update all 250 entries simultaneously (using a temporary copy or in-place with a fresh array) to avoid double-counting.

By induction, after processing all $N$ elements, $dp[j]$ equals (mod $M$) the number of subsets with sum $\equiv j \pmod{250}$. $\square$

**Lemma 1 (Periodicity of residues).** The sequence $r_i = i^i \bmod 250$ has period dividing $500$.

**Proof.** By CRT, $250 = 2 \times 125$. We show periodicity modulo each factor:
- Mod 2: $i^i \bmod 2$ depends only on $i \bmod 2$, period 2.
- Mod 125: Write $i = 125q + r$. For $\gcd(i, 125) = 1$, $i^i \bmod 125$ depends on $i \bmod 125$ (for the base) and $i \bmod \varphi(125) = i \bmod 100$ (for the exponent). Since $\text{lcm}(125, 100) = 500$, the period divides 500. For $5 \mid i$, similar analysis via lifting-the-exponent gives the same bound.
- Combined: $\text{lcm}(2, 500) = 500$. $\square$

**Theorem 3 (Final answer).** The number of non-empty subsets with sum divisible by 250 is
$$\text{answer} = (dp[0] - 1) \bmod 10^{16}$$
where $dp[0]$ counts all subsets (including empty) with sum $\equiv 0$.

**Proof.** $dp[0]$ includes the empty subset, which has sum 0. Subtracting 1 excludes it. $\square$

## Editorial
.., 250250^250250} with sum divisible by 250. DP on residue classes mod 250. Answer mod 10^16. Key: Only need i^i mod 250 for each element. DP array of size 250. We use fast modular exponentiation. We then dP on residue classes. Finally, subtract empty subset.

## Pseudocode

```text
Compute residues r_i = i^i mod 250
Use fast modular exponentiation
DP on residue classes
Subtract empty subset
```

## Complexity Analysis

- **Time:** $O(N \times 250) = O(250250 \times 250) \approx 6.25 \times 10^7$ modular additions. Computing $r_i$ takes $O(N \log N)$ via modular exponentiation ($O(\log i)$ per element). Total: $O(N \cdot 250)$ dominates.
- **Space:** $O(250)$ for the DP array.

## Answer

$$\boxed{1425480602091519}$$

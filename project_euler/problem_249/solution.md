# Problem 249: Prime Subset Sums

## Problem Statement

Let $S = \{2, 3, 5, 7, 11, \ldots\}$ be the set of all primes less than $5000$. Find the number of subsets of $S$ whose elements sum to a prime number. Give the last 16 digits (i.e., the answer modulo $10^{16}$).

## Mathematical Foundation

**Theorem 1 (Subset-sum counting via 0/1 knapsack DP).** Let $p_1 < p_2 < \cdots < p_K$ be the primes less than $5000$ (where $K = 669$), and let $\sigma = \sum_{i=1}^{K} p_i = 1{,}548{,}136$. Define $dp[s]$ as the number of subsets of $\{p_1, \ldots, p_K\}$ that sum to exactly $s$, reduced modulo $M = 10^{16}$. Then the standard 0/1 knapsack recurrence correctly computes $dp[s]$ for all $0 \le s \le \sigma$.

**Proof.** We prove by induction on $j$ that after processing primes $p_1, \ldots, p_j$, $dp[s]$ equals (mod $M$) the number of subsets of $\{p_1, \ldots, p_j\}$ summing to $s$.

*Base case ($j = 0$):* $dp[0] = 1$ (empty subset), $dp[s] = 0$ for $s > 0$. Correct.

*Inductive step:* Assume the claim holds after $j - 1$ primes. When processing $p_j$, for $s$ from $\sigma$ down to $p_j$, we update $dp[s] \leftarrow dp[s] + dp[s - p_j]$. The reverse iteration ensures that the value $dp[s - p_j]$ used is from the $(j-1)$-th stage (before $p_j$ was available). Thus:
- $dp[s]$ after update $=$ (subsets of $\{p_1,\ldots,p_{j-1}\}$ summing to $s$) $+$ (subsets of $\{p_1,\ldots,p_{j-1}\}$ summing to $s - p_j$).
The first term counts subsets not containing $p_j$; the second counts subsets containing $p_j$ (since adding $p_j$ to a subset summing to $s - p_j$ gives a subset summing to $s$). Together, these are exactly the subsets of $\{p_1,\ldots,p_j\}$ summing to $s$. $\square$

**Lemma 1 (Prime sieve correctness).** The Sieve of Eratosthenes correctly identifies all primes up to $N$ in $O(N \log \log N)$ time.

**Proof.** Classical result. Each composite $n \le N$ has a prime factor $p \le \sqrt{N}$, so it is marked during the sieve of $p$. No prime is ever marked. $\square$

**Theorem 2 (Final summation).** The answer is
$$\text{answer} = \sum_{\substack{2 \le s \le \sigma \\ s \text{ prime}}} dp[s] \bmod 10^{16}.$$

**Proof.** Each subset with prime sum contributes exactly 1 to $dp[s]$ for exactly one prime $s$. Summing $dp[s]$ over prime $s$ counts each qualifying subset once. $\square$

## Editorial
We collect primes below 5000. We then 0/1 Knapsack DP. Finally, sum over prime target sums. We use dynamic programming over the state space implied by the derivation, apply each admissible transition, and read the answer from the final table entry.

## Pseudocode

```text
Sieve primes up to sigma = 1,548,136
Collect primes below 5000
|primes_5000| = 669
0/1 Knapsack DP
Sum over prime target sums
```

## Complexity Analysis

- **Time:** The DP performs $K \times \sigma = 669 \times 1{,}548{,}136 \approx 1.04 \times 10^9$ additions modulo $10^{16}$. The sieve takes $O(\sigma \log \log \sigma)$. The final summation is $O(\sigma)$. Total: $O(K \cdot \sigma)$.
- **Space:** $O(\sigma) = O(1{,}548{,}136)$ for the DP array and sieve, approximately 12 MB for 64-bit entries.

## Answer

$$\boxed{9275262564250418}$$

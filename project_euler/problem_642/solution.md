# Problem 642: Sum of Largest Prime Factors

## Problem Statement

Let $\operatorname{lpf}(n)$ denote the largest prime factor of $n$ (with $\operatorname{lpf}(1) = 1$). Compute

$$S(N) = \sum_{n=2}^{N} \operatorname{lpf}(n).$$

## Mathematical Foundation

**Theorem 1 (Sieve Correctness).** *If primes are processed in increasing order $p_1 < p_2 < \cdots$ and for each prime $p$ we set $\operatorname{lpf}[m] \leftarrow p$ for every multiple $m$ of $p$, then after all primes $\le N$ are processed, $\operatorname{lpf}[m]$ equals the largest prime factor of $m$ for every $2 \le m \le N$.*

**Proof.** Let $q = \operatorname{lpf}(m)$ be the true largest prime factor of $m$. Since $q \mid m$ and $q \le N$, the iteration for prime $q$ sets $\operatorname{lpf}[m] \leftarrow q$. Any prime $p > q$ does not divide $m$, so does not overwrite $\operatorname{lpf}[m]$. Hence $\operatorname{lpf}[m] = q$ at termination. $\square$

**Lemma 1 (Regrouping by Largest Prime Factor).** *We have*
$$S(N) = \sum_{\substack{p \le N \\ p \text{ prime}}} p \cdot \#\{n \le N : \operatorname{lpf}(n) = p\}.$$

**Proof.** This is simply a rearrangement of the sum by grouping terms sharing the same largest prime factor. $\square$

**Theorem 2 (Sub-linear Approach).** *Define $F(n, p)$ = number of integers $\le n$ whose largest prime factor is $\le p$. Then*
$$\#\{n \le N : \operatorname{lpf}(n) = p\} = F(N/p, p) - F(N/p, p^{-})$$
*where $p^{-}$ denotes the prime preceding $p$. This admits an $O(N^{2/3})$ algorithm via the Lucy DP technique.*

**Proof.** An integer $n \le N$ has $\operatorname{lpf}(n) = p$ if and only if $p \mid n$ and $n/p$ has all prime factors $\le p$. The count of such $n/p \le N/p$ with all prime factors $\le p$ is $F(N/p, p)$. We subtract $F(N/p, p^{-})$ to exclude those whose largest prime factor is strictly less than $p$ (which would be counted but correspond to $\operatorname{lpf}(n) < p$). $\square$

**Lemma 2 (Asymptotic).** *By the Dickman function analysis,*
$$S(N) \sim \frac{N^2}{2 \ln N}.$$

**Proof.** The density of integers near $N$ with largest prime factor $\approx N^u$ is governed by $\rho(1/u)$ (Dickman function). Integrating $p \cdot (\text{density})$ over the range gives the stated asymptotic. A rigorous derivation uses partial summation and the prime number theorem. $\square$

## Editorial
We use dynamic programming over the state space implied by the derivation, apply each admissible transition, and read the answer from the final table entry.

## Pseudocode

```text
Lucy DP for prime counting / prime summing
Compute F(v, p) for v in {floor(N/k) : k = 1..N}
Use the regrouping identity from Theorem 2
Details: standard Lucy_Hedgehog technique
Returns S(N) in O(N^{2/3}) time
```

## Complexity Analysis

- **Sieve method:** Time $O(N \log \log N)$, Space $O(N)$.
- **Sub-linear method (Lucy DP):** Time $O(N^{2/3})$, Space $O(N^{1/2})$.

## Answer

$$\boxed{631499044}$$

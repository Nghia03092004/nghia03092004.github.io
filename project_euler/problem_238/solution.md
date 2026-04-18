# Problem 238: Infinite String Tour

## Problem Statement

Define the Blum Blum Shub (BBS) sequence:
$$s_0 = 14025256, \quad s_{n+1} = s_n^2 \bmod 20300713.$$

Concatenate all $s_n$ to form the infinite string $w = \text{"14025256741014958..."}$.

For a positive integer $k$, define:
$$p(k) = \begin{cases} 0 & \text{if no substring of } w \text{ has digit sum } k, \\ z & \text{if } z \text{ is the earliest starting position (1-indexed) of a substring with digit sum } k. \end{cases}$$

Given: $\sum_{k=1}^{10^3} p(k) = 4742$. Find $\sum_{k=1}^{2 \times 10^{15}} p(k)$.

## Mathematical Foundation

**Theorem (BBS Periodicity).** The sequence $\{s_n\}$ defined by $s_{n+1} = s_n^2 \bmod M$ with $M = 20300713 = 5003 \times 4057$ is purely periodic with period $\lambda = 2\,534\,198$ starting from $s_0$.

**Proof.** Since $M$ is a product of two distinct primes $p = 5003$ and $q = 4057$, the BBS generator has period $\lambda = \mathrm{lcm}(\lambda_p, \lambda_q)$ where $\lambda_p$ and $\lambda_q$ are the multiplicative orders of the squaring map modulo $p$ and $q$ respectively. These are computed as divisors of $\mathrm{lcm}(p-1, q-1)/2$. The period is verified computationally to be $2\,534\,198$. $\square$

**Lemma (Cumulative Digit Sum Structure).** Let $d_i$ be the $i$-th digit of $w$ (1-indexed) and $C(i) = \sum_{j=1}^{i} d_j$. Let $L$ be the total number of digits in one period of the BBS sequence and $\Delta = C(L)$ the total digit sum of one period. Then:

$$C(i + L) = C(i) + \Delta \quad \text{for all } i \geq 0.$$

**Proof.** Since the BBS sequence is periodic with period $\lambda$, the digit string repeats with period $L$ (the total length of one cycle's concatenation). Thus $d_{i+L} = d_i$ for all $i \geq 1$, and $C(i+L) - C(i) = \sum_{j=i+1}^{i+L} d_j = \sum_{j=1}^{L} d_j = \Delta$. $\square$

**Theorem (Digit Sum Achievability).** The digit sum of substring $w[z..z+m-1]$ is $C(z+m-1) - C(z-1)$. For starting position $z$, the set of achievable digit sums is

$$A(z) = \{C(z+m-1) - C(z-1) : m = 1, 2, 3, \ldots\}.$$

The value $p(k) = z$ iff $k \in A(z)$ and $k \notin A(z')$ for all $z' < z$.

**Proof.** By definition, $p(k)$ is the smallest $z$ such that some substring starting at $z$ has digit sum $k$. Since the digit sum of $w[z..z+m-1]$ is $C(z+m-1) - C(z-1)$, the set $A(z)$ captures all achievable sums from position $z$. The claim follows. $\square$

**Corollary (Periodic Extension).** Since $A(z)$ for $z$ in the $t$-th period is a shift of the corresponding position in the first period by $t \cdot \Delta$, we can process period by period, accumulating contributions from each period's starting positions.

## Editorial
BBS generator: s_0 = 14025256, s_{n+1} = s_n^2 mod 20300713 Concatenate to form w = "14025256741014958..." p(k) = earliest starting position z such that some substring starting at z has digit sum = k. If none exists, p(k) = 0. Find sum of p(k) for 0 < k <= 2*10^15. Key insight: p(k) = min_reach[k mod S] where min_reach[c] = min z such that (c + C[z-1]) mod S is in V. Process z in order; |U| drains quickly (geometric decay ~21% per step). Verification: sum p(k) for k=1..1000 = 4742. We generate one full period of BBS, compute digit string. We then compute cumulative sums for one period. Finally, iterate over each starting position z in [1, L], determine.

## Pseudocode

```text
K = 2 * 10^15
Generate one full period of BBS, compute digit string
Compute cumulative sums for one period
For each starting position z in [1, L], determine
which k values get p(k) = z (not achievable from earlier positions)
Use periodicity to extend across multiple periods
Positions in period t cover digit sums shifted by t * Delta
Sum p(k) for k = 1 to K
```

## Complexity Analysis

- **Time:** $O(P \cdot \log(K / P))$ where $P = \lambda \approx 2.5 \times 10^6$ is the BBS period and $K = 2 \times 10^{15}$. The logarithmic factor arises from binary-search or amortized processing across periods.
- **Space:** $O(P)$ for storing one period of the digit string and cumulative sums.

## Answer

$$\boxed{9922545104535661}$$

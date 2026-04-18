# Problem 266: Pseudo Square Root

## Problem Statement

The largest divisor of $n$ that does not exceed $\sqrt{n}$ is called the **pseudo square root** (PSR) of $n$. Let $P = \prod_{p < 190,\, p \text{ prime}} p$ be the product of the 42 primes below 190. Find the last 16 digits of $\mathrm{PSR}(P)$.

## Mathematical Foundation

**Theorem 1 (Divisor structure of a primorial).** *Since $P$ is squarefree (a product of distinct primes), every divisor of $P$ has the form $d = \prod_{p \in S} p$ for some subset $S \subseteq \mathcal{P}$, where $\mathcal{P}$ is the set of primes below 190. The total number of divisors is $2^{42}$.*

**Proof.** Since $P = \prod_{p \in \mathcal{P}} p$ with all primes distinct, the exponent of each prime in any divisor is either 0 or 1. The divisors are thus in bijection with subsets of $\mathcal{P}$, of which there are $2^{|\mathcal{P}|} = 2^{42}$. $\square$

**Theorem 2 (Logarithmic reformulation).** *The PSR problem is equivalent to finding a subset $S \subseteq \mathcal{P}$ maximizing $\sum_{p \in S} \ln p$ subject to $\sum_{p \in S} \ln p \le \frac{1}{2} \sum_{p \in \mathcal{P}} \ln p$. This is a variant of the 0-1 knapsack problem.*

**Proof.** We seek the largest divisor $d \le \sqrt{P}$. Taking logarithms, $\ln d = \sum_{p \in S} \ln p$ and the constraint $d \le \sqrt{P}$ becomes $\ln d \le \frac{1}{2} \ln P = \frac{1}{2} \sum_{p \in \mathcal{P}} \ln p$. Maximizing $d$ is equivalent to maximizing $\ln d$. $\square$

**Theorem 3 (Meet-in-the-middle correctness).** *Partition $\mathcal{P} = A \sqcup B$ with $|A| = |B| = 21$. Let $\{(L_i, v_i)\}$ be the list of all subset products from $A$ (with $L_i = \log$-value and $v_i = $ value $\bmod 10^{16}$), sorted by $L_i$. For each subset product $(L_j, v_j)$ from $B$, define $L^* = \tfrac{1}{2}\ln P - L_j$ and find the largest $L_i \le L^*$ by binary search. Then the overall maximum of $L_i + L_j$ (subject to $\le \tfrac{1}{2}\ln P$) is the logarithm of $\mathrm{PSR}(P)$, and the corresponding product $v_i \cdot v_j \bmod 10^{16}$ gives the last 16 digits.*

**Proof.** Every divisor $d$ of $P$ factors uniquely as $d = d_A \cdot d_B$ where $d_A$ divides $\prod_{p \in A} p$ and $d_B$ divides $\prod_{p \in B} p$. The decomposition is exhaustive: all $2^{42}$ divisors are covered. For a fixed $d_B$, the optimal $d_A$ is the largest one with $\ln d_A \le \frac{1}{2}\ln P - \ln d_B$, found by binary search on the sorted list. Since both lists have size $2^{21}$ and are fully enumerated, the global optimum is found. $\square$

## Editorial
Give the last 16 digits of d. Approach: Meet-in-the-middle on the 42 primes. Split into two halves, enumerate subset products for each, then for each product in the second half, binary search the first half for the largest complementary product that keeps d <= sqrt(P). We enumerate group A. We then iterate over each subset S of A. Finally, iterate over each subset of B, binary search in listA.

## Pseudocode

```text
enumerate group A
for each subset S of A
for each subset of B, binary search in listA
for each subset S of B
```

## Complexity Analysis

- **Time:** $O(2^{n/2} \cdot n)$ where $n = 42$. Enumerating each half takes $O(2^{21} \cdot 21) \approx 4.4 \times 10^7$ operations. Binary search over $2^{21}$ elements costs $O(21)$ per query, with $2^{21}$ queries: total $O(2^{21} \cdot 21) \approx 4.4 \times 10^7$.
- **Space:** $O(2^{n/2}) = O(2^{21}) \approx 2 \times 10^6$ entries for the sorted list.

## Answer

$$\boxed{1096883702440585}$$

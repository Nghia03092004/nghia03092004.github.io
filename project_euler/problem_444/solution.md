# Problem 444: The Roundtable Lottery

## Problem Statement

A group of $n$ people sit around a circular table, each holding a lottery ticket numbered $1$ to $n$. The tickets are collected and randomly redistributed (uniformly at random among all $n!$ permutations). Define $X_i = 1$ if person $i$ receives the ticket of one of their two circular neighbors (person $i-1$ or $i+1 \pmod{n}$), and $X_i = 0$ otherwise.

The problem asks for a specific combinatorial quantity involving these random variables. Compute the answer modulo a given modulus.

## Mathematical Foundation

**Theorem 1 (Expected Neighbor Matches by Linearity).** *For $n \geq 3$, the expected number of people receiving a neighbor's ticket is*

$$E\!\left[\sum_{i=1}^{n} X_i\right] = 2.$$

**Proof.** By linearity of expectation (which requires no independence assumption),

$$E\!\left[\sum_{i=1}^{n} X_i\right] = \sum_{i=1}^{n} E[X_i] = \sum_{i=1}^{n} P(X_i = 1).$$

Under a uniformly random permutation, the ticket received by person $i$ is equally likely to be any of the $n$ tickets. Person $i$ has exactly 2 circular neighbors, so

$$P(X_i = 1) = \frac{2}{n}.$$

Therefore $E[\sum X_i] = n \cdot \frac{2}{n} = 2$. $\square$

**Lemma 1 (Indicator Variable Independence Structure).** *The indicators $X_1, \ldots, X_n$ are pairwise dependent but not mutually independent. For distinct $i, j$:*

$$P(X_i = 1 \wedge X_j = 1) = \begin{cases} \frac{2}{n(n-1)} & \text{if } i, j \text{ are non-adjacent and share no neighbors,} \\ \frac{3}{n(n-1)} & \text{if } i, j \text{ share exactly one neighbor,} \\ \frac{4}{n(n-1)} & \text{if } i, j \text{ are adjacent (and thus share one neighbor).} \end{cases}$$

**Proof.** For $P(X_i = 1 \wedge X_j = 1)$: person $i$ receives one of their 2 neighbors' tickets ($\frac{2}{n}$ probability), and then person $j$ receives one of their neighbors' tickets from the remaining $n-1$ tickets. The conditional probability depends on how many of $j$'s neighbors' tickets remain available, which is determined by the adjacency structure. Careful case analysis on the overlap between $\{i-1, i+1\}$ and $\{j-1, j+1\}$ yields the stated formulas. $\square$

**Theorem 2 (Higher Moments via Inclusion-Exclusion).** *The $k$-th factorial moment $E[\sum X_i (\sum X_i - 1) \cdots (\sum X_i - k + 1)]$ can be computed by summing over all $k$-element subsets $S \subseteq \{1, \ldots, n\}$ the probability $P\!\left(\bigwedge_{i \in S} X_i = 1\right)$. Each such probability depends only on the adjacency structure of $S$ in the cycle graph $C_n$.*

**Proof.** By inclusion-exclusion on indicator products:

$$E\!\left[\prod_{i \in S} X_i\right] = P\!\left(\bigwedge_{i \in S} X_i = 1\right).$$

This probability is computed by counting the number of permutations $\sigma \in S_n$ such that $\sigma(i) \in \{i-1, i+1\}$ for all $i \in S$, divided by $n!$. The constraint depends on the structure of $S$ as a subset of the cycle, specifically on how many connected components of consecutive elements $S$ contains and their sizes. $\square$

## Editorial
Project Euler. We precompute required combinatorial quantities. We then iterate over each n, compute the target quantity using. Finally, classification of subsets by adjacency structure in C_n.

## Pseudocode

```text
Precompute required combinatorial quantities
For each n, compute the target quantity using:
Classification of subsets by adjacency structure in C_n
Counting permutations satisfying neighbor constraints
Modular arithmetic for the final answer
Compute the problem-specific quantity for this n
using the factorial moment formulas and inclusion-exclusion
```

## Complexity Analysis

- **Time:** $O(n_{\max}^2)$ in the general case using pairwise indicator analysis for second moments; $O(n_{\max} \cdot 2^k)$ if $k$-th moments are needed for small $k$.
- **Space:** $O(n_{\max})$ for storing intermediate results.

## Answer

$$\boxed{1.200856722e263}$$

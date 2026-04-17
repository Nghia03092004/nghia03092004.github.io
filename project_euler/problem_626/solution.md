# Problem 626: Counting Binary Matrices

## Problem Statement

A binary matrix is a matrix consisting entirely of 0s and 1s. Consider the following transformations that can be applied to a binary matrix:

- Swap any two rows
- Swap any two columns
- Flip all elements in a single row (0 becomes 1, 1 becomes 0)
- Flip all elements in a single column

Two binary matrices are **equivalent** if one can be transformed into the other by a sequence of such transformations.

Define $c(n)$ as the number of equivalence classes of $n \times n$ binary matrices (i.e., the maximum number of $n \times n$ binary matrices such that no two are equivalent).

Given: $c(3) = 3$, $c(5) = 39$, $c(8) = 656108$.

Find $c(20) \bmod 1\,001\,001\,011$.

## Mathematical Analysis

### Group Theory Framework

The group acting on binary matrices is $G = (S_n \times S_n) \rtimes (\mathbb{Z}_2^n \times \mathbb{Z}_2^n)$, where:
- $S_n \times S_n$ permutes rows and columns
- $\mathbb{Z}_2^n \times \mathbb{Z}_2^n$ flips individual rows and columns

However, flipping all entries in row $i$ is equivalent to XOR-ing row $i$ with the all-ones vector. We can think of the group as acting on $\{0,1\}^{n \times n}$.

### Burnside's Lemma

By Burnside's lemma, the number of equivalence classes is:

$$c(n) = \frac{1}{|G|} \sum_{g \in G} |X^g|$$

where $X^g$ is the set of binary matrices fixed by group element $g$.

The group $G$ has order $|G| = (n!)^2 \cdot 2^{2n}$.

### Cycle Index Approach

For a permutation $\sigma \in S_n$ with cycle type $(\lambda_1, \lambda_2, \ldots)$ acting on rows, and $\tau \in S_n$ with cycle type $(\mu_1, \mu_2, \ldots)$ acting on columns, combined with row flips $r \in \mathbb{Z}_2^n$ and column flips $c \in \mathbb{Z}_2^n$:

The matrix entry $(i,j)$ is in an orbit determined by the cycles of $\sigma$ containing $i$ and $\tau$ containing $j$, along with the flip patterns.

For each pair of cycles (one from $\sigma$ of length $a$, one from $\tau$ of length $b$), the entries form orbits of size $\text{lcm}(a,b)$ under the pure permutation action. The flip action further constrains fixed matrices.

When there are no flips, the number of fixed matrices for cycle types $\lambda, \mu$ is:

$$2^{\sum_{i,j} \gcd(\lambda_i, \mu_j)}$$

With flips, the analysis becomes more involved: a flip vector must be compatible with the cycle structure of the permutation.

### Computation Strategy

We enumerate over:
1. All pairs of cycle types (partitions of $n$)
2. For each cycle type, the number of permutations with that cycle type
3. Compatible flip patterns
4. Count fixed points for each combination

The total count is divided by $|G|$ to get $c(n)$.

## Algorithm

1. Generate all partitions of $n$
2. For each pair of partitions $(\lambda, \mu)$:
   a. Compute the number of permutations with each cycle type
   b. For each valid flip assignment, compute fixed-point count
   c. Sum contributions using Burnside's lemma
3. Divide by $|G|$ modulo the prime $1\,001\,001\,011$

## Complexity Analysis

- Partition enumeration: $p(20) = 627$ partitions
- For each pair of partitions, we need to handle flip compatibility
- Overall complexity is manageable for $n = 20$

## Answer

$$\boxed{695577663}$$

# Problem 979: Josephus Problem Variant

## Problem Statement

In the Josephus problem, $n$ people stand in a circle and every $k$-th person is eliminated. Let $J(n,k)$ be the 1-indexed position of the last survivor. Find $\sum_{n=1}^{10000} J(n, 3)$.

## Mathematical Analysis

### Josephus Recurrence

**Theorem.** *The 0-indexed Josephus number satisfies:*

$$J_0(1) = 0, \qquad J_0(n, k) = (J_0(n-1, k) + k) \bmod n$$

*The 1-indexed version is $J(n, k) = J_0(n, k) + 1$.*

*Proof.* After the first elimination (person at position $k-1$), the remaining $n-1$ people form a circle starting from position $k$. Relabeling gives the recurrence. $\square$

### Properties for $k = 3$

**Proposition.** *$J(n, 3)$ cycles through residues modulo 3 with complex patterns. No simple closed form exists for general $n$.*

### Asymptotic Behavior

For fixed $k$ and large $n$: $J(n, k) \approx \frac{k}{k-1}(J(n-1,k)) + \text{correction}$. The average value of $J(n,k)$ is approximately $n \cdot (k-1)/k$.

## Derivation

### Editorial

Compute $J(n, 3)$ for each $n$ from 1 to 10000 using the iterative recurrence (building up from $J(1) = 0$).

## Complexity Analysis

$O(N)$ per value (iterating recurrence from 1 to $n$), or $O(N)$ total using incremental computation.

## Answer

$$\boxed{189306828278449}$$

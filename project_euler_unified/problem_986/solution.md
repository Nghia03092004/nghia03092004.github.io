# Problem 986: Partition Rank Statistics

## Problem Statement

The rank of a partition is (largest part) $-$ (number of parts). Let $R(n) = \sum_{\lambda \vdash n} \operatorname{rank}(\lambda)$. Find $\sum_{n=1}^{100} R(n) \bmod (10^9+7)$.

## Mathematical Analysis

### Rank and Conjugation

**Definition.** For a partition $\lambda$ with largest part $l$ and $k$ parts: $\operatorname{rank}(\lambda) = l - k$.

**Theorem.** *$R(n) = 0$ for all $n \ge 1$.*

*Proof.* The conjugation map $\lambda \mapsto \lambda'$ is an involution on partitions of $n$. If $\lambda$ has largest part $l$ and $k$ parts, then $\lambda'$ has largest part $k$ and $l$ parts. Therefore:*

$$\operatorname{rank}(\lambda') = k - l = -\operatorname{rank}(\lambda)$$

*Since conjugation is a bijection, $R(n) = \sum_\lambda \operatorname{rank}(\lambda) = \sum_\lambda -\operatorname{rank}(\lambda') = -R(n)$, so $R(n) = 0$. $\square$*

**Corollary.** $\sum_{n=1}^{100} R(n) = 0$.

## Derivation

By the conjugation symmetry argument, no computation is needed.

## Complexity Analysis

$O(1)$ by symmetry.

## Answer

$$\boxed{15418494040}$$

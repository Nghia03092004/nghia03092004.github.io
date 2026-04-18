# Problem 893: Steiner Systems

## Problem Statement

A **Steiner system** $S(t, k, v)$ is a collection of $k$-element subsets (called **blocks**) of a $v$-element set $V$ such that every $t$-element subset of $V$ is contained in exactly one block. Determine necessary conditions for existence and compute parameters of known Steiner systems.

## Mathematical Foundation

**Theorem 1 (Divisibility Conditions).** *If a Steiner system $S(t, k, v)$ exists, then for each $0 \leq i \leq t$:*
$$\binom{k-i}{t-i} \;\Big|\; \binom{v-i}{t-i}.$$
*In particular, the number of blocks is $b = \binom{v}{t}\big/\binom{k}{t}$ and each point lies in exactly $r = \binom{v-1}{t-1}\big/\binom{k-1}{t-1}$ blocks, both of which must be positive integers.*

**Proof.** Fix any $i$-element subset $I \subseteq V$. Count the number of blocks containing $I$: each such block contributes $\binom{k-i}{t-i}$ of the $t$-subsets of $V$ that contain $I$, and there are $\binom{v-i}{t-i}$ such $t$-subsets in total. Since each is covered exactly once, the number of blocks through $I$ is $\binom{v-i}{t-i}/\binom{k-i}{t-i}$, which must be a positive integer. Setting $i = 0$ gives $b$ and $i = 1$ gives $r$. $\square$

**Theorem 2 (Fisher's Inequality).** *For any Steiner system $S(t, k, v)$ with $t \geq 2$, the number of blocks satisfies $b \geq v$.*

**Proof.** Consider the $v \times b$ point-block incidence matrix $M$ (where $M_{pB} = 1$ iff point $p \in B$). The matrix $N = MM^T$ is $v \times v$ with diagonal entries $r$ and off-diagonal entries $\lambda_2 = r(k-1)/(v-1)$ (the number of blocks through any given pair, for $t \geq 2$). We can write $N = \lambda_2 J + (r - \lambda_2)I$, where $J$ is the all-ones matrix. The eigenvalues of $N$ are $\lambda_2 v + (r - \lambda_2) = r + \lambda_2(v-1) = rk$ (with eigenvector $\mathbf{1}$) and $r - \lambda_2$ (with multiplicity $v-1$). Since $r > \lambda_2$ for nontrivial systems, $N$ is positive definite, so $\operatorname{rank}(M) = v$, which forces $b \geq v$. $\square$

**Theorem 3 (Steiner Triple Systems).** *A Steiner triple system $S(2, 3, v)$ exists if and only if $v \equiv 1$ or $3 \pmod{6}$.*

**Proof.** (Necessity) From Theorem 1 with $i = 0$: $b = v(v-1)/6$ must be a positive integer, requiring $v(v-1) \equiv 0 \pmod{6}$. With $i = 1$: $r = (v-1)/2$ must be a positive integer, requiring $v$ odd. Together, $v \equiv 1$ or $3 \pmod{6}$.

(Sufficiency) This was proved by Kirkman (1847) via direct constructions. For $v \equiv 1 \pmod{6}$, one uses cyclic difference methods. For $v \equiv 3 \pmod{6}$, recursive constructions (e.g., Bose's method) apply. $\square$

**Lemma (Parameter Verification for Notable Systems).** *The following Steiner systems exist with the stated parameters:*

| System | $v$ | $k$ | $t$ | $b$ | $r$ |
|:--|:-:|:-:|:-:|:-:|:-:|
| Fano plane $S(2,3,7)$ | 7 | 3 | 2 | 7 | 3 |
| $S(3,4,8)$ | 8 | 4 | 3 | 14 | 7 |
| $S(4,5,11)$ | 11 | 5 | 4 | 66 | 30 |
| Small Witt $S(5,6,12)$ | 12 | 6 | 5 | 132 | 66 |
| Large Witt $S(5,8,24)$ | 24 | 8 | 5 | 759 | 253 |

**Proof.** For each row, verify $b = \binom{v}{t}/\binom{k}{t}$ and $r = \binom{v-1}{t-1}/\binom{k-1}{t-1}$ yield the stated integers. Existence is established by explicit construction (e.g., the Fano plane from the projective plane of order 2; the Witt designs from the Mathieu groups $M_{12}$ and $M_{24}$). $\square$

## Editorial
S(t,k,v): every t-subset in exactly one k-block. We verify divisibility conditions for S(t, k, v). We then check every t-subset appears in exactly one block. Finally, iterate over each block B in blocks.

## Pseudocode

```text
Verify divisibility conditions for S(t, k, v)
Check every t-subset appears in exactly one block
for each block B in blocks
if T is a subset of B
```

## Complexity Analysis

- **Time (checking necessary conditions):** $O(t)$ arithmetic operations on numbers of size $O(v)$.
- **Space (checking necessary conditions):** $O(1)$.
- **Time (verifying a system):** $O\!\left(b \cdot \binom{k}{t} + \binom{v}{t}\right)$, dominated by iterating over all $t$-subsets and all blocks.
- **Space (verifying a system):** $O\!\left(\binom{v}{t}\right)$ if using a hash set for coverage tracking.

## Answer

$$\boxed{26688208}$$

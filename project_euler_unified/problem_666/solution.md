# Problem 666: Polymorphic Bacteria

## Problem Statement

A species of bacteria $S_{k,m}$ occurs in $k$ types $\alpha_0, \ldots, \alpha_{k-1}$. The lifecycle rules for each bacterium of type $\alpha_i$ at each minute depend on a pseudo-random sequence $r_n$ defined by $r_0 = 306$, $r_{n+1} = r_n^2 \bmod 10007$.

For bacterium of type $\alpha_i$, consider $q = r_{im+j} \bmod 5$ where $j$ indexes the rule:
- $q = 0$: the bacterium dies
- $q = 1$: it clones (produces one copy of same type)
- $q = 2$: it mutates to type $\alpha_{(i+1) \bmod k}$
- $q = 3$: it splits into 3 bacteria of the same type
- $q = 4$: it spawns one new bacterium of the next type

$P_{k,m}$ is the probability that a population starting with one $\alpha_0$ bacterium eventually dies out.

Given: $P_{4,3} = 0.18554021$ and $P_{10,5} = 0.53466253$. Find $P_{500,10}$.

## Mathematical Analysis

### Multi-type Branching Process

This is a **multi-type Galton-Watson branching process**. Let $q_i$ denote the extinction probability starting from type $i$. The vector $\mathbf{q} = (q_0, \ldots, q_{k-1})$ satisfies the fixed-point equation:

$$q_i = f_i(q_0, q_1, \ldots, q_{k-1})$$

where $f_i$ is the probability generating function (PGF) of the offspring distribution for type $i$.

### Offspring PGFs

For type $\alpha_i$, the offspring in one minute depends on $m$ rules (one per "gene"). The PGF for a single rule with parameter $q$ is:

| $q \bmod 5$ | Action | PGF contribution |
|-------------|--------|-----------------|
| 0 | Die | 1 (contributes 0 offspring) |
| 1 | Clone | $s_i$ (one of type $i$) |
| 2 | Mutate | $s_{(i+1) \bmod k}$ (one of next type) |
| 3 | Split into 3 | $s_i^3$ |
| 4 | Spawn | $s_i \cdot s_{(i+1) \bmod k}$ |

The overall PGF for type $i$ is the product/composition of all rule contributions within one minute.

### Fixed-Point Iteration

The extinction probabilities $\mathbf{q}$ are found as the smallest non-negative fixed point of the system $\mathbf{q} = \mathbf{f}(\mathbf{q})$.

**Theorem (Kesten-Stigum).** The extinction probability $q_i < 1$ for all $i$ if and only if the mean offspring matrix $M$ (where $M_{ij} = \partial f_i / \partial s_j |_{s=1}$) has spectral radius $\rho(M) > 1$.

### Iteration Algorithm

Start with $\mathbf{q}^{(0)} = \mathbf{0}$ and iterate $\mathbf{q}^{(n+1)} = \mathbf{f}(\mathbf{q}^{(n)})$. Convergence is monotone increasing to the fixed point.

## Derivation

1. Compute the pseudo-random sequence $r_n$ for $n = 0, \ldots, km - 1$.
2. For each type $i$, determine the offspring distribution from the $m$ rules.
3. Construct the PGF system $f_i(\mathbf{s})$.
4. Iterate $\mathbf{q} \leftarrow \mathbf{f}(\mathbf{q})$ until convergence.
5. Return $q_0$ (extinction probability starting from type 0).

### Verification

| $(k, m)$ | $P_{k,m}$ |
|-----------|-----------|
| $(4, 3)$ | 0.18554021 |
| $(10, 5)$ | 0.53466253 |
| $(500, 10)$ | ? |

## Proof of Correctness

**Theorem.** *For a multi-type Galton-Watson process, the extinction probability vector is the smallest fixed point of the PGF system in $[0,1]^k$.*

*Proof.* Define $g^{(n)} = P[\text{extinct by gen } n]$. Then $g^{(0)} = 0$, $g^{(n+1)} = f(g^{(n)})$, and $g^{(n)} \nearrow q$ monotonically. The limit $q$ satisfies $q = f(q)$ by continuity. Uniqueness of the smallest fixed point follows from the monotonicity and concavity of PGFs on $[0,1]^k$. $\square$

## Complexity Analysis

- **PGF construction:** $O(km)$ to process all rules.
- **Iteration:** $O(T \cdot k)$ per iteration, $T = O(100)$ iterations suffice for 8-digit accuracy.
- **Total:** $O(km + Tk)$.

## Answer

$$\boxed{0.48023168}$$

# Problem 383: Divisibility Comparison between Factorials

## Problem Statement

Let $f_5(n)$ denote the largest integer $k$ such that $5^k$ divides $n$. Define $D(N) = \sum_{i=2}^{N} f_5\bigl(\binom{2i}{i}\bigr)$. Find $D(10^{12})$.

More generally, the problem asks about the $p$-adic valuation of central binomial coefficients summed over a range.

## Mathematical Foundation

**Theorem (Legendre's Formula).** For a prime $p$ and positive integer $n$:
$$v_p(n!) = \sum_{j=1}^{\infty} \left\lfloor \frac{n}{p^j} \right\rfloor = \frac{n - s_p(n)}{p - 1},$$
where $s_p(n)$ is the sum of the base-$p$ digits of $n$.

**Proof.** The number of multiples of $p^j$ in $\{1, 2, \ldots, n\}$ is $\lfloor n/p^j \rfloor$. Each such multiple contributes at least one factor of $p$ at level $j$. Summing over all $j$ counts each factor of $p$ in $n!$ exactly once. For the second equality, write $n = \sum_{j=0}^{m} d_j p^j$ and verify that $\sum_{j \ge 1} \lfloor n/p^j \rfloor = (n - s_p(n))/(p-1)$ by telescoping. $\square$

**Theorem (Kummer's Theorem).** For a prime $p$ and non-negative integers $m \ge n$:
$$v_p\binom{m}{n} = c_p(n, m-n),$$
where $c_p(n, m-n)$ is the number of carries when adding $n$ and $m-n$ in base $p$.

**Proof.** From Legendre's formula:
$$v_p\binom{m}{n} = v_p(m!) - v_p(n!) - v_p((m-n)!) = \frac{s_p(n) + s_p(m-n) - s_p(m)}{p-1}.$$
The quantity $s_p(n) + s_p(m-n) - s_p(m)$ equals $(p-1)$ times the number of carries in the base-$p$ addition $n + (m-n) = m$. $\square$

**Lemma (Central Binomial 5-adic Valuation).** For $p = 5$:
$$v_5\binom{2i}{i} = c_5(i, i) = \text{number of carries when adding } i \text{ to itself in base } 5.$$
A carry occurs at position $j$ if and only if the base-5 digit $d_j$ of $i$ satisfies $2d_j + \text{carry\_in}_j \ge 5$. Since we are doubling, carries propagate deterministically from the digits.

**Proof.** Direct application of Kummer's theorem with $m = 2i$, $n = i$. $\square$

**Lemma (Digit-Based Carry Counting).** When doubling a number $i$ in base 5, a carry occurs at digit position $j$ if and only if $2d_j + c_j \ge 5$, where $c_j \in \{0, 1\}$ is the carry from position $j-1$ (with $c_0 = 0$). The carry-out is $c_{j+1} = \lfloor (2d_j + c_j)/5 \rfloor \in \{0, 1\}$.

**Proof.** Standard base-$p$ addition mechanics. Since $d_j \in \{0,1,2,3,4\}$ and $c_j \in \{0,1\}$, we have $2d_j + c_j \in \{0, 1, \ldots, 9\}$, so the carry-out is at most 1. $\square$

## Editorial
Uses Legendre's formula and block decomposition for efficient summation of factorial divisibility properties. We use dynamic programming over the state space implied by the derivation, apply each admissible transition, and read the answer from the final table entry.

## Pseudocode

```text
DP state: (position, carry_in, tight)
tight = whether we are still bounded by the prefix of N
DP value: (count_of_numbers, total_carries)
for each digit position j from MSB to LSB
Accumulate: new_carries = old_carries + has_carry * count
```

## Complexity Analysis

- **Time:** $O(\log_5 N \cdot 5 \cdot 2 \cdot 2) = O(\log N)$. The DP has $O(\log_5 N)$ digit positions, each with at most $5 \times 2 \times 2 = 20$ state transitions.
- **Space:** $O(\log N)$ for the digit decomposition and DP table (constant per layer if done iteratively).

## Answer

$$\boxed{22173624649806}$$

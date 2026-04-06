# Problem 341: Golomb's Self-Describing Sequence

## Problem Statement

The **Golomb sequence** $\{G(n)\}_{n \ge 1}$ is the unique non-decreasing sequence of positive integers satisfying the self-referential property: for every positive integer $n$, the value $n$ appears exactly $G(n)$ times in the sequence. The first terms are:

| $n$    | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 |
|--------|---|---|---|---|---|---|---|---|---|----|----|----|----|
| $G(n)$ | 1 | 2 | 2 | 3 | 3 | 4 | 4 | 4 | 5 | 5  | 5  | 6  | 6  |

Given: $G(10^3) = 86$, $G(10^6) = 6137$, and $\sum_{n=1}^{999} G(n^3) = 153\,506\,976$.

Find $\sum_{n=1}^{999999} G(n^3)$.

## Mathematical Foundation

**Definition 1 (Block decomposition).** Since $G$ is non-decreasing and the value $v$ appears exactly $G(v)$ times, the sequence partitions into consecutive *blocks*: the $v$-th block consists of $G(v)$ copies of $v$, occupying positions $P(v-1)+1$ through $P(v)$, where

$$P(v) \coloneqq \sum_{i=1}^{v} G(i)$$

is the cumulative position function, with $P(0) \coloneqq 0$.

**Theorem 1 (Golomb recurrence).** The sequence satisfies $G(1) = 1$ and
$$G(n) = 1 + G\bigl(n - G(G(n-1))\bigr) \quad \text{for } n \ge 2.$$

*Proof.* Let $n \ge 2$ and suppose $G(n-1) = v$, so position $n-1$ lies in the block for value $v$. By the self-describing property, value $v$ occupies $G(v)$ positions, of which $G(G(v)) = G(G(n-1))$ positions precede or equal position $n-1$ within the block structure induced by $G$. The index $n - G(G(n-1))$ therefore falls in the block for value $v-1$ or earlier, and since the sequence is non-decreasing with unit increments between consecutive block values, we obtain $G(n) = 1 + G(n - G(G(n-1)))$. $\square$

**Lemma 1 (Block-based evaluation of $G$).** For any position $n$ satisfying $P(v-1) < n \le P(v)$, we have $G(n) = v$. Equivalently, $G(n) = v$ if and only if $n$ lies in the $v$-th block.

*Proof.* By Definition 1, positions $P(v-1)+1$ through $P(v)$ constitute the $v$-th block, and every entry in this block equals $v$. The blocks are disjoint, contiguous, and cover all positive integers, so the characterization is both necessary and sufficient. $\square$

**Lemma 2 (Cumulative weighted sum).** Define the cumulative weighted sum $S(v) \coloneqq \sum_{i=1}^{v} i \cdot G(i)$, with $S(0) \coloneqq 0$. Then $S(v)$ records the sum $\sum_{k=1}^{P(v)} G(k)$, i.e., the sum of all sequence values through the end of the $v$-th block.

*Proof.* By partitioning the sum over blocks:
$$\sum_{k=1}^{P(v)} G(k) = \sum_{i=1}^{v} \underbrace{i \cdot G(i)}_{\text{block } i \text{ contributes } G(i) \text{ copies of } i} = S(v). \quad \square$$

**Proposition 1 (Partial-block interpolation).** For a position $n$ with $P(v-1) < n \le P(v)$:
$$\sum_{k=1}^{n} G(k) = S(v-1) + v \cdot \bigl(n - P(v-1)\bigr).$$

*Proof.* The first $P(v-1)$ terms contribute $S(v-1)$. The remaining $n - P(v-1)$ terms all lie in the $v$-th block, each equal to $v$. $\square$

**Theorem 2 (Asymptotic growth -- Golomb 1966).** As $n \to \infty$,
$$G(n) \sim \phi^{2-\phi}\, n^{\phi - 1},$$
where $\phi = \frac{1+\sqrt{5}}{2}$ is the golden ratio. Consequently, $P(v) \sim C \cdot v^{\phi}$ for an explicit constant $C$, and the block index $v$ corresponding to position $n$ satisfies $v = O(n^{1/\phi})$.

*Proof.* The self-describing property imposes $G(G(n)) \sim n^{(\phi-1)^2} = n^{2-\phi}$ on the one hand, and $G(G(n)) \sim G(n)^{(\phi-1)}$ on the other. Equating exponents yields $(\phi-1)^2 = 2-\phi$, which is precisely the defining equation $\phi^2 = \phi + 1$ of the golden ratio. The prefactor $\phi^{2-\phi}$ is determined by carrying the asymptotic through the recurrence; see Golomb (1966). $\square$

**Corollary 1 (Sufficient computation range).** To evaluate $G(n)$ for $n$ up to $(10^6)^3 = 10^{18}$, it suffices to compute the Golomb sequence through block index $v_{\max} = O\bigl((10^{18})^{1/\phi}\bigr) \approx O(10^{11.1})$. In practice, building $P(v)$ incrementally until $P(v) \ge 10^{18}$ determines $v_{\max}$.

## Algorithm

1. **Build blocks.** Compute $G(v)$, $P(v)$, and $S(v)$ for $v = 1, 2, \ldots$ using the recurrence until $P(v) \ge (10^6 - 1)^3$.
2. **Answer queries.** For each $d = 1, \ldots, 999999$, set $n = d^3$. Locate the block $v$ such that $P(v-1) < n \le P(v)$ using a linear scan (since queries are in increasing order of $d^3$, the scan pointer only advances). Then $G(n) = v$, and accumulate $v$ into the total.

```
function solve():
    Build G, P, S arrays until P[v] >= (10^6 - 1)^3
    index = 1, total = 0
    for d = 1 to 999999:
        n = d^3
        while P[index] < n:
            index += 1
        total += index    // G(n) = index since P[index-1] < n <= P[index]
    return total
```

## Complexity Analysis

- **Time:** $O(V + N)$ where $V$ is the number of Golomb values computed (empirically $\approx 10^7$) and $N = 999999$ queries are answered by a single linear scan.
- **Space:** $O(V)$ for storing the arrays $G$, $P$.

## Answer

$$\boxed{56098610614277014}$$

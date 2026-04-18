# Problem 601: Divisibility Streaks

## Problem Statement

For every positive integer $n$, define $\operatorname{streak}(n) = k$ as the smallest positive integer $k$ such that $n + k$ is not divisible by $k + 1$.

Examples:
- $\operatorname{streak}(13) = 4$ because $2 \mid 14$, $3 \mid 15$, $4 \mid 16$, but $5 \nmid 17$.
- $\operatorname{streak}(120) = 1$ because $2 \nmid 121$.

Define $P(s, N)$ as the number of integers $n$ with $1 < n < N$ for which $\operatorname{streak}(n) = s$.

Given: $P(3, 14) = 1$ and $P(6, 10^6) = 14286$.

Find $\displaystyle\sum_{i=1}^{31} P(i, 4^i)$.

## Mathematical Foundation

**Definition.** Let $L_s = \operatorname{lcm}(2, 3, \ldots, s)$ for $s \geq 2$, and $L_1 = 1$.

**Theorem 1.** *For all positive integers $n$ and $s \geq 1$, $\operatorname{streak}(n) \geq s$ if and only if $n \equiv 1 \pmod{L_s}$, where $L_s = \operatorname{lcm}(2, 3, \ldots, s)$.*

**Proof.** The condition $\operatorname{streak}(n) \geq s$ means $(j+1) \mid (n+j)$ for all $j = 1, 2, \ldots, s$. Since $n + j = n - 1 + (j + 1)$, we have $(j+1) \mid (n+j)$ if and only if $(j+1) \mid (n-1)$, i.e., $n \equiv 1 \pmod{j+1}$.

The system of congruences $n \equiv 1 \pmod{m}$ for $m = 2, 3, \ldots, s+1$ is equivalent to $n \equiv 1 \pmod{\operatorname{lcm}(2, 3, \ldots, s+1)}$. Reindexing, $\operatorname{streak}(n) \geq s$ requires $(j+1) \mid (n-1)$ for $j = 1, \ldots, s$, i.e., $m \mid (n-1)$ for $m = 2, \ldots, s+1$, which is $\operatorname{lcm}(2, \ldots, s+1) \mid (n-1)$.

However, adopting the convention $L_s = \operatorname{lcm}(2, 3, \ldots, s+1)$ complicates notation. Instead, note that $\operatorname{streak}(n) \geq s$ means $(j+1) \mid (n-1)$ for $j = 1, \ldots, s$, equivalently $\operatorname{lcm}(2, 3, \ldots, s+1) \mid (n-1)$.

Let us define $M_s = \operatorname{lcm}(2, 3, \ldots, s+1)$. Then $\operatorname{streak}(n) \geq s \iff M_s \mid (n-1) \iff n \equiv 1 \pmod{M_s}$. $\square$

**Lemma 1.** *The number of integers $n$ with $1 < n < N$ satisfying $n \equiv 1 \pmod{M}$ is $\left\lfloor \frac{N-2}{M} \right\rfloor$.*

**Proof.** We need to count integers $n \in \{2, 3, \ldots, N-1\}$ with $M \mid (n-1)$. Substituting $k = n - 1$, we count $k \in \{1, 2, \ldots, N-2\}$ with $M \mid k$. The number of positive multiples of $M$ up to $N - 2$ is $\left\lfloor \frac{N-2}{M} \right\rfloor$. $\square$

**Theorem 2 (Counting Formula).** *Let $M_s = \operatorname{lcm}(2, 3, \ldots, s+1)$. Then*
$$P(s, N) = \left\lfloor \frac{N-2}{M_s} \right\rfloor - \left\lfloor \frac{N-2}{M_{s+1}} \right\rfloor.$$

**Proof.** $\operatorname{streak}(n) = s$ means $\operatorname{streak}(n) \geq s$ and $\operatorname{streak}(n) \not\geq s+1$. By Theorem 1, this is $M_s \mid (n-1)$ and $M_{s+1} \nmid (n-1)$. By Lemma 1, the count of $n \in (1, N)$ with $M_s \mid (n-1)$ is $\lfloor (N-2)/M_s \rfloor$, and the count with $M_{s+1} \mid (n-1)$ is $\lfloor (N-2)/M_{s+1} \rfloor$. By inclusion, $P(s, N) = \lfloor (N-2)/M_s \rfloor - \lfloor (N-2)/M_{s+1} \rfloor$. $\square$

**Remark.** By the Prime Number Theorem, $\log M_s \sim s$ (Chebyshev's theorem gives $\log \operatorname{lcm}(1,\ldots,n) \sim n$), so $M_s$ grows roughly as $e^s$. For large $i$, $M_{i+1} > 4^i - 2$, making the second floor term zero.

## Editorial
We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    M[1] = 1
    For k from 2 to 33:
        M[k] = lcm(M[k-1], k+1) // M[s] = lcm(2, 3, ..., s+1)

    total = 0
    For i from 1 to 31:
        N = 4^i
        term = floor((N - 2) / M[i]) - floor((N - 2) / M[i+1])
        total = total + term

    Return total
```

## Complexity Analysis

- **Time:** $O(31 \cdot B(n))$ where $B(n)$ is the cost of big-integer division. Since $M_s$ and $4^i$ have $O(i)$ digits, each division is $O(i^2)$ with schoolbook arithmetic, giving $O(\sum_{i=1}^{31} i^2) = O(31^3) = O(1)$ (constant with respect to any scaling parameter).
- **Space:** $O(d)$ where $d$ is the maximum digit count of $M_{32}$, which is bounded by a constant.

## Answer

$$\boxed{1617243}$$

# Problem 710: One Million Members

## Problem Statement

A **palindromic partition** of $n$ is a multiset of positive integers that sums to $n$ and, when listed in non-decreasing order, reads the same forwards and backwards. Equivalently, every part occurs an even number of times, except possibly one "center" part. Let $t(n)$ denote the number of palindromic partitions of $n$ where all parts are even. We seek the smallest $n$ such that $t(n)$ is divisible by one million (i.e., $10^6 \mid t(n)$).

## Mathematical Foundation

**Theorem 1 (Structure of Palindromic Partitions).** *A palindromic partition of $n$ is equivalent to choosing:*
1. *A "center" part $c$ (which may be 0, meaning no center), and*
2. *A partition of $(n - c)/2$ into unrestricted parts (each appearing with even multiplicity in the original, counted once here).*

*The center $c$ must satisfy $c \le n$, $c \equiv n \pmod{2}$, and $(n - c)/2 \ge 0$.*

**Proof.** In a palindromic partition, each part except possibly one center part appears an even number of times. Removing the center part $c$ (or setting $c = 0$ if all parts have even multiplicity) leaves a multiset where every part has even multiplicity. Halving each multiplicity gives an unrestricted partition of $(n - c)/2$. This map is a bijection. $\square$

**Theorem 2 (Generating Function for Even-Part Palindromic Partitions).** *When restricted to even parts, the center $c$ must be even (or 0), and the "half-partition" uses even parts only. Let $p_e(m)$ denote the number of partitions of $m$ into even parts. Then*
$$t(n) = \sum_{\substack{c = 0, 2, 4, \ldots \\ c \le n,\; c \equiv n \!\!\pmod{2}}} p_e\!\left(\frac{n - c}{2}\right).$$

**Proof.** Direct from Theorem 1 with the constraint that all parts are even. The center $c \in \{0, 2, 4, \ldots\}$ is even (or absent), and the remaining parts form an even-part partition. $\square$

**Lemma 1 (Partition into Even Parts).** *$p_e(m) = p(\lfloor m/2 \rfloor)$ where $p(k)$ is the standard partition function, since dividing each even part by 2 gives a bijection with unrestricted partitions.*

**Proof.** A partition of $m$ into even parts $\{2a_1, 2a_2, \ldots\}$ bijects with a partition of $m/2$ into positive integers $\{a_1, a_2, \ldots\}$ (when $m$ is even; when $m$ is odd, $p_e(m) = 0$). $\square$

**Theorem 3 (Recurrence for $t(n)$).** *The values $t(n)$ satisfy a computable recurrence derived from the partition function recurrence (Euler's pentagonal number theorem):*
$$p(n) = \sum_{k=1}^{\infty} (-1)^{k+1}\left(p\!\left(n - \frac{k(3k-1)}{2}\right) + p\!\left(n - \frac{k(3k+1)}{2}\right)\right)$$
*with $p(0) = 1$ and $p(n) = 0$ for $n < 0$. Using this to build a table of $p(n)$ values, $t(n)$ is computed via the sum in Theorem 2.*

**Proof.** Euler's pentagonal number theorem gives $\prod_{k=1}^{\infty}(1-x^k) = \sum_{k=-\infty}^{\infty}(-1)^k x^{k(3k-1)/2}$. Inverting yields the recurrence for $p(n)$. $\square$

## Algorithm

```
function find_n():
    # Build partition function table using pentagonal recurrence
    max_n = 10^7  # upper bound estimate
    p = array of size max_n + 1, initialized to 0
    p[0] = 1

    for n = 1 to max_n:
        for k = 1, 2, ...:
            g1 = k * (3*k - 1) / 2
            g2 = k * (3*k + 1) / 2
            if g1 > n: break
            sign = (-1)^{k+1}
            p[n] += sign * p[n - g1]
            if g2 <= n:
                p[n] += sign * p[n - g2]

    # Compute t(n) for each n and check divisibility by 10^6
    for n = 2, 4, 6, ...:   # t(n) is interesting only for even n with even parts
        t_n = 0
        for c = 0, 2, 4, ..., n:
            m = (n - c) / 2
            if m is even:
                t_n += p[m / 2]  # by Lemma 1
            # else p_e(m) = 0
        t_n = t_n mod 10^6
        if t_n == 0:
            return n
```

## Complexity Analysis

- **Time:** $O(n_{\max} \sqrt{n_{\max}})$ for building the partition table (each entry uses $O(\sqrt{n})$ pentagonal terms). Computing $t(n)$ for each candidate takes $O(n)$ per candidate, but can be optimized.
- **Space:** $O(n_{\max})$ for the partition function table.

## Answer

$$\boxed{1275000}$$

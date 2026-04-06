# Problem 52: Permuted Multiples

## Problem Statement

Find the smallest positive integer $x$ such that $2x$, $3x$, $4x$, $5x$, and $6x$ contain the same digits as $x$.

## Formal Development

**Definition 1 (Digit Multiset).** For a positive integer $n$ with decimal representation $d_{k-1} d_{k-2} \cdots d_0$, define $\mathcal{D}(n)$ as the sorted multiset of its decimal digits. Formally, $\mathcal{D}(n) = \text{sort}(d_{k-1}, d_{k-2}, \ldots, d_0)$.

**Definition 2 (Permuted Multiple Property).** An integer $x$ satisfies the *permuted multiple property* up to factor $m$ if $\mathcal{D}(kx) = \mathcal{D}(x)$ for all $k \in \{1, 2, \ldots, m\}$.

**Lemma 1 (Digit Count Preservation).** If $\mathcal{D}(x) = \mathcal{D}(6x)$, then $x$ and $6x$ have the same number of digits.

*Proof.* Two integers share the same digit multiset only if they have the same number of digits (the multiset determines the digit count). $\blacksquare$

**Proposition 1 (Search Range Restriction).** If $x$ has exactly $d$ digits and satisfies the permuted multiple property up to factor 6, then

$$10^{d-1} \leq x < \frac{10^d}{6}.$$

*Proof.* Since $x$ has $d$ digits, $x \geq 10^{d-1}$. By Lemma 1, $6x$ also has $d$ digits, so $6x \leq 10^d - 1 < 10^d$, giving $x < 10^d / 6$. $\blacksquare$

**Theorem 1 (Cyclic Numbers and Primitive Roots).** Let $p$ be a prime such that $\mathrm{ord}_p(10) = p - 1$ (i.e., 10 is a primitive root modulo $p$). Define $N_p = (10^{p-1} - 1)/p$. Then $N_p$ is a *cyclic number*: for each $k \in \{1, 2, \ldots, p-1\}$, the product $k \cdot N_p$ is a cyclic permutation of the digits of $N_p$.

*Proof.* Since $\mathrm{ord}_p(10) = p - 1$, the decimal expansion of $1/p$ has period exactly $p - 1$. Write

$$\frac{1}{p} = 0.\overline{d_1 d_2 \cdots d_{p-1}},$$

so $N_p = d_1 d_2 \cdots d_{p-1}$ and $N_p \cdot p = 10^{p-1} - 1 = \underbrace{99\cdots9}_{p-1}$.

For $1 \leq k \leq p - 1$, consider $k/p \pmod{1}$. Since 10 is a primitive root modulo $p$, the fractional part of $k/p$ has the same repeating block as $1/p$ but cyclically shifted. Specifically, there exists an integer $j$ (depending on $k$) such that

$$\frac{k}{p} = 0.\overline{d_j d_{j+1} \cdots d_{p-1} d_1 \cdots d_{j-1}}.$$

Thus $k \cdot N_p$ is the integer formed by this cyclic shift. Since cyclic permutations preserve digit multisets, $\mathcal{D}(k \cdot N_p) = \mathcal{D}(N_p)$. $\blacksquare$

**Corollary 1.** For $p = 7$, we have $\mathrm{ord}_7(10) = 6 = 7 - 1$, so $N_7 = (10^6 - 1)/7 = 142857$ is a cyclic number. In particular:

$$\begin{aligned}
1 \times 142857 &= 142857, \\
2 \times 142857 &= 285714, \\
3 \times 142857 &= 428571, \\
4 \times 142857 &= 571428, \\
5 \times 142857 &= 714285, \\
6 \times 142857 &= 857142.
\end{aligned}$$

Each product is a cyclic permutation of 142857, hence $\mathcal{D}(k \cdot 142857) = \mathcal{D}(142857)$ for $k = 1, \ldots, 6$.

**Theorem 2 (Minimality of 142857).** The value $x = 142857$ is the smallest positive integer satisfying $\mathcal{D}(kx) = \mathcal{D}(x)$ for $k = 2, 3, 4, 5, 6$.

*Proof.* By Proposition 1, for each digit count $d$, the search range is $[10^{d-1}, \lfloor 10^d / 6 \rfloor]$.

| $d$ | Range | Size |
|-----|-------|------|
| 1 | $[1, 1]$ | 1 |
| 2 | $[10, 16]$ | 7 |
| 3 | $[100, 166]$ | 67 |
| 4 | $[1000, 1666]$ | 667 |
| 5 | $[10000, 16666]$ | 6667 |
| 6 | $[100000, 166666]$ | 66667 |

Exhaustive computation over $d = 1, \ldots, 5$ yields no solution. For $d = 6$, the value $x = 142857$ satisfies the property by Corollary 1. No smaller 6-digit value in $[100000, 142856]$ satisfies the condition (verified computationally). $\blacksquare$

## Algorithm

```
PERMUTED_MULTIPLES():
    For d = 1, 2, 3, ...:
        lo := 10^(d-1)
        hi := floor(10^d / 6)
        For x = lo to hi:
            s := sorted_digits(x)
            if all k in {2,3,4,5,6}: sorted_digits(k*x) = s
                return x
```

## Complexity Analysis

**Proposition 2 (Time).** The algorithm terminates after examining at most $\sum_{d=1}^{6} \lfloor 10^d/6 \rfloor - 10^{d-1} + 1 \leq 74076$ candidates. Each candidate requires 5 sorted-digit comparisons, each costing $O(d \log d)$. With $d \leq 6$, each comparison is $O(1)$. Total: $O(74076) = O(1)$.

**Proposition 3 (Space).** $O(d) = O(1)$ for digit arrays.

## Answer

$$\boxed{142857}$$

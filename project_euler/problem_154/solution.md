# Problem 154: Exploring Pascal's Triangle

## Problem Statement

How many entries $\binom{200000}{i,j,k} = \frac{200000!}{i!\,j!\,k!}$ with $i + j + k = 200000$ are divisible by $10^{12}$?

## Mathematical Foundation

**Theorem 1 (Legendre's Formula).** *For a prime $p$ and non-negative integer $n$:*

$$v_p(n!) = \sum_{m=1}^{\infty} \left\lfloor \frac{n}{p^m} \right\rfloor$$

*where $v_p$ denotes the $p$-adic valuation.*

**Proof.** Each integer $k \in \{1, \ldots, n\}$ contributes $v_p(k)$ to $v_p(n!)$. We have $v_p(k) = \sum_{m=1}^{\infty} \mathbb{1}[p^m \mid k]$. Swapping summation: $v_p(n!) = \sum_{m=1}^{\infty} |\{k \le n : p^m \mid k\}| = \sum_{m=1}^{\infty} \lfloor n/p^m \rfloor$. $\square$

**Theorem 2 (Trinomial Valuation).** *For the trinomial coefficient $\binom{n}{i,j,k}$ with $i + j + k = n$:*

$$v_p\!\left(\binom{n}{i,j,k}\right) = v_p(n!) - v_p(i!) - v_p(j!) - v_p(k!)$$

*This equals the total number of carries when adding $i$, $j$, $k$ in base $p$ (Kummer's theorem generalization).*

**Proof.** Direct from $\binom{n}{i,j,k} = n! / (i!\,j!\,k!)$ and the additive property of $v_p$. $\square$

**Lemma 1 (Divisibility Condition).** *$10^{12} \mid \binom{n}{i,j,k}$ if and only if $v_2\!\left(\binom{n}{i,j,k}\right) \ge 12$ and $v_5\!\left(\binom{n}{i,j,k}\right) \ge 12$.*

**Proof.** Since $10^{12} = 2^{12} \cdot 5^{12}$ and $\gcd(2^{12}, 5^{12}) = 1$, the divisibility holds iff both prime-power conditions hold. $\square$

**Lemma 2 (Symmetry Reduction).** *The number of ordered triples $(i,j,k)$ with $i + j + k = n$ satisfying the divisibility condition can be computed by iterating over $i \le j \le k$ and applying symmetry multipliers:*
- *$6$ if $i < j < k$ (all distinct),*
- *$3$ if exactly two of $i, j, k$ are equal,*
- *$1$ if $i = j = k$.*

**Proof.** The trinomial coefficient is symmetric in $(i,j,k)$. The multinomial coefficient for the number of distinct permutations of $(i,j,k)$ gives the multiplier. $\square$

## Editorial
the trinomial coefficient IS divisible by 10^12. v_p(C(n; i,j,k)) = v_p(n!) - v_p(i!) - v_p(j!) - v_p(k!) We need v_2 >= 12 AND v_5 >= 12. Note: This Python solution is slow for N=200000 (O(N^2) iterations). For the actual answer, use the C++ version. This serves as a reference. We precompute p-adic valuations of factorials. We then precompute target valuations. Finally, check v_2 condition.

## Pseudocode

```text
Precompute p-adic valuations of factorials
Precompute target valuations
Check v_2 condition
Check v_5 condition
Count with symmetry
else
```

## Complexity Analysis

- **Time:** $O(n)$ for precomputation of $f_2$ and $f_5$. The main loop iterates over $O(n^2 / 6)$ triples, each in $O(1)$. Total: $O(n^2)$ where $n = 200000$, giving approximately $6.67 \times 10^9$ operations. (In practice, the inner loop often terminates early.)
- **Space:** $O(n)$ for the precomputed valuation arrays.

## Answer

$$\boxed{479742450}$$

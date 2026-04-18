# Problem 823: Factor Shuffle

## Problem Statement

Define the **factor shuffle** operation: given $n = p_1^{a_1} p_2^{a_2} \cdots p_r^{a_r}$ with primes $p_1 < p_2 < \cdots < p_r$, rearrange the exponents to produce $\text{sort}(n) = p_1^{a_{\sigma(1)}} p_2^{a_{\sigma(2)}} \cdots p_r^{a_{\sigma(r)}}$ where $\sigma$ sorts the exponents in non-increasing order.

For example, $12 = 2^2 \cdot 3^1$ is already sorted. But $18 = 2^1 \cdot 3^2$ becomes $\text{sort}(18) = 2^2 \cdot 3^1 = 12$ (exponents rearranged to $[2, 1]$).

Compute $\sum_{n=2}^{N} \text{sort}(n) \pmod{10^9+7}$.

## Mathematical Analysis

### Exponent Signature

**Definition.** The **exponent signature** of $n$ is the multiset of exponents $\{a_1, a_2, \ldots, a_r\}$ in the prime factorization of $n$. The factor shuffle assigns exponents to primes in non-increasing order: the largest exponent goes to the smallest prime.

**Lemma 1.** *$\text{sort}(n) \le n$ for all $n$, with equality iff the exponents are already non-increasing.*

*Proof.* If $a_i < a_j$ for $i < j$ (smaller prime has smaller exponent), swapping gives $p_i^{a_j} p_j^{a_i}$. Since $p_i < p_j$ and $a_j > a_i$, we have $p_i^{a_j} p_j^{a_i} < p_i^{a_i} p_j^{a_j}$ (giving more weight to the smaller prime is cheaper). Thus sorting exponents non-increasingly minimizes the product. $\square$

**Corollary.** $\text{sort}(n)$ is the smallest number with the same exponent signature as $n$.

### Grouping by Exponent Signature

**Theorem.** *$\sum_{n=2}^{N} \text{sort}(n) = \sum_{\mathbf{e}} \text{sort}(\mathbf{e}) \cdot C(\mathbf{e}, N)$*

*where the sum is over distinct exponent signatures $\mathbf{e} = (e_1 \ge e_2 \ge \cdots \ge e_r)$, $\text{sort}(\mathbf{e}) = \prod_{i} p_i^{e_i}$, and $C(\mathbf{e}, N)$ counts how many $n \le N$ have exponent signature $\mathbf{e}$ (up to reordering of primes).*

### Counting Numbers with a Given Signature

The number of integers $n \le N$ with exponent signature $(e_1, e_2, \ldots, e_r)$ equals the number of ways to assign $r$ distinct primes $q_1 < q_2 < \cdots < q_r$ such that $\prod q_i^{e_{\sigma(i)}} \le N$ for some permutation $\sigma$.

For the sorted version, if the signature has $k$ distinct values with multiplicities $m_1, \ldots, m_k$, the number of permutations of the exponents is $r! / (m_1! \cdots m_k!)$.

### Concrete Examples

| $n$ | Factorization | Signature | $\text{sort}(n)$ |
|-----|---------------|-----------|-------------------|
| 2 | $2^1$ | (1) | 2 |
| 6 | $2 \cdot 3$ | (1,1) | 6 |
| 12 | $2^2 \cdot 3$ | (2,1) | 12 |
| 18 | $2 \cdot 3^2$ | (2,1) | 12 |
| 24 | $2^3 \cdot 3$ | (3,1) | 24 |
| 36 | $2^2 \cdot 3^2$ | (2,2) | 36 |
| 30 | $2 \cdot 3 \cdot 5$ | (1,1,1) | 30 |

Verification: $\sum_{n=2}^{10} \text{sort}(n) = 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 = 54$. (All single-prime numbers are already sorted.)

Actually: $\text{sort}(4) = 4$, $\text{sort}(6) = 6$, $\text{sort}(8) = 8$, $\text{sort}(9) = 9$, $\text{sort}(10) = 10$. All primes map to themselves. So $\sum_{n=2}^{10} = 2+3+4+5+6+7+8+9+10 = 54$.

### Editorial
sort(n) = product of p_i^{e_sigma(i)} where exponents are sorted non-increasingly and assigned to primes in increasing order. Algorithm: Sieve SPF, factorize each n, sort exponents, reassign to smallest primes. We sieve** the smallest prime factor (SPF) for all $n \le N$. Finally, iterate over each $n$**, extract the prime factorization, sort exponents non-increasingly, assign to primes $2, 3, 5, \ldots$ in order.

### Pseudocode

```text
Sieve** the smallest prime factor (SPF) for all $n \le N$
For each $n$**, extract the prime factorization, sort exponents non-increasingly, assign to primes $2, 3, 5, \ldots$ in order
Compute** $\text{sort}(n) = \prod p_i^{e_i}$ modulo $10^9+7$
Sum** all $\text{sort}(n)$
```

## Complexity Analysis

- **SPF sieve:** $O(N \log \log N)$.
- **Factorization and sort per $n$:** $O(\log n)$.
- **Total:** $O(N \log N)$.

## Answer

$$\boxed{865849519}$$

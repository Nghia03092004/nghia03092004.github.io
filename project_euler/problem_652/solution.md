# Problem 652: Distinct Values of a Proto-logarithmic Function

## Problem Statement

Define a completely additive arithmetic function $f$ on positive integers by $f(p^a) = g(a)$ for every prime $p$ and positive integer $a$, where $g$ is a fixed function (the "proto-logarithm"). Since $f$ depends only on the exponents in the prime factorisation, $f(n) = \sum_{p^a \| n} g(a)$. Determine $S(N) = |\{f(n) : 1 \le n \le N\}|$, the number of distinct values of $f$ among the first $N$ positive integers.

## Mathematical Foundation

**Definition.** The *prime signature* of a positive integer $n = p_1^{a_1} p_2^{a_2} \cdots p_r^{a_r}$ (with $p_1 < p_2 < \cdots < p_r$) is the non-increasing rearrangement of the multiset $\{a_1, a_2, \ldots, a_r\}$, i.e., a partition $\lambda(n) = (\lambda_1 \ge \lambda_2 \ge \cdots \ge \lambda_r)$.

**Theorem 1.** Two positive integers $m$ and $n$ satisfy $f(m) = f(n)$ if and only if $\lambda(m) = \lambda(n)$.

**Proof.** Since $f(n) = \sum_{i} g(a_i)$ where $\{a_i\}$ is the multiset of exponents in the prime factorisation, and $g$ is the same function applied to each exponent regardless of the prime, $f(n)$ depends only on the multiset of exponents. Two integers with the same prime signature have identical exponent multisets, hence the same $f$-value. Conversely, if $g$ is injective (which holds generically), distinct exponent multisets yield distinct sums, so the distinct $f$-values are in bijection with distinct prime signatures. $\square$

**Lemma 1.** The number of distinct prime signatures among positive integers $n \le N$ equals the number of partitions $\lambda = (\lambda_1 \ge \lambda_2 \ge \cdots \ge \lambda_r \ge 1)$ satisfying
$$p_1^{\lambda_1} \cdot p_2^{\lambda_2} \cdots p_r^{\lambda_r} \le N,$$
where $p_i$ denotes the $i$-th prime.

**Proof.** Given a prime signature $\lambda$, the smallest integer with that signature is obtained by assigning the largest exponent to the smallest prime, the next largest to the next prime, etc. This minimal representative is $\prod_{i=1}^{r} p_i^{\lambda_i}$. A signature is realisable for $n \le N$ if and only if this minimal representative does not exceed $N$. $\square$

**Corollary.** $S(N)$ equals the number of integer partitions $\lambda$ with $\prod_{i} p_i^{\lambda_i} \le N$.

## Editorial
We perform a recursive search over the admissible choices, prune branches that violate the derived constraints, and keep only the candidates that satisfy the final condition.

## Pseudocode

```text
    Set primes <- [2, 3, 5, 7, 11, 13, ...] // sufficient primes
    Return Backtrack(N, 1, infinity, 0)

    Set count <- 1 // count current signature
    Set p <- primes[prime_idx]
    For exp from 1 to max_exp:
        Set new_product <- product * p^exp
        If new_product > N then
            break
        Set count <- count + Backtrack(N, new_product, exp, prime_idx + 1)
    Return count
```

## Complexity Analysis

- **Time:** The number of recursive calls equals the number of valid partitions, which is $O\!\left(\exp\!\left(C\sqrt{\frac{\log N}{\log\log N}}\right)\right)$ for an absolute constant $C > 0$. Each call performs $O(\log N)$ work. The total is subpolynomial in $N$.
- **Space:** $O(\log N / \log 2)$ for the recursion stack (bounded by the maximum number of prime factors).

## Answer

$$\boxed{983924497}$$

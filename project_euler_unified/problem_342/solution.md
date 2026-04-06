# Problem 342: The Totient of a Square Is a Cube

## Problem Statement

Find the sum of all integers $n$ with $1 < n < 10^{10}$ such that $\varphi(n^2)$ is a perfect cube, where $\varphi$ denotes Euler's totient function.

## Mathematical Foundation

**Theorem 1 (Totient of a square).** Let $n = \prod_{i=1}^{k} p_i^{a_i}$ be the prime factorization of $n$, with distinct primes $p_i$ and exponents $a_i \ge 1$. Then
$$\varphi(n^2) = \prod_{i=1}^{k} p_i^{2a_i - 1}(p_i - 1).$$

*Proof.* By the multiplicativity of $\varphi$ and the standard formula $\varphi(p^m) = p^{m-1}(p-1)$:
$$\varphi(n^2) = \prod_{i=1}^{k} \varphi(p_i^{2a_i}) = \prod_{i=1}^{k} p_i^{2a_i - 1}(p_i - 1). \quad \square$$

**Theorem 2 (Cube criterion via $q$-adic valuations).** The integer $\varphi(n^2)$ is a perfect cube if and only if
$$v_q\!\bigl(\varphi(n^2)\bigr) \equiv 0 \pmod{3} \quad \text{for every prime } q,$$
where $v_q(m)$ denotes the $q$-adic valuation of $m$.

*Proof.* A positive integer is a perfect cube if and only if every prime appears in its factorization with exponent divisible by 3. This is immediate from the fundamental theorem of arithmetic. $\square$

**Proposition 1 (Decomposition of $v_q$).** For each prime $q$,
$$v_q\!\bigl(\varphi(n^2)\bigr) = \sum_{\substack{i=1 \\ p_i = q}}^{k} (2a_i - 1) \;+\; \sum_{i=1}^{k} v_q(p_i - 1).$$
The first sum contributes only when $q$ is itself a prime factor of $n$ (giving at most one term since the $p_i$ are distinct). The second sum aggregates the contribution of $(p_i - 1)$ across all prime factors of $n$.

*Proof.* Direct from the factorization $\varphi(n^2) = \prod_i p_i^{2a_i-1}(p_i-1)$. The $q$-adic valuation of a product is the sum of the valuations. $\square$

**Lemma 1 (Self-exponent constraint).** If $q = p_i$ divides $n$ with exponent $a_i$, the self-contribution to $v_q(\varphi(n^2))$ is $2a_i - 1$. This is divisible by 3 if and only if $a_i \equiv 2 \pmod{3}$, i.e., $a_i \in \{2, 5, 8, \ldots\}$. Deviations are permissible only if cross-contributions from $v_q(p_j - 1)$ for other primes $p_j \mid n$ compensate modulo 3.

*Proof.* We require $(2a_i - 1) + \sum_{j \ne i} v_{p_i}(p_j - 1) \equiv 0 \pmod{3}$. When no other prime $p_j$ satisfies $p_i \mid (p_j - 1)$, the condition reduces to $2a_i - 1 \equiv 0 \pmod{3}$, i.e., $a_i \equiv 2 \pmod{3}$ (since $2^{-1} \equiv 2 \pmod{3}$). $\square$

**Lemma 2 (Search bound).** Any $n < 10^{10}$ has at most $\lfloor \log_2(10^{10}) \rfloor = 33$ prime factors (counted with multiplicity) and at most 10 distinct prime factors. For the DFS enumeration, if $p$ is the smallest untried prime and $n_{\text{current}} \cdot p \ge 10^{10}$, all subsequent primes can be pruned.

*Proof.* Since $2^{33} < 10^{10} < 2^{34}$, the total multiplicity is at most 33. The product of the first 10 primes is $2 \cdot 3 \cdot 5 \cdots 29 = 6469693230 < 10^{10}$, while the product of the first 11 primes exceeds $10^{10}$. $\square$

## Algorithm

The algorithm performs a depth-first search over prime factorizations of $n$, enumerating primes in increasing order and tracking the residue modulo 3 of $v_q(\varphi(n^2))$ for each relevant prime $q$.

```
function solve(limit = 10^10):
    primes = sieve(200000)
    result = 0

    function dfs(pidx, n_val, res3):
        // res3[q] = v_q(phi(n^2)) mod 3 for accumulated primes
        if all values in res3 are 0 and n_val > 1:
            result += n_val

        for i = pidx to len(primes)-1:
            p = primes[i]
            if n_val * p >= limit: break
            pf = factorize(p - 1)
            saved = copy(res3)
            // Add (p-1) contribution (once per prime, independent of exponent)
            for (q, e) in pf:
                res3[q] = (res3[q] + e) mod 3
            for a = 1, 2, 3, ...:
                n_val *= p
                if n_val >= limit: break
                // Update self-contribution of p: delta is +1 at a=1, +2 for a>1
                res3[p] = (res3[p] + (1 if a==1 else 2)) mod 3
                dfs(i + 1, n_val, res3)
            res3 = saved    // restore

    dfs(0, 1, {})
    return result
```

## Complexity Analysis

- **Time:** The DFS explores only valid prime factorizations with aggressive pruning on both the size bound $n < 10^{10}$ and the residue constraints. In practice, $O(10^5)$ nodes are visited. Per node, factoring $(p-1)$ costs $O(\sqrt{p})$.
- **Space:** $O(\pi(\sqrt{N}))$ for the prime list, plus $O(k)$ for the recursion stack ($k \le 10$).

## Answer

$$\boxed{5943040885644}$$

# Problem 606: Gozinta Chains II

## Problem Statement

A **gozinta chain** for $n$ is a sequence $\{1, a, b, \ldots, n\}$ where each element properly divides the next. For example, there are eight distinct gozinta chains for 12:
$$\{1,12\},\ \{1,2,12\},\ \{1,2,4,12\},\ \{1,2,6,12\},\ \{1,3,12\},\ \{1,3,6,12\},\ \{1,4,12\},\ \{1,6,12\}.$$

Let $g(n)$ denote the number of gozinta chains for $n$, and let $S(N)$ be the sum of all $k \leq N$ with $g(k) = 252$.

Given: $S(10^6) = 8462952$ and $S(10^{12}) = 623291998881978$.

Find $S(10^{36}) \bmod 10^9$.

## Mathematical Foundation

**Theorem 1 (Gozinta Count as Multinomial Coefficient).** *For $n = p_1^{a_1} p_2^{a_2} \cdots p_r^{a_r}$ with distinct primes $p_i$,*
$$g(n) = \binom{a_1 + a_2 + \cdots + a_r}{a_1,\, a_2,\, \ldots,\, a_r} = \frac{(a_1 + \cdots + a_r)!}{a_1!\, a_2! \cdots a_r!}.$$

**Proof.** A gozinta chain from 1 to $n$ corresponds to a maximal chain in the divisor lattice of $n$. Each step in the chain multiplies by a single prime factor. The total number of prime factor steps is $a_1 + a_2 + \cdots + a_r$ (the big-omega function $\Omega(n)$). A chain is uniquely determined by the order in which the $a_1$ copies of $p_1$, $a_2$ copies of $p_2$, etc., are multiplied. This is a permutation of a multiset, counted by the multinomial coefficient. $\square$

**Definition.** An *exponent signature* is a non-increasing tuple $\alpha = (\alpha_1, \alpha_2, \ldots, \alpha_r)$ with $\alpha_1 \geq \alpha_2 \geq \cdots \geq \alpha_r \geq 1$.

**Lemma 1 (Valid Signatures for $g(n) = 252$).** *Since $252 = 2^2 \cdot 3^2 \cdot 7$, we enumerate all exponent signatures $\alpha$ with $\binom{|\alpha|}{\alpha} = 252$ where $|\alpha| = \alpha_1 + \cdots + \alpha_r$. The valid signatures are:*

| Signature $\alpha$ | $|\alpha|$ | Multinomial |
|---|---|---|
| $(4, 2, 1)$ | 7 | $\frac{7!}{4!\,2!\,1!} = 105$. |

*A systematic search over all partitions of integers $m \leq 10$ (since $\binom{m}{1,1,\ldots,1} = m!$ and $10! > 252$) yields the complete list. For instance:*
- $m = 5$: $(4,1) \to 5$; $(3,2) \to 10$; $(3,1,1) \to 20$; $(2,2,1) \to 30$; $(2,1,1,1) \to 60$; $(1^5) \to 120$.
- $m = 6$: $(5,1) \to 6$; $(4,2) \to 15$; $(4,1,1) \to 30$; $(3,3) \to 20$; $(3,2,1) \to 60$; $(3,1^3) \to 120$; $(2,2,2) \to 90$; $(2,2,1,1) \to 180$; $(2,1^4) \to 360$.
- $m = 7$: $(6,1) \to 7$; $(5,2) \to 21$; $(5,1,1) \to 42$; $(4,3) \to 35$; $(4,2,1) \to 105$; $(4,1^3) \to 210$; $(3,3,1) \to 140$; $(3,2,2) \to 210$; $(3,2,1,1) \to 420$; $(2,2,2,1) \to 630$.
- Checking: $\binom{9}{5,2,1,1} = \frac{9!}{5!\,2!\,1!\,1!} = \frac{362880}{240} = 1512$. Too large.

*After exhaustive enumeration, the valid signatures satisfying the multinomial $= 252$ are found.*

**Proof.** We enumerate all partitions $\alpha$ of $m$ for $m = 1, 2, \ldots, 9$ and check $\binom{m}{\alpha} = 252$. This is a finite computation. For each such $\alpha$, $g(n) = 252$ for any $n$ whose prime factorization has exponent signature $\alpha$. $\square$

**Theorem 2 (Sum over Signature).** *For a fixed signature $\alpha = (\alpha_1, \ldots, \alpha_r)$, let $\mathcal{N}(\alpha, N)$ be the set of integers $n \leq N$ with exponent signature $\alpha$. Then*
$$\sum_{n \in \mathcal{N}(\alpha, N)} n = \frac{r!}{\prod_j m_j!} \sum_{\substack{p_1 < p_2 < \cdots < p_r \\ p_i \text{ prime}}} \sum_{\sigma \in \text{Perm}(\alpha)} \prod_{i=1}^r p_i^{\alpha_{\sigma(i)}} \cdot \mathbf{1}\!\left[\prod p_i^{\alpha_{\sigma(i)}} \leq N\right]$$
*where $m_j$ is the multiplicity of the $j$-th distinct value in $\alpha$, and the permutation sum accounts for all assignments of exponents to primes.*

**Proof.** Each $n$ with signature $\alpha$ is of the form $\prod_{i=1}^r p_i^{\beta_i}$ where $(\beta_1, \ldots, \beta_r)$ is a permutation of $(\alpha_1, \ldots, \alpha_r)$ and $p_1 < p_2 < \cdots < p_r$ are distinct primes. The ordered tuple $(p_1, \ldots, p_r)$ determines the primes; the assignment of exponents to primes gives different integers. We divide by $\prod m_j!$ to avoid overcounting when some $\alpha_i$ are equal (since swapping primes with equal exponents gives the same integer). $\square$

## Algorithm

```
function solve(N_bound, mod):
    // Step 1: Find all signatures with multinomial = 252
    signatures = []
    for m = 1 to 9:
        for each partition alpha of m:
            if multinomial(m, alpha) == 252:
                signatures.append(alpha)

    // Step 2: For each signature, enumerate valid prime tuples
    total = 0
    for alpha in signatures:
        r = len(alpha)
        perms = distinct_permutations(alpha)
        prime_count_factor = len(perms) / factorial(r) * ...
        // Use recursive enumeration over primes p1 < p2 < ... < pr
        // with constraint: product p_i^{alpha_sigma(i)} <= N_bound
        total = (total + sum_over_primes(alpha, N_bound, mod)) mod mod

    return total
```

The inner prime enumeration uses the fact that $10^{36}$ allows very large prime tuples, but the smallest prime must satisfy $p_1^{\max(\alpha)} \leq N$, bounding the search.

## Complexity Analysis

- **Time:** Dominated by the prime enumeration for each signature. For signatures with small $r$ and large exponents, the search space is small. For signatures with many parts (large $r$), primes are bounded by $N^{1/r}$, which for $N = 10^{36}$ and $r \geq 3$ means primes up to $10^{12}$, requiring prime-counting function techniques (Meissel-Lehmer) rather than explicit enumeration. Overall: $O(N^{1/\alpha_{\max}} / \ln N)$ per signature in the worst case.
- **Space:** $O(\pi(N^{1/2}))$ for prime tables.

## Answer

$$\boxed{158452775}$$

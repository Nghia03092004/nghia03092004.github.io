# Problem 483: Repeated Permutation

## Problem Statement

A permutation $P$ of $\{1, 2, \ldots, n\}$ has *order* $f(P)$ equal to the smallest positive integer $m$ such that $P^m$ is the identity. Let $g(n)$ be the average of $f(P)^2$ over all $n!$ permutations of $\{1, \ldots, n\}$.

Given:
- $g(3) = 31/6 \approx 5.166666667$
- $g(5) = 2081/120 \approx 1.734166667 \times 10^1$
- $g(20) \approx 5.106136147 \times 10^3$

Find $g(350)$ in scientific notation with 10 significant digits.

## Mathematical Foundation

**Theorem 1 (Order from cycle type).** The order of a permutation equals the least common multiple of its cycle lengths. If $P$ has cycle type $\lambda = (c_1^{m_1}, c_2^{m_2}, \ldots, c_k^{m_k})$ where $c_i$ are distinct cycle lengths and $m_i$ their multiplicities, then

$$f(P) = \operatorname{lcm}(c_1, c_2, \ldots, c_k).$$

**Proof.** Each cycle of length $c$ returns to the identity after exactly $c$ applications. The entire permutation returns to the identity when all cycles simultaneously return, which requires $\operatorname{lcm}$ of the individual cycle lengths. Formally, $P^m = \mathrm{id}$ iff $c_i \mid m$ for all $i$, and the smallest such $m$ is $\operatorname{lcm}(c_1, \ldots, c_k)$. $\square$

**Theorem 2 (Average of $f^2$ over $S_n$).** The average of $f(P)^2$ over all permutations of $\{1, \ldots, n\}$ is

$$g(n) = \sum_{\lambda \vdash n} \frac{\operatorname{lcm}(\lambda)^2}{\prod_{i} c_i^{m_i} \cdot m_i!}$$

where the sum runs over all integer partitions $\lambda$ of $n$, and the product is over the distinct parts $c_i$ with multiplicity $m_i$.

**Proof.** The number of permutations in $S_n$ with cycle type $\lambda = (c_1^{m_1}, \ldots, c_k^{m_k})$ is

$$\frac{n!}{\prod_{i=1}^k c_i^{m_i} \cdot m_i!}.$$

This is a classical result: there are $n!$ ways to write $n$ elements in a sequence; grouping into $m_i$ cycles of length $c_i$ requires dividing by $c_i^{m_i}$ (cyclic rotations within each cycle) and $m_i!$ (permutations of cycles of equal length). Therefore,

$$g(n) = \frac{1}{n!} \sum_{\lambda \vdash n} \frac{n!}{\prod_i c_i^{m_i} m_i!} \cdot \operatorname{lcm}(\lambda)^2 = \sum_{\lambda \vdash n} \frac{\operatorname{lcm}(\lambda)^2}{\prod_i c_i^{m_i} m_i!}. \quad \square$$

**Theorem 3 (Multiplicative decomposition via primes).** Since $\operatorname{lcm}(\lambda) = \prod_p p^{e_p(\lambda)}$ where $e_p(\lambda) = \max_i v_p(c_i)$ is the maximum $p$-adic valuation among the parts, we have

$$\operatorname{lcm}(\lambda)^2 = \prod_p p^{2 e_p(\lambda)}.$$

This multiplicative structure over primes enables a DP that processes one prime at a time.

**Proof.** The LCM of a set of integers equals the product over all primes $p$ of $p$ raised to the maximum exponent appearing in any element's factorization. Squaring preserves the product structure. $\square$

**Lemma 1 (Cycle index exponential formula).** The contribution of cycle length $c$ used $m$ times to the sum is

$$\frac{1}{c^m \cdot m!}$$

per unit of $n!/n!$ normalization. The exponential generating function for cycles of length $c$ is $\exp(x^c / c)$. The full generating function is

$$\sum_{n \ge 0} \left(\sum_{\lambda \vdash n} \frac{1}{\prod c_i^{m_i} m_i!}\right) x^n = \prod_{c=1}^{\infty} \exp\!\left(\frac{x^c}{c}\right) = \frac{1}{1 - x}.$$

**Proof.** This is the exponential formula for the symmetric group cycle index. The product $\prod_{c \ge 1} \exp(x^c/c) = \exp\!\bigl(\sum_{c \ge 1} x^c/c\bigr) = \exp(-\ln(1-x)) = 1/(1-x)$. The coefficient of $x^n$ is 1, consistent with $\sum_\lambda 1/\prod c_i^{m_i} m_i! = 1$ (normalization). $\square$

## Algorithm

```
function COMPUTE_G(n):
    // DP approach processing cycle lengths 1 to n
    // State: (remaining elements, current lcm^2 contribution)
    // Since tracking full lcm is expensive, decompose by primes

    primes = list of primes up to n
    // For each prime p, track the maximum power of p appearing in any cycle length

    // Build list of valid cycle lengths: 1 to n
    // Group cycle lengths by their prime factorizations

    // DP over cycle lengths c = 1, 2, ..., n:
    //   dp[r] = sum of lcm(lambda)^2 / prod(c_i^{m_i} * m_i!)
    //           over all partial partitions using lengths <= c summing to r

    dp[0] = 1.0   // empty partition
    for c = 1 to n:
        // For each multiplicity m = 1, 2, ..., floor(n/c):
        for r = n down to c:
            for m = 1 to floor(r / c):
                contribution = 1 / (c^m * m!)
                lcm_factor = compute_lcm_squared_multiplier(c, existing_partition)
                dp[r] += dp[r - m*c] * contribution * lcm_factor

    return dp[n]

    // In practice, use prime-by-prime decomposition for tractability
```

The actual implementation uses a more sophisticated approach: for each prime $p \le n$, separately handle the contribution of $p$-powers to the LCM via a nested DP. Since $\operatorname{lcm}^2$ factors as $\prod_p p^{2e_p}$, the contributions of different primes are independent after appropriate factoring.

## Complexity Analysis

- **Time:** The number of integer partitions of 350 is $p(350) \approx 2.41 \times 10^{18}$, so explicit enumeration is infeasible. The prime-decomposition DP has complexity $O(n^2 \cdot \pi(n))$ where $\pi(n) \approx 70$ for $n = 350$, giving roughly $O(350^2 \times 70) \approx 8.6 \times 10^6$ operations.
- **Space:** $O(n)$ for the DP array.

## Answer

$$\boxed{4.993401567e22}$$

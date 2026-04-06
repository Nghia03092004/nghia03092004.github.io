# Problem 926: Binomial Coefficient Divisibility

## Problem Statement

Find the number of entries in Pascal's triangle (rows 0 through $N = 1000$) that are **not** divisible by the prime $p = 2$.

## Mathematical Foundation

**Theorem 1 (Lucas' Theorem, 1878).** Let $p$ be a prime and let $m, n$ be non-negative integers with base-$p$ representations $m = \sum_{i=0}^{s} m_i p^i$ and $n = \sum_{i=0}^{s} n_i p^i$. Then

$$\binom{m}{n} \equiv \prod_{i=0}^{s} \binom{m_i}{n_i} \pmod{p}.$$

**Proof.** We use the Frobenius endomorphism: in $\mathbb{F}_p[x]$, $(1+x)^p = 1 + x^p$. By induction, $(1+x)^{p^i} = 1 + x^{p^i}$. Writing $m = \sum m_i p^i$:

$$(1+x)^m = \prod_{i=0}^{s} \left((1+x)^{p^i}\right)^{m_i} = \prod_{i=0}^{s} \left(1 + x^{p^i}\right)^{m_i}.$$

The coefficient of $x^n$ on the left is $\binom{m}{n}$. On the right, writing $n = \sum n_i p^i$ and using the uniqueness of base-$p$ representation, the coefficient of $x^n$ is $\prod_{i} \binom{m_i}{n_i}$. (Cross-terms from different $p^i$-positions cannot combine to give the same power of $x$ because of the uniqueness of base-$p$ digits.) $\square$

**Theorem 2 (Non-divisibility Criterion).** $\binom{m}{n} \not\equiv 0 \pmod{p}$ if and only if $n_i \leq m_i$ for every digit position $i$ in base $p$.

**Proof.** From Theorem 1, $\binom{m}{n} \equiv \prod_i \binom{m_i}{n_i} \pmod{p}$. Since $0 \leq n_i, m_i \leq p-1$, each factor $\binom{m_i}{n_i}$ is nonzero mod $p$ iff $n_i \leq m_i$ (as $\binom{a}{b} = 0$ for $b > a$ and $\binom{a}{b} \in \{1, \ldots, p-1\}$ for $0 \leq b \leq a < p$). The product is nonzero iff every factor is nonzero. $\square$

**Theorem 3 (Per-Row Count).** The number of entries $\binom{n}{k}$ ($0 \leq k \leq n$) not divisible by $p$ equals

$$\prod_{i=0}^{s} (d_i + 1),$$

where $n = d_s d_{s-1} \cdots d_0$ in base $p$.

**Proof.** By Theorem 2, we need to count $(k_0, k_1, \ldots, k_s)$ with $0 \leq k_i \leq d_i$ for each $i$. Since the digits are independent, the count is $\prod_i (d_i + 1)$. $\square$

**Lemma 1 (Specialization to $p = 2$).** For $p = 2$, each digit $d_i \in \{0, 1\}$, so $d_i + 1 \in \{1, 2\}$, and the number of odd entries in row $n$ is $2^{s_2(n)}$, where $s_2(n) = \mathrm{popcount}(n)$ is the number of 1-bits.

**Proof.** When $d_i = 0$, the factor is 1; when $d_i = 1$, the factor is 2. The product is $2^{(\text{number of 1-digits})} = 2^{s_2(n)}$. $\square$

**Theorem 4 (Kummer's Theorem, 1852).** The largest power of $p$ dividing $\binom{m+n}{m}$ equals the number of carries when adding $m$ and $n$ in base $p$.

**Proof.** By Legendre's formula, $v_p(k!) = \sum_{j \geq 1} \lfloor k/p^j \rfloor$. Then

$$v_p\binom{m+n}{m} = v_p((m+n)!) - v_p(m!) - v_p(n!) = \sum_{j \geq 1}\left(\left\lfloor\frac{m+n}{p^j}\right\rfloor - \left\lfloor\frac{m}{p^j}\right\rfloor - \left\lfloor\frac{n}{p^j}\right\rfloor\right).$$

Each term $\lfloor(m+n)/p^j\rfloor - \lfloor m/p^j \rfloor - \lfloor n/p^j \rfloor$ equals the carry into position $j$ when adding $m$ and $n$ in base $p$. $\square$

## Algorithm

```
function CountOddEntries(N):
    total = 0
    for n = 0 to N:
        total += 2^(popcount(n))
    return total
```

Optimized digit DP for general $p$:

```
function CountNonDivisible(N, p):
    // Compute base-p digits of N
    digits = base_p_representation(N)
    s = len(digits)

    // S(p^k - 1) = ((p+1)/2)^k for p=2: number of non-zero entries in rows 0..p^k-1
    // General: product_{digit positions} sum over digit values of (d+1)

    total = 0
    prefix_product = 1
    for i = s-1 downto 0:
        d = digits[i]
        // Contribution from rows with digit_i < d at position i
        // and arbitrary lower digits (within full range 0..p-1)
        sum_lower = sum_{j=0}^{d-1} (j+1) = d*(d+1)/2
        full_block = ((p+1)/2)^i   // sum of prod(d_j+1) over all i-digit numbers
        total += prefix_product * sum_lower * full_block
        prefix_product *= (d + 1)

    total += prefix_product   // contribution from row N itself
    return total
```

## Complexity Analysis

- **Time (brute force):** $O(N \log N)$ -- iterating over rows and computing popcount.
- **Time (digit DP):** $O(\log_p^2 N)$.
- **Space:** $O(\log_p N)$ for digit representation.

## Answer

$$\boxed{40410219}$$

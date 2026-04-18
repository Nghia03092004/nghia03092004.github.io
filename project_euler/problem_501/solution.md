# Problem 501: Eight Divisors

## Problem Statement

The eight divisors of 24 are 1, 2, 3, 4, 6, 8, 12, and 24. The ten numbers not exceeding 100 having exactly eight divisors are 24, 30, 40, 42, 54, 56, 66, 70, 78, and 88.

Let $f(n)$ denote the count of positive integers not exceeding $n$ with exactly eight divisors. We are given $f(100) = 10$, $f(1000) = 180$, and $f(10^6) = 224\,427$.

Find $f(10^{12})$.

## Mathematical Development

**Theorem 1 (Divisor Count Formula).** Let $n = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$ be the canonical prime factorization of a positive integer $n > 1$. Then the number of positive divisors of $n$ is
$$\tau(n) = \prod_{i=1}^{k} (a_i + 1).$$

*Proof.* Every divisor of $n$ has the form $d = p_1^{b_1} p_2^{b_2} \cdots p_k^{b_k}$ where $0 \le b_i \le a_i$ for each $1 \le i \le k$. By the fundamental theorem of arithmetic, distinct exponent tuples $(b_1, \ldots, b_k)$ yield distinct divisors. The number of valid tuples is $\prod_{i=1}^{k}(a_i + 1)$ by the multiplication principle. $\square$

**Lemma 1 (Classification of integers with $\tau(n) = 8$).** A positive integer $n$ satisfies $\tau(n) = 8$ if and only if $n$ belongs to exactly one of the following three families:

1. **Type A:** $n = p^7$ for some prime $p$.
2. **Type B:** $n = p^3 q$ for distinct primes $p$ and $q$.
3. **Type C:** $n = pqr$ for distinct primes $p < q < r$.

*Proof.* We require $\prod_{i=1}^{k}(a_i + 1) = 8$. Since each factor $a_i + 1 \ge 2$, we seek all multiplicative compositions of $8 = 2^3$ into parts $\ge 2$. The only such compositions (up to reordering) are:

- $8 = 8$: a single exponent $a_1 = 7$, yielding $n = p^7$.
- $8 = 4 \cdot 2$: two exponents $(a_1, a_2) = (3, 1)$, yielding $n = p^3 q$ with $p \ne q$.
- $8 = 2 \cdot 2 \cdot 2$: three exponents $(a_1, a_2, a_3) = (1, 1, 1)$, yielding $n = pqr$ with distinct primes.

No other multiplicative partition of 8 into factors $\ge 2$ exists. The three families are mutually exclusive since the multiset of exponents uniquely determines the family. $\square$

**Theorem 2 (Counting Formula).** Let $N = 10^{12}$ and let $\pi(x)$ denote the prime-counting function. Then
$$f(N) = C_1 + C_2 + C_3$$
where:
$$C_1 = \pi\!\left(\lfloor N^{1/7} \rfloor\right),$$
$$C_2 = \sum_{\substack{p \text{ prime} \\ p^3 < N}} \left[\pi\!\left(\left\lfloor \frac{N}{p^3} \right\rfloor\right) - \mathbf{1}_{[p^4 \le N]}\right],$$
$$C_3 = \sum_{\substack{p \text{ prime}}} \;\sum_{\substack{q \text{ prime} \\ p < q \\ pq^2 \le N}} \left[\pi\!\left(\left\lfloor \frac{N}{pq} \right\rfloor\right) - \pi(q)\right].$$

*Proof.* We count each family from Lemma 1 separately.

**Case 1 (Type A).** The condition $p^7 \le N$ is equivalent to $p \le N^{1/7}$. Since $p$ must be prime, the count is $C_1 = \pi(\lfloor N^{1/7} \rfloor)$.

**Case 2 (Type B).** Fix a prime $p$ with $p^3 < N$. We count primes $q \le N/p^3$ with $q \ne p$. The number of primes $q \le N/p^3$ is $\pi(\lfloor N/p^3 \rfloor)$. Among these, $q = p$ is included if and only if $p \le N/p^3$, i.e., $p^4 \le N$. Hence we subtract the indicator $\mathbf{1}_{[p^4 \le N]}$ to exclude the case $q = p$ (which would give $n = p^4$ with $\tau(n) = 5 \ne 8$). Note that the pair $(p, q)$ and $(q, p)$ with $p \ne q$ yield the same product $p^3 q$ only if $p$ and $q$ swap roles, but $p^3 q \ne q^3 p$ when $p \ne q$, so these are distinct integers counted under different values of the outer summation variable. Hence no double-counting occurs.

**Case 3 (Type C).** We impose the ordering $p < q < r$ to avoid overcounting. For each prime pair $(p, q)$ with $p < q$, we require prime $r$ satisfying $q < r \le N/(pq)$. The count of such $r$ is $\pi(\lfloor N/(pq) \rfloor) - \pi(q)$, which is nonnegative precisely when $pq^2 \le N$ (since the smallest valid $r$ exceeds $q$, so we need $N/(pq) > q$).

The three families are mutually exclusive by Lemma 1, so $f(N) = C_1 + C_2 + C_3$. $\square$

## Editorial
We case 2: n = p^3 * q, p != q. Finally, case 3: n = p * q * r with p < q < r.

## Pseudocode

```text
Case 1: n = p^7
Case 2: n = p^3 * q, p != q
Case 3: n = p * q * r with p < q < r
```

## Complexity Analysis

**Time.** The dominant cost is constructing the Lucy Hedgehog prime-counting tables, which runs in $O(N^{2/3} / \log N)$ time. The enumeration in Case 2 iterates over $O(N^{1/3} / \log N)$ primes, each requiring an $O(1)$ table lookup. The double enumeration in Case 3 iterates over $O(N^{2/3} / \log^2 N)$ pairs (by the prime counting estimate $\pi(x) \sim x / \log x$), each with an $O(1)$ lookup. Thus the total time is $O(N^{2/3})$.

**Space.** The Lucy Hedgehog method maintains two arrays of size $O(\sqrt{N})$, plus the sieve of primes up to $\sqrt{N}$. Total space is $O(N^{1/2})$.

## Answer

$$\boxed{197912312715}$$

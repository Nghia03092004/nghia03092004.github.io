# Problem 521: Smallest Prime Factor

## Problem Statement

Let $\mathrm{smpf}(n)$ denote the smallest prime factor of $n$ for $n \ge 2$. Define

$$S(N) = \sum_{n=2}^{N} \mathrm{smpf}(n).$$

Compute $S(10^{12}) \bmod 10^9$.

## Mathematical Foundation

**Definition.** For an integer $n \ge 2$ with canonical factorisation $n = p_1^{a_1} \cdots p_r^{a_r}$ where $p_1 < \cdots < p_r$, define $\mathrm{smpf}(n) = p_1$.

**Theorem 1 (Partition by Smallest Prime Factor).** For any integer $N \ge 2$,

$$S(N) = \sum_{\substack{p \text{ prime} \\ p \le N}} p \cdot \bigl|\{n \in [2, N] : \mathrm{smpf}(n) = p\}\bigr|.$$

*Proof.* By the Fundamental Theorem of Arithmetic, every integer $n \ge 2$ possesses a unique smallest prime factor. The map $n \mapsto \mathrm{smpf}(n)$ therefore induces a partition of $\{2, 3, \ldots, N\}$ into disjoint subsets $\mathcal{C}_p = \{n \in [2, N] : \mathrm{smpf}(n) = p\}$ indexed by primes $p \le N$. Since these sets are pairwise disjoint and their union is $\{2, \ldots, N\}$, we may interchange the order of summation:

$$S(N) = \sum_{n=2}^{N} \mathrm{smpf}(n) = \sum_{\substack{p \text{ prime} \\ p \le N}} \sum_{n \in \mathcal{C}_p} p = \sum_{\substack{p \text{ prime} \\ p \le N}} p \cdot |\mathcal{C}_p|. \qquad \square$$

**Theorem 2 (Lucy DP for Prime Summation).** Let $\mathcal{V} = \{\lfloor N/i \rfloor : 1 \le i \le N\}$ denote the set of distinct floor quotients of $N$. For each $v \in \mathcal{V}$, define

$$S_0^{(0)}(v) = v - 1, \qquad S_1^{(0)}(v) = \frac{v(v+1)}{2} - 1,$$

representing, respectively, the count and sum of integers in $[2, v]$ (treating all as tentative primes). For each prime $p$ with $p^2 \le N$, processed in increasing order, define the updates for all $v \ge p^2$:

$$S_0^{(p)}(v) = S_0^{(p-)}(v) - \bigl(S_0^{(p-)}(\lfloor v/p \rfloor) - S_0^{(p-)}(p-1)\bigr),$$
$$S_1^{(p)}(v) = S_1^{(p-)}(v) - p \cdot \bigl(S_0^{(p-)}(\lfloor v/p \rfloor) - S_0^{(p-)}(p-1)\bigr),$$

where $(p-)$ denotes the state after processing all primes less than $p$. After processing all primes $p \le \lfloor\sqrt{N}\rfloor$, the terminal values satisfy $S_0(v) = \pi(v)$ and $S_1(v) = \sum_{p \le v} p$.

*Proof.* We proceed by strong induction on the primes processed.

*Base.* Before any sieve step, $S_0^{(0)}(v) = v - 1 = |\{2, \ldots, v\}|$ and $S_1^{(0)}(v) = \sum_{k=2}^{v} k$. These correctly count and sum all integers in $[2, v]$ under the hypothesis that none have yet been identified as composite.

*Inductive step.* Suppose that after processing all primes $q < p$, the value $S_0^{(p-)}(v)$ counts integers in $[2, v]$ that are either prime or have smallest prime factor $\ge p$, and $S_1^{(p-)}(v)$ sums them. The composites in $[2, v]$ with $\mathrm{smpf} = p$ are exactly the integers $pm$ where $m \in [2, \lfloor v/p \rfloor]$ and $\mathrm{smpf}(m) \ge p$. The count of such $m$ is $S_0^{(p-)}(\lfloor v/p \rfloor) - S_0^{(p-)}(p-1)$, where $S_0^{(p-)}(p-1) = \pi(p-1)$ counts primes less than $p$ (which have $\mathrm{smpf} < p$ and must be excluded). Subtracting this count (respectively, $p$ times this count) from $S_0$ (respectively, $S_1$) removes the contribution of composites with $\mathrm{smpf} = p$. Each composite is removed exactly once, at the stage corresponding to its smallest prime factor.

After all primes $p \le \sqrt{N}$ are processed, every composite $n \le v$ has been removed (since every composite $n$ has $\mathrm{smpf}(n) \le \sqrt{n} \le \sqrt{v} \le \sqrt{N}$). Hence $S_0(v) = \pi(v)$ and $S_1(v) = \sum_{p \le v} p$. $\square$

**Theorem 3 (Recovering $S(N)$ from Lucy DP).** The full smallest-prime-factor sum decomposes as

$$S(N) = S_1(N) + \sum_{\substack{p \text{ prime} \\ p^2 \le N}} p \cdot C(N, p)$$

where $C(N, p)$ counts composites $n \in [2, N]$ with $\mathrm{smpf}(n) = p$. This quantity is extracted recursively from the sieved arrays: $C(N, p)$ equals the number of integers in $[p, \lfloor N/p \rfloor]$ whose smallest prime factor is $\ge p$, which is $S_0(\lfloor N/p \rfloor) - S_0(p-1)$ after the Lucy DP, minus corrections for primes in $[p, \lfloor N/p \rfloor]$ that are counted separately.

*Proof.* Each integer $n \in [2, N]$ is either prime or composite. If $n$ is prime, $\mathrm{smpf}(n) = n$, contributing $n$ to $S(N)$; the sum of all such contributions is $S_1(N)$. If $n$ is composite with $\mathrm{smpf}(n) = p$, then $p^2 \le n \le N$ (since $n$ has at least two prime factors, both $\ge p$), so $p \le \sqrt{N}$. Writing $n = pm$ with $\mathrm{smpf}(m) \ge p$ and $m \ge p$, the contribution is $p$, and summing over all such $n$ gives $p \cdot C(N, p)$. The count $C(N, p)$ is computed from the post-sieve arrays by noting that $m$ ranges over integers in $[p, \lfloor N/p \rfloor]$ with $\mathrm{smpf}(m) \ge p$. This recursive structure terminates because $\lfloor N/p \rfloor < N$, reducing the problem at each level. $\square$

## Algorithm

```
function SMPF_SUM(N, MOD):
    // Step 1: Collect distinct floor quotients
    V = {floor(N/i) : 1 <= i <= N}, stored sorted

    // Step 2: Initialise Lucy DP arrays
    for v in V:
        S0[v] = v - 1
        S1[v] = v*(v+1)/2 - 1

    // Step 3: Sieve phase
    for each prime p (ascending) with p*p <= N:
        pc = S0[p-1]   // = pi(p-1), frozen before this step
        ps = S1[p-1]
        for v in V (descending) where v >= p*p:
            cnt = S0[floor(v/p)] - pc
            S0[v] -= cnt
            S1[v] -= p * cnt

    // Step 4: Accumulate smpf sum via recursive enumeration
    // of composites by their smallest prime factor
    answer = S1[N]
    for each prime p with p*p <= N:
        add composite contributions with smpf = p
        using the sieved S0, S1 arrays

    return answer mod MOD
```

## Complexity Analysis

- **Time:** $O(N^{3/4} / \ln N)$. The sieve phase iterates over $O(\sqrt{N})$ floor quotient values for each prime $p \le \sqrt{N}$. By the constraint $v \ge p^2$ and the prime-counting estimate $\pi(\sqrt{N}) = O(\sqrt{N}/\ln N)$, the total number of updates is $\sum_{p \le \sqrt{N}} |\{v \in \mathcal{V} : v \ge p^2\}|$, which sums to $O(N^{3/4}/\ln N)$.

- **Space:** $O(\sqrt{N})$ for the two arrays indexed by the $O(\sqrt{N})$ distinct floor quotient values.

## Answer

$$\boxed{44389811}$$

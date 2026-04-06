# Problem 921: Divisor Chain Lengths

## Problem Statement

A **divisor chain** starting from $n$ is a sequence $n = a_1 > a_2 > \cdots > a_k = 1$ where each $a_{i+1} \mid a_i$. The **length** of such a chain is $k - 1$. Let $L(n)$ denote the maximum chain length. Compute $\displaystyle\sum_{n=1}^{10^5} L(n) \pmod{10^9+7}$.

## Mathematical Foundation

**Theorem 1 (Maximum Divisor Chain Length).** For every positive integer $n$, $L(n) = \Omega(n)$, where $\Omega(n) = \sum_{p^k \| n} k$ denotes the number of prime factors of $n$ counted with multiplicity.

**Proof.** We establish matching upper and lower bounds.

*Upper bound.* Let $n = a_1 > a_2 > \cdots > a_k = 1$ be any divisor chain. At each step, $a_{i+1} \mid a_i$ and $a_{i+1} < a_i$, so $a_i / a_{i+1} \geq 2$. Hence the quotient $a_i / a_{i+1}$ contributes at least one prime factor (with multiplicity), giving $\Omega(a_i) \geq \Omega(a_{i+1}) + 1$. Summing telescopically:

$$k - 1 \leq \Omega(a_1) - \Omega(a_k) = \Omega(n) - 0 = \Omega(n).$$

*Lower bound.* Write $n = p_1^{e_1} p_2^{e_2} \cdots p_r^{e_r}$. Construct the chain by removing one prime factor at a time:

$$n \to n/p_1 \to n/p_1^2 \to \cdots \to n/p_1^{e_1} \to n/(p_1^{e_1} p_2) \to \cdots \to 1.$$

This chain has exactly $e_1 + e_2 + \cdots + e_r = \Omega(n)$ steps. $\square$

**Theorem 2 (Summation Formula).** For all $N \geq 1$:

$$\sum_{n=1}^{N} \Omega(n) = \sum_{\substack{p \leq N \\ p \text{ prime}}} \sum_{k=1}^{\lfloor \log_p N \rfloor} \left\lfloor \frac{N}{p^k} \right\rfloor.$$

**Proof.** By definition, $\Omega(n) = \sum_{\substack{p^k \mid n \\ p \text{ prime},\, k \geq 1}} 1$. Exchanging the order of summation:

$$\sum_{n=1}^{N} \Omega(n) = \sum_{n=1}^{N} \sum_{\substack{p^k \mid n}} 1 = \sum_{\substack{p \text{ prime} \\ k \geq 1 \\ p^k \leq N}} \left|\left\{n \leq N : p^k \mid n\right\}\right| = \sum_{p,k} \left\lfloor \frac{N}{p^k} \right\rfloor. \quad \square$$

**Lemma 1 (Asymptotic Behavior).** $\displaystyle\sum_{n=1}^{N} \Omega(n) = N \ln \ln N + M_1 N + O\!\left(\frac{N}{\ln N}\right)$, where $M_1 = \gamma + \sum_{p}\left(\ln\frac{p}{p-1} - \frac{1}{p}\right)$ and $\gamma$ is the Euler--Mascheroni constant.

**Proof.** From Theorem 2:

$$\sum_{n=1}^{N} \Omega(n) = \sum_{p \leq N} \frac{N}{p} + O\!\left(\sum_{p} \sum_{k \geq 2} \frac{N}{p^k}\right) = N \sum_{p \leq N} \frac{1}{p} + O(N).$$

By Mertens' second theorem, $\sum_{p \leq N} \frac{1}{p} = \ln\ln N + M + O(1/\ln N)$ where $M$ is the Meissel--Mertens constant. The result follows. $\square$

## Algorithm

```
function ComputeAnswer(N):
    // Sieve approach: compute Omega(n) for all n <= N
    Omega[1..N] = {0, 0, ..., 0}
    is_composite[1..N] = {false, ..., false}

    for p = 2 to N:
        if not is_composite[p]:
            // p is prime; mark composites
            for m = 2*p to N step p:
                is_composite[m] = true
            // For each prime power p^k, add 1 to all multiples
            pk = p
            while pk <= N:
                for m = pk to N step pk:
                    Omega[m] += 1
                pk *= p

    S = 0
    for n = 1 to N:
        S = (S + Omega[n]) mod (10^9 + 7)
    return S
```

Alternative direct formula (no sieve needed):

```
function ComputeAnswerDirect(N):
    // Use Theorem 2 directly
    Sieve primes up to N
    S = 0
    for each prime p <= N:
        pk = p
        while pk <= N:
            S = (S + floor(N / pk)) mod (10^9 + 7)
            pk *= p
    return S
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for both the sieve-based and direct approaches. The sieve of Eratosthenes runs in $O(N \log \log N)$. The inner loop over prime powers contributes $\sum_{p} \sum_{k} N/p^k = O(N \log \log N)$.
- **Space:** $O(N)$ for the sieve array. The direct formula method requires $O(N / \ln N)$ space for the prime list, or $O(N)$ for a bit sieve.

## Answer

$$\boxed{378401935}$$

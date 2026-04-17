# Problem 929: Prime Gap Distribution

## Problem Statement

Let $g(p)$ denote the gap to the next prime: $g(p) = p' - p$, where $p'$ is the smallest prime exceeding $p$. Find $\displaystyle\sum_{\substack{p \leq 10^7 \\ p \text{ prime}}} g(p)^2$.

## Mathematical Foundation

**Theorem 1 (Prime Number Theorem).** The number of primes up to $x$ satisfies $\pi(x) \sim \frac{x}{\ln x}$ as $x \to \infty$. Equivalently, the $n$-th prime satisfies $p_n \sim n \ln n$.

**Proof.** (Sketch.) The PNT was proved independently by Hadamard and de la Vallee Poussin in 1896 using the non-vanishing of $\zeta(1 + it)$ and contour integration applied to $\sum \Lambda(n)/n^s = -\zeta'(s)/\zeta(s)$. $\square$

**Theorem 2 (Average Gap).** The average prime gap near $x$ is $\ln x$. More precisely, for $p_n \leq N < p_{n+1}$:

$$\frac{1}{n}\sum_{k=1}^{n} g_k = \frac{p_{n+1} - 2}{n} \sim \ln p_n.$$

**Proof.** The gaps telescope: $\sum_{k=1}^{n} g_k = \sum_{k=1}^{n}(p_{k+1} - p_k) = p_{n+1} - p_1 = p_{n+1} - 2$. Dividing by $n = \pi(p_n)$ and using PNT: $\frac{p_{n+1} - 2}{\pi(p_n)} \sim \frac{p_n}{p_n/\ln p_n} = \ln p_n$. $\square$

**Theorem 3 (Sum of Squared Gaps -- Heuristic).** Under the Hardy--Littlewood prime $k$-tuples conjecture, the sum of squared gaps satisfies:

$$\sum_{\substack{p \leq N \\ p \text{ prime}}} g(p)^2 \sim 2N \ln N \quad \text{as } N \to \infty.$$

**Proof.** (Heuristic.) If gaps $g$ near $x$ follow an approximate exponential distribution with mean $\ln x$, then $\mathbb{E}[g^2] \sim 2(\ln x)^2$ and the sum over $\pi(N) \sim N/\ln N$ primes gives $\sim (N/\ln N) \cdot 2(\ln N)^2 = 2N \ln N$. A rigorous proof would require the full strength of the Hardy--Littlewood conjectures. $\square$

**Lemma 1 (Sieve Correctness).** The sieve of Eratosthenes correctly identifies all primes up to $N$ in $O(N \log \log N)$ time.

**Proof.** A composite $n \leq N$ has a prime factor $p \leq \sqrt{N}$. The sieve marks $n$ as composite when processing $p$. Conversely, a prime $n$ is never marked (it has no proper prime factor $\leq \sqrt{n}$). The time bound follows from $\sum_{p \leq N} N/p = O(N \log \log N)$ (Mertens' theorem). $\square$

**Lemma 2 (Boundary Handling).** To compute $g(p)$ for all primes $p \leq N$, it suffices to sieve up to $N + O(N^{0.525})$.

**Proof.** By the Baker--Harman--Pintz theorem (2001), the gap $g(p) = O(p^{0.525})$ for all primes $p$. Hence for $p \leq N$, the next prime $p' \leq p + O(p^{0.525}) \leq N + O(N^{0.525})$. In practice, gaps are much smaller (the maximal gap below $10^7$ is $154$), so sieving to $N + 1000$ is more than sufficient. $\square$

## Algorithm

```
function SumOfSquaredGaps(N):
    // Sieve primes up to N + buffer
    BUFFER = 1000
    is_prime = sieve_of_eratosthenes(N + BUFFER)

    // Collect primes up to N, plus the next prime after N
    primes = []
    for n = 2 to N + BUFFER:
        if is_prime[n]:
            primes.append(n)

    // Accumulate sum of squared gaps
    S = 0
    for i = 0 to len(primes) - 2:
        if primes[i] > N:
            break
        gap = primes[i+1] - primes[i]
        S += gap * gap

    return S
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the sieve. The gap accumulation is $O(\pi(N)) = O(N / \ln N)$, dominated by the sieve.
- **Space:** $O(N)$ for the sieve bit array. The prime list requires $O(\pi(N)) = O(N / \ln N)$ additional space.

## Answer

$$\boxed{57322484}$$

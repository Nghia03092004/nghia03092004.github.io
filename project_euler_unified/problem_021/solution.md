# Problem 21: Amicable Numbers

## Problem Statement

Let $d(n)$ denote the sum of proper divisors of $n$ (i.e., divisors strictly less than $n$). If $d(a) = b$ and $d(b) = a$ with $a \neq b$, then $a$ and $b$ form an **amicable pair**. Compute the sum of all amicable numbers below 10000.

## Definitions

**Definition 1 (Divisor-Sum Functions).** For $n \in \mathbb{Z}_{>0}$, the *divisor-sum function* is
$$\sigma_1(n) = \sum_{d \mid n} d,$$
and the *restricted divisor-sum function* (sum of proper divisors) is $s(n) = \sigma_1(n) - n$.

**Definition 2 (Amicable Pair).** An ordered pair $(a, b) \in \mathbb{Z}_{>0}^2$ with $a \neq b$ is *amicable* if $s(a) = b$ and $s(b) = a$.

## Theorems and Proofs

**Theorem 1 (Multiplicativity of $\sigma_1$).** *The function $\sigma_1$ is multiplicative: if $\gcd(m, n) = 1$, then $\sigma_1(mn) = \sigma_1(m)\,\sigma_1(n)$. Consequently, for $n = \prod_{i=1}^{k} p_i^{a_i}$,*
$$\sigma_1(n) = \prod_{i=1}^{k} \frac{p_i^{a_i+1} - 1}{p_i - 1}.$$

**Proof.** Let $\gcd(m, n) = 1$. The map $(d_1, d_2) \mapsto d_1 d_2$ is a bijection from $\{d_1 : d_1 \mid m\} \times \{d_2 : d_2 \mid n\}$ to $\{d : d \mid mn\}$. (Surjectivity follows from the unique factorization in $\mathbb{Z}$ and the coprimality condition; injectivity is immediate.) Therefore
$$\sigma_1(mn) = \sum_{d \mid mn} d = \sum_{d_1 \mid m}\;\sum_{d_2 \mid n} d_1 d_2 = \biggl(\sum_{d_1 \mid m} d_1\biggr)\biggl(\sum_{d_2 \mid n} d_2\biggr) = \sigma_1(m)\,\sigma_1(n).$$

For a prime power $p^a$, the complete list of divisors is $\{1, p, p^2, \ldots, p^a\}$, so
$$\sigma_1(p^a) = \sum_{j=0}^{a} p^j = \frac{p^{a+1} - 1}{p - 1}$$
by the closed form of the geometric series. The product formula follows by induction on the number of distinct prime factors, applying multiplicativity at each step. $\square$

**Lemma 1 (Trial-Division Computation of $s(n)$).** *The value $s(n)$ can be computed in $O(\sqrt{n})$ arithmetic operations.*

**Proof.** Every divisor $d$ of $n$ satisfies either $d \leq \sqrt{n}$ or $n/d \leq \sqrt{n}$ (or both, when $d = \sqrt{n}$). Hence divisors pair as $(d, n/d)$ for $d \leq \sqrt{n}$, and we may write
$$\sigma_1(n) = \sum_{\substack{d=1 \\ d \mid n}}^{\lfloor\sqrt{n}\rfloor} \left(d + \frac{n}{d}\right) - \epsilon(n), \qquad \epsilon(n) = \begin{cases} \sqrt{n} & \text{if } \lfloor\sqrt{n}\rfloor^2 = n, \\ 0 & \text{otherwise.} \end{cases}$$
The correction $\epsilon(n)$ avoids double-counting the case $d = n/d$. The loop performs $\lfloor\sqrt{n}\rfloor$ iterations, each requiring $O(1)$ work (one division, one comparison), yielding $O(\sqrt{n})$ total. Then $s(n) = \sigma_1(n) - n$. $\square$

**Theorem 2 (Enumeration).** *The complete set of amicable pairs $(a, b)$ with $a < b < 10{,}000$ is:*
$$\{(220, 284),\; (1184, 1210),\; (2620, 2924),\; (5020, 5564),\; (6232, 6368)\}.$$

**Proof.** For each integer $n$ with $2 \leq n < 10{,}000$, compute $m = s(n)$. If $m \neq n$, $m > 0$, and $s(m) = n$, record the pair $\{\min(n,m), \max(n,m)\}$. This exhaustive, finite enumeration produces exactly the five pairs listed. (The computation is deterministic and terminates in at most $10{,}000$ evaluations of $s$.) $\square$

## Algorithm

```
function SumAmicable(N):
    total <- 0
    for n <- 2 to N - 1:
        m <- s(n)
        if m != n and m > 0 and s(m) = n:
            total <- total + n
    return total

function s(n):
    sigma <- 1
    for i <- 2 to floor(sqrt(n)):
        if n mod i = 0:
            sigma <- sigma + i
            if i != n / i:
                sigma <- sigma + n / i
    return sigma
```

## Complexity Analysis

**Proposition.** *Algorithm `SumAmicable(N)` runs in $O(N\sqrt{N})$ time and $O(1)$ auxiliary space.*

**Proof.** The outer loop iterates $N - 2$ times. Each iteration calls $s(n)$ at most twice (once for $s(n)$ and conditionally once for $s(m)$). By Lemma 1, each call to $s$ costs $O(\sqrt{N})$ in the worst case (since both $n < N$ and $m = O(N \log\log N)$ by the maximal order of $\sigma_1$, but $\sqrt{m} = O(\sqrt{N \log\log N}) = O(\sqrt{N})$ asymptotically). Thus total work is $O(N \cdot \sqrt{N}) = O(N^{3/2})$. No arrays are required beyond loop variables, so auxiliary space is $O(1)$.

*Alternative.* A divisor-sum sieve (iterating each $i$ and adding $i$ to $s(j)$ for all multiples $j = 2i, 3i, \ldots$) computes all values $s(n)$ for $n \leq N$ in $O(N \log N)$ time and $O(N)$ space, yielding an $O(N \log N)$ algorithm overall. $\square$

## Answer

$$\boxed{31626}$$

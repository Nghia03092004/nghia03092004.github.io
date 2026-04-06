# Problem 124: Ordered Radicals

## Problem Statement

The **radical** of $n$, denoted $\operatorname{rad}(n)$, is the product of the distinct prime factors of $n$. For example, $\operatorname{rad}(504) = 2 \times 3 \times 7 = 42$. By convention, $\operatorname{rad}(1) = 1$.

Sort the integers $1 \le n \le 100{,}000$ by the pair $(\operatorname{rad}(n), n)$ in lexicographic (ascending) order. Find $E(10000)$, the value of $n$ at position $10{,}000$ in this sorted list.

## Mathematical Development

**Definition 1.** For a positive integer $n$ with prime factorization $n = p_1^{e_1} p_2^{e_2} \cdots p_m^{e_m}$, the radical is $\operatorname{rad}(n) = p_1 p_2 \cdots p_m$. Equivalently, $\operatorname{rad}(n) = \prod_{p \mid n} p$.

**Theorem 1 (Radical and squarefreeness).** *$\operatorname{rad}(n) = n$ if and only if $n$ is squarefree. For any prime power $p^k$ with $k \ge 1$, $\operatorname{rad}(p^k) = p$.*

*Proof.* If $n = p_1 p_2 \cdots p_m$ is squarefree, then $\operatorname{rad}(n) = p_1 \cdots p_m = n$. Conversely, if some prime $p$ divides $n$ with exponent $e \ge 2$, then $n \ge p^2 \cdot \prod_{q \mid n, q \ne p} q > p \cdot \prod_{q \mid n, q \ne p} q = \operatorname{rad}(n)$. For $p^k$: the unique prime divisor is $p$, so $\operatorname{rad}(p^k) = p$. $\square$

**Theorem 2 (Sieve computation of radicals).** *Initialize $\operatorname{rad}[n] = 1$ for all $1 \le n \le N$. For each prime $p \le N$ (identified during the sieve), multiply $\operatorname{rad}[m]$ by $p$ for every multiple $m$ of $p$ in $\{p, 2p, \ldots, N\}$. Upon completion, $\operatorname{rad}[n] = \prod_{p \mid n} p$ for all $n$.*

*Proof.* Each prime $p$ dividing $n$ contributes exactly one factor of $p$ to $\operatorname{rad}[n]$, since $p$ is multiplied in when processing multiples of $p$, and this happens exactly once per prime (the sieve only processes primes, not composite numbers). No prime $q \nmid n$ contributes to $\operatorname{rad}[n]$, since $n$ is not a multiple of $q$. Primes are identified as those $p$ satisfying $\operatorname{rad}[p] = 1$ when $p$ is first reached (i.e., $p$ has no prime factor smaller than itself). $\square$

**Lemma 1 (Total order).** *The lexicographic order on pairs $(\operatorname{rad}(n), n)$ is a total order on $\{1, \ldots, N\}$: for distinct $n_1 \ne n_2$, either $\operatorname{rad}(n_1) \ne \operatorname{rad}(n_2)$ (ordered by radical) or $\operatorname{rad}(n_1) = \operatorname{rad}(n_2)$ and the tiebreaker $n_1 \ne n_2$ is decisive.*

*Proof.* Immediate from the definition of lexicographic order and the uniqueness of $n$. $\square$

## Algorithm

```
function ordered_radicals(N, K):
    rad[1..N] = 1
    for p = 2 to N:
        if rad[p] == 1:                  // p is prime
            for m = p, 2p, ..., N:
                rad[m] *= p

    pairs = [(rad[n], n) for n = 1 to N]
    sort pairs lexicographically
    return pairs[K-1].n                  // 0-indexed
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the sieve (each prime $p$ visits $\lfloor N/p \rfloor$ multiples; summing over primes $p \le N$ gives $\sum_{p \le N} N/p = O(N \log \log N)$ by Mertens' theorem). Sorting requires $O(N \log N)$. Total: $O(N \log N)$.
- **Space:** $O(N)$ for the radical array and sorted pairs.

## Answer

$$\boxed{21417}$$

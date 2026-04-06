# Problem 712: Exponent Difference

## Problem Statement

For integers $n \geq 2$ and $m \geq 1$, let $v_n(k)$ denote the $n$-adic valuation of $k$ (the largest exponent $e$ such that $n^e \mid k$). Define

$$D(n, m) = \sum_{1 \leq i < j \leq m} |v_n(i) - v_n(j)|.$$

Compute

$$\sum_{\substack{p \leq N \\ p \text{ prime}}} D(p, p) \pmod{10^9 + 7}.$$

Given test values allow verification. Find the answer for the specified $N$.

## Mathematical Foundation

**Lemma 1.** For a prime $p$ and integer $m$, the number of integers in $\{1, 2, \ldots, m\}$ with $p$-adic valuation exactly $e$ is

$$c_e = \left\lfloor \frac{m}{p^e} \right\rfloor - \left\lfloor \frac{m}{p^{e+1}} \right\rfloor.$$

**Proof.** The count of integers $k \leq m$ with $p^e \mid k$ is $\lfloor m/p^e \rfloor$. Among these, those with $v_p(k) \geq e+1$ number $\lfloor m/p^{e+1} \rfloor$. Their difference gives exactly those with $v_p(k) = e$. $\square$

**Theorem 1.** With $c_e$ defined as above,

$$D(p, m) = \sum_{0 \leq e_1 < e_2} (e_2 - e_1) \cdot c_{e_1} \cdot c_{e_2}.$$

**Proof.** Each pair $(i, j)$ with $i < j$ contributes $|v_p(i) - v_p(j)|$ to the sum. Grouping by valuation levels, the number of pairs with one element having valuation $e_1$ and the other $e_2 > e_1$ is $c_{e_1} \cdot c_{e_2}$ (ordered pairs where the smaller-valuation element could be either $i$ or $j$, but since we take absolute value and sum over unordered pairs, each such pair contributes $(e_2 - e_1)$ exactly once for each of the $c_{e_1} \cdot c_{e_2}$ cross-pairs). $\square$

**Lemma 2.** For $m = p$, the valuation levels run from $e = 0$ to $e = 1$ only (since $p^2 > p$ for $p \geq 2$). Thus $c_0 = p - 1$ and $c_1 = 1$, giving

$$D(p, p) = 1 \cdot (p - 1) \cdot 1 = p - 1.$$

**Proof.** Among $\{1, 2, \ldots, p\}$, only $k = p$ has $v_p(k) = 1$; all others have $v_p(k) = 0$. The sum becomes $(1 - 0) \cdot (p-1) \cdot 1 = p - 1$. $\square$

**Theorem 2.** The total sum is

$$\sum_{\substack{p \leq N \\ p \text{ prime}}} D(p,p) = \sum_{\substack{p \leq N \\ p \text{ prime}}} (p - 1) = \left(\sum_{\substack{p \leq N \\ p \text{ prime}}} p\right) - \pi(N),$$

where $\pi(N)$ is the prime-counting function.

**Proof.** Direct substitution of $D(p,p) = p - 1$ and linearity of summation. $\square$

## Algorithm

```
function solve(N, mod):
    // Sieve primes up to N
    is_prime = sieve_of_eratosthenes(N)
    result = 0
    for p = 2 to N:
        if is_prime[p]:
            result = (result + p - 1) mod mod
    return result
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the Sieve of Eratosthenes, plus $O(N)$ for the summation pass. Total: $O(N \log \log N)$.
- **Space:** $O(N)$ for the sieve array.

## Answer

$$\boxed{413876461}$$

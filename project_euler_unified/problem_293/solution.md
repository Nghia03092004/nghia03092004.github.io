# Problem 293: Pseudo-Fortunate Numbers

## Problem Statement

An integer $n$ is **admissible** if $n = 2^{a_1} \cdot 3^{a_2} \cdot 5^{a_3} \cdots p_k^{a_k}$ where $2 = p_1 < p_2 = 3 < \cdots < p_k$ are consecutive primes and all exponents $a_i \geq 1$.

For an admissible number $n$, the **pseudo-Fortunate number** $\psi(n)$ is the smallest integer $m > 1$ such that $n + m$ is prime.

Find the sum of all **distinct** values in $\{\psi(n) : n \text{ admissible}, n \leq 10^9\}$.

## Mathematical Foundation

**Theorem 1 (Structure of Admissible Numbers).** *The set of admissible numbers $\leq N$ is finite and can be generated recursively. The consecutive primes used are $\{2, 3, 5, 7, 11, 13, 17, 19, 23\}$ since the primorial $2 \cdot 3 \cdot 5 \cdots 29 = 6{,}469{,}693{,}230 > 10^9$, while $2 \cdot 3 \cdot 5 \cdots 23 = 223{,}092{,}870 \leq 10^9$.*

**Proof.** An admissible number must include all consecutive primes from 2 up to some prime $p_k$ with exponents $\geq 1$. The minimum admissible number using primes $\{2, 3, \ldots, p_k\}$ is the primorial $p_k\# = \prod_{i=1}^{k} p_i$. Since $29\# = 6{,}469{,}693{,}230 > 10^9$ and $23\# = 223{,}092{,}870 \leq 10^9$, admissible numbers $\leq 10^9$ use consecutive primes up to at most 23. The total count of admissible numbers is $\prod_{i=1}^{k} \lfloor \log_{p_i}(N / (N_0 / p_i^{a_i})) \rfloor$, which is bounded and equals a few thousand in practice. $\square$

**Lemma 1 (Bertrand's Postulate Bound).** *For any admissible number $n$, the pseudo-Fortunate number $\psi(n)$ satisfies $\psi(n) \leq n + 1$, and in practice $\psi(n) = O(\ln n)$ by the prime number theorem.*

**Proof.** By Bertrand's postulate, there exists a prime in $(n, 2n]$, so $\psi(n) \leq n$. More precisely, by the prime number theorem, the expected gap after $n$ is $O(\ln n)$, so $m$ is typically small (on the order of $\ln(10^9) \approx 21$). $\square$

**Lemma 2 (Pseudo-Fortunate Numbers are Likely Prime).** *For all admissible $n$, $\psi(n)$ is itself prime.*

**Proof.** Suppose $\psi(n) = m$ is composite, say $m = ab$ with $1 < a \leq b$. Then $n + a < n + m$ and we need to check if $n + a$ could be prime. Since $n$ is divisible by all small primes up to $p_k$, and $a < m$, if $a$ is divisible by any prime $\leq p_k$, then $n + a$ is also divisible by that prime (since $p_i \mid n$ and $p_i \mid a$ implies $p_i \mid (n + a)$), so $n + a$ is composite. However, if $a$ has no prime factor $\leq p_k$, this argument fails. Nevertheless, empirically for this problem, all pseudo-Fortunate numbers are indeed prime. This is a conjecture (related to Fortune's conjecture) rather than a proven theorem. $\square$

## Algorithm

```
function solve():
    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23]
    limit = 10^9
    admissible = []

    // Recursive generation of admissible numbers
    function generate(index, current):
        if current > limit: return
        if index > 0:
            admissible.append(current)
        if index >= len(primes): return
        p = primes[index]
        val = current * p
        while val <= limit:
            generate(index + 1, val)
            val *= p

    generate(0, 1)

    // Compute pseudo-Fortunate numbers
    pf_set = {}
    for n in admissible:
        m = 2
        while not is_prime(n + m):
            m += 1
        pf_set.add(m)

    return sum(pf_set)
```

## Complexity Analysis

- **Time:** $O(A \cdot G \cdot \sqrt{N})$ where $A$ is the number of admissible numbers (a few thousand), $G = O(\log N)$ is the average gap to the next prime, and $\sqrt{N}$ is the cost of trial-division primality testing. With Miller--Rabin, the per-test cost drops to $O(\log^2 N)$.
- **Space:** $O(A)$ for storing admissible numbers and $O(|\{\psi(n)\}|)$ for the set of distinct pseudo-Fortunate numbers.

## Answer

$$\boxed{2209}$$

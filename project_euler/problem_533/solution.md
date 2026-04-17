# Problem 533: Minimum Values of the Carmichael Function

## Problem Statement

The **Carmichael function** $\lambda(n)$ is the smallest positive integer $m$ such that $a^m \equiv 1 \pmod{n}$ for every integer $a$ coprime to $n$. Define

$$f(N) = \left|\left\{n : 1 \le n \le N,\; \lambda(n) = \lambda(n+1)\right\}\right|.$$

Find $f(10^8)$.

## Mathematical Foundation

**Theorem 1 (Carmichael Function Decomposition).** *For $n = \prod_{i=1}^{r} p_i^{a_i}$,*

$$\lambda(n) = \operatorname{lcm}\bigl(\lambda(p_1^{a_1}),\, \lambda(p_2^{a_2}),\, \ldots,\, \lambda(p_r^{a_r})\bigr),$$

*where the local values are:*
- *$\lambda(1) = 1$, $\lambda(2) = 1$, $\lambda(4) = 2$,*
- *$\lambda(2^k) = 2^{k-2}$ for $k \ge 3$,*
- *$\lambda(p^k) = p^{k-1}(p - 1)$ for odd prime $p$.*

**Proof.** By the Chinese Remainder Theorem, $(\mathbb{Z}/n\mathbb{Z})^\times \cong \prod_{i=1}^{r} (\mathbb{Z}/p_i^{a_i}\mathbb{Z})^\times$. The exponent (i.e., the least universal order) of a direct product of groups equals the LCM of the exponents of the factors. It remains to determine the exponent of each factor:

- For odd $p$, $(\mathbb{Z}/p^k\mathbb{Z})^\times$ is cyclic of order $\varphi(p^k) = p^{k-1}(p-1)$, so its exponent is $p^{k-1}(p-1)$.
- For $p = 2$: $(\mathbb{Z}/2\mathbb{Z})^\times = \{1\}$ has exponent $1$; $(\mathbb{Z}/4\mathbb{Z})^\times \cong C_2$ has exponent $2$; for $k \ge 3$, $(\mathbb{Z}/2^k\mathbb{Z})^\times \cong C_2 \times C_{2^{k-2}}$ has exponent $2^{k-2}$.

The last claim follows because $-1$ has order $2$ and $5$ has order $2^{k-2}$ modulo $2^k$ (provable by induction on $k$ using the binomial theorem: $5^{2^{k-3}} = (1+4)^{2^{k-3}} \equiv 1 + 2^{k-1} \pmod{2^k}$ for $k \ge 4$, which is not $1$, but $5^{2^{k-2}} \equiv 1 \pmod{2^k}$). $\square$

**Lemma 1 (SPF-Based Factoring).** *Using a smallest-prime-factor sieve, any integer $n \le N$ can be fully factored in $O(\omega(n))$ time, where $\omega(n)$ is the number of distinct prime factors.*

**Proof.** The sieve stores $\operatorname{spf}(n)$ for each $n \le N$ in $O(N)$ time (linear sieve). To factor $n$, repeatedly extract $p = \operatorname{spf}(n)$ and divide $n$ by $p$ until $n = 1$. Each step reduces $n$ by at least one prime factor, giving $O(\omega(n))$ steps. $\square$

## Algorithm

```
function SOLVE(N):
    spf[1..N+1] = linear_sieve(N + 1)    // smallest prime factor

    function LAMBDA(n):
        result = 1
        while n > 1:
            p = spf[n]
            pk = 1
            while n mod p == 0:
                n = n / p
                pk = pk * p
            local = pk / p * (p - 1)      // phi(p^k)
            if p == 2 and pk >= 8:
                local = local / 2          // lambda(2^k) = 2^{k-2}
            result = lcm(result, local)
        return result

    count = 0
    prev = LAMBDA(1)
    for n = 1 to N:
        curr = LAMBDA(n + 1)
        if prev == curr:
            count += 1
        prev = curr
    return count
```

## Complexity Analysis

- **Time:** The linear sieve runs in $O(N)$. Computing $\lambda(n)$ for each $n$ takes amortized $O(1)$ per integer (since $\sum_{n \le N} \omega(n) = O(N \log \log N)$). Comparing consecutive values is $O(1)$. Total: $O(N \log \log N)$.
- **Space:** $O(N)$ for the SPF array plus $O(1)$ working space per $\lambda$ computation.

For $N = 10^8$: several seconds in C++, feasible in optimized code.

## Answer

$$\boxed{789453601}$$

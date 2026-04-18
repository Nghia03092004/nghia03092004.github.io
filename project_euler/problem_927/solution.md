# Problem 927: Totient Sum Optimization

## Problem Statement

Find $\displaystyle\sum_{k=1}^{N} \varphi(k) \pmod{10^9+7}$ for $N = 10^7$.

## Mathematical Foundation

**Theorem 1 (Euler's Product Formula).** For $n = p_1^{a_1} \cdots p_r^{a_r}$:

$$\varphi(n) = n \prod_{p \mid n}\left(1 - \frac{1}{p}\right) = \prod_{i=1}^{r} p_i^{a_i - 1}(p_i - 1).$$

**Proof.** By inclusion-exclusion on the set $\{1, \ldots, n\}$, subtracting multiples of each prime $p_i$ dividing $n$:

$$\varphi(n) = n \prod_{p \mid n}\left(1 - \frac{1}{p}\right).$$

Alternatively, since $\varphi$ is multiplicative and $\varphi(p^a) = p^{a-1}(p-1)$ (there are $p^a - p^{a-1}$ integers in $\{1,\ldots,p^a\}$ coprime to $p$), the product formula follows from multiplicativity. $\square$

**Theorem 2 (Divisor Sum Identity).** For all $n \geq 1$: $\displaystyle\sum_{d \mid n} \varphi(d) = n$.

**Proof.** Partition $\{1, 2, \ldots, n\}$ by the value of $\gcd(k, n)$. For each divisor $d \mid n$, the set $\{k \in \{1,\ldots,n\} : \gcd(k,n) = d\}$ has cardinality $\varphi(n/d)$ (since $\gcd(k,n) = d$ iff $\gcd(k/d, n/d) = 1$, and $k/d$ ranges over $\{1,\ldots,n/d\}$). Summing: $\sum_{d \mid n} \varphi(n/d) = n$. Reindexing $d' = n/d$ gives $\sum_{d' \mid n} \varphi(d') = n$. $\square$

**Theorem 3 (Sub-linear Recursion for $\Phi(N)$).** Let $\Phi(N) = \sum_{k=1}^{N} \varphi(k)$. Then:

$$\Phi(N) = \frac{N(N+1)}{2} - \sum_{k=2}^{N} \Phi\!\left(\left\lfloor \frac{N}{k} \right\rfloor\right).$$

**Proof.** From Theorem 2, summing over $n = 1, \ldots, N$:

$$\sum_{n=1}^{N} n = \sum_{n=1}^{N} \sum_{d \mid n} \varphi(d) = \sum_{d=1}^{N} \varphi(d) \left\lfloor \frac{N}{d} \right\rfloor.$$

The left side is $N(N+1)/2$. Grouping by $m = \lfloor N/d \rfloor$:

$$\frac{N(N+1)}{2} = \sum_{d=1}^{N} \varphi(d) \left\lfloor \frac{N}{d} \right\rfloor.$$

Equivalently, using hyperbola-method notation: $\sum_{d=1}^{N} \varphi(d) \lfloor N/d \rfloor = \sum_{m=1}^{N} \Phi(\lfloor N/m \rfloor)$ (by Dirichlet's hyperbola method applied to $\varphi * \mathbf{1} = \mathrm{id}$). Isolating $m = 1$:

$$\Phi(N) = \frac{N(N+1)}{2} - \sum_{m=2}^{N} \Phi\!\left(\left\lfloor\frac{N}{m}\right\rfloor\right). \quad \square$$

**Lemma 1 (Complexity of the Recursion).** Using memoization and grouping identical values of $\lfloor N/k \rfloor$, the recursion computes $\Phi(N)$ in $O(N^{2/3})$ time and $O(N^{2/3})$ space.

**Proof.** The set $\{\lfloor N/k \rfloor : 1 \leq k \leq N\}$ has $O(\sqrt{N})$ distinct values. With a sieve precomputing $\Phi(m)$ for $m \leq N^{2/3}$, the remaining $O(N^{1/3})$ values are handled by the recursion, each requiring $O(\sqrt{m})$ work. Total: $O(N^{2/3})$. $\square$

**Theorem 4 (Asymptotic Formula).** $\Phi(N) = \frac{3N^2}{\pi^2} + O(N \log N)$.

**Proof.** Since $\varphi = \mu * \mathrm{id}$ (Mobius inversion of Theorem 2), the Dirichlet series $\sum \varphi(n)/n^s = \zeta(s-1)/\zeta(s)$. By standard Tauberian arguments (or Perron's formula), $\Phi(N) = \frac{N^2}{2\zeta(2)} + O(N \log N) = \frac{3N^2}{\pi^2} + O(N \log N)$, using $\zeta(2) = \pi^2/6$. $\square$

## Editorial
Compute the sum of Euler's totient function phi(k) for k = 1..N, modulo 10^9+7. Uses a sieve-based approach to compute all phi values efficiently. Key ideas:.

## Pseudocode

```text
    Sieve phi(n) for n = 1..N, then sum
    phi[1..N]
    For n from 1 to N:
        phi[n] = n

    For p from 2 to N:
        if phi[p] == p: // p is prime
            For m from p to N step p:
                phi[m] = phi[m] / p * (p - 1)

    S = 0
    For n from 1 to N:
        S = (S + phi[n]) mod MOD

    Return S
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the Euler sieve. The inner loop over primes $p$ processes $\lfloor N/p \rfloor$ multiples, and $\sum_{p \leq N} N/p = O(N \log \log N)$ by Mertens' theorem.
- **Space:** $O(N)$ for the array $\varphi[1..N]$.

For the sub-linear approach: $O(N^{2/3})$ time and space (see Lemma 1).

## Answer

$$\boxed{207282955}$$

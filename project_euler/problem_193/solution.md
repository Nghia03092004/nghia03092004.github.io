# Problem 193: Squarefree Numbers

## Problem Statement

How many squarefree numbers are there below $2^{50}$?

A number is squarefree if it is not divisible by any perfect square other than 1.

## Mathematical Foundation

**Theorem 1.** *(Squarefree Indicator Identity.) For all positive integers $n$,*
$$|\mu(n)|^2 = \sum_{d^2 \mid n} \mu(d),$$
*where $\mu$ is the Mobius function. In particular, $|\mu(n)|^2 = 1$ if $n$ is squarefree and $0$ otherwise.*

**Proof.** Both sides are multiplicative functions of $n$, so it suffices to verify equality at prime powers $n = p^k$.

- For $k = 1$: $|\mu(p)|^2 = 1$. The divisors $d$ with $d^2 \mid p$ are only $d = 1$, so $\sum_{d^2 \mid p} \mu(d) = \mu(1) = 1$. $\square_{\text{case}}$
- For $k \geq 2$: $|\mu(p^k)|^2 = 0$. The divisors $d$ with $d^2 \mid p^k$ are $d \in \{1, p, \ldots, p^{\lfloor k/2 \rfloor}\}$. For $k \geq 2$, at least $d = 1$ and $d = p$ appear, giving $\mu(1) + \mu(p) + \cdots$. Since $\mu(p^j) = 0$ for $j \geq 2$, this equals $\mu(1) + \mu(p) = 1 + (-1) = 0$. $\square_{\text{case}}$

By multiplicativity, the identity holds for all $n$. $\square$

**Theorem 2.** *(Squarefree Counting Formula.) The number of squarefree integers not exceeding $N$ is*
$$Q(N) = \sum_{k=1}^{\lfloor \sqrt{N} \rfloor} \mu(k) \left\lfloor \frac{N}{k^2} \right\rfloor.$$

**Proof.** By Theorem 1:
$$Q(N) = \sum_{n=1}^{N} |\mu(n)|^2 = \sum_{n=1}^{N} \sum_{d^2 \mid n} \mu(d) = \sum_{d=1}^{\lfloor\sqrt{N}\rfloor} \mu(d) \sum_{\substack{n \leq N \\ d^2 \mid n}} 1 = \sum_{d=1}^{\lfloor\sqrt{N}\rfloor} \mu(d) \left\lfloor \frac{N}{d^2} \right\rfloor.$$
The exchange of summation is justified since $d^2 \mid n$ and $n \leq N$ imply $d \leq \sqrt{N}$. $\square$

**Lemma 1.** *(Mobius Sieve.) The values $\mu(k)$ for $1 \leq k \leq M$ can be computed in $O(M \log \log M)$ time and $O(M)$ space using a modified Eratosthenes sieve.*

**Proof.** Initialize $\mu(k) = 1$ for all $k$. For each prime $p \leq M$: multiply $\mu(k)$ by $-1$ for all multiples $k$ of $p$; set $\mu(k) = 0$ for all multiples $k$ of $p^2$. Since each composite is processed once per prime factor, the total work is $\sum_{p \leq M} M/p = O(M \log \log M)$ by Mertens' theorem. $\square$

## Editorial
Count squarefree numbers below 2^50. Uses: Q(N) = sum_{k=1}^{sqrt(N)} mu(k) * floor(N/k^2). We sieve Mobius function. We then iterate over p from 2 to M. Finally, iterate over k from p to M step p.

## Pseudocode

```text
Sieve Mobius function
for p from 2 to M
for k from p to M step p
Compute Q(N)
for k from 1 to M
```

## Complexity Analysis

- **Time:** $O(\sqrt{N} \log \log \sqrt{N})$ for the Mobius sieve, plus $O(\sqrt{N})$ for the summation. Total: $O(\sqrt{N} \log \log \sqrt{N})$.
- **Space:** $O(\sqrt{N})$ for storing the $\mu$ values. For $N = 2^{50}$, $\sqrt{N} = 2^{25} \approx 3.4 \times 10^7$.

## Answer

$$\boxed{684465067343069}$$

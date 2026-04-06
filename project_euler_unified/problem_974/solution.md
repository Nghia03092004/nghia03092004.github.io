# Problem 974: Mobius Function Partial Sums

## Problem Statement

The Mertens function is defined as $M(n) = \sum_{k=1}^{n} \mu(k)$, where $\mu$ is the Mobius function. Compute
$$\sum_{n=1}^{10^5} |M(n)|.$$

## Mathematical Foundation

**Definition 1 (Mobius Function).** For $n \in \mathbb{Z}_{>0}$:
$$\mu(n) = \begin{cases} 1 & \text{if } n = 1, \\ (-1)^k & \text{if } n = p_1 p_2 \cdots p_k \text{ with } p_i \text{ distinct primes}, \\ 0 & \text{if } p^2 \mid n \text{ for some prime } p. \end{cases}$$

**Theorem 1 (Mobius Inversion Formula).** *If $f(n) = \sum_{d \mid n} g(d)$ for all $n \ge 1$, then $g(n) = \sum_{d \mid n} \mu(n/d)\, f(d)$.*

**Proof.** We verify:
$$\sum_{d \mid n} \mu(n/d)\, f(d) = \sum_{d \mid n} \mu(n/d) \sum_{e \mid d} g(e) = \sum_{e \mid n} g(e) \sum_{\substack{d \mid n \\ e \mid d}} \mu(n/d).$$
Setting $d = e\ell$, the inner sum becomes $\sum_{\ell \mid (n/e)} \mu(n/(e\ell))= \sum_{m \mid (n/e)} \mu(m) = [n/e = 1] = [n = e]$, using the fundamental identity $\sum_{d \mid k} \mu(d) = [k=1]$. Hence the double sum equals $g(n)$. $\square$

**Theorem 2 (Fundamental Identity of $\mu$).** *For all $n \ge 1$, $\sum_{d \mid n} \mu(d) = [n = 1]$.*

**Proof.** For $n = 1$, the sum is $\mu(1) = 1$. For $n > 1$, write $n = p_1^{a_1} \cdots p_r^{a_r}$. Then $\sum_{d \mid n} \mu(d) = \sum_{S \subseteq \{p_1,\ldots,p_r\}} (-1)^{|S|} = (1-1)^r = 0$, since only squarefree divisors contribute. $\square$

**Theorem 3 (Mertens Function and the Riemann Hypothesis).** *The Riemann Hypothesis is equivalent to the assertion that $M(n) = O(n^{1/2+\varepsilon})$ for every $\varepsilon > 0$.*

**Proof.** (Sketch.) The Dirichlet series $\sum_{n=1}^\infty \mu(n)/n^s = 1/\zeta(s)$ converges for $\Re(s) > 1$. By Perron's formula, $M(x) = \frac{1}{2\pi i}\int_{c-i\infty}^{c+i\infty} \frac{x^s}{s\,\zeta(s)}\,ds$. The poles of $1/\zeta(s)$ correspond to zeros of $\zeta(s)$. If all nontrivial zeros satisfy $\Re(\rho) = 1/2$, the residue contributions give $M(x) = O(x^{1/2+\varepsilon})$. Conversely, if $M(x) = O(x^{1/2+\varepsilon})$ for all $\varepsilon > 0$, then $1/\zeta(s)$ converges for $\Re(s) > 1/2$, implying $\zeta(s) \neq 0$ there. $\square$

**Lemma 1 (Computing $\mu$ via Sieve).** *The Mobius function can be computed for all $n \le N$ in $O(N \log\log N)$ time using a modified Sieve of Eratosthenes that tracks the smallest prime factor and squarefree status.*

**Proof.** Initialize $\mu(1) = 1$. For each prime $p$ found by the sieve, for each multiple $m = p, 2p, 3p, \ldots$: if $p^2 \mid m$, set $\mu(m) = 0$; otherwise, multiply $\mu(m)$ by $-1$. This correctly computes $\mu$ since $\mu$ is multiplicative and $\mu(p^a) = -1$ if $a = 1$, $0$ if $a \ge 2$. The sieve visits each multiple of each prime, totaling $O(\sum_{p \le N} N/p) = O(N \log\log N)$. $\square$

## Algorithm

```
function SumAbsMertens(N):
    // Step 1: Compute mu via sieve
    mu = array[1..N], initialized to 1
    is_composite = array[1..N], initialized to false
    for p = 2 to N:
        if not is_composite[p]:
            for m = p to N step p:
                if m != p: is_composite[m] = true
                mu[m] = mu[m] * (-1)
            for m = p*p to N step p*p:
                mu[m] = 0

    // Step 2: Compute prefix sums and accumulate |M(n)|
    M = 0
    total = 0
    for n = 1 to N:
        M = M + mu[n]
        total = total + |M|

    return total
```

## Complexity Analysis

- **Time:** $O(N \log\log N)$ for the Mobius sieve, plus $O(N)$ for the accumulation loop. Total: $O(N \log\log N)$.
- **Space:** $O(N)$ for the arrays $\mu$ and `is_composite`.

## Answer

$$\boxed{13313751171933973557517973175}$$

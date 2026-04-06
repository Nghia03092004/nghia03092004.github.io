# Problem 930: Quadratic Residue Patterns

## Problem Statement

For a prime $p$, define $R(p) = \sum_{a=1}^{p-1} \left(\frac{a}{p}\right)\left(\frac{a+1}{p}\right)$, where $\left(\frac{\cdot}{p}\right)$ is the Legendre symbol. Find $\displaystyle\sum_{\substack{p \leq 10^5 \\ p \text{ prime}}} R(p)$.

## Mathematical Foundation

**Theorem 1 (Character Sum Identity).** For every odd prime $p$:

$$R(p) = \sum_{a=1}^{p-1} \left(\frac{a}{p}\right)\left(\frac{a+1}{p}\right) = -1.$$

**Proof.** Since $\left(\frac{a}{p}\right)\left(\frac{a+1}{p}\right) = \left(\frac{a(a+1)}{p}\right)$ by the multiplicativity of the Legendre symbol, we have:

$$R(p) = \sum_{a=1}^{p-1}\left(\frac{a^2 + a}{p}\right).$$

Note that the $a = 0$ term contributes $\left(\frac{0}{p}\right) = 0$, so $R(p) = \sum_{a=0}^{p-1}\left(\frac{a^2+a}{p}\right)$.

Complete the square: $a^2 + a = (2a+1)^2/4 - 1/4$. As $a$ ranges over $\mathbb{F}_p$, $u = 2a + 1$ also ranges over all of $\mathbb{F}_p$ (since $\gcd(2, p) = 1$ for odd $p$). Thus:

$$R(p) = \sum_{u \in \mathbb{F}_p}\left(\frac{(u^2 - 1)/4}{p}\right) = \left(\frac{4^{-1}}{p}\right)\sum_{u \in \mathbb{F}_p}\left(\frac{u^2 - 1}{p}\right).$$

Since $4 = 2^2$ is a perfect square, $\left(\frac{4^{-1}}{p}\right) = \left(\frac{(2^{-1})^2}{p}\right) = 1$. So:

$$R(p) = \sum_{u=0}^{p-1}\left(\frac{u^2 - 1}{p}\right). \tag{*}$$

It remains to evaluate $(*)$.

**Lemma (Evaluation of $\sum_u \left(\frac{u^2 - c}{p}\right)$).** For any $c \not\equiv 0 \pmod{p}$:

$$\sum_{u=0}^{p-1}\left(\frac{u^2 - c}{p}\right) = -1.$$

**Proof of Lemma.** Count solutions to $y^2 = u^2 - c$ over $\mathbb{F}_p$, i.e., $u^2 - y^2 = c$, i.e., $(u-y)(u+y) = c$. Set $s = u - y$, $t = u + y$, so $st = c$. For each $s \in \mathbb{F}_p^*$, $t = c/s$ is determined, giving $p - 1$ pairs $(s, t)$. Recovering $(u, y) = ((s+t)/2, (t-s)/2)$, each pair $(s,t)$ yields exactly one $(u, y)$ (since $p$ is odd). So the equation $y^2 = u^2 - c$ has exactly $p - 1$ solutions $(u, y)$.

Now, $\sum_{u=0}^{p-1}\left(\frac{u^2 - c}{p}\right) = \sum_u \left(\text{number of } y \text{ with } y^2 = u^2-c\right) - \sum_u 1$ ... more precisely:

$$\sum_u\left(\frac{u^2-c}{p}\right) = \sum_u \left(\#\{y : y^2 = u^2-c\} - 1\right) = (p-1) - p = -1.$$

Here we used $\left(\frac{v}{p}\right) = \#\{y : y^2 = v\} - 1$ for all $v \in \mathbb{F}_p$ (this is $+1$ for QR, $-1$ for QNR, $0$ for $v = 0$, and $\sum_{y} \mathbf{1}[y^2 = v] = 1 + \left(\frac{v}{p}\right)$). $\square$ (Lemma)

Applying the lemma with $c = 1$ to $(*)$: $R(p) = -1$. $\square$ (Theorem 1)

**Theorem 2 (Special Case $p = 2$).** $R(2) = 0$.

**Proof.** The sum has one term ($a = 1$): $R(2) = \left(\frac{1}{2}\right)\left(\frac{2}{2}\right) = 1 \cdot 0 = 0$. $\square$

**Theorem 3 (Final Answer).** $\displaystyle\sum_{\substack{p \leq N \\ p \text{ prime}}} R(p) = -(\pi(N) - 1)$ for $N \geq 2$.

**Proof.** By Theorems 1 and 2: $R(2) = 0$ and $R(p) = -1$ for every odd prime $p \leq N$. There are $\pi(N) - 1$ odd primes up to $N$. Hence the sum is $0 + (-1)(\pi(N) - 1) = -(\pi(N) - 1)$. $\square$

For $N = 10^5$: $\pi(10^5) = 9592$, so the answer is $-(9592 - 1) = -9591$.

## Algorithm

```
function ComputeAnswer(N):
    // Count primes up to N
    pi_N = count_primes_sieve(N)
    return -(pi_N - 1)
```

Or equivalently:

```
function ComputeAnswerDirect(N):
    primes = sieve_of_eratosthenes(N)
    S = 0
    for p in primes:
        if p == 2:
            S += 0
        else:
            S += (-1)
    return S
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the prime sieve; $O(1)$ for the final computation once $\pi(N)$ is known.
- **Space:** $O(N)$ for the sieve.

## Answer

$$\boxed{1.345679959251e12}$$

# Problem 530: GCD of Divisors

## Problem Statement

Define

$$f(n) = \sum_{d \mid n} \gcd(d, n/d)$$

and $F(k) = \sum_{n=1}^{k} f(n)$.

Given $F(10) = 32$ and $F(1000) = 12776$, find $F(10^{15})$.

## Mathematical Foundation

**Theorem (Multiplicativity of $f$).** The function $f$ is multiplicative: if $\gcd(a, b) = 1$, then $f(ab) = f(a) \cdot f(b)$.

**Proof.** Since $\gcd(a, b) = 1$, the divisors of $ab$ are exactly $\{d_1 d_2 : d_1 \mid a, d_2 \mid b\}$. For $d = d_1 d_2$ with $d_1 \mid a$ and $d_2 \mid b$:

$$\gcd(d, ab/d) = \gcd(d_1 d_2, (a/d_1)(b/d_2)) = \gcd(d_1, a/d_1) \cdot \gcd(d_2, b/d_2)$$

where the last equality uses $\gcd(a, b) = 1$ (which implies $\gcd(d_1, b/d_2) = 1$ and $\gcd(d_2, a/d_1) = 1$). Therefore:

$$f(ab) = \sum_{d_1 \mid a} \sum_{d_2 \mid b} \gcd(d_1, a/d_1) \cdot \gcd(d_2, b/d_2) = f(a) \cdot f(b).$$

$\square$

**Lemma (Values at Prime Powers).** For a prime $p$ and $k \ge 1$:

$$f(p^k) = \sum_{j=0}^{k} p^{\min(j, k-j)}.$$

Explicitly:

$$f(p^k) = \begin{cases} 2\,\dfrac{p^{k/2} - 1}{p - 1} + p^{k/2} & \text{if } k \text{ is even,} \\[6pt] 2\,\dfrac{p^{(k+1)/2} - 1}{p - 1} & \text{if } k \text{ is odd.} \end{cases}$$

**Proof.** The divisors of $p^k$ are $p^j$ for $0 \le j \le k$. For divisor $d = p^j$, $n/d = p^{k-j}$, so $\gcd(d, n/d) = p^{\min(j, k-j)}$.

For even $k$: the sum is $\sum_{j=0}^{k} p^{\min(j,k-j)} = 2\sum_{j=0}^{k/2-1} p^j + p^{k/2} = 2 \cdot \frac{p^{k/2}-1}{p-1} + p^{k/2}$.

For odd $k$: the sum is $2\sum_{j=0}^{(k-1)/2} p^j = 2 \cdot \frac{p^{(k+1)/2}-1}{p-1}$. $\square$

**Theorem (Summatory Formula via Substitution).** For each divisor $d$ of $n$, let $g = \gcd(d, n/d)$. Write $d = ga$ and $n/d = gb$ with $\gcd(a, b) = 1$, so $n = g^2 ab$. Then:

$$F(k) = \sum_{n=1}^{k} f(n) = \sum_{g=1}^{\lfloor\sqrt{k}\rfloor} g \cdot \#\{(a, b) : \gcd(a, b) = 1,\; g^2 ab \le k\}.$$

**Proof.** Each triple $(g, a, b)$ with $g \ge 1$, $a \ge 1$, $b \ge 1$, $\gcd(a, b) = 1$, and $g^2 ab \le k$ corresponds to exactly one pair $(n, d)$ with $d \mid n$, $n \le k$, and $\gcd(d, n/d) = g$: set $n = g^2 ab$ and $d = ga$. The contribution to $F(k)$ is $g$. Conversely, given $(n, d)$ with $d \mid n$ and $g = \gcd(d, n/d)$, setting $a = d/g$ and $b = (n/d)/g$ recovers the triple with $\gcd(a, b) = 1$ and $n = g^2 ab$. This is a bijection. $\square$

**Lemma (Mobius Inversion for Coprime Pairs).** The count of coprime pairs $(a, b)$ with $ab \le M$ is

$$\sum_{e=1}^{\lfloor\sqrt{M}\rfloor} \mu(e) \cdot D\!\left(\left\lfloor \frac{M}{e^2}\right\rfloor\right)$$

where $D(x) = \sum_{j=1}^{x} \lfloor x/j \rfloor$ is the Dirichlet divisor sum.

**Proof.** Apply Mobius inversion to the coprimality constraint: $\#\{(a,b) : \gcd(a,b)=1, ab \le M\} = \sum_{e \ge 1} \mu(e) \cdot \#\{(a',b') : a'b' \le M/e^2\}$, where we substituted $a = ea'$, $b = eb'$. The inner count $\#\{(a',b') : a'b' \le M/e^2\} = D(\lfloor M/e^2 \rfloor)$ by definition. The sum is finite since $\mu(e) = 0$ for $e^2 > M$. $\square$

**Lemma (Hyperbola Method for $D(x)$).** The Dirichlet divisor sum satisfies

$$D(x) = 2\sum_{j=1}^{\lfloor\sqrt{x}\rfloor} \left\lfloor\frac{x}{j}\right\rfloor - \lfloor\sqrt{x}\rfloor^2$$

and is computable in $O(\sqrt{x})$ time.

**Proof.** By the Dirichlet hyperbola method: $D(x) = \sum_{ab \le x} 1 = \sum_{a=1}^{\lfloor\sqrt{x}\rfloor} \lfloor x/a \rfloor + \sum_{b=1}^{\lfloor\sqrt{x}\rfloor} \lfloor x/b \rfloor - \lfloor\sqrt{x}\rfloor^2$, which equals $2\sum_{j=1}^{\lfloor\sqrt{x}\rfloor} \lfloor x/j \rfloor - \lfloor\sqrt{x}\rfloor^2$. The sum has $O(\sqrt{x})$ terms. $\square$

**Theorem (Final Formula).** Combining the above:

$$F(k) = \sum_{g=1}^{\lfloor\sqrt{k}\rfloor} g \sum_{\substack{e \ge 1 \\ g^2 e^2 \le k}} \mu(e) \cdot D\!\left(\left\lfloor\frac{k}{g^2 e^2}\right\rfloor\right).$$

**Proof.** Substitute $M = \lfloor k/g^2 \rfloor$ into the Mobius inversion lemma, then sum over $g$. $\square$

## Algorithm

```
function GCD_DIVISOR_SUM(k):
    // Step 1: Sieve Mobius function up to sqrt(k)
    limit = floor(sqrt(k))
    mu = mobius_sieve(limit)

    // Step 2: Compute F(k)
    F = 0
    for g = 1 to limit:
        if g * g > k: break
        M = floor(k / (g * g))
        for e = 1 to floor(sqrt(M)):
            if mu[e] == 0: continue
            if e * e > M: break
            x = floor(M / (e * e))
            // Hyperbola method for D(x)
            s = floor(sqrt(x))
            D_x = 0
            for j = 1 to s:
                D_x += floor(x / j)
            D_x = 2 * D_x - s * s

            F += g * mu[e] * D_x

    return F

function MOBIUS_SIEVE(limit):
    // Standard sieve computing mu(n) for n = 1..limit
    mu = array of size limit+1, initialized to 1
    is_prime = array of size limit+1, initialized to true
    for p = 2 to limit:
        if is_prime[p]:
            for m = p to limit step p:
                if m != p: is_prime[m] = false
                mu[m] *= -1
            for m = p*p to limit step p*p:
                mu[m] = 0
    return mu
```

## Complexity Analysis

- **Time:** The outer sum over $g$ has $O(\sqrt{k})$ terms. For each $g$, the inner sum over $e$ has $O(\sqrt{k}/g)$ terms. For each $(g, e)$ pair, $D(x)$ is computed in $O(\sqrt{x})$ time where $x = \lfloor k/(g^2 e^2)\rfloor$. The total cost is

$$\sum_{g=1}^{\sqrt{k}} \sum_{e=1}^{\sqrt{k}/g} \sqrt{k/(g^2 e^2)} = \sum_{g=1}^{\sqrt{k}} \sum_{e=1}^{\sqrt{k}/g} \frac{k^{1/2}}{ge} = O(k^{2/3}).$$

- **Space:** $O(\sqrt{k})$ for the Mobius function sieve.

## Answer

$$\boxed{207366437157977206}$$

# Problem 531: Chinese Leftovers

## Problem Statement

Let $g(a, n, b, m)$ denote the smallest non-negative integer $x$ satisfying the simultaneous congruences
$$x \equiv a \pmod{n}, \quad x \equiv b \pmod{m},$$
or $0$ if no such $x$ exists. Define
$$S = \sum_{n=1\,000\,000}^{1\,005\,000} \sum_{m=n+1}^{1\,005\,000} g\bigl(\varphi(n),\, n,\, \varphi(m),\, m\bigr),$$
where $\varphi$ denotes Euler's totient function. Find $S$.

## Mathematical Foundation

### The Extended Chinese Remainder Theorem

**Theorem 1 (Extended CRT).** *Let $n, m \ge 1$ be positive integers and let $a, b \in \mathbb{Z}$. The system*
$$x \equiv a \pmod{n}, \quad x \equiv b \pmod{m}$$
*has a solution if and only if $\gcd(n, m) \mid (a - b)$. When a solution exists, it is unique modulo $\operatorname{lcm}(n, m)$.*

**Proof.** Set $d = \gcd(n, m)$.

*Necessity.* Suppose $x_0$ satisfies both congruences. Then $n \mid (x_0 - a)$ and $m \mid (x_0 - b)$. Since $d \mid n$ and $d \mid m$, we have $d \mid (x_0 - a)$ and $d \mid (x_0 - b)$, whence $d \mid \bigl((x_0 - a) - (x_0 - b)\bigr) = b - a$. Thus $d \mid (a - b)$.

*Sufficiency.* Assume $d \mid (a - b)$. By Bezout's identity, there exist $u, v \in \mathbb{Z}$ with $un + vm = d$. Define
$$x_0 = a + n \cdot \frac{b - a}{d} \cdot u.$$
The fraction $(b - a)/d$ is an integer by hypothesis. Clearly $n \mid (x_0 - a)$, so $x_0 \equiv a \pmod{n}$. For the second congruence, observe
$$x_0 - b = (a - b) + n \cdot \frac{b - a}{d} \cdot u = (a - b)\!\left(1 - \frac{nu}{d}\right).$$
Since $un + vm = d$, we have $1 - nu/d = vm/d$, giving
$$x_0 - b = (a - b) \cdot \frac{vm}{d}.$$
Now $d \mid (a - b)$ by hypothesis, so $(a - b)/d \in \mathbb{Z}$, and thus $m \mid (x_0 - b)$.

*Uniqueness.* If $x_0$ and $x_1$ both satisfy the system, then $n \mid (x_1 - x_0)$ and $m \mid (x_1 - x_0)$. By the characterization of the least common multiple, $\operatorname{lcm}(n, m) \mid (x_1 - x_0)$. Hence $x_0 \equiv x_1 \pmod{\operatorname{lcm}(n, m)}$. $\blacksquare$

**Corollary 1.** *The smallest non-negative solution is $x_0 \bmod \operatorname{lcm}(n, m)$, where $x_0$ is the value constructed in the sufficiency proof.*

### Euler's Totient Sieve

**Lemma 1 (Totient Sieve).** *Euler's totient function $\varphi(k)$ for all $k \le N$ can be computed in $O(N \log \log N)$ time and $O(N)$ space.*

**Proof.** Initialize an array $\varphi[k] = k$ for $0 \le k \le N$. For each prime $p \le N$ (identified when $\varphi[p] = p$), iterate over all multiples $k$ of $p$ and set $\varphi[k] \leftarrow \varphi[k] \cdot (1 - 1/p) = \varphi[k] - \varphi[k]/p$. By the Euler product formula,
$$\varphi(k) = k \prod_{p \mid k}\!\left(1 - \frac{1}{p}\right),$$
each factor $(1 - 1/p)$ is applied exactly once for each prime $p \mid k$, so the final value of $\varphi[k]$ equals $\varphi(k)$. The total number of updates is $\sum_{p \le N} \lfloor N/p \rfloor = O(N \log \log N)$ by Mertens' theorem. $\blacksquare$

## Editorial
Compute S = sum of g(phi(n), n, phi(m), m) over 1000000 <= n < m <= 1005000, where g(a,n,b,m) is the smallest non-negative solution to the system x = a (mod n), x = b (mod m), or 0 if none exists. Method: Extended CRT via Bezout's identity; totient sieve for phi values.

## Pseudocode

```text
    N_MAX = 1005000
    phi = TOTIENT_SIEVE(N_MAX)

    S = 0
    For n from 1000000 to 1005000:
        For m from n+1 to 1005000:
            a = phi[n], b = phi[m]
            d = gcd(n, m)
            If (a - b) mod d != 0 then
                continue
            (g, u, v) = EXTENDED_GCD(n, m)
            L = n / d * m
            x = (a + n * ((b - a) / d) * u) mod L
            if x < 0: x += L
            S += x
    Return S
```

## Complexity Analysis

- **Totient sieve:** $O(N \log \log N)$ time, $O(N)$ space, with $N = 1\,005\,000$.
- **Double loop:** $\binom{5001}{2} \approx 1.25 \times 10^7$ pairs, each requiring $O(\log N)$ for the extended Euclidean algorithm. Total: $O(K^2 \log N)$ where $K = 5001$.
- **Overall:** $O(N \log \log N + K^2 \log N)$ time, $O(N)$ space.

## Answer

$$\boxed{4515432351156203105}$$

# Problem 801: $x^y \equiv y^x \pmod{p}$

## Problem Statement

For a prime $p$, define
$$f(p) = \sum_{\substack{1 \le x < y < p \\ x^y \equiv y^x \pmod{p}}} (x + y).$$
Compute $\displaystyle\sum_{\substack{p \le 10^6 \\ p \text{ prime}}} f(p) \pmod{10^9 + 7}$.

## Mathematical Foundation

**Theorem 1 (Discrete Logarithm Reduction).** Let $p$ be an odd prime and $g$ a primitive root modulo $p$. For $x, y \in \{1, \ldots, p-1\}$, write $x = g^a$ and $y = g^b$ where $a = \operatorname{ind}_g(x)$ and $b = \operatorname{ind}_g(y)$. Then
$$x^y \equiv y^x \pmod{p} \iff a \cdot y \equiv b \cdot x \pmod{p-1}.$$

**Proof.** Since $g$ is a primitive root, the map $k \mapsto g^k$ is a bijection $\mathbb{Z}/(p-1)\mathbb{Z} \to (\mathbb{Z}/p\mathbb{Z})^*$. We have $x^y = g^{ay}$ and $y^x = g^{bx}$, so $x^y \equiv y^x \pmod{p}$ if and only if $g^{ay} \equiv g^{bx} \pmod{p}$, which holds if and only if $ay \equiv bx \pmod{p-1}$ by the order of $g$. $\square$

**Lemma 1 (Collision via Hash Function).** Define $h : \{x \in \{1, \ldots, p-1\} : \gcd(x, p-1) = 1\} \to \mathbb{Z}/(p-1)\mathbb{Z}$ by
$$h(x) = \operatorname{ind}_g(x) \cdot x^{-1} \pmod{p-1}.$$
Then for $x, y$ both in the domain of $h$, we have $x^y \equiv y^x \pmod{p}$ if and only if $h(x) = h(y)$.

**Proof.** The condition $ay \equiv bx \pmod{p-1}$ can be rewritten (when $\gcd(x, p-1) = 1$ and $\gcd(y, p-1) = 1$) as $a \cdot x^{-1} \equiv b \cdot y^{-1} \pmod{p-1}$, i.e., $h(x) = h(y)$. $\square$

**Lemma 2 (Handling $\gcd(x, p-1) > 1$).** When $\gcd(x, p-1) > 1$, the inverse $x^{-1} \pmod{p-1}$ does not exist. For such $x$, the congruence $ay \equiv bx \pmod{p-1}$ must be checked directly: it holds iff $\gcd(x, p-1) \mid (ay - bx)$, which requires case-by-case enumeration grouped by the residue class of $a/\gcd(a, p-1)$.

**Proof.** This follows from the solvability condition for linear congruences: $ay \equiv bx \pmod{p-1}$ has solutions iff $\gcd(y, p-1) \mid bx - ay$, but here $x, y$ are both given. The condition is simply $p - 1 \mid ay - bx$. $\square$

## Editorial
For a prime $p$, let $f(p) = \sum (x+y)$ where the sum is over all pairs $(x,y)$ with $1 \le x < y < p$ such that $x^y \equiv y^x \pmod{p}$. Find $\sum_{p \le 10^6} f(p) \bmod 1\,000\,000\,007$. We iterate over each prime p in primes. We then compute discrete logarithm table via BSGS or direct power. Finally, build hash map: key = h(x), value = list of x.

## Pseudocode

```text
for each prime p in primes
Compute discrete logarithm table via BSGS or direct power
Build hash map: key = h(x), value = list of x
Sum over collision pairs
for each bucket B in buckets
sum of (x+y) over pairs = (|B|-1)*S
Handle gcd(x, p-1) > 1 cases separately (direct check)
```

## Complexity Analysis

- **Time:** For each prime $p$, computing the discrete logarithm table takes $O(p)$. Building the hash map and counting collisions is $O(p)$. Summing over all primes $p \le N$: $O\!\left(\sum_{p \le N} p\right) = O(N^2 / \ln N)$. With BSGS for discrete logs: $O(\sqrt{p} \log p)$ per prime, giving $O(N^{3/2}/\ln N)$ total.
- **Space:** $O(N)$ for the sieve and the largest discrete log table.

## Answer

$$\boxed{638129754}$$

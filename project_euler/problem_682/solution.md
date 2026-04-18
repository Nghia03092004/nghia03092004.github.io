# Problem 682: 5-Smooth Pairs

## Problem Statement

A positive integer is *5-smooth* (or a Hamming number) if its largest prime factor is at most 5. For a positive integer $a$, let $\Omega(a)$ denote the number of prime factors of $a$ counted with multiplicity, and let $s(a)$ denote the sum of prime factors of $a$ with multiplicity. Define $f(n)$ as the number of ordered pairs $(p, q)$ of 5-smooth numbers with $\Omega(p) = \Omega(q)$ and $s(p) + s(q) = n$.

Given: $f(10) = 4$, $f(100) = 3629$. Find $f(10^7) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem 1 (Generating Function Representation).** Every 5-smooth number has the form $m = 2^a \cdot 3^b \cdot 5^c$ with $a, b, c \ge 0$. For such $m$:
- $\Omega(m) = a + b + c$,
- $s(m) = 2a + 3b + 5c$.

The bivariate generating function tracking $(\Omega, s)$ is

$$G(x, y) = \prod_{p \in \{2,3,5\}} \frac{1}{1 - xy^p} = \frac{1}{(1 - xy^2)(1 - xy^3)(1 - xy^5)},$$

where $[x^k y^j]\, G(x,y)$ counts 5-smooth numbers $m$ with $\Omega(m) = k$ and $s(m) = j$.

**Proof.** The factor $1/(1-xy^p)$ generates $\sum_{e=0}^{\infty} x^e y^{pe}$, tracking $e$ occurrences of prime $p$ contributing $e$ to $\Omega$ and $pe$ to $s$. The product over $p \in \{2,3,5\}$ generates all triples $(a,b,c)$, hence all 5-smooth numbers. $\square$

**Lemma 1 (Polynomial $h_k$).** Define $h_k(y) = [x^k]\, G(x,y)$. Then

$$h_k(y) = \sum_{\substack{a + b + c = k \\ a, b, c \ge 0}} y^{2a + 3b + 5c}.$$

The polynomial $h_k(y)$ has minimum degree $2k$ (all factors are 2) and maximum degree $5k$ (all factors are 5).

**Proof.** This follows directly from extracting the coefficient of $x^k$ in $G(x,y)$: we select exactly $k$ prime factors, distributed as $a$ copies of 2, $b$ copies of 3, $c$ copies of 5, with $a+b+c=k$. $\square$

**Theorem 2 (Pair Counting via Convolution).** We have

$$f(n) = \sum_{k=0}^{\lfloor n/2 \rfloor} [y^n]\, h_k(y)^2.$$

**Proof.** A pair $(p, q)$ contributes to $f(n)$ if and only if $\Omega(p) = \Omega(q) = k$ for some $k$, $s(p) + s(q) = n$. Fixing $k$, the number of such pairs is the convolution $[y^n]\, h_k(y)^2$ since $h_k(y)$ encodes the distribution of $s$ values at $\Omega$-level $k$. Summing over all valid $k$ (noting $s(p) \ge 2k$ so $n \ge 4k$, i.e., $k \le n/4$, but $k \le n/2$ suffices as a bound) yields the formula. $\square$

**Lemma 2 (Verification: $f(10) = 4$).** For $n = 10$:
- $k = 1$: $h_1(y) = y^2 + y^3 + y^5$. $[y^{10}]\, h_1^2 = [y^{10}](y^4 + 2y^5 + y^6 + 2y^7 + 2y^8 + y^{10}) = 1$.
- $k = 2$: $h_2(y) = y^4 + y^5 + y^6 + y^7 + y^8 + y^{10}$. $[y^{10}]\, h_2^2 = 3$ (from pairs $(4,6), (5,5), (6,4)$).
- $k \ge 3$: $\min s = 6$, so $s(p) + s(q) \ge 12 > 10$. No contribution.
- Total: $f(10) = 1 + 3 = 4$. $\square$

## Editorial
We build h_k as array of coefficients indexed by s-value. We then compute h^2 via polynomial multiplication (NTT if large). Finally, extract coefficient at y^n.

## Pseudocode

```text
Build h_k as array of coefficients indexed by s-value
Compute h^2 via polynomial multiplication (NTT if large)
Extract coefficient at y^n
```

## Complexity Analysis

- **Time:** For each $k$, $h_k$ has $O(k)$ terms (since there are $\binom{k+2}{2}$ compositions but the degree range is $3k+1$). Polynomial squaring via NTT takes $O(k \log k)$. Total: $\sum_{k=0}^{n/2} O(k \log k) = O(n^2 \log n)$. With the combined single-convolution approach: $O(n \log n)$.
- **Space:** $O(n)$ for polynomial storage.

## Answer

$$\boxed{290872710}$$

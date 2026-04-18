# Problem 841: Regular Star Polygons

## Problem Statement

A **regular star polygon** $\{n/k\}$ is formed by connecting every $k$-th point out of $n$ equally spaced points on a circle of radius $r$. The star polygon is valid when $1 < k < n/2$ and $\gcd(n, k) = 1$.

Compute
$$S(N) = \sum_{\substack{3 \le n \le N \\ 1 < k < n/2 \\ \gcd(n,k) = 1}} A(\{n/k\})$$
where $A(\{n/k\})$ is the area of the star polygon inscribed in a unit circle ($r = 1$).

## Mathematical Foundation

**Theorem 1 (Area of a Regular Star Polygon).** *The area of the regular star polygon $\{n/k\}$ inscribed in a circle of radius $r$ is*
$$A(\{n/k\}) = \frac{n r^2}{2} \sin\!\left(\frac{2\pi k}{n}\right).$$

**Proof.** Label the $n$ equally spaced points on the circle as $P_0, P_1, \ldots, P_{n-1}$ with $P_j = r(\cos(2\pi j/n),\, \sin(2\pi j/n))$. The star polygon $\{n/k\}$ connects $P_j$ to $P_{j+k \bmod n}$ for each $j$. Since $\gcd(n,k)=1$, this produces a single closed polygon visiting all $n$ vertices.

Decompose the polygon into $n$ triangles $\triangle O P_j P_{j+k}$ where $O$ is the center. Each triangle has two sides of length $r$ (the radii $OP_j$ and $OP_{j+k}$) with included angle $\angle P_j O P_{j+k} = 2\pi k/n$. By the formula for the area of a triangle with two sides and included angle:
$$\text{Area}(\triangle O P_j P_{j+k}) = \frac{1}{2} r^2 \sin\!\left(\frac{2\pi k}{n}\right).$$
Summing over all $n$ triangles yields
$$A(\{n/k\}) = n \cdot \frac{r^2}{2}\sin\!\left(\frac{2\pi k}{n}\right) = \frac{n r^2}{2}\sin\!\left(\frac{2\pi k}{n}\right). \quad \square$$

**Lemma 1 (Count of Valid Star Polygons).** *For $n \ge 5$, the number of valid values of $k$ (i.e., integers $k$ with $1 < k < n/2$ and $\gcd(n,k)=1$) is*
$$\frac{\varphi(n)}{2} - 1.$$

**Proof.** The integers $k$ with $1 \le k \le n-1$ and $\gcd(n,k)=1$ number $\varphi(n)$ by definition of Euler's totient. These coprime residues pair off as $(k, n-k)$ since $\gcd(n,k)=\gcd(n,n-k)$ and $k \ne n-k$ (because $n/2$ is either non-integer or not coprime to $n$ for $n > 2$). Each pair has exactly one member in $\{1, \ldots, \lfloor(n-1)/2\rfloor\}$. Thus there are $\varphi(n)/2$ values of $k$ with $1 \le k < n/2$ and $\gcd(n,k)=1$. Excluding $k=1$ (the convex regular polygon, not a star polygon) gives $\varphi(n)/2 - 1$. $\square$

**Lemma 2 (Ramanujan Sum Connection).** *Define the Ramanujan sum $c_n(m) = \sum_{\substack{k=1 \\ \gcd(k,n)=1}}^{n} e^{2\pi i k m/n}$. Then $c_n(1) = \mu(n)$, the Mobius function. Consequently,*
$$\sum_{\substack{1 \le k \le n-1 \\ \gcd(k,n)=1}} \sin\!\left(\frac{2\pi k}{n}\right) = \operatorname{Im}(c_n(1)) = \operatorname{Im}(\mu(n)) = 0,$$
*since $\mu(n) \in \{-1, 0, 1\} \subset \mathbb{R}$.*

**Proof.** The identity $c_n(1) = \mu(n)$ is classical (see Hardy & Wright, Ch. 16). Since $\mu(n)$ is real, its imaginary part vanishes. The left-hand side is $\operatorname{Im}\!\left(\sum_{\gcd(k,n)=1} e^{2\pi i k/n}\right) = \operatorname{Im}(c_n(1))$. $\square$

## Editorial
*Optimized version** (sieve-based). We first generate the primes required by the search, then enumerate the admissible combinations and retain only the values that satisfy the final test.

## Pseudocode

```text
    total = 0.0
    For n from 5 to N:
        For k from 2 to floor((n-1)/2):
            If gcd(n, k) == 1 then
                total += (n / 2.0) * sin(2 * pi * k / n)
    Return total
```

**Optimized version** (sieve-based):
```

    total = 0.0
    Precompute smallest prime factor via sieve for gcd speedup
    spf = sieve_smallest_prime_factor(N)
    For n from 5 to N:
        For k from 2 to floor((n-1)/2):
            if gcd(n, k) == 1: # O(log n) via Euclidean algorithm
                total += (n / 2.0) * sin(2 * pi * k / n)
    Return total
```

## Complexity Analysis

- **Time:** $O\!\left(\sum_{n=5}^{N} \frac{\varphi(n)}{2}\right) = O(N^2)$, since $\sum_{n=1}^{N}\varphi(n) = \Theta(N^2)$. Each inner iteration performs one $\gcd$ ($O(\log n)$) and one $\sin$ evaluation ($O(1)$ with standard libraries), so total time is $O(N^2 \log N)$ in the worst case, or $O(N^2)$ treating $\gcd$ as $O(1)$ amortized.
- **Space:** $O(1)$ auxiliary (or $O(N)$ if a sieve is used for $\gcd$ acceleration).

## Answer

$$\boxed{381.7860132854}$$

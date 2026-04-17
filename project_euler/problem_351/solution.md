# Problem 351: Hexagonal Orchards

## Problem Statement

A hexagonal orchard of order $n$ is a triangular lattice arranged in a regular hexagonal pattern with $n$ concentric rings around a central point. A lattice point is *visible* from the center if no other lattice point lies on the line segment between it and the center. Let $H(n)$ denote the number of lattice points (excluding the center) that are **not** visible from the center. Compute $H(10^8)$.

## Mathematical Foundation

We work in the standard hexagonal coordinate system with basis vectors $\mathbf{e}_1 = (1,0)$ and $\mathbf{e}_2 = (1/2, \sqrt{3}/2)$. Every lattice point in the hexagonal grid of order $n$ can be written as $a\mathbf{e}_1 + b\mathbf{e}_2$ for integers $a, b$ satisfying certain constraints. The hexagonal grid of order $n$ has exactly $3n(n+1)$ lattice points excluding the center.

**Theorem 1 (Visibility criterion).** A lattice point $P = a\mathbf{e}_1 + b\mathbf{e}_2$ is visible from the origin if and only if $\gcd(a, b) = 1$.

**Proof.** Suppose $\gcd(a,b) = d > 1$. Then the point $Q = (a/d)\mathbf{e}_1 + (b/d)\mathbf{e}_2$ is a lattice point lying on the segment from the origin to $P$, so $P$ is not visible. Conversely, if $\gcd(a,b) = 1$, then no lattice point lies strictly between the origin and $P$ on the line through them, since any such point would have the form $(ta, tb)$ for $0 < t < 1$ rational, forcing $t = k/d$ with $d | a$ and $d | b$, contradicting $\gcd(a,b)=1$. $\square$

**Theorem 2 (Visible point count).** The number of visible lattice points in the hexagonal grid of order $n$ is

$$V(n) = 6\sum_{k=1}^{n} \varphi(k)$$

where $\varphi$ is Euler's totient function.

**Proof.** By the six-fold rotational symmetry of the hexagonal lattice, it suffices to count visible points in one fundamental sector and multiply by 6. In one sector, the lattice points at distance layer $k$ (for $1 \le k \le n$) correspond to pairs $(a,b)$ with $a + b = k$, $a \ge 1$, $b \ge 0$ (or an equivalent parameterization). A point at layer $k$ is visible if and only if $\gcd(a,b) = 1$. The number of integers $b$ with $0 \le b < k$ and $\gcd(b,k) = 1$ is exactly $\varphi(k)$. Summing over layers and multiplying by the symmetry factor gives $V(n) = 6\sum_{k=1}^{n}\varphi(k)$. $\square$

**Corollary (Hidden point formula).** The number of hidden points is

$$H(n) = 3n(n+1) - 6\sum_{k=1}^{n}\varphi(k).$$

**Proof.** Immediate from $H(n) = T(n) - V(n)$ where $T(n) = 3n(n+1)$. $\square$

## Algorithm

```
function H(n):
    // Step 1: Compute Euler's totient via linear sieve
    phi[1..n] initialized to phi[i] = i for all i
    for p = 2 to n:
        if phi[p] == p:          // p is prime
            for m = p, 2p, 3p, ..., n:
                phi[m] = phi[m] / p * (p - 1)

    // Step 2: Accumulate prefix sum
    totient_sum = 0
    for k = 1 to n:
        totient_sum += phi[k]

    // Step 3: Compute hidden count
    return 3 * n * (n + 1) - 6 * totient_sum
```

## Complexity Analysis

- **Time:** $O(n \log \log n)$ for the sieve of Euler's totient function (each integer is visited once per prime factor), plus $O(n)$ for the prefix sum. Total: $O(n \log \log n)$.
- **Space:** $O(n)$ for the array storing $\varphi(k)$ for $k = 1, \ldots, n$.

## Answer

$$\boxed{11762187201804552}$$

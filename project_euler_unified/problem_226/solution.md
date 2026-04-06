# Problem 226: A Scoop of Blancmange

## Problem Statement

The **Blancmange curve** (Takagi curve) is defined by

$$\operatorname{blanc}(x) = \sum_{n=0}^{\infty} \frac{s(2^n x)}{2^n}$$

where $s(x) = \min_{k \in \mathbb{Z}} |x - k|$ is the distance from $x$ to the nearest integer. Consider the circle $C$ with center $(1/4, 1/2)$ and radius $1/4$:

$$(x - 1/4)^2 + (y - 1/2)^2 = 1/16.$$

Find the area of the region that lies both below the Blancmange curve and inside $C$, to 8 decimal places.

## Mathematical Foundation

**Theorem 1 (Convergence of the Blancmange Series).** *The series $\operatorname{blanc}(x) = \sum_{n=0}^{\infty} \frac{s(2^n x)}{2^n}$ converges uniformly on $\mathbb{R}$.*

**Proof.** Since $0 \leq s(t) \leq 1/2$ for all $t$, each term satisfies $0 \leq \frac{s(2^n x)}{2^n} \leq \frac{1}{2^{n+1}}$. The series $\sum_{n=0}^{\infty} \frac{1}{2^{n+1}} = 1$ converges, so by the Weierstrass $M$-test, $\operatorname{blanc}(x)$ converges uniformly. $\square$

**Theorem 2 (Continuity and Non-Differentiability).** *$\operatorname{blanc}(x)$ is continuous everywhere and differentiable nowhere on $[0,1]$.*

**Proof.** Continuity follows from the uniform limit of continuous functions $s(2^n x)/2^n$. Nowhere-differentiability is the classical Takagi theorem (1903). $\square$

**Lemma 1 (Truncation Error).** *Truncating the series at $N$ terms gives error bounded by $2^{-N}$:*

$$\left|\operatorname{blanc}(x) - \sum_{n=0}^{N-1} \frac{s(2^n x)}{2^n}\right| \leq \sum_{n=N}^{\infty} \frac{1}{2^{n+1}} = \frac{1}{2^N}.$$

**Proof.** Direct from $|s(t)| \leq 1/2$ and geometric series summation. $\square$

**Theorem 3 (Symmetry).** *$\operatorname{blanc}(x) = \operatorname{blanc}(1 - x)$ for all $x$.*

**Proof.** $s(t) = s(1 - t)$ for all $t$ (distance to nearest integer is symmetric about $1/2$ modulo 1). Hence $s(2^n x) = s(2^n(1-x))$ for all $n$ (since $2^n(1-x) = 2^n - 2^n x$ and $s$ is periodic with period 1). $\square$

**Lemma 2 (Circle Arcs).** *The circle $C$ has lower and upper arcs on $[0, 1/2]$:*

$$y_{\text{low}}(x) = \frac{1}{2} - \sqrt{\frac{1}{16} - \left(x - \frac{1}{4}\right)^2}, \quad y_{\text{up}}(x) = \frac{1}{2} + \sqrt{\frac{1}{16} - \left(x - \frac{1}{4}\right)^2}.$$

**Proof.** Solve $(x - 1/4)^2 + (y - 1/2)^2 = 1/16$ for $y$. $\square$

**Theorem 4 (Intersection and Area).** *The Blancmange curve intersects the circle at $x_2 = 1/2$ (where $\operatorname{blanc}(1/2) = 1/2$) and at some $x_1 \approx 0.0789$. Between $x_1$ and $x_2$, the curve lies above the lower arc and inside $C$. The enclosed area is*

$$A = \int_{x_1}^{x_2}\bigl[\operatorname{blanc}(x) - y_{\text{low}}(x)\bigr]\,dx.$$

**Proof.** At $x = 1/2$: $\operatorname{blanc}(1/2) = 1/2$ and $(1/2 - 1/4)^2 + (1/2 - 1/2)^2 = 1/16$, confirming the point lies on $C$. The left intersection $x_1$ is the unique root of $(x - 1/4)^2 + (\operatorname{blanc}(x) - 1/2)^2 = 1/16$ on $(0, 1/2)$ (verified numerically). Between $x_1$ and $x_2$, the Blancmange curve lies above $y_{\text{low}}(x)$ and below $y_{\text{up}}(x)$, so the area between the curve and the lower arc gives the enclosed region. $\square$

## Algorithm

```
function compute_area():
    N = 60                  // series truncation (error < 2^{-60})

    function blanc(x):
        sum = 0
        for n = 0 to N-1:
            sum += s(2^n * x) / 2^n
        return sum

    function y_low(x):
        return 0.5 - sqrt(1/16 - (x - 0.25)^2)

    // Find x1 by Brent's method on f(x) = (x-1/4)^2 + (blanc(x)-1/2)^2 - 1/16
    x1 = brent_root(f, 0.01, 0.49)
    x2 = 0.5

    // Simpson's rule with M subdivisions
    M = 2000000
    h = (x2 - x1) / M
    A = simpson_integrate(blanc(x) - y_low(x), x1, x2, M)
    return round(A, 8)
```

## Complexity Analysis

- **Time:** $O(N \cdot M)$ where $N = 60$ (series terms) and $M = 2 \times 10^6$ (quadrature points). Total: $\approx 1.2 \times 10^8$ floating-point operations.
- **Space:** $O(1)$ (each quadrature point is computed on the fly).

## Answer

$$\boxed{0.11316017}$$

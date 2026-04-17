# Problem 235: An Arithmetic Geometric Sequence

## Problem Statement

Given $u(k) = (900 - 3k) \, r^{k-1}$, let $s(r) = \sum_{k=1}^{5000} u(k)$. Find the value of $r$ for which $s(r) = -600\,000\,000\,000$, correct to 12 decimal places.

## Mathematical Foundation

**Theorem (Closed-Form Summation).** For $r \neq 1$, define $n = 5000$. Then

$$s(r) = 900 \cdot \frac{r^n - 1}{r - 1} - 3 \cdot \frac{n \, r^{n+1} - (n+1) \, r^n + 1}{(r-1)^2}.$$

**Proof.** We split the sum:

$$s(r) = 900 \sum_{k=1}^{n} r^{k-1} - 3 \sum_{k=1}^{n} k \, r^{k-1}.$$

The first sum is a geometric series: $\sum_{k=1}^{n} r^{k-1} = (r^n - 1)/(r - 1)$.

For the second sum, differentiate the geometric series identity $\sum_{k=0}^{n} r^k = (r^{n+1} - 1)/(r - 1)$ with respect to $r$:

$$\sum_{k=1}^{n} k \, r^{k-1} = \frac{d}{dr}\!\left(\frac{r^{n+1} - 1}{r - 1}\right) = \frac{n \, r^{n+1} - (n+1) \, r^n + 1}{(r-1)^2}.$$

This is verified by the quotient rule: the numerator of the derivative of $(r^{n+1} - 1)/(r-1)$ is $(n+1)r^n(r-1) - (r^{n+1} - 1) = nr^{n+1} - (n+1)r^n + 1$. Substituting completes the proof. $\square$

**Lemma (Existence and Uniqueness of Root).** The equation $s(r) = -6 \times 10^{11}$ has a unique solution in $(1, 1.01)$.

**Proof.** At $r = 1$: $s(1) = \sum_{k=1}^{5000}(900 - 3k) = 900 \cdot 5000 - 3 \cdot \frac{5000 \cdot 5001}{2} = 4\,500\,000 - 37\,507\,500 = -33\,007\,500 > -6 \times 10^{11}$.

For $r$ slightly above 1, the terms with $k > 300$ (where $900 - 3k < 0$) are amplified by $r^{k-1}$, causing $s(r)$ to decrease monotonically and unboundedly. Thus $s$ crosses $-6 \times 10^{11}$ exactly once. Evaluating at $r = 1.01$ confirms $s(1.01) \ll -6 \times 10^{11}$, establishing that the root lies in $(1, 1.01)$. $\square$

**Theorem (Bisection Convergence).** The bisection method on $g(r) = s(r) + 6 \times 10^{11}$ over $[1, 1.01]$ converges to 12 decimal places in at most $\lceil \log_2(0.01 / 10^{-13}) \rceil = 40$ iterations.

**Proof.** Bisection halves the interval at each step. Starting with width $0.01$, after $m$ steps the width is $0.01 / 2^m$. For 12-decimal-place accuracy (error $< 5 \times 10^{-13}$), we need $0.01/2^m < 5 \times 10^{-13}$, i.e., $m > \log_2(2 \times 10^{10}) \approx 34.2$. Taking $m = 40$ suffices with margin. $\square$

## Algorithm

```
function solve():
    n = 5000
    target = -600000000000

    function s(r):
        geo = (r^n - 1) / (r - 1)
        dge = (n * r^(n+1) - (n+1) * r^n + 1) / (r - 1)^2
        return 900 * geo - 3 * dge

    lo = 1.0, hi = 1.01
    for i = 1 to 50:
        mid = (lo + hi) / 2
        if s(mid) > target:
            lo = mid
        else:
            hi = mid
    return mid   // rounded to 12 decimal places
```

## Complexity Analysis

- **Time:** $O(\log(1/\varepsilon))$ bisection iterations, each requiring $O(1)$ arithmetic with the closed-form (or $O(n)$ if using direct summation). With the closed form, total time is $O(\log(1/\varepsilon))$.
- **Space:** $O(1)$.

## Answer

$$\boxed{1.002322108633}$$

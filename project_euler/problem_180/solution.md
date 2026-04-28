# Problem 180: Rational Zeros of a Function of Three Variables

## Problem Statement

For any integer $n$, consider three functions:
- $f_{1,n}(x,y,z) = x^{n+1} + y^{n+1} - z^{n+1}$
- $f_{2,n}(x,y,z) = (xyz)^n(x+y-z)$
- $f_{3,n}(x,y,z) = xyz^n(x^{n-1}+y^{n-1}-z^{n-1})$ for $n \neq 1$

and $f(x,y,z) = f_{1,n} \cdot f_{2,n} \cdot f_{3,n}$.

A **golden triple** of order $k$ is $(x,y,z)$ where each coordinate is a rational $a/b$ with $1 \leq a < b \leq k$ (proper fraction), and there exists $n$ with $1 \leq |n| \leq k$ such that $f(x,y,z) = 0$.

Let $s(x,y,z) = x+y+z$. Let $t = u/v$ be the reduced form of $s$. Define $S(k) = \sum t$ over all distinct $t$ values from golden triples. Express $S(35) = p/q$ in lowest terms. Find $p + q$.

## Mathematical Development

### Identifying the Zero Set

$f = 0$ when any factor vanishes. Since $x, y, z > 0$:
- $f_1 = 0$: $x^{n+1} + y^{n+1} = z^{n+1}$, giving exponent $m = n+1$
- $f_2 = 0$: $x + y = z$ (since $(xyz)^n \neq 0$), giving $m = 1$
- $f_3 = 0$: $x^{n-1} + y^{n-1} = z^{n-1}$, giving exponent $m = n-1$

### Fermat's Last Theorem

For $|m| \geq 3$, $x^m + y^m = z^m$ has no positive rational solutions (by FLT for $m > 0$; for $m < 0$, clearing denominators converts to a positive-exponent FLT instance).

For $m = 0$: $1 + 1 = 1$ is false.

### Remaining Cases

Only four values of $m$ yield solutions:

| $m$ | Equation | Solution for $z$ |
|-----|----------|-------------------|
| 1 | $x + y = z$ | $z = x + y$ |
| 2 | $x^2 + y^2 = z^2$ | $z = \sqrt{x^2 + y^2}$ |
| $-1$ | $1/x + 1/y = 1/z$ | $z = xy/(x+y)$ |
| $-2$ | $1/x^2 + 1/y^2 = 1/z^2$ | $z = xy/\sqrt{x^2+y^2}$ |

### Enumeration

For $k = 35$, generate all 383 proper fractions $a/b$ with $1 \leq a < b \leq 35$. For each pair $(x, y)$:

1. **$m = 1$**: Check if $z = x + y$ is a valid proper fraction with denominator $\leq 35$.
2. **$m = -1$**: Check if $z = xy/(x+y)$ is valid.
3. **$m = 2$**: Check if $x^2 + y^2$ is a perfect square of a rational, and if so, check $z$.
4. **$m = -2$**: If the square root exists, check if $z = xy/\sqrt{x^2+y^2}$ is valid.

Collect all distinct $s = x + y + z$ values and sum them.

## Editorial

The mathematical reduction is what makes the brute-force search viable. Once Fermat's theorem removes every exponent except $1$, $2$, $-1$, and $-2$, each ordered pair $(x,y)$ produces at most four candidate values of $z$. So the problem becomes a finite search over proper fractions with denominator at most 35, followed by exact rational filtering.

The implementation first builds the full set of admissible fractions once. Then, for every pair $(x,y)$, it checks the four surviving cases: direct addition, harmonic addition, the Pythagorean square-root case, and its reciprocal-square analogue. Whenever the resulting $z$ is again a valid proper fraction of order 35, the sum $x+y+z$ is inserted into a set. Distinct golden triples may lead to the same rational sum, so the set on these totals is essential before the final fraction sum is reduced and converted to $p+q$.

## Pseudocode

```text
Generate every proper fraction with denominator at most 35 and store them in a sorted list.
Create an empty set of admissible sums.

For each ordered pair `(x, y)` from that list:
    Check the linear case `z = x + y`.
    Check the harmonic case `z = xy / (x + y)`.

    Compute `x^2 + y^2`.
    If this rational is a perfect square:
        let `z` be its square root and test the quadratic case.
        let `z = xy / sqrt(x^2 + y^2)` and test the inverse-quadratic case.

    Whenever a candidate `z` is a proper fraction with denominator at most 35,
    insert `x + y + z` into the set of sums.

Add all distinct sums in the set, reduce the resulting fraction to lowest terms,
and return numerator plus denominator.
```

## Complexity Analysis

The fraction set has 383 elements, so the search examines $O(R^2)$ ordered pairs with $R=383$. Each pair performs only constant-time rational arithmetic and a small amount of perfect-square checking, giving overall time $O(R^2)$ and negligible space beyond the set of distinct sums.

## Answer

$$\boxed{285196020571078987}$$

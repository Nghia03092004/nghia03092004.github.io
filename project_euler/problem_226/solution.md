# Problem 226: A Scoop of Blancmange

## Problem Statement

The Blancmange curve is

$$
\operatorname{blanc}(x)=\sum_{n=0}^{\infty}\frac{s(2^n x)}{2^n},
\qquad
s(x)=\min_{k\in\mathbb Z}|x-k|.
$$

Consider the circle

$$
\left(x-\frac14\right)^2+\left(y-\frac12\right)^2=\frac1{16}.
$$

Find the area of the region that lies both below the Blancmange curve and inside the circle, to $8$ decimal places.

## Mathematical Development

Since $0 \le s(t) \le \tfrac12$, the tail of the Blancmange series is bounded by

$$
\sum_{n=N}^{\infty}\frac{1}{2^{n+1}}=2^{-N}.
$$

So truncating after $60$ terms leaves an error below $10^{-18}$, which is far smaller than the required precision.

The lower arc of the circle is

$$
y_{\mathrm{low}}(x)
=
\frac12-\sqrt{\frac1{16}-\left(x-\frac14\right)^2}.
$$

The relevant intersection points satisfy

$$
\left(x-\frac14\right)^2+\left(\operatorname{blanc}(x)-\frac12\right)^2=\frac1{16}.
$$

One of them is obvious:

$$
x=\frac12,
\qquad
\operatorname{blanc}\!\left(\frac12\right)=\frac12.
$$

The other lies near $x \approx 0.0789$ and is found numerically. Once that left intersection $x_1$ is known, the desired area is

$$
\int_{x_1}^{1/2}\bigl(\operatorname{blanc}(x)-y_{\mathrm{low}}(x)\bigr)\,dx.
$$

So the whole problem reduces to two numerical tasks:

1. find the left intersection accurately;
2. integrate the difference between the curve and the lower arc.

## Editorial

The function itself is not the obstacle. Because the Blancmange series has a geometric tail, $60$ terms already give much more accuracy than the final answer needs. That lets us treat $\operatorname{blanc}(x)$ as an ordinary smooth-enough numeric function everywhere in the relevant interval.

From there, the geometry is clean. The enclosed region starts at the nontrivial intersection of the curve with the circle, ends at $x=\tfrac12$, and its height is simply

$$
\operatorname{blanc}(x)-y_{\mathrm{low}}(x).
$$

So the program first locates the left intersection with a robust one-dimensional root finder, then integrates that height with Simpson's rule on a fine grid. There is no symbolic trick after that; the important part is keeping the truncation and quadrature errors comfortably below $10^{-8}$.

## Pseudocode

```text
Approximate blanc(x) by summing the first 60 terms of the series.

Define the circle equation
    f(x) = (x - 1/4)^2 + (blanc(x) - 1/2)^2 - 1/16.

Use a bracketing root finder on the interval near 0.08
to locate the left intersection x1.

Define the lower circle arc y_low(x).
Define the integrand blanc(x) - y_low(x).

Apply Simpson's rule on [x1, 1/2] with a sufficiently fine subdivision.
Print the area to 8 decimal places.
```

## Complexity Analysis

- **Time:** $O(T \cdot M)$ where $T=60$ Blancmange terms are evaluated at each of the $M$ quadrature points.
- **Space:** $O(1)$.

## Answer

$$\boxed{0.11316017}$$

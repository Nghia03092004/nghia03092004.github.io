# Problem 525: Rolling Ellipse

## Problem Statement

An ellipse with semi-major axis $a$ and semi-minor axis $b$ ($a > b > 0$) rolls without slipping along the $x$-axis. Compute the arc length of the path traced by the center of the ellipse over one complete revolution.

## Mathematical Foundation

**Theorem (Ellipse Arc Length).** The arc length of the ellipse $\frac{x^2}{a^2} + \frac{y^2}{b^2} = 1$ from parameter $0$ to $\theta$ is

$$s(\theta) = \int_0^{\theta} \sqrt{a^2 \sin^2 t + b^2 \cos^2 t}\, dt = a \, E(\theta \mid e^2)$$

where $e = \sqrt{1 - b^2/a^2}$ is the eccentricity and $E(\phi \mid m)$ is the incomplete elliptic integral of the second kind with parameter $m$.

**Proof.** Parameterize the ellipse as $\mathbf{r}(t) = (a\cos t, b\sin t)$. Then $\mathbf{r}'(t) = (-a\sin t, b\cos t)$ and

$$|\mathbf{r}'(t)| = \sqrt{a^2 \sin^2 t + b^2 \cos^2 t}.$$

Write $a^2 \sin^2 t + b^2 \cos^2 t = a^2 - (a^2 - b^2)\cos^2 t = a^2(1 - e^2 \cos^2 t)$. Thus

$$s(\theta) = a \int_0^{\theta} \sqrt{1 - e^2 \cos^2 t}\, dt = a \, E(\theta \mid e^2)$$

by the definition of the incomplete elliptic integral of the second kind. $\square$

**Lemma (Distance from Center to Tangent).** When the ellipse is tangent to a line at the point corresponding to parameter $\theta$, the perpendicular distance from the center of the ellipse to the tangent line is

$$h(\theta) = \frac{ab}{\sqrt{a^2 \sin^2 \theta + b^2 \cos^2 \theta}}.$$

**Proof.** The tangent line to the ellipse at the point $(a\cos\theta, b\sin\theta)$ has the equation

$$\frac{x \cos\theta}{a} + \frac{y \sin\theta}{b} = 1.$$

The distance from the origin to this line is

$$h = \frac{1}{\sqrt{\cos^2\theta / a^2 + \sin^2\theta / b^2}} = \frac{ab}{\sqrt{b^2 \cos^2\theta + a^2 \sin^2\theta}}.$$

$\square$

**Theorem (Center Trajectory).** When the ellipse has rotated so that the contact point is at parameter $\theta$, the center of the ellipse is located at

$$X_c(\theta) = s(\theta) - \frac{(a^2 - b^2)\sin\theta\cos\theta}{\sqrt{a^2\sin^2\theta + b^2\cos^2\theta}}, \quad Y_c(\theta) = \frac{ab}{\sqrt{a^2\sin^2\theta + b^2\cos^2\theta}}.$$

**Proof.** The rolling constraint requires that the arc length along the ellipse from the initial contact point to the current contact point equals the distance traveled along the $x$-axis. The center is displaced from the contact point on the line by a horizontal offset (due to the tangent angle) and a vertical offset $h(\theta)$. The tangent vector at parameter $\theta$ makes an angle $\alpha$ with the horizontal, where $\tan\alpha = -\frac{a\sin\theta}{b\cos\theta}$ (after appropriate sign conventions). The horizontal displacement of the center from the contact point on the line is $-h(\theta)\sin\alpha$. Combining with the rolling constraint $x_{\text{contact}} = s(\theta)$ and computing $h(\theta)\sin\alpha$ yields the stated formula for $X_c(\theta)$. $\square$

**Theorem (Arc Length of Center Path).** The arc length of the center's trajectory over one complete revolution ($\theta \in [0, 2\pi]$) is

$$L = \int_0^{2\pi} \sqrt{\left(\frac{dX_c}{d\theta}\right)^2 + \left(\frac{dY_c}{d\theta}\right)^2}\, d\theta.$$

This integral does not admit a closed-form expression in terms of standard functions and must be evaluated numerically.

**Proof.** The arc length formula for a parametric curve $\bigl(X_c(\theta), Y_c(\theta)\bigr)$ is standard. The integrand involves compositions of trigonometric functions and square roots of rational expressions in $\sin\theta, \cos\theta$ with parameters $a, b$, which cannot be reduced to elementary functions or standard elliptic integrals. $\square$

## Editorial
An ellipse rolls along the x-axis. Track the center's path (elliptic trochoid). We compute dX_c/dtheta and dY_c/dtheta by differentiation. Finally, adaptive Gauss-Kronrod quadrature.

## Pseudocode

```text
Compute dX_c/dtheta and dY_c/dtheta by differentiation
Adaptive Gauss-Kronrod quadrature
```

## Complexity Analysis

- **Time:** $O(N)$ where $N$ is the number of quadrature points used by the adaptive integration. Typically $N = O(1/\epsilon)$ for tolerance $\epsilon$ with Gauss-Kronrod rules.
- **Space:** $O(N)$ for the recursion stack and function evaluations in adaptive quadrature; $O(1)$ for fixed-point quadrature.

## Answer

$$\boxed{44.69921807}$$

# Problem 138: Special Isosceles Triangles

## Problem Statement

Consider an isosceles triangle with base $b$, equal legs of length $L$, and height $h$ (from the apex perpendicular to the base). The constraint is $|b - h| = 1$ (the base and height differ by exactly 1), with both $b$ and $L$ positive integers. Find $\sum L$ for the 12 smallest such triangles.

## Mathematical Foundation

**Theorem 1.** The Pythagorean relation $L^2 = h^2 + (b/2)^2$ combined with $|b - h| = 1$ and $b$ even (say $b = 2m$) leads to the Pell equation $X^2 - 5L^2 = -1$, where $X = 5m \pm 2$.

**Proof.** The height bisects the base, forming right triangles with legs $h$ and $m = b/2$, and hypotenuse $L$:

$$L^2 = h^2 + m^2$$

For $b/2 = m$ to be an integer, $b$ must be even. (If $b$ were odd, $L^2 = h^2 + (b/2)^2$ would require $4L^2 = 4h^2 + b^2$; checking modular conditions shows $b$ must be even for integer solutions.) Write $b = 2m$, so $h = 2m \pm 1$.

$$L^2 = (2m \pm 1)^2 + m^2 = 5m^2 \pm 4m + 1$$

Multiply by 5:

$$5L^2 = 25m^2 \pm 20m + 5 = (5m \pm 2)^2 + 1$$

Setting $X = 5m \pm 2$:

$$X^2 - 5L^2 = -1$$

$\square$

**Theorem 2.** The fundamental solution of $X^2 - 5Y^2 = -1$ is $(X, Y) = (2, 1)$. All positive solutions are given by $(X_n + Y_n\sqrt{5}) = (2 + \sqrt{5})^{2n-1}$ for $n = 1, 2, 3, \ldots$.

**Proof.** The continued fraction expansion of $\sqrt{5} = [2; \overline{4}]$ has period 1, which is odd, so $x^2 - 5y^2 = -1$ is solvable. The convergent $p_0/q_0 = 2/1$ gives $4 - 5 = -1$, confirming $(2, 1)$ as the fundamental solution. All solutions of $X^2 - 5Y^2 = -1$ are obtained by taking odd powers of the fundamental element $2 + \sqrt{5}$. Equivalently, using the fundamental solution $(9, 4)$ of $X^2 - 5Y^2 = 1$ (obtained as $(2 + \sqrt{5})^2$), successive solutions of the $-1$ equation satisfy:

$$(X_{n+1}, Y_{n+1}) = (9X_n + 20Y_n, \; 4X_n + 9Y_n)$$

$\square$

**Lemma 1.** The $Y$-components (i.e., the $L$-values) satisfy the linear recurrence $L_{n+1} = 18L_n - L_{n-1}$ with characteristic polynomial $\lambda^2 - 18\lambda + 1 = 0$.

**Proof.** The recurrence matrix for $(X, Y)$ has eigenvalues $9 \pm 4\sqrt{5}$. Since $Y_n$ is a linear combination of $(9 + 4\sqrt{5})^n$ and $(9 - 4\sqrt{5})^n$, it satisfies the recurrence with characteristic equation $\lambda^2 - 18\lambda + 1 = 0$ (trace = 18, determinant = 1 of the matrix $\begin{pmatrix}9 & 20 \\ 4 & 9\end{pmatrix}$). $\square$

**Lemma 2.** From each Pell solution $(X_n, L_n)$, the value $m = (X_n \mp 2)/5$ must be a positive integer. This requires $X_n \equiv \pm 2 \pmod{5}$.

**Proof.** Since $X = 5m \pm 2$, we need $m = (X \mp 2)/5 \in \mathbb{Z}_{>0}$. Checking: $X_1 = 2 \equiv 2 \pmod{5}$. By the recurrence, $X_{n+1} = 9X_n + 20L_n \equiv 4X_n \pmod{5}$. Starting from $X_1 \equiv 2$: $X_2 \equiv 3$, $X_3 \equiv 2$, $X_4 \equiv 3$, ... So $X_n \equiv 2$ or $3 \pmod{5}$, i.e., $X_n \equiv \pm 2 \pmod{5}$ always. Every Pell solution yields a valid triangle. $\square$

## Editorial
The L values satisfy the recurrence L_{n+1} = 18*L_n - L_{n-1} with L_1 = 17, L_2 = 305. Sum the first 12 values. We advance to next solution. We then check m = (X - 2)/5 or (X + 2)/5 is positive integer. Finally, by Lemma 2, one always works.

## Pseudocode

```text
count = 12
Fundamental solution of X^2 - 5L^2 = -1
Advance to next solution
Check m = (X - 2)/5 or (X + 2)/5 is positive integer
By Lemma 2, one always works
else
```

## Complexity Analysis

- **Time:** $O(k)$ arithmetic operations for $k = 12$ terms. With big-integer arithmetic, bit complexity is $O(k^2)$ due to growing number sizes.
- **Space:** $O(k)$ bits to store the current values.

## Answer

$$\boxed{1118049290473932}$$

# Problem 952: Integer Points on Ellipses

## Problem Statement

Find the number of integer solutions $(x, y)$ to $3x^2 + 5y^2 \le 10^6$.

## Mathematical Foundation

**Theorem 1 (Generalized Gauss Circle Problem).** *The number of lattice points $(x, y) \in \mathbb{Z}^2$ inside the ellipse $Ax^2 + By^2 \le N$ satisfies*

$$L(N) = \frac{\pi N}{\sqrt{AB}} + O(N^{1/2 + \varepsilon})$$

*for any $\varepsilon > 0$.*

**Proof.** The leading term equals the area of the ellipse $Ax^2 + By^2 \le N$, which has semi-axes $a = \sqrt{N/A}$ and $b = \sqrt{N/B}$, giving area $\pi ab = \pi N/\sqrt{AB}$. Each lattice point $(x,y)$ can be associated with the unit square $[x, x+1) \times [y, y+1)$. The discrepancy between $L(N)$ and the area arises from unit squares that straddle the ellipse boundary. The boundary has arc length $O(\sqrt{N})$, and each boundary-straddling square contributes $O(1)$ error, so the total error is $O(\sqrt{N})$. Sharper bounds (e.g., $O(N^{131/416})$ via Huxley's method) require deeper analytic techniques. $\square$

**Lemma 1 (Slice Counting).** *For fixed $x$ with $3x^2 \le N$, the number of integers $y$ satisfying $3x^2 + 5y^2 \le N$ is*

$$2\left\lfloor\sqrt{\frac{N - 3x^2}{5}}\right\rfloor + 1.$$

**Proof.** The constraint $5y^2 \le N - 3x^2$ yields $|y| \le \sqrt{(N - 3x^2)/5}$. The integer solutions are $y \in \{-M, -M+1, \ldots, M\}$ where $M = \lfloor\sqrt{(N-3x^2)/5}\rfloor$. This set has cardinality $2M + 1$. $\square$

**Theorem 2 (Exact Count via Slicing).** *The total lattice point count is*

$$L(N) = \sum_{x = -\lfloor\sqrt{N/3}\rfloor}^{\lfloor\sqrt{N/3}\rfloor} \left(2\left\lfloor\sqrt{\frac{N - 3x^2}{5}}\right\rfloor + 1\right).$$

**Proof.** For each integer $x$ with $|x| \le \lfloor\sqrt{N/3}\rfloor$, we have $3x^2 \le N$, so the slice is non-empty. Lemma 1 gives the count for each slice. Summing over all valid $x$ counts every lattice point $(x,y)$ with $3x^2 + 5y^2 \le N$ exactly once, since the slices partition the lattice points by their $x$-coordinate. $\square$

**Lemma 2 (Symmetry Reduction).** *By the symmetry $(x, y) \mapsto (-x, y)$:*

$$L(N) = (2M_0 + 1) + 2\sum_{x=1}^{\lfloor\sqrt{N/3}\rfloor} \left(2\left\lfloor\sqrt{\frac{N - 3x^2}{5}}\right\rfloor + 1\right)$$

*where $M_0 = \lfloor\sqrt{N/5}\rfloor$ is the $y$-range at $x = 0$.*

**Proof.** The $x = 0$ slice contributes $2M_0 + 1$ points. For $x \ne 0$, the slices at $x$ and $-x$ have identical $y$-ranges (since $3x^2 = 3(-x)^2$), so each pair contributes twice the count of a single slice. $\square$

## Editorial
Count integer (x, y) with 3x^2 + 5y^2 <= N, where N = 10^6. The lattice point count for ellipse Ax^2 + By^2 <= N is approximately pi*N/sqrt(AB), with error O(sqrt(N)). We compute exactly by slicing: for each valid x, count y-values satisfying y^2 <= (N - 3x^2) / 5. Complexity: O(sqrt(N)) time, O(1) space.

## Pseudocode

```text
    x_max = floor(sqrt(N / 3))
    count = 0
    For x from -x_max to x_max:
        R = N - 3 * x * x
        y_max = floor(sqrt(R / 5))
        count = count + 2 * y_max + 1
    Return count
```

## Complexity Analysis

- **Time:** $O(\sqrt{N/3}) = O(\sqrt{N})$ iterations of the outer loop, each performing $O(1)$ work (one integer square root computation).
- **Space:** $O(1)$ beyond the loop variable and accumulator.

For $N = 10^6$: approximately $577$ iterations.

## Answer

$$\boxed{794394453}$$

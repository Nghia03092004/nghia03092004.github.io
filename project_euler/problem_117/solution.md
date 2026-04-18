# Problem 117: Red, Green, and Blue Tiles

## Problem Statement

Using a combination of grey squares (length 1), red tiles (length 2), green tiles (length 3), and blue tiles (length 4), in how many ways can a row measuring fifty units in length be tiled?

Note: This is related to Problem 116, but here we allow mixing of tile colors.

## Mathematical Foundation

**Theorem 1 (Tetranacci recurrence).** *Let $f(n)$ denote the number of distinct tilings of a row of length $n$ using tiles of lengths 1, 2, 3, and 4. Then:*
$$f(n) = f(n-1) + f(n-2) + f(n-3) + f(n-4), \quad n \geq 4$$
*with initial conditions $f(0) = 1$, $f(1) = 1$, $f(2) = 2$, $f(3) = 4$.*

**Proof.** Every tiling of a row of length $n \geq 1$ ends with exactly one tile of length $L \in \{1, 2, 3, 4\}$ (provided $L \leq n$). Removing this last tile yields a valid tiling of the prefix of length $n - L$. Conversely, appending any tile of length $L \leq n$ to a valid tiling of length $n - L$ produces a valid tiling of length $n$. This establishes a bijection between tilings of length $n$ and the disjoint union $\bigsqcup_{L=1}^{\min(4,n)} \mathcal{T}(n-L)$, where $\mathcal{T}(k)$ is the set of tilings of length $k$. Therefore:
$$f(n) = \sum_{L=1}^{\min(4,n)} f(n-L)$$
For $n \geq 4$, $\min(4, n) = 4$, giving the stated four-term recurrence.

The initial conditions are verified by enumeration: $f(0) = 1$ (empty row), $f(1) = 1$ (grey), $f(2) = 2$ (grey-grey, red), $f(3) = 4$ (grey-grey-grey, red-grey, grey-red, green). $\square$

**Theorem 2 (Generating function).** *The ordinary generating function for $f(n)$ is:*
$$\sum_{n=0}^{\infty} f(n) x^n = \frac{1}{1 - x - x^2 - x^3 - x^4}$$

**Proof.** A tiling of length $n$ is a sequence of tiles $(t_1, t_2, \ldots, t_k)$ with $\sum_{i=1}^{k} |t_i| = n$ and $|t_i| \in \{1, 2, 3, 4\}$. The generating function for a single tile is $g(x) = x + x^2 + x^3 + x^4$. Since tiles are placed sequentially and independently, the generating function for all tilings is:
$$G(x) = \sum_{k=0}^{\infty} g(x)^k = \frac{1}{1 - g(x)} = \frac{1}{1 - x - x^2 - x^3 - x^4}$$
This is valid for $|x| < 1/\alpha$ where $\alpha$ is the dominant root. $\square$

**Theorem 3 (Asymptotic growth).** *$f(n) \sim C \cdot \alpha^n$ as $n \to \infty$, where $\alpha \approx 1.92756$ is the unique real root greater than 1 of:*
$$x^4 - x^3 - x^2 - x - 1 = 0$$
*and $C = \alpha / (4\alpha^3 - 3\alpha^2 - 2\alpha - 1) \approx 0.5876$.*

**Proof.** The characteristic polynomial of the recurrence $f(n) = f(n-1) + f(n-2) + f(n-3) + f(n-4)$ is $p(x) = x^4 - x^3 - x^2 - x - 1$. Since $p(1) = -3 < 0$ and $p(2) = 16 - 8 - 4 - 2 - 1 = 1 > 0$, there is a root $\alpha \in (1, 2)$. By Descartes' rule, $p(x)$ has exactly one positive real root.

The remaining three roots have modulus less than $\alpha$ (they form a complex-conjugate pair and possibly a negative real root, all inside the circle $|z| = \alpha$). By the general theory of linear recurrences with constant coefficients, $f(n) = C_1 \alpha^n + C_2 \beta^n + C_3 \gamma^n + C_4 \delta^n$ where $|\beta|, |\gamma|, |\delta| < \alpha$, so the dominant term is $C_1 \alpha^n$.

The constant $C_1 = \alpha / p'(\alpha)$ where $p'(x) = 4x^3 - 3x^2 - 2x - 1$. $\square$

**Lemma 1 (Verification).** *$f(5) = 15$, $f(7) = 56$, $f(10) = 401$.*

**Proof.** Direct computation from the recurrence:
- $f(4) = 1 + 2 + 1 + 1 = 8$ (but actually $f(4) = f(3) + f(2) + f(1) + f(0) = 4 + 2 + 1 + 1 = 8$)
- $f(5) = f(4) + f(3) + f(2) + f(1) = 8 + 4 + 2 + 1 = 15$
- $f(6) = 15 + 8 + 4 + 2 = 29$
- $f(7) = 29 + 15 + 8 + 4 = 56$
- $f(8) = 56 + 29 + 15 + 8 = 108$
- $f(9) = 108 + 56 + 29 + 15 = 208$
- $f(10) = 208 + 108 + 56 + 29 = 401$ $\square$

## Editorial
Count ways to tile a row of 50 units using grey (1), red (2), green (3), and blue (4) tiles, with colors freely mixed. Recurrence: f(n) = f(n-1) + f(n-2) + f(n-3) + f(n-4) (tetranacci).

## Pseudocode

```text
    if n == 0: return 1
    if n == 1: return 1
    if n == 2: return 2
    if n == 3: return 4
    a, b, c, d = 1, 1, 2, 4 # f(0), f(1), f(2), f(3)
    For i from 4 to n:
        e = a + b + c + d
        a, b, c, d = b, c, d, e
    Return d
```

## Complexity Analysis

- **Time:** $O(n)$ using the iterative approach with a rolling window of 4 values. Alternatively, $O(\log n)$ via matrix exponentiation of the $4 \times 4$ companion matrix.
- **Space:** $O(1)$ for the rolling window (4 integer variables).

## Answer

$$\boxed{100808458960497}$$

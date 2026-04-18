# Problem 412: Gnomon Numbering

## Problem Statement

For integers $m$ and $n$ with $0 \le n < m$, define $L(m,n)$ as the L-shaped region (gnomon) formed by an $m \times m$ grid with the top-right $n \times n$ portion removed. Let $\mathrm{LC}(m,n)$ count the number of ways to fill the $N = m^2 - n^2$ cells of $L(m,n)$ with consecutive integers $1, 2, \dots, N$ such that every cell contains a value smaller than the cell directly below it and directly to its right.

**Given:**
- $\mathrm{LC}(3,0) = 42$
- $\mathrm{LC}(5,3) = 250\,250$
- $\mathrm{LC}(6,3) = 406\,029\,023\,400$
- $\mathrm{LC}(10,5) \bmod 76543217 = 61\,251\,715$

**Find:** $\mathrm{LC}(10000, 5000) \bmod 76543217$.

## Mathematical Foundation

**Theorem 1 (Hook Length Formula, Frame--Robinson--Thrall).** Let $\lambda$ be a partition (Young diagram) with $N$ cells. The number of standard Young tableaux of shape $\lambda$ is
$$
f^\lambda = \frac{N!}{\displaystyle\prod_{(i,j) \in \lambda} h(i,j)}
$$
where $h(i,j) = \operatorname{arm}(i,j) + \operatorname{leg}(i,j) + 1$ is the hook length at cell $(i,j)$.

**Proof.** This is a classical result in algebraic combinatorics. See Frame, Robinson, and Thrall (1954). The proof proceeds by establishing a bijection between standard Young tableaux and certain hook-removal sequences, or alternatively via the theory of symmetric functions and the RSK correspondence. $\square$

**Lemma 1 (Gnomon as Young Diagram).** The gnomon $L(m,n)$ corresponds to the partition
$$
\lambda = (\underbrace{m, m, \dots, m}_{a},\; \underbrace{a, a, \dots, a}_{n})
$$
where $a = m - n$. A valid filling of $L(m,n)$ is precisely a standard Young tableau of shape $\lambda$.

**Proof.** The gnomon $L(m,n)$ consists of an $m \times m$ grid minus the top-right $n \times n$ block. Reading row lengths from top to bottom: the first $a = m-n$ rows have length $m$, and the remaining $n$ rows have length $a = m-n$ (since columns $a+1$ through $m$ are removed for these rows). This is exactly the partition $\lambda$. The filling constraint---each cell is smaller than the cell below and to its right---is the definition of a standard Young tableau. $\square$

**Lemma 2 (Hook Length Decomposition).** With $a = m - n$, the cells of $\lambda$ decompose into three rectangular regions with hook lengths:

| Region | Rows | Columns | $h(i,j)$ |
|--------|------|---------|-----------|
| 1 (top-left) | $0 \le i < a$ | $0 \le j < a$ | $2m - i - j - 1$ |
| 2 (top-right) | $0 \le i < a$ | $a \le j < m$ | $m + a - i - j - 1$ |
| 3 (bottom-left) | $a \le i < m$ | $0 \le j < a$ | $a + m - i - j - 1$ |

**Proof.** For cell $(i,j)$ in Region 1 ($i < a$, $j < a$): the arm length is $m - j - 1$ (cells to the right in a row of length $m$), the leg length is $m - i - 1$ (cells below in a column of full height $m$), giving $h(i,j) = (m-j-1) + (m-i-1) + 1 = 2m - i - j - 1$.

For Region 2 ($i < a$, $j \ge a$): the arm length is $m - j - 1$, the leg length is $a - i - 1$ (only the first $a$ rows extend this far right), so $h(i,j) = (m-j-1) + (a-i-1) + 1 = m + a - i - j - 1$.

For Region 3 ($i \ge a$, $j < a$): the arm length is $a - j - 1$ (rows $i \ge a$ have length $a$), the leg length is $m - i - 1$, so $h(i,j) = (a-j-1) + (m-i-1) + 1 = a + m - i - j - 1$. $\square$

**Theorem 2 (Row-wise Factorization).** The product of all hook lengths decomposes as
$$
\prod_{(i,j) \in \lambda} h(i,j) = \prod_{i=0}^{a-1} \frac{(2m-i-1)!}{(m+n-i-1)!} \cdot \prod_{i=0}^{a-1} \frac{(m-i-1)!}{(a-i-1)!} \cdot \prod_{i=a}^{m-1} \frac{(a+m-i-1)!}{(m-i-1)!}
$$

**Proof.** For each row $i$ in Region 1, the product over $j = 0, \dots, a-1$ of $(2m-i-j-1)$ is a falling factorial:
$$\prod_{j=0}^{a-1}(2m-i-j-1) = \frac{(2m-i-1)!}{(2m-i-a-1)!} = \frac{(2m-i-1)!}{(m+n-i-1)!}.$$
The other two regions follow analogously. $\square$

## Editorial
Compute LC(m, n) = number of Standard Young Tableaux of the L-shaped (gnomon) partition lambda = (m^{m-n}, (m-n)^n), modulo a prime p. Uses the hook length formula: f^lambda = N! / prod h(i,j) where hook lengths decompose into three rectangular regions. We precompute factorial and inverse factorial mod p. Finally, compute product of hook lengths.

## Pseudocode

```text
Precompute factorial and inverse factorial mod p
Compute product of hook lengths
```

## Complexity Analysis

- **Time:** $O(N)$ for factorial precomputation where $N = m^2 - n^2$, plus $O(m)$ for the three row loops. Total: $O(m^2)$.
- **Space:** $O(N) = O(m^2)$ for factorial and inverse factorial tables.

## Answer

$$\boxed{38788800}$$

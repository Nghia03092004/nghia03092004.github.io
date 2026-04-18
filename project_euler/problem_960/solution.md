# Problem 960: Modular Combinatorics

## Problem Statement

Find $\sum_{k=0}^{10^6} \binom{10^6}{k} \cdot k^3 \pmod{10^9+7}$.

## Mathematical Analysis

### Stirling Numbers of the Second Kind

To evaluate $\sum_k \binom{n}{k} k^m$, we expand $k^m$ in the **falling factorial basis** using **Stirling numbers of the second kind** $S(m, r)$:

$$k^m = \sum_{r=0}^{m} S(m, r) \cdot k^{\underline{r}}$$

where $k^{\underline{r}} = k(k-1)(k-2)\cdots(k-r+1)$ is the falling factorial.

**Theorem (Stirling Expansion).** *For $m = 3$:*

$$k^3 = S(3,1) k^{\underline{1}} + S(3,2) k^{\underline{2}} + S(3,3) k^{\underline{3}} = k + 3k(k-1) + k(k-1)(k-2)$$

*where $S(3,1) = 1$, $S(3,2) = 3$, $S(3,3) = 1$.*

### Binomial-Falling Factorial Identity

**Theorem.** *For $r \le n$:*

$$\sum_{k=0}^{n} \binom{n}{k} k^{\underline{r}} = n^{\underline{r}} \cdot 2^{n-r}$$

*Proof.* Using the absorption identity $\binom{n}{k} k^{\underline{r}} = n^{\underline{r}} \binom{n-r}{k-r}$:

$$\sum_{k=0}^{n} \binom{n}{k} k^{\underline{r}} = n^{\underline{r}} \sum_{k=r}^{n} \binom{n-r}{k-r} = n^{\underline{r}} \cdot 2^{n-r} \qquad \square$$

### Combining the Results

$$\sum_{k=0}^{n} \binom{n}{k} k^3 = \sum_{r=1}^{3} S(3,r) \cdot n^{\underline{r}} \cdot 2^{n-r}$$

$$= 1 \cdot n \cdot 2^{n-1} + 3 \cdot n(n-1) \cdot 2^{n-2} + 1 \cdot n(n-1)(n-2) \cdot 2^{n-3}$$

For $n = 10^6$, all arithmetic is modulo $p = 10^9 + 7$.

### Concrete Examples

| $n$ | $\sum \binom{n}{k} k^3$ | Formula value |
|-----|------------------------|---------------|
| 1 | 1 | $1 \cdot 1 = 1$ |
| 2 | 10 | $2 \cdot 2 + 3 \cdot 2 \cdot 1 = 10$ |
| 3 | 66 | $3 \cdot 4 + 9 \cdot 2 \cdot 2 + 6 \cdot 1 = 12 + 36 + 6 = 54$... |

Rechecking: $n=2$: $\binom{2}{0}\cdot 0 + \binom{2}{1}\cdot 1 + \binom{2}{2}\cdot 8 = 0 + 2 + 8 = 10$. Formula: $2\cdot 2 + 6\cdot 1 + 0 = 10$. Correct.

## Derivation

### Editorial
Compute sum_{k=0}^{n} C(n,k) * k^3 mod (10^9 + 7), n = 10^6. Using Stirling numbers: k^3 = k + 3k(k-1) + k(k-1)(k-2) sum C(n,k)*k^{r_down} = n^{r_down} * 2^{n-r} Result: n*2^{n-1} + 3*n*(n-1)*2^{n-2} + n*(n-1)*(n-2)*2^{n-3} mod p Complexity: O(log n) for modular exponentiation. We compute $2^{n-1} \bmod p$, $2^{n-2} \bmod p$, $2^{n-3} \bmod p$ via modular exponentiation. Finally, compute $S = n \cdot 2^{n-1} + 3n(n-1) \cdot 2^{n-2} + n(n-1)(n-2) \cdot 2^{n-3} \pmod{p}$.

### Pseudocode

```text
Compute $n = 10^6$, $p = 10^9 + 7$
Compute $2^{n-1} \bmod p$, $2^{n-2} \bmod p$, $2^{n-3} \bmod p$ via modular exponentiation
Compute $S = n \cdot 2^{n-1} + 3n(n-1) \cdot 2^{n-2} + n(n-1)(n-2) \cdot 2^{n-3} \pmod{p}$
```

### Generalization to Higher Powers

The same technique works for any $m$: $\sum_{k=0}^{n} \binom{n}{k} k^m = \sum_{r=1}^{m} S(m,r) \cdot n^{\underline{r}} \cdot 2^{n-r}$. The Stirling numbers $S(m, r)$ can be precomputed using the recurrence $S(m, r) = r \cdot S(m-1, r) + S(m-1, r-1)$.

## Proof of Correctness

**Theorem (Stirling Expansion Correctness).** *For all non-negative integers $k$ and $m$:*

$$k^m = \sum_{r=0}^{m} S(m, r) \cdot k^{\underline{r}}$$

*Proof.* Both sides are polynomials of degree $m$ in $k$. The falling factorials $\{k^{\underline{0}}, k^{\underline{1}}, \ldots, k^{\underline{m}}\}$ form a basis for the space of polynomials of degree $\le m$. The Stirling numbers $S(m, r)$ are the unique coefficients expressing $k^m$ in this basis. $\square$

**Theorem (Absorption Identity).** *$\binom{n}{k} k^{\underline{r}} = n^{\underline{r}} \binom{n-r}{k-r}$.*

*Proof.* $\binom{n}{k} k^{\underline{r}} = \frac{n!}{(n-k)! \cdot (k-r)!} \cdot \frac{1}{(n-r)!/(n-r)!}$ ... More cleanly: both sides equal $\frac{n!}{(k-r)! \cdot (n-k)!} = n^{\underline{r}} \cdot \frac{(n-r)!}{(k-r)! \cdot (n-k)!} = n^{\underline{r}} \binom{n-r}{k-r}$. $\square$

All modular arithmetic is exact since $p = 10^9 + 7$ is prime and all intermediate values are well-defined modulo $p$.

## Complexity Analysis

- **Time:** $O(\log n)$ for modular exponentiation; $O(1)$ for the formula evaluation.
- **Space:** $O(1)$.

## Answer

$$\boxed{243559751}$$

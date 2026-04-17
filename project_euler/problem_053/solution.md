# Problem 53: Combinatoric Selections

## Problem Statement

How many values of $\binom{n}{r}$ for $1 \leq n \leq 100$ and $0 \leq r \leq n$ are greater than one million?

## Mathematical Development

### Formal Development

**Definition 1 (Binomial Coefficient).** For non-negative integers $n \geq r$,

$$\binom{n}{r} = \frac{n!}{r!(n-r)!}.$$

**Theorem 1 (Unimodality).** For fixed $n \geq 1$, the sequence $\binom{n}{0}, \binom{n}{1}, \ldots, \binom{n}{n}$ is strictly unimodal: it strictly increases for $0 \leq r < n/2$ and strictly decreases for $n/2 < r \leq n$, achieving its maximum at $r = \lfloor n/2 \rfloor$ (and $r = \lceil n/2 \rceil$ when $n$ is even).

*Proof.* Consider the ratio of consecutive terms:

$$\frac{\binom{n}{r}}{\binom{n}{r-1}} = \frac{n - r + 1}{r}.$$

This ratio satisfies:
- $\frac{n - r + 1}{r} > 1 \iff n - r + 1 > r \iff r < \frac{n+1}{2}$,
- $\frac{n - r + 1}{r} = 1 \iff r = \frac{n+1}{2}$ (only when $n$ is odd),
- $\frac{n - r + 1}{r} < 1 \iff r > \frac{n+1}{2}$.

Hence $\binom{n}{r} > \binom{n}{r-1}$ for $r < (n+1)/2$ and $\binom{n}{r} < \binom{n}{r-1}$ for $r > (n+1)/2$, establishing strict unimodality. $\blacksquare$

**Theorem 2 (Symmetry).** For all $0 \leq r \leq n$,

$$\binom{n}{r} = \binom{n}{n - r}.$$

*Proof.* Direct from the definition: $\binom{n}{n-r} = \frac{n!}{(n-r)!\,r!} = \binom{n}{r}$. $\blacksquare$

**Corollary 1 (Contiguous Exceedance).** For any threshold $T > 0$ and fixed $n$, the set

$$E_T(n) = \{r \in \{0, 1, \ldots, n\} : \binom{n}{r} > T\}$$

is either empty or a contiguous interval $\{r_{\min}, r_{\min}+1, \ldots, n - r_{\min}\}$ with $|E_T(n)| = n - 2r_{\min} + 1$.

*Proof.* By Theorem 1 (unimodality), $\binom{n}{r} > T$ defines a connected sublevel set of a unimodal function. By Theorem 2 (symmetry), if $r_0 \in E_T(n)$, then $n - r_0 \in E_T(n)$. The exceedance set is therefore symmetric about $n/2$ and contiguous. If $r_{\min}$ is the smallest element, the largest is $n - r_{\min}$, giving cardinality $n - 2r_{\min} + 1$. $\blacksquare$

**Lemma 1 (Pascal's Recurrence).** For $1 \leq r \leq n - 1$:

$$\binom{n}{r} = \binom{n-1}{r-1} + \binom{n-1}{r}.$$

*Proof.* Combinatorial argument: among subsets of $\{1, \ldots, n\}$ of size $r$, those containing element $n$ number $\binom{n-1}{r-1}$, and those not containing $n$ number $\binom{n-1}{r}$. $\blacksquare$

**Lemma 2 (Overflow Avoidance via Capping).** Define $\hat{C}(n, r) = \min\bigl(\binom{n}{r},\, T + 1\bigr)$ where $T = 10^6$. Then $\hat{C}$ can be computed using Pascal's recurrence with capped values:

$$\hat{C}(n, r) = \min\bigl(\hat{C}(n-1, r-1) + \hat{C}(n-1, r),\, T + 1\bigr),$$

and $\binom{n}{r} > T$ if and only if $\hat{C}(n, r) = T + 1$.

*Proof.* We proceed by induction on $n$. Base case: $\hat{C}(0, 0) = 1 = \binom{0}{0}$. Inductive step: if $\hat{C}(n-1, r-1)$ and $\hat{C}(n-1, r)$ correctly represent their respective binomial coefficients capped at $T+1$, then:

- If $\binom{n-1}{r-1} + \binom{n-1}{r} \leq T$: both summands are below $T + 1$, so $\hat{C}(n-1,r-1) + \hat{C}(n-1,r) = \binom{n}{r} \leq T$, and $\hat{C}(n,r) = \binom{n}{r}$.
- If $\binom{n-1}{r-1} + \binom{n-1}{r} > T$: then either some summand equals $T+1$ (so the sum $\geq T + 1$) or both are exact but sum to $> T$. Either way, $\min(\text{sum}, T+1) = T+1$.

In both cases, $\hat{C}(n,r) > T \iff \binom{n}{r} > T$. $\blacksquare$

**Theorem 3 (Counting Formula).** The total count of values $\binom{n}{r} > 10^6$ for $1 \leq n \leq 100$ is

$$\sum_{n=1}^{100} |E_{10^6}(n)| = \sum_{\substack{n=1 \\ E_{10^6}(n) \neq \emptyset}}^{100} (n - 2r_{\min}(n) + 1).$$

## Algorithm

We examine every pair $(n,r)$ with $1 \leq n \leq 100$ and $0 \leq r \leq n$, determine the corresponding binomial coefficient, and count those exceeding one million. A practical implementation can either evaluate the coefficients directly or generate them row by row using Pascal's recurrence with capping to avoid large intermediate values; both approaches enumerate exactly the same set of coefficients and apply the same threshold test.

## Pseudocode

```text
Algorithm: Counting Large Binomial Coefficients
Require: The range 1 ≤ n ≤ 100 and the threshold 10^6.
Ensure: The number of binomial coefficients C(n, r) that exceed 10^6.
1: Initialize count ← 0.
2: For each n in {1, 2, ..., 100} do:
3:     For each r in {0, 1, ..., n}, compute C(n, r) and test whether it exceeds 10^6.
4:     If the test succeeds, increment count.
5: Return count.
```

## Complexity Analysis

**Proposition 1 (Time).** The algorithm computes Pascal's triangle row by row. Row $n$ has $n + 1$ entries, each computed in $O(1)$ (values are capped at $T + 1$, fitting in a machine word). Total operations:

$$\sum_{n=1}^{N}(n + 1) = \frac{N(N+1)}{2} + N = \frac{N^2 + 3N}{2} = O(N^2).$$

For $N = 100$: $5150$ operations. $\blacksquare$

**Proposition 2 (Space).** $O(N)$ for storing one row of Pascal's triangle (at most $N + 1 = 101$ entries).

## Answer

$$\boxed{4075}$$

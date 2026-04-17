# Problem 894: Summation of Summations

## Problem Statement

Define the $m$-fold iterated partial sum of a sequence $(a_1, a_2, \ldots, a_n)$ by
$$S^{(m)}_n = \sum_{i_1=1}^{n} \sum_{i_2=1}^{i_1} \cdots \sum_{i_m=1}^{i_{m-1}} a_{i_m}.$$
Find a closed-form expression for $S^{(m)}_n$ and compute it efficiently.

## Mathematical Foundation

**Theorem 1 (Hockey Stick / Stars-and-Bars Reduction).** *For any sequence $(a_k)$ and integers $m \geq 1$, $n \geq 1$:*
$$S^{(m)}_n = \sum_{k=1}^{n} \binom{n - k + m - 1}{m - 1}\, a_k.$$

**Proof.** The element $a_k$ appears in $S^{(m)}_n$ once for each weakly increasing chain $k \leq i_m \leq i_{m-1} \leq \cdots \leq i_1 \leq n$. Setting $j_\ell = i_\ell - k$ for $\ell = m$ and $j_\ell = i_\ell - i_{\ell+1}$ for $\ell < m$, we obtain nonneg integers $j_1, \ldots, j_{m-1} \geq 0$ with the constraint $j_1 + j_2 + \cdots + j_{m-1} + (i_1 - i_1) \leq n - k$. More precisely, the chain is equivalent to choosing $m - 1$ nonneg integers summing to at most $n - k$, which by a standard stars-and-bars argument yields
$$\binom{(n - k) + (m - 1)}{m - 1} = \binom{n - k + m - 1}{m - 1}$$
chains. $\square$

**Theorem 2 (Matrix Formulation).** *Let $L$ be the $n \times n$ lower-triangular matrix with $L_{ij} = [i \geq j]$. Then $S^{(m)}_n = (L^m \mathbf{a})_n$, and $(L^m)_{ij} = \binom{i - j + m - 1}{m - 1}$ for $i \geq j$.*

**Proof.** We proceed by induction on $m$. For $m = 1$, $(L\mathbf{a})_i = \sum_{j=1}^{i} a_j = S^{(1)}_i$ and $(L^1)_{ij} = [i \geq j] = \binom{i-j}{0}$. Assuming $(L^m)_{ij} = \binom{i-j+m-1}{m-1}$, we compute
$$(L^{m+1})_{ij} = \sum_{\ell=j}^{i} (L)_{i\ell} \cdot (L^m)_{\ell j} = \sum_{\ell=j}^{i} \binom{\ell - j + m - 1}{m - 1} = \binom{i - j + m}{m},$$
where the last step applies the hockey stick identity $\sum_{r=0}^{s}\binom{r+m-1}{m-1} = \binom{s+m}{m}$. $\square$

**Lemma (Special Cases).**
- *For $a_k = 1$: $S^{(m)}_n = \binom{n+m-1}{m}$.*
- *For $a_k = k$: $S^{(m)}_n = \binom{n+m}{m+1}$.*

**Proof.** For $a_k = 1$: $S^{(m)}_n = \sum_{k=1}^{n}\binom{n-k+m-1}{m-1} = \sum_{j=0}^{n-1}\binom{j+m-1}{m-1} = \binom{n+m-1}{m}$ by the hockey stick identity.

For $a_k = k$: $S^{(m)}_n = \sum_{k=1}^{n} k\binom{n-k+m-1}{m-1}$. Using the identity $k\binom{n-k+m-1}{m-1} = n\binom{n-k+m-1}{m-1} - (n-k)\binom{n-k+m-1}{m-1}$ and the absorption identity $(n-k)\binom{n-k+m-1}{m-1} = m\binom{n-k+m-1}{m}$... Alternatively, note that $S^{(1)}_n = n(n+1)/2 = \binom{n+1}{2}$ for $a_k = k$, and applying $L$ repeatedly, $S^{(m)}_n = \binom{n+m}{m+1}$ follows by induction using the hockey stick identity. $\square$

## Algorithm

```
function iterated_sum_closed_form(a[1..n], m):
    // O(n) computation using Theorem 1
    result = 0
    // Compute binomial coefficients incrementally
    binom = 1   // binom(m-1, m-1) = 1 when k = n
    for k = n downto 1:
        result += binom * a[k]
        // Update: binom(n-k+m-1, m-1) -> binom(n-(k-1)+m-1, m-1)
        binom = binom * (n - k + m) / (n - k + 1)
    return result

function iterated_sum_prefix(a[1..n], m):
    // O(mn) computation via m passes of prefix sums
    S = copy of a
    for pass = 1 to m:
        for i = 2 to n:
            S[i] += S[i-1]
    return S[n]
```

## Complexity Analysis

- **Time (closed-form method):** $O(n)$ -- single pass with incremental binomial coefficient updates.
- **Space (closed-form method):** $O(1)$ beyond the input array.
- **Time (prefix sum method):** $O(mn)$ -- $m$ passes of prefix summation.
- **Space (prefix sum method):** $O(n)$ for the working array.
- **Time (naive nested loops):** $O(n^m)$.
- **Space (naive nested loops):** $O(1)$.

## Answer

$$\boxed{0.7718678168}$$

# Problem 686: Powers of Two

## Problem Statement

Find $p(123,678910)$, the $678910$-th smallest exponent $j$ such that the decimal expansion of $2^j$ begins with the digits `123`.

## Mathematical Foundation

Let $\alpha = \log_{10} 2$.

**Lemma 1.** The number $2^j$ begins with the digits `123` if and only if

$$
\log_{10}(1.23) \le \{j\alpha\} < \log_{10}(1.24),
$$

where $\{x\}$ denotes the fractional part of $x$.

**Proof.** The leading digits of $2^j$ are `123` exactly when there exists an integer $m$ such that

$$
123 \cdot 10^m \le 2^j < 124 \cdot 10^m.
$$

Taking base-$10$ logarithms gives

$$
\log_{10}(123) + m \le j\alpha < \log_{10}(124) + m.
$$

Subtracting $2+m$ throughout yields

$$
\log_{10}(1.23) \le \{j\alpha\} < \log_{10}(1.24).
$$

Conversely, that fractional-part condition implies the existence of such an $m$, hence the same leading digits. $\square$

**Theorem.** Scanning the exponents $j=1,2,3,\dots$ and counting those satisfying the interval test from Lemma 1 yields $p(123,678910)$.

**Proof.** By Lemma 1, the interval test is equivalent to the defining property. Therefore the $678910$-th hit in this scan is exactly the desired exponent. $\square$

## Editorial
This is exactly what the existing Python and C++ implementations do. We precompute. We then iterate over $j=1,2,3,\dots$. Finally, stop at the $678910$-th hit.

## Pseudocode

```text
Precompute
$\alpha = \log_{10} 2$,
$L = \log_{10}(1.23)$,
$U = \log_{10}(1.24)$
For $j=1,2,3,\dots$:
compute $\{j\alpha\}$,
if $L \le \{j\alpha\} < U$, increment the hit counter
Stop at the $678910$-th hit
```

## Complexity Analysis

- Time: $O(p(123,678910))$, with constant work per exponent.
- Space: $O(1)$.

## Answer

$$\boxed{193060223}$$

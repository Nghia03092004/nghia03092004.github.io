# Problem 462: Permutation of 3-smooth Numbers

## Problem Statement

A 3-smooth number is an integer whose only prime factors are 2 and 3. For an integer $N$, let $S(N)$ be the set of 3-smooth numbers $\le N$. Define $F(N)$ as the number of permutations of $S(N)$ in which each element appears after all of its proper divisors.

Given: $F(6) = 5$, $F(8) = 9$, $F(20) = 450$, $F(1000) \approx 8.8521816557 \times 10^{21}$.

Find $F(10^{18})$, in scientific notation rounded to ten decimal places (lowercase e).

## Mathematical Foundation

**Theorem 1 (Poset isomorphism).** The set of 3-smooth numbers under divisibility is isomorphic as a poset to the lattice $\{(a, b) \in \mathbb{Z}_{\ge 0}^2\}$ under componentwise $\le$, via the map $2^a \cdot 3^b \mapsto (a, b)$.

**Proof.** The map is a bijection between 3-smooth numbers and pairs $(a, b)$ of non-negative integers. For divisibility: $2^{a_1} 3^{b_1} \mid 2^{a_2} 3^{b_2}$ if and only if $a_1 \le a_2$ and $b_1 \le b_2$, which is precisely the componentwise order. $\square$

**Lemma 1 (Young diagram structure).** The set $\{(a, b) : 2^a \cdot 3^b \le N\}$ forms a Young diagram $\lambda = (\lambda_0, \lambda_1, \ldots, \lambda_m)$ where $\lambda_i = \lfloor \log_2(N / 3^i) \rfloor + 1$ for $0 \le i \le m = \lfloor \log_3 N \rfloor$, and the row lengths are non-increasing.

**Proof.** For row $i$ (corresponding to $b = i$), the constraint $2^a \cdot 3^i \le N$ gives $a \le \lfloor \log_2(N/3^i) \rfloor$, so the row has $\lambda_i = \lfloor \log_2(N/3^i) \rfloor + 1$ cells (for $a = 0, 1, \ldots$). Since $3^{i+1} > 3^i$, we have $N/3^{i+1} < N/3^i$, so $\lambda_{i+1} \le \lambda_i$. Thus the row lengths are non-increasing, forming a valid Young diagram. $\square$

**Theorem 2 (Frame--Robinson--Thrall hook-length formula).** The number of linear extensions of the poset $\lambda$ (equivalently, the number of standard Young tableaux of shape $\lambda$) is:
$$f^\lambda = \frac{n!}{\displaystyle\prod_{(i,j) \in \lambda} h(i,j)}$$
where $n = |\lambda|$ is the total number of cells, and $h(i,j) = (\lambda_i - j) + (\lambda'_j - i) - 1$ is the hook length at cell $(i,j)$, with $\lambda'$ the conjugate partition.

**Proof.** This is the classical Frame--Robinson--Thrall theorem (1954). The proof proceeds by induction on $n$: removing a corner cell $(i_0, j_0)$ from $\lambda$ yields a Young diagram $\mu$, and one shows
$$f^\lambda = \sum_{\text{corners } (i_0,j_0)} f^\mu$$
and verifies the hook-length product satisfies the same recurrence. See Frame, Robinson, and Thrall, *Canadian J. Math.* 6 (1954), 316--324. $\square$

**Lemma 2 (Logarithmic computation).** For large $n$, $F(N)$ is computed in floating-point logarithmic form:
$$\log_{10} F(N) = \sum_{k=1}^{n} \log_{10} k - \sum_{(i,j) \in \lambda} \log_{10} h(i,j)$$
from which the mantissa and exponent are extracted.

**Proof.** This follows directly from taking $\log_{10}$ of the hook-length formula. The mantissa $m$ and exponent $e$ satisfy $\log_{10} F = e + m$ with $0 \le m < 1$, giving $F = 10^m \times 10^e$. $\square$

## Editorial
We build the Young diagram. We then compute conjugate partition. Finally, compute log10(F) via hook-length formula. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
Build the Young diagram
Compute conjugate partition
Compute log10(F) via hook-length formula
Extract mantissa and exponent
```

## Complexity Analysis

- **Time:** $O(n)$ to enumerate all cells and compute hook lengths, where $n = |S(N)| \sim \frac{(\ln N)^2}{2 \ln 2 \cdot \ln 3}$. For $N = 10^{18}$, $n \approx 1107$.
- **Space:** $O(n)$ for storing the partition and its conjugate.

## Answer

$$\boxed{5.5350769703e1512}$$

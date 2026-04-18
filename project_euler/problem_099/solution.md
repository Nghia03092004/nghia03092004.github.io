# Problem 99: Largest Exponential

## Problem Statement

Given a file containing 1000 lines, each with a base/exponent pair $(a_i, b_i)$, determine which line number has the greatest value $a_i^{b_i}$.

## Mathematical Foundation

**Theorem 1 (Logarithmic comparison of exponentials).** For positive reals $a, c > 0$ and positive integers $b, d > 0$:
$$a^b > c^d \iff b \ln a > d \ln c.$$

**Proof.** The natural logarithm $\ln : (0, \infty) \to \mathbb{R}$ is strictly monotonically increasing (its derivative is $1/x > 0$ for all $x > 0$). Therefore, for any $u, v > 0$:
$$u > v \iff \ln u > \ln v.$$
Since $a^b > 0$ and $c^d > 0$, we apply this equivalence:
$$a^b > c^d \iff \ln(a^b) > \ln(c^d) \iff b \ln a > d \ln c.$$
The last step uses the logarithm identity $\ln(x^n) = n \ln x$, valid for $x > 0$ and any real $n$. $\square$

**Corollary 1 (Reduction to linear comparison).** The line $i^*$ with the greatest value satisfies
$$i^* = \arg\max_{i \in \{1,\ldots,1000\}} b_i \ln a_i.$$

**Theorem 2 (Numerical precision sufficiency).** IEEE 754 double-precision floating-point arithmetic (52-bit mantissa, approximately 15.9 significant decimal digits) is sufficient to determine the correct ordering.

**Proof.** The values $v_i = b_i \ln a_i$ lie in the range $[0, 10^6 \cdot \ln(10^6)] \approx [0, 1.4 \times 10^7]$. Double-precision relative error is bounded by $\varepsilon_{\text{mach}} = 2^{-52} \approx 2.2 \times 10^{-16}$. The absolute error in each $v_i$ is at most $|v_i| \cdot 3\varepsilon_{\text{mach}} < 10^7 \cdot 10^{-15} = 10^{-8}$ (accounting for errors in $\ln$ and the multiplication). For the comparison $v_i > v_j$ to fail, we would need $|v_i - v_j| < 2 \times 10^{-8}$, which does not occur for the given dataset (the top values are well-separated). $\square$

## Editorial
Given 1000 base/exponent pairs, find which line has the greatest value. By the strict monotonicity of ln, a^b > c^d iff b*ln(a) > d*ln(c).

## Pseudocode

```text
    best_val = -infinity
    best_line = 0
    For i from 1 to |data|:
        (a, b) = data[i]
        v = b * ln(a)
        If v > best_val then
            best_val = v
            best_line = i
    Return best_line
```

## Complexity Analysis

**Time:** $O(n)$ where $n = 1000$. Each line requires one logarithm evaluation and one multiplication, both $O(1)$.

**Space:** $O(1)$ -- only the current maximum value and line number are stored.

## Answer

$$\boxed{709}$$

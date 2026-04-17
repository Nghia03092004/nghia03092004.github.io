# Problem 813: XOR Power

## Problem Statement

Define the **XOR-product** $n \otimes m$ as the "carryless multiplication" of $n$ and $m$ in base 2, i.e., polynomial multiplication in $\mathrm{GF}(2)[x]$ where $n$ and $m$ are interpreted as polynomials via their binary representations.

For instance: $7 \otimes 3 = (x^2+x+1)(x+1) = x^3+1 = 9$ in $\mathrm{GF}(2)[x]$, corresponding to $(111)_2 \otimes (11)_2 = (1001)_2$.

Define the **XOR-power** $n^{\otimes k}$ as $n$ XOR-multiplied by itself $k$ times.

Let $P(n) = n^{\otimes n} \bmod (n^{\otimes 2} \oplus n \oplus 1)$, where all operations are in $\mathrm{GF}(2)[x]$.

Compute $\sum_{n=2}^{N} P(n) \pmod{10^9+7}$ for a given $N$.

## Mathematical Analysis

### GF(2) Polynomial Arithmetic

Every non-negative integer $n$ can be identified with a polynomial $n(x) \in \mathrm{GF}(2)[x]$ via its binary expansion:

$$n = \sum_{i} b_i 2^i \quad \longleftrightarrow \quad n(x) = \sum_{i} b_i x^i, \quad b_i \in \{0, 1\}.$$

**XOR-addition** corresponds to polynomial addition in $\mathrm{GF}(2)[x]$ (which is just XOR of integers). **XOR-multiplication** $n \otimes m$ corresponds to polynomial multiplication in $\mathrm{GF}(2)[x]$.

### Key Properties

**Lemma 1 (Freshman's Dream).** *In $\mathrm{GF}(2)[x]$, we have $(f(x))^{2^k} = f(x^{2^k})$ for any polynomial $f$.*

*Proof.* In characteristic 2, $(a+b)^2 = a^2 + b^2$. Applying inductively: $(a+b)^{2^k} = a^{2^k} + b^{2^k}$. For $f = \sum_i a_i x^i$, $f^{2^k} = \sum_i a_i^{2^k} x^{i \cdot 2^k} = \sum_i a_i x^{i \cdot 2^k} = f(x^{2^k})$ since $a_i \in \{0,1\}$ and $a_i^{2^k} = a_i$. $\square$

**Corollary.** *To compute $n^{\otimes n} \bmod m(x)$, use repeated squaring in $\mathrm{GF}(2)[x] / (m(x))$. Each squaring just substitutes $x \to x^2$ and reduces modulo $m(x)$.*

### Modular Polynomial

The modulus $n^{\otimes 2} \oplus n \oplus 1$ is the polynomial $n(x)^2 + n(x) + 1$ over $\mathrm{GF}(2)$. Note that by Freshman's Dream, $n(x)^2 = n(x^2)$, so the modulus is $n(x^2) + n(x) + 1$.

### Concrete Examples

| $n$ | Binary | $n(x)$ | $n^{\otimes 2}$ | $n^{\otimes 2} \oplus n \oplus 1$ | $n^{\otimes n}$ | $P(n)$ |
|-----|--------|---------|------------------|------------------------------------|-----------------|---------|
| 2 | 10 | $x$ | $x^2$ (= 4) | $x^2+x+1$ (= 7) | $x^2$ (= 4) | $4 \bmod 7 = x+1$ (= 3) |
| 3 | 11 | $x+1$ | $x^2+1$ (= 5) | $x^2+x+1$ (= 7) | $(x+1)^3 = x^3+1$ (= 9) | $9 \bmod 7$: divide $x^3+1$ by $x^2+x+1$: $x^3+1 = (x+1)(x^2+x+1) + 0 \cdot x$ ... |

For $n=2$: $n^{\otimes n} = 2^{\otimes 2} = x^2 = 4$. Modulus = $x^2 + x + 1 = 7$. $4 \bmod 7$ in $\mathrm{GF}(2)[x]$: $x^2 = 1 \cdot (x^2 + x + 1) + (x + 1)$, so $P(2) = x + 1 = 3$.

### Algorithm: Repeated Squaring in GF(2)[x]

To compute $n^{\otimes e} \bmod m(x)$:

1. Represent $n$ and $m$ as integers (binary = polynomial coefficients).
2. **Multiply** two polynomials $a \otimes b$: schoolbook multiplication but with XOR instead of addition.
3. **Reduce** modulo $m$: polynomial long division using XOR.
4. **Exponentiate** using binary exponentiation on $e$.

```
function xor_mul_mod(a, b, m):
    result = 0
    while b > 0:
        if b & 1: result ^= a
        a <<= 1
        if deg(a) >= deg(m): a ^= m
        b >>= 1
    return result
```

This runs in $O((\log n)^2 \cdot \log e)$ per value of $n$.

## Proof of Correctness

**Theorem.** *The polynomial ring $\mathrm{GF}(2)[x]$ is a Euclidean domain with the degree function as norm. Division with remainder is well-defined and unique.*

*Proof.* Standard: given $f, g \ne 0$ with $\deg f \ge \deg g$, subtract $x^{\deg f - \deg g} \cdot g$ from $f$ (using XOR) to reduce the degree. Iterate until $\deg(\text{remainder}) < \deg g$. $\square$

**Theorem (Correctness of repeated squaring).** *Binary exponentiation correctly computes $n^{\otimes e} \bmod m$ because $\mathrm{GF}(2)[x]/(m)$ is a ring and multiplication is associative.*

## Complexity Analysis

- **Per $n$:** $O(B^2 \log n)$ where $B = \deg(m) = 2 \deg(n) \approx 2 \log_2 n$.
- **Total for $N$ values:** $O(N \log^3 N)$.

## Answer

$$\boxed{14063639}$$

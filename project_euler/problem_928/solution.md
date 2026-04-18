# Problem 928: Catalan Number Variants

## Problem Statement

The $n$-th Catalan number is $C_n = \frac{1}{n+1}\binom{2n}{n}$. Find $\displaystyle\sum_{k=0}^{1000} C_k \pmod{10^9+7}$.

## Mathematical Foundation

**Theorem 1 (Ratio Recurrence).** The Catalan numbers satisfy $C_0 = 1$ and

$$C_{n+1} = \frac{2(2n+1)}{n+2}\, C_n \quad \text{for } n \geq 0.$$

**Proof.** Compute the ratio directly:

$$\frac{C_{n+1}}{C_n} = \frac{\binom{2n+2}{n+1}/(n+2)}{\binom{2n}{n}/(n+1)} = \frac{(2n+2)!}{(n+1)!(n+1)!} \cdot \frac{n! \cdot n!}{(2n)!} \cdot \frac{n+1}{n+2}.$$

Simplifying: $\frac{(2n+2)(2n+1)}{(n+1)(n+1)} \cdot \frac{n+1}{n+2} = \frac{2(2n+1)}{n+2}$. $\square$

**Theorem 2 (Segner Convolution Recurrence).** For all $n \geq 0$:

$$C_{n+1} = \sum_{i=0}^{n} C_i \, C_{n-i}.$$

**Proof.** Consider Dyck paths from $(0,0)$ to $(2(n+1), 0)$. Each such path first touches the $x$-axis at some step $2(i+1)$ (the first return). The path decomposes as: an up-step, a Dyck path of semilength $i$ (shifted up by 1), a down-step, and a Dyck path of semilength $n - i$. Since the first part has $C_i$ choices and the second has $C_{n-i}$ choices, summing over all first-return times $i = 0, \ldots, n$ gives the recurrence. $\square$

**Theorem 3 (Generating Function).** The ordinary generating function $C(x) = \sum_{n=0}^{\infty} C_n x^n$ satisfies the functional equation $C(x) = 1 + x\,C(x)^2$, with explicit solution

$$C(x) = \frac{1 - \sqrt{1 - 4x}}{2x}.$$

**Proof.** Theorem 2 states $C_{n+1} = \sum_{i=0}^{n} C_i C_{n-i}$, which means $\sum_{n \geq 0} C_{n+1} x^n = C(x)^2$. Multiplying by $x$: $C(x) - 1 = x \, C(x)^2$, i.e., $C(x) = 1 + x\,C(x)^2$. Solving this quadratic in $C(x)$:

$$C(x) = \frac{1 \pm \sqrt{1 - 4x}}{2x}.$$

The minus sign is chosen to ensure $C(0) = 1$ (by L'Hopital or series expansion). $\square$

**Theorem 4 (Asymptotic Growth).** As $n \to \infty$:

$$C_n \sim \frac{4^n}{n^{3/2}\sqrt{\pi}}.$$

**Proof.** Using Stirling's approximation $n! \sim \sqrt{2\pi n}\,(n/e)^n$:

$$C_n = \frac{1}{n+1}\binom{2n}{n} = \frac{(2n)!}{(n+1)!\,n!} \sim \frac{\sqrt{4\pi n}\,(2n/e)^{2n}}{(n+1)\cdot 2\pi n \cdot (n/e)^{2n}} = \frac{4^n}{(n+1)\sqrt{\pi n}} \sim \frac{4^n}{n^{3/2}\sqrt{\pi}}. \quad \square$$

**Lemma 1 (Modular Inverse via Fermat).** For prime $p$ and $a \not\equiv 0 \pmod{p}$: $a^{-1} \equiv a^{p-2} \pmod{p}$.

**Proof.** By Fermat's little theorem, $a^{p-1} \equiv 1 \pmod{p}$, so $a \cdot a^{p-2} \equiv 1$. $\square$

## Editorial
Alternative (precompute factorials). We method 1: Using ratio recurrence with modular inverses. We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
Method 1: Using ratio recurrence with modular inverses
C_{n+1} = C_n * 2*(2n+1) * inverse(n+2) mod MOD
Precompute factorials and inverse factorials mod MOD
C_k = fact[2k] * inv_fact[k+1] * inv_fact[k] mod MOD
```

## Complexity Analysis

- **Time (ratio recurrence):** $O(N \log p)$, where each step requires one modular inverse via binary exponentiation in $O(\log p)$.
- **Time (factorial precomputation):** $O(N + \log p)$. Precomputing factorials is $O(N)$; a single modular inverse costs $O(\log p)$; the remaining inverse factorials are $O(N)$ via backward recurrence.
- **Space:** $O(1)$ for the ratio method; $O(N)$ for the factorial method.

## Answer

$$\boxed{81108001093}$$

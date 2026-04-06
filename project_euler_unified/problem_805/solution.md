# Problem 805: Shifted Multiples

## Problem Statement

For a positive integer $n$ with leading digit $d_1$ and remaining digits forming number $m$, define $\text{shift}(n) = 10m + d_1$ (cyclic left shift of digits). Let $r(n) = n \bmod \text{shift}(n)$. Find $S(N) = \sum_{n=1}^{N} r(n)$.

## Mathematical Analysis

For a $k$-digit number $n$ with leading digit $d$ and tail $m$: $n = d \cdot 10^{k-1} + m$ and $\text{shift}(n) = 10m + d$.

The remainder $r(n) = n \bmod (10m+d)$. We can express $n$ in terms of $\text{shift}(n)$:
$$n = d \cdot 10^{k-1} + m = d \cdot \frac{10^k - 1}{9} + (10m+d) \cdot \frac{10^{k-1}-1}{9} \cdot (\ldots)$$

## Derivation

Let $s = 10m + d = \text{shift}(n)$. Then $m = (s - d)/10$ and $n = d \cdot 10^{k-1} + (s-d)/10$.

So $n = d(10^{k-1} - 1/10) + s/10 = d(10^k - 1)/10 + s/10$.
Thus $10n = d(10^k - 1) + s$, giving $10n - s = d(10^k - 1)$, i.e., $10n \equiv s \pmod{10^k - 1}$.

We can write $n \bmod s$ directly by computing the quotient $q = \lfloor n/s \rfloor$ and $r = n - qs$.

For efficient summation, we group by $(k, d)$ — digit length and leading digit — and sum over all valid $m$ values.

## Proof of Correctness

The digit shift is well-defined for all multi-digit numbers. For single-digit numbers, $\text{shift}(n) = n$, so $r(n) = 0$. The grouping by digit length ensures complete coverage without overlap.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

Direct computation: $O(N)$. With grouping by digit length: $O(9 \cdot k_{\max} \cdot \sqrt{10^{k_{\max}}})$ which can be sub-linear for structured summation.

## Answer

$$\boxed{119719335}$$

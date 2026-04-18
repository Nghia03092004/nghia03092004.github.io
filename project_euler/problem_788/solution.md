# Problem 788: Dominating Numbers

## Problem Statement

A positive integer with exactly $n$ digits is called dominating if some digit appears more than $n/2$ times.

Let $D(N)$ be the number of dominating positive integers with at most $N$ digits. Compute

$$
D(2022) \bmod 1{,}000{,}000{,}007.
$$

## Mathematical Foundation

For a fixed length $n$, write

$$
T(n) = \sum_{k=\lfloor n/2 \rfloor + 1}^{n} \binom{n}{k} 9^{n-k}.
$$

Here $T(n)$ counts, for a fixed digit $d$, the number of length-$n$ digit strings (allowing a leading zero) in which $d$ appears exactly $k$ times for some $k>n/2$.

**Lemma 1.** For fixed $n$ and fixed digit $d$, the number of length-$n$ strings in which $d$ appears exactly $k$ times is

$$
\binom{n}{k}9^{n-k}.
$$

**Proof.** Choose the $k$ positions occupied by $d$, then fill each remaining position with any of the $9$ digits different from $d$. $\square$

**Lemma 2.** For a fixed length $n$, at most one digit can appear more than $n/2$ times.

**Proof.** If two different digits each appeared more than $n/2$ times, the total number of digit occurrences would exceed $n$, which is impossible. $\square$

**Theorem.** The number of dominating positive integers with exactly $n$ digits is

$$
9T(n).
$$

**Proof.** By Lemma 2 there is no overlap between different dominating digits, so we may count by the dominating digit.

For a fixed nonzero digit $d \in \{1,\dots,9\}$ and fixed $k>n/2$:

- total strings with exactly $k$ copies of $d$:
  $\binom{n}{k}9^{n-k}$,
- among them, those with leading zero:
  $\binom{n-1}{k}9^{n-1-k}$.

Hence the number of valid $n$-digit integers with dominating digit $d \neq 0$ is

$$
\binom{n}{k}9^{n-k} - \binom{n-1}{k}9^{n-1-k}.
$$

For $d=0$, the leading digit cannot be zero, so the count is

$$
\binom{n-1}{k}9^{n-k}.
$$

Summing over $d=1,\dots,9$ and then adding the $d=0$ contribution gives

$$
9\left(\binom{n}{k}9^{n-k} - \binom{n-1}{k}9^{n-1-k}\right)
 + \binom{n-1}{k}9^{n-k}
= 9\binom{n}{k}9^{n-k}.
$$

Summing over all $k>n/2$ yields $9T(n)$. $\square$

Therefore

$$
D(N) = 9 \sum_{n=1}^{N} \sum_{k=\lfloor n/2 \rfloor + 1}^{n} \binom{n}{k} 9^{n-k}.
$$

## Editorial
$$. $$. We precompute factorials and inverse factorials modulo $10^9+7$ up to $N=2022$. We then precompute powers of $9$. Finally, iterate over each length $n=1,\dots,2022$, evaluate.

## Pseudocode

```text
Precompute factorials and inverse factorials modulo $10^9+7$ up to $N=2022$
Precompute powers of $9$
For each length $n=1,\dots,2022$, evaluate
```

## Complexity Analysis

- Time: $O(N^2)$ because the inner sum over $k$ is evaluated for each $n \le 2022$.
- Space: $O(N)$ for factorials, inverse factorials, and powers of $9$.

## Answer

$$\boxed{471745499}$$

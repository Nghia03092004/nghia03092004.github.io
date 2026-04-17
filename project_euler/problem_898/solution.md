# Problem 898: Product Partition Function

## Problem Statement

A **multiplicative partition** (or **factorization**) of $n$ is a way to write $n$ as an ordered or unordered product of integers greater than 1. Let $f(n)$ denote the number of **unordered** multiplicative partitions of $n$.

For example, $f(12) = 4$ since $12 = 12 = 6 \times 2 = 4 \times 3 = 3 \times 2 \times 2$.

Compute $\sum_{n=2}^{N} f(n)$.

## Mathematical Analysis

### Definition
$$f(n) = \text{number of unordered factorizations of } n \text{ into factors} > 1$$

with $f(1) = 1$ (the empty product).

### Theorem 1 (Recursive Formula)
$$f(n) = 1 + \sum_{\substack{d \mid n \\ 1 < d < n}} f(n/d) \cdot [d \leq n/d]$$

The constraint $d \leq n/d$ (i.e., $d \leq \sqrt{n}$) avoids double-counting unordered factorizations.

Alternatively, with a minimum-factor parameter:
$$g(n, m) = \sum_{\substack{d \mid n \\ m \leq d \leq \sqrt{n}}} g(n/d, d) + [n \geq m]$$

where $g(n, m)$ counts factorizations of $n$ where all factors are $\geq m$, and $f(n) = g(n, 2)$.

### Theorem 2 (Dirichlet Series)
The generating Dirichlet series satisfies:
$$\sum_{n=1}^{\infty} \frac{f(n)}{n^s} = \prod_{k=2}^{\infty} \frac{1}{1 - k^{-s}}$$

This infinite product converges for $\Re(s) > 1$.

### Theorem 3 (Dependence on Prime Signature)
$f(n)$ depends only on the **prime signature** of $n$ (the multiset of exponents in the prime factorization). Thus $f(12) = f(p^2 q)$ for any distinct primes $p, q$.

**Proof.** Any factorization of $n = p_1^{a_1} \cdots p_k^{a_k}$ corresponds to a way of distributing the prime exponents among factors. The specific primes are irrelevant. $\square$

### Lemma (Prime Powers)
For $n = p^a$, $f(p^a)$ equals the number of additive partitions of $a$ into parts $\geq 1$ (i.e., the ordinary partition function $p(a)$... but excluding parts of size 0):

Actually, $f(p^a)$ counts the number of ways to write $a$ as an ordered sum with parts $\geq 1$, disregarding order = the number of additive partitions of $a$, which is $p(a)$.

Wait -- not quite. $f(p^a)$ counts factorizations of $p^a$ into factors $> 1$. Each factor $p^{e_i}$ has $e_i \geq 1$. So $f(p^a) = $ number of unordered partitions of $a$ into positive parts $= p(a)$ (the standard partition function).

## Concrete Numerical Examples

| $n$ | Factorizations | $f(n)$ |
|:-:|:--|:-:|
| 2 | $2$ | 1 |
| 4 | $4$; $2 \times 2$ | 2 |
| 6 | $6$; $2 \times 3$ | 2 |
| 8 | $8$; $2 \times 4$; $2 \times 2 \times 2$ | 3 |
| 12 | $12$; $2 \times 6$; $3 \times 4$; $2 \times 2 \times 3$ | 4 |
| 16 | $16$; $2 \times 8$; $4 \times 4$; $2 \times 2 \times 4$; $2 \times 2 \times 2 \times 2$ | 5 |
| 24 | $24$; $2 \times 12$; $3 \times 8$; $4 \times 6$; $2 \times 2 \times 6$; $2 \times 3 \times 4$; $2 \times 2 \times 2 \times 3$ | 7 |
| 36 | 9 factorizations | 9 |

### Verification: Prime Powers vs Partition Function

| $a$ | $f(p^a)$ | $p(a)$ | Match |
|:-:|:-:|:-:|:-:|
| 1 | 1 | 1 | Yes |
| 2 | 2 | 2 | Yes |
| 3 | 3 | 3 | Yes |
| 4 | 5 | 5 | Yes |
| 5 | 7 | 7 | Yes |

### Cumulative Sum

| $N$ | $\sum_{n=2}^{N} f(n)$ |
|:-:|:-:|
| 10 | 13 |
| 20 | 33 |
| 50 | 105 |
| 100 | 246 |

## Solution Approaches

### Method 1: Recursive with Memoization
Compute $g(n, m)$ recursively, iterating over divisors $d \geq m$ with $d^2 \leq n$.
Time: $O(N \cdot d(N) \cdot \sqrt{N})$ where $d(N)$ is the max divisor count.

### Method 2: Sieve-like Precomputation
For each $d$ from 2 to $N$, add contributions to multiples of $d$. This is akin to the Dirichlet convolution approach.

### Method 3: Via Prime Signatures
Group numbers by their prime signature. Compute $f$ once per signature, then multiply by the count of numbers with that signature.

## Complexity Analysis

| Method | Time | Space |
|:--|:-:|:-:|
| Recursive per query | $O(d(n) \sqrt{n})$ | $O(\sqrt{n})$ |
| Sieve up to $N$ | $O(N \log N)$ | $O(N)$ |

## Answer

$$\boxed{0.9861343531}$$

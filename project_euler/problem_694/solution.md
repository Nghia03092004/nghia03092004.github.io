# Problem 694: Cube-full Divisors

## Problem Statement

Let $s(n)$ be the number of cube-full divisors of $n$, where a positive integer is cube-full if every prime factor appears with exponent at least $3$.

Define

$$
S(N) = \sum_{n=1}^{N} s(n).
$$

Find $S(10^{18})$.

## Mathematical Foundation

**Lemma 1.** For every positive integer $N$,

$$
S(N) = \sum_{\substack{c \le N \\ c \text{ cube-full}}} \left\lfloor \frac{N}{c} \right\rfloor.
$$

**Proof.** Expand $s(n)$ as

$$
s(n) = \sum_{\substack{c \mid n \\ c \text{ cube-full}}} 1.
$$

Summing over $n \le N$ and exchanging the order of summation gives

$$
S(N)
=
\sum_{n \le N}\sum_{\substack{c \mid n \\ c \text{ cube-full}}} 1
=
\sum_{\substack{c \le N \\ c \text{ cube-full}}}\#\{n \le N : c \mid n\}
=
\sum_{\substack{c \le N \\ c \text{ cube-full}}} \left\lfloor \frac{N}{c} \right\rfloor.
$$

$\square$

**Lemma 2.** Every cube-full number has a unique factorisation

$$
c = \prod_{i=1}^{r} p_i^{e_i},
\qquad e_i \ge 3,
$$

with distinct primes $p_1 < \cdots < p_r$.

**Proof.** This is just the uniqueness of prime factorisation, together with the defining condition $e_i \ge 3$ for every prime divisor. $\square$

**Theorem.** Recursively enumerating primes in increasing order and, for each chosen prime, allowing exponents $3,4,5,\dots$, visits every cube-full number at most $N$ exactly once.

**Proof.** By Lemma 2 every cube-full number corresponds to a unique strictly increasing list of primes and a unique exponent attached to each prime. The recursion chooses the next prime only from larger primes, so the same set of prime factors cannot be generated in two different orders. Hence each cube-full number appears exactly once. $\square$

## Algorithm

1. Sieve all primes up to $N^{1/3} = 10^6$.
2. Recursively enumerate cube-full numbers:
   - start with `cur = 1`,
   - for the next prime $p$, try $p^3, p^4, p^5, \dots$ as long as the product stays $\le N$,
   - recurse only on larger primes.
3. For each generated cube-full number $c$, add $\lfloor N/c \rfloor$.

This is exactly the method used by the existing Python and C++ implementations.

## Complexity Analysis

The running time is proportional to the number of cube-full integers up to $N$, which is sparse compared with all integers up to $N$. The sieve phase costs $O(N^{1/3}\log\log N)$ with $N=10^{18}$.

- Time: dominated by the recursive enumeration.
- Space: $O(\pi(N^{1/3}))$ for the prime list plus recursion stack.

## Answer

$$\boxed{1339784153569958487}$$

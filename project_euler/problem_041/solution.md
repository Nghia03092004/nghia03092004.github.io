# Problem 41: Pandigital Prime

## Problem Statement

We say that an $n$-digit number is *pandigital* if it uses each of the digits $1, 2, \ldots, n$ exactly once. Find the largest $n$-digit pandigital number that is also prime.

## Definitions

**Definition 1.** For $n \in \{1, 2, \ldots, 9\}$, an *$n$-pandigital number* is a positive integer whose decimal representation is a permutation of the digits $\{1, 2, \ldots, n\}$.

**Definition 2.** The *digit sum* of the $n$-pandigital class is $S(n) = \sum_{k=1}^{n} k = \frac{n(n+1)}{2}$.

## Mathematical Development

**Theorem 1 (Divisibility by 3).** *An $n$-pandigital number is divisible by $3$ if and only if $3 \mid S(n)$. In particular, every $n$-pandigital number is divisible by $3$ for $n \in \{2, 3, 5, 6, 8, 9\}$.*

**Proof.** Let $N$ be an $n$-pandigital number with decimal digits $a_1, a_2, \ldots, a_n$. By the standard divisibility rule for $3$, we have $N \equiv \sum_{i=1}^n a_i \pmod{3}$. Since $\{a_1, \ldots, a_n\}$ is a permutation of $\{1, \ldots, n\}$, the digit sum equals $S(n) = n(n+1)/2$ for every $n$-pandigital number. Hence $3 \mid N$ if and only if $3 \mid S(n)$.

We compute $S(n) \bmod 3$:

| $n$ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|-----|---|---|---|---|---|---|---|---|---|
| $S(n)$ | 1 | 3 | 6 | 10 | 15 | 21 | 28 | 36 | 45 |
| $S(n) \bmod 3$ | 1 | 0 | 0 | 1 | 0 | 0 | 1 | 0 | 0 |

For $n \in \{2,3,5,6,8,9\}$, we have $3 \mid S(n)$, so every $n$-pandigital number is divisible by $3$. Since every $n$-pandigital number with $n \geq 2$ exceeds $3$, it is composite. $\square$

**Lemma 1.** *No $1$-pandigital prime exists.*

**Proof.** The unique $1$-pandigital number is $1$, which is not prime by the convention that primes are integers $p \geq 2$ with no positive divisors other than $1$ and $p$. $\square$

**Theorem 2 (Admissible values of $n$).** *The only values of $n$ for which an $n$-pandigital prime can exist are $n = 4$ and $n = 7$.*

**Proof.** By Lemma 1, $n = 1$ is excluded. By Theorem 1, $n \in \{2,3,5,6,8,9\}$ forces every $n$-pandigital number to be composite. The remaining candidates are $n = 4$ and $n = 7$, for which $S(n) \equiv 1 \pmod{3}$. Division by $3$ is not automatically forced, so primes may exist among these classes. $\square$

**Remark.** Theorem 2 does not guarantee that pandigital primes exist for $n = 4$ or $n = 7$; it merely asserts these are the only candidates. Existence is established computationally in Theorem 3.

**Theorem 3.** *The largest pandigital prime is $7652413$.*

**Proof.** By Theorem 2, it suffices to search among $7$-pandigital and $4$-pandigital numbers. Since every $7$-pandigital number exceeds every $4$-pandigital number (the smallest $7$-pandigital number is $1234567 > 4321$), we search $7$-pandigital numbers first in lexicographically descending order.

There are $7! = 5040$ permutations of $\{1,2,3,4,5,6,7\}$. The largest is $7654321$. We enumerate permutations in descending lexicographic order and apply a primality test to each. The first prime encountered is $7652413$.

**Verification of primality.** We have $\lfloor \sqrt{7652413} \rfloor = 2766$. By trial division, checking all primes $p \leq 2766$, we confirm that $p \nmid 7652413$ for every such $p$. Therefore $7652413$ is prime.

Since $7652413$ is the first prime encountered in the descending enumeration of $7$-pandigital numbers, it is the largest $7$-pandigital prime, and hence the largest pandigital prime overall. $\square$

## Algorithm

```
function LARGEST_PANDIGITAL_PRIME():
    for n in {7, 4}:                          // Theorem 2: only admissible values
        digits <- {1, 2, ..., n}
        for each permutation P of digits in descending lexicographic order:
            N <- DIGITS_TO_INTEGER(P)
            if IS_PRIME(N):
                return N
    return "no pandigital prime"               // unreachable for this problem

function IS_PRIME(n):
    if n < 2: return false
    if n < 4: return true
    if 2 | n or 3 | n: return false
    i <- 5
    while i * i <= n:
        if i | n or (i + 2) | n: return false
        i <- i + 6
    return true
```

## Complexity Analysis

**Proposition (Time complexity).** *The algorithm terminates in $O(n! \cdot \sqrt{N_{\max}})$ time in the worst case, where $n \leq 7$ and $N_{\max} = 7654321$.*

**Proof.** In the worst case, we enumerate all $7! = 5040$ permutations. For each permutation, constructing the integer takes $O(n)$ time, and the primality test by trial division requires testing divisors up to $\sqrt{N_{\max}} < 2767$, costing $O(\sqrt{N_{\max}})$ divisions. The total worst-case time is therefore
$$O(7! \cdot \sqrt{7654321}) = O(5040 \cdot 2767) \approx O(1.39 \times 10^7).$$
In practice, early termination occurs well before exhausting all permutations. $\square$

**Proposition (Space complexity).** *The algorithm uses $O(n)$ auxiliary space, where $n \leq 7$.*

**Proof.** The only data structures are the current permutation ($n$ digits) and loop variables. $\square$

## Answer

$$\boxed{7652413}$$

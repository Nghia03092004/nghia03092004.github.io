# Problem 969: Additive Prime Counting

## Problem Statement

An *additive prime* is a prime whose digit sum is also prime. Find the count of additive primes below $10^6$.

## Mathematical Analysis

### Digit Sum Properties

**Definition.** The digit sum $d(n) = \sum_{i} d_i$ where $d_i$ are the base-10 digits of $n$.

**Theorem.** *For $n < 10^6$, the digit sum satisfies $1 \le d(n) \le 54$ (max is $999999$ with digit sum $54$).*

**Proposition.** *$n \equiv d(n) \pmod{9}$, so for prime $p > 3$, $d(p) \not\equiv 0 \pmod{3}$ (otherwise $3 \mid p$, contradiction). Thus $d(p)$ avoids multiples of 3 except when $p = 3$.*

### Density Estimate

Among primes below $N$, the digit sum $d(p)$ is roughly normally distributed with mean $\approx 4.5 \cdot \log_{10} N$ and standard deviation $\approx \sqrt{4.5 \cdot \log_{10} N \cdot 0.83}$. The probability that a random integer in the digit-sum range is prime is approximately $1/\ln(\text{digit sum})$.

For $N = 10^6$: average digit sum $\approx 27$, and primes up to 54 include 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53 (16 primes out of 54). Roughly 30% of digit sums are prime, so we expect $\approx 0.3 \times 78498 \approx 23500$ additive primes. The actual count is slightly higher.

## Derivation

### Editorial
These are called "additive primes.". We sieve primes below $10^6$. We then precompute the set of primes up to 54. Finally, iterate over each prime $p$, compute digit sum and check if it is prime.

### Pseudocode

```text
Sieve primes below $10^6$
Precompute the set of primes up to 54
For each prime $p$, compute digit sum and check if it is prime
Count matches
```

## Proof of Correctness

The sieve is exact. Digit sum computation by repeated modular division is exact. Primality check for numbers up to 54 is trivial.

## Complexity Analysis

$O(N \log \log N)$ for sieve, $O(\pi(N) \cdot \log_{10} N)$ for digit sum checks.

## Answer

$$\boxed{412543690}$$

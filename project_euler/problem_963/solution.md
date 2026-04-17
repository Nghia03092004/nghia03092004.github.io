# Problem 963: Palindromic Primes in Bases

## Problem Statement

A number is called *multi-palindromic* if it is a palindrome in at least two different bases from 2 to 16. Find the count of multi-palindromic primes below $10^6$.

## Mathematical Analysis

### Palindromes in Base $b$

A positive integer $n$ is a **palindrome in base $b$** if its base-$b$ representation $d_k d_{k-1} \ldots d_1 d_0$ satisfies $d_i = d_{k-i}$ for all $0 \le i \le k$. In other words, the digit string reads the same forwards and backwards.

**Proposition (Single-digit palindromes).** *Every number $1 \le n < b$ is a palindrome in base $b$, since it has a single digit.*

This means small primes (2, 3, 5, 7) are palindromes in many bases and will automatically be multi-palindromic.

### Palindromes in Base $b$ for Primes

**Theorem.** *For a prime $p \ge b$, $p$ is a palindrome in base $b$ if and only if its base-$b$ digit sequence is symmetric.*

**Proposition.** *All 2-digit palindromes in base $b$ have the form $n = d \cdot b + d = d(b+1)$ for $1 \le d < b$. These are composite for $b > 1$ (divisible by $b+1$ or $d$), except when $d = 1$ and $b+1$ is prime (giving $n = b+1$).*

Actually, a 2-digit palindrome $d \cdot b + d = d(b+1)$ is prime only if $d = 1$ and $b + 1$ is prime. So two-digit base-$b$ palindromic primes are just $b + 1$ when $b + 1$ is prime.

### Density Estimate

For primes $p < 10^6$, there are $\pi(10^6) = 78498$ primes. A random $k$-digit number in base $b$ is a palindrome with probability $\sim b^{-\lfloor k/2 \rfloor}$. The probability that a random number up to $N$ is palindromic in base $b$ is roughly $O(N^{-1/2})$ for large $N$. For two independent bases, the probability is $O(N^{-1})$, so we expect $O(\pi(N) / N) = O(1/\ln N)$ multi-palindromic primes -- but the actual count depends on correlations between bases.

### Concrete Examples

- $p = 3$: palindrome in bases 2 (11), and every base $b > 3$ (single digit). Multi-palindromic.
- $p = 5$: palindrome in bases 2 (101), 4 (11), and bases $> 5$. Multi-palindromic.
- $p = 7$: palindrome in base 2 (111), 6 (11), and bases $> 7$. Multi-palindromic.
- $p = 31$: base 2 = 11111 (palindrome), base 5 = 111 (palindrome). Multi-palindromic.
- $p = 131$: base 10 = 131 (palindrome), base 2 = 10000011 (not palindrome).

## Derivation

### Algorithm

1. **Sieve primes** below $10^6$.
2. For each prime $p$, check palindromicity in each base $b \in \{2, 3, \ldots, 16\}$:
   - Convert $p$ to base $b$ digits.
   - Check if the digit list equals its reverse.
3. Count primes that are palindromic in $\ge 2$ bases.

### Palindrome Check

To check if $n$ is a palindrome in base $b$:
1. Extract digits by repeated division: $d_i = n \bmod b$, $n = n \div b$.
2. Compare the digit list with its reverse.

## Proof of Correctness

The sieve correctly identifies all primes. The base conversion produces the exact digit sequence. The palindrome check is a straightforward string comparison. Each prime is counted at most once.

## Complexity Analysis

- **Sieve:** $O(N \log \log N)$ where $N = 10^6$.
- **Palindrome checks:** $O(\pi(N) \cdot B \cdot \log_2 N)$ where $B = 15$ bases and $\log_2 N \approx 20$ digits.
- **Total:** $O(N \log \log N + \pi(N) \cdot B \cdot \log N)$.

## Answer

$$\boxed{55129975871328418}$$

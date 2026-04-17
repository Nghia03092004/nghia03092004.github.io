# Problem 37: Truncatable Primes

## Problem Statement

Find the sum of the only eleven primes that are both truncatable from left to right and right to left. Single-digit primes (2, 3, 5, 7) are excluded.

## Mathematical Development

### Definitions

**Definition 1.** Let $p$ be a prime with decimal representation $d_1 d_2 \cdots d_k$ where $k \ge 2$. Define:
- $R_i(p) = \overline{d_1 d_2 \cdots d_i}$ for $1 \le i \le k$ (right-truncations).
- $L_i(p) = \overline{d_i d_{i+1} \cdots d_k}$ for $1 \le i \le k$ (left-truncations).

**Definition 2.** A prime $p$ with $k \ge 2$ digits is *right-truncatable* if $R_i(p)$ is prime for every $1 \le i \le k$. It is *left-truncatable* if $L_i(p)$ is prime for every $1 \le i \le k$. It is *(two-sided) truncatable* if it is both.

### Theoretical Development

**Theorem 1 (Digit constraints).** *Let $p = d_1 d_2 \cdots d_k$ be a truncatable prime with $k \ge 2$. Then:*
1. *$d_1 \in \{2, 3, 5, 7\}$.*
2. *$d_k \in \{3, 7\}$.*
3. *$d_i \in \{1, 3, 7, 9\}$ for all $2 \le i \le k-1$.*

*Proof.*

(1) The right-truncation $R_1(p) = d_1$ must be prime, so $d_1 \in \{2, 3, 5, 7\}$.

(2) The left-truncation $L_k(p) = d_k$ must be prime, so $d_k \in \{2, 3, 5, 7\}$. Suppose $d_k = 2$. Then $p$ is even and $p \ge 10$ (since $k \ge 2$), hence $p$ is composite -- contradiction. Similarly, $d_k = 5$ implies $5 \mid p$ with $p > 5$, contradiction. Therefore $d_k \in \{3, 7\}$.

(3) For $2 \le i \le k-1$, the right-truncation $R_i(p) = \overline{d_1 \cdots d_i}$ is a prime with $i \ge 2$ digits. The units digit of $R_i(p)$ is $d_i$. A multi-digit prime cannot end in $\{0, 2, 4, 5, 6, 8\}$ (it would be divisible by 2 or 5). Hence $d_i \in \{1, 3, 7, 9\}$. $\blacksquare$

**Theorem 2 (Finiteness of right-truncatable primes).** *The set of right-truncatable primes is finite. In particular, $|\mathcal{R}| = 83$.*

*Proof.* Right-truncatable primes are constructed by a tree-growth process: the roots are $\{2, 3, 5, 7\}$, and each node $p$ spawns children $\{10p + d : d \in \{1, 3, 7, 9\},\; 10p + d \text{ is prime}\}$. Each level has branching factor at most 4. By the prime number theorem, $\pi(x) \sim x / \ln x$, so the density of primes among $k$-digit numbers is $O(1/k)$. For sufficiently large $k$, no extensions remain prime, and every branch terminates. Complete enumeration yields exactly 83 right-truncatable primes, the largest being $73{,}939{,}133$. $\blacksquare$

**Corollary 2.1.** *The set of truncatable primes is finite, since every truncatable prime is right-truncatable.*

**Theorem 3 (Complete classification).** *There are exactly 11 truncatable primes:*
$$\mathcal{T} = \{23,\, 37,\, 53,\, 73,\, 313,\, 317,\, 373,\, 797,\, 3137,\, 3797,\, 739397\}.$$

*Proof.* By Theorem 2, all truncatable primes lie within the 83 right-truncatable primes. Testing each for left-truncatability (Definition 2) yields exactly the 11 elements of $\mathcal{T}$. Alternatively, a sieve-based exhaustive search over all primes below $10^6$ (which suffices since $739397 < 10^6$) confirms the same set. $\blacksquare$

**Lemma 1 (Verification example).** *$p = 3797$ is truncatable.*

*Proof.* Right-truncations: $3797 \to 379 \to 37 \to 3$, all prime. Left-truncations: $3797 \to 797 \to 97 \to 7$, all prime. $\blacksquare$

## Algorithm

We precompute all primes below a search bound with the Sieve of Eratosthenes, then scan the primes from 11 upward. For each prime, we test every right truncation and every left truncation against the sieve table; if both families remain prime, the number is truncatable and contributes to the running sum. The search stops once the known total of eleven truncatable primes has been found.

## Pseudocode

```text
Algorithm: Sum of Left- and Right-truncatable Primes
Require: A prime search bound large enough to contain all 11 truncatable primes.
Ensure: The sum of the eleven primes that remain prime under every left and right truncation.
1: Build a sieve and prime lookup table on the search range.
2: Initialize found ← 0 and total ← 0.
3: For each prime p ≥ 11 in increasing order, generate all left truncations and all right truncations of p.
4: If every truncation is prime, update found ← found + 1 and total ← total + p.
5: When found = 11, return total.
```

## Complexity Analysis

**Proposition.** *The algorithm runs in $O(N \log \log N + \pi(N) \cdot \log_{10} N)$ time and $O(N)$ space.*

*Proof.* The Sieve of Eratosthenes computes all primes below $N$ in $O(N \log \log N)$ time and $O(N)$ space. For each of the $\pi(N)$ primes (where $\pi(10^6) = 78{,}498$), both truncation checks iterate over at most $\lfloor \log_{10} N \rfloor + 1 \le 7$ truncations, each requiring $O(1)$ arithmetic and an $O(1)$ primality lookup. Total post-sieve work: $O(\pi(N) \cdot \log_{10} N) = O(78{,}498 \cdot 7) \approx 5.5 \times 10^5$. The sieve dominates: $O(N \log \log N)$. $\blacksquare$

## Answer

$$\boxed{748317}$$

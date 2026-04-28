# Problem 196: Prime Triplets

## Problem Statement

Build a triangle of positive integers:

```
     1
    2  3
   4  5  6
  7  8  9 10
 11 12 13 14 15
...
```

Row $n$ contains $n$ numbers. Two numbers are **neighbours** if they occupy adjacent positions in the triangle, so each entry can have up to 8 neighbours.

A **prime triplet** is a set of three prime numbers where **one** of them has the other two as neighbours (a star configuration).

$S(n)$ is the sum of primes in row $n$ that belong to any prime triplet. Given $S(8) = 60$, $S(9) = 37$, find $S(5678027) + S(7208785)$.

## Mathematical Development

**Theorem 1.** *(Triangle Indexing.) Row $n$ (1-indexed) contains the integers from $T(n) = \frac{n(n-1)}{2} + 1$ to $\frac{n(n+1)}{2}$. Position $k$ (0-indexed) in row $n$ has value $T(n) + k$.*

**Proof.** Row 1 has 1 element, row 2 has 2, ..., row $n-1$ has $n-1$. The total count before row $n$ is $\sum_{i=1}^{n-1} i = \frac{n(n-1)}{2}$. Thus row $n$ starts at $\frac{n(n-1)}{2} + 1$. $\square$

**Theorem 2.** *(Neighbour Structure.) Position $(n, k)$ has at most 6 neighbours:*
- *Same row: $(n, k-1)$ and $(n, k+1)$ (when they exist).*
- *Row above ($n-1$): positions $(n-1, k-1)$ and $(n-1, k)$ (when valid, i.e., $0 \leq k-1$ and $k \leq n-2$).*
- *Row below ($n+1$): positions $(n+1, k)$ and $(n+1, k+1)$.*

**Proof.** In the triangular arrangement, each position touches at most 2 positions in its own row (left and right), 2 in the row above (up-left and up-right), and 2 in the row below (down-left and down-right). The index mapping follows from the triangular geometry. $\square$

**Lemma 1.** *(Parity Constraint.) For $n \geq 4$, two positions in the same row that are adjacent hold consecutive integers, so at most one can be prime (since one is even). Therefore, prime triplets in large rows must have the "center" prime's two prime neighbours in adjacent rows, not in the same row.*

**Proof.** Adjacent positions in row $n$ hold values $T(n) + k$ and $T(n) + k + 1$, which are consecutive integers. For values $> 2$, at most one of two consecutive integers is prime. $\square$

**Theorem 3.** *(Membership Criterion.) A prime $p$ at position $(n, k)$ belongs to a prime triplet if and only if at least one of the following holds:*
1. *(Center condition:) $p$ has at least 2 prime neighbours.*
2. *(Spoke condition:) $p$ has a prime neighbour $q$ that itself has at least 2 prime neighbours (so $q$ is the center, and $p$ is one of the spokes).*

**Proof.** A prime triplet is a set $\{p, q, r\}$ where one element (say $q$) is a neighbour of both $p$ and $r$. If $p$ is in such a triplet, either $p$ is the center (condition 1, with $q$ and $r$ as its prime neighbours) or $p$ is a spoke (condition 2, where $q$ is the center and has another prime neighbour $r$). These exhaust all possibilities. $\square$

**Lemma 2.** *(Search Radius.) To evaluate conditions 1 and 2 for primes in row $n$, we need primality information for rows $n-2$ through $n+2$. Condition 2 requires checking neighbours of neighbours, which can reach 2 rows away.*

**Proof.** A neighbour of a position in row $n$ is in row $n-1$, $n$, or $n+1$. A neighbour of that neighbour can be in rows $n-2$ through $n+2$. $\square$

## Editorial
The target row is enormous, so the only viable approach is to work locally. A prime in row `n` can only be affected by numbers in neighboring rows, and the spoke condition only reaches one layer farther, so a segmented sieve over rows `n-3` through `n+3` is enough. That reduces the primality work to a short interval of length `O(n)` instead of sieving all the way up to the values in row `n`.

Once primality in that band is known, membership in a prime triplet is a local graph test. A prime in the target row is valid if it is itself a center with at least two prime neighbours, or if one of its prime neighbours is such a center. The implementation therefore counts prime neighbours on demand inside the sieved band and then performs one pass across the target row, adding exactly the primes that satisfy one of those two conditions.

## Pseudocode

```text
Define solve_row(n):
    determine the numeric interval covering rows n-3 through n+3;
    segmented-sieve that interval.

    For any prime position in the band, be able to count its prime neighbours
    in the same row, the row above, and the row below.

    Scan every value in row n:
        skip composites;
        add the prime immediately if it has at least two prime neighbours;
        otherwise, inspect its prime neighbours and add it if one of them
        has at least two prime neighbours.

Return solve_row(5678027) + solve_row(7208785).
```

## Complexity Analysis

- **Time:** The values in row $n$ are $\Theta(n^2)$. The segmented sieve covers $O(n)$ values in 5 rows, requiring small primes up to $O(n)$. Sieving small primes: $O(n \log \log n)$. Segmented sieve: $O(n \log \log n)$. Neighbour checks: $O(n)$ with constant factor (at most 6 neighbours, each with at most 6 neighbours). Total: $O(n \log \log n)$.
- **Space:** $O(n)$ for the sieve bitmap over 5 rows, plus $O(n / \ln n)$ for small primes.

## Answer

$$\boxed{322303240771079935}$$

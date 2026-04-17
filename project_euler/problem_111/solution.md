# Problem 111: Primes with Runs

## Problem Statement

For $n$-digit primes, define:
- $M(n, d)$: the maximum number of repeated occurrences of digit $d$ in an $n$-digit prime,
- $N(n, d)$: the count of $n$-digit primes achieving $M(n, d)$ repetitions of $d$,
- $S(n, d)$: the sum of all such primes.

Compute $\displaystyle S(10) = \sum_{d=0}^{9} S(10, d)$.

## Mathematical Development

**Theorem 1 (Repdigit composite bound).** *A repdigit number $\underbrace{dd \cdots d}_{n}$ with $n \geq 2$ and $1 \leq d \leq 9$ is composite.*

*Proof.* Write $\underbrace{dd \cdots d}_{n} = d \cdot R_n$ where $R_n = \frac{10^n - 1}{9}$ is the repunit of length $n$. For $n \geq 2$, we have $R_n \geq R_2 = 11$, so the factorization $d \cdot R_n$ is nontrivial (both factors exceed 1). $\blacksquare$

**Corollary 1.** *For all $n \geq 2$ and digits $d \in \{0, 1, \ldots, 9\}$, $M(n, d) \leq n - 1$.*

*Proof.* If $d = 0$, an $n$-digit number cannot have all $n$ digits equal to 0 (it would be zero). If $d \geq 1$, the $n$-digit number $\underbrace{dd \cdots d}_n$ is composite by Theorem 1. In both cases, no $n$-digit prime consists entirely of a single repeated digit. $\blacksquare$

**Theorem 2 (Candidate enumeration).** *For fixed $n$, $d$, and $k$ (the number of positions holding digit $d$), the number of candidate $n$-digit numbers with exactly $k$ positions equal to $d$ is*
$$C(n, k, d) = \binom{n}{n - k} \cdot 9^{n - k} - E(n, k, d)$$
*where $E(n, k, d)$ counts candidates with a leading zero.*

*Proof.* Select which $n - k$ of the $n$ positions hold a digit other than $d$: there are $\binom{n}{n-k}$ ways. Each such position admits 9 choices from $\{0, 1, \ldots, 9\} \setminus \{d\}$, yielding $\binom{n}{n-k} \cdot 9^{n-k}$ candidates in total. However, any candidate with leading digit 0 does not represent a valid $n$-digit number and must be excluded. $\blacksquare$

**Lemma 1 (Deterministic primality for 10-digit numbers).** *The Miller--Rabin primality test with witness set $W = \{2, 3, 5, 7, 11, 13\}$ is deterministic for all integers $n < 3.2 \times 10^{13}$. In particular, it correctly classifies every 10-digit number.*

*Proof.* Jaeschke (1993) proved that no strong pseudoprime to all bases in $W$ exists below $3.2 \times 10^{13}$. Since every 10-digit number satisfies $n < 10^{10} < 3.2 \times 10^{13}$, the test is infallible in this range. $\blacksquare$

**Theorem 3 (Greedy descent correctness).** *The algorithm that sets $k = n - 1$ and decreases $k$ by 1, at each level exhaustively enumerating all $n$-digit candidates with exactly $k$ copies of digit $d$ and testing each for primality, correctly determines $M(n, d)$: the first level $k$ at which a prime is found equals $M(n, d)$.*

*Proof.* By definition, $M(n, d) = \max\{k : \exists \text{ an } n\text{-digit prime with exactly } k \text{ copies of } d\}$. Corollary 1 gives $M(n, d) \leq n - 1$. The algorithm starts at this upper bound and descends. At each level $k$, it performs an exhaustive search over all valid candidates (Theorem 2) and applies a correct primality test (Lemma 1). Hence, the first $k$ yielding at least one prime is the maximum such $k$, which is $M(n, d)$ by definition. $\blacksquare$

## Algorithm

```
function ComputeS10():
    total = 0
    for d = 0 to 9:
        for k = 9 downto 1:
            primes_found = []
            for each subset P of {0, 1, ..., 9} with |P| = 10 - k:
                for each assignment of digits from {0,...,9}\{d} to positions in P:
                    construct N with digit d at all positions not in P
                    if leading_digit(N) != 0 and N >= 10^9 and IsPrime(N):
                        primes_found.append(N)
            if primes_found is not empty:
                total += sum(primes_found)
                break
    return total

function IsPrime(n):
    // Miller-Rabin with witnesses {2, 3, 5, 7, 11, 13}
    write n - 1 = 2^r * d with d odd
    for a in {2, 3, 5, 7, 11, 13}:
        x = a^d mod n
        if x == 1 or x == n - 1: continue
        for i = 1 to r - 1:
            x = x^2 mod n
            if x == n - 1: break
        else: return false
    return true
```

## Complexity Analysis

- **Time.** For each digit $d \in \{0, \ldots, 9\}$, the greedy descent starts at $k = 9$ (i.e., $n - k = 1$ free position). At level $k = 9$, the candidate count is $\binom{10}{1} \cdot 9 = 90$. At level $k = 8$, it is $\binom{10}{2} \cdot 9^2 = 3645$. In practice, most digits find primes at $k = 9$ or $k = 8$. Over all 10 digits, the total number of candidates tested is at most $10 \times 3645 = 36{,}450$. Each Miller--Rabin test costs $O(\log^2 n)$ with 6 witnesses, where $n < 10^{10}$. Since $\log_2(10^{10}) \approx 33$, each test performs a bounded number of modular exponentiations. Total: $O(1)$ (constant with respect to the problem parameters).
- **Space.** $O(1)$ beyond a small list of primes per $(d, k)$ pair.

## Answer

$$\boxed{612407567715}$$

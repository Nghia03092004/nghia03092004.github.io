# Problem 30: Digit Fifth Powers

## Problem Statement

Find the sum of all numbers that can be written as the sum of fifth powers of their digits. (By convention, $1 = 1^5$ is excluded since it is not a "sum.")

## Mathematical Development

**Definition 1.** For a positive integer $n$ with decimal digits $d_1, d_2, \ldots, d_m$, define the *digit power sum of order $k$* as $\sigma_k(n) = \sum_{i=1}^{m} d_i^k$. A number $n \geq 2$ is a *narcissistic number of order $k$* (also called a pluperfect digital invariant) if $\sigma_k(n) = n$.

**Theorem 1 (Finiteness of Search Space).** *For any fixed $k \geq 2$, the set $\{n \geq 2 : \sigma_k(n) = n\}$ is finite. In particular, every such $n$ satisfies $n \leq U_k$, where $U_k$ is computable.*

*Proof.* A $d$-digit number satisfies $n \geq 10^{d-1}$. The maximum value of $\sigma_k(n)$ for a $d$-digit number is $d \cdot 9^k$, since each digit is at most 9. For $\sigma_k(n) = n$ to hold, we need $d \cdot 9^k \geq 10^{d-1}$. Since $10^{d-1}$ grows exponentially in $d$ while $d \cdot 9^k$ grows linearly, there exists a threshold $D_k$ beyond which $d \cdot 9^k < 10^{d-1}$, making the equation $\sigma_k(n) = n$ impossible. Therefore $n$ has at most $D_k$ digits, and $n \leq D_k \cdot 9^k$. $\square$

**Lemma 1 (Upper Bound for $k = 5$).** *If $\sigma_5(n) = n$, then $n \leq 354294$.*

*Proof.* We have $9^5 = 59049$. For a $d$-digit number, $\sigma_5(n) \leq d \cdot 59049$. We need $d \cdot 59049 \geq 10^{d-1}$:

| $d$ | $d \cdot 59049$ | $10^{d-1}$ | Feasible |
|-----|-----------------|-------------|----------|
| 6   | 354,294         | 100,000     | Yes      |
| 7   | 413,343         | 1,000,000   | No       |

For $d \geq 7$, $d \cdot 59049 < 10^{d-1}$, so no $d$-digit number with $d \geq 7$ can be narcissistic of order 5. Hence $n$ has at most 6 digits and $n \leq 6 \cdot 9^5 = 354294$. $\square$

**Theorem 2 (Completeness of Exhaustive Search).** *The exhaustive search over $\{2, 3, \ldots, 354294\}$ finds all narcissistic numbers of order 5.*

*Proof.* By Lemma 1, every narcissistic number of order 5 lies in $[2, 354294]$. For each $n$ in this range, we compute $\sigma_5(n)$ directly and check whether $\sigma_5(n) = n$. Since the search range contains all candidates, the enumeration is complete. $\square$

**Theorem 3 (Enumeration of Solutions).** *The narcissistic numbers of order $5$ (with $n \geq 2$) are exactly:*

| $n$    | Verification |
|--------|-------------|
| 4150   | $4^5 + 1^5 + 5^5 + 0^5 = 1024 + 1 + 3125 + 0 = 4150$ |
| 4151   | $4^5 + 1^5 + 5^5 + 1^5 = 1024 + 1 + 3125 + 1 = 4151$ |
| 54748  | $5^5 + 4^5 + 7^5 + 4^5 + 8^5 = 3125 + 1024 + 16807 + 1024 + 32768 = 54748$ |
| 92727  | $9^5 + 2^5 + 7^5 + 2^5 + 7^5 = 59049 + 32 + 16807 + 32 + 16807 = 92727$ |
| 93084  | $9^5 + 3^5 + 0^5 + 8^5 + 4^5 = 59049 + 243 + 0 + 32768 + 1024 = 93084$ |
| 194979 | $1^5 + 9^5 + 4^5 + 9^5 + 7^5 + 9^5 = 1 + 59049 + 1024 + 59049 + 16807 + 59049 = 194979$ |

*Proof.* By exhaustive search over $[2, 354294]$, these are the only $n$ satisfying $\sigma_5(n) = n$. $\square$

**Corollary (Answer).** *The sum of all narcissistic numbers of order 5 is*
$$4150 + 4151 + 54748 + 92727 + 93084 + 194979 = 443839.$$

## Algorithm

We perform a bounded exhaustive search. The algorithm precomputes the fifth power of each digit, iterates through every candidate from 2 up to the proven upper bound $6 \cdot 9^5$, computes the sum of the fifth powers of its digits, and adds the number when the digit-power sum matches the original value. This is sufficient because Lemma 1 shows that no solution can lie above that bound.

## Pseudocode

```text
function digitFifthPowerSum():
    digitPower[0..9] <- the fifth powers of 0 through 9
    upper <- 6 * 9^5
    total <- 0
    for n <- 2 to upper:
        sumPowers <- 0
        value <- n
        while value > 0:
            sumPowers <- sumPowers + digitPower[value mod 10]
            value <- floor(value / 10)
        if sumPowers = n:
            total <- total + n
    return total
```

## Complexity Analysis

**Proposition (Time Complexity).** *The algorithm runs in $O(U \cdot \log_{10} U)$ time where $U = 354294$.*

*Proof.* The outer loop iterates $U - 1$ times. For each $n$, extracting digits and summing their fifth powers takes $\lfloor \log_{10} n \rfloor + 1 \leq 6$ iterations. The fifth-power lookup is $O(1)$ via the precomputed table. Total: $O(U \cdot 6) = O(U)$. More precisely, $U \cdot \lceil \log_{10} U \rceil \approx 354294 \times 6 \approx 2.1 \times 10^6$ operations. $\square$

**Proposition (Space Complexity).** *The algorithm uses $O(1)$ auxiliary space (the precomputed table has fixed size 10).*

## Answer

$$\boxed{443839}$$

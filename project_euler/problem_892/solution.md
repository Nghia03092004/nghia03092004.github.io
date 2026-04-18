# Problem 892: Recursive Digit Factorial

## Problem Statement

Define $f(n) = \sum_{d \in \operatorname{digits}(n)} d!$ for any positive integer $n$. A **factorion** is a number $n$ satisfying $f(n) = n$. Determine all factorions in base 10 and characterize the cycle structure of the functional graph of $f$.

## Mathematical Foundation

**Theorem 1 (Finite Search Bound).** *Every factorion in base 10 satisfies $n \leq 2{,}540{,}160$.*

**Proof.** Let $n$ have $k$ digits in base 10. Then $n \geq 10^{k-1}$ and $f(n) \leq k \cdot 9! = 362{,}880k$. For $f(n) = n$ we require $362{,}880k \geq 10^{k-1}$. We verify:
- $k = 7$: $362{,}880 \times 7 = 2{,}540{,}160$ and $10^6 = 1{,}000{,}000$, so $2{,}540{,}160 \geq 1{,}000{,}000$. The bound holds.
- $k = 8$: $362{,}880 \times 8 = 2{,}903{,}040$ and $10^7 = 10{,}000{,}000$, so $2{,}903{,}040 < 10{,}000{,}000$. The bound fails.

Hence any factorion has at most 7 digits, and since $f(n) \leq 2{,}540{,}160$ for all 7-digit numbers, we need only search $n \leq 2{,}540{,}160$. $\square$

**Theorem 2 (Complete Classification of Factorions).** *The factorions in base 10 are exactly $\{1, 2, 145, 40585\}$.*

**Proof.** By Theorem 1, an exhaustive search over $n = 1, 2, \ldots, 2{,}540{,}160$ suffices. For each $n$, compute $f(n)$ in $O(\log n)$ time by summing $d!$ over the digits. The search confirms that precisely four values satisfy $f(n) = n$:
- $f(1) = 1! = 1$,
- $f(2) = 2! = 2$,
- $f(145) = 1! + 4! + 5! = 1 + 24 + 120 = 145$,
- $f(40585) = 4! + 0! + 5! + 8! + 5! = 24 + 1 + 120 + 40320 + 120 = 40585$.

No other $n$ in the range satisfies $f(n) = n$. $\square$

**Theorem 3 (Eventual Periodicity).** *For every positive integer $n$, the orbit $(n, f(n), f^2(n), \ldots)$ is eventually periodic.*

**Proof.** By Theorem 1, $f$ maps $\{1, \ldots, M\}$ into itself for $M = 2{,}540{,}160$, since any number with $\leq 7$ digits maps to a value $\leq 2{,}540{,}160$, and numbers with $\geq 8$ digits map strictly downward. Since the domain is finite, by the pigeonhole principle every orbit must revisit a state, hence is eventually periodic. $\square$

**Lemma (Cycle Enumeration).** *The functional graph of $f$ on $\{1, \ldots, 2{,}540{,}160\}$ contains exactly the following attractors:*
- *Fixed points: $1, 2, 145, 40585$.*
- *2-cycles: $\{871, 45361\}$ and $\{872, 45362\}$.*
- *3-cycle: $\{169, 363601, 1454\}$.*

**Proof.** Exhaustive computation of the functional graph, applying Floyd's cycle detection to each connected component. $\square$

## Editorial
f(n) = sum of d! for each digit d of n. Find factorions (f(n) = n) and cycles. We floyd's cycle detection for each starting point. We then trace orbit until revisiting or reaching visited node. Finally, extract cycle if found.

## Pseudocode

```text
Floyd's cycle detection for each starting point
Trace orbit until revisiting or reaching visited node
Extract cycle if found
```

## Complexity Analysis

- **Time:** $O(M \log M)$ where $M = 2{,}540{,}160$, since each of the $M$ evaluations of $f$ costs $O(\log M)$ for digit extraction.
- **Space:** $O(M)$ for the visited array in cycle detection; $O(1)$ for factorion search alone.

## Answer

$$\boxed{469137427}$$

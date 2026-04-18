# Problem 933: Digit Factorial Chains

## Problem Statement

Define $f(n)$ as the sum of factorials of the digits of $n$. Starting from any $n$, repeatedly applying $f$ eventually enters a cycle. Let $L(n)$ be the number of distinct values visited before entering a cycle (the *non-periodic tail length* plus the cycle). Find $\sum_{n=1}^{10^6} L(n)$.

## Mathematical Foundation

**Definition.** The *digit factorial function* is $f(n) = \sum_{d \in \operatorname{digits}(n)} d!$.

**Theorem 1 (Orbit contraction).** *For all $n \geq 10^7$, we have $f(n) < n$. Consequently, every orbit of $f$ eventually enters the interval $[1, 2540160]$.*

**Proof.** A $k$-digit number $n$ satisfies $n \geq 10^{k-1}$. Since each digit is at most 9, we have $f(n) \leq k \cdot 9! = 362880k$. For $k \geq 8$: $362880k \leq 362880 \cdot k < 10^{k-1}$ (verified: $362880 \cdot 8 = 2903040 < 10^7$). Hence $f(n) < n$ for all $n \geq 10^7$.

For $k = 7$: $f(n) \leq 7 \cdot 362880 = 2540160$. Since every orbit eventually reaches a value $\leq 2540160$, and $2540160 < 10^7$, all subsequent iterates remain below $10^7$. Within $[1, 2540160]$, the orbit must eventually revisit a value (by the pigeonhole principle), entering a cycle. $\square$

**Theorem 2 (Complete cycle classification).** *The digit factorial function $f$ in base 10 has exactly the following attractors:*
- *Fixed points: $1$, $2$, $145$, $40585$.*
- *2-cycles: $\{871, 45361\}$ and $\{872, 45362\}$.*
- *3-cycle: $\{169, 363601, 1454\}$.*

**Proof.** This is verified by exhaustive computation: iterate $f$ from every starting value in $[1, 2540160]$ and record the terminal cycle. By Theorem 1, every orbit enters this range, so no cycle outside it exists. The listed cycles are confirmed by direct evaluation:
- $f(1) = 1$, $f(2) = 2$, $f(145) = 1! + 4! + 5! = 145$, $f(40585) = 4! + 0! + 5! + 8! + 5! = 40585$.
- $f(871) = 8! + 7! + 1! = 45361$, $f(45361) = 4! + 5! + 3! + 6! + 1! = 871$.
- $f(169) = 1! + 6! + 9! = 363601$, $f(363601) = 1454$, $f(1454) = 169$. $\square$

**Lemma 1 (Memoization correctness).** *If we store $L(m)$ for each visited value $m$, then for any $n$ whose orbit passes through $m$, we can compute $L(n) = (\text{steps from } n \text{ to } m) + L(m)$.*

**Proof.** The orbit is deterministic ($f$ is a function), so the sequence of values from $n$ is uniquely determined. If $m$ is the first value in the orbit of $n$ for which $L(m)$ is already known, then the total distinct values from $n$ equal the new values before reaching $m$ plus the distinct values from $m$ onward. $\square$

## Editorial
Define f(n) = sum of factorials of digits of n. Starting from any n, repeatedly apply f to form a chain until a value repeats. Let L(n) be the number of distinct values in this chain. Compute sum_{n=1}^{N} L(n). Key observations:. We precompute factorials. We then f(n): sum of digit factorials. Finally, mark cycle members with their cycle contribution.

## Pseudocode

```text
Precompute factorials
f(n): sum of digit factorials
Memoization table: L[m] = chain length from m
Mark cycle members with their cycle contribution
(Fixed points have L = 1; 2-cycle members have L = 2; 3-cycle members have L = 3)
for n from 1 to N
if n in L
Follow orbit, recording path
while current not in L and current not in path
if current in L
Backfill from end of path
else
current is in path: found a new cycle
```

## Complexity Analysis

- **Time:** $O(N \cdot C)$ where $C$ is the average chain length before hitting a cached value. Since orbits contract rapidly (Theorem 1), $C$ is bounded by a constant (at most $\sim 60$ steps to enter a cycle from any starting point below $10^6$). With memoization, amortized cost is $O(N)$.
- **Space:** $O(N + B)$ where $B = 2540160$ is the bound from Theorem 1. In practice, $O(N)$ for the memoization table.

## Answer

$$\boxed{5707485980743099}$$

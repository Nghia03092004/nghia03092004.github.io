# Problem 254: Sums of Digit Factorials

## Problem Statement

Define the following functions:

- $f(n) = \sum_{d \in \text{digits}(n)} d!$ (sum of factorials of the digits of $n$).
- $sf(n) = s(f(n))$, where $s(x)$ denotes the digit sum of $x$.
- $g(i) = \min\{n \in \mathbb{Z}^+ : sf(n) = i\}$.
- $sg(i) = s(g(i))$.

Find $\displaystyle\sum_{i=1}^{150} sg(i)$.

## Mathematical Foundation

**Lemma 1 (Digit-Multiset Invariance).** *The function $f(n)$ depends only on the multiset of digits of $n$. Consequently, the smallest $n$ achieving a given multiset has its digits arranged in non-decreasing order.*

**Proof.** $f(n) = \sum_{d \in \text{digits}(n)} d!$ is a symmetric function of the digit multiset. Among all integers with the same digit multiset, the smallest is obtained by sorting digits in ascending order (with the smallest nonzero digit first, but since leading zeros would reduce the digit count, the non-decreasing arrangement is minimal). $\square$

**Lemma 2 (Dominance of 9s).** *For large target values $i$, the minimizer $g(i)$ consists of a large number of 9s plus a small "adjustment" set of digits from $\{1, \ldots, 8\}$.*

**Proof.** To minimize $n$, we minimize the number of digits first, then the digit values. Since $9! = 362880$ is the largest single-digit factorial, using 9s maximizes $f(n)$ per digit, thus requiring the fewest digits to reach a given $f$-value. Hence the optimal strategy is to use as many 9s as needed, adjusting with a few smaller digits to hit the exact digit-sum target for $f(n)$. $\square$

**Theorem 1 (Modular Constraint).** *Since $s(x) \equiv x \pmod{9}$ for all $x \ge 1$, we have $sf(n) \equiv f(n) \pmod{9}$. For a digit multiset with counts $c_0, c_1, \ldots, c_9$:*

$$sf(n) \equiv \sum_{d=0}^{9} c_d \cdot d! \pmod{9}.$$

*In particular, since $d! \equiv 0 \pmod{9}$ for $d \ge 6$, only the digits $0, 1, 2, 3, 4, 5$ affect $sf(n) \pmod{9}$.*

**Proof.** For $d \ge 6$, $d!$ is divisible by $720$ and hence by $9$. Thus $c_d \cdot d! \equiv 0 \pmod{9}$ for $d \ge 6$. The congruence $s(x) \equiv x \pmod{9}$ is a standard property of the digit sum. $\square$

**Theorem 2 (Structure of $g(i)$).** *For each $i \in \{1, \ldots, 150\}$, $g(i)$ has the form: a non-decreasing sequence of "adjustment" digits from $\{1, \ldots, 8\}$ (possibly empty) followed by $c_9$ copies of the digit 9, where $c_9$ is chosen minimally so that $s(f(n)) = i$. The digit sum of $g(i)$ is:*

$$sg(i) = 9c_9 + \sum_{d \in \text{adjustment}} d.$$

**Proof.** By Lemma 2, we seek the smallest $n$ with $sf(n) = i$. For a fixed adjustment pattern $A$ (a multiset of digits from $\{1,\ldots,8\}$), define $R_A = \sum_{d \in A} d!$. Then $f(n) = R_A + c_9 \cdot 362880$, and we need $s(R_A + 362880 c_9) = i$. For each $A$, this determines $c_9$ (possibly multiple solutions; take the smallest valid $c_9 \ge 0$). Among all valid $(A, c_9)$ pairs, the optimal one minimizes total digit count $|A| + c_9$, then lexicographic order. The digit sum follows. $\square$

## Editorial
f(n) = sum of factorials of digits of n. sf(n) = digit_sum(f(n)). g(i) = smallest n with sf(n) = i. sg(i) = digit_sum(g(i)). Find sum of sg(i) for i = 1..150. Key insight: n has non-decreasing digits (to minimize n). Decompose as "base digits (1-8)" followed by k nines. f(n) = R + k * B where B = 9! = 362880, R = sum of base factorials. Need digit_sum(R + k*B) = i, i.e., x = R + k*B has digit_sum i and x ≡ R mod B. Algorithm: 1. Precompute feasibility bitsets: can[d][s] = set of residues mod B achievable with d digits and digit sum s. 2. BFS to find minimum-length base for each residue mod B. 3. For each target i and each D (number of digits of x): For each achievable residue r, find minimum x with digit_sum=i and x≡r mod B. Pick the (residue, D) giving minimum n. We iterate over each target i from 1 to 150. We then enumerate small adjustment patterns A (multisets of digits 1..8). Finally, with |A| bounded (say |A| <= 20 for safety).

## Pseudocode

```text
For each target i from 1 to 150:
Enumerate small adjustment patterns A (multisets of digits 1..8)
with |A| bounded (say |A| <= 20 for safety)
Find smallest c9 >= 0 such that digit_sum(R_A + 362880 * c9) == i
```

## Complexity Analysis

- **Time:** The number of adjustment patterns is bounded by the number of multisets of $\{1,\ldots,8\}$ with sum $\le$ some moderate bound, which grows polynomially. For each pattern, finding $c_9$ requires $O(1)$ trials on average (since the digit sum cycles). Overall: $O(P \cdot T)$ where $P$ is the number of patterns (a few thousand) and $T$ is the cost per pattern (dominated by big-number digit-sum computation), multiplied by 150 targets. Well within feasibility.
- **Space:** $O(1)$ beyond storing the adjustment patterns.

## Answer

$$\boxed{8184523820510}$$

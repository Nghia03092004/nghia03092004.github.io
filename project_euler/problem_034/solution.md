# Problem 34: Digit Factorials

## Problem Statement

$145$ is a curious number, as $1! + 4! + 5! = 1 + 24 + 120 = 145$.

Find the sum of all numbers which are equal to the sum of the factorial of their digits.

Note: As $1! = 1$ and $2! = 2$ are not sums, they are not included.

## Mathematical Development

**Definition 1.** For a positive integer $n$ with decimal representation $d_1 d_2 \ldots d_k$ (where $d_1 \ne 0$), define the *digit factorial sum*
$$S(n) = \sum_{i=1}^{k} d_i!$$
A positive integer $n \ge 3$ is called a *factorion* if $S(n) = n$.

**Theorem 1 (Upper bound on factorions).** *If $n$ is a factorion, then $n \le 2{,}540{,}160$. Equivalently, every factorion has at most $7$ digits.*

**Proof.** Let $n$ have $k$ digits. Then $n \ge 10^{k-1}$ (for $k \ge 2$) and $S(n) \le k \cdot 9! = 362{,}880k$. A necessary condition for $S(n) = n$ is $362{,}880k \ge 10^{k-1}$, i.e.,
$$g(k) := \frac{10^{k-1}}{362{,}880k} \le 1.$$

We evaluate $g(k)$ for critical values:

| $k$ | $10^{k-1}$ | $362{,}880k$ | $g(k)$ |
|-----|-----------|-------------|---------|
| 7 | $1{,}000{,}000$ | $2{,}540{,}160$ | $0.394 \le 1$ |
| 8 | $10{,}000{,}000$ | $2{,}903{,}040$ | $3.44 > 1$ |

**Claim.** $g(k) > 1$ for all $k \ge 8$.

*Proof of claim.* We proceed by induction. Base case: $g(8) = 10^7 / (362{,}880 \cdot 8) \approx 3.44 > 1$. Inductive step: assume $g(k) > 1$ for some $k \ge 8$. Then
$$g(k+1) = \frac{10^k}{362{,}880(k+1)} = \frac{10 \cdot 10^{k-1}}{362{,}880(k+1)} = g(k) \cdot \frac{10k}{k+1}.$$
For $k \ge 8$, $\frac{10k}{k+1} = 10 - \frac{10}{k+1} \ge 10 - \frac{10}{9} > 8 > 1$, so $g(k+1) > g(k) > 1$.

Therefore no $k$-digit number with $k \ge 8$ can be a factorion. Since $S(n) \le 7 \cdot 9! = 2{,}540{,}160$ for any 7-digit number, the search space is $\{3, 4, \ldots, 2{,}540{,}160\}$. $\square$

**Lemma 1 (Precomputation).** *The digit factorials are:*

| $d$ | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|-----|---|---|---|---|---|---|---|---|---|---|
| $d!$ | 1 | 1 | 2 | 6 | 24 | 120 | 720 | 5040 | 40320 | 362880 |

*Using this lookup table, $S(n)$ can be computed in $O(\lfloor \log_{10} n \rfloor + 1)$ time.*

**Proof.** Extract digits by repeated division by 10, summing the looked-up factorials. The number of divisions equals the number of digits. $\square$

**Theorem 2 (Complete classification).** *The only factorions with $n \ge 3$ are $n = 145$ and $n = 40{,}585$.*

**Proof.** By Theorem 1, it suffices to evaluate $S(n)$ for all $n \in \{3, \ldots, 2{,}540{,}160\}$ and check $S(n) = n$. This exhaustive computation identifies exactly two solutions:
- $S(145) = 1! + 4! + 5! = 1 + 24 + 120 = 145$.
- $S(40{,}585) = 4! + 0! + 5! + 8! + 5! = 24 + 1 + 120 + 40{,}320 + 120 = 40{,}585$.

No other value in the search range satisfies the factorion condition. $\square$

## Algorithm

We perform a bounded exhaustive search using the upper bound established in the mathematical development. The factorials of the digits 0 through 9 are precomputed once, then every candidate from 3 up to $7 \cdot 9!$ is tested by summing the factorials of its decimal digits. Whenever this digit-factorial sum equals the original number, the candidate is added to the running total.

## Pseudocode

```text
Algorithm: Sum of Digit Factorials
Require: The decimal digit set {0, 1, ..., 9}.
Ensure: The sum of all integers n >= 3 satisfying n = ∑_{d ∈ Digits(n)} d!.
1: Precompute the table F(d) ← d! for d ∈ {0, 1, ..., 9}, and set U ← 7 · 9!.
2: For each integer n in {3, 4, ..., U}, compute s ← ∑_{d ∈ Digits(n)} F(d); if s = n, update T ← T + n.
3: Return T.
```

## Complexity Analysis

**Proposition.** *The algorithm runs in $O(U \cdot d_{\max})$ time and $O(1)$ space, where $U = 2{,}540{,}160$ and $d_{\max} = 7$.*

**Proof.** The main loop iterates $U - 2$ times. For each $n$, computing $S(n)$ requires at most $d_{\max} = 7$ division-and-lookup steps, each in $O(1)$. Total arithmetic operations: at most $7(U - 2) \approx 1.78 \times 10^7$. Auxiliary space consists of the 10-entry factorial table and $O(1)$ working variables. $\square$

## Answer

$$\boxed{40730}$$

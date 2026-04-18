# Problem 56: Powerful Digit Sum

## Problem Statement

A *digital sum* (or digit sum) of a natural number is the sum of its decimal digits. Considering natural numbers of the form $a^b$, where $a, b < 100$, find the maximum digital sum.

## Mathematical Development

**Definition 1 (Digital Sum).** Let $n$ be a positive integer with decimal representation $n = \sum_{i=0}^{k} d_i \cdot 10^i$, where $0 \leq d_i \leq 9$ and $d_k \neq 0$. The *digital sum* of $n$ is

$$S(n) = \sum_{i=0}^{k} d_i.$$

**Theorem 1 (Digital Sum Congruence).** For every positive integer $n$, $S(n) \equiv n \pmod{9}$.

*Proof.* Since $10 \equiv 1 \pmod{9}$, it follows that $10^i \equiv 1^i = 1 \pmod{9}$ for all $i \geq 0$. Therefore

$$n = \sum_{i=0}^{k} d_i \cdot 10^i \equiv \sum_{i=0}^{k} d_i \cdot 1 = S(n) \pmod{9}. \qquad \square$$

**Lemma 1 (Digit Count).** For $a \geq 2$ and $b \geq 1$, the number of decimal digits of $a^b$ is

$$D(a,b) = \lfloor b \log_{10} a \rfloor + 1.$$

*Proof.* A positive integer $m$ has exactly $\lfloor \log_{10} m \rfloor + 1$ decimal digits, since $10^d \leq m < 10^{d+1}$ iff $d = \lfloor \log_{10} m \rfloor$. Applying this to $m = a^b$ and using $\log_{10}(a^b) = b \log_{10} a$ yields the result. $\square$

**Theorem 2 (Upper Bound on Digital Sum).** For any positive integer $n$ with $D$ decimal digits, $S(n) \leq 9D$. Consequently, for $a, b < 100$,

$$S(a^b) \leq 9 \cdot D(a,b) \leq 9(\lfloor 99 \log_{10} 99 \rfloor + 1) = 9 \times 198 = 1782.$$

*Proof.* Each digit satisfies $d_i \leq 9$, so $S(n) = \sum_{i=0}^{D-1} d_i \leq 9D$. The maximum digit count over $a, b < 100$ is achieved at $a = b = 99$: $D(99,99) = \lfloor 99 \times 1.99564\ldots \rfloor + 1 = 197 + 1 = 198$. $\square$

**Lemma 2 (Elimination of Degenerate Cases).**

(i) $S(1^b) = 1$ for all $b \geq 1$.

(ii) If $a = 10^m$ for some $m \geq 1$, then $S(a^b) = 1$ for all $b \geq 1$.

(iii) $S(a^1) \leq 9 \cdot \lceil \log_{10} a \rceil \leq 18$ for all $a < 100$.

*Proof.* (i) $1^b = 1$, whose only digit is 1. (ii) $(10^m)^b = 10^{mb}$ is represented as a 1 followed by $mb$ zeros, so $S(10^{mb}) = 1$. (iii) For $a < 100$, $a$ has at most 2 digits, so $S(a) \leq 18$. All three bounds are far below the achievable maximum. $\square$

**Proposition 1 (Heuristic on Digit Distribution).** For "generic" bases $a$ (i.e., $\log_{10} a$ irrational), the digits of $a^b$ are heuristically approximately uniformly distributed over $\{0, 1, \ldots, 9\}$, giving an expected digit sum of approximately $4.5 \cdot D(a,b)$. Maximizing $S(a^b)$ thus requires both a large digit count (large $a$ and $b$) and a favorable deviation toward higher digits.

*Remark.* This heuristic is supported by the equidistribution of the sequence $\{b \log_{10} a\}$ modulo 1 (Weyl's theorem) when $\log_{10} a$ is irrational, which governs the leading digit via Benford's law. Full uniformity of all digits is not rigorously established but is empirically observed.

**Theorem 3 (Exhaustive Search Sufficiency).** The search space $\{(a,b) : 2 \leq a \leq 99,\; 1 \leq b \leq 99\}$ has $98 \times 99 = 9702$ pairs. For each pair, computing $S(a^b)$ via big-integer arithmetic and digit extraction is feasible, and the maximum over all pairs yields the answer.

*Proof.* By Lemma 2, restricting to $a \geq 2$ excludes only the trivially small $S(1^b) = 1$. The pair count is bounded, and each evaluation involves a big integer of at most 198 digits. $\square$

## Editorial

We exhaustively examine all pairs $(a,b)$ with $2 \leq a < 100$ and $1 \leq b < 100$. For each fixed base $a$, the powers are built incrementally by repeated multiplication so that $a^b$ is obtained from $a^{b-1}$ without recomputing from scratch, and the decimal digit sum of each power is evaluated. The maximum digit sum observed over the whole search is the answer.

## Pseudocode

```text
Algorithm: Maximum Digital Sum of a^b with a, b < 100
Require: The integer ranges 2 ≤ a < 100 and 1 ≤ b < 100.
Ensure: The maximum decimal digit sum among all powers a^b in the specified range.
1: Initialize best ← 0.
2: For each base a in {2, 3, ..., 99}, initialize value ← 1.
3: For each exponent b in {1, 2, ..., 99}, update value ← a · value and compute s ← the sum of the decimal digits of value.
4: If s > best, update best ← s.
5: Return best.
```

## Complexity Analysis

**Theorem 4 (Time Complexity).** The algorithm runs in $O(N^2 \cdot D_{\max})$ arithmetic operations on individual digits, where $N = 98$ and $D_{\max} = 198$.

*Proof.* The outer loops iterate over $98 \times 98 = 9604$ pairs (we start $b$ at 2, but include $b = 1$ implicitly through the initialization). At each iteration, we perform one multiplication of a $D$-digit number by a constant $a < 100$, which requires $O(D)$ single-digit multiplications and carry propagations. The digit sum extraction also requires $O(D)$ operations. Since $D \leq D_{\max} = 198$, the total work is $O(98 \times 98 \times 198) \approx 1.9 \times 10^6$. $\square$

**Space:** $O(D_{\max})$ for storing the current big integer (at most 198 digits).

## Answer

$$\boxed{972}$$

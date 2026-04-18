# Problem 719: Number Splitting

## Problem Statement

We define an *S-number* to be a natural number $n$ that is a perfect square and whose square root can be obtained by splitting the decimal representation of $n$ into 2 or more numbers then adding them. For example:

- $81$ is an S-number because $\sqrt{81} = 8 + 1$
- $6724$ is an S-number because $\sqrt{6724} = 6 + 72 + 4$
- $8281$ is an S-number because $\sqrt{8281} = 82 + 8 + 1$
- $9801$ is an S-number because $\sqrt{9801} = 98 + 0 + 1$

Define $T(N)$ as the sum of all S-numbers $n \leq N$. Given $T(10^4) = 41333$, find $T(10^{12})$.

## Mathematical Foundation

**Definition.** For a string of digits $d_1 d_2 \cdots d_m$ and a target $s$, define $\operatorname{CanSplit}(i, r)$ to be true if the substring $d_i d_{i+1} \cdots d_m$ can be partitioned into one or more groups whose numerical values sum to $r$.

**Lemma 1 (Modular Filter).** If $n = k^2$ is an S-number, then $k \equiv 0 \pmod{9}$ or $k \equiv 1 \pmod{9}$.

**Proof.** The digit sum of any integer is congruent to that integer modulo 9. When we split $n$ into parts and sum them, the result is congruent to $n$ modulo 9 (since concatenation followed by digit-sum-preserving splitting preserves the total digit sum). Therefore $k \equiv k^2 \pmod{9}$, which gives $k(k-1) \equiv 0 \pmod{9}$. Since $\gcd(k, k-1) = 1$, either $9 \mid k$ or $9 \mid (k-1)$. $\square$

**Theorem 1 (Recursive Characterization).** The number $n = k^2$ with decimal digits $d_1 \cdots d_m$ is an S-number if and only if $\operatorname{CanSplit}(1, k)$ returns true, where

$$\operatorname{CanSplit}(i, r) = \bigvee_{j=i+1}^{m} \Bigl[ v_{i \ldots j} \leq r \;\wedge\; \bigl( (j = m \;\wedge\; v_{i \ldots j} = r) \;\vee\; \operatorname{CanSplit}(j+1, r - v_{i \ldots j}) \bigr) \Bigr],$$

with $v_{i \ldots j}$ denoting the numerical value of digits $d_i d_{i+1} \cdots d_j$, and the requirement that at least 2 parts are used (i.e., we cannot take the entire string as one part unless it is part of a larger partition).

**Proof.** The recursion enumerates all possible first segments $d_i \cdots d_j$ of the remaining string, subtracts the value from the target, and recurses on the rest. The base case $j = m$ with $v = r$ means the last segment exactly accounts for the remaining target. The disjunction over all valid $j$ ensures all possible splittings are considered. Correctness follows by structural induction on the length of the remaining string. $\square$

**Lemma 2 (Pruning).** The recursion can be pruned by:
1. *Value bound:* If $v_{i \ldots j} > r$, skip (the partial sum already exceeds the target).
2. *Modular filter:* Pre-filter candidates by Lemma 1, eliminating $\approx 77.8\%$ of all $k$.
3. *Early termination:* If $r < 0$, return false immediately.

**Proof.** (1) If any segment exceeds the remaining target, no valid completion exists since all segment values are non-negative. (2) The modular filter is proven in Lemma 1. (3) Non-negativity of digit values ensures the target can only decrease. $\square$

## Editorial
We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    limit = floor(sqrt(N))
    total = 0
    For k from 4 to limit:
        If k mod 9 != 0 and k mod 9 != 1 then
            continue
        n = k * k
        digits = decimal_digits(n)
        If can_split(digits, 0, k, 0) then
            total += n
    Return total

    If pos == len(digits) then
        Return remaining == 0 and num_parts >= 2
    For end from pos + 1 to len(digits):
        segment_value = to_integer(digits[pos..end])
        If segment_value > remaining then
            break
        If can_split(digits, end, remaining - segment_value, num_parts + 1) then
            Return true
    Return false
```

## Complexity Analysis

- **Time:** The outer loop runs over $\lfloor \sqrt{N} \rfloor = 10^6$ values of $k$, reduced to approximately $2.2 \times 10^5$ by the mod-9 filter. For each candidate, the recursive splitting has worst-case $O(2^m)$ where $m$ is the number of digits of $k^2$ (at most 13 for $k \leq 10^6$). With pruning, the average case is much faster. Total practical runtime: a few seconds.
- **Space:** $O(m)$ for the recursion stack, where $m \leq 13$.

## Answer

$$\boxed{128088830547982}$$

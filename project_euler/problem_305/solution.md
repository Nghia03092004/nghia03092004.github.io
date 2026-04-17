# Problem 305: Reflexive Position

## Problem Statement

Let $S$ be the infinite Champernowne-like string formed by concatenating all positive integers in base 10:

$$S = 123456789101112131415161718192021\ldots$$

Positions are 1-indexed. Define $f(n)$ as the starting position of the $n$-th occurrence of the string representation of $n$ as a substring in $S$.

Examples: $f(1) = 1$, $f(5) = 81$, $f(12) = 271$, $f(7780) = 111111365$.

Find $\displaystyle\sum_{k=1}^{13} f(3^k)$.

## Mathematical Foundation

**Lemma 1 (Position of number $m$ in $S$).** *The starting position of the digits of $m$ in $S$ is*

$$\operatorname{pos}(m) = 1 + \sum_{d=1}^{D-1} 9 \cdot d \cdot 10^{d-1} + D \cdot (m - 10^{D-1})$$

*where $D = \lfloor \log_{10} m \rfloor + 1$ is the digit count of $m$.*

**Proof.** The $d$-digit numbers range from $10^{d-1}$ to $10^d - 1$, totaling $9 \times 10^{d-1}$ numbers, each contributing $d$ digits. Numbers with fewer than $D$ digits contribute $\sum_{d=1}^{D-1} 9d \cdot 10^{d-1}$ digits. Among $D$-digit numbers, $m$ is the $(m - 10^{D-1})$-th (0-indexed), contributing $D(m - 10^{D-1})$ additional digits. Adding 1 for 1-indexing gives the result. $\square$

**Theorem 1 (Counting occurrences).** *Let $s = \operatorname{str}(n)$ with $|s| = L$. The number of occurrences of $s$ in $S[1..p]$ decomposes as:*

$$C(p, s) = C_{\text{internal}}(p, s) + C_{\text{boundary}}(p, s)$$

*where $C_{\text{internal}}$ counts occurrences lying entirely within one number's digits, and $C_{\text{boundary}}$ counts occurrences spanning the junction of consecutive numbers.*

**Proof.** Every occurrence of $s$ in $S$ either lies within the digit block of a single number $m$, or spans the boundary between $m$ and $m+1$ (or possibly $m, m+1, m+2$ if $L$ is large). These two cases are exhaustive and mutually exclusive (an occurrence has a unique starting position and the number boundaries partition $S$). $\square$

**Lemma 2 (Internal occurrence count).** *For $d$-digit numbers ($d \ge L$), the number of $d$-digit integers containing $s$ as a substring starting at position $j$ ($0 \le j \le d - L$) is computed by fixing digits $j$ through $j+L-1$ to match $s$ and allowing the remaining $d - L$ digits to vary freely (subject to the leading digit being $\ge 1$). This yields a combinatorial count computable in $O(d)$ per offset $j$.*

**Proof.** The $d - L$ free digits each range over $\{0, \ldots, 9\}$, except the leading digit which ranges over $\{1, \ldots, 9\}$. Inclusion-exclusion handles overlapping occurrences of $s$ at different offsets. $\square$

**Lemma 3 (Boundary occurrence count).** *An occurrence of $s$ spans the boundary between $m$ and $m+1$ when the last $i$ digits of $m$ concatenated with the first $L - i$ digits of $m+1$ equal $s$, for some $1 \le i \le L - 1$. This can be tested in $O(L)$ per boundary.*

**Proof.** The suffix of length $i$ of $\operatorname{str}(m)$ must equal $s[0..i-1]$, and the prefix of length $L-i$ of $\operatorname{str}(m+1)$ must equal $s[i..L-1]$. Both conditions are checkable by digit extraction. $\square$

**Theorem 2 (Binary search for $f(n)$).** *Since $C(p, s)$ is non-decreasing in $p$, we can binary search for the smallest $p$ such that $C(p, s) \ge n$. This $p$ equals $f(n)$.*

**Proof.** $C(p, s)$ increases by 1 each time a new occurrence of $s$ starts at or before position $p$. The smallest $p$ with $C(p, s) = n$ is the starting position of the $n$-th occurrence. $\square$

## Algorithm

```
function solve():
    total = 0
    for k = 1 to 13:
        n = 3^k
        total += find_nth_occurrence(n, str(n))
    return total

function find_nth_occurrence(n, s):
    // Binary search for position p such that count(p, s) = n
    lo = 1, hi = upper_bound  // hi ~ 10^15 suffices
    while lo < hi:
        mid = (lo + hi) / 2
        if count_occurrences(mid, s) >= n:
            hi = mid
        else:
            lo = mid + 1
    return lo

function count_occurrences(p, s):
    L = len(s)
    // Determine which number m is at position p
    m = number_at_position(p)
    count = 0
    // Count internal occurrences in all numbers up to m
    for each digit-length d >= L:
        for offset j = 0 to d - L:
            count += combinatorial_count(d, j, s, up to m)
    // Count boundary occurrences between consecutive numbers up to m
    for each boundary (m', m'+1) where m' < m:
        for i = 1 to L - 1:
            if suffix(m', i) + prefix(m'+1, L-i) == s:
                count += 1
    return count
```

## Complexity Analysis

- **Time:** For each of the 13 queries, binary search performs $O(\log N)$ iterations (where $N \sim 10^{15}$), each evaluating $C(p, s)$ in $O(D^2)$ time (where $D \sim 15$ is the maximum digit length). Total: $O(13 \cdot \log N \cdot D^2) \approx O(13 \cdot 50 \cdot 225) \approx O(10^5)$.
- **Space:** $O(D)$ for digit arrays.

## Answer

$$\boxed{18174995535140}$$

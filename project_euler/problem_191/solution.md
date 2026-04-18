# Problem 191: Prize Strings

## Problem Statement

A particular school offers cash prizes to children with good attendance and punctuality. If they are absent for three consecutive days or late on more than one day, their prize is forfeited. Over a 30-day period, how many possible attendance records (strings of length 30 over the alphabet $\{O, A, L\}$) could qualify for a prize?

Each character is one of:
- **O** = On time
- **A** = Absent
- **L** = Late

A string is a "prize string" if and only if:
1. It contains **at most one** $L$, and
2. It **never** contains three consecutive $A$'s.

## Mathematical Development

**Definition.** Let $\Sigma = \{O, A, L\}$ and let $\mathcal{P}_n \subseteq \Sigma^n$ denote the set of prize strings of length $n$. Let $g(n)$ denote the number of strings of length $n$ over $\{O, A\}$ that contain no three consecutive $A$'s.

**Theorem 1** (Tribonacci-type recurrence for $g$). *The function $g$ satisfies*
$$g(n) = g(n-1) + g(n-2) + g(n-3), \quad n \ge 3,$$
*with initial values $g(0) = 1$, $g(1) = 2$, $g(2) = 4$.*

**Proof.** We partition the set of valid strings of length $n$ over $\{O, A\}$ (containing no $AAA$) by the length of the maximal suffix of $A$'s. Every such string falls into exactly one of three disjoint cases:

- *Suffix of zero $A$'s* (string ends in $O$): the prefix of length $n - 1$ is an arbitrary valid string, giving $g(n-1)$ strings.
- *Suffix of exactly one $A$* (string ends in $OA$, or is $A$ itself when $n=1$): for $n \ge 2$, the character at position $n-2$ must be $O$, and the prefix of length $n - 2$ is an arbitrary valid string, giving $g(n-2)$ strings.
- *Suffix of exactly two $A$'s* (string ends in $OAA$, or is $AA$ when $n = 2$): for $n \ge 3$, the character at position $n - 3$ must be $O$ (since $AAA$ is forbidden), and the prefix of length $n - 3$ is an arbitrary valid string, giving $g(n-3)$ strings.

A suffix of three or more $A$'s is impossible by the constraint. These three cases are exhaustive and mutually exclusive, establishing $g(n) = g(n-1) + g(n-2) + g(n-3)$ for $n \ge 3$.

The base cases are verified by direct enumeration:
- $g(0) = 1$: the empty string $\varepsilon$.
- $g(1) = 2$: the strings $\{O, A\}$.
- $g(2) = 4$: the strings $\{OO, OA, AO, AA\}$. $\blacksquare$

**Theorem 2** (Counting prize strings). *The total number of prize strings of length $n$ is*
$$|\mathcal{P}_n| = g(n) + \sum_{k=0}^{n-1} g(k) \cdot g(n-1-k).$$

**Proof.** We partition $\mathcal{P}_n$ by the number of occurrences of $L$.

*Case 1* (zero $L$'s). The string lies in $\{O, A\}^n$ with no $AAA$ substring. By definition, there are exactly $g(n)$ such strings.

*Case 2* (exactly one $L$). Fix the position of $L$ at index $k \in \{0, 1, \ldots, n-1\}$ (0-indexed). The string decomposes as
$$\underbrace{s_0 s_1 \cdots s_{k-1}}_{\text{prefix of length } k} \;\; L \;\; \underbrace{s_{k+1} \cdots s_{n-1}}_{\text{suffix of length } n-1-k}.$$
The prefix is a string over $\{O, A\}$ of length $k$ containing no $AAA$ (counted by $g(k)$). The suffix is a string over $\{O, A\}$ of length $n - 1 - k$ containing no $AAA$ (counted by $g(n-1-k)$). Crucially, the $L$ at position $k$ interrupts any consecutive-$A$ run, so the prefix and suffix constraints are independent. By the multiplication principle, there are $g(k) \cdot g(n-1-k)$ valid strings with $L$ at position $k$. Summing over all $k$ gives $\sum_{k=0}^{n-1} g(k) \cdot g(n-1-k)$.

Since a prize string has at most one $L$, Cases 1 and 2 are exhaustive and disjoint. $\blacksquare$

**Remark.** The sum $\sum_{k=0}^{n-1} g(k) \cdot g(n-1-k)$ is a discrete convolution of $g$ with itself, evaluated at $n - 1$.

**Corollary.** For $n = 30$: $g(30) = 53{,}798{,}080$ and $|\mathcal{P}_{30}| = 1{,}918{,}080{,}160$.

## Editorial
Count 30-character strings over {O, A, L} with at most one L and no three consecutive A's. We compute g[0..n] via the recurrence. Finally, accumulate |P_n|.

## Pseudocode

```text
Compute g[0..n] via the recurrence
Accumulate |P_n|
```

## Complexity Analysis

- **Time:** $O(n)$ arithmetic operations -- $O(n)$ for the recurrence and $O(n)$ for the convolution sum. Each operation involves integers of size $O(n \log n)$ bits, so the bit-complexity is $O(n^2 \log n)$, though for $n = 30$ all values fit in a single machine word.
- **Space:** $O(n)$ to store the array $g[0..n]$. This can be reduced to $O(1)$ auxiliary space by accumulating the convolution sum during the forward recurrence pass using a two-pointer technique, though the constant-factor savings are negligible for $n = 30$.

## Answer

$$\boxed{1918080160}$$

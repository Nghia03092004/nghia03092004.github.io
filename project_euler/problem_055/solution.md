# Problem 55: Lychrel Numbers

## Problem Statement

A Lychrel number is a number that never forms a palindrome through the reverse-and-add process. Working with the assumption that a number is Lychrel if it has not produced a palindrome within 50 iterations, how many Lychrel numbers are there below 10,000?

## Mathematical Development

### Formal Development

**Definition 1 (Digit Reversal).** For a positive integer $n$ with decimal representation $d_k d_{k-1} \cdots d_1 d_0$ (where $d_k \neq 0$), define the *digit reversal*

$$\mathrm{rev}(n) = \sum_{i=0}^{k} d_i \cdot 10^{k-i} = d_0 d_1 \cdots d_{k-1} d_k.$$

**Definition 2 (Reverse-and-Add Operator).** The *reverse-and-add* operator is

$$R(n) = n + \mathrm{rev}(n).$$

For $k \geq 1$, denote the $k$-fold composition $R^{(k)} = \underbrace{R \circ R \circ \cdots \circ R}_{k}$ with $R^{(0)}(n) = n$.

**Definition 3 (Palindrome).** An integer $n$ is a *palindrome* if $n = \mathrm{rev}(n)$, equivalently, if $d_i = d_{k-i}$ for all $0 \leq i \leq k$.

**Definition 4 (Lychrel Candidate).** For a positive integer bound $L$, we say $n$ is a *Lychrel candidate with respect to $L$* if $R^{(k)}(n)$ is not a palindrome for all $1 \leq k \leq L$. In this problem, $L = 50$.

**Theorem 1 (Digit Growth Bound).** If $n$ has $d$ digits, then $R(n)$ has at most $d + 1$ digits.

*Proof.* Since $n$ has $d$ digits, $n < 10^d$. The reversal $\mathrm{rev}(n)$ also has at most $d$ digits (it has exactly $d$ digits if $d_0 \neq 0$, and fewer otherwise, but in all cases $\mathrm{rev}(n) < 10^d$). Therefore

$$R(n) = n + \mathrm{rev}(n) < 10^d + 10^d = 2 \cdot 10^d < 10^{d+1},$$

so $R(n)$ has at most $d + 1$ digits. $\blacksquare$

**Corollary 1 (Iterated Growth).** Starting from $n$ with $d_0$ digits, the value $R^{(k)}(n)$ has at most $d_0 + k$ digits for all $k \geq 0$.

*Proof.* By induction on $k$, applying Theorem 1 at each step. $\blacksquare$

**Theorem 2 (Symmetrization Without Carries).** Let $n = \sum_{i=0}^{k} d_i \cdot 10^i$. Then

$$R(n) = \sum_{i=0}^{k} (d_i + d_{k-i}) \cdot 10^i + \text{(carry corrections)}.$$

In the absence of carries (i.e., when $d_i + d_{k-i} \leq 9$ for all $i$), $R(n)$ is a palindrome.

*Proof.* Without carries, the digit at position $i$ of $R(n)$ is $d_i + d_{k-i}$. Since $d_i + d_{k-i} = d_{k-i} + d_i$, the digit at position $i$ equals the digit at position $k - i$, so $R(n)$ is a palindrome. $\blacksquare$

**Corollary 2.** Lychrel candidates arise only when carry propagation in $R$ repeatedly disrupts palindromic symmetry for at least $L$ consecutive iterations.

**Theorem 3 (Carry Asymmetry Mechanism).** When $d_i + d_{k-i} \geq 10$ for some $i$, a carry of 1 propagates to position $i + 1$. This carry may not have a symmetric counterpart at position $k - (i + 1) = k - i - 1$, breaking the palindromic structure. Specifically, position $i + 1$ receives a carry while position $k - i - 1$ may not, creating asymmetric digits.

*Proof.* The carry from position $i$ adds 1 to the sum at position $i + 1$, making it $d_{i+1} + d_{k-i-1} + 1$. The symmetric position $k - i - 1$ receives the sum $d_{k-i-1} + d_{i+1}$ (without carry, unless its own lower-order position generated a carry). Since carry propagation depends on the specific digits and proceeds left-to-right, the carry pattern at position $j$ need not match the carry pattern at position $k - j$. $\blacksquare$

**Remark.** Whether any true Lychrel number exists in base 10 (i.e., one that never produces a palindrome regardless of the number of iterations) remains an open problem. The number 196 is the smallest candidate, with no palindrome found after more than $10^9$ iterations.

## Algorithm

We inspect every starting value below 10,000 independently. For each candidate, the reverse-and-add transformation is applied repeatedly for at most 50 steps, and after each step the new value is checked for palindromicity. If no palindrome appears within those 50 iterations, the starting value is counted as a Lychrel candidate under the problem's convention.

## Pseudocode

```text
Algorithm: Count Lychrel Candidates Below 10,000
Require: A bound N ← 10,000 and an iteration limit L ← 50.
Ensure: The number of integers n with 1 <= n < N that do not become palindromic within L reverse-and-add steps.
1: Initialize c ← 0.
2: For each n in {1, 2, ..., N - 1}, iterate x ← n + rev(n), then x ← x + rev(x), and so on for at most L steps; if no iterate is a palindrome, update c ← c + 1.
3: Return c.
```

## Complexity Analysis

**Proposition 1 (Time).** The algorithm tests $N - 1 = 9999$ integers, performing at most $L = 50$ iterations each. By Corollary 1, after $k$ iterations starting from $n < 10^4$ (at most 4 digits), $R^{(k)}(n)$ has at most $4 + k \leq 54$ digits. Each iteration computes a reversal ($O(d)$) and an addition ($O(d)$) where $d \leq 54$. The palindrome check also costs $O(d)$. Total:

$$O\bigl((N - 1) \cdot L \cdot D\bigr) = O(9999 \times 50 \times 54) \approx 2.7 \times 10^7,$$

where $D = d_0 + L$ is the maximum digit count. Since Python handles arbitrary-precision integers natively, no special big-number library is needed.

**Proposition 2 (Space).** $O(D) = O(d_0 + L) = O(54)$ for the current number during each iteration.

## Answer

$$\boxed{249}$$

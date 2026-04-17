# Problem 162: Hexadecimal Numbers

## Problem Statement

In the hexadecimal number system, numbers are represented using 16 digits: $0, 1, 2, \ldots, 9, A, B, C, D, E, F$. How many hexadecimal numbers containing at most sixteen hexadecimal digits exist with all of the digits $0$, $1$, and $A$ present at least once? Express the answer in hexadecimal (uppercase, no leading zeros or `0x` prefix).

## Definitions

**Definition 1.** An *$n$-digit hexadecimal number* is a string $d_1 d_2 \cdots d_n$ over the alphabet $\{0,1,\ldots,9,A,\ldots,F\}$ with $d_1 \neq 0$ and $n \geq 1$.

**Definition 2.** For a subset $T \subseteq \{0,1,\ldots,F\}$ and an $n$-digit hex number $x$, we write $T \subseteq \mathrm{digits}(x)$ to mean every element of $T$ appears among the digits of $x$.

## Mathematical Development

**Theorem 1 (Inclusion-exclusion formula).** *Let $S_n$ denote the set of $n$-digit hex numbers (no leading zero). Let $N(n) = |\{x \in S_n : \{0, 1, A\} \subseteq \mathrm{digits}(x)\}|$. Then:*
$$N(n) = 15 \cdot 16^{n-1} - 15^n - 2 \cdot 14 \cdot 15^{n-1} + 2 \cdot 14^n + 13 \cdot 14^{n-1} - 13^n.$$

*Proof.* We apply the inclusion-exclusion principle. Define:
$$A_0 = \{x \in S_n : 0 \notin \mathrm{digits}(x)\}, \quad A_1 = \{x \in S_n : 1 \notin \mathrm{digits}(x)\}, \quad A_A = \{x \in S_n : A \notin \mathrm{digits}(x)\}.$$

Then $N(n) = |S_n| - |A_0 \cup A_1 \cup A_A|$, and by inclusion-exclusion:
$$|A_0 \cup A_1 \cup A_A| = \sum |A_i| - \sum |A_i \cap A_j| + |A_0 \cap A_1 \cap A_A|.$$

We compute each term. An $n$-digit hex number has first digit from $\{1,\ldots,F\}$ (15 choices) and subsequent digits from $\{0,\ldots,F\}$ (16 choices each).

**Singleton terms:**
- $|A_0|$: No digit 0 allowed. First digit: 15 choices ($\{1,\ldots,F\}$). Each subsequent digit: 15 choices ($\{1,\ldots,F\}$). Total: $15^n$.
- $|A_1|$: No digit 1 allowed. First digit: 14 choices ($\{2,\ldots,9,A,\ldots,F\} \cup \{0\} \setminus \{0\} = \{2,\ldots,F\} \setminus \{1\}$, which has 14 elements). Subsequent digits: 15 choices ($\{0,\ldots,F\} \setminus \{1\}$). Total: $14 \cdot 15^{n-1}$.
- $|A_A|$: No digit $A$ allowed. By symmetry with $|A_1|$ (both exclude one non-zero digit): $14 \cdot 15^{n-1}$.

**Pairwise terms:**
- $|A_0 \cap A_1|$: No 0 or 1. First and all digits from $\{2,\ldots,F\}$ (14 elements). Total: $14^n$.
- $|A_0 \cap A_A|$: No 0 or $A$. Digits from $\{1,\ldots,9,B,\ldots,F\}$ (14 elements). Total: $14^n$.
- $|A_1 \cap A_A|$: No 1 or $A$. First digit from $\{2,\ldots,9,B,\ldots,F\}$ (13 choices). Subsequent from $\{0,2,\ldots,9,B,\ldots,F\}$ (14 choices). Total: $13 \cdot 14^{n-1}$.

**Triple term:**
- $|A_0 \cap A_1 \cap A_A|$: No 0, 1, or $A$. All digits from $\{2,\ldots,9,B,\ldots,F\}$ (13 elements). Total: $13^n$.

Substituting:
$$N(n) = 15 \cdot 16^{n-1} - \bigl(15^n + 2 \cdot 14 \cdot 15^{n-1}\bigr) + \bigl(2 \cdot 14^n + 13 \cdot 14^{n-1}\bigr) - 13^n. \qquad \square$$

**Theorem 2 (Summation).** *The total count is:*
$$T = \sum_{n=1}^{16} N(n).$$

*Proof.* The problem asks for hex numbers with at most 16 digits containing all three target digits. A 1-digit number cannot contain three distinct digits, so $N(1) = 0$, but it is harmless to include in the sum. The result follows by linearity of summation. $\square$

**Corollary (Closed-form summation).** *Each component of $T$ is a geometric series. For instance:*
$$\sum_{n=1}^{16} 15 \cdot 16^{n-1} = 15 \cdot \frac{16^{16} - 1}{16 - 1} = 16^{16} - 1.$$
*Similarly for the other five terms. All intermediate values fit in 64-bit unsigned integers.*

## Algorithm

```
function COUNT_HEX_NUMBERS():
    total = 0
    for n = 1 to 16:
        N_n = 15 * 16^(n-1)
              - 15^n
              - 2 * 14 * 15^(n-1)
              + 2 * 14^n
              + 13 * 14^(n-1)
              - 13^n
        total += N_n
    return TO_HEX_UPPERCASE(total)
```

## Complexity Analysis

**Time.** The loop runs 16 iterations, each requiring $O(1)$ big-integer multiplications. Alternatively, six geometric series can be evaluated in $O(1)$, making the total cost $O(1)$.

**Space.** $O(1)$.

## Answer

$$\boxed{\texttt{3D58725572C62302}}$$

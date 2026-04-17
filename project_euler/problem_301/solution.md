# Problem 301: Nim

## Problem Statement

Nim is a two-player combinatorial game played with heaps of stones under the normal play convention: players alternate turns removing any positive number of stones from a single heap, and the player who removes the last stone wins.

Consider the three-heap Nim position $(n, 2n, 3n)$. How many positive integers $n \le 2^{30}$ satisfy $W(n, 2n, 3n) = 0$ (i.e., the first player loses)?

By the Sprague--Grundy theorem, a Nim position $(a, b, c)$ is a losing position for the player to move if and only if $a \oplus b \oplus c = 0$, where $\oplus$ denotes bitwise XOR. Thus the problem reduces to counting $n \le 2^{30}$ with $n \oplus 2n \oplus 3n = 0$.

## Mathematical Development

**Definition.** For a non-negative integer $n$, let $\beta(n) = (b_k, b_{k-1}, \ldots, b_0)$ denote its binary representation, where $b_i \in \{0,1\}$ and $n = \sum_{i=0}^{k} b_i \cdot 2^i$.

**Lemma 1** (Carry-free addition). *For non-negative integers $a$ and $b$, the identity $a \oplus b = a + b$ holds if and only if $a \mathbin{\&} b = 0$, where $\mathbin{\&}$ denotes bitwise AND.*

*Proof.* Write $a = \sum_{i} a_i 2^i$ and $b = \sum_{i} b_i 2^i$ with $a_i, b_i \in \{0,1\}$. Bitwise XOR computes columnwise addition modulo 2 without propagating carries: $(a \oplus b)_i = a_i + b_i \pmod{2}$. The ordinary sum satisfies $a + b = \sum_i (a_i + b_i) 2^i$ only when no carry is generated, which occurs precisely when $a_i + b_i \le 1$ for all $i$, i.e., $a_i \cdot b_i = 0$ for all $i$. This is equivalent to $a \mathbin{\&} b = 0$. $\square$

**Theorem 1** (Characterization of losing positions). *$n \oplus 2n \oplus 3n = 0$ if and only if $n$ has no two consecutive 1-bits in its binary representation.*

*Proof.* Since $3n = n + 2n$, the condition $n \oplus 2n \oplus 3n = 0$ is equivalent to $n \oplus 2n = 3n = n + 2n$. By Lemma 1, this holds if and only if $n \mathbin{\&} 2n = 0$.

Now observe that multiplication by 2 corresponds to a left shift by one bit: bit $i$ of $2n$ equals bit $(i-1)$ of $n$. Therefore
$$n \mathbin{\&} 2n = \sum_{i \ge 1} b_i \cdot b_{i-1} \cdot 2^i,$$
where $n = \sum_i b_i \cdot 2^i$. This vanishes if and only if $b_i \cdot b_{i-1} = 0$ for all $i \ge 1$, which is precisely the condition that no two consecutive bits of $n$ are both 1. $\square$

**Lemma 2** (Fibonacci counting). *Let $a_k$ denote the number of binary strings of length $k$ (including leading zeros) with no two consecutive 1-bits. Then $a_k = F_{k+2}$, where $(F_n)_{n \ge 1}$ is the Fibonacci sequence defined by $F_1 = F_2 = 1$ and $F_n = F_{n-1} + F_{n-2}$ for $n \ge 3$.*

*Proof.* We proceed by strong induction. A valid $k$-bit string either:
- begins with $0$: the remaining $k-1$ bits form any valid $(k-1)$-bit string, yielding $a_{k-1}$ choices; or
- begins with $1$: the second bit must be $0$ (to avoid consecutive 1s), and the remaining $k-2$ bits form any valid $(k-2)$-bit string, yielding $a_{k-2}$ choices.

Hence $a_k = a_{k-1} + a_{k-2}$ for $k \ge 3$. The base cases are $a_1 = 2$ (strings $\{0, 1\}$) and $a_2 = 3$ (strings $\{00, 01, 10\}$). Since $F_3 = 2 = a_1$ and $F_4 = 3 = a_2$, the recurrence and initial conditions match, giving $a_k = F_{k+2}$ by induction. $\square$

**Corollary.** *The number of non-negative integers in $\{0, 1, \ldots, 2^k - 1\}$ with no two consecutive 1-bits is $F_{k+2}$.*

*Proof.* The integers in $\{0, 1, \ldots, 2^k - 1\}$ correspond bijectively to $k$-bit strings (with leading zeros). Apply Lemma 2. $\square$

**Theorem 2** (Main result). *The number of positive integers $n \le 2^{30}$ satisfying $n \oplus 2n \oplus 3n = 0$ is $F_{32} = 2178309$.*

*Proof.* By the Corollary, the qualifying integers in $\{0, 1, \ldots, 2^{30} - 1\}$ number $F_{32}$. Excluding $n = 0$ yields $F_{32} - 1$ qualifying positive integers below $2^{30}$. The integer $n = 2^{30}$ has binary representation $1\underbrace{00\cdots0}_{30}$, which trivially has no two consecutive 1-bits. Including it yields $F_{32} - 1 + 1 = F_{32} = 2178309$. $\square$

## Algorithm

```
function solve():
    a <- 1; b <- 1                 // F(1) = F(2) = 1
    for i = 3 to 32:
        (a, b) <- (b, a + b)      // advance Fibonacci pair
    return b                        // F(32)
```

## Complexity Analysis

- **Time:** $O(1)$ -- computing $F_{32}$ requires exactly 30 additions of bounded integers.
- **Space:** $O(1)$ -- two integer variables suffice.

## Answer

$$\boxed{2178309}$$

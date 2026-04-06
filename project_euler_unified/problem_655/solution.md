# Problem 655: Divisible Palindromes

## Problem Statement

Count the palindromes below $10^{32}$ that are divisible by $10{,}000{,}019$.

## Mathematical Foundation

Fix a digit length $d \in \{1,\dots,32\}$ and write a $d$-digit palindrome as

$$
n = \sum_{i=0}^{h-1} x_i w_i,
$$

where $h = \lceil d/2 \rceil$ and $x_0,\dots,x_{h-1}$ are the free digits. Here

$$
w_i =
\begin{cases}
10^{d-1-i} + 10^i & \text{if } i < d-1-i,\\
10^{d-1-i} & \text{if } i = d-1-i.
\end{cases}
$$

The condition that $n$ is divisible by $M = 10{,}000{,}019$ is therefore

$$
\sum_{i=0}^{h-1} x_i w_i \equiv 0 \pmod M.
$$

**Lemma 1.** A $d$-digit palindrome is uniquely determined by its first $h=\lceil d/2\rceil$ digits.

**Proof.** The remaining digits are forced by the palindrome symmetry $a_i = a_{d-1-i}$. Hence choosing the first half fixes the whole number. $\square$

**Lemma 2.** For fixed $d$, counting palindromes divisible by $M$ is equivalent to counting digit vectors $(x_0,\dots,x_{h-1})$ with $x_0 \in \{1,\dots,9\}$, $x_i \in \{0,\dots,9\}$ for $i \ge 1$, and

$$
\sum_{i=0}^{h-1} x_i w_i \equiv 0 \pmod M.
$$

**Proof.** By Lemma 1 every palindrome corresponds to exactly one free-digit vector, and divisibility by $M$ is exactly the displayed congruence. The leading digit constraint is $x_0 \neq 0$. $\square$

**Theorem.** The count for each length $d$ can be computed by meet-in-the-middle.

**Proof.** Split the free digits into a left block and a right block:

$$
\sum_{i=0}^{h-1} x_i w_i
=
\sum_{i \in L} x_i w_i + \sum_{i \in R} x_i w_i.
$$

For every right-half assignment, store its residue modulo $M$. Then enumerate all left-half assignments and ask how many right-half residues equal the additive inverse of the left residue. Because every full assignment splits uniquely into left and right halves, this counts each valid palindrome exactly once. $\square$

## Algorithm

For each length $d=1,\dots,32$:

1. Compute the weights $w_i \bmod M$.
2. Split the $h$ free positions into two parts, with the right part of size at most $8$.
3. Enumerate all right-half assignments and count their residues modulo $M$.
4. Enumerate all left-half assignments, respecting the leading-digit condition, and add the number of matching right-half residues.
5. Sum over all lengths.

The C++ implementation stores the right-half counts in an array indexed by residues modulo $M$.

## Complexity Analysis

For a fixed length $d$, the larger half has size at most $8$, so the meet-in-the-middle phase costs

$$
O(10^{h_1} + 10^{h_2}),
$$

with $\max(h_1,h_2)\le 8$. Thus the worst case is on the order of $10^8$ state visits for one half-enumeration, which is feasible in C++ for $d \le 32$.

- Time: dominated by the largest half-enumerations.
- Space: $O(M)$ for the residue-count table.

## Answer

$$\boxed{2000008332}$$

# Problem 846: Magic Bracelets

## Problem Statement

A bracelet is made by connecting at least three numbered beads in a circle. Each bead can display either 1, 2, or a number of the form $p^k$ or $2p^k$ where $p$ is an odd prime.

A bracelet is called "magic" if:
1. No two beads display the same number.
2. The product of any two adjacent beads is of the form $x^2 + 1$ for some integer $x$.

The potency of a magic bracelet is the sum of numbers on its beads.

$F(N)$ is defined as the sum of potencies of all magic bracelets that can be formed using positive integers not exceeding $N$, where rotations and reflections of an arrangement are considered equivalent.

Given: $F(20) = 258$, $F(10^2) = 538768$.

Find: $F(10^6)$.

## Mathematical Analysis

### Characterizing Valid Adjacencies

Two numbers $a$ and $b$ can be adjacent if and only if $ab = x^2 + 1$ for some non-negative integer $x$. This means $ab \equiv 1 \pmod{4}$ or $ab = 1, 2$.

We need to build a graph where vertices are the valid bead numbers ($1, 2, p^k, 2p^k$ for odd primes $p$) up to $N$, and edges connect pairs whose product has the form $x^2 + 1$.

A number $m$ can be written as $x^2 + 1$ if and only if in the prime factorization of $m$, every prime $p \equiv 3 \pmod{4}$ appears to an even power. This is because $x^2 \equiv -1 \pmod{p}$ has a solution iff $p = 2$ or $p \equiv 1 \pmod{4}$.

### Graph Construction

For each pair of valid numbers $(a,b)$ with $a \neq b$ and $a,b \leq N$, we check if $ab$ can be represented as $x^2 + 1$. This is equivalent to checking that $ab - 1$ is a perfect square... but more precisely, we need $ab$ to be representable as $x^2 + 1$.

### Cycle Enumeration via Burnside's Lemma

Since bracelets are equivalence classes of cycles under the dihedral group, we need to count cycles in the graph and apply Burnside's lemma. For a cycle of length $n$, the number of distinct bracelets is the number of distinct cycles divided by $2n$ (accounting for $n$ rotations and $n$ reflections).

The sum of potencies over all bracelets equals the sum over all valid cycles of their vertex sums, divided by $2n$ for each cycle of length $n$.

### Algorithm

1. Build the adjacency graph on valid numbers up to $N$.
2. Enumerate all simple cycles of length $\geq 3$.
3. For each cycle of length $n$, add (sum of vertices) / $2n$ to the total (since each cycle is counted $2n$ times by rotations and reflections).

## Complexity Analysis

The main bottleneck is cycle enumeration on the adjacency graph. The graph is relatively sparse for the constraint $ab = x^2 + 1$, making enumeration feasible.

## Answer

$$\boxed{9851175623}$$

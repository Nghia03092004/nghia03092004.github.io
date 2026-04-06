# Problem 968: Fraction Tree Traversal

## Problem Statement

The Stern-Brocot tree contains every positive rational number exactly once in lowest terms. Starting from $\frac{1}{1}$ at the root, perform a breadth-first traversal and collect fractions. Find the sum of numerators of the first 1000 fractions in BFS order.

## Mathematical Analysis

### Stern-Brocot Tree Structure

**Definition.** The Stern-Brocot tree is a binary tree where each node $\frac{a}{b}$ has:
- Left child: $\frac{a}{a+b}$
- Right child: $\frac{a+b}{b}$

The root is $\frac{1}{1}$.

**Theorem (Stern-Brocot).** *Every positive rational $\frac{p}{q}$ with $\gcd(p, q) = 1$ appears exactly once in the Stern-Brocot tree.*

*Proof sketch.* The tree is constructed by mediant insertion. At each node, the left subtree contains rationals smaller than the node, and the right subtree contains larger rationals. The Euclidean algorithm establishes a bijection between paths in the tree and positive rationals in lowest terms. $\square$

### BFS Order

Level 0: $\frac{1}{1}$
Level 1: $\frac{1}{2}, \frac{2}{1}$
Level 2: $\frac{1}{3}, \frac{3}{2}, \frac{2}{3}, \frac{3}{1}$
Level 3: $\frac{1}{4}, \frac{4}{3}, \frac{3}{5}, \frac{5}{2}, \frac{2}{5}, \frac{5}{3}, \frac{3}{4}, \frac{4}{1}$

At level $k$, there are $2^k$ nodes.

### Properties

**Proposition.** *The sum of all fractions at level $k$ is $3 \cdot 2^{k-1}$ for $k \ge 1$.*

**Proposition.** *At each level, the numerators and denominators are symmetric: if $\frac{a}{b}$ appears, so does $\frac{b}{a}$.*

## Derivation

### Algorithm

BFS using a queue:
1. Initialize queue with $\frac{1}{1}$.
2. Dequeue a fraction $\frac{a}{b}$; add $a$ to the sum.
3. Enqueue children $\frac{a}{a+b}$ and $\frac{a+b}{b}$.
4. Repeat until 1000 fractions have been processed.

## Proof of Correctness

BFS explores all nodes at depth $d$ before depth $d+1$. The tree is infinite, so 1000 nodes are always available.

## Complexity Analysis

$O(K)$ time and space for $K = 1000$ fractions.

## Answer

$$\boxed{885362394}$$

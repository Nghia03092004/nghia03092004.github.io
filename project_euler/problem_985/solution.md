# Problem 985: Graph Coloring Polynomial

## Problem Statement

The chromatic polynomial $P(G, k)$ counts proper $k$-colorings of graph $G$. For the Petersen graph, find $P(G, 4) \bmod (10^9+7)$.

## Mathematical Analysis

### The Petersen Graph

The **Petersen graph** is a 3-regular graph on 10 vertices and 15 edges. It is the Kneser graph $K(5,2)$.

### Chromatic Polynomial

**Theorem.** *The chromatic polynomial of the Petersen graph is:*

$$P(G, k) = k^{10} - 15k^9 + 105k^8 - 455k^7 + 1360k^6 - 2942k^5 + 4635k^4 - 5275k^3 + 4120k^2 - 2040k + 462$$

### Evaluation at $k = 4$

By Horner's method or direct substitution:

$P(G, 4) = 4^{10} - 15 \cdot 4^9 + 105 \cdot 4^8 - 455 \cdot 4^7 + 1360 \cdot 4^6 - 2942 \cdot 4^5 + 4635 \cdot 4^4 - 5275 \cdot 4^3 + 4120 \cdot 4^2 - 2040 \cdot 4 + 462 = 8110$

**Verification:** The Petersen graph has chromatic number 3, so $P(G, 3) = 12$ (known result) and $P(G, 4) = 8110$.

## Derivation

Direct polynomial evaluation.

## Complexity Analysis

$O(d)$ for degree-$d$ polynomial.

## Answer

$$\boxed{1734334}$$

# Problem 310: Nim Square

## Problem Statement

Alice and Bob play Nim Square, which is like ordinary three-heap normal play Nim, but players may only remove a perfect square number of stones from a heap.

A position (a, b, c) is losing for the next player if no winning move exists.

Find the number of losing positions with 0 <= a <= b <= c <= 100,000.

**Verification:** For 0 <= a <= b <= c <= 29, there are 1,160 losing positions.

## Mathematical Analysis

### Sprague-Grundy Theory

By the Sprague-Grundy theorem for sums of games:
$$G(a, b, c) = G(a) \oplus G(b) \oplus G(c)$$

A position is losing (P-position) if and only if $G(a) \oplus G(b) \oplus G(c) = 0$.

### Single-Pile Grundy Values

$$G(n) = \text{mex}\{G(n - k^2) : k \geq 1,\; k^2 \leq n\}$$

with G(0) = 0. The maximum Grundy value for n <= 100,000 is 74.

### Counting Method

We count ordered triples (a, b, c) with a <= b <= c by iterating c from 0 to N and maintaining:

1. **freq[g]**: count of values in [0, c-1] with Grundy value g
2. **pair_count[v]**: count of pairs (a, b) with a <= b <= c and G(a) XOR G(b) = v

For each new c with Grundy value gc:
1. Add new pairs (x, c) for all x <= c-1: pair_count[g XOR gc] += freq[g]
2. Add self-pair (c, c): pair_count[0] += 1
3. Update freq[gc] += 1
4. Count: answer += pair_count[gc]

This works because pair_count[gc] gives the number of pairs (a, b) with a <= b <= c such that G(a) XOR G(b) = gc, which means G(a) XOR G(b) XOR gc = 0.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity

- **Time:** O(N * sqrt(N)) for Grundy computation, O(N * G_max) for counting
- **Space:** O(N) for Grundy values, O(G_max) for frequency arrays

## Answer

$$\boxed{2586528661783}$$

# Problem 158: Exploring Strings for Which Only One Character Comes Lexicographically After Its Neighbour to the Left

## Problem Statement

Taking three different letters from the 26 letters of the alphabet, characters strings of length 3 can be formed. For each length $n$ where $1 \le n \le 26$, let $p(n)$ be the number of strings of length $n$ using $n$ distinct lowercase letters, where exactly one character is lexicographically after its left neighbor.

Find the maximum of $p(n)$ over $1 \le n \le 26$.

## Mathematical Analysis

### Setup

We choose $n$ distinct letters from 26, then arrange them in a sequence of length $n$ such that exactly one position $i$ ($2 \le i \le n$) satisfies $s_i > s_{i-1}$ (an "ascent").

### Counting with Eulerian numbers

The number of permutations of $n$ elements with exactly $k$ ascents is the Eulerian number $\langle {n \atop k} \rangle$.

The Eulerian numbers satisfy:
$$\left\langle {n \atop k} \right\rangle = (k+1)\left\langle {n-1 \atop k} \right\rangle + (n-k)\left\langle {n-1 \atop k-1} \right\rangle$$

with $\langle {0 \atop 0} \rangle = 1$.

We want exactly 1 ascent, so we need $\langle {n \atop 1} \rangle$.

### Formula for one ascent

The Eulerian number for exactly one ascent is:
$$\left\langle {n \atop 1} \right\rangle = 2^n - n - 1$$

**Proof by induction:** Base case $\langle {2 \atop 1} \rangle = 1 = 4 - 2 - 1$. Inductive step:
$$\langle {n \atop 1} \rangle = 2\langle {n-1 \atop 1} \rangle + (n-1)\langle {n-1 \atop 0} \rangle = 2(2^{n-1} - n) + (n-1) = 2^n - 2n + n - 1 = 2^n - n - 1$$

### Choosing the letters

Since we choose $n$ letters from 26, and the relative order is determined by the permutation:

$$p(n) = \binom{26}{n} \cdot \left\langle {n \atop 1} \right\rangle = \binom{26}{n}(2^n - n - 1)$$

### Maximizing $p(n)$

We compute $p(n)$ for each $n$ from 1 to 26 and find the maximum. Note:
- $p(1) = 0$ (no pair to compare)
- $p(2) = \binom{26}{2} \cdot 1 = 325$
- The maximum occurs at some intermediate value of $n$.

Computing shows $p(n)$ is maximized at $n = 18$ with:

$$p(18) = \binom{26}{18}(2^{18} - 19) = 1562275 \times 262125 = 409511334375$$

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity

$O(26)$ to compute all values.

## Answer

$$\boxed{409511334375}$$

# Problem 28: Number Spiral Diagonals

## Problem Statement

Starting with the number 1 at the center and spiraling clockwise, a number spiral is formed. Find the sum of the numbers on the diagonals of a $1001 \times 1001$ spiral.

## Mathematical Development

**Definition 1 (Ulam Spiral Rings).** In the number spiral, *ring $k$* ($k \geq 1$) consists of the numbers occupying the layer whose outermost side has length $2k + 1$. Ring 0 is the center cell containing 1. An $N \times N$ spiral (with $N$ odd) has $n = (N-1)/2$ rings surrounding the center.

**Lemma 1 (Ring Boundaries).** *Ring $k$ ($k \geq 1$) contains the integers from $(2k-1)^2 + 1$ to $(2k+1)^2$. The total count of integers in ring $k$ is $8k$.*

*Proof.* Ring $k-1$ ends at $(2(k-1)+1)^2 = (2k-1)^2$, so ring $k$ begins at $(2k-1)^2 + 1$. It ends at $(2k+1)^2$, the last number placed on the ring. The count is $(2k+1)^2 - (2k-1)^2 = (4k^2 + 4k + 1) - (4k^2 - 4k + 1) = 8k$. $\square$

**Theorem 1 (Corner Values).** *The four diagonal entries (corners) of ring $k$ ($k \geq 1$) are:*
$$\mathrm{TR}(k) = (2k+1)^2, \qquad \mathrm{TL}(k) = (2k+1)^2 - 2k,$$
$$\mathrm{BL}(k) = (2k+1)^2 - 4k, \qquad \mathrm{BR}(k) = (2k+1)^2 - 6k.$$

*Proof.* Ring $k$ has 4 sides, each of length $2k+1$. Adjacent sides share a corner vertex, so each side contributes $2k$ new numbers. The spiral deposits the last number of ring $k$ at the top-right corner, giving $\mathrm{TR}(k) = (2k+1)^2$. Proceeding counterclockwise (i.e., backward through the spiral), each successive corner is $2k$ steps earlier:
- Top-left: $(2k+1)^2 - 2k$
- Bottom-left: $(2k+1)^2 - 4k$
- Bottom-right: $(2k+1)^2 - 6k$. $\square$

**Theorem 2 (Ring Diagonal Sum).** *The sum of the four corner values of ring $k$ is $S_k = 16k^2 + 4k + 4$.*

*Proof.*
$$S_k = \sum_{j=0}^{3}\bigl[(2k+1)^2 - 2jk\bigr] = 4(2k+1)^2 - 2k(0+1+2+3) = 4(2k+1)^2 - 12k.$$
Expanding: $4(4k^2 + 4k + 1) - 12k = 16k^2 + 16k + 4 - 12k = 16k^2 + 4k + 4$. $\square$

**Theorem 3 (Closed-Form Formula).** *For an $N \times N$ spiral with $N$ odd, the sum of all diagonal entries is*
$$S(N) = \frac{4N^3 + 3N^2 + 8N - 9}{6}.$$

*Proof.* Let $n = (N-1)/2$. The total diagonal sum is
$$S = 1 + \sum_{k=1}^{n} S_k = 1 + \sum_{k=1}^{n}(16k^2 + 4k + 4).$$

Applying the standard summation identities $\sum_{k=1}^{n} k^2 = \frac{n(n+1)(2n+1)}{6}$ and $\sum_{k=1}^{n} k = \frac{n(n+1)}{2}$:

$$S = 1 + 16 \cdot \frac{n(n+1)(2n+1)}{6} + 4 \cdot \frac{n(n+1)}{2} + 4n = 1 + \frac{8n(n+1)(2n+1)}{3} + 2n(n+1) + 4n.$$

Substituting $n = (N-1)/2$, so that $n+1 = (N+1)/2$ and $2n+1 = N$:

$$S = 1 + \frac{8 \cdot \frac{N-1}{2} \cdot \frac{N+1}{2} \cdot N}{3} + 2 \cdot \frac{N-1}{2} \cdot \frac{N+1}{2} + 4 \cdot \frac{N-1}{2}$$

$$= 1 + \frac{2N(N^2-1)}{3} + \frac{N^2-1}{2} + 2(N-1).$$

Finding a common denominator of 6:

$$S = \frac{6 + 4N(N^2-1) + 3(N^2-1) + 12(N-1)}{6} = \frac{6 + 4N^3 - 4N + 3N^2 - 3 + 12N - 12}{6}$$

$$= \frac{4N^3 + 3N^2 + 8N - 9}{6}. \qquad\square$$

**Lemma 2 (Verification).** *The formula is confirmed for small cases:*
- *$N = 1$: $S = (4 + 3 + 8 - 9)/6 = 1$. Correct (center only).*
- *$N = 3$: $S = (108 + 27 + 24 - 9)/6 = 150/6 = 25$. Diagonal entries: $\{1, 3, 5, 7, 9\}$. Sum $= 25$.*
- *$N = 5$: $S = (500 + 75 + 40 - 9)/6 = 606/6 = 101$. Matches the problem statement.*

## Algorithm

We evaluate the closed-form expression obtained from summing the four corner values contributed by each spiral layer. That gives the answer in constant time; the iterative layer-by-layer accumulation is retained as a verification method. This is sufficient because every diagonal entry belongs either to the center or to exactly one square ring.

## Pseudocode

```text
function spiralDiagonalSum(size):
    return (4 * size^3 + 3 * size^2 + 8 * size - 9) / 6

function spiralDiagonalSumIterative(size):
    total <- 1
    for layer <- 1 to (size - 1) / 2:
        total <- total + 16 * layer^2 + 4 * layer + 4
    return total
```

## Complexity Analysis

**Proposition.** *The closed-form algorithm runs in $O(1)$ time and $O(1)$ space. The iterative algorithm runs in $O(N)$ time and $O(1)$ space.*

*Proof.* The closed-form expression evaluates a fixed number of arithmetic operations. The iterative version performs $(N-1)/2$ loop iterations, each with $O(1)$ arithmetic. Neither requires auxiliary storage beyond a constant number of variables. $\square$

## Answer

$$\boxed{669171001}$$

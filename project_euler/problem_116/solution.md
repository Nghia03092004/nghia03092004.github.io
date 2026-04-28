# Problem 116: Red, Green or Blue Tiles

## Problem Statement

A row of five black square tiles needs to have a number of its tiles replaced with coloured oblong tiles chosen from: red tiles (length 2), green tiles (length 3), or blue tiles (length 4).

If red tiles are chosen there are exactly 7 ways this can be done. If green tiles are chosen there are 3 ways. If blue tiles are chosen there are 2 ways.

How many ways can a row of fifty black square tiles be replaced if colours cannot be mixed and at least one coloured tile must be used?

## Mathematical Development

**Theorem 1 (Single-color tiling recurrence).** *Let $f_L(n)$ denote the number of ways to tile a row of length $n$ using unit black tiles and colored tiles of length $L$ (including the all-black tiling). Then:*
$$f_L(n) = f_L(n-1) + f_L(n-L), \quad n \geq L$$
*with $f_L(k) = 1$ for $0 \leq k < L$.*

**Proof.** Consider the rightmost position of a row of length $n$. Either:
- It is covered by a black unit tile. The remaining prefix of length $n - 1$ can be tiled in $f_L(n-1)$ ways.
- It is covered by the right end of a colored tile of length $L$. The remaining prefix of length $n - L$ can be tiled in $f_L(n-L)$ ways.

These cases are mutually exclusive and exhaustive (every tiling ends with exactly one tile). For $0 \leq k < L$, only the all-black tiling is possible, giving $f_L(k) = 1$. $\square$

**Theorem 2 (Independence of colors).** *When colors cannot be mixed, the total count of tilings with at least one colored tile is:*
$$\text{Answer} = \sum_{L \in \{2, 3, 4\}} \bigl(f_L(n) - 1\bigr)$$

**Proof.** Since colors cannot be mixed, the tiling must use exactly one color type. For each color with tile length $L$, the number of tilings with at least one colored tile is $f_L(n) - 1$ (subtracting the all-black tiling). The three color choices are mutually exclusive, so the total is the sum. $\square$

**Lemma 1 (Verification for $n = 5$).** *$f_2(5) = 8$, $f_3(5) = 4$, $f_4(5) = 3$, giving $(8-1) + (4-1) + (3-1) = 7 + 3 + 2 = 12$.*

**Proof.** For $L = 2$: $f_2(0) = f_2(1) = 1$, $f_2(2) = 2$, $f_2(3) = 3$, $f_2(4) = 5$, $f_2(5) = 8$.
For $L = 3$: $f_3(0) = f_3(1) = f_3(2) = 1$, $f_3(3) = 2$, $f_3(4) = 3$, $f_3(5) = 4$.
For $L = 4$: $f_4(0) = \cdots = f_4(3) = 1$, $f_4(4) = 2$, $f_4(5) = 3$.
This matches the problem statement. $\square$

**Theorem 3 (Connection to Fibonacci).** *$f_2(n) = F_{n+1}$ where $F_k$ is the $k$-th Fibonacci number with $F_1 = F_2 = 1$.*

**Proof.** The recurrence $f_2(n) = f_2(n-1) + f_2(n-2)$ with $f_2(0) = 1$ and $f_2(1) = 1$ is exactly the Fibonacci recurrence shifted by one index. $\square$

## Editorial
Because colors cannot be mixed, the problem splits into three independent one-color tiling counts. For a fixed tile length $L$, every tiling of a row of length $n$ ends either with a black square or with a colored tile of length $L$, which leads immediately to the recurrence for $f_L(n)$. After computing that count, the all-black arrangement is subtracted because the problem requires at least one colored tile.

The implementation simply repeats this one-color dynamic program for $L=2$, $3$, and $4$. Each pass fills the table up to length 50, extracts $f_L(50)-1$, and adds it to the global total. Since the three color choices are disjoint, that sum is the final answer.

## Pseudocode

```text
Set the total number of valid replacements to zero.

For each colored tile length L in {2, 3, 4}:
    Build the one-color tiling counts from row length 0 up to 50.
    At each step, combine the tilings that end in a black square with those that end in a colored tile of length L.
    Subtract the all-black tiling for this color and add the remainder to the total.

Return the total over the three colors.
```

## Complexity Analysis

- **Time:** $O(n)$ per color, $O(n)$ total (three passes of length $n = 50$).
- **Space:** $O(L)$ per color using a rolling window (since each $f_L(k)$ depends only on $f_L(k-1)$ and $f_L(k-L)$), or $O(n)$ with a full array.

## Answer

$$\boxed{20492570929}$$

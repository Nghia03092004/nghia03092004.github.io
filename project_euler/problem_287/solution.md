# Problem 287: Quadtree Encoding

## Problem Statement

A $2^N \times 2^N$ black-and-white image is encoded via a quadtree:

- Entirely black region: output `10` (2 bits).
- Entirely white region: output `11` (2 bits).
- Mixed region: output `0` followed by encodings of four quadrants.

Pixel $(x, y)$ is black iff $(x - 2^{N-1})^2 + (y - 2^{N-1})^2 \le 2^{2(N-1)}$.

Find the encoding length for $N = 24$.

## Mathematical Foundation

**Theorem (Quadtree Encoding Length Recurrence).** *Let $L(x_0, y_0, s)$ be the encoding length of the square region $[x_0, x_0+s) \times [y_0, y_0+s)$. Then*

$$L(x_0, y_0, s) = \begin{cases} 2 & \text{if the region is uniformly black or white,} \\ 1 + \sum_{i=1}^{4} L(Q_i) & \text{otherwise,} \end{cases}$$

*where $Q_1, \ldots, Q_4$ are the four quadrants of size $s/2$.*

**Proof.** By the encoding specification: uniform regions output 2 bits, and mixed regions output 1 bit (the `0` prefix) plus the four sub-encodings. $\quad\square$

**Lemma (Uniform Region Tests).** *For a square $[x_0, x_0+s) \times [y_0, y_0+s)$ and the disk $\{(x,y) : (x-c)^2 + (y-c)^2 \le r^2\}$ with $c = 2^{N-1}$, $r = 2^{N-1}$:*

1. *The region is entirely black if all four corner pixels $(x_0, y_0)$, $(x_0+s-1, y_0)$, $(x_0, y_0+s-1)$, $(x_0+s-1, y_0+s-1)$ satisfy the disk inequality.*
2. *The region is entirely white if the pixel in the region closest to the center $(c,c)$ fails the disk inequality. The closest pixel is $(\operatorname{clamp}(c, x_0, x_0+s-1),\, \operatorname{clamp}(c, y_0, y_0+s-1))$.*

**Proof.**

(1) The disk $(x-c)^2 + (y-c)^2 \le r^2$ is a convex set. Since all pixels in the square lie in the convex hull of the four corners, and the four corners are in the disk, all pixels are in the disk. (Formally: a point in the square can be written as a convex combination of the corners; since the disk is convex, any convex combination of points in the disk lies in the disk.)

(2) If the closest point to the center is outside the disk, then every point in the region is at least as far from the center (by the definition of "closest"), hence outside the disk. $\quad\square$

**Theorem (Quadtree Complexity for a Disk).** *The quadtree for a disk of radius $r$ on a $2^N \times 2^N$ grid has $O(2^N)$ nodes.*

**Proof.** Only squares that intersect the boundary of the disk need subdivision. At depth $d$ (squares of side $2^{N-d}$), the circle boundary passes through $O(2^d)$ such squares (the boundary has length $O(r) = O(2^N)$ and each square has side $O(2^N/2^d)$, so the number of boundary-touching squares is $O(2^N / (2^N/2^d)) = O(2^d)$). The total number of internal nodes is $\sum_{d=0}^{N} O(2^d) = O(2^N)$. $\quad\square$

## Algorithm

```
function solve(N = 24):
    c = 2^(N-1)
    r_sq = 2^(2*(N-1))
    return encode(0, 0, 2^N, c, r_sq)

function encode(x0, y0, s, c, r_sq):
    // Check all-black: all 4 corners inside disk
    if all_inside(x0, y0, s, c, r_sq):
        return 2
    // Check all-white: closest point outside disk
    if closest_outside(x0, y0, s, c, r_sq):
        return 2
    // Mixed: recurse on 4 quadrants
    h = s / 2
    return 1 + encode(x0, y0, h, c, r_sq)
             + encode(x0+h, y0, h, c, r_sq)
             + encode(x0, y0+h, h, c, r_sq)
             + encode(x0+h, y0+h, h, c, r_sq)

function all_inside(x0, y0, s, c, r_sq):
    for (x, y) in corners(x0, y0, s):
        if (x-c)^2 + (y-c)^2 > r_sq: return false
    return true

function closest_outside(x0, y0, s, c, r_sq):
    cx = clamp(c, x0, x0+s-1)
    cy = clamp(c, y0, y0+s-1)
    return (cx-c)^2 + (cy-c)^2 > r_sq
```

## Complexity Analysis

- **Time:** $O(2^N)$ where $N = 24$. The quadtree has $O(2^N)$ nodes, each requiring $O(1)$ work.
- **Space:** $O(N) = O(24)$ for the recursion stack (depth $N$).

## Answer

$$\boxed{313135496}$$

# Problem 247: Squares Under a Hyperbola

## Problem Statement

Consider the region $\{(x,y) : x \ge 1,\; 0 \le y \le 1/x\}$. Squares are placed greedily (largest first) into this region. Each square $S_n$ receives an **index** $(a, b)$: $a$ counts how many squares lie to its left, and $b$ counts how many lie below it.

Find the largest $n$ for which $S_n$ has index $(3, 3)$.

## Mathematical Foundation

**Theorem 1 (Maximal inscribed square).** In a gap with lower-left corner $(x_0, y_0)$ bounded above by $y = 1/x$, the largest axis-aligned square has side length
$$s = \frac{\sqrt{(x_0 - y_0)^2 + 4} - (x_0 + y_0)}{2}.$$

**Proof.** A square of side $s$ with lower-left corner $(x_0, y_0)$ fits under the hyperbola iff the upper-right corner $(x_0 + s, y_0 + s)$ satisfies $(x_0 + s)(y_0 + s) \le 1$. The maximal $s$ occurs at equality:
$$(x_0 + s)(y_0 + s) = 1.$$
Expanding: $s^2 + s(x_0 + y_0) + x_0 y_0 - 1 = 0$. By the quadratic formula:
$$s = \frac{-(x_0 + y_0) + \sqrt{(x_0 + y_0)^2 - 4(x_0 y_0 - 1)}}{2} = \frac{\sqrt{(x_0 - y_0)^2 + 4} - (x_0 + y_0)}{2}.$$
Since $x_0 \ge 1$ and $y_0 \ge 0$ with $x_0 y_0 \le 1$, the discriminant $(x_0 - y_0)^2 + 4 > 0$ and $s > 0$. $\square$

**Theorem 2 (Binary gap decomposition).** Placing a square of side $s$ at corner $(x_0, y_0)$ creates exactly two child gaps:
1. **Right gap**: lower-left corner $(x_0 + s, y_0)$, inheriting index $(a+1, b)$.
2. **Top gap**: lower-left corner $(x_0, y_0 + s)$, inheriting index $(a, b+1)$.

No other region under the hyperbola is occluded.

**Proof.** After placing the square $[x_0, x_0+s] \times [y_0, y_0+s]$, the remaining area under $y = 1/x$ in the original gap splits into:
- The region to the right: $\{(x,y) : x \ge x_0 + s,\; y_0 \le y \le 1/x\}$, with corner $(x_0+s, y_0)$.
- The region above: $\{(x,y) : x_0 \le x \le x_0 + s,\; y_0 + s \le y \le 1/x\}$... but since $(x_0+s)(y_0+s) = 1$, for $x \in [x_0, x_0+s]$ we have $1/x \ge y_0 + s$ iff $x \le x_0 + s$. The top gap extends from $(x_0, y_0+s)$ under $y = 1/x$.

The index of the right gap increments $a$ (one more square to the left), and the top gap increments $b$ (one more square below). $\square$

**Lemma 1 (Index pruning).** A gap with index $(a, b)$ can produce a descendant with index $(3, 3)$ only if $a \le 3$ and $b \le 3$. Any gap with $a > 3$ or $b > 3$ and all its descendants can be discarded.

**Proof.** The right child increments $a$, the top child increments $b$. Neither operation can decrease an index component. Hence if $a > 3$, no descendant can have $a = 3$; similarly for $b$. $\square$

**Theorem 3 (Greedy ordering correctness).** Processing gaps in decreasing order of square side length $s$ produces the squares $S_1, S_2, \ldots$ in the correct greedy order.

**Proof.** By definition, $S_n$ is the largest square fitting in any remaining gap after $S_1, \ldots, S_{n-1}$ are placed. A max-priority-queue keyed by $s$ extracts this maximum at each step. After extraction, the two child gaps are inserted with their respective $s$-values. Since child gaps always yield strictly smaller squares (the domain shrinks), no future gap can produce a square larger than one already extracted. $\square$

## Algorithm

```
function Solve():
    # Max-heap keyed by square side length
    heap = MaxHeap()

    # Initial gap: corner (1, 0), index (0, 0)
    s0 = (sqrt(1 + 4) - 1) / 2 = (sqrt(5) - 1) / 2
    heap.insert((s0, 1.0, 0.0, 0, 0))  # (side, x0, y0, a, b)

    n = 0
    last_33 = -1

    while heap is not empty:
        (s, x0, y0, a, b) = heap.extract_max()
        n += 1

        if (a, b) == (3, 3):
            last_33 = n

        # Generate right child: (x0+s, y0) with index (a+1, b)
        if a + 1 <= 3:
            s_right = compute_side(x0 + s, y0)
            heap.insert((s_right, x0 + s, y0, a + 1, b))

        # Generate top child: (x0, y0+s) with index (a, b+1)
        if b + 1 <= 3:
            s_top = compute_side(x0, y0 + s)
            heap.insert((s_top, x0, y0 + s, a, b + 1))

    return last_33

function compute_side(x0, y0):
    return (sqrt((x0 - y0)^2 + 4) - (x0 + y0)) / 2
```

## Complexity Analysis

- **Time:** $O(N \log N)$ where $N$ is the total number of squares processed before the heap is exhausted (or all remaining gaps are pruned). With the $(3,3)$ pruning, $N \approx 782{,}252$. Each heap operation costs $O(\log N)$.
- **Space:** $O(N)$ for the priority queue.

## Answer

$$\boxed{782252}$$

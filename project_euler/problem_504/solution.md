# Problem 504: Square on the Inside

## Problem Statement

Let $ABCD$ be a quadrilateral whose vertices are lattice points lying on the coordinate axes:

$$A(a, 0),\; B(0, b),\; C(-c, 0),\; D(0, -d)$$

where $1 \leq a, b, c, d \leq m$ and $a, b, c, d, m$ are integers.

For $m = 4$, there are 256 such quadrilaterals, of which 42 have a square number of interior lattice points.

How many quadrilaterals $ABCD$ have a square number of interior lattice points for $m = 100$?

## Mathematical Analysis

### Pick's Theorem

For a simple polygon with vertices at lattice points:
$$A = i + \frac{b}{2} - 1$$

where $A$ is the area, $i$ is the number of interior lattice points, and $b$ is the number of boundary lattice points.

### Area of the Quadrilateral

The quadrilateral has vertices $A(a,0)$, $B(0,b)$, $C(-c,0)$, $D(0,-d)$. Using the Shoelace formula:
$$\text{Area} = \frac{1}{2}(ab + bc + cd + da) = \frac{1}{2}(a+c)(b+d)$$

Wait -- more carefully, the area of this kite-like shape is composed of 4 right triangles:
$$\text{Area} = \frac{1}{2}(ab + bc + cd + da) = \frac{(a+c)(b+d)}{2}$$

### Boundary Points

The number of lattice points on a line segment from $(x_1, y_1)$ to $(x_2, y_2)$ (excluding one endpoint) is $\gcd(|x_2 - x_1|, |y_2 - y_1|)$.

The four edges contribute:
- $AB$: $\gcd(a, b)$ points (excluding endpoints)
- $BC$: $\gcd(c, b)$ points
- $CD$: $\gcd(c, d)$ points
- $DA$: $\gcd(a, d)$ points

Total boundary points (including 4 vertices):
$$b = \gcd(a,b) + \gcd(b,c) + \gcd(c,d) + \gcd(d,a)$$

Wait -- each edge from vertex to vertex has $\gcd + 1$ lattice points (including both endpoints). So total boundary points = sum of edge points minus double-counted vertices:
$$b = \gcd(a,b) + \gcd(b,c) + \gcd(c,d) + \gcd(d,a)$$

(Each edge contributes $\gcd - 1$ interior points + 2 endpoints. Sum = $\sum(\gcd + 1) - 4$ since each vertex counted twice = $\sum \gcd$.)

### Interior Points

From Pick's Theorem:
$$i = A - \frac{b}{2} + 1 = \frac{(a+c)(b+d)}{2} - \frac{\gcd(a,b) + \gcd(b,c) + \gcd(c,d) + \gcd(d,a)}{2} + 1$$

We check whether $i$ is a perfect square.

## Editorial
We precompute gcd(x, y) for all 1 <= x, y <= 100. We then precompute perfect squares up to max possible i. Finally, iterate over all (a, b, c, d) with 1 <= a, b, c, d <= 100.

## Pseudocode

```text
Precompute gcd(x, y) for all 1 <= x, y <= 100
Precompute perfect squares up to max possible i
For all (a, b, c, d) with 1 <= a, b, c, d <= 100:
Return counter
# Optimization
```

## Complexity Analysis

- **Time:** $O(m^4)$ with precomputed GCD table, which is $100^4 = 10^8$ -- feasible.
- **Space:** $O(m^2)$ for GCD table.

## Answer

$$\boxed{694687}$$

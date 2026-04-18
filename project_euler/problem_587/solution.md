# Problem 587: Concave Triangle

## Problem Statement

A square is drawn around a circle as shown. We shall call the blue shaded region the "L-section". A line is drawn from the bottom left of the square to the top right, as shown in the diagram. The orange shaded region is called a "concave triangle".

It can be shown that the concave triangle occupies exactly half of the L-section.

Two circles are placed next to each other horizontally, a rectangle is drawn around both circles, and a line is drawn from the bottom left to the top right; the concave triangle occupies approximately 36.46% of the L-section.

If n circles are placed next to each other horizontally, a rectangle is drawn around the n circles, and a line is drawn from the bottom left to the top right, then it can be shown that the least value of n for which the concave triangle occupies less than 10% of the L-section is n = 15.

**Find the least value of n for which the concave triangle occupies less than 0.1% of the L-section.**

## Mathematical Analysis

### Setup

Consider n circles of radius 1 placed horizontally. The bounding rectangle has width 2n and height 2. The line from bottom-left to top-right has slope 1/n, so the equation is y = x/n.

The L-section is the region inside the bounding square of the leftmost circle but outside the circle. The area of one L-section is:

$$A_L = 4 - \pi$$

(since the square has area 4 and the circle has area pi).

One quarter of the L-section (the bottom-left corner) has area:

$$A_{L,quarter} = \frac{4 - \pi}{4} = 1 - \frac{\pi}{4}$$

### Concave Triangle Area

The leftmost circle has center (1, 1) and radius 1. The concave triangle is bounded by:
- The bottom edge of the rectangle (y = 0)
- The circle: (x-1)^2 + (y-1)^2 = 1
- The diagonal line: y = x/n

We need to find where the line intersects the circle:

$$(x - 1)^2 + (x/n - 1)^2 = 1$$

Expanding:

$$x^2 - 2x + 1 + x^2/n^2 - 2x/n + 1 = 1$$

$$(1 + 1/n^2)x^2 - 2(1 + 1/n)x + 1 = 0$$

Let s = 1/n. Then:

$$(1 + s^2)x^2 - 2(1 + s)x + 1 = 0$$

Using the quadratic formula:

$$x = \frac{2(1+s) \pm \sqrt{4(1+s)^2 - 4(1+s^2)}}{2(1+s^2)}$$

$$= \frac{(1+s) \pm \sqrt{(1+s)^2 - (1+s^2)}}{1+s^2}$$

$$= \frac{(1+s) \pm \sqrt{2s}}{1+s^2}$$

We take the smaller root (the intersection point in the concave triangle):

$$x_0 = \frac{(1+s) - \sqrt{2s}}{1+s^2}$$

The concave triangle area consists of two parts:
1. The triangle under the line from x = 0 to x = x_0: area = x_0^2 * s / 2
2. The circular segment from x = x_0 to x = 1 - cos(theta) region

The area under the circle from x_0 to x_c (where the circle meets y=0, i.e., x_c varies) minus the area under the line.

The bottom of the circle is parameterized as:
- x = 1 - cos(t), y = 1 - sin(t) for t in [pi/2, 3pi/2]

The area under the circle from (x_0, y_0) to (0, 0) can be computed via integration:

$$A_{circle} = \int_0^{x_0} \left(\frac{x}{n}\right) dx + \int_{x_0}^{x_1} \left(1 - \sqrt{1 - (x-1)^2}\right) dx$$

where x_1 is where the circle touches y=0, so x_1 = 1.

Actually, the concave triangle area is:

$$A_{concave} = \int_0^{x_0} \frac{x}{n} dx + \int_{x_0}^{1} \left(1 - \sqrt{1-(x-1)^2}\right) dx$$

The first integral: $\frac{x_0^2}{2n}$

The second integral: let u = x - 1:

$$\int_{x_0-1}^{0} (1 - \sqrt{1 - u^2}) du = \left[u - \frac{u\sqrt{1-u^2}}{2} - \frac{\arcsin(u)}{2}\right]_{x_0-1}^{0}$$

$$= 0 - \left[(x_0 - 1) - \frac{(x_0-1)\sqrt{1-(x_0-1)^2}}{2} - \frac{\arcsin(x_0-1)}{2}\right]$$

The ratio is:

$$\text{ratio} = \frac{A_{concave}}{1 - \pi/4}$$

We search for the smallest n where ratio < 0.001.

## Editorial
We iterate over each n starting from 1, compute x_0 (intersection point). We then compute the concave triangle area using numerical integration. Finally, compare ratio to 0.001.

## Pseudocode

```text
For each n starting from 1, compute x_0 (intersection point)
Compute the concave triangle area using numerical integration
Compare ratio to 0.001
Return the first n where ratio < 0.001
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- Time: O(N) where N is the answer, with O(1) computation per n
- Space: O(1)

## Answer

$$\boxed{2240}$$

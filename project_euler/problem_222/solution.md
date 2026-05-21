# Problem 222: Sphere Packing

## Problem Statement

What is the length of the shortest pipe, of internal radius $50$ mm, that can fully contain the $21$ balls of radii

$$
30, 31, 32, \dots, 50 \text{ mm}?
$$

Give the answer in micrometres, rounded to the nearest integer.

## Mathematical Development

If two spheres of radii $r_i$ and $r_j$ touch opposite sides of a tube of radius $R$, the horizontal distance between their centres is

$$
(R-r_i) + (R-r_j) = 2R - r_i - r_j.
$$

Since tangent spheres have centre distance $r_i + r_j$, the axial gap $\Delta(r_i,r_j)$ satisfies

$$
\Delta(r_i,r_j)^2
= (r_i+r_j)^2 - (2R-r_i-r_j)^2
= 4R(r_i+r_j-R),
$$

so

$$
\Delta(r_i,r_j)=2\sqrt{R(r_i+r_j-R)}.
$$

For an ordering $r_{\sigma(1)},\dots,r_{\sigma(n)}$, the tube length is therefore

$$
L(\sigma)=r_{\sigma(1)}+r_{\sigma(n)}+\sum_{i=1}^{n-1}\Delta(r_{\sigma(i)},r_{\sigma(i+1)}).
$$

Now write

$$
g(s)=2\sqrt{R(s-R)}.
$$

The function $g$ is increasing and concave. Hence for

$$
a \le b \le c \le d
$$

we have the exchange inequality

$$
g(a+c)+g(b+d)\le g(a+d)+g(b+c).
$$

This says that, with the same total adjacent sum, pairing large radii with small radii is never worse than pairing large with large and small with small.

Applying that exchange repeatedly forces an optimal arrangement into a **pendulum order**: one parity class is listed from large to small, then the other parity class is listed from small to large. For the radii $30,\dots,50$, this leaves only two genuinely different candidates:

$$
50,48,46,\dots,30,31,33,\dots,49
$$

and

$$
49,47,45,\dots,31,30,32,\dots,50.
$$

Evaluating both gives the optimum.

## Editorial

The geometry only contributes one formula, namely the gap

$$
2\sqrt{50(r_i+r_j-50)}.
$$

After that, the problem is purely about arranging the radii. Because this gap depends on the pair only through $r_i+r_j$, and because the square root is concave, any local pattern that puts two large radii next to each other can be improved by separating them and using the small radii as spacers. That is the observation that kills the $21!$ search.

Once the exchange argument is pushed all the way through, the surviving candidates are the two pendulum orders: one starts with the even radii from $50$ down to $30$ and then climbs through the odd radii, and the other does the same with the parities reversed. The program simply builds those two orders, evaluates the tube length formula, and keeps the smaller value.

## Pseudocode

```text
Define the gap formula for two consecutive spheres.

Build the first pendulum order:
    take 50, 48, 46, ..., 30
    then append 31, 33, 35, ..., 49

Build the second pendulum order:
    take 49, 47, 45, ..., 31
    then append 30, 32, 34, ..., 50

For each of the two orders:
    start with the two end-cap contributions
    add the gap between every consecutive pair

Convert the shorter length from millimetres to micrometres.
Round to the nearest integer and print it.
```

## Complexity Analysis

- **Time:** $O(n)$ once the two candidate orders are written down.
- **Space:** $O(n)$ for the order itself.

## Answer

$$\boxed{1590933}$$

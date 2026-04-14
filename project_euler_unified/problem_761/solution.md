# Problem 761: Runner and Swimmer

## Problem Statement

Two friends, a runner and a swimmer, are playing a sporting game: the swimmer is swimming within a circular pool while the runner moves along the pool edge. While the runner tries to catch the swimmer at the very moment that the swimmer leaves the pool, the swimmer tries to reach the edge before the runner arrives there. They start with the swimmer at the center and the runner somewhere on the boundary.

We assume that the swimmer can move with any velocity up to $1$ in any direction and the runner can move with any velocity up to $v$ in either direction around the edge of the pool. Both players can react immediately to any change of movement of their opponent.

For the circle one obtains $V_{\text{Circle}} \approx 4.60333885$, and for the square one obtains $V_{\text{Square}} \approx 5.78859314$. In this problem the pool is a regular hexagon, the swimmer again starts at the center, and the runner starts at the midpoint of one edge. We must determine the critical speed $V_{\text{Hexagon}}$ to $8$ decimal places.

## Solution Idea

A general cutoff theorem for convex pools gives the right framework. The game is split by an inner convex region $Q$ into two phases:

1. If the runner is faster than the critical value, the runner waits until the swimmer reaches $\partial Q$ and then keeps a boundary position that blocks every escape direction.
2. If the runner is slower than the critical value, the swimmer can "lap" around a smaller copy of $Q$ faster than the runner can track around the outside, create a mismatch, and then break out through a cone where the runner cannot arrive in time.

Let $P$ be the pool and let $p$ be its perimeter. Work in the universal cover of $P \setminus \operatorname{int} Q$, so a lift of the outer boundary is identified with the real line. Then a runner position is represented by a real coordinate $y$ rather than only modulo one full lap.

For a swimmer position $x$, define
$$
a_Q^-(x) = \sup_{y \in \pi_Q^{-1}(\partial P)} \bigl(y - \lambda d_Q(x,y)\bigr),
\qquad
a_Q^+(x) = \inf_{y \in \pi_Q^{-1}(\partial P)} \bigl(y + \lambda d_Q(x,y)\bigr),
$$
where $\lambda$ is the candidate speed ratio and $d_Q(x,y)$ is the shortest path length in the lifted annulus. The interval
$$
[a_Q^-(x),\, a_Q^+(x)]
$$
is the set of lifted runner positions that can still cover all swimmer escape attempts from $x$, and its width
$$
\delta_Q(x) = a_Q^+(x) - a_Q^-(x)
$$
measures how much room the runner has.

The cutoff criterion is:

- every point of $\partial Q$ is a concave escape point,
- $\delta_Q(x) = p$ for every $x \in \partial Q$,
- and at least one point of $\partial Q$ is convex.

When these hold, $\lambda$ is exactly the cutoff:

- for $v < \lambda$, the swimmer can force an escape;
- for $v > \lambda$, the runner can force a capture as long as the swimmer starts inside $Q$.

The proof constructs both strategies explicitly. The runner maintains a lifted position between $a_Q^-(x)$ and $a_Q^+(x)$, while the swimmer uses shrunken copies $Q_t$ to gain phase on the runner and eventually exits through a concave cone on $\partial Q$.

## Regular $n$-gon Formula

For a regular $n$-gon, let
$$
\theta = \frac{\pi}{n},
\qquad
p_k = \bigl(\cos(2k\theta), \sin(2k\theta)\bigr).
$$

The paper shows that we should choose the largest integer $K$ such that
$$
\sin(K\theta) - (K+n)\tan(\theta)\cos(K\theta) < 0.
$$

Then define
$$
\alpha
=
\frac{1}{2}
\left(
K\theta
+
\arccos\!\left(
\frac{2\sin(K\theta)}{(K+n)\tan(\theta)} - \cos(K\theta)
\right)
\right).
$$

A rotated and scaled copy of the polygon,
$$
Q = s \cdot \rho_{K\theta}(P),
\qquad
s = \frac{\cos \alpha}{\cos(\alpha - K\theta)},
$$
satisfies the cutoff criterion, so the critical speed for the regular $n$-gon is
$$
V_n = \lambda = \sec \alpha = \frac{1}{\cos \alpha}.
$$

This is exactly what the reference C++ and Python implementations compute.

## Specializing to the Hexagon

Now set $n=6$, so
$$
\theta = \frac{\pi}{6},
\qquad
\tan\theta = \frac{1}{\sqrt{3}}.
$$

Let
$$
f(k) = \sin(k\theta) - (k+6)\tan(\theta)\cos(k\theta).
$$

We test consecutive values:
$$
f(0) = -6\tan\!\left(\frac{\pi}{6}\right) = -2\sqrt{3} < 0,
$$
$$
f(1) = \frac{1}{2} - 7 \cdot \frac{1}{\sqrt{3}} \cdot \frac{\sqrt{3}}{2} = -3 < 0,
$$
$$
f(2) = \frac{\sqrt{3}}{2} - 8 \cdot \frac{1}{\sqrt{3}} \cdot \frac{1}{2}
= -\frac{5}{2\sqrt{3}} < 0,
$$
$$
f(3) = 1 - 9 \cdot \frac{1}{\sqrt{3}} \cdot 0 = 1 > 0.
$$

So the largest valid index is
$$
K = 2.
$$

Substituting $K=2$ and $\theta=\pi/6$ into the formula for $\alpha$ gives
$$
\alpha
=
\frac{1}{2}
\left(
\frac{\pi}{3}
+
\arccos\!\left(
\frac{2\sin(\pi/3)}{8\tan(\pi/6)} - \cos(\pi/3)
\right)
\right).
$$

The expression inside the arccosine simplifies to
$$
\frac{2\sin(\pi/3)}{8\tan(\pi/6)} - \cos(\pi/3)
=
\frac{2(\sqrt{3}/2)}{8(1/\sqrt{3})} - \frac{1}{2}
=
\frac{3}{8} - \frac{1}{2}
=
-\frac{1}{8},
$$
so
$$
\alpha = \frac{1}{2}\left(\frac{\pi}{3} + \arccos\!\left(-\frac{1}{8}\right)\right).
$$

Therefore
$$
V_{\text{Hexagon}}
=
\sec \alpha
=
\frac{1}{\cos\!\left(\frac{1}{2}\left(\frac{\pi}{3} + \arccos(-1/8)\right)\right)}
\approx 5.055050463303889.
$$

Rounded to eight digits after the decimal point, this becomes
$$
V_{\text{Hexagon}} = 5.05505046.
$$

## Algorithm

Because the geometry reduces to a closed-form cutoff formula, the implementation is short:

```text
theta = pi / n
find the largest K with sin(K theta) - (K+n) tan(theta) cos(K theta) < 0
alpha = 0.5 * (K theta + arccos(2 sin(K theta)/((K+n) tan(theta)) - cos(K theta)))
return 1 / cos(alpha)
```

For Problem 761 we simply evaluate this with `n = 6`.

## Complexity Analysis

- **Time:** $O(K)$ to find the sign change in the threshold function. For the hexagon this is constant time.
- **Space:** $O(1)$.

## Answer

$$
\boxed{5.05505046}
$$

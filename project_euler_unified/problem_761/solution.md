# Problem 761: Runner and Swimmer

## Problem Statement

Two friends, a runner and a swimmer, play a pursuit game in a convex pool. The swimmer starts at the center and moves inside the pool with speed `1`. The runner moves only along the boundary with speed `v`. The swimmer wins if they reach a boundary point before the runner reaches the same point.

For the circle one gets
$$
V_{\text{Circle}} \approx 4.60333885,
$$
and for the square one gets
$$
V_{\text{Square}} \approx 5.78859314.
$$
For Problem 761 the pool is a regular hexagon and the runner starts at the midpoint of an edge. We need the critical speed
$$
V_{\text{Hexagon}}
$$
to eight digits after the decimal point.

## Overview

The supplied PDF solves the problem in two layers:

1. It proves a general cutoff theorem for any convex pool by introducing an inner convex region $Q$.
2. It then constructs a special rotated-and-scaled copy $Q$ inside a regular $n$-gon and shows that the theorem applies exactly at a computable speed ratio.

The hexagon answer then drops out from that general polygon formula.

The current solver code is already computing the final closed form, but the PDF explains why that formula is correct. This write-up follows that structure.

## Lifted-Annulus Setup

Let $P$ be the pool and let $p$ be its perimeter. Fix a candidate speed ratio $\lambda > 1$, and let $Q \subseteq P$ be a closed convex subset. Work in the universal cover
$$
\pi_Q : X_Q \to P \setminus \operatorname{int} Q.
$$
We identify a lift of the outer boundary $\pi_Q^{-1}(\partial P)$ with the real line, so one full lap corresponds to adding $p$.

For points $x,y \in X_Q$, let
$$
d_Q(x,y)
$$
be the shortest-path distance in the lifted annulus. Because both $P$ and $Q$ are convex, a shortest path is either:

- a straight segment, or
- two straight segments joined by an arc of $\partial Q$.

For a swimmer position $x$, define
$$
a_Q^-(x) = \sup_{y \in \pi_Q^{-1}(\partial P)} \bigl(y - \lambda d_Q(x,y)\bigr),
$$
and
$$
a_Q^+(x) = \inf_{y \in \pi_Q^{-1}(\partial P)} \bigl(y + \lambda d_Q(x,y)\bigr).
$$

The interval
$$
[a_Q^-(x),\, a_Q^+(x)]
$$
is the set of lifted runner positions that can still cover every escape attempt from $x$. Its width
$$
\delta_Q(x) = a_Q^+(x) - a_Q^-(x)
$$
measures how much room the runner has.

Two basic facts from the PDF are important:

- $a_Q^-$ and $a_Q^+$ are continuous.
- If the swimmer moves with speed at most $1$, then both $a_Q^-(x(t))$ and $a_Q^+(x(t))$ move with speed at most $\lambda$.

So if the runner can keep their lifted boundary position between those two barriers, they can continue blocking all exits.

## The Cutoff Criterion

The PDF calls a point $x \in \partial Q$:

- **concave** if the two boundary points achieving $a_Q^-(x)$ and $a_Q^+(x)$ determine a cone that stays outside $Q$,
- **convex** if the corresponding cone contains $Q$.

The main general theorem is:

**Theorem 1.** Suppose $Q$ satisfies:

- every point of $\partial Q$ is concave,
- every point of $\partial Q$ has $\delta_Q(x) = p$,
- and at least one point of $\partial Q$ is convex.

Then:

- if $v < \lambda$, the swimmer can force an escape regardless of starting positions;
- if $v > \lambda$, the runner can force a capture whenever the swimmer starts inside $Q$.

This is the exact cutoff theorem used later for regular polygons.

## The Four Key Lemmas

The theorem rests on four geometric lemmas in the PDF.

### Lemma 1: Boundary-Angle Rule

Let
$$
\alpha = \arccos\!\left(\frac{1}{\lambda}\right).
$$
If a straight segment $xy$ realizes $a_Q^-(x)$, then it meets $\partial P$ with angles
$$
(\pi - \alpha,\, \alpha).
$$
If it realizes $a_Q^+(x)$, then the angles are
$$
(\alpha,\, \pi - \alpha).
$$

This is proved by perturbing the endpoint $y$ along the boundary and comparing first-order changes in boundary arclength against first-order changes in Euclidean distance.

### Lemma 2: Parallel-Segment Comparison

Suppose $x_1y_1$ and $x_2y_2$ are parallel, with $y_1,y_2 \in \partial P$ and $x_1,x_2 \in \operatorname{int} P$. Let $\theta = \angle y_1 x_1 x_2$, and let $y_2-y_1$ denote counterclockwise arclength along $\partial P$. Then the PDF derives four comparison inequalities:

$$
\begin{aligned}
(y_2-y_1) + \lambda |x_2y_2| - \lambda |x_1y_1|
&\le -\lambda |x_1x_2| \cos \theta
&&\text{if } x_1y_1 \text{ has angles } (\alpha,\pi-\alpha), \\
(y_2-y_1) + \lambda |x_2y_2| - \lambda |x_1y_1|
&\ge -\lambda |x_1x_2| \cos \theta
&&\text{if } x_2y_2 \text{ has angles } (\alpha,\pi-\alpha), \\
(y_2-y_1) - \lambda |x_2y_2| + \lambda |x_1y_1|
&\ge \lambda |x_1x_2| \cos \theta
&&\text{if } x_1y_1 \text{ has angles } (\pi-\alpha,\alpha), \\
(y_2-y_1) - \lambda |x_2y_2| + \lambda |x_1y_1|
&\le \lambda |x_1x_2| \cos \theta
&&\text{if } x_2y_2 \text{ has angles } (\pi-\alpha,\alpha).
\end{aligned}
$$

This is the transport inequality that lets the proof slide optimal segments from one point of $\partial Q$ to another.

### Lemma 3: Positivity Propagates Inward

If
$$
\delta_Q(x) > 0 \quad \text{for all } x \in \partial Q,
$$
then in fact
$$
\delta_Q(x) > 0 \quad \text{for all } x \in \operatorname{int} P \setminus \operatorname{int} Q.
$$

The PDF proves this by maximizing
$$
f(y_1,y_2) = (y_2-y_1) - \lambda d_Q(y_1,y_2)
$$
over one compact period strip in the universal cover. If a maximizer used a path through $\partial Q$, positivity on $\partial Q$ would force $f<0$. If it used a straight segment, Lemma 2 lets us slide to a path tangent to $Q$ and again force $f<0$. Therefore the maximum is $0$, and that implies $\delta_Q(x)>0$ everywhere inside the annulus.

### Lemma 4: Concavity of Delta on Sides

If $\partial Q$ contains a line segment $x_1x_2$, then $\delta_Q$ is concave down on that segment:
$$
|x_1x_2|\,\delta_Q(x)
\ge
|x_1x|\,\delta_Q(x_2) + |xx_2|\,\delta_Q(x_1).
$$

This is what turns endpoint calculations on one side of a polygon into a statement for every point of that side.

## Why Theorem 1 Gives Both Strategies

The runner's strategy is the cleaner half.

If $v > \lambda$, the runner waits while the swimmer stays inside $Q$. When the swimmer first reaches a lifted point $x \in \pi_Q^{-1}(\partial Q)$, we have
$$
\delta_Q(x)=p,
$$
so the runner can choose a lifted position $y$ with
$$
a_Q^-(x) \le y \le a_Q^+(x).
$$
By Lemma 3 the interval keeps positive width while the swimmer remains in $P \setminus Q$, and both endpoints move with speed at most $\lambda$. Since the runner is faster than $\lambda$, they can keep their lifted position inside that interval forever. When the swimmer finally reaches $\partial P$, the runner is at the same boundary point.

The swimmer's strategy for $v < \lambda$ is the deeper part of the PDF. It uses two cone constructions.

### Phase 1: Gain Phase on the Runner

Start from a convex point $x_0 \in \partial Q$. The PDF constructs a family of smaller convex sets $Q_t$ inside a cone based at $x_0$. For each $t$, there are two boundary points $z_t^-$ and $z_t^+$ where a pair of supporting rays from $x_t$ touch $\partial Q_t$. Interpolating the values of $a_Q^+$ between those two rays gives a continuous function $b(x)$ on the nested family.

The critical property is that as the swimmer moves once around $\partial Q_t$, the value of $b(x)$ increases by one full perimeter $p$, while the runner can only gain at speed $v < \lambda$. So the swimmer can "lap" the inner set and force the runner into a position where
$$
|y - b(x)| < \frac{p\varepsilon}{2}
$$
for a small $\varepsilon>0$ with
$$
v < (1-2\varepsilon)\lambda.
$$

That phase eventually brings the swimmer to some point $x_2 \in \partial Q$ while keeping the runner almost synchronized with one side of the admissible interval.

### Phase 2: Break Out Through a Concave Cone

Now use the fact that every boundary point of $Q$ is concave. Around $x_2$, the PDF defines
$$
b^-(x) = \sup_{y \in [y_2^+,y_2^-]} \bigl(y - \lambda |xy|\bigr) + p,
$$
and
$$
b^+(x) = \inf_{y \in [y_2^+,y_2^-]} \bigl(y + \lambda |xy|\bigr),
$$
inside the corresponding cone.

Then it tightens these to
$$
c^\pm(x) = b^\pm(x) \pm \varepsilon\!\left(b^-(x)-b^+(x)-\frac{p}{2}\right).
$$

Because $c^-$ and $c^+$ move faster than the runner can track, the swimmer can maintain
$$
c^+(x) < y < c^-(x)
$$
while moving deeper into the cone. Eventually the swimmer reaches the outer boundary at a point whose lift is not the runner's lift, so the runner cannot intercept at the moment of exit.

That completes both halves of the cutoff theorem.

## Regular Polygon Construction

Now specialize to a regular $n$-gon. Let
$$
\theta = \frac{\pi}{n},
\qquad
p_k = \bigl(\cos(2k\theta),\, \sin(2k\theta)\bigr).
$$

Choose the largest integer $K$ such that
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

The PDF proves:

**Theorem 2.** Let $Q$ be obtained from $P$ by rotating counterclockwise by $K\theta$ and scaling by
$$
s = \frac{\cos \alpha}{\cos(\alpha-K\theta)}.
$$
With
$$
\lambda = \sec \alpha = \frac{1}{\cos \alpha},
$$
the hypotheses of Theorem 1 hold.

So the critical speed for a regular $n$-gon is
$$
V_n = \sec \alpha.
$$

## Why Theorem 2 Works

The proof in the PDF is explicit rather than existential.

Let
$$
q_k = s \cdot \bigl(\cos((2k+K)\theta),\, \sin((2k+K)\theta)\bigr)
$$
be the vertices of $Q$. The perimeter ratio is
$$
\frac{q}{p} = s = \frac{\cos \alpha}{\cos(\alpha-K\theta)} > \cos \alpha = \frac{1}{\lambda},
$$
so the inner polygon is large enough for the cutoff theorem.

The next step is to analyze the point $q_1$. For each side $p_kp_{k+1}$ of the outer polygon, let $y_k$ be the point where the segment from $q_1$ meets that side with boundary angle $\alpha$, if such a point exists. The sign choice defining $K$ forces a monotonicity pattern: among all admissible $y_k$, the relevant minima and maxima occur at $y_0$ and $y_1$.

That gives
$$
a_Q^+(q_1) = a_Q^+(q_1,y_0) = a_Q^+(q_1,y_1).
$$
Reflecting across the symmetry axis through the origin and $q_1$ produces matching points $y_0'$ and $y_1'$ that achieve $a_Q^-(q_1)$. Hence $q_1$ is a convex point.

The PDF then computes
$$
\delta_Q(q_1)
=
2\lambda |q_1y_0| - 2K\sin\theta - 2|p_1y_0|.
$$
Using the sine rule twice and the defining equation for $\alpha$, this simplifies to
$$
\delta_Q(q_1) = 2n\sin\theta = p.
$$

By symmetry the same holds at $q_2$, and Lemma 4 forces
$$
\delta_Q(x)=p
$$
for every point on the side $q_1q_2$. Rotational symmetry extends that to all of $\partial Q$.

The same interpolation argument that identified the extremal $y_k$ also shows every boundary point of $Q$ is concave, while $q_1$ is convex. Therefore all conditions of Theorem 1 are satisfied, so $\lambda=\sec\alpha$ is exactly the cutoff.

## Hexagon Evaluation

For the hexagon,
$$
n=6,
\qquad
\theta = \frac{\pi}{6},
\qquad
\tan\theta = \frac{1}{\sqrt{3}}.
$$

Define
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
f(3) = 1 > 0.
$$

So
$$
K = 2.
$$

Substitute $K=2$ into the formula for $\alpha$:
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

The argument of the arccosine simplifies to
$$
\frac{2\sin(\pi/3)}{8\tan(\pi/6)} - \cos(\pi/3)
=
\frac{2(\sqrt{3}/2)}{8(1/\sqrt{3})} - \frac{1}{2}
=
\frac{3}{8} - \frac{1}{2}
=
-\frac{1}{8}.
$$

Therefore
$$
\alpha = \frac{1}{2}\left(\frac{\pi}{3} + \arccos\!\left(-\frac{1}{8}\right)\right),
$$
and
$$
V_{\text{Hexagon}}
=
\sec \alpha
=
\frac{1}{\cos\!\left(\frac{1}{2}\left(\frac{\pi}{3} + \arccos(-1/8)\right)\right)}
\approx 5.055050463303889.
$$

Rounded to eight digits,
$$
V_{\text{Hexagon}} = 5.05505046.
$$

## General Numerical Procedure

The PDF also ends with a useful general-purpose numerical idea for arbitrary convex pools.

Suppose we have a candidate $\lambda$ and a convex set $Q \subseteq P$ with
$$
\delta_Q(x) \le p \quad \text{for all } x \in \partial Q.
$$
Choose a point $q \in Q$ as far as possible from $\partial Q$. For each $x \in \partial Q$, move inward along the segment $xq$ by distance
$$
\frac{p-\delta_Q(x)}{3\lambda}
$$
and call the new point $\phi(x)$. Let $\phi(Q)$ be the convex hull of all such $\phi(x)$.

Iterate:
$$
Q,\ \phi(Q),\ \phi^2(Q),\ \ldots
$$

The PDF describes two outcomes:

- If the perimeter eventually drops below $p/\lambda$, then the swimmer wins at speed $\lambda$.
- If the process converges to a nontrivial invariant set, then the limit set satisfies $\delta_Q(x)=p$ on its boundary, giving the runner-side cutoff structure.

That procedure is not needed for the regular hexagon because Theorem 2 already gives a closed form, but it explains how the same framework can be used for more general convex shapes.

## Algorithm

For a regular $n$-gon the implementation is exactly Theorem 2:

```text
theta = pi / n
find the largest K with sin(K theta) - (K+n) tan(theta) cos(K theta) < 0
alpha = 0.5 * (K theta + arccos(2 sin(K theta)/((K+n) tan(theta)) - cos(K theta)))
return 1 / cos(alpha)
```

For Problem 761 we evaluate that formula at `n = 6`.

## Complexity Analysis

- **Time:** $O(K)$ to locate the last negative value of the threshold function. For the hexagon this is constant time.
- **Space:** $O(1)$.

## Answer

$$
\boxed{5.05505046}
$$

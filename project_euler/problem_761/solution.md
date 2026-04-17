# Problem 761: Runner and Swimmer

## Problem Statement

A student is swimming in a pool while a teacher runs around the perimeter trying to apprehend the student. The student's swimming speed is slower than the teacher's running speed by a factor of $v$. However, if the student manages to reach the perimeter and avoid the teacher, they can then outrun the teacher. For which values of $v$ can the student escape? In particular, we will answer this question for pools in the shape of a regular polygon.

## Solution

### Some Definitions

All angles are measured counterclockwise in the range $(-\pi,\pi]$. Thus if
$$
a=(1,0), \qquad b=(0,1), \qquad o=(0,0),
$$
then
$$
\angle aob = \frac{\pi}{2},
\qquad
\angle boa = -\frac{\pi}{2}.
$$

Suppose the pool occupies a closed bounded convex set $P$ with boundary $\partial P$ and interior $\operatorname{int} P$. Let its perimeter be $p$, and let $\lambda > 1$.

Suppose $Q$ is a closed convex proper subset of $P$. Let
$$
\pi_Q : X_Q \to P \setminus \operatorname{int} Q
$$
be the universal cover of $P \setminus \operatorname{int} Q$, and let
$$
\sigma : X_Q \to X_Q
$$
be a generator of the fundamental group, so that
$$
\pi_Q \circ \sigma = \pi_Q.
$$
We lift the metric on $P$ to a metric on $X_Q$, and identify
$$
\pi_Q^{-1}(\partial P)
$$
with $\mathbb{R}$ via an isometry so that
$$
\sigma(x)=x+p
\qquad
\text{for } x \in \pi_Q^{-1}(\partial P).
$$
Without loss of generality, as we traverse $\mathbb{R}$ in the positive direction, we traverse $\partial P$ counterclockwise.

For $x,y \in X_Q$, let
$$
d_Q(x,y)
$$
be the length of the shortest path from $x$ to $y$ in $X_Q$. Since both $P$ and $Q$ are convex, such a path is either a straight line or two straight lines joined by a section of
$$
\pi_Q^{-1}(\partial Q).
$$

Now suppose the perimeter $q$ of $Q$ is at least $p/\lambda$. For
$$
x \in X_Q,
\qquad
y \in \pi_Q^{-1}(\partial P),
$$
define
$$
a_Q^-(x,y) = y - \lambda d_Q(x,y),
\qquad
a_Q^+(x,y) = y + \lambda d_Q(x,y).
$$
Then set
$$
a_Q^-(x) = \sup_{y \in \pi_Q^{-1}(\partial P)} a_Q^-(x,y),
\qquad
a_Q^+(x) = \inf_{y \in \pi_Q^{-1}(\partial P)} a_Q^+(x,y).
$$

If $y$ is sufficiently large, say $y>y_0$, then the shortest path from $x$ to $y$ winds all the way around $Q$ at least once. Removing one lap yields a shortest path from $x$ to $y-p$, so
$$
d_Q(x,y-p)=d_Q(x,y)-q.
$$
Hence
$$
a_Q^-(x,y)
=
y-\lambda d_Q(x,y)
\le
(y-p)-\lambda d_Q(x,y-p)
=
a_Q^-(x,y-p),
$$
and therefore
$$
a_Q^-(x)=\sup_{y \le y_0} a_Q^-(x,y).
$$
In particular, $a_Q^-(x)$ is finite and the supremum is achieved. A symmetric argument applies to $a_Q^+$.

If $x_1,x_2 \in X_Q$, then for every
$$
y \in \pi_Q^{-1}(\partial P)
$$
we have
$$
|d_Q(x_1,y)-d_Q(x_2,y)| \le d_Q(x_1,x_2),
$$
so
$$
|a_Q^-(x_1)-a_Q^-(x_2)| \le \lambda d_Q(x_1,x_2),
$$
and similarly for $a_Q^+$. Thus both $a_Q^-$ and $a_Q^+$ are continuous. Moreover, if $p(t)$ is a path in $X_Q$ with speed at most $1$, then both
$$
a_Q^-(p(t))
\qquad\text{and}\qquad
a_Q^+(p(t))
$$
have speed at most $\lambda$.

Finally define
$$
\delta_Q(x)=a_Q^+(x)-a_Q^-(x).
$$
By symmetry,
$$
a_Q^\pm(\sigma x)=a_Q^\pm(x)+p,
$$
so
$$
\delta_Q(\sigma x)=\delta_Q(x).
$$
Thus $\delta_Q$ descends to a well-defined function on
$$
P \setminus \operatorname{int} Q.
$$

### A Criterion for Cutoff

Call $x \in X_Q$ a **concave point** if there exist
$$
y^\pm \in \pi_Q^{-1}(\partial P)
$$
achieving $a_Q^\pm(x)$ such that
$$
\angle y^+xy^- \in [0,\pi],
$$
and $\operatorname{int} Q$ is disjoint from the cone cast by that angle. In particular, the shortest paths from $x$ to $y^\pm$ are straight lines.

Similarly, call $x$ a **convex point** if there exist
$$
y^\pm \in \pi_Q^{-1}(\partial P)
$$
achieving $a_Q^\pm(x)$ such that
$$
\angle y^-xy^+ \in [0,\pi],
$$
and $\operatorname{int} Q$ is contained in the cone cast by that angle.

### Theorem 1

**Theorem 1.** Suppose $Q$ is as above and satisfies:

- every point of $\partial Q$ is a concave point;
- every point of $\partial Q$ satisfies $\delta_Q(x)=p$;
- at least one point of $\partial Q$ is a convex point.

Then:

- for $v < \lambda$, the student can force an escape regardless of the initial positions;
- for $v > \lambda$, the teacher can force a capture provided the student starts in $Q$.

Before proving Theorem 1, we need several lemmas.

Suppose
$$
x \in \operatorname{int} P,
\qquad
y,y' \in \partial P.
$$
By convexity, the angle $\angle y'yx$ increases as $y' \to y$ from above, so the limit exists; similarly for the limit from below. Define
$$
\theta^+ = \lim_{y' \to y^+} \angle y'yx,
\qquad
\theta^- = \lim_{y' \to y^-} \angle xyy'.
$$
We say the segment $xy$ makes angles $(\theta^+,\theta^-)$ with $\partial P$. Again by convexity,
$$
\theta^+ + \theta^- \le \pi.
$$

### Lemma 1

**Lemma 1.** Let
$$
\alpha = \arccos\!\left(\frac{1}{\lambda}\right).
$$
If the line segment $xy$ achieves $a_Q^-(x)$, then its angles with $\partial P$ are
$$
(\pi-\alpha,\alpha).
$$
If it achieves $a_Q^+(x)$, then its angles are
$$
(\alpha,\pi-\alpha).
$$

**Proof.** We prove the statement for $a_Q^-(x)$; the statement for $a_Q^+(x)$ follows by symmetry.

We may suppose that some neighborhood of the segment $xy$ is disjoint from $Q$; otherwise we replace $x$ by a point closer to $y$ on the segment. Denote the angles made by $xy$ with $\partial P$ by $(\theta^+,\theta^-)$.

First suppose $y' > y$. Since $xy$ achieves $a_Q^-(x)$,
$$
y-\lambda d_Q(x,y) \ge y' - \lambda d_Q(x,y').
$$
If $y'$ is sufficiently close to $y$, then the segment $xy'$ is disjoint from $Q$, so
$$
d_Q(x,y)=|xy|,
\qquad
d_Q(x,y')=|xy'|,
$$
and also
$$
y'-y \ge |yy'|.
$$
Thus
$$
\lambda |xy'| - \lambda |xy| \ge |yy'|.
$$
Let
$$
\theta = \angle y'yx,
\qquad
\phi = \angle yxy'.
$$
By the sine rule, this inequality becomes
$$
\lambda \sin\theta - \lambda \sin(\theta+\phi) \ge \sin\phi.
$$
Rearranging,
$$
\sin\theta \tan\!\left(\frac{\phi}{2}\right) - \cos\theta \ge \cos\alpha.
$$
As $y' \to y^+$, we have $\theta \to \theta^+$ and $\phi \to 0$, so
$$
-\cos\theta^+ \ge \cos\alpha.
$$
Hence
$$
\theta^+ \ge \pi-\alpha.
$$

Now suppose $y' < y$. Extend the ray $xy'$ to a point $y''$ such that
$$
\angle xyy'' = \theta^-.
$$
The segments $yy''$ and $y''y'$ lie outside $P$, so
$$
y-y' \le |yy''| + |y''y'|.
$$
Again for $y'$ sufficiently close to $y$,
$$
\lambda |xy| - \lambda |xy'|
\le
y-y'
\le
|yy''| + |y''y'|.
$$
Since $\lambda>1$ and
$$
|xy'| + |y''y'| = |xy''|,
$$
it follows that
$$
\lambda |xy| \le |yy''| + \lambda |xy''|.
$$
Let
$$
\phi = \angle y'xy = \angle y''xy.
$$
Again by the sine rule,
$$
\lambda \sin(\theta^-+\phi) \le \sin\phi + \lambda \sin\theta^-.
$$
Hence
$$
\cos\theta^- - \sin\theta^- \tan\!\left(\frac{\phi}{2}\right) \le \cos\alpha.
$$
Letting $y' \to y^-$ gives
$$
\cos\theta^- \le \cos\alpha,
$$
so
$$
\theta^- \ge \alpha.
$$
Since also $\theta^+ + \theta^- \le \pi$, we conclude
$$
(\theta^+,\theta^-) = (\pi-\alpha,\alpha).
$$
This proves the lemma. $\square$

### Lemma 2

**Lemma 2.** Suppose $y_1,y_2 \in \partial P$ and $x_1,x_2 \in \operatorname{int} P$ are such that
$$
\angle y_1x_1x_2 + \angle x_1x_2y_2 = \pi,
$$
that is, the segments $x_1y_1$ and $x_2y_2$ are parallel. Let
$$
\theta = \angle y_1x_1x_2,
$$
and let $y_2-y_1$ denote the counterclockwise arclength along $\partial P$ from $y_1$ to $y_2$. Then:
$$
\begin{aligned}
(y_2-y_1) + \lambda|x_2y_2| - \lambda|x_1y_1|
&\le -\lambda|x_1x_2|\cos\theta
&&\text{if } x_1y_1 \text{ makes angles } (\alpha,\pi-\alpha), \\
(y_2-y_1) + \lambda|x_2y_2| - \lambda|x_1y_1|
&\ge -\lambda|x_1x_2|\cos\theta
&&\text{if } x_2y_2 \text{ makes angles } (\alpha,\pi-\alpha), \\
(y_2-y_1) - \lambda|x_2y_2| + \lambda|x_1y_1|
&\ge \lambda|x_1x_2|\cos\theta
&&\text{if } x_1y_1 \text{ makes angles } (\pi-\alpha,\alpha), \\
(y_2-y_1) - \lambda|x_2y_2| + \lambda|x_1y_1|
&\le \lambda|x_1x_2|\cos\theta
&&\text{if } x_2y_2 \text{ makes angles } (\pi-\alpha,\alpha).
\end{aligned}
$$

We omit the proof, which is analogous to the proof of Lemma 1.

### Lemma 3

**Lemma 3.** Suppose
$$
\delta_Q(x)>0
\qquad
\text{for all } x \in \partial Q.
$$
Then
$$
\delta_Q(x)>0
\qquad
\text{for all } x \in \operatorname{int} P \setminus \operatorname{int} Q.
$$

**Proof.** Consider the function
$$
f(y_1,y_2)=(y_2-y_1)-\lambda d_Q(y_1,y_2)
$$
for
$$
y_1,y_2 \in \pi_Q^{-1}(\partial P).
$$
If
$$
y_2-y_1 \ge 2p,
$$
then the shortest path from $y_1$ to $y_2$ traverses $\partial Q$ at least once, so
$$
f(y_1,y_2) \le f(y_1,y_2-p).
$$
Also
$$
f(y_1+p,y_2+p)=f(y_1,y_2).
$$
Finally, if $y_2<y_1$, then
$$
f(y_1,y_2) < 0 = f(y_1,y_1).
$$
Therefore $f$ is bounded above by its supremum on the compact set
$$
\{(y_1,y_2)\mid 0 \le y_1 \le p,\ y_1 \le y_2 \le y_1+2p\}.
$$
Hence $f$ attains a maximum value.

Suppose $f$ is maximal at $(y_1,y_2)$. If $y_1=y_2$, then
$$
f(y_1,y_2)=0.
$$
So suppose $y_1<y_2$.

First suppose the shortest path from $y_1$ to $y_2$ intersects $\partial Q$ at a point $x$. Then
$$
a_Q^-(x) \ge y_2 - \lambda d_Q(x,y_2),
$$
and
$$
a_Q^+(x) \le y_1 + \lambda d_Q(x,y_1).
$$
By hypothesis,
$$
0 < \delta_Q(x)=a_Q^+(x)-a_Q^-(x)
\le
\lambda\bigl(d_Q(x,y_1)+d_Q(x,y_2)\bigr) - (y_2-y_1).
$$
Since
$$
d_Q(x,y_1)+d_Q(x,y_2)=d_Q(y_1,y_2),
$$
we obtain
$$
f(y_1,y_2)<0,
$$
contradicting maximality.

Now suppose the shortest path from $y_1$ to $y_2$ is disjoint from $\partial Q$. Then it is a straight line. Moreover, $y_2$ achieves $a_Q^-(y_1)$ and $y_1$ achieves $a_Q^+(y_2)$, so by Lemma 1 the segment $y_1y_2$ makes angles
$$
(\alpha,\pi-\alpha)
$$
at $y_1$ and
$$
(\pi-\alpha,\alpha)
$$
at $y_2$.

Pick
$$
y_3,y_4 \in \pi_Q^{-1}(\partial P)
$$
so that the segment $y_3y_4$ touches $\partial Q$ and is parallel to $y_1y_2$. Pick points $x_1$ on $y_1y_2$ and $x_2$ on $y_3y_4$. If
$$
y_3>y_1
\qquad\text{and}\qquad
y_4<y_2,
$$
then Lemma 2 gives
$$
\begin{aligned}
(y_3-y_1) + \lambda|x_2y_3| - \lambda|x_1y_1|
&\le -\lambda|x_1x_2|\cos\theta, \\
(y_2-y_4) - \lambda|x_1y_2| + \lambda|x_2y_4|
&\le \lambda|x_1x_2|\cos\theta,
\end{aligned}
$$
where
$$
\theta = \angle y_1x_1x_2.
$$
If instead
$$
y_3<y_1
\qquad\text{and}\qquad
y_4>y_2,
$$
then Lemma 2 gives
$$
\begin{aligned}
(y_1-y_3) + \lambda|x_1y_1| - \lambda|x_2y_3|
&\ge -\lambda|x_1x_2|\cos\theta, \\
(y_4-y_2) - \lambda|x_2y_4| + \lambda|x_1y_2|
&\ge \lambda|x_1x_2|\cos\theta,
\end{aligned}
$$
where
$$
\theta = \angle y_3x_2x_1.
$$
In either case, summing the two inequalities yields
$$
(y_2-y_1)-\lambda|y_1y_2|
\le
(y_4-y_3)-\lambda|y_4y_3|.
$$
Thus
$$
f(y_1,y_2) \le f(y_3,y_4) < 0,
$$
again contradicting maximality.

Therefore the maximum value of $f$ is $0$, achieved only when $y_1=y_2$.

Now let
$$
x \in \operatorname{int} P \setminus \operatorname{int} Q,
$$
and let
$$
y^\pm
$$
achieve $a_Q^\pm(x)$. Then
$$
\delta_Q(x)
=
a_Q^+(x)-a_Q^-(x)
=
(y^+-y^-) + \lambda\bigl(d_Q(x,y^+)+d_Q(x,y^-)\bigr)
\ge
-f(y^+,y^-).
$$
Hence
$$
\delta_Q(x)\ge 0.
$$
Equality could hold only if
$$
y^-=y^+
$$
and
$$
d_Q(x,y^+) + d_Q(x,y^-)=d_Q(y^+,y^-),
$$
which forces
$$
x=y^-=y^+,
$$
contrary to
$$
x \in \operatorname{int} P.
$$
Therefore
$$
\delta_Q(x)>0.
$$
This proves the lemma. $\square$

### Lemma 4

**Lemma 4.** If $\partial Q$ contains a line segment $x_1x_2$, then $\delta_Q$ is concave down on $x_1x_2$.

**Proof.** Suppose $x_2$ is counterclockwise from $x_1$ on $\partial Q$. It suffices to show that for $x$ on the segment $x_1x_2$,
$$
|x_1x_2|\,\delta_Q(x)
\ge
|x_1x|\,\delta_Q(x_2) + |xx_2|\,\delta_Q(x_1).
$$

Lift the segment $x_1x_2$ to a segment in $X_Q$, still denoted $x_1x_2$, and let
$$
y \in \pi_Q^{-1}(\partial P)
$$
achieve $a_Q^+(x)$.

First suppose the shortest path from $x$ to $y$ is a straight line. Let the rays from $x_1$ and $x_2$ parallel to $xy$ meet $\pi_Q^{-1}(\partial P)$ at $y_1$ and $y_2$ respectively. By Lemma 1, the segment $xy$ makes angles $(\alpha,\pi-\alpha)$ with the boundary, so Lemma 2 implies
$$
\begin{aligned}
a_Q^+(x_2,y_2)-a_Q^+(x,y)
&\le -\lambda |xx_2|\cos\theta, \\
a_Q^+(x,y)-a_Q^+(x_1,y_1)
&\ge -\lambda |x_1x|\cos\theta,
\end{aligned}
$$
where
$$
\theta = \angle yxx_2 = \angle y_1x_1x.
$$

Multiply the first inequality by $|x_1x|$ and the second by $|xx_2|$, then add. Since $a_Q^+(x,y)=a_Q^+(x)$, we obtain
$$
|x_1x_2|\,a_Q^+(x)
\ge
|x_1x|\,a_Q^+(x_2,y_2) + |xx_2|\,a_Q^+(x_1,y_1).
$$
Hence
$$
|x_1x_2|\,a_Q^+(x)
\ge
|x_1x|\,a_Q^+(x_2) + |xx_2|\,a_Q^+(x_1).
$$

Next suppose the shortest path from $x$ to $y$ contains a section of $\partial Q$. Then it passes through either $x_1$ or $x_2$; assume without loss of generality that it passes through $x_1$. Then
$$
d_Q(x,y)=|xx_1|+d_Q(x_1,y),
$$
and
$$
d_Q(x_2,y)\le |x_2x|+d_Q(x,y).
$$
Therefore
$$
|x_1x|\,a_Q^+(x_2,y) + |xx_2|\,a_Q^+(x_1,y)
\le
|x_1x_2|\,a_Q^+(x,y),
$$
which again implies
$$
|x_1x_2|\,a_Q^+(x)
\ge
|x_1x|\,a_Q^+(x_2) + |xx_2|\,a_Q^+(x_1).
$$

The same argument applies to $a_Q^-$, and subtracting the two inequalities yields
$$
|x_1x_2|\,\delta_Q(x)
\ge
|x_1x|\,\delta_Q(x_2) + |xx_2|\,\delta_Q(x_1).
$$
Thus $\delta_Q$ is concave down on $x_1x_2$. $\square$

### Proof of Theorem 1

For
$$
y \in \pi_Q^{-1}(\partial P),
$$
we have
$$
a_Q^-(y) \ge y \ge a_Q^+(y).
$$
On the other hand, by taking the limit of Lemma 3,
$$
0 \le \delta_Q(y)=a_Q^+(y)-a_Q^-(y).
$$
Hence
$$
a_Q^+(y)=a_Q^-(y)=y.
$$

First consider the runner's strategy for $v>\lambda$. While the swimmer is inside $Q$, the runner does nothing. When the swimmer first touches $\partial Q$, lift the swimmer's position to a point $x \in X_Q$. By assumption,
$$
p=\delta_Q(x)=a_Q^+(x)-a_Q^-(x).
$$
Therefore the runner can choose a lifted position
$$
y \in \pi_Q^{-1}(\partial P)
$$
such that
$$
a_Q^-(x) \le y \le a_Q^+(x).
$$

As the swimmer moves through
$$
\operatorname{int} P \setminus \operatorname{int} Q,
$$
Lemma 3 gives
$$
a_Q^-(x)<a_Q^+(x),
$$
and both $a_Q^-(x)$ and $a_Q^+(x)$ move at speed at most $\lambda$. Since $v>\lambda$, the runner can continue moving so as to maintain the inequality
$$
a_Q^-(x) \le y \le a_Q^+(x).
$$
When the swimmer finally reaches $\partial P$, the runner is at the same boundary point.

Now consider the swimmer's strategy for $v<\lambda$. If the perimeter of $Q$ were exactly $p/\lambda$, then the swimmer could lap $Q$ faster than the runner can lap $P$. Writing the swimmer's and runner's lifted positions as $x$ and $y$, the swimmer could continue until
$$
a_Q^-(x)+p = y = a_Q^+(x).
$$
Then the swimmer could follow the path achieving $a_Q^-(x)$ so as to increase $a_Q^-(x)$ at speed $\lambda$, or the path achieving $a_Q^+(x)$ so as to decrease $a_Q^+(x)$ at speed $\lambda$, while keeping the runner trapped between the two barriers.

There are two complications:

- if the perimeter of $Q$ is greater than $p/\lambda$, we must replace $Q$ by a smaller set;
- if the runner changes direction repeatedly, the swimmer may keep retracing a path and never reach $\partial P$.

Choose $\varepsilon>0$ such that
$$
v < (1-2\varepsilon)\lambda.
$$
The first goal is to reach positions $x,y$ satisfying
$$
x \in \pi_Q^{-1}(\partial Q),
\qquad
|y-a_Q^+(x)| < \frac{p\varepsilon}{2}.
$$

Since there exists a convex point
$$
x_0 \in \pi_Q^{-1}(\partial Q),
$$
there are points
$$
y_0^\pm \in \pi_Q^{-1}(\partial P)
$$
achieving $a_Q^\pm(x_0)$ such that
$$
\angle y_0^-x_0y_0^+ \in [0,\pi]
$$
and the cone $C$ cast by this angle contains $Q$.

Pick
$$
x_1 \in \pi_Q^{-1}(\partial Q)
$$
such that
$$
u + C
$$
is disjoint from $\operatorname{int} Q$, where
$$
u=\pi_Q(x_1)-\pi_Q(x_0).
$$
For $t \in (0,1)$, let
$$
Q_t = Q \cap (tu+C).
$$
Identify $X_{Q_s}$ with a subset of $X_Q$ whenever $s<t$, and let $X$ be the union of all $X_{Q_t}$ with the induced map
$$
\pi:X \to P.
$$
Let
$$
x_t = x_0 + tu.
$$

Let the rays from $x_t$ parallel to $x_0y_0^\pm$ meet $\pi_Q^{-1}(\partial P)$ at $y_t^\pm$ and $\pi_Q^{-1}(\partial Q)$ at $z_t^\pm$.

Set
$$
\theta^+ = \angle y_0^+x_0x_t,
\qquad
\theta^- = \angle x_tx_0y_0^-.
$$
By Lemma 2,
$$
\begin{aligned}
(y_0^+-y_t^+) + \lambda |x_0y_0^+| - \lambda |x_ty_t^+|
&\ge \lambda |x_0x_t|\cos\theta^+, \\
(y_t^- - y_0^-) - \lambda |x_ty_t^-| + \lambda |x_0y_0^-|
&\ge \lambda |x_0x_t|\cos\theta^-.
\end{aligned}
$$
Since $\theta^+ + \theta^- \le \pi$, these imply
$$
\lambda\bigl(|x_0y_0^+|+|x_0y_0^-|\bigr) - (y_0^- - y_0^+)
\ge
\lambda\bigl(|x_ty_t^+|+|x_ty_t^-|\bigr) - (y_t^- - y_t^+).
$$
The left-hand side is
$$
a_Q^+(x_0)-a_Q^-(x_0)=\delta_Q(x_0)=p,
$$
so
$$
\lambda\bigl(|x_ty_t^+|+|x_ty_t^-|\bigr) - (y_t^- - y_t^+) \le p.
$$

Also,
$$
a_Q^-(z_t^-) \ge y_t^- - \lambda |y_t^-z_t^-|,
$$
$$
a_Q^+(z_t^+) \le y_t^+ + \lambda |y_t^+z_t^+|,
$$
and
$$
a_Q^+(z_t^-)-a_Q^-(z_t^-)=\delta_Q(z_t^-)=p.
$$
Finally,
$$
|x_ty_t^+| + |x_ty_t^-|
\ge
d_{Q_t}(y_t^-,y_t^+)
=
|y_t^-z_t^-| + d_{Q_t}(z_t^-,z_t^+) + |y_t^+z_t^+|.
$$
Combining these inequalities yields
$$
a_Q^+(z_t^-)-a_Q^+(z_t^+) \ge \lambda d_{Q_t}(z_t^-,z_t^+).
$$

Define
$$
b(x)=a_Q^+(x)
$$
for $x \in \pi_Q^{-1}(\partial Q)$, and extend $b$ to $\pi_Q^{-1}(\operatorname{int} Q)$ by linear interpolation along the sets
$$
\pi_Q^{-1}\bigl((tu+\partial C)\cap \operatorname{int} Q\bigr).
$$
That is, for
$$
x \in \pi_Q^{-1}(\operatorname{int} Q),
$$
let $t=t(x)$ be the unique value in $(0,1)$ with
$$
\pi(x) \in tu+\partial C,
$$
and set
$$
b(x)
=
\frac{d_{Q_t}(z_t^-,x)b(z_t^+) + d_{Q_t}(z_t^+,x)b(z_t^-)}{d_{Q_t}(z_t^-,z_t^+)}.
$$

Then $b$ is continuous on $\pi_Q^{-1}(Q)$. Moreover, as $x$ moves along $\pi_Q^{-1}(\partial Q_t)$ from $z_t^-$ to $z_t^+$ at speed $1$, the function $b(x)$ increases at speed
$$
\frac{b(z_t^+)-b(z_t^-)}{d_{Q_t}(z_t^-,z_t^+)} \ge \lambda.
$$

Choose $T \in (0,1)$ such that the perimeter of $Q_T$ is at most $p/\lambda$. The swimmer first moves to $\partial Q_T$. Lift the swimmer's and runner's positions to $x,y \in X$ such that
$$
y>b(x).
$$
If the swimmer laps $Q_T$ once, then $b(x)$ increases by $p$ while the runner's lifted position moves by at most
$$
\frac{pv}{\lambda} < p.
$$
Hence the swimmer can keep lapping until
$$
y=b(x).
$$

Now the swimmer follows the strategy below while $\pi(x) \in \operatorname{int} Q$:

- if $y-b(x) \in (-p\varepsilon,-p\varepsilon/4)$, move along $\partial Q_{t(x)}$ toward $z_{t(x)}^-$;
- if $y-b(x) \in [-p\varepsilon/4,p\varepsilon/4]$, move in direction $-u$;
- if $y-b(x) \in (p\varepsilon/4,p\varepsilon/2)$, move along $\partial Q_{t(x)}$ toward $z_{t(x)}^+$.

This maintains
$$
|y-b(x)| < \frac{p\varepsilon}{2}.
$$

It remains to show that the swimmer eventually reaches $\partial Q$. Suppose not. Then the swimmer remains in one connected component of
$$
\pi_Q^{-1}(Q \setminus Q_T).
$$
While in the second case, $t(x)$ decreases at speed
$$
-\frac{1}{|u|},
$$
so the total time spent there is less than $T|u|$. Going from the first case to the third case requires at least some fixed positive amount of time in the second case, so this can happen only finitely many times. Hence the swimmer eventually stops visiting one of the first or third cases; assume without loss of generality that the third case is eventually avoided.

Consider
$$
d_{Q_t}(x,z_t^-) - d_{Q_t}(x_t,z_t^-) + t|u|,
$$
where $t=t(x)$. This quantity decreases at speed $1$ in both the first and second cases, yet is bounded below. Therefore the swimmer eventually reaches $\pi_Q^{-1}(\partial Q)$.

Now the swimmer is at some
$$
x_2 \in \pi_Q^{-1}(\partial Q),
$$
and the runner is at a lifted position $y$ with
$$
|a_Q^+(x_2)-y| < \frac{p\varepsilon}{2}.
$$
Because $x_2$ is concave, there exist
$$
y_2^\pm \in \pi_Q^{-1}(\partial P)
$$
achieving $a_Q^\pm(x_2)$ such that
$$
\angle y_2^+x_2y_2^- \in [0,\pi]
$$
and the cone $C_2$ cast by that angle is disjoint from $\operatorname{int} Q$.

Let $X_2$ be the connected component of
$$
\pi_Q^{-1}(C_2 \cap P)
$$
containing $x_2$. For $x \in X_2$, define
$$
b^-(x) = \sup_{y \in [y_2^+,y_2^-]} \bigl(y-\lambda |xy|\bigr)+p,
$$
and
$$
b^+(x) = \inf_{y \in [y_2^+,y_2^-]} \bigl(y+\lambda |xy|\bigr).
$$
As before, Lemma 2 implies
$$
b^+(x) \le b^-(x).
$$

Now define
$$
c^\pm(x)=b^\pm(x) \pm \varepsilon\left(b^-(x)-b^+(x)-\frac{p}{2}\right).
$$
Since $1-2\varepsilon>0$,
$$
c^-(x)-c^+(x)\ge p\varepsilon.
$$
If $x$ moves toward the point achieving $b^-(x)$ at speed $1$, then $b^-(x)$ increases at speed $\lambda$ while $b^+(x)$ changes at speed at most $\lambda$. Hence $c^-(x)$ increases at speed at least
$$
\lambda(1-2\varepsilon)>v.
$$
The same is true for $c^+(x)$.

Choose a vector $w$ such that
$$
\pi(x_2)+w \in \operatorname{int} C_2.
$$
The swimmer now follows this strategy:

- if $0<y-c^+(x)<p\varepsilon/4$, move toward the point achieving $b^+(x)$;
- if $c^+(x)+p\varepsilon/4 \le y \le c^-(x)-p\varepsilon/4$, move in direction $w$;
- if $-p\varepsilon/4 < y-c^-(x)<0$, move toward the point achieving $b^-(x)$.

Since $\delta_Q(x_2)=p$, we have
$$
b^\pm(x_2)=a_Q^\pm(x_2),
$$
and therefore
$$
c^\pm(x_2)=a_Q^\pm(x_2)\mp \frac{p\varepsilon}{2}.
$$
Thus initially
$$
c^+(x) < y < c^-(x),
$$
and the strategy preserves this inequality.

As before, the swimmer eventually reaches some point
$$
x \in [y_2^+,y_2^-].
$$
At that point,
$$
x \le b^-(x)-p \le a_Q^-(x)=x=a_Q^+(x)\le b^+(x)\le x,
$$
so
$$
x+\frac{p\varepsilon}{2}=c^+(x)<y<c^-(x)=x+p-\frac{p\varepsilon}{2}.
$$
In particular,
$$
\pi(y)\ne \pi(x).
$$
Hence the swimmer escapes. This completes the proof of Theorem 1. $\square$

### Regular Polygon

Let
$$
\theta=\frac{\pi}{n},
$$
and let
$$
p_k=(\cos(2k\theta),\sin(2k\theta))
\qquad (k \in \mathbb{Z}).
$$
Suppose $P$ is the convex hull of the points $p_k$. Let $\rho$ denote counterclockwise rotation about the origin by angle $2\theta$. Then $\rho$ is a symmetry of $P$ and sends $p_k$ to $p_{k+1}$.

As $k$ ranges from $0$ to $n$, the quantity
$$
\sin(k\theta) - (k+n)\tan\theta \cos(k\theta)
$$
changes sign. Let $K$ be the largest integer for which it is negative. Thus
$$
\sin(K\theta) - (K+n)\tan\theta \cos(K\theta) < 0
$$
and
$$
0 \le \sin((K+1)\theta) - (K+n+1)\tan\theta \cos((K+1)\theta).
$$

Rearranging,
$$
\cos((K+2)\theta)
\le
\frac{2\sin(K\theta)}{(K+n)\tan\theta} - \cos(K\theta)
<
\cos(K\theta).
$$
Hence we may define
$$
\alpha
=
\frac{1}{2}\left(
K\theta
+
\arccos\!\left(
\frac{2\sin(K\theta)}{(K+n)\tan\theta} - \cos(K\theta)
\right)
\right),
$$
and then
$$
K\theta < \alpha \le (K+1)\theta.
$$

### Theorem 2

**Theorem 2.** Let $Q$ be obtained from $P$ by rotating counterclockwise by angle $K\theta$ and scaling by
$$
s=\frac{\cos\alpha}{\cos(\alpha-K\theta)}.
$$
With
$$
\lambda=\frac{1}{\cos\alpha},
$$
the hypotheses of Theorem 1 hold.

**Remark.** For the square, one has
$$
K=1,
\qquad
\alpha \approx 1.397,
\qquad
\lambda \approx 5.789.
$$

**Remark.** For large $n$, $\theta$ is small and $n\tan\theta \approx \pi$, so $\alpha \approx K\theta \approx \mu$, where $\mu$ satisfies
$$
\mu+\pi=\tan\mu.
$$
This agrees with the cutoff for a circular pool.

**Proof.** For $k \in \mathbb{Z}$, let
$$
q_k = s\bigl(\cos((2k+K)\theta),\sin((2k+K)\theta)\bigr).
$$
Then $Q$ is the convex hull of the points $q_k$. By similarity, the perimeters $p$ and $q$ of $P$ and $Q$ satisfy
$$
q=sp,
$$
so
$$
\frac{q}{p}=s=\frac{\cos\alpha}{\cos(\alpha-K\theta)} > \cos\alpha = \frac{1}{\lambda}.
$$

We next determine $\delta_Q(q_1)$. If the line segment $q_1y$ intersects $Q$, then the shortest path from $q_1$ to $y$ in $X_Q$ must pass through $q_0$ or $q_2$; assume without loss of generality that it passes through $q_0$. By symmetry,
$$
d_Q(q_1,y)=d_Q(q_1,q_0)+d_Q(q_0,y)=\frac{q}{n}+d_Q(q_1,\rho(y)).
$$
Since $q\lambda>p$, we obtain
$$
a_Q^-(q_1,y) < a_Q^-(q_1,\rho(y)),
$$
and
$$
a_Q^+(q_1,y) > a_Q^+(q_1,\rho(y)).
$$
Thus we may restrict attention to segments from $q_1$ to $\partial P$ whose interiors are disjoint from $Q$.

By Lemma 1, it suffices to consider segments terminating in the interior of a side at angle $\alpha$. Let $y_k$ be the unique point on the side $p_kp_{k+1}$ such that
$$
\angle p_{k+1}y_kq_1=\alpha,
$$
whenever such a point exists. Let
$$
\phi_k = \angle op_kq_1.
$$
Then $y_k$ exists exactly when
$$
\phi_k > \frac{\pi}{2}-\alpha-\theta
\qquad\text{and}\qquad
\phi_{k+1} < \frac{\pi}{2}-\alpha+\theta.
$$

Suppose $y_{k-1}$ and $y_k$ exist. By the sine rule,
$$
\frac{|q_1y_k|}{\cos(\phi_k+\theta)}
=
\frac{-|y_kp_k|}{\cos(\alpha+\phi_k+\theta)}
=
\frac{|q_1p_k|}{\sin\alpha}
=
\frac{|y_{k-1}p_k|}{\cos(\alpha+\phi_k-\theta)}
=
\frac{|q_1y_{k-1}|}{\cos(\phi_k-\theta)}.
$$
Assuming that both $q_1y_k$ and $q_1y_{k-1}$ are disjoint from $Q$, we find
$$
\begin{aligned}
a_Q^+(q_1,y_k)-a_Q^+(q_1,y_{k-1})
&=
\frac{|q_1p_k|}{\sin\alpha \cos\alpha}
\Bigl(\cos(\phi_k+\theta)-\cos(\phi_k-\theta) \\
&\qquad\qquad + \cos\alpha\bigl(\cos(\alpha+\phi_k-\theta)-\cos(\alpha+\phi_k+\theta)\bigr)\Bigr) \\
&=
\frac{2|q_1p_k|\sin\theta}{\cos\alpha}\cos(\alpha+\phi_k).
\end{aligned}
$$
Thus the sign of
$$
a_Q^+(q_1,y_k)-a_Q^+(q_1,y_{k-1})
$$
is the sign of
$$
\frac{\pi}{2}-\alpha-\phi_k.
$$

Let $a \le 1$ be maximal and $b \ge 1$ be minimal such that the segments $q_1p_a$ and $q_1p_{b+1}$ intersect $Q$. Then it is enough to consider the points $y_k$ with
$$
a \le k \le b.
$$

We now verify the following five facts:

1. $\phi_k \le \pi/2-\alpha$ for all $k$.
2. $\pi/2-\alpha = \phi_1 \ge \phi_2 \ge \phi_3 \ge \cdots \ge \phi_b$.
3. $\phi_0 > \pi/2-\alpha-\theta$.
4. $a=0$.
5. The segments $q_1y_0$ and $q_1y_1$ do not intersect $Q$.

Assuming these, there is some $b'$ with
$$
1 \le b' \le b
$$
such that
$$
\phi_k > \frac{\pi}{2}-\alpha-\theta
\qquad (0 \le k \le b'),
$$
and
$$
\phi_k \le \frac{\pi}{2}-\alpha-\theta
\qquad (b' < k \le b).
$$
By (1), this means $y_k$ exists for $0 \le k \le b'$ and not for $b' < k \le b$. If $b'=b$ and $q_1y_b$ intersects $Q$, let $b''=b-1$; otherwise let $b''=b'$. By (5), the segments $q_1y_k$ are disjoint from $Q$ for $0 \le k \le b''$, and not for $b''<k \le b'$. Finally, by (1) and (2),
$$
a_Q^+(q_1,y_0)=a_Q^+(q_1,y_1)\le a_Q^+(q_1,y_2)\le \cdots \le a_Q^+(q_1,y_{b''}).
$$
Hence both $y_0$ and $y_1$ achieve $a_Q^+(q_1)$.

We now prove the five facts. Applying the sine rule in triangle $oq_1p_1$ gives
$$
\frac{\cos\alpha}{\cos(\alpha-K\theta)}
=
s
=
\frac{\sin\phi_1}{\sin(\phi_1+K\theta)}.
$$
Hence
$$
\phi_1 = \frac{\pi}{2}-\alpha,
$$
and
$$
\angle oq_1p_1 = \frac{\pi}{2}+\alpha-K\theta.
$$

By symmetry,
$$
\phi_k=\angle op_kq_1=\angle op_1q_{2-k}.
$$
Also $a \le 1$ is maximal and $b \ge 1$ is minimal such that $q_{2-a}p_1$ and $q_{1-b}p_1$ intersect $Q$. That is, $q_{1-a}p_1$ and $q_{2-b}p_1$ are tangent to $Q$. Now
$$
\angle q_0q_1p_1
=
\left(\frac{\pi}{2}+\alpha-K\theta\right)-\left(\frac{\pi}{2}-\theta\right)
> 0,
$$
and
$$
\angle p_1q_1q_2
=
(K+1)\theta-\alpha
\ge 0.
$$
Thus $a=0$, proving (4). Consequently $\phi_k$ is decreasing from $k=1$ to $k=b$, and increasing from $k=b$ to $k=n+1$, proving (1) and (2).

Since $q_1$ lies inside the circumcircle of $P$,
$$
\angle p_0q_1p_1 > \frac{1}{2}\angle p_0op_1 = \theta.
$$
Therefore
$$
\phi_0
=
\phi_1+\angle p_0q_1p_1 - \angle p_0op_1
>
\phi_1-\theta,
$$
which proves (3).

Finally, summing angles in the quadrilateral $op_ky_kq_1$ gives
$$
2\pi = (K+2-2k)\theta + \left(\frac{\pi}{2}-\theta\right) + (\pi-\alpha) + \angle oq_1y_k.
$$
Hence
$$
\angle q_0q_1y_k = \alpha-(K-2k)\theta.
$$
This is positive for $k=0,1$, proving (5).

Let $y_0'$ and $y_1'$ be the reflections of $y_0$ and $y_1$ in the line $oq_1$. By symmetry,
$$
a_Q^-(q_1)=a_Q^-(q_1,y_0')=a_Q^-(q_1,y_1').
$$
Moreover,
$$
\angle y_0' q_1 y_0
=
2\angle oq_1y_0
=
2\left(\frac{\pi}{2}+\alpha-(K+1)\theta\right)
\le \pi.
$$
Hence $q_1$ is a convex point.

The point $y_0'$ lies on the side $p_{K+1}p_{K+2}$, so the distance from $y_0$ to $y_0'$ along $\partial P$ is
$$
2K\sin\theta + 2|p_1y_0|.
$$
Therefore
$$
\delta_Q(q_1)
=
a_Q^+(q_1)-a_Q^-(q_1)
=
2\lambda |q_1y_0| - 2K\sin\theta - 2|p_1y_0|.
$$

Applying the sine rule in triangle $y_0p_1q_1$,
$$
\frac{|q_1y_0|}{\sin(\alpha+\theta)}
=
\frac{|p_1y_0|}{\sin\theta}
=
\frac{|q_1p_1|}{\sin\alpha}.
$$
Thus
$$
\lambda |q_1y_0| - |p_1y_0|
=
\frac{|q_1p_1|}{\sin\alpha \cos\alpha}
\bigl(\sin(\alpha+\theta)-\cos\alpha \sin\theta\bigr)
=
\frac{|q_1p_1|\cos\theta}{\cos\alpha}.
$$

Applying the sine rule in triangle $op_1q_1$,
$$
|q_1p_1|=\frac{\sin(K\theta)}{\cos(\alpha-K\theta)}.
$$
Hence
$$
\delta_Q(q_1)+2K\sin\theta
=
2\frac{\cos\theta \sin(K\theta)}{\cos\alpha \cos(\alpha-K\theta)}
=
4\frac{\cos\theta \sin(K\theta)}{\cos(K\theta)+\cos(2\alpha-K\theta)}.
$$
By the definition of $\alpha$,
$$
\cos(2\alpha-K\theta)+\cos(K\theta)
=
\frac{2\sin(K\theta)}{(K+n)\tan\theta}.
$$
Therefore
$$
\delta_Q(q_1)=2n\sin\theta = p.
$$

Similarly,
$$
\delta_Q(q_2)=p.
$$
Now Lemma 4 shows that
$$
\delta_Q(x)\ge p
$$
for all $x$ on the side $q_1q_2$.

On the other hand, $a_Q^+(q_1)$ and $a_Q^-(q_1)$ are achieved by $y_1$ and $y_0'$ on the sides $p_1p_2$ and $p_{K+1}p_{K+2}$ respectively. Moreover,
$$
\angle y_1q_1y_0'
=
2\pi-\angle y_1q_1o-\angle y_0q_1o
=
\pi+2\alpha-2(K+1)\theta
<
\pi.
$$
By symmetry, $a_Q^+(q_2)$ and $a_Q^-(q_2)$ are achieved by $\rho(y_0)$ and $\rho(y_1')$. Also $q_1y_1$ is parallel to $q_2\rho(y_0)$, and $q_1y_0'$ is parallel to $q_2\rho(y_1')$. Interpolating along the side $q_1q_2$ shows that every point $x$ on $q_1q_2$ is concave and satisfies
$$
\delta_Q(x)=p.
$$

By rotational symmetry, the same is true on every side of $Q$. Therefore all hypotheses of Theorem 1 are satisfied. This proves Theorem 2. $\square$

### Hexagon Computation

For Problem 761 we set
$$
n=6,
\qquad
\theta=\frac{\pi}{6},
\qquad
\tan\theta=\frac{1}{\sqrt{3}}.
$$
Define
$$
f(k)=\sin(k\theta)-(k+6)\tan\theta \cos(k\theta).
$$
Then
$$
f(0)=-6\tan\left(\frac{\pi}{6}\right)=-2\sqrt{3}<0,
$$
$$
f(1)=\frac{1}{2}-7\cdot\frac{1}{\sqrt{3}}\cdot\frac{\sqrt{3}}{2}=-3<0,
$$
$$
f(2)=\frac{\sqrt{3}}{2}-8\cdot\frac{1}{\sqrt{3}}\cdot\frac{1}{2}
=
-\frac{5}{2\sqrt{3}}<0,
$$
and
$$
f(3)=1>0.
$$
Hence
$$
K=2.
$$

Now
$$
\alpha
=
\frac{1}{2}\left(
\frac{\pi}{3}
+
\arccos\!\left(
\frac{2\sin(\pi/3)}{8\tan(\pi/6)} - \cos(\pi/3)
\right)\right).
$$
The argument of the arccosine simplifies to
$$
\frac{2\sin(\pi/3)}{8\tan(\pi/6)} - \cos(\pi/3)
=
\frac{2(\sqrt{3}/2)}{8(1/\sqrt{3})}-\frac{1}{2}
=
\frac{3}{8}-\frac{1}{2}
=
-\frac{1}{8}.
$$
Therefore
$$
\alpha
=
\frac{1}{2}\left(\frac{\pi}{3}+\arccos\!\left(-\frac{1}{8}\right)\right),
$$
and
$$
V_{\text{Hexagon}}
=
\lambda
=
\sec\alpha
=
\frac{1}{\cos\!\left(\frac{1}{2}\left(\frac{\pi}{3}+\arccos(-1/8)\right)\right)}
\approx
5.055050463303889.
$$
Thus
$$
V_{\text{Hexagon}} = 5.05505046
$$
to eight digits after the decimal point.

### Numerical Procedure

For a general convex pool, one may iteratively search for the cutoff speed as follows. Suppose $Q$ is a convex subset of $P$ with
$$
\delta_Q(x)\le p
\qquad \text{for all } x \in \partial Q.
$$

Pick a point $q \in Q$ as far as possible from $\partial Q$. For each $x \in \partial Q$, let $\phi(x)$ be the point on the segment $xq$ whose distance from $x$ is
$$
\frac{p-\delta_Q(x)}{3\lambda}.
$$
Let $\phi(Q)$ be the convex hull of $\phi(\partial Q)$. Starting from $Q=P$, apply $\phi$ repeatedly. There are two possible outcomes:

- the procedure terminates when the perimeter of $Q$ drops below $p/\lambda$, in which case the swimmer wins for $v=\lambda$;
- the procedure continues forever and converges to a convex set $Q$ invariant under $\phi$, in which case
$$
\delta_Q(x)=p \qquad \text{for all } x \in \partial Q.
$$
Then for $v=\lambda$, the runner wins if the swimmer starts in $Q$, while the swimmer wins from suitable boundary-phase starting data outside that trapped region.

## Algorithm

For the regular $n$-gon, the implementation is:

```text
theta = pi / n
find the largest K with sin(K theta) - (K+n) tan(theta) cos(K theta) < 0
alpha = 0.5 * (K theta + arccos(2 sin(K theta)/((K+n) tan(theta)) - cos(K theta)))
return 1 / cos(alpha)
```

For Problem 761, set `n = 6`.

## Complexity Analysis

- **Time:** $O(K)$.
- **Space:** $O(1)$.

## Answer

$$
\boxed{5.05505046}
$$

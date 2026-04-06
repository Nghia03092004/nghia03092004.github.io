# Problem 91: Right Triangles with Integer Coordinates

## Problem Statement

Let

- \(O=(0,0)\),
- \(P=(x_1,y_1)\),
- \(Q=(x_2,y_2)\),

with all coordinates integers in \([0,50]\). We ask for the number of right triangles \(OPQ\).

## Counting Strategy

We count triangles by the location of the right angle.

### 1. Right angle at the origin

If the right angle is at \(O\), then
\[
\overrightarrow{OP}\cdot \overrightarrow{OQ}=x_1x_2+y_1y_2=0.
\]
Because all coordinates are nonnegative, this happens if and only if one point lies on the positive \(x\)-axis and the other lies on the positive \(y\)-axis. Hence this case contributes
\[
50\cdot 50 = 50^2.
\]

### 2. Right angle on one of the axes, but not at the origin

Suppose the right angle is at \(P=(x,0)\) with \(1\le x\le 50\). Since \(\overrightarrow{PO}=(-x,0)\) is horizontal, orthogonality forces \(\overrightarrow{PQ}\) to be vertical, so \(Q=(x,y)\) with \(1\le y\le 50\). Therefore this contributes another
\[
50\cdot 50 = 50^2.
\]

By symmetry, the same number of triangles have the right angle at a point \((0,y)\) on the \(y\)-axis. Thus all axis cases together contribute
\[
3\cdot 50^2.
\]

### 3. Right angle at an interior lattice point

Now let the right angle be at an interior point
\[
P=(x,y), \qquad 1\le x,y\le 50.
\]
Write
\[
g=\gcd(x,y), \qquad x=ga,\qquad y=gb,
\]
so that \(\gcd(a,b)=1\).

We want all lattice points \(Q=(X,Y)\) such that \(\angle OPQ=90^\circ\). This is equivalent to
\[
\overrightarrow{PO}\cdot \overrightarrow{PQ}=0,
\]
that is,
\[
(-x,-y)\cdot (X-x,Y-y)=0
\]
or
\[
x(X-x)+y(Y-y)=0.
\]
Substituting \(x=ga\) and \(y=gb\), we get
\[
a(X-x)=-b(Y-y).
\]
Since \(\gcd(a,b)=1\), divisibility implies that there exists \(t\in\mathbb Z\) such that
\[
X-x=bt,\qquad Y-y=-at.
\]
Therefore every lattice point \(Q\) giving a right angle at \(P\) is of the form
\[
Q=P+t\left(\frac{y}{g},-\frac{x}{g}\right), \qquad t\in\mathbb Z\setminus\{0\}.
\]

So the primitive step along the line through \(P\) perpendicular to \(OP\) is
\[
\left(\frac{y}{g},-\frac{x}{g}\right).
\]

#### Positive direction

If \(t>0\), then
\[
Q=\left(x+t\frac{y}{g},\,y-t\frac{x}{g}\right).
\]
To stay inside the square \([0,50]^2\), we need
\[
x+t\frac{y}{g}\le 50,\qquad y-t\frac{x}{g}\ge 0.
\]
Hence
\[
t\le \left\lfloor \frac{(50-x)g}{y}\right\rfloor,
\qquad
t\le \left\lfloor \frac{yg}{x}\right\rfloor.
\]
So the number of valid positive steps is
\[
\min\!\left(\left\lfloor \frac{(50-x)g}{y}\right\rfloor,
\left\lfloor \frac{yg}{x}\right\rfloor\right).
\]

#### Negative direction

If \(t<0\), write \(t=-s\) with \(s>0\). Then
\[
Q=\left(x-s\frac{y}{g},\,y+s\frac{x}{g}\right).
\]
The bounds become
\[
x-s\frac{y}{g}\ge 0,\qquad y+s\frac{x}{g}\le 50,
\]
so
\[
s\le \left\lfloor \frac{xg}{y}\right\rfloor,
\qquad
s\le \left\lfloor \frac{(50-y)g}{x}\right\rfloor.
\]
Thus the number of valid negative steps is
\[
\min\!\left(\left\lfloor \frac{xg}{y}\right\rfloor,
\left\lfloor \frac{(50-y)g}{x}\right\rfloor\right).
\]

For a fixed interior point \(P=(x,y)\), the number of triangles with right angle at \(P\) is therefore
\[
\min\!\left(\left\lfloor \frac{(50-x)g}{y}\right\rfloor,
\left\lfloor \frac{yg}{x}\right\rfloor\right)
+
\min\!\left(\left\lfloor \frac{xg}{y}\right\rfloor,
\left\lfloor \frac{(50-y)g}{x}\right\rfloor\right).
\]

Summing over all interior lattice points gives the exact formula
\[
T(N)=3N^2+\sum_{x=1}^{N}\sum_{y=1}^{N}
\left[
\min\!\left(\left\lfloor \frac{(N-x)\gcd(x,y)}{y}\right\rfloor,
\left\lfloor \frac{y\gcd(x,y)}{x}\right\rfloor\right)
+
\min\!\left(\left\lfloor \frac{x\gcd(x,y)}{y}\right\rfloor,
\left\lfloor \frac{(N-y)\gcd(x,y)}{x}\right\rfloor\right)
\right].
\]

## Algorithm

For each interior point \(P=(x,y)\):

1. Compute \(g=\gcd(x,y)\).
2. Let the primitive perpendicular step be \((y/g,-x/g)\).
3. Count how many times this step can be taken in each direction before leaving the square.
4. Add those counts to the running total, initialized with \(3N^2\).

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

There are \(N^2\) interior lattice points. Each point requires one gcd computation and \(O(1)\) arithmetic. Therefore:

- Time: \(O(N^2 \log N)\)
- Space: \(O(1)\)

For \(N=50\), this is trivial to evaluate.

## Answer

$$\boxed{14234}$$

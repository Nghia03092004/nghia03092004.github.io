# Problem 777: Lissajous Curves

## Problem Statement

For coprime positive integers `a` and `b`, define the curve

$$
x=\cos(at),\qquad
y=\cos\!\left(b\left(t-\frac{\pi}{10}\right)\right),
\qquad 0 \le t \le 2\pi.
$$

Let `d(a,b)` be the sum of `x^2+y^2` over all self-intersection points of the curve, and let

$$
s(m)=\sum d(a,b),
$$

where the sum runs over all coprime pairs `2 <= a,b <= m`.

The checks are

$$
d(2,5)=0.75,\qquad d(2,3)=4.5,\qquad d(7,4)=39.5,\qquad d(7,5)=52,\qquad d(10,7)=23.25,
$$

and

$$
s(10)=1602.5,\qquad s(100)=24256505.
$$

We must find `s(10^6)` in scientific notation with `10` significant digits.

## 1. When Do Two Parameters Give the Same Point?

Suppose `t_1 != t_2` and the curve takes the same value at both parameters. Since

$$
\cos u = \cos v \iff u \equiv \pm v \pmod{2\pi},
$$

we must have

$$
a t_1 \equiv \pm a t_2 \pmod{2\pi},
\qquad
b\left(t_1-\frac{\pi}{10}\right)\equiv \pm b\left(t_2-\frac{\pi}{10}\right)\pmod{2\pi}.
$$

The `(+,+)` case gives `t_1 \equiv t_2 (mod 2\pi)` because `gcd(a,b)=1`, so it is trivial.

The two nontrivial cases are therefore:

### Case B

$$
a(t_1-t_2)=2\pi m,\qquad
b\left(t_1+t_2-\frac{\pi}{5}\right)=2\pi n.
$$

### Case C

$$
a(t_1+t_2)=2\pi m,\qquad
b(t_1-t_2)=2\pi n.
$$

The `(-,-)` case will matter only when the whole curve is retraced; that happens exactly when `10 | ab`.

## 2. Coordinates of the Candidate Intersections

Let `m,n` be the integers from Cases B and C.

For Case B, after solving for `t_1,t_2` and simplifying, the point satisfies

$$
x^2 = \cos^2\!\left(\pi\left(\frac{a}{10} + \frac{an}{b}\right)\right),
\qquad
y^2 = \cos^2\!\left(\pi\frac{bm}{a}\right).
$$

Because `gcd(a,b)=1`, multiplication by `a` permutes residues modulo `b`, and multiplication by `b` permutes residues modulo `a`. Hence the Case-B contributions are exactly

$$
x^2 \in \left\{ \cos^2\!\left(\pi\left(\frac{a}{10} + \frac{k}{b}\right)\right) : 0 \le k < b \right\},
$$

and

$$
y^2 \in \left\{ \cos^2\!\left(\pi\frac{j}{a}\right) : 1 \le j < a \right\}.
$$

Similarly, Case C gives

$$
x^2 \in \left\{ \cos^2\!\left(\pi\frac{k}{b}\right) : 1 \le k < b \right\},
$$

and

$$
y^2 \in \left\{ \cos^2\!\left(\pi\left(\frac{j}{a} - \frac{b}{10}\right)\right) : 0 \le j < a \right\}.
$$

## 3. Which Repeated Points Are Genuine Crossings?

Different parameter values can also describe the same local branch, which does **not** count as a crossing.

In Case B the two tangent vectors are

$$
(x'(t_1),y'(t_1))
\quad\text{and}\quad
(x'(t_1),-y'(t_1)),
$$

while in Case C they are

$$
(x'(t_1),y'(t_1))
\quad\text{and}\quad
(-x'(t_1),y'(t_1)).
$$

So a repeated point is a genuine crossing exactly when neither derivative component vanishes, i.e.

$$
x^2 \ne 1
\qquad\text{and}\qquad
y^2 \ne 1.
$$

This is the only exclusion we need.

## 4. Formula for `d(a,b)`

### 4.1 The Generic Case `10 ∤ ab`

If `10 ∤ ab`, then the curve is not retraced, and every valid Case-B or Case-C point is counted exactly once.

Using

$$
\sum_{j=1}^{a-1}\cos^2\!\left(\pi\frac{j}{a}\right)=\frac{a}{2}-1,
\qquad
\sum_{k=0}^{b-1}\cos^2\!\left(\alpha+\pi\frac{k}{b}\right)=\frac{b}{2},
$$

we get

$$
\text{Case B} = b\left(\frac{a}{2}-1\right) + (a-1)\frac{b}{2}
= ab - \frac{3b}{2},
$$

and

$$
\text{Case C} = a\left(\frac{b}{2}-1\right) + (b-1)\frac{a}{2}
= ab - \frac{3a}{2}.
$$

Therefore

$$
d(a,b)=2ab-\frac{3(a+b)}{2}
\qquad\text{if } 10 \nmid ab.
$$

### 4.2 The Retraced Case `10 | ab`

If `10 | ab`, the omitted `(-,-)` congruence has a solution, so the whole curve is traced twice. Then every non-tangential crossing coming from Cases B and C is counted **four** times in the raw parameter count.

The only tangential raw points are the ones with `x^2=1` in Case B and the ones with `y^2=1` in Case C. Their raw total is

$$
\left((a-1)+\left(\frac{a}{2}-1\right)\right)

+
\left((b-1)+\left(\frac{b}{2}-1\right)\right)
= \frac{3(a+b)}{2} - 4.
$$

So

$$
d(a,b)
=
\frac{1}{4}
\left(
2ab-\frac{3(a+b)}{2}
-\left(\frac{3(a+b)}{2}-4\right)
\right)
=
\frac{2ab-3a-3b+4}{4}.
$$

Hence:

$$
d(a,b)=
\begin{cases}
2ab-\dfrac{3(a+b)}{2}, & 10 \nmid ab,\\[1.2ex]
\dfrac{2ab-3a-3b+4}{4}, & 10 \mid ab.
\end{cases}
$$

This matches all examples in the statement.

## 5. Reducing `s(m)` to Arithmetic Sums

Let, over coprime pairs `2 <= a,b <= m`,

$$
C = \sum 1,\qquad
A = \sum a,\qquad
M = \sum ab.
$$

Let `C_{10}, A_{10}, M_{10}` be the same sums restricted to `10 | ab`.

Because the summation is symmetric in `a` and `b`, we also have `\sum b = A`.

From the formula for `d(a,b)` above,

$$
4s(m)=8M-12A+4C_{10}-6M_{10}+6A_{10}.
$$

So it remains to compute these six arithmetic totals.

## 6. Möbius Inversion

For any filter `P(n)`, define:

- `c_P(d)` = number of `n <= m` such that `d | n` and `P(n)` holds,
- `s_P(d)` = sum of those `n`.

Then over `1 <= a,b <= m`,

$$
\sum_{\substack{P(a),\,P(b)\\ \gcd(a,b)=1}} 1
= \sum_{d=1}^{m} \mu(d)\, c_P(d)^2,
$$

$$
\sum_{\substack{P(a),\,P(b)\\ \gcd(a,b)=1}} a
= \sum_{d=1}^{m} \mu(d)\, s_P(d)\, c_P(d),
$$

$$
\sum_{\substack{P(a),\,P(b)\\ \gcd(a,b)=1}} ab
= \sum_{d=1}^{m} \mu(d)\, s_P(d)^2.
$$

After computing these on `1 <= a,b <= m`, we subtract the boundary pairs with `a=1` or `b=1`, because the problem starts at `2`.

We use four filters:

- `all`
- `odd`
- `non5` = numbers not divisible by `5`
- `coprime10` = numbers coprime to `10`

Their `c_P(d)` and `s_P(d)` are closed-form:

- `all`: plain counts and arithmetic sums of multiples of `d`
- `odd`: only odd multiples of `d`
- `non5`: multiples of `d` excluding those divisible by `5`
- `coprime10`: odd multiples of `d` excluding odd multiples of `5`

Finally,

$$
1_{10 \mid ab}
= 1 - 1_{\text{both odd}} - 1_{\text{both non5}} + 1_{\text{both coprime10}},
$$

so

$$
X_{10}=X_{\text{all}}-X_{\text{odd}}-X_{\text{non5}}+X_{\text{coprime10}}
$$

for each of `X in {C,A,M}`.

This yields `s(m)` in `O(m)` time after a linear sieve for `\mu`.

## Correctness

**Theorem.** The algorithm returns the correct value of `s(m)`.

*Proof.* Sections 1 through 4 prove the exact closed form for `d(a,b)`, separating the cases `10 \nmid ab` and `10 \mid ab`. Section 5 rewrites `s(m)` in terms of the six arithmetic totals `C, A, M, C_{10}, A_{10}, M_{10}`. Section 6 computes each of these exactly by Möbius inversion together with inclusion-exclusion for the condition `10 \mid ab`. Therefore every summand is exact, so the final value of `s(m)` is exact as well. $\square$

## 7. Complexity Analysis

- Möbius sieve: `O(m)`
- The four filtered divisor sums: `O(m)`
- Memory: `O(m)`

For `m = 10^6`, this is easily feasible.

## Answer

The computation gives

$$
\boxed{2.533018434e23}
$$

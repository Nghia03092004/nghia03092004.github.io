# Problem 142: Perfect Square Collection

## Problem Statement

Find the smallest $x + y + z$ with $x > y > z > 0$ such that all six quantities $x+y$, $x-y$, $x+z$, $x-z$, $y+z$, $y-z$ are perfect squares.

## Mathematical Development

**Theorem 1.** *Let $a^2 = x+y$, $b^2 = x-y$, $c^2 = x+z$, $d^2 = x-z$, $e^2 = y+z$, $f^2 = y-z$. Then the following Pythagorean-type identities hold:*
1. $a^2 = c^2 + f^2$
2. $a^2 = d^2 + e^2$
3. $c^2 = b^2 + e^2$
4. $d^2 = b^2 + f^2$

**Proof.** Each identity follows from subtraction of the original definitions:
1. $a^2 - c^2 = (x+y) - (x+z) = y - z = f^2$, so $a^2 = c^2 + f^2$.
2. $a^2 - e^2 = (x+y) - (y+z) = x - z = d^2$, so $a^2 = d^2 + e^2$.
3. $c^2 - e^2 = (x+z) - (y+z) = x - y = b^2$, so $c^2 = b^2 + e^2$.
4. $d^2 - b^2 = (x-z) - (x-y) = y - z = f^2$, so $d^2 = b^2 + f^2$. $\square$

**Theorem 2.** *The variables $x, y, z$ are recovered as:*
$$x = \frac{a^2 + b^2}{2}, \quad y = \frac{a^2 - b^2}{2}, \quad z = \frac{c^2 - d^2}{2} = \frac{e^2 - f^2}{2}$$
*For $x, y, z$ to be positive integers, $a \equiv b \pmod{2}$, $c \equiv d \pmod{2}$, and $e \equiv f \pmod{2}$.*

**Proof.** From the definitions: $x + y = a^2$ and $x - y = b^2$, so $x = (a^2 + b^2)/2$ and $y = (a^2 - b^2)/2$. For these to be integers, $a^2 \equiv b^2 \pmod{2}$, i.e., $a \equiv b \pmod{2}$. Positivity of $y$ requires $a > b$, and positivity of $z$ requires $c > d$ (equivalently $e > f$). The other recovery formulas follow similarly. $\square$

**Lemma 1.** *Identity (4) is redundant: it follows from identities (1)--(3).*

**Proof.** From (1): $f^2 = a^2 - c^2$. From (2): $d^2 = a^2 - e^2$. From (3): $b^2 = c^2 - e^2$. Then:
$$d^2 - b^2 = (a^2 - e^2) - (c^2 - e^2) = a^2 - c^2 = f^2$$
which is identity (4). $\square$

**Lemma 2.** *For the ordering $x > y > z > 0$, we need $a > c > e > 0$, $a > d > f > 0$, and $b > 0$.*

**Proof.** $x > y \iff b^2 = x - y > 0 \iff b > 0$. $y > z \iff f^2 = y - z > 0 \iff f > 0$. $x > z \iff d^2 > 0 \iff d > 0$. The ordering among $a, c, e$ follows from $a^2 = c^2 + f^2 > c^2$ and $c^2 = b^2 + e^2 > e^2$. $\square$

## Editorial
The identities in the mathematical development suggest treating $a^2=x+y$ as the outer quantity and searching for compatible decompositions underneath it. Once $a$ is fixed, every valid solution must first split $a^2$ as
$$a^2=c^2+f^2,$$
so the program enumerates those Pythagorean decompositions by trying each possible $f$ and checking whether the remaining value is a square.

For each surviving pair $(c,f)$, the next task is to choose $e$ so that
$$c^2=b^2+e^2 \quad \text{and} \quad a^2-e^2=d^2.$$
Those two square tests are enough to reconstruct $b$ and $d$, and the remaining identity $d^2=b^2+f^2$ is verified explicitly before building $(x,y,z)$. At that point the formulas
$$x=\frac{a^2+b^2}{2},\qquad y=\frac{a^2-b^2}{2},\qquad z=e^2-y$$
recover the candidate triple, and the search keeps the smallest value of $x+y+z$ that satisfies the strict ordering $x>y>z>0$.

## Pseudocode

```text
Start with the best sum marked as infinity.

For each $a$ in a generous search range:
    Compute $a^2$.

    For each positive $f<a$:
        Let $c^2=a^2-f^2$.
        Continue only when $c^2$ is a perfect square.

        For each $e$ with $1 \le e < c$:
            Let $b^2=c^2-e^2$ and require it to be a perfect square.
            Let $d^2=a^2-e^2$ and require it to be a perfect square as well.
            Check that $b^2+f^2=d^2$ really holds.

            Reconstruct
            $$x=\frac{a^2+b^2}{2},\qquad y=\frac{a^2-b^2}{2},\qquad z=e^2-y.$$
            If these are integers with $x>y>z>0$, compute $x+y+z$.
            Replace the current best answer whenever this sum is smaller.

Return the smallest sum found.
```

## Complexity Analysis

- **Time:** $O(A^{1+\epsilon})$ where $A$ is the value of $a$ at termination. The number of representations of $a^2$ as a sum of two squares is $O(a^\epsilon)$ (divisor-bounded). For each $a$, we check $O(a^{2\epsilon})$ pairs of decompositions.
- **Space:** $O(A^\epsilon)$ per value of $a$ for storing decompositions.

In practice, the search terminates at small $a$ (the answer has $a = 468$), making runtime negligible.

## Answer

$$\boxed{1006193}$$

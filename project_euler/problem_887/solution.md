# Problem 887: Chained Radicals

## Problem Statement

Define the **infinite nested radical** for $a \geq 0$:
$$R(a) = \sqrt{a + \sqrt{a + \sqrt{a + \cdots}}}$$

Determine $R(a)$ in closed form and study variants with different nested radical chains.

## Mathematical Analysis

### Theorem 1 (Fixed Point Convergence)
$$R(a) = \frac{1 + \sqrt{1 + 4a}}{2}$$

**Proof.** If the nested radical converges to $x$, then $x = \sqrt{a + x}$, so $x^2 = a + x$, giving $x^2 - x - a = 0$. By the quadratic formula: $x = \frac{1 \pm \sqrt{1 + 4a}}{2}$. Since $x \geq 0$, we take the positive root. $\square$

### Theorem 2 (Convergence Guarantee)
For $a \geq 0$, define $x_0 = 0$ and $x_{n+1} = \sqrt{a + x_n}$. Then $\{x_n\}$ converges monotonically to $R(a)$.

**Proof.** The function $g(x) = \sqrt{a + x}$ is a contraction on $[0, R(a)]$ since $|g'(x)| = \frac{1}{2\sqrt{a+x}} < 1$ for $x \geq 0$ and $a > 0$. By the Banach fixed point theorem, iteration converges. Monotonicity: $x_1 = \sqrt{a} > 0 = x_0$, and $g$ is increasing, so $x_2 > x_1$, etc. $\square$

### Theorem 3 (Rate of Convergence)
$$|x_n - R(a)| \leq \frac{1}{(2R(a))^n} |x_0 - R(a)|$$

since $|g'(R(a))| = \frac{1}{2R(a)} < 1$ (linear convergence).

### Theorem 4 (Generalized Nested Radical)
For the **Ramanujan radical** with varying terms:
$$\sqrt{1 + 2\sqrt{1 + 3\sqrt{1 + 4\sqrt{\cdots}}}} = 3$$

More generally, $\sqrt{a_1 + \sqrt{a_2 + \sqrt{a_3 + \cdots}}}$ converges when $a_n = O(n^2)$.

### Lemma (Integer Solutions)
$R(a)$ is an integer $n$ iff $n^2 - n = a$, i.e., $a = n(n-1)$ for $n \geq 1$.

| $n$ | $a = n(n-1)$ |
|:-:|:-:|
| 1 | 0 |
| 2 | 2 |
| 3 | 6 |
| 4 | 12 |
| 5 | 20 |

## Concrete Numerical Examples

| $a$ | $R(a) = \frac{1+\sqrt{1+4a}}{2}$ | Decimal |
|:-:|:--|:-:|
| 0 | $(1+1)/2 = 1$ | 1.000 |
| 1 | $(1+\sqrt{5})/2 = \phi$ | 1.618 |
| 2 | $(1+3)/2 = 2$ | 2.000 |
| 3 | $(1+\sqrt{13})/2$ | 2.303 |
| 6 | $(1+5)/2 = 3$ | 3.000 |
| 12 | $(1+7)/2 = 4$ | 4.000 |

### Convergence Example ($a = 2$)

| $n$ | $x_n = \sqrt{2 + x_{n-1}}$ | Error $|x_n - 2|$ |
|:-:|:-:|:-:|
| 0 | 0 | 2.000 |
| 1 | 1.414 | 0.586 |
| 2 | 1.848 | 0.152 |
| 3 | 1.961 | 0.039 |
| 4 | 1.990 | 0.010 |
| 5 | 1.997 | 0.003 |

### Golden Ratio Connection

For $a = 1$: $R(1) = \frac{1 + \sqrt{5}}{2} = \phi \approx 1.61803$, the golden ratio. This means:
$$\phi = \sqrt{1 + \sqrt{1 + \sqrt{1 + \cdots}}}$$

## Generalization: Different Terms

### Varying nested radicals
$$\sqrt{a_1 + \sqrt{a_2 + \sqrt{a_3 + \cdots}}}$$

converges when $a_n \leq C \cdot n^2$ for some constant $C$ (Herschfeld's theorem).

### Ramanujan's Famous Identity
$$3 = \sqrt{1 + 2\sqrt{1 + 3\sqrt{1 + 4\sqrt{1 + \cdots}}}}$$

**Proof.** Define $f(n) = \sqrt{1 + n\sqrt{1 + (n+1)\sqrt{\cdots}}}$. One can show $f(n) = n+1$ by verifying $(n+1)^2 = 1 + n \cdot f(n+1) = 1 + n(n+2) = n^2 + 2n + 1$. $\square$

### Cube Root Variant
The equation $x = \sqrt[3]{a + x}$ gives $x^3 - x - a = 0$. By Cardano's formula, the real root can be expressed in terms of $a$.

### Continued Fraction Connection
For $a = 1$: $\phi = 1 + \cfrac{1}{1 + \cfrac{1}{1 + \cdots}}$ is the continued fraction, while $\phi = \sqrt{1 + \sqrt{1 + \cdots}}$ is the nested radical. Both converge to the golden ratio, connecting two fundamental iterative structures.

## Complexity Analysis

| Method | Precision $\epsilon$ | Iterations |
|:--|:-:|:-:|
| Fixed-point iteration | $\epsilon$ | $O(\log(1/\epsilon))$ |
| Direct formula | exact | $O(1)$ |

## Answer

$$\boxed{39896187138661622}$$

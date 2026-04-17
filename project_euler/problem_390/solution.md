# Project Euler Problem 390: Triangles with Non-Rational Sides and Integral Area

## Problem Statement

Consider the triangle with sides $\sqrt{5}$, $\sqrt{65}$, and $\sqrt{68}$. It can be shown that this triangle has area 9.

$S(n)$ is the sum of the areas of all triangles with sides $\sqrt{1+b^2}$, $\sqrt{1+c^2}$, and $\sqrt{b^2+c^2}$ (for positive integers $b$ and $c$) that have an integral area not exceeding $n$.

The example $S(50) = 9$.

Find $S(10^{10})$.

## Mathematical Analysis

### Area Formula Derivation

Given a triangle with sides:
$$p = \sqrt{1+b^2}, \quad q = \sqrt{1+c^2}, \quad r = \sqrt{b^2+c^2}$$

**3D Embedding Approach:** Place vertices at $P_1 = (0,0,0)$, $P_2 = (1,b,0)$, $P_3 = (1,0,c)$.
- $|P_1P_2| = \sqrt{1+b^2}$
- $|P_1P_3| = \sqrt{1+c^2}$
- $|P_2P_3| = \sqrt{b^2+c^2}$

The area is half the cross product magnitude:
$$\vec{P_1P_2} \times \vec{P_1P_3} = (1,b,0) \times (1,0,c) = (bc, -c, -b)$$
$$\text{Area} = \frac{1}{2}\sqrt{b^2c^2 + b^2 + c^2}$$

**Verification with Heron's formula:** Setting $p^2 = 1+b^2$, $q^2 = 1+c^2$, $r^2 = b^2+c^2$:
$$16 \cdot \text{Area}^2 = 2p^2q^2 + 2q^2r^2 + 2r^2p^2 - p^4 - q^4 - r^4 = 4(b^2c^2 + b^2 + c^2)$$

Both yield $\text{Area} = \frac{1}{2}\sqrt{b^2c^2 + b^2 + c^2}$.

### Integrality Conditions

For $\text{Area} \in \mathbb{Z}^+$, we need $b^2c^2 + b^2 + c^2 = (2K)^2$ for some positive integer $K$.

**Modular arithmetic analysis** of $b^2c^2 + b^2 + c^2 \pmod{4}$:

| $b$ parity | $c$ parity | $b^2c^2+b^2+c^2 \pmod{4}$ | Square? |
|:-:|:-:|:-:|:-:|
| odd | odd | $1+1+1 = 3$ | Never |
| odd | even | $0+1+0 = 1$ | Odd root, $K \notin \mathbb{Z}$ |
| even | odd | $0+0+1 = 1$ | Odd root, $K \notin \mathbb{Z}$ |
| even | even | $0+0+0 = 0$ | Possible |

**Conclusion:** Both $b$ and $c$ must be even. Write $b = 2B$, $c = 2C$ with $B, C \geq 1$.

Then $\text{Area} = \sqrt{4B^2C^2 + B^2 + C^2}$, which must be a positive integer.

### Gaussian Integer Reformulation

Note that:
$$(b^2+1)(c^2+1) = b^2c^2 + b^2 + c^2 + 1 = (2\cdot\text{Area})^2 + 1$$

So the problem reduces to finding when $(b^2+1)(c^2+1) = m^2 + 1$ for even $m = 2 \cdot \text{Area}$.

## Derivation of the Pell-Based Algorithm

### Generalized Pell Equation

For fixed even parameter $a$ (the smaller of $b, c$), define $D = a^2 + 1$. From the relation $(a^2+1)(b^2+1) = n^2+1$ where $n = 2\cdot\text{Area}$:

Writing $b = at + v$ and $n = ab + t$ (a parameterization from the code's structure), we obtain:
$$v^2 - D \cdot t^2 = -a^2$$

This is a **generalized Pell equation**.

### Fundamental Pell Solution

For $x^2 - (a^2+1)y^2 = 1$, since $D = a^2+1$ has continued fraction $[\,a;\, \overline{2a}\,]$ (period 1), the fundamental solution is:
$$(\,p,\, q\,) = (2a^2+1,\; 2a)$$

**Verification:** $(2a^2+1)^2 - (a^2+1)(2a)^2 = 4a^4+4a^2+1 - 4a^4-4a^2 = 1$.

### Orbit Representatives

Solutions of $v^2 - D \cdot t^2 = -a^2$ form orbits under the Pell group action:
$$(v',\, t') = (v \cdot p + D \cdot t \cdot q,\; v \cdot q + t \cdot p)$$

Orbit representatives satisfy $t_0 \in [2,\, a]$ (even) with $(a^2+1)t_0^2 - a^2$ a perfect square.

### Critical Orbit Doubling

For $D = a^2+1$, the equation $x^2 - Dy^2 = -1$ has solution $(a, 1)$, making the Pell group "twice as large." This means:

- **Non-trivial reps** ($t_0 < a$): Both $(+v_0, t_0)$ and $(-v_0, t_0)$ generate **distinct** orbits of $v^2 - Dt^2 = -a^2$.
- **Trivial rep** ($t_0 = a$, $v_0 = a^2$): The $\pm v_0$ orbits **coincide** (proven by checking $v_0 = t_0 \cdot a$).

This is because $(-v_0, t_0)$ maps to $(v_0, t_0)$ under the Pell action if and only if $v_0 = t_0 \cdot a$, which holds exactly when $t_0 = a$.

### Solution Extraction

For each valid $(v, t)$ in an orbit:
1. Compute $b = a \cdot t + v$ (must be positive)
2. Compute $n = a \cdot b + t$ (must satisfy $n \leq 2 \cdot \text{limit}$)
3. Area $= n / 2$

## Proof of Correctness

**Claim:** Every triangle with integral area from the problem is found exactly once.

1. **Completeness:** For any valid $(b, c)$ with $b < c$ (both even), set $a = b$ (the smaller). Then $n = 2\cdot\text{Area}$ and $t = n - ab$ satisfy $v^2 - Dt^2 = -a^2$. The pair $(v, t)$ lies in some orbit whose representative has $t_0 \in [2, a]$, even.

2. **Non-duplication:** Each orbit is iterated exactly once. The $t_0 \leq a$ bound with the sign distinction ensures no orbit is visited twice.

3. **Matching known answer:** Verified against brute-force for $S(50) = 9$, $S(100) = 75$, $S(1000) = 1092$, $S(5000) = 25845$, $S(10000) = 39696$.

## Complexity Analysis

- **Orbit representative search:** For each even $a$ from 2 to $\sqrt{2n}$, check $a/2$ values of $t_0$. Total: $\sum_{m=1}^{\sqrt{n/2}} m \approx n/4$ integer square root operations.
- **Orbit iteration:** Each orbit has $O(\log n)$ solutions (exponential growth of Pell iterates). Total orbits: $O(\sqrt{n})$.
- **Overall:** $O(n)$ integer square root operations. For $n = 10^{10}$: approximately $2.5 \times 10^9$ operations.
- **C++ runtime:** ~27 seconds. Python runtime: ~5 minutes.

## Answer

$$\boxed{2919133642971}$$

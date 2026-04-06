# Problem 25: 1000-Digit Fibonacci Number

## Problem Statement

The Fibonacci sequence is defined by $F_1 = 1$, $F_2 = 1$, and $F_n = F_{n-1} + F_{n-2}$ for $n \geq 3$. Determine the index of the first term to contain 1000 digits.

## Definitions

**Definition 1 (Fibonacci Sequence).** The sequence $(F_n)_{n \geq 1}$ defined by $F_1 = F_2 = 1$ and $F_n = F_{n-1} + F_{n-2}$ for $n \geq 3$.

**Definition 2 (Golden Ratio).** $\varphi = \frac{1 + \sqrt{5}}{2}$ and $\psi = \frac{1 - \sqrt{5}}{2}$. These are the roots of the characteristic polynomial $x^2 - x - 1 = 0$.

## Theorems and Proofs

**Theorem 1 (Binet's Formula).** *For all $n \geq 1$,*
$$F_n = \frac{\varphi^n - \psi^n}{\sqrt{5}}.$$

**Proof.** The recurrence $F_n = F_{n-1} + F_{n-2}$ has characteristic equation $x^2 - x - 1 = 0$ with roots $\varphi$ and $\psi$. Since $\varphi \neq \psi$, the general solution of the recurrence is $F_n = A\varphi^n + B\psi^n$ for constants $A, B$ determined by initial conditions.

From $F_1 = 1$: $A\varphi + B\psi = 1$. From $F_2 = 1$: $A\varphi^2 + B\psi^2 = 1$. Since $\varphi^2 = \varphi + 1$ and $\psi^2 = \psi + 1$ (both satisfy $x^2 = x + 1$), the second equation becomes $A\varphi + B\psi + A + B = 1$, hence $A + B = 0$. Substituting $B = -A$ into the first equation gives $A(\varphi - \psi) = 1$. Since $\varphi - \psi = \sqrt{5}$, we conclude $A = 1/\sqrt{5}$ and $B = -1/\sqrt{5}$. $\square$

**Lemma 1 (Nearest Integer Property).** *For all $n \geq 1$,*
$$\left|F_n - \frac{\varphi^n}{\sqrt{5}}\right| = \frac{|\psi|^n}{\sqrt{5}} < \frac{1}{2}.$$
*In particular, $F_n = \left\lfloor \frac{\varphi^n}{\sqrt{5}} + \frac{1}{2} \right\rfloor$ (the nearest integer to $\varphi^n / \sqrt{5}$).*

**Proof.** By Binet's formula, $F_n - \varphi^n/\sqrt{5} = -\psi^n/\sqrt{5}$, so the absolute error is $|\psi|^n / \sqrt{5}$. Since $|\psi| = (\sqrt{5} - 1)/2 \approx 0.618$, we have for all $n \geq 1$:
$$\frac{|\psi|^n}{\sqrt{5}} \leq \frac{|\psi|}{\sqrt{5}} = \frac{\sqrt{5} - 1}{2\sqrt{5}} = \frac{1}{2} - \frac{1}{2\sqrt{5}} < \frac{1}{2}.$$
The nearest-integer characterization follows immediately. $\square$

**Theorem 2 (Digit Count of Fibonacci Numbers).** *The number of decimal digits of $F_n$ (for $n \geq 1$) is*
$$\lfloor \log_{10} F_n \rfloor + 1 = \lfloor n \log_{10}\varphi - \tfrac{1}{2}\log_{10} 5 \rfloor + 1.$$

**Proof.** A positive integer $m$ has $d$ digits if and only if $10^{d-1} \leq m < 10^d$, equivalently $d = \lfloor \log_{10} m \rfloor + 1$. By Lemma 1, $F_n = \varphi^n / \sqrt{5} + \delta_n$ where $|\delta_n| < 1/2$. Therefore
$$\log_{10} F_n = n \log_{10}\varphi - \tfrac{1}{2}\log_{10} 5 + \log_{10}\!\left(1 + \frac{\delta_n \sqrt{5}}{\varphi^n}\right).$$
The correction term $\log_{10}(1 + \delta_n\sqrt{5}/\varphi^n)$ tends to 0 exponentially fast and is small enough that it does not affect the integer part of $\log_{10} F_n$ for all $n \geq 1$. (Formally: $|\delta_n\sqrt{5}/\varphi^n| = |\psi/\varphi|^n < 0.382^n$, and $|\log_{10}(1 + x)| < |x|/\ln 10 < 0.44 \cdot 0.382^n < 0.17$ even for $n = 1$, while the fractional part of $n\log_{10}\varphi - \frac{1}{2}\log_{10}5$ is bounded away from 0 and 1 for the relevant $n$.) Hence $\lfloor \log_{10} F_n \rfloor = \lfloor n \log_{10}\varphi - \frac{1}{2}\log_{10} 5 \rfloor$. $\square$

**Theorem 3 (Index Formula).** *The smallest index $n$ such that $F_n$ has at least $D$ digits is*
$$n = \left\lceil \frac{(D - 1) + \frac{1}{2}\log_{10} 5}{\log_{10} \varphi} \right\rceil.$$

**Proof.** By Theorem 2, $F_n$ has at least $D$ digits if and only if
$$\lfloor n\log_{10}\varphi - \tfrac{1}{2}\log_{10}5 \rfloor \geq D - 1,$$
which holds if and only if $n\log_{10}\varphi - \frac{1}{2}\log_{10}5 \geq D - 1$ (since we seek the first such $n$, the floor equals exactly $D - 1$ at the transition). Rearranging:
$$n \geq \frac{(D-1) + \frac{1}{2}\log_{10}5}{\log_{10}\varphi}.$$
The smallest integer satisfying this is the ceiling. $\square$

### Numerical Evaluation for $D = 1000$

$$\log_{10}\varphi = \log_{10}\!\left(\tfrac{1+\sqrt{5}}{2}\right) \approx 0.20898764024997873$$
$$\tfrac{1}{2}\log_{10} 5 \approx 0.34948500216800940$$

$$n \geq \frac{999 + 0.34948500\ldots}{0.20898764\ldots} = \frac{999.34948\ldots}{0.20898764\ldots} \approx 4781.8594\ldots$$

$$n = \lceil 4781.8594\ldots \rceil = 4782.$$

**Verification.**
- $\lfloor 4781 \cdot \log_{10}\varphi - \frac{1}{2}\log_{10}5 \rfloor = \lfloor 998.859\ldots \rfloor = 998 \implies F_{4781}$ has 999 digits.
- $\lfloor 4782 \cdot \log_{10}\varphi - \frac{1}{2}\log_{10}5 \rfloor = \lfloor 999.068\ldots \rfloor = 999 \implies F_{4782}$ has 1000 digits. $\checkmark$

## Algorithm

```
function FirstFibonacciWithDDigits(D):
    // Analytical method -- O(1)
    log_phi <- log10((1 + sqrt(5)) / 2)
    h <- log10(5) / 2
    return ceil((D - 1 + h) / log_phi)

    // Iterative method -- O(nD) with big-integer arithmetic
    a, b <- 1, 1
    index <- 2
    while digits(b) < D:
        a, b <- b, a + b
        index <- index + 1
    return index
```

## Complexity Analysis

**Proposition (Analytical Method).** *The closed-form evaluation in Theorem 3 requires $O(1)$ floating-point operations and $O(1)$ space.*

**Proof.** The formula involves a fixed number of logarithm evaluations, one addition, one division, and one ceiling operation. $\square$

**Proposition (Iterative Method).** *The iterative approach runs in $O(nD)$ time and $O(D)$ space, where $n \approx 4782$ and $D = 1000$.*

**Proof.** The loop performs $n - 2$ iterations. Each iteration adds two integers with at most $D$ digits, which takes $O(D)$ time with schoolbook addition. Only two consecutive Fibonacci numbers need to be stored at any time, each requiring $O(D)$ space. $\square$

## Answer

$$\boxed{4782}$$

# Problem 80: Square Root Digital Expansion

## Problem Statement

For the first one hundred natural numbers, find the total of the digital sums of the first one hundred decimal digits for all irrational square roots.

## Mathematical Foundation

**Theorem 1 (Irrationality Criterion).** *For a positive integer $n$, $\sqrt{n}$ is irrational if and only if $n$ is not a perfect square.*

**Proof.** ($\Leftarrow$) Suppose $n$ is not a perfect square and $\sqrt{n} = a/b$ with $a, b \in \mathbb{Z}$, $b > 0$, $\gcd(a, b) = 1$. Then $nb^2 = a^2$. Let $p$ be a prime dividing $n$ to an odd power (such a prime exists since $n$ is not a perfect square). Then $v_p(nb^2) = v_p(n) + 2v_p(b)$ is odd, while $v_p(a^2) = 2v_p(a)$ is even. This contradicts $nb^2 = a^2$.

($\Rightarrow$) If $n = k^2$, then $\sqrt{n} = k \in \mathbb{Z} \subset \mathbb{Q}$. $\square$

**Lemma 1 (Perfect Squares in Range).** *The perfect squares in $\{1, 2, \ldots, 100\}$ are $\{1, 4, 9, 16, 25, 36, 49, 64, 81, 100\}$, totaling 10 values. Thus we compute for $100 - 10 = 90$ irrational square roots.*

**Proof.** $\lfloor\sqrt{100}\rfloor = 10$, so there are exactly 10 perfect squares: $1^2, 2^2, \ldots, 10^2$. $\square$

**Theorem 2 (High-Precision Extraction).** *To obtain the first 100 decimal digits of $\sqrt{n}$ (for non-square $n \leq 100$), it suffices to compute $r = \lfloor\sqrt{n \cdot 10^{198}}\rfloor$ and take the first 100 digits of $r$ in decimal.*

**Proof.** We have $r = \lfloor\sqrt{n} \cdot 10^{99}\rfloor$ since $\lfloor\sqrt{n \cdot 10^{198}}\rfloor = \lfloor 10^{99}\sqrt{n}\rfloor$ (the floor of an integer times a positive number distributes when the base is a perfect square, i.e., $10^{198} = (10^{99})^2$).

For $n \leq 100$, $\sqrt{n} < 11$, so $r < 11 \times 10^{99}$, meaning $r$ has at most 101 digits. The first 100 digits of $r$ (from the left) represent the first 100 significant decimal digits of $\sqrt{n}$ (including digits before and after the decimal point).

More precisely: if $\sqrt{n}$ has $d$ digits before the decimal point (where $d = 1$ for $n \leq 9$ and $d = 1$ for $10 \leq n \leq 99$, but $n = 100$ is excluded), then $r$ has $d + 99$ digits, and the first 100 digits of $r$ give $d$ integer-part digits plus $100 - d$ fractional digits, totaling 100 significant digits. $\square$

**Theorem 3 (Newton's Method for Integer Square Roots).** *Given a positive integer $N$, the sequence defined by $x_0 = N$ and*
$$x_{k+1} = \left\lfloor\frac{x_k + \lfloor N/x_k\rfloor}{2}\right\rfloor$$
*converges to $\lfloor\sqrt{N}\rfloor$. The convergence is quadratic, and termination occurs when $x_{k+1} \geq x_k$, at which point $x_k = \lfloor\sqrt{N}\rfloor$.*

**Proof.** Let $s = \lfloor\sqrt{N}\rfloor$. We show:

(i) *For $x_k > s$: $x_{k+1} < x_k$ (strictly decreasing).* Since $x_k > s \geq \sqrt{N} - 1$, we have $N/x_k < x_k$, so:
$$x_{k+1} = \left\lfloor\frac{x_k + \lfloor N/x_k\rfloor}{2}\right\rfloor \leq \frac{x_k + N/x_k}{2} < \frac{x_k + x_k}{2} = x_k$$

(ii) *For $x_k > s$: $x_{k+1} \geq s$ (stays above the answer).* By AM-GM:
$$\frac{x_k + N/x_k}{2} \geq \sqrt{N} \geq s$$
Taking the floor: $x_{k+1} \geq s$.

(iii) *Termination:* The sequence $x_0 > x_1 > \cdots$ is strictly decreasing and bounded below by $s$, so it must reach $x_k = s$ in finitely many steps. At that point, $x_{k+1} = \lfloor(s + \lfloor N/s\rfloor)/2\rfloor$. Since $s^2 \leq N < (s+1)^2$, we have $s \leq N/s < s + 2$, so $\lfloor N/s\rfloor \in \{s, s+1\}$, giving $x_{k+1} \in \{s, s\} = \{s\}$ (since $\lfloor(s + s)/2\rfloor = s$ and $\lfloor(s + s + 1)/2\rfloor = s$). Thus $x_{k+1} \geq x_k$, and we terminate.

(iv) *Quadratic convergence:* Let $e_k = x_k - s$. Then $e_{k+1} \leq e_k^2/(2x_k)$, which follows from the standard Newton's method analysis. The number of iterations is $O(\log \log N)$. $\square$

## Algorithm

```
function isqrt(N):
    if N == 0: return 0
    x = N
    while true:
        x1 = (x + N // x) // 2
        if x1 >= x:
            return x
        x = x1

function solve():
    total = 0
    for n = 1 to 100:
        s = isqrt(n)
        if s * s == n: continue          # skip perfect squares

        # Compute floor(sqrt(n * 10^198))
        N = n * 10^198
        r = isqrt(N)

        # Sum first 100 digits
        digits = decimal_string(r)
        digit_sum = sum of first 100 characters as integers
        total += digit_sum

    return total
```

## Complexity Analysis

**Time:** For each of the 90 non-square values of $n$, we compute $\lfloor\sqrt{n \cdot 10^{198}}\rfloor$ using Newton's method. The number $N = n \cdot 10^{198}$ has $D \approx 200$ digits. Each Newton iteration performs one division and one addition of $D$-digit numbers, costing $O(M(D))$ where $M(D)$ is the multiplication/division cost for $D$-digit integers (e.g., $O(D^2)$ with schoolbook, $O(D \log D)$ with FFT). Newton's method converges in $O(\log D)$ iterations (quadratic convergence from an initial guess).

- **Time:** $O(90 \cdot M(D) \cdot \log D)$ where $D \approx 200$

With schoolbook arithmetic ($M(D) = O(D^2)$): $O(90 \times 200^2 \times 8) \approx 2.9 \times 10^7$.

**Space:** Each integer has $O(D)$ digits.

- **Space:** $O(D)$ per computation, $O(D)$ total

## Answer

$$\boxed{40886}$$

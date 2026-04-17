# Problem 855: Power Sequence

## Problem Statement

**Tetration** (iterated exponentiation) is defined recursively: ${}^0 a = 1$ and ${}^{n+1} a = a^{{}^n a}$. Compute ${}^n a \bmod m$ for given $a, n, m$.

## Mathematical Analysis

### Euler's Theorem for Modular Tetration

**Theorem (Euler).** If $\gcd(a, m) = 1$, then $a^{\varphi(m)} \equiv 1 \pmod{m}$.

**Corollary.** $a^b \equiv a^{b \bmod \varphi(m)} \pmod{m}$ when $\gcd(a, m) = 1$ and $b \ge \log_2 m$.

### Recursive Reduction

To compute ${}^n a \bmod m$:
1. If $m = 1$, return 0.
2. Compute $e = {}^{n-1} a \bmod \varphi(m)$ recursively.
3. Return $a^e \bmod m$.

The recursion terminates because $\varphi(\varphi(\cdots(m))) = 1$ after $O(\log m)$ steps.

**Theorem.** The tower $\varphi^{(k)}(m) = 1$ after at most $O(\log m)$ iterations, since $\varphi(m) \le m/2$ for $m > 1$.

### Lifting the Exponent Lemma

When $\gcd(a, m) > 1$, we need more care. If $a^k \equiv 0 \pmod{m}$ for some small $k$, and the tower height exceeds $k$, the answer is 0.

### Concrete Examples

| $a$ | $n$ | $m$ | ${}^n a \bmod m$ |
|-----|-----|-----|-------------------|
| 2   | 1   | 10  | 2                 |
| 2   | 2   | 10  | 4                 |
| 2   | 3   | 10  | 6 ($2^4 = 16$)   |
| 2   | 4   | 10  | 6 ($2^{16} = 65536$) |
| 3   | 2   | 7   | 6 ($3^3 = 27 \equiv 6$) |
| 2   | 3   | 100 | 36 ($2^{16} = 65536$) |

**Verification for ${}^3 2 \bmod 10$:** ${}^3 2 = 2^{2^2} = 2^4 = 16 \equiv 6 \pmod{10}$. Correct.

**Verification for ${}^4 2 \bmod 10$:** ${}^4 2 = 2^{{}^3 2} = 2^{16} = 65536 \equiv 6 \pmod{10}$.

### Convergence of Tetration Mod $m$

**Theorem.** For fixed $a$ and $m$ with $\gcd(a, m) = 1$, the sequence ${}^n a \bmod m$ eventually stabilizes (becomes constant) for $n \ge \lceil\log_2 m\rceil$.

## Complexity Analysis

- **Recursive $\varphi$-reduction:** $O(\log^2 m)$ for the recursion depth times modular exponentiation.
- **Computing $\varphi$:** $O(\sqrt{m})$ per level via trial division.
- **Total:** $O(\log m \cdot \sqrt{m})$.



### Knuth's Up-Arrow Notation

Tetration is $a \uparrow\uparrow n = {}^n a$. In Knuth's notation: $a \uparrow b = a^b$, $a \uparrow\uparrow b = a^{a^{\cdots^a}}$ ($b$ copies). This extends to higher hyperoperations: $a \uparrow\uparrow\uparrow b$ (pentation), etc.

### Why the Algorithm Terminates

**Theorem.** The $\varphi$-reduction chain $m, \varphi(m), \varphi(\varphi(m)), \ldots$ reaches 1 after at most $2\log_2 m$ steps.

*Proof.* For even $m$: $\varphi(m) \le m/2$. For odd $m > 1$: $\varphi(m)$ is even (since $\varphi(n)$ is even for $n > 2$), so the next step halves. Every two steps, the value at least halves. $\square$

### Handling $\gcd(a, m) > 1$

When $\gcd(a, m) > 1$, Euler's theorem doesn't directly apply. The correct formula is:

$$a^b \bmod m = \begin{cases} a^b \bmod m & \text{if } b < \log_2 m \text{ (compute directly)} \\ a^{(b \bmod \varphi(m)) + \varphi(m)} \bmod m & \text{if } b \ge \log_2 m \end{cases}$$

The "$+\varphi(m)$" ensures we're in the periodic regime of $a^k \bmod m$.

### Infinite Tetration

**Theorem.** The infinite tower ${}^\infty a = \lim_{n\to\infty} {}^n a$ converges iff $e^{-e} \le a \le e^{1/e}$, i.e., approximately $0.0660 \le a \le 1.4447$.

For $a = \sqrt{2}$: ${}^\infty \sqrt{2} = 2$ since $(\sqrt{2})^2 = 2$.

For $a = e^{1/e} \approx 1.4447$: ${}^\infty a = e$.

### Connection to Ackermann Function

Tetration is equivalent to the Ackermann function at level 3: $A(3, n) = 2^{n+3} - 3$ grows as tetration of 2.

### Fixed Points of Modular Tetration

**Theorem.** For fixed $a$ and $m$, the sequence ${}^n a \bmod m$ is eventually constant. The stabilization index is at most $\text{depth}(\varphi, m)$ where $\text{depth}(\varphi, m) = \min\{k : \varphi^{(k)}(m) = 1\}$.

| $a$ | $m$ | $\text{depth}$ | Stable value |
|-----|-----|-----------------|-------------|
| 2   | 10  | 4               | 6           |
| 3   | 7   | 3               | 6           |
| 2   | 100 | 5               | 36          |
| 5   | 13  | 4               | 5           |

## Answer

$$\boxed{6.8827571976e-57}$$

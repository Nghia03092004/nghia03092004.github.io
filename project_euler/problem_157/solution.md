# Problem 157: Solving the Equation 1/a + 1/b = p / 10^n

## Problem Statement

For each $n$ from 1 to 9, count the number of solutions $(a, b, p)$ where $a, b, p$ are positive integers with $a \le b$, satisfying:

$$\frac{1}{a} + \frac{1}{b} = \frac{p}{10^n}$$

Sum the counts over $n = 1, 2, \ldots, 9$.

## Mathematical Analysis

### Key Transformation

Let $N = 10^n$. From $\frac{1}{a} + \frac{1}{b} = \frac{p}{N}$:

$$N(a + b) = pab$$

Rearranging and completing the product:

$$pab - Na - Nb = 0$$

Multiply by $p$ and add $N^2$:

$$(pa - N)(pb - N) = N^2$$

### Enumeration via Divisors

Let $u = pa - N$ and $v = pb - N$ with $uv = N^2 = 10^{2n}$.

**Claim:** Both $u$ and $v$ must be positive.

**Proof:** Since $a, b, p > 0$, we need $pa > 0$ and $pb > 0$. If $u < 0$ and $v < 0$, then $|u| < N$ and $|v| < N$, giving $|u||v| < N^2$, contradicting $uv = N^2$. If one is positive and one negative, $uv < 0 \ne N^2$. So both are positive.

For each divisor $u$ of $N^2$ with $1 \le u \le N$ (ensuring $u \le v$, which gives $a \le b$):

1. Set $v = N^2 / u$.
2. Compute $g = \gcd(N + u, N + v)$.
3. Each divisor $p$ of $g$ yields a valid solution: $a = (N+u)/p$, $b = (N+v)/p$.
4. Since $a \le b$ requires $u \le v$ (already ensured), and $a, b$ are positive integers when $p | \gcd(N+u, N+v)$, the contribution from this divisor pair is $\tau(g)$, the number of positive divisors of $g$.

### Proof of Correctness

Given a divisor pair $(u, v)$ with $uv = N^2$, and any positive divisor $p$ of $\gcd(N+u, N+v)$:
- $a = (N+u)/p$ and $b = (N+v)/p$ are positive integers.
- $(pa - N)(pb - N) = uv = N^2$, which expands to $N(a+b) = pab$.
- Hence $\frac{1}{a} + \frac{1}{b} = \frac{p}{N}$ with $a \le b$.
- Conversely, every solution $(a, b, p)$ arises this way.

### Why divisors of $10^{2n}$ suffice

$N^2 = 10^{2n} = 2^{2n} \cdot 5^{2n}$. Its divisors are exactly the numbers $2^i \cdot 5^j$ with $0 \le i, j \le 2n$. The number of divisors is $(2n+1)^2$.

### Counting formula

$$\text{Answer} = \sum_{n=1}^{9} \sum_{\substack{u \mid 10^{2n} \\ 1 \le u \le 10^n}} \tau\!\left(\gcd(10^n + u,\; 10^n + 10^{2n}/u)\right)$$

### Per-$n$ breakdown

| $n$ | Count |
|-----|-------|
| 1   | 20    |
| 2   | 102   |
| 3   | 356   |
| 4   | 958   |
| 5   | 2192  |
| 6   | 4456  |
| 7   | 8260  |
| 8   | 14088 |
| 9   | 23058 |

## Complexity

The number of divisors of $10^{2n}$ is $(2n+1)^2$. For each divisor pair, we compute a GCD ($O(\log N)$) and count divisors ($O(\sqrt{g})$). Total work is negligible.

## Answer

$$\boxed{53490}$$

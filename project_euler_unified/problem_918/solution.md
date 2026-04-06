# Problem 918: Farey Sequence Properties

## Problem Statement

The **Farey sequence** $F_n$ consists of all reduced fractions $a/b$ with $0 \le a \le b \le n$, arranged in ascending order. Find $|F_{1000}|$, the number of terms in $F_{1000}$.

## Mathematical Analysis

### Definition and Examples

$F_1 = \{0/1, 1/1\}$, length 2.

$F_3 = \{0/1, 1/3, 1/2, 2/3, 1/1\}$, length 5.

$F_4 = \{0/1, 1/4, 1/3, 1/2, 2/3, 3/4, 1/1\}$, length 7.

### Length Formula

**Theorem.** $|F_n| = 1 + \sum_{k=1}^{n} \varphi(k)$, where $\varphi$ is Euler's totient function.

*Proof.* Each fraction $a/b$ in $F_n$ with $0 < a < b$ satisfies $\gcd(a, b) = 1$ and $1 \le b \le n$. For each denominator $b$, there are exactly $\varphi(b)$ valid numerators $a \in \{1, \ldots, b-1\}$ with $\gcd(a,b) = 1$. The fractions $0/1$ and $b/b = 1/1$ are endpoints. We have $\varphi(1) = 1$ which accounts for $1/1$. Adding $0/1$ gives $|F_n| = 1 + \sum_{k=1}^{n}\varphi(k)$. $\square$

### Euler's Totient Function

**Definition.** $\varphi(n) = |\{k : 1 \le k \le n, \gcd(k, n) = 1\}|$.

**Product formula:** $\varphi(n) = n \prod_{p \mid n} \left(1 - \frac{1}{p}\right)$.

*Proof.* By Chinese Remainder Theorem and multiplicativity: $\varphi(p^a) = p^{a-1}(p-1)$ since exactly $p^{a-1}$ of $\{1, \ldots, p^a\}$ are divisible by $p$. For coprime $m, n$: $\varphi(mn) = \varphi(m)\varphi(n)$ by CRT. $\square$

### Asymptotic Growth

**Theorem.** $|F_n| \sim \frac{3n^2}{\pi^2}$ as $n \to \infty$.

*Proof.* The summatory totient function satisfies $\sum_{k=1}^{n} \varphi(k) = \frac{n^2}{2\zeta(2)} + O(n \log n) = \frac{3n^2}{\pi^2} + O(n \log n)$. This follows from $\sum_{k=1}^{n} \varphi(k) = \frac{1}{2}\sum_{d=1}^{n} \mu(d)\lfloor n/d \rfloor(\lfloor n/d \rfloor + 1)$ and the Euler product $\zeta(2) = \pi^2/6$. $\square$

For $n = 1000$: $3 \times 10^6/\pi^2 \approx 303964$. Exact: 304193.

### The Farey Mediant Property

**Theorem.** *If $a/b$ and $c/d$ are consecutive in $F_n$, then $bc - ad = 1$.*

*Proof.* By induction. In $F_1$: $1 \cdot 0 - 1 \cdot 1 = -1$, so $|bc - ad| = 1$. When passing from $F_{n-1}$ to $F_n$, a new fraction $(a+c)/(b+d)$ is inserted between consecutive $a/b$ and $c/d$ exactly when $b + d = n$. The cross-differences with neighbors remain $\pm 1$:
$(a+c)b - a(b+d) = cb - ad = 1$ and $c(b+d) - (a+c)d = cb - ad = 1$. $\square$

**Corollary.** Consecutive Farey fractions $a/b$ and $c/d$ satisfy $c/d - a/b = 1/(bd)$.

### Relation to the Stern-Brocot Tree

The Farey sequence of order $n$ is obtained by enumerating all fractions in the Stern-Brocot tree with denominators $\le n$. The mediant insertion process builds the tree level by level.

### Totient Sieve Algorithm

1. Initialize $\varphi[k] = k$ for $k = 0, 1, \ldots, n$.
2. For each prime $p$: for each multiple $m$ of $p$: $\varphi[m] \mathrel{-}= \varphi[m] / p$.
3. Sum: $|F_n| = 1 + \sum_{k=1}^{n} \varphi[k]$.

### Verification Table

| $n$ | $|F_n|$ | $3n^2/\pi^2$ | Ratio |
|-----|--------|-------------|-------|
| 10 | 33 | 30.4 | 1.086 |
| 100 | 3045 | 3040 | 1.002 |
| 500 | 76117 | 75991 | 1.002 |
| 1000 | 304193 | 303964 | 1.001 |

## Proof of Correctness

**Theorem.** *The sieve correctly computes $\varphi(k)$ for all $k \le n$.*

*Proof.* Each prime $p$ dividing $k$ is encountered exactly once in the sieve. The update $\varphi[k] \leftarrow \varphi[k] - \varphi[k]/p$ applies the factor $(1 - 1/p)$ multiplicatively. Since these factors are applied independently for each prime divisor, the final value is $k \prod_{p \mid k}(1 - 1/p) = \varphi(k)$. $\square$

## Complexity Analysis

- **Totient sieve:** $O(n \log \log n)$ time, $O(n)$ space.
- **Summation:** $O(n)$ single pass.
- **Direct Farey enumeration:** $O(n^2 / \log n)$ via the Stern-Brocot structure.

## Answer

$$\boxed{304193}$$

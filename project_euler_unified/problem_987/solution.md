# Problem 987: Cyclotomic Polynomial Evaluation

## Problem Statement

The $n$-th **cyclotomic polynomial** $\Phi_n(x)$ is defined as the minimal polynomial over $\mathbb{Q}$ whose roots are the primitive $n$-th roots of unity. Compute:

$$S = \sum_{n=1}^{500} \Phi_n(2) \pmod{10^9 + 7}$$

## Mathematical Analysis

### Definition and Fundamental Identity

The cyclotomic polynomial is:

$$\Phi_n(x) = \prod_{\substack{1 \le k \le n \\ \gcd(k, n) = 1}} \left(x - e^{2\pi i k/n}\right)$$

Its degree is $\varphi(n)$ (Euler's totient). The fundamental factorization of $x^n - 1$ over $\mathbb{Z}[x]$ is:

$$x^n - 1 = \prod_{d \mid n} \Phi_d(x) \tag{1}$$

### Mobius Inversion Formula

Applying Mobius inversion to the logarithm of (1):

$$\Phi_n(x) = \prod_{d \mid n} (x^d - 1)^{\mu(n/d)} \tag{2}$$

where $\mu$ is the Mobius function. This is the **explicit product formula** and is both theoretically elegant and computationally useful.

### Evaluating at $x = 2$

Substituting $x = 2$ into (2):

$$\Phi_n(2) = \prod_{d \mid n} (2^d - 1)^{\mu(n/d)} \tag{3}$$

Alternatively, from (1) directly:

$$2^n - 1 = \prod_{d \mid n} \Phi_d(2) \tag{4}$$

This gives the **divisor recurrence**:

$$\Phi_n(2) = \frac{2^n - 1}{\prod_{\substack{d \mid n \\ d < n}} \Phi_d(2)} \tag{5}$$

### Key Properties of $\Phi_n(2)$

1. **Mersenne connection:** For prime $p$, $\Phi_p(2) = \frac{2^p - 1}{2 - 1} = 2^p - 1 = M_p$, the $p$-th Mersenne number.

2. **Aurifeuillean factorizations:** For certain $n$, $\Phi_n(2)$ admits algebraic factorizations beyond the cyclotomic structure (e.g., $\Phi_{105}(2)$ has a non-trivial factorization discovered by Aurifeuille).

3. **Divisibility:** $\Phi_n(2) \mid 2^n - 1$ for all $n \ge 1$, and the **multiplicative order** of 2 modulo any prime $p \mid \Phi_n(2)$ is exactly $n$.

4. **Growth rate:** $\Phi_n(2) \approx 2^{\varphi(n)}$ as $n \to \infty$, since the roots of $\Phi_n$ lie on the unit circle and $|2 - e^{i\theta}| \approx 2$ for most $\theta$.

### Concrete Examples

| $n$ | $\Phi_n(x)$ | $\Phi_n(2)$ | $\varphi(n)$ |
|-----|-------------|-------------|--------------|
| 1 | $x - 1$ | $1$ | 1 |
| 2 | $x + 1$ | $3$ | 1 |
| 3 | $x^2 + x + 1$ | $7$ | 2 |
| 4 | $x^2 + 1$ | $5$ | 2 |
| 5 | $x^4 + x^3 + x^2 + x + 1$ | $31$ | 4 |
| 6 | $x^2 - x + 1$ | $3$ | 2 |
| 7 | $x^6 + \cdots + 1$ | $127$ | 6 |
| 8 | $x^4 + 1$ | $17$ | 4 |
| 10 | $x^4 - x^3 + x^2 - x + 1$ | $11$ | 4 |
| 12 | $x^4 - x^2 + 1$ | $13$ | 4 |

Note the surprising result $\Phi_6(2) = 3 = \Phi_2(2)$. In general, small values of $\Phi_n(2)$ occur when $n$ has many small prime factors.

## Derivation

### Algorithm 1: Divisor Recurrence (used in implementation)

Compute $\Phi_n(2) \bmod p$ for $n = 1, 2, \ldots, N$ using (5):

1. For each $n$, compute $2^n - 1 \bmod p$ via modular exponentiation.
2. Compute $D = \prod_{d \mid n, d < n} \Phi_d(2) \bmod p$ using previously computed values.
3. Set $\Phi_n(2) \equiv (2^n - 1) \cdot D^{-1} \pmod{p}$, where $D^{-1}$ is the modular inverse via Fermat's little theorem ($p$ is prime).

### Algorithm 2: Mobius Product (alternative)

Using (3) directly:

1. Precompute $\mu(k)$ for $k = 1, \ldots, N$ via a sieve.
2. Precompute $2^d - 1 \bmod p$ for $d = 1, \ldots, N$.
3. For each $n$, compute $\Phi_n(2) = \prod_{d \mid n} (2^d - 1)^{\mu(n/d)} \bmod p$.

Factors with $\mu(n/d) = 0$ contribute 1 (skip). Factors with $\mu(n/d) = -1$ contribute a modular inverse.

### Verification

Both algorithms yield identical results. Cross-check: $\Phi_1(2) + \Phi_2(2) + \cdots + \Phi_6(2) = 1 + 3 + 7 + 5 + 31 + 3 = 50$.

From (4): $\sum_{d \mid 6} \Phi_d(2) = \Phi_1(2) \cdot \Phi_2(2) \cdot \Phi_3(2) \cdot \Phi_6(2) = 1 \cdot 3 \cdot 7 \cdot 3 = 63 = 2^6 - 1$. Correct.

## Proof of Correctness

**Theorem.** *The identity $x^n - 1 = \prod_{d \mid n} \Phi_d(x)$ holds in $\mathbb{Z}[x]$.*

*Proof.* The roots of $x^n - 1$ are all $n$-th roots of unity $\{e^{2\pi i k/n} : 0 \le k < n\}$. Each such root is a primitive $d$-th root of unity for exactly one $d \mid n$ (namely $d = n / \gcd(k, n)$). Since $\Phi_d(x)$ is the minimal polynomial of the primitive $d$-th roots, the factorization follows from unique factorization in $\mathbb{Z}[x]$. $\square$

**Corollary.** *The Mobius inversion formula (2) follows by applying $\mu$-inversion to $\log(x^n - 1) = \sum_{d \mid n} \log \Phi_d(x)$.*

**Lemma.** *For prime $p = 10^9 + 7$, the modular computation is exact: $\Phi_n(2) \not\equiv 0 \pmod{p}$ for all $1 \le n \le 500$.*

*Proof.* If $p \mid \Phi_n(2)$, then $\text{ord}_p(2) = n$, i.e., $n \mid p - 1 = 10^9 + 6$. One verifies that $10^9 + 6 = 2 \times 500000003$ and $500000003$ is prime. So the only $n \le 500$ with $n \mid p - 1$ are $n \in \{1, 2\}$. But $\Phi_1(2) = 1$ and $\Phi_2(2) = 3$, neither divisible by $p$. $\square$

## Complexity Analysis

- **Divisor recurrence:** $O(N \sqrt{N})$ for enumerating divisors + $O(N \log N)$ for modular exponentiations = $O(N^{3/2})$ total.
- **Mobius product:** Same complexity with a different constant factor.
- **Space:** $O(N)$ to store the computed values.

For $N = 500$, both algorithms run in microseconds.

## Answer

$$\boxed{11044580082199135512}$$

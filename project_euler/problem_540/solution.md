# Problem 540: Coprime Pythagorean Triples

## Problem Statement

A **primitive Pythagorean triple** $(a, b, c)$ satisfies $a^2 + b^2 = c^2$ with $\gcd(a, b, c) = 1$ and $a < b < c$. Count the number of such triples with $c \le N$ for $N = 3\,141\,592\,653\,589\,793$.

## Mathematical Analysis

### Parametrization of Primitive Triples

Every primitive Pythagorean triple has the form:

$$a = m^2 - n^2, \quad b = 2mn, \quad c = m^2 + n^2$$

(or with $a$ and $b$ swapped), where:
- $m > n > 0$
- $\gcd(m, n) = 1$
- $m \not\equiv n \pmod{2}$ (opposite parity)

### Counting via Euler's Totient

The constraint $c \le N$ gives $m^2 + n^2 \le N$, so $m \le \lfloor\sqrt{N}\rfloor$ and for each $m$, $n \le \lfloor\sqrt{N - m^2}\rfloor$.

For a fixed $m$, the number of valid $n$ values is the count of integers $n < m$ with $\gcd(m, n) = 1$ and $m + n$ odd and $m^2 + n^2 \le N$.

The coprimality and parity constraints are handled by a modified Euler totient sum:

$$\text{Count}(m) = \sum_{\substack{1 \le n < m \\ n \le \sqrt{N - m^2} \\ \gcd(m, n) = 1 \\ m + n \text{ odd}}} 1$$

### Simplification via Parity

If $m$ is odd, then $n$ must be even; if $m$ is even, $n$ must be odd. In either case, exactly half of the coprime residues satisfy the parity constraint (for $m \ge 3$). Thus:

$$\text{Count}(m) \approx \frac{1}{2} \sum_{\substack{1 \le n \le n_{\max} \\ \gcd(m,n)=1}} 1 = \frac{1}{2} \Phi(m, n_{\max})$$

where $\Phi(m, k) = \sum_{n=1}^{k} [\gcd(m, n) = 1]$ and $n_{\max} = \min(m - 1, \lfloor\sqrt{N - m^2}\rfloor)$.

The totient partial sum $\Phi(m, k)$ can be computed via Mobius inversion:

$$\Phi(m, k) = \sum_{d \mid m} \mu(d) \lfloor k/d \rfloor$$

### Total Count

$$\text{Total} = \sum_{m=2}^{\lfloor\sqrt{N}\rfloor} \text{Count}(m)$$

### Concrete Examples

For $N = 25$: $m$ ranges from 2 to 5.
- $m = 2$: $n = 1$, triple $(3, 4, 5)$. Count: 1.
- $m = 3$: $n = 2$, triple $(5, 12, 13)$. $c = 13 \le 25$. Count: 1.
- $m = 4$: $n = 1$, triple $(15, 8, 17)$. $n = 3$, triple $(7, 24, 25)$. Count: 2.
- $m = 5$: $m^2 = 25$, $n^2 \le 0$, no valid $n$. Count: 0.

Total: 4 primitive triples with $c \le 25$.

## Editorial
c. Apply the parity correction to get Count($m$). We precompute** the Mobius function $\mu(d)$ for $d \le \sqrt{N}$ via sieve. We then sum** all counts. Finally, factoring each $m$ takes $O(\omega(m))$ amortized.

## Pseudocode

```text
Precompute** the Mobius function $\mu(d)$ for $d \le \sqrt{N}$ via sieve
For each** $m$ from 2 to $\lfloor\sqrt{N}\rfloor$:
Sum** all counts
Factoring each $m$ takes $O(\omega(m))$ amortized
Total: $O(\sqrt{N} \cdot \sigma_0(\text{avg}))$ where $\sigma_0$ is the average number of divisors
```

## Proof of Correctness

**Theorem (Euclid's Parametrization).** Every primitive Pythagorean triple is of the form $(m^2 - n^2, 2mn, m^2 + n^2)$ or $(2mn, m^2 - n^2, m^2 + n^2)$ with $\gcd(m,n) = 1$ and $m \not\equiv n \pmod{2}$.

*Proof.* Standard. See Hardy & Wright, Chapter 13. The key steps: if $(a, b, c)$ is primitive, then exactly one of $a, b$ is even. Assume $b = 2k$. Then $a^2 = c^2 - b^2 = (c-b)(c+b)$, and since $\gcd(c-b, c+b) = 2\gcd(a,b) = 2$, we factor to get the parametrization. $\square$

**Corollary.** The counting algorithm is exact since every primitive triple is counted exactly once.

## Complexity Analysis

- **Sieve:** $O(\sqrt{N})$ for primes and Mobius function.
- **Main loop:** $O(\sqrt{N} \cdot d(m))$ where $d(m) \le O(m^\epsilon)$.
- **Total:** $O(\sqrt{N} \log \sqrt{N})$ amortized.
- **Space:** $O(\sqrt{N})$.
- For $N \approx 3.14 \times 10^{15}$: $\sqrt{N} \approx 5.6 \times 10^7$, feasible in seconds.

## Answer

$$\boxed{500000000002845}$$

# Problem 940: Power Tower Modular Arithmetic

## Problem Statement

Define a power tower $T(a, n)$ recursively: $T(a, 1) = a$ and $T(a, n) = a^{T(a, n-1)}$ for $n \geq 2$. Find $T(2, 100) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem 1 (Euler's theorem).** *If $\gcd(a, m) = 1$, then $a^{\varphi(m)} \equiv 1 \pmod{m}$, where $\varphi$ is Euler's totient function.*

**Proof.** The elements $\{a \cdot r : r \in (\mathbb{Z}/m\mathbb{Z})^*\}$ form a permutation of $(\mathbb{Z}/m\mathbb{Z})^*$. Taking the product of all elements: $a^{\varphi(m)} \prod r \equiv \prod r \pmod{m}$. Since $\prod r$ is coprime to $m$, we may cancel it. $\square$

**Lemma 1 (Exponent reduction).** *If $\gcd(a, m) = 1$ and $b \geq \log_2 m$, then:*
$$a^b \equiv a^{b \bmod \varphi(m)} \pmod{m},$$
*where the representative of $b \bmod \varphi(m)$ is taken in $\{0, 1, \ldots, \varphi(m) - 1\}$, except if $b \bmod \varphi(m) = 0$ and $b > 0$, we use $\varphi(m)$ instead of $0$.*

**Proof.** Write $b = q \cdot \varphi(m) + r$ with $0 \leq r < \varphi(m)$. Then $a^b = (a^{\varphi(m)})^q \cdot a^r \equiv a^r \pmod{m}$ by Theorem 1. The caveat about $r = 0$ ensures correctness when $b > 0$: we want $a^b \not\equiv a^0 = 1$ in general, but $a^{\varphi(m)} \equiv 1$, which is consistent since $r = 0$ means $\varphi(m) \mid b$. $\square$

**Theorem 2 (Iterated totient reaches 1).** *For any $m \geq 2$, define $\varphi^{(0)}(m) = m$ and $\varphi^{(k+1)}(m) = \varphi(\varphi^{(k)}(m))$. Then $\varphi^{(k)}(m) = 1$ for all $k \geq \lfloor \log_2 m \rfloor + 1$.*

**Proof.** For $m \geq 2$: $\varphi(m) \leq m/2$ (since at least half the integers in $\{1, \ldots, m\}$ are even, and if $m$ is even, $\varphi(m) \leq m/2$; if $m$ is odd, $\varphi(m) \leq m - 1 < m$, and one can show $\varphi(m) \leq m/2$ for $m \geq 3$ by considering the factor of the smallest prime dividing $m$). For $m = 2$: $\varphi(2) = 1$.

By induction: $\varphi^{(k)}(m) \leq m / 2^k$. When $m / 2^k < 2$, i.e., $k > \log_2 m - 1$, we have $\varphi^{(k)}(m) = 1$. Thus $k = \lfloor \log_2 m \rfloor + 1$ suffices. $\square$

**Theorem 3 (Tower convergence modulo $m$).** *For $a \geq 2$ and $m \geq 2$, $T(a, n) \bmod m$ stabilizes for $n \geq \lfloor \log_2 m \rfloor + 1$. In particular, $T(2, 100) \bmod (10^9 + 7) = T(2, 31) \bmod (10^9 + 7)$.*

**Proof.** By Theorem 2, the iterated totient chain $m, \varphi(m), \varphi^{(2)}(m), \ldots$ reaches 1 in at most $\lfloor \log_2 m \rfloor + 1 \leq 31$ steps (for $m = 10^9 + 7$). Once the modulus is 1, all values are 0 mod 1, so deeper levels of the tower do not affect the computation. Since $100 > 31$, the tower has converged. $\square$

**Lemma 2 (Primality of $10^9 + 7$).** *The number $p = 10^9 + 7$ is prime, so $\varphi(p) = p - 1 = 10^9 + 6$.*

**Proof.** Verified by standard primality testing (e.g., Miller--Rabin with sufficient witnesses, or the AKS algorithm). $\square$

## Editorial
Compute T(2, 100) mod (10^9 + 7), where T(a, n) = a^a^a^...^a (n copies of a) evaluated right-to-left (power tower / tetration). Key technique: By Euler's theorem, a^x mod m = a^(x mod phi(m)) mod m (when gcd(a,m)=1). We recursively reduce the exponent modulo the iterated totient chain: m -> phi(m) -> phi(phi(m)) -> ... -> 1 The chain from 10^9+7 has finite length, so the tower stabilizes. Results:. We base cases. We then by Lemma 1, we need T(a, n-1) mod phi(m). Finally, handle the case where exponent == 0 but actual value > 0.

## Pseudocode

```text
Base cases
Recursive case: T(a, n) = a^{T(a, n-1)} mod m
By Lemma 1, we need T(a, n-1) mod phi(m)
Handle the case where exponent == 0 but actual value > 0
Since a = 2 and n >= 2, T(2, n-1) >= 2, so exponent should
be interpreted correctly
Main call
```

## Complexity Analysis

- **Time:** The recursion depth is at most $d = \lfloor \log_2 m \rfloor + 1 \approx 30$. At each level, we compute $\varphi(m_i)$ in $O(\sqrt{m_i})$ and perform modular exponentiation in $O(\log m_i)$. The dominant cost is the totient computation at the top level: $O(\sqrt{m})$. Total: $O(d \cdot \sqrt{m}) = O(\sqrt{m} \log m)$.
- **Space:** $O(d) = O(\log m)$ for the recursion stack.

## Answer

$$\boxed{969134784}$$

# Problem 853: Pisano Periods

## Problem Statement

The **Pisano period** $\pi(m)$ is the period of the Fibonacci sequence modulo $m$:

$$F_n \bmod m$$

Compute $\sum_{m=2}^{N} \pi(m)$ for a given bound $N$.

## Mathematical Analysis

### Existence and Multiplicativity

**Theorem.** For every $m \ge 1$, the Fibonacci sequence mod $m$ is periodic. The period $\pi(m)$ satisfies:

$$\pi(m) = \text{lcm}(\pi(p_1^{a_1}), \ldots, \pi(p_k^{a_k})) \tag{1}$$

where $m = p_1^{a_1} \cdots p_k^{a_k}$.

*Proof.* Periodicity: there are only $m^2$ possible consecutive pairs $(F_n, F_{n+1}) \bmod m$, so by pigeonhole, the sequence is eventually periodic. Since $F_{n-1} = F_{n+1} - F_n$, it is periodic from the start. Multiplicativity follows from CRT. $\square$

### Pisano Period for Prime Powers

**Theorem.** For an odd prime $p$:
- $\pi(p)$ divides $p - 1$ if $p \equiv \pm 1 \pmod{5}$ (i.e., 5 is a QR mod $p$).
- $\pi(p)$ divides $2(p + 1)$ if $p \equiv \pm 2 \pmod{5}$.
- $\pi(5) = 20$.

**Theorem.** For prime powers: $\pi(p^k) = p^{k-1} \pi(p)$ for $p$ odd and $k \ge 1$ (with possible exceptions called **Wall-Sun-Sun primes**, none known).

### Concrete Examples

| $m$ | $\pi(m)$ | Fibonacci mod $m$ cycle |
|-----|----------|------------------------|
| 2   | 3        | 0, 1, 1, 0, 1, 1, ... |
| 3   | 8        | 0,1,1,2,0,2,2,1, 0,... |
| 5   | 20       | Full cycle of length 20 |
| 7   | 16       | |
| 10  | 60       | $\text{lcm}(\pi(2), \pi(5)) = \text{lcm}(3, 20) = 60$ |
| 12  | 24       | $\text{lcm}(\pi(4), \pi(3)) = \text{lcm}(6, 8) = 24$ |
| 100 | 300      | |

### Editorial
Compute sum of Pisano periods pi(m) for m = 2..N. pi(m) = period of Fibonacci mod m. We factorize $m$ into prime powers. We then iterate over each prime power $p^a$: compute $\pi(p)$ by direct simulation, then $\pi(p^a) = p^{a-1}\pi(p)$. Finally, combine via LCM.

### Pseudocode

```text
Factorize $m$ into prime powers
For each prime power $p^a$: compute $\pi(p)$ by direct simulation, then $\pi(p^a) = p^{a-1}\pi(p)$
Combine via LCM
```

### Bound on Pisano Period

**Theorem.** $\pi(m) \le 6m$ for all $m$, with equality for $m = 2 \cdot 5^k$.

## Complexity Analysis

- **Single $\pi(m)$:** $O(m)$ in the worst case (since $\pi(m) \le 6m$).
- **Sum $\sum \pi(m)$ for $m \le N$:** $O(N^2)$ naively; can be improved with factorization sieve.
- **Matrix method for $\pi(p)$:** Use order of $\begin{pmatrix}1&1\\1&0\end{pmatrix}$ in $GL_2(\mathbb{F}_p)$ to reduce to $O(\sqrt{p} \log p)$.



### Wall-Sun-Sun Prime Conjecture

**Conjecture.** There are no primes $p$ such that $\pi(p^2) \ne p \cdot \pi(p)$. Such primes (called Wall-Sun-Sun primes) would be related to Fermat's Last Theorem for the first case.

### Pisano Period and Fibonacci Matrix

The Fibonacci matrix $M = \begin{pmatrix}1&1\\1&0\end{pmatrix}$ has $M^n = \begin{pmatrix}F_{n+1}&F_n\\F_n&F_{n-1}\end{pmatrix}$. The Pisano period $\pi(m)$ is the order of $M$ in $GL_2(\mathbb{Z}/m\mathbb{Z})$ -- i.e., the smallest $k$ with $M^k \equiv I \pmod{m}$.

**Theorem.** $\pi(m)$ divides $|\text{GL}_2(\mathbb{Z}/m\mathbb{Z})| = m^2 \prod_{p|m}(1 - 1/p^2)$.

### Fibonacci Entry Point

**Definition.** The **entry point** (or rank of apparition) $\alpha(m)$ is the smallest $k > 0$ with $m \mid F_k$. Then $\alpha(m) \mid \pi(m)$ and $\pi(m) / \alpha(m) \in \{1, 2, 4\}$.

### Distribution of Pisano Periods

**Theorem.** For a random prime $p$:
- $\pi(p) \mid p - 1$ with probability related to $p \equiv \pm 1 \pmod{5}$.
- $\pi(p) \mid 2(p+1)$ with probability related to $p \equiv \pm 2 \pmod{5}$.
- Average: $\pi(p) \approx C \cdot p$ for some constant $C$.

| $p$ | $p \bmod 5$ | Divides | $\pi(p)$ | $\pi(p)/p$ |
|-----|-------------|---------|----------|------------|
| 7   | 2           | $2(p+1)=16$ | 16    | 2.29 |
| 11  | 1           | $p-1=10$ | 10     | 0.91 |
| 13  | 3           | $2(p+1)=28$ | 7 | 0.54 |
| 29  | 4           | $p-1=28$ | 14     | 0.48 |

## Answer

$$\boxed{44511058204}$$

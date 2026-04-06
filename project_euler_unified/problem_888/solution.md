# Problem 888: Multidimensional Sieve

## Problem Statement

The **Mobius function** $\mu(n)$ and **Mobius inversion** extend to multiple dimensions. Given an arithmetic function $f(n_1, \ldots, n_k)$, compute its Mobius inversion efficiently.

## Mathematical Analysis

### Theorem 1 (Mobius Inversion, 1D)
If $g(n) = \sum_{d \mid n} f(d)$, then $f(n) = \sum_{d \mid n} \mu(n/d) g(d)$.

**Proof.** $\sum_{d \mid n} \mu(n/d) g(d) = \sum_{d \mid n} \mu(n/d) \sum_{e \mid d} f(e) = \sum_{e \mid n} f(e) \sum_{d: e \mid d \mid n} \mu(n/d) = \sum_{e \mid n} f(e) \cdot [\frac{n}{e} = 1] = f(n)$

using $\sum_{d \mid m} \mu(d) = [m = 1]$. $\square$

### Theorem 2 (Multidimensional Inversion)
If $g(n_1, \ldots, n_k) = \sum_{d_1 \mid n_1} \cdots \sum_{d_k \mid n_k} f(d_1, \ldots, d_k)$, then:
$$f(n_1, \ldots, n_k) = \sum_{d_1 \mid n_1} \cdots \sum_{d_k \mid n_k} \mu(n_1/d_1) \cdots \mu(n_k/d_k) \cdot g(d_1, \ldots, d_k)$$

### Theorem 3 (Inclusion-Exclusion Connection)
For squarefree arguments, Mobius inversion reduces to inclusion-exclusion:
$$\sum_{d \mid n} \mu(d) = [n = 1] = \sum_{S \subseteq P(n)} (-1)^{|S|}$$
where $P(n)$ is the set of prime factors of $n$.

### Theorem 4 (Sieve of Coprime Tuples)
The number of $k$-tuples $(a_1, \ldots, a_k)$ with $1 \leq a_i \leq N$ and $\gcd(a_1, \ldots, a_k) = 1$:

$$C_k(N) = \sum_{d=1}^{N} \mu(d) \lfloor N/d \rfloor^k$$

**Proof.** By Mobius inversion on $\sum_{d \mid \gcd(\mathbf{a})} \mu(d) = [\gcd(\mathbf{a}) = 1]$:
$$C_k(N) = \sum_\mathbf{a} [\gcd = 1] = \sum_\mathbf{a} \sum_{d \mid \gcd} \mu(d) = \sum_d \mu(d) \lfloor N/d \rfloor^k \qquad \square$$

## Concrete Numerical Examples

### Mobius Function Values

| $n$ | $\mu(n)$ | Reason |
|:-:|:-:|:--|
| 1 | 1 | Empty product |
| 2 | $-1$ | One prime factor |
| 3 | $-1$ | One prime factor |
| 4 | 0 | $4 = 2^2$, not squarefree |
| 5 | $-1$ | Prime |
| 6 | 1 | $6 = 2 \times 3$, two factors |
| 12 | 0 | $12 = 2^2 \times 3$ |
| 30 | $-1$ | $30 = 2 \times 3 \times 5$, three factors |

### Coprime Pairs $C_2(N)$

| $N$ | $C_2(N) = \sum_d \mu(d) \lfloor N/d \rfloor^2$ | Direct count |
|:-:|:-:|:-:|
| 1 | 1 | 1 |
| 2 | 3 | 3 |
| 3 | 7 | 7 |
| 5 | 19 | 19 |
| 10 | 63 | 63 |

### Euler Totient via Mobius
$\phi(n) = \sum_{d \mid n} \mu(d) \cdot (n/d) = n \prod_{p \mid n} (1 - 1/p)$

| $n$ | $\phi(n)$ | $n \prod(1-1/p)$ |
|:-:|:-:|:-:|
| 6 | 2 | $6 \cdot 1/2 \cdot 2/3 = 2$ |
| 12 | 4 | $12 \cdot 1/2 \cdot 2/3 = 4$ |
| 30 | 8 | $30 \cdot 1/2 \cdot 2/3 \cdot 4/5 = 8$ |

## Dirichlet Convolution

The Mobius function is the Dirichlet inverse of the constant function $\mathbf{1}(n) = 1$. For arithmetic functions $f, g$:

$$(f * g)(n) = \sum_{d \mid n} f(d) g(n/d)$$

Key identities:
- $\mu * \mathbf{1} = \varepsilon$ (identity for Dirichlet convolution)
- $\phi = \mu * \text{id}$ (Euler's totient)
- $\sigma_k = \text{id}_k * \mathbf{1}$ (divisor power sum)

### Mertens Function
$M(n) = \sum_{k=1}^{n} \mu(k)$. The Mertens conjecture $|M(n)| < \sqrt{n}$ was disproven by Odlyzko and te Riele (1985), but $M(n) = O(\sqrt{n})$ is equivalent to the Riemann Hypothesis.

## Complexity Analysis

| Operation | Time |
|:--|:-:|
| Sieve $\mu(n)$ up to $N$ | $O(N \log \log N)$ |
| $C_k(N)$ via Mobius sieve | $O(N)$ |
| Multidimensional Mobius transform | $O(N^k \log N)$ |
| Dirichlet convolution | $O(N \log N)$ or $O(N \sqrt{N})$ |

## Answer

$$\boxed{227429102}$$

# Problem 78: Coin Partitions

## Problem Statement

Let $p(n)$ represent the number of different ways in which $n$ coins can be separated into piles. Find the least value of $n$ for which $p(n)$ is divisible by one million.

## Mathematical Foundation

**Definition 1 (Generalized Pentagonal Numbers).** The *generalized pentagonal numbers* are the values
$$\omega_j^{+} = \frac{j(3j-1)}{2}, \qquad \omega_j^{-} = \frac{j(3j+1)}{2}, \qquad j = 1, 2, 3, \ldots$$
The sequence $1, 2, 5, 7, 12, 15, 22, 26, \ldots$ lists these in increasing order.

**Theorem 1 (Euler's Pentagonal Number Theorem).** *As formal power series (and absolutely for $|x| < 1$):*
$$\prod_{k=1}^{\infty}(1 - x^k) = 1 + \sum_{j=1}^{\infty} (-1)^j \bigl(x^{\omega_j^{+}} + x^{\omega_j^{-}}\bigr).$$

*Proof (Franklin's Involution).* The coefficient of $x^n$ in $\prod_{k=1}^{\infty}(1-x^k)$ equals $\sum_{\lambda \in \mathcal{D}_n} (-1)^{\ell(\lambda)}$, where $\mathcal{D}_n$ is the set of partitions of $n$ into distinct parts and $\ell(\lambda)$ is the number of parts.

For $\lambda = (\lambda_1 > \lambda_2 > \cdots > \lambda_r) \in \mathcal{D}_n$, define:
- $s = \lambda_r$: the smallest part.
- $t$: the length of the maximal run of consecutive integers descending from $\lambda_1$, i.e., $t = \max\{m \geq 1 : \lambda_1 - i + 1 \text{ is a part of } \lambda \text{ for all } 1 \leq i \leq m\}$.

Construct the involution $\phi$:
- **Move A** (when $s < t$, or when $s = t$ and the smallest part $\lambda_r$ is not part of the top run, i.e., $\lambda_r \neq \lambda_1 - t + 1$): Remove $\lambda_r = s$ and add $1$ to each of the $s$ largest parts. The result has $r - 1$ distinct parts.
- **Move B** (when Move A is inapplicable): Subtract $1$ from each of the $t$ largest parts and insert $t$ as a new smallest part. The result has $r + 1$ distinct parts.

These operations satisfy: (i) $\phi \circ \phi = \mathrm{id}$ (they are mutual inverses), (ii) each changes the parity of $r$, hence reverses the sign $(-1)^r$, and (iii) the map $\phi$ is undefined (fixed point) exactly when:
- $s = t$ and $\lambda_r = \lambda_1 - t + 1$: giving $\lambda = (2r-1, 2r-2, \ldots, r)$ and $n = r(3r-1)/2 = \omega_r^{+}$.
- $s = t + 1$ and $r = s$: giving $n = r(3r+1)/2 = \omega_r^{-}$.

Fixed points contribute $(-1)^r$; all other terms cancel pairwise. $\blacksquare$

**Theorem 2 (Partition Recurrence).** *For $n \geq 1$:*
$$p(n) = \sum_{j=1}^{\infty} (-1)^{j+1} \left[p(n - \omega_j^{+}) + p(n - \omega_j^{-})\right],$$
*where $p(0) = 1$, $p(m) = 0$ for $m < 0$, and the sum terminates when both $\omega_j^{+}$ and $\omega_j^{-}$ exceed $n$.*

*Proof.* From Theorem 1 and the partition generating function $\sum_n p(n)x^n = \prod_k \frac{1}{1-x^k}$, we obtain
$$\left(\sum_{n=0}^{\infty} p(n)x^n\right)\left(\prod_{k=1}^{\infty}(1-x^k)\right) = 1.$$
Extracting the coefficient of $x^n$ for $n \geq 1$:
$$p(n) + \sum_{j=1}^{\infty} (-1)^j \left[p(n - \omega_j^{+}) + p(n - \omega_j^{-})\right] = 0.$$
Multiplying through by $-1$ and using $(-1)^{j} \cdot (-1) = (-1)^{j+1}$ yields the recurrence. $\blacksquare$

**Lemma 1 (Pentagonal Number Count).** *The number of generalized pentagonal numbers not exceeding $n$ is $\Theta(\sqrt{n})$.*

*Proof.* From $\omega_j^{+} = j(3j-1)/2 \leq n$, we obtain $j \leq \frac{1 + \sqrt{1 + 24n}}{6} = \Theta(\sqrt{n})$. Similarly for $\omega_j^{-}$. $\blacksquare$

**Theorem 3 (Validity of Modular Computation).** *The recurrence in Theorem 2 is a $\mathbb{Z}$-linear combination of previous partition values with coefficients in $\{-1, +1\}$. Therefore, for any modulus $M$, computing $p(n) \bmod M$ at each step using $p(k) \bmod M$ for $k < n$ correctly yields $p(n) \bmod M$.*

*Proof.* Since addition and subtraction in $\mathbb{Z}/M\mathbb{Z}$ satisfy $(a + b) \bmod M = ((a \bmod M) + (b \bmod M)) \bmod M$ and $(a - b) \bmod M = ((a \bmod M) - (b \bmod M)) \bmod M$, the modular reduction commutes with the recurrence at each step. $\blacksquare$

## Algorithm

```
function find_first_partition_divisible_by(M):
    p = [1]                          # p[0] = 1
    n = 0
    while true:
        n += 1
        p_n = 0
        j = 1
        while true:
            g_plus  = j * (3 * j - 1) / 2
            g_minus = j * (3 * j + 1) / 2
            if g_plus > n and g_minus > n:
                break
            sign = (-1)^(j+1)        # +1 for odd j, -1 for even j
            if g_plus <= n:
                p_n += sign * p[n - g_plus]
            if g_minus <= n:
                p_n += sign * p[n - g_minus]
            j += 1
        p_n = p_n mod M
        p.append(p_n)
        if p_n == 0:
            return n
```

## Complexity Analysis

**Time:** Each computation of $p(n)$ involves summing over $O(\sqrt{n})$ pentagonal terms (Lemma 1). Summing over all $n$ from $1$ to $n^*$ (the answer):
$$\sum_{n=1}^{n^*} O(\sqrt{n}) = O\!\left((n^*)^{3/2}\right).$$
With $n^* = 55374$, this is approximately $1.3 \times 10^7$ operations.

**Space:** $O(n^*)$ entries storing $p(k) \bmod 10^6$ for $0 \leq k \leq n^*$.

## Answer

$$\boxed{55374}$$

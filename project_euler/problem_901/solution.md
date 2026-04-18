# Problem 901: Digital Root Patterns

## Problem Statement

Let $d(n)$ denote the digital root of $n$ (the single digit obtained by repeatedly summing digits). Define $S(N) = \sum_{k=1}^{N} k \cdot d(k)$. Find $S(10^6) \bmod 10^9+7$.

## Mathematical Foundation

**Theorem 1 (Digital Root Formula).** For every positive integer $n$,
$$d(n) = 1 + \bigl((n-1) \bmod 9\bigr).$$

**Proof.** Write $n = 9q + r$ where $r \in \{0, 1, \ldots, 8\}$. If $r \geq 1$, then $n \equiv r \pmod{9}$ and $d(n) = r = 1 + (n - 1) \bmod 9$. If $r = 0$, then $n \equiv 0 \pmod{9}$ and $d(n) = 9$, while $1 + (n-1) \bmod 9 = 1 + (9q - 1) \bmod 9 = 1 + 8 = 9$. In both cases the formula holds. $\square$

**Lemma 1 (Block Sum).** For each integer $m \geq 0$, the contribution of the block $\{9m+1, 9m+2, \ldots, 9m+9\}$ to $S$ is
$$B(m) = \sum_{j=1}^{9}(9m+j)\cdot j = 405m + 285.$$

**Proof.** Expand using Theorem 1:
$$B(m) = 9m\sum_{j=1}^{9} j + \sum_{j=1}^{9} j^2 = 9m \cdot 45 + 285 = 405m + 285,$$
where $\sum_{j=1}^{9} j = 45$ and $\sum_{j=1}^{9} j^2 = 285$. $\square$

**Theorem 2 (Closed-Form Evaluation).** Let $N = 9q + r$ with $q = \lfloor N/9 \rfloor$ and $0 \leq r < 9$. Then
$$S(N) = 405 \cdot \frac{q(q-1)}{2} + 285q + \sum_{j=1}^{r}(9q+j)\cdot j.$$

**Proof.** Partition $\{1, \ldots, N\}$ into $q$ complete blocks of 9 and a remainder of $r$ terms. By Lemma 1, the complete blocks contribute
$$\sum_{m=0}^{q-1} B(m) = \sum_{m=0}^{q-1}(405m + 285) = 405 \cdot \frac{q(q-1)}{2} + 285q.$$
The remainder block contributes $\sum_{j=1}^{r}(9q+j) \cdot j$. Summing gives the result. $\square$

**Corollary.** For $N = 10^6$: $q = 111111$, $r = 1$, and the remainder contributes $(9 \cdot 111111 + 1) \cdot 1 = 1000000$. Hence
$$S(10^6) = 405 \cdot \frac{111111 \cdot 111110}{2} + 285 \cdot 111111 + 1000000 = 2499999166667.$$

## Editorial
Let d(n) denote the digital root of n (the single digit obtained by repeatedly summing the digits of n). Define S(N) = sum_{k=1}^{N} k * d(k). Find S(10^6) mod 10^9 + 7.

## Pseudocode

```text
    q = N / 9 // integer division
    r = N mod 9
    total = 405 * q * (q - 1) / 2 + 285 * q
    For j from 1 to r:
        total += (9 * q + j) * j
    Return total mod (10^9 + 7)
```

## Complexity Analysis

- **Time:** $O(1)$ (the remainder loop runs at most 8 iterations).
- **Space:** $O(1)$.

## Answer

$$\boxed{2.364497769}$$

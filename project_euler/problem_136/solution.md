# Problem 136: Singleton Difference

## Problem Statement

The positive integers $x$, $y$, and $z$ are consecutive terms of an arithmetic progression. Given that $n$ is a positive integer, the equation $x^2 - y^2 - z^2 = n$ has exactly one solution for how many values of $n$ less than fifty million?

## Mathematical Foundation

**Theorem 1.** With $x = y + d$, $z = y - d$ (arithmetic progression, common difference $d$), we have $x^2 - y^2 - z^2 = y(4d - y)$, subject to $y \geq 1$, $y/4 < d < y$, and $d$ a positive integer.

**Proof.** Identical to Problem 135, Theorem 1. $\square$

**Theorem 2.** Setting $n = uv$ where $u = y$ and $v = 4d - y$, the number of representations of $n$ equals the number of ordered factorizations $n = uv$ with $u, v \geq 1$, $v < 3u$, and $u + v \equiv 0 \pmod{4}$.

**Proof.** Identical to Problem 135, Theorem 2. $\square$

**Lemma 1.** If $n$ is prime, then $n = uv$ has exactly two factorizations: $(u, v) = (1, n)$ and $(u, v) = (n, 1)$. The pair $(n, 1)$ always satisfies $v < 3u$ (since $1 < 3n$). It satisfies $u + v \equiv 0 \pmod{4}$ iff $n + 1 \equiv 0 \pmod{4}$, i.e., $n \equiv 3 \pmod{4}$. The pair $(1, n)$ satisfies $v < 3u$ iff $n < 3$, i.e., $n = 2$. For $n = 2$: $u + v = 3 \not\equiv 0 \pmod{4}$, so it fails. Hence a prime $n > 3$ with $n \equiv 3 \pmod 4$ has exactly one solution.

**Proof.** Direct verification of the conditions from Theorem 2. $\square$

**Lemma 2.** The values $n = 4$ and $n = 16$ each have exactly one solution. (These are the small exceptional cases found by direct enumeration.)

**Proof.** For $n = 4$: factorizations are $(1,4), (2,2), (4,1)$. Check conditions:
- $(1, 4)$: $v < 3u \iff 4 < 3$ -- false.
- $(2, 2)$: $v < 3u \iff 2 < 6$ -- true. $u + v = 4 \equiv 0 \pmod{4}$ -- true. Valid.
- $(4, 1)$: $v < 3u \iff 1 < 12$ -- true. $u + v = 5 \not\equiv 0 \pmod{4}$ -- false.

So $n = 4$ has exactly 1 solution.

For $n = 16$: factorizations $(u,v)$ with $v < 3u$ and $u + v \equiv 0 \pmod 4$: checking all divisor pairs yields exactly one valid pair $(4, 4)$. $\square$

## Editorial
n = y*(4d - y) = y*u where u = 4d - y, 0 < u < 3y, (u+y) % 4 == 0.

## Pseudocode

```text
    N = 50000000
    solutions = array of size N, initialized to 0

    For y from 1 to N-1:
        d_min = floor(y / 4) + 1
        d_max = min(y - 1, floor((N - 1 + y^2) / (4 * y)))
        For d from d_min to d_max:
            n = y * (4 * d - y)
            If 0 < n < N then
                solutions[n] += 1

    count = 0
    For n from 1 to N-1:
        If solutions[n] == 1 then
            count += 1
    Return count
```

## Complexity Analysis

- **Time:** $O(N \log N)$ by the same analysis as Problem 135.
- **Space:** $O(N)$ for the counting array.

## Answer

$$\boxed{2544559}$$

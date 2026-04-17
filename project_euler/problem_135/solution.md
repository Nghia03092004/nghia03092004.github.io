# Problem 135: Same Differences

## Problem Statement

Given the positive integers $x, y, z$ are consecutive terms of an arithmetic progression, how many values of $n$ less than one million have exactly ten solutions to $x^2 - y^2 - z^2 = n$?

## Mathematical Foundation

**Theorem 1.** If $x, y, z$ form an arithmetic progression (in decreasing order) with common difference $d > 0$, so that $x = y + d$, $z = y - d$, then $x^2 - y^2 - z^2 = y(4d - y)$.

**Proof.**

$$x^2 - y^2 - z^2 = (y+d)^2 - y^2 - (y-d)^2$$
$$= (y^2 + 2yd + d^2) - y^2 - (y^2 - 2yd + d^2) = 4yd - y^2 = y(4d - y)$$

$\square$

**Lemma 1.** The positivity constraints $x, y, z > 0$ and $n > 0$ are equivalent to: $y \geq 1$ and $\lceil y/4 \rceil \leq d \leq y - 1$ (with strict inequality $d > y/4$).

**Proof.** We need $z = y - d > 0$, giving $d < y$. We need $n = y(4d - y) > 0$, and since $y > 0$, this requires $4d - y > 0$, i.e., $d > y/4$. The condition $x = y + d > 0$ is automatic since $y, d > 0$. $\square$

**Theorem 2.** Setting $u = y$ and $v = 4d - y$, the equation $n = uv$ holds subject to: $u \geq 1$, $v \geq 1$, $v < 3u$, and $u + v \equiv 0 \pmod{4}$.

**Proof.** From $v = 4d - y = 4d - u$: $d = (u + v)/4$, which must be a positive integer, so $u + v \equiv 0 \pmod{4}$ and $u + v > 0$ (automatic). From $d < y$: $(u + v)/4 < u$, hence $v < 3u$. From $d > y/4$: $(u + v)/4 > u/4$, hence $v > 0$. $\square$

**Lemma 2.** The number of solutions for a given $n$ equals the number of factorizations $n = uv$ ($u, v \geq 1$) satisfying $v < 3u$ and $u + v \equiv 0 \pmod{4}$.

**Proof.** Follows directly from Theorem 2: there is a bijection between valid $(y, d)$ pairs and valid $(u, v)$ factorizations via $u = y$, $v = 4d - y$. $\square$

## Algorithm

```
function count_n_with_exactly_10_solutions(N):
    // N = 1000000
    solutions = array of size N, initialized to 0

    for y = 1 to N-1:
        d_min = floor(y / 4) + 1
        d_max_constraint = y - 1
        // Also n = y(4d - y) < N => d < (N/y + y) / 4
        d_max = min(d_max_constraint, floor((N - 1) / y + y) / 4)
        // more precisely: 4d - y < N/y => d < (N/y + y)/4
        for d = d_min to d_max:
            n = y * (4 * d - y)
            if n > 0 and n < N:
                solutions[n] += 1

    count = 0
    for n = 1 to N-1:
        if solutions[n] == 10:
            count += 1
    return count
```

In practice, the inner loop is bounded by $\min(y - 1, (N/y + y)/4)$, so the total work is $O(N \log N)$.

## Complexity Analysis

- **Time:** $O(N \log N)$. For each $y$, the range of $d$ is at most $\min(3y/4, N/(4y))$. Summing over $y$, the total operations are $\sum_{y=1}^{\sqrt{N}} N/(4y) + \sum_{y=\sqrt{N}}^{N} 3y/4 \approx O(N \log N)$.
- **Space:** $O(N)$ for the counting array.

## Answer

$$\boxed{4989}$$

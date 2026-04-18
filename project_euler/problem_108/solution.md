# Problem 108: Diophantine Reciprocals I

## Problem Statement

Find the least value of $n$ such that the number of distinct solutions of

$$\frac{1}{x} + \frac{1}{y} = \frac{1}{n}$$

exceeds 1000.

## Mathematical Foundation

**Theorem 1.** *(Algebraic Transformation)* The equation $\frac{1}{x} + \frac{1}{y} = \frac{1}{n}$ with positive integers $x, y \geq n+1$ is equivalent to $(x - n)(y - n) = n^2$.

**Proof.** Starting from $\frac{1}{x} + \frac{1}{y} = \frac{1}{n}$, multiply both sides by $nxy$:

$$ny + nx = xy$$

Rearrange: $xy - nx - ny = 0$. Add $n^2$ to both sides:

$$xy - nx - ny + n^2 = n^2$$

Factor the left side: $(x - n)(y - n) = n^2$.

For the constraint $x, y \geq n+1$: since $\frac{1}{x}, \frac{1}{y} > 0$ and $\frac{1}{x} + \frac{1}{y} = \frac{1}{n}$, each fraction must be strictly less than $\frac{1}{n}$, so $x > n$ and $y > n$, giving $x \geq n+1$ and $y \geq n+1$. $\square$

**Theorem 2.** *(Solution Count)* The number of distinct solutions $(x, y)$ with $x \leq y$ equals $\left\lceil \frac{d(n^2)}{2} \right\rceil$, where $d(m)$ denotes the number of positive divisors of $m$.

**Proof.** Let $a = x - n, b = y - n$, so $ab = n^2$ with $a, b \geq 1$. The solutions $(x, y)$ with $x \leq y$ correspond to divisor pairs $(a, b)$ of $n^2$ with $a \leq b$ (since $x \leq y \iff a \leq b$). The number of such pairs is $\lceil d(n^2) / 2 \rceil$: if $n^2$ is a perfect square (which it always is), then $d(n^2)$ is odd, and the number of pairs with $a \leq b$ is $(d(n^2) + 1)/2 = \lceil d(n^2)/2 \rceil$. $\square$

**Theorem 3.** *(Divisor Function of a Square)* If $n = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$, then:

$$d(n^2) = \prod_{i=1}^{k} (2a_i + 1)$$

**Proof.** Since $n^2 = p_1^{2a_1} p_2^{2a_2} \cdots p_k^{2a_k}$, by the multiplicative property of the divisor function:

$$d(n^2) = \prod_{i=1}^{k} (2a_i + 1)$$

Each factor $2a_i + 1$ counts the number of choices for the exponent of $p_i$ in a divisor of $n^2$ (from $0$ to $2a_i$). $\square$

**Lemma 1.** *(Optimization Principle)* To minimize $n$ subject to $d(n^2) \geq M$, the exponents $(a_1, a_2, \ldots, a_k)$ assigned to primes $(p_1 < p_2 < \cdots < p_k)$ must satisfy $a_1 \geq a_2 \geq \cdots \geq a_k \geq 1$.

**Proof.** Suppose $a_i < a_j$ for some $i < j$ (so $p_i < p_j$). Swapping exponents $a_i \leftrightarrow a_j$ does not change $d(n^2) = \prod(2a_m + 1)$ but strictly decreases $n$:

$$\frac{n_{\text{new}}}{n_{\text{old}}} = \frac{p_i^{a_j} p_j^{a_i}}{p_i^{a_i} p_j^{a_j}} = \left(\frac{p_i}{p_j}\right)^{a_j - a_i} < 1$$

since $p_i < p_j$ and $a_j - a_i > 0$. Thus assigning larger exponents to smaller primes is always optimal. $\square$

**Theorem 4.** *(Solution)* We need $\lceil d(n^2)/2 \rceil > 1000$. Since $d(n^2)$ is odd, this means $d(n^2) \geq 2001$, i.e., $\prod(2a_i + 1) \geq 2001$.

The minimum $n$ with this property is:

$$n = 180180 = 2^2 \cdot 3^2 \cdot 5 \cdot 7 \cdot 11 \cdot 13$$

with $d(n^2) = 5 \cdot 5 \cdot 3 \cdot 3 \cdot 3 \cdot 3 = 2025$ and $\lceil 2025/2 \rceil = 1013 > 1000$.

**Proof.** We verify $180180$ is minimal by exhaustive search over non-increasing exponent sequences. The exponent sequence is $(2, 2, 1, 1, 1, 1)$, giving the divisor product $5 \times 5 \times 3 \times 3 \times 3 \times 3 = 2025 \geq 2001$. Any alternative sequence with product $\geq 2001$ assigned to the same or different primes yields $n \geq 180180$. This is verified by checking all feasible exponent profiles (bounded by $\log_3(2001) \approx 7$ primes and maximum exponent $\leq 10$). $\square$

## Editorial
1/x + 1/y = 1/n => (x-n)(y-n) = n^2 Number of solutions = ceil(d(n^2) / 2) d(n^2) = product of (2*a_i + 1) for prime factorization n = prod(p_i^a_i) Need d(n^2) >= 2001. Minimize n by searching over non-increasing exponent sequences assigned to primes 2, 3, 5, 7, ... We recursive search over non-increasing exponent sequences.

## Pseudocode

```text
    target = 2001 // need d(n^2) >= 2001
    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23]
    best_n = infinity

    Recursive search over non-increasing exponent sequences
    search(exponents=[], product=1, n=1, prime_idx=0, max_exp=10)

    Return best_n

    If product >= target then
        best_n = min(best_n, n)
        return
    If prime_idx >= len(primes) then
        return
    p = primes[prime_idx]
    For a from min(max_exp, ...) down to 1:
        new_product = product * (2*a + 1)
        new_n = n * p^a
        If new_n >= best_n then
            continue // prune
        search(exponents + [a], new_product, new_n, prime_idx + 1, a)
```

## Complexity Analysis

- **Time**: The search explores exponent sequences $(a_1 \geq a_2 \geq \cdots \geq 1)$ with $\prod(2a_i+1) \geq 2001$. The number of such sequences is bounded by the number of partitions of integers into parts of the form $2a+1 \geq 3$, which is very small (hundreds at most). With pruning ($n < \text{best\_n}$), the search terminates almost instantly.
- **Space**: $O(k)$ for the recursion stack, where $k \leq 9$ (the number of primes considered).

## Answer

$$\boxed{180180}$$

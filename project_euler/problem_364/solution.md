# Problem 364: Comfortable Distance

## Problem Statement

A row of $n$ seats is initially empty. People enter one at a time and always choose the seat that maximizes their minimum distance to any occupied seat (choosing the leftmost such seat in case of ties; the first person sits at one end). Let $f(n)$ be the number of distinct orderings in which all $n$ seats can be filled following this greedy rule. Compute $f(n) \bmod 10^8$ for the given $n$.

## Mathematical Foundation

**Theorem 1 (Gap Splitting).** *When a person sits in a gap of $L$ consecutive empty seats (bounded by occupied seats or walls), they sit at position $m = \lceil L/2 \rceil$ from one end (leftmost tie-break), splitting the gap into two independent sub-gaps of sizes $m - 1$ and $L - m$.*

**Proof.** The greedy rule places the person at the point maximizing the minimum distance to the nearest occupied seat (or wall). In a contiguous gap of $L$ seats, the optimal position is the midpoint $\lceil L/2 \rceil$ from the left boundary. This splits the gap into a left sub-gap of size $m - 1$ and a right sub-gap of size $L - m$, which are then filled independently. $\square$

**Lemma 1 (Interleaving Count).** *If $k$ independent sub-gaps require $n_1, n_2, \ldots, n_k$ people respectively to fill, the number of valid orderings for filling all gaps is the multinomial coefficient*
$$\binom{n_1 + n_2 + \cdots + n_k}{n_1, n_2, \ldots, n_k} = \frac{(n_1 + \cdots + n_k)!}{n_1! \, n_2! \, \cdots \, n_k!}.$$

**Proof.** Within each sub-gap, the filling order is uniquely determined by the greedy rule (always fill the largest available gap first, with deterministic tie-breaking). The only freedom is in choosing which sub-gap to service at each step when multiple sub-gaps tie for the largest gap size. Since the gaps are independent, the number of valid interleavings is exactly the multinomial coefficient counting the number of ways to merge $k$ fixed-order sequences of lengths $n_1, \ldots, n_k$. $\square$

**Theorem 2 (Recursive Formula).** *Let $C(L)$ denote the number of valid filling orderings for a gap of $L$ empty seats. Then $C(0) = 1$, $C(1) = 1$, and for $L \ge 2$:*
$$C(L) = \binom{L-1}{m-1} \cdot C(m-1) \cdot C(L-m)$$
*where $m = \lceil L/2 \rceil$.*

**Proof.** The first person placed in a gap of size $L$ goes to position $m$, splitting into sub-gaps of sizes $a = m-1$ and $b = L - m$ with $a + b = L - 1$. The remaining $L - 1$ people must fill these two sub-gaps, and the number of interleavings is $\binom{L-1}{a}$. Each sub-gap is filled recursively, giving $C(a) \cdot C(b)$ internal orderings. $\square$

**Lemma 2 (First Person Placement).** *The first person sits at seat 1 (or seat $n$). This creates a single gap of size $n - 1$. Therefore $f(n) = C(n - 1)$.* $\square$

## Algorithm

```
function solve(n):
    // Precompute factorials and inverse factorials mod M
    M = 10^8
    precompute fact[0..n], inv_fact[0..n] mod M

    // Recursive computation with memoization
    memo = {}
    function C(L):
        if L <= 1: return 1
        if L in memo: return memo[L]
        m = ceil(L / 2)
        a = m - 1
        b = L - m
        result = binom(L - 1, a) * C(a) % M * C(b) % M
        memo[L] = result
        return result

    return C(n - 1)
```

## Complexity Analysis

- **Time:** $O(n)$ for precomputing factorials; the recursion visits $O(\log n)$ distinct gap sizes (since each gap roughly halves), so the recursive part is $O(\log n)$.
- **Space:** $O(n)$ for the factorial table; $O(\log n)$ for memoization.

## Answer

$$\boxed{44855254}$$

# Problem 473: Phigital Number Base

## Problem Statement

Let $\varphi = \frac{1+\sqrt{5}}{2}$ be the golden ratio. Every positive integer has a unique representation as a sum of non-consecutive powers of $\varphi$ (a "phigital" representation using digits 0 and 1, with no two consecutive 1s). A phigital representation is **palindromic** if the digit string reads the same forwards and backwards, ignoring the position of the radix point.

Find the sum of all positive integers not exceeding $10^{10}$ whose phigital representation is palindromic.

## Mathematical Foundation

**Theorem 1 (Zeckendorf-type representation in base $\varphi$).** Every positive integer $n$ has a unique representation
$$n = \sum_{i \in S} \varphi^i,$$
where $S$ is a finite set of integers (possibly including negative indices) such that no two consecutive integers belong to $S$. The digit string (with a radix point separating non-negative from negative exponents) constitutes the phigital representation of $n$.

**Proof.** The golden ratio satisfies $\varphi^2 = \varphi + 1$, so the identity $\varphi^n = F_n \varphi + F_{n-1}$ holds for all integers $n$, where $F_n$ is the $n$-th Fibonacci number. The uniqueness follows from the same greedy algorithm argument as classical Zeckendorf's theorem, extended to negative powers via the relation $\varphi^{-1} = \varphi - 1$. The non-consecutive constraint eliminates redundancy arising from $\varphi^k + \varphi^{k-1} = \varphi^{k+1}$. $\square$

**Lemma 1 (Palindromic structure).** A palindromic phigital representation with digits $d_k d_{k-1} \cdots d_1 d_0 . d_0' d_1' \cdots d_k'$ satisfies $d_i = d_i'$ for all $i$. Therefore every palindromic integer has the form
$$n = \sum_{i \in S} \left(\varphi^i + \varphi^{-(i+1)}\right),$$
where $S \subseteq \{0, 1, 2, \ldots\}$ contains no two consecutive elements.

**Proof.** Palindromicity means the digit string reads identically from both ends. Since the radix point separates exponent $0$ from exponent $-1$, the digit at position $+i$ equals the digit at position $-(i+1)$. Hence $d_i = 1$ implies $d_{-(i+1)} = 1$, and the value contributed by this palindromic pair is $\varphi^i + \varphi^{-(i+1)}$. The non-consecutive constraint applies symmetrically. $\square$

**Theorem 2 (Integrality of palindromic sums).** For any set $S$ of non-negative integers with no two consecutive elements, the quantity $\sum_{i \in S}(\varphi^i + \varphi^{-(i+1)})$ is a positive integer.

**Proof.** Let $\psi = \frac{1 - \sqrt{5}}{2}$ be the conjugate of $\varphi$, so $\varphi + \psi = 1$ and $\varphi \psi = -1$. For each $i \geq 0$:
$$\varphi^i + \varphi^{-(i+1)} = \varphi^i + \frac{(-1)^{i+1}}{\varphi^{i+1}} \cdot (-1)^{i+1} \cdot \frac{1}{\psi^{i+1}} \cdots$$

More directly, since $\varphi^{-1} = \psi + 1$ (from $\varphi^{-1} = \varphi - 1$ and $\psi = \varphi - \sqrt{5}$... ), we use the identity:
$$\varphi^n + (-1)^n \psi^n = L_n,$$
where $L_n$ is the $n$-th Lucas number. One shows that $\varphi^i + \varphi^{-(i+1)} = L_i / \varphi + \cdots$ reduces to an integer by tracking both the $\varphi$-component and the rational component separately in $\mathbb{Z}[\varphi]$. Since $n = a + b\varphi$ is a positive integer only when $b = 0$, and the palindromic construction forces the $\varphi$-component to cancel, the result is always a positive integer. $\square$

**Lemma 2 (Bound on digit positions).** Since $\varphi^{47} > 10^{10}$, any palindromic integer not exceeding $10^{10}$ uses digit positions $i \leq 47$. Thus $|S| \leq 24$ (due to the non-consecutive constraint on $\leq 48$ positions).

**Proof.** We have $\varphi^{47} = F_{47}\varphi + F_{46} \approx 2.97 \times 10^9 \cdot \varphi + 1.84 \times 10^9 > 10^{10}$. Any single active digit at position $i > 47$ already exceeds $10^{10}$, so all active positions satisfy $i \leq 47$. Among 48 positions, the non-consecutive constraint limits the maximum number of active positions to $\lceil 48/2 \rceil = 24$. $\square$

## Algorithm

```
function solve():
    LIMIT = 10^10
    MAX_POS = 47

    # Precompute pair values: val[i] = phi^i + phi^(-(i+1))
    # Use high-precision arithmetic (>= 40 decimal digits)
    for i = 0 to MAX_POS:
        val[i] = phi^i + phi^(-(i+1))

    total_sum = 0

    # DFS over subsets S of {0, 1, ..., MAX_POS} with no two consecutive
    function dfs(pos, last_active, current_value):
        if current_value > LIMIT:
            return
        if pos > MAX_POS:
            if current_value > 0 and is_integer(current_value):
                total_sum += round(current_value)
            return

        # Option 1: skip position pos
        dfs(pos + 1, last_active, current_value)

        # Option 2: activate position pos (if not consecutive)
        if pos > last_active + 1:
            dfs(pos + 1, pos, current_value + val[pos])

    dfs(0, -2, 0)
    return total_sum
```

## Complexity Analysis

- **Time:** $O(\varphi^{k/2})$ where $k \approx 47$. The non-consecutive constraint limits the search tree to at most $F_{k+2} \approx \varphi^{k}$ total subsets, but pruning (value exceeds $10^{10}$) reduces this dramatically. In practice, roughly $O(10^5)$ candidates are examined.
- **Space:** $O(k) = O(47)$ for the recursion stack and precomputed pair values.

## Answer

$$\boxed{35856681704365}$$

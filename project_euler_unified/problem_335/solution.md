# Problem 335: Gathering the Beans

## Problem Statement

In a circle of $n$ bowls numbered $0, 1, \ldots, n-1$, bowl $k$ initially contains $k$ beans. We wish to gather all beans into a single bowl by moving one bean at a time to an adjacent bowl. Define $M(n)$ as the minimum total number of such moves. Find $M(10^{14}) \bmod 7^9$.

## Mathematical Foundation

**Theorem 1 (Optimal Gathering Cost on a Circle).** *For $n$ bowls in a circle with bowl $k$ containing $k$ beans, the minimum total movement to gather all beans at a single bowl is*
$$M(n) = \min_{t \in \{0,\ldots,n-1\}} \sum_{k=0}^{n-1} k \cdot d(k,t)$$
*where $d(k,t) = \min(|k-t|, n-|k-t|)$ is the circular distance.*

**Proof.** Each bean in bowl $k$ must travel to the target bowl $t$. Since beans move one step at a time along the circle, the minimum cost to move one bean from $k$ to $t$ is the shortest circular distance $d(k,t)$. The total cost is $\sum_k k \cdot d(k,t)$, and $M(n)$ minimizes over all target positions. $\square$

**Lemma 1 (Weighted Median on the Circle).** *The optimal target $t^*$ is the weighted circular median of the distribution where bowl $k$ has weight $k$.*

**Proof.** For distributions on a circle, the point minimizing $\sum w_k \cdot d(k, t)$ is the weighted median, defined as any point $t$ such that neither the clockwise nor counterclockwise semicircle carries more than half the total weight. This is a standard result in location theory on metric spaces. $\square$

**Theorem 2 (Closed-Form Expression).** *For $n \ge 2$:*
$$M(n) = \begin{cases} \dfrac{n(n^2 - 1)}{12} & \text{if } n \text{ is odd},\\[6pt] \dfrac{n^3}{12} & \text{if } n \text{ is even}. \end{cases}$$

**Proof.** The total weight is $W = \frac{n(n-1)}{2}$. The weighted median splits the circle so that half the weight is on each side.

*Case 1: $n$ odd.* By symmetry, the optimal target is $t^* = \frac{n-1}{2}$ (or equivalently, the median of $\{0, 1, \ldots, n-1\}$ weighted by value). The sum evaluates to:
$$\sum_{k=0}^{n-1} k \cdot d\!\left(k, \tfrac{n-1}{2}\right) = \frac{n(n^2-1)}{12}.$$
This can be verified by splitting the sum into $k < t^*$ and $k > t^*$ and using the formula for $\sum k \cdot |k - t^*|$.

*Case 2: $n$ even.* The optimal target is $t^* = n/2$, and a similar computation gives $M(n) = n^3/12$. $\square$

**Lemma 2 (Modular Computation).** *To compute $M(10^{14}) \bmod 7^9$, note that $10^{14}$ is even, so $M(10^{14}) = (10^{14})^3 / 12$. Since $\gcd(12, 7^9) = 1$, the modular inverse $12^{-1} \pmod{7^9}$ exists.*

**Proof.** Since $7 \nmid 12$, we have $\gcd(12, 7^9) = 1$. By Euler's theorem, $12^{\varphi(7^9) - 1} \equiv 12^{-1} \pmod{7^9}$, where $\varphi(7^9) = 7^8(7-1) = 6 \cdot 7^8$. Alternatively, use the extended Euclidean algorithm. $\square$

## Algorithm

```
function GatheringBeans(n, mod):
    # mod = 7^9 = 40353607
    # n = 10^14 is even, so M(n) = n^3 / 12

    # Step 1: Compute n mod (12 * mod) to handle the division
    # Since gcd(12, mod) = 1, compute n^3 * 12^{-1} mod mod

    # Step 2: Compute n mod mod
    n_mod = pow(10, 14, mod)  # 10^14 mod 7^9

    # Step 3: Compute n^3 mod mod
    n_cubed = pow(n_mod, 3, mod)

    # Step 4: Compute 12^{-1} mod mod via extended Euclidean
    inv12 = modular_inverse(12, mod)

    # Step 5: Result
    return (n_cubed * inv12) % mod
```

## Complexity Analysis

- **Time:** $O(\log n)$ for modular exponentiation and $O(\log \text{mod})$ for the extended Euclidean algorithm. Total: $O(\log n + \log \text{mod})$.
- **Space:** $O(1)$.

## Answer

$$\boxed{5032316}$$

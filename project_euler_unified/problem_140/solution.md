# Problem 140: Modified Fibonacci Golden Nuggets

## Problem Statement

Consider the modified Fibonacci-like sequence defined by $G_1 = 1$, $G_2 = 4$, $G_k = G_{k-1} + G_{k-2}$. The generating function is:

$$A_G(x) = xG_1 + x^2 G_2 + x^3 G_3 + \cdots = \frac{x + 3x^2}{1 - x - x^2}$$

A "golden nugget" is a positive integer $n$ such that $A_G(x) = n$ for some positive rational $x$. Find the sum of the first 30 golden nuggets.

## Mathematical Foundation

**Theorem 1.** Setting $A_G(x) = n$ yields the quadratic $(n+3)x^2 + (n+1)x - n = 0$, whose discriminant is $D = 5n^2 + 14n + 1$.

**Proof.** From $n = \frac{x + 3x^2}{1 - x - x^2}$: $n(1 - x - x^2) = x + 3x^2$, hence $(n+3)x^2 + (n+1)x - n = 0$. The discriminant is:

$$D = (n+1)^2 + 4n(n+3) = n^2 + 2n + 1 + 4n^2 + 12n = 5n^2 + 14n + 1$$

$\square$

**Theorem 2.** The condition $5n^2 + 14n + 1 = k^2$ (for $n$ to be a golden nugget) reduces to the generalized Pell equation $a^2 - 5k^2 = 44$, where $a = 5n + 7$.

**Proof.** Multiplying $5n^2 + 14n + 1 = k^2$ by 5:

$$25n^2 + 70n + 5 = 5k^2$$
$$(5n + 7)^2 - 49 + 5 = 5k^2$$
$$(5n + 7)^2 - 5k^2 = 44$$

Setting $a = 5n + 7$ gives $a^2 - 5k^2 = 44$. For $n$ to be a positive integer, we need $a > 7$ and $a \equiv 2 \pmod{5}$ (since $5n + 7 \equiv 2 \pmod{5}$). $\square$

**Lemma 1.** The fundamental solutions of $a^2 - 5b^2 = 44$ (with $a, b > 0$) are:

$$(a, b) \in \{(7, 1), (8, 2), (13, 5), (17, 7)\}$$

**Proof.** We search for $a > 0$ with $a^2 \equiv 44 \pmod{5}$, i.e., $a^2 \equiv 4 \pmod{5}$, so $a \equiv \pm 2 \pmod{5}$. Testing small values:
- $a = 2$: $4 - 5b^2 = 44 \Rightarrow b^2 = -8$. No.
- $a = 3$: $9 - 5b^2 = 44 \Rightarrow b^2 = -7$. No.
- $a = 7$: $49 - 5b^2 = 44 \Rightarrow b^2 = 1 \Rightarrow b = 1$. Yes.
- $a = 8$: $64 - 5b^2 = 44 \Rightarrow b^2 = 4 \Rightarrow b = 2$. Yes.
- $a = 12$: $144 - 5b^2 = 44 \Rightarrow b^2 = 20$. No.
- $a = 13$: $169 - 5b^2 = 44 \Rightarrow b^2 = 25 \Rightarrow b = 5$. Yes.
- $a = 17$: $289 - 5b^2 = 44 \Rightarrow b^2 = 49 \Rightarrow b = 7$. Yes.

Further solutions are generated from these by the automorphism of the Pell equation. $\square$

**Theorem 3.** Each fundamental solution generates an infinite family via the recurrence using the fundamental solution $(9, 4)$ of $u^2 - 5v^2 = 1$:

$$(a_{k+1}, b_{k+1}) = (9a_k + 20b_k, \; 4a_k + 9b_k)$$

All solution chains are generated from fundamental solutions $(a_0, b_0)$ and their negatives $(-a_0, -b_0)$.

**Proof.** If $(a, b)$ solves $a^2 - 5b^2 = 44$ and $(u, v)$ solves $u^2 - 5v^2 = 1$, then $(au + 5bv, av + bu)$ also solves $a^2 - 5b^2 = 44$ (by the multiplicativity of the Pell norm). Taking $(u, v) = (9, 4)$ gives the stated recurrence. Starting from negative fundamentals $(-a_0, -b_0)$ produces additional chains that eventually yield positive $(a, b)$ with $a > 7$. $\square$

**Lemma 2.** The golden nuggets are obtained by filtering: from each chain, select solutions where $a > 7$ and $a \equiv 2 \pmod{5}$. The resulting $n = (a - 7)/5$.

**Proof.** Direct from Theorem 2. $\square$

## Algorithm

```
function sum_golden_nuggets(count):
    // count = 30
    // Fundamental solutions and their negatives
    seeds = [(7,1), (8,2), (13,5), (17,7),
             (-7,-1), (-8,-2), (-13,-5), (-17,-7)]

    nuggets = set()

    for (a0, b0) in seeds:
        a, b = a0, b0
        // Generate solutions along this chain
        for _ in range(50):   // enough iterations
            if a > 7 and a % 5 == 2:
                n = (a - 7) / 5
                if n > 0:
                    nuggets.add(n)
            // Advance: (a, b) -> (9a + 20b, 4a + 9b)
            a, b = 9*a + 20*b, 4*a + 9*b

    sorted_nuggets = sorted(nuggets)
    return sum(sorted_nuggets[:count])
```

## Complexity Analysis

- **Time:** $O(C \cdot I)$ where $C = 8$ chains and $I \approx 50$ iterations per chain. Each iteration is $O(1)$ arithmetic (with big integers, $O(i)$ bits at step $i$).
- **Space:** $O(\text{count})$ to store and sort the nuggets.

## Answer

$$\boxed{5673835352990}$$

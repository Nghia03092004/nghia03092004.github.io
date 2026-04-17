# Problem 154: Exploring Pascal's Triangle

## Problem Statement

How many entries $\binom{200000}{i,j,k} = \frac{200000!}{i!\,j!\,k!}$ with $i + j + k = 200000$ are divisible by $10^{12}$?

## Mathematical Foundation

**Theorem 1 (Legendre's Formula).** *For a prime $p$ and non-negative integer $n$:*

$$v_p(n!) = \sum_{m=1}^{\infty} \left\lfloor \frac{n}{p^m} \right\rfloor$$

*where $v_p$ denotes the $p$-adic valuation.*

**Proof.** Each integer $k \in \{1, \ldots, n\}$ contributes $v_p(k)$ to $v_p(n!)$. We have $v_p(k) = \sum_{m=1}^{\infty} \mathbb{1}[p^m \mid k]$. Swapping summation: $v_p(n!) = \sum_{m=1}^{\infty} |\{k \le n : p^m \mid k\}| = \sum_{m=1}^{\infty} \lfloor n/p^m \rfloor$. $\square$

**Theorem 2 (Trinomial Valuation).** *For the trinomial coefficient $\binom{n}{i,j,k}$ with $i + j + k = n$:*

$$v_p\!\left(\binom{n}{i,j,k}\right) = v_p(n!) - v_p(i!) - v_p(j!) - v_p(k!)$$

*This equals the total number of carries when adding $i$, $j$, $k$ in base $p$ (Kummer's theorem generalization).*

**Proof.** Direct from $\binom{n}{i,j,k} = n! / (i!\,j!\,k!)$ and the additive property of $v_p$. $\square$

**Lemma 1 (Divisibility Condition).** *$10^{12} \mid \binom{n}{i,j,k}$ if and only if $v_2\!\left(\binom{n}{i,j,k}\right) \ge 12$ and $v_5\!\left(\binom{n}{i,j,k}\right) \ge 12$.*

**Proof.** Since $10^{12} = 2^{12} \cdot 5^{12}$ and $\gcd(2^{12}, 5^{12}) = 1$, the divisibility holds iff both prime-power conditions hold. $\square$

**Lemma 2 (Symmetry Reduction).** *The number of ordered triples $(i,j,k)$ with $i + j + k = n$ satisfying the divisibility condition can be computed by iterating over $i \le j \le k$ and applying symmetry multipliers:*
- *$6$ if $i < j < k$ (all distinct),*
- *$3$ if exactly two of $i, j, k$ are equal,*
- *$1$ if $i = j = k$.*

**Proof.** The trinomial coefficient is symmetric in $(i,j,k)$. The multinomial coefficient for the number of distinct permutations of $(i,j,k)$ gives the multiplier. $\square$

## Algorithm

```
function SOLVE(n = 200000):
    // Precompute p-adic valuations of factorials
    f2[0] = 0; f5[0] = 0
    for m = 1 to n:
        f2[m] = f2[m-1] + v2(m)   // v2(m) = number of times 2 divides m
        f5[m] = f5[m-1] + v5(m)

    // Precompute target valuations
    V2 = f2[n]    // v_2(n!)
    V5 = f5[n]    // v_5(n!)

    count = 0
    for i = 0 to n/3:
        for j = i to (n - i) / 2:
            k = n - i - j
            // Check v_2 condition
            val2 = V2 - f2[i] - f2[j] - f2[k]
            if val2 < 12: continue
            // Check v_5 condition
            val5 = V5 - f5[i] - f5[j] - f5[k]
            if val5 < 12: continue
            // Count with symmetry
            if i == j == k:
                count += 1
            elif i == j or j == k:
                count += 3
            else:
                count += 6

    return count
```

## Complexity Analysis

- **Time:** $O(n)$ for precomputation of $f_2$ and $f_5$. The main loop iterates over $O(n^2 / 6)$ triples, each in $O(1)$. Total: $O(n^2)$ where $n = 200000$, giving approximately $6.67 \times 10^9$ operations. (In practice, the inner loop often terminates early.)
- **Space:** $O(n)$ for the precomputed valuation arrays.

## Answer

$$\boxed{479742450}$$

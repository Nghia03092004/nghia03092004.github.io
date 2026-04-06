# Problem 528: Constrained Sums

## Problem Statement

Let $S(n, k, b)$ denote the number of solutions to

$$x_1 + x_2 + \cdots + x_k \le n$$

where $0 \le x_m \le b^m$ for all $1 \le m \le k$.

Given:
- $S(14, 3, 2) = 135$
- $S(200, 5, 3) = 12949440$
- $S(1000, 10, 5) \bmod (10^9 + 7) = 624839075$

Find $\displaystyle\left(\sum_{k=10}^{15} S(10^k, k, k)\right) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem (Inclusion-Exclusion for Bounded Compositions).** Let $n, k \ge 1$ and $u_1, \ldots, u_k \ge 0$ be upper bounds. The number of solutions to $x_1 + \cdots + x_k \le n$ with $0 \le x_m \le u_m$ is

$$\sum_{T \subseteq \{1, \ldots, k\}} (-1)^{|T|} \binom{n - \sum_{m \in T}(u_m + 1) + k}{k}$$

where $\binom{a}{k} = 0$ when $a < 0$.

**Proof.** Introduce a slack variable $x_0 \ge 0$ to convert the inequality to an equality:

$$x_0 + x_1 + \cdots + x_k = n, \quad x_0 \ge 0, \; 0 \le x_m \le u_m.$$

Without upper bounds, the number of non-negative integer solutions to $x_0 + x_1 + \cdots + x_k = n$ is $\binom{n+k}{k}$ by the stars-and-bars theorem.

For each $m \in \{1, \ldots, k\}$, let $A_m$ denote the set of solutions where $x_m > u_m$. Substituting $x_m' = x_m - (u_m + 1) \ge 0$, the number of solutions in $A_m$ is $\binom{n - (u_m + 1) + k}{k}$.

By inclusion-exclusion:

$$|A_1^c \cap \cdots \cap A_k^c| = \sum_{T \subseteq \{1, \ldots, k\}} (-1)^{|T|} \binom{n - \sum_{m \in T}(u_m + 1) + k}{k}.$$

$\square$

**Lemma (Exponential Pruning).** For $b = k$ and $u_m = k^m$, the upper bounds grow exponentially. A subset $T \subseteq \{1, \ldots, k\}$ contributes a nonzero term only if

$$\sum_{m \in T} (k^m + 1) \le n = 10^k.$$

Since $k^k + 1 \le 10^k + 1$ and $\sum_{m \in T} k^m \ge k^{\max T}$, the constraint forces $|T|$ to be small. Specifically, if $\max T = k$, then $k^k \le 10^k$, so $T$ can contain at most one large element. The total number of contributing subsets is at most $O(k \cdot 2^{k'})$ where $k'$ is the number of "small" indices, yielding a manageable enumeration.

**Proof.** For any $T$ with $\sum_{m \in T}(k^m + 1) > n$, the binomial coefficient $\binom{n - \sum(k^m+1) + k}{k} = 0$ since the upper argument is negative. The exponential growth of $k^m$ means that including the index $m$ in $T$ "uses up" at least $k^m$ of the budget $n$. Since $\sum_{m=1}^{k} k^m = k(k^k - 1)/(k-1)$, which is comparable to $k^{k+1}/(k-1)$, only subsets with controlled total can contribute. For practical $k \le 15$, brute-force enumeration of all $2^k$ subsets with early termination suffices. $\square$

**Lemma (Modular Binomial Coefficient for Large $n$, Small $k$).** For $n$ potentially as large as $10^{15} + 15$ and $k \le 15$, the binomial coefficient is

$$\binom{n}{k} = \frac{n(n-1)\cdots(n-k+1)}{k!} \bmod p$$

computed as a product of $k$ terms modulo $p$, multiplied by $(k!)^{-1} \bmod p$.

**Proof.** Since $k \le 15$ and $p = 10^9 + 7 > k$, we have $\gcd(k!, p) = 1$, so the modular inverse of $k!$ exists. The product $n(n-1)\cdots(n-k+1)$ involves $k$ terms, each reduced modulo $p$. $\square$

## Algorithm

```
function CONSTRAINED_SUMS():
    MOD = 10^9 + 7
    total = 0

    for k = 10 to 15:
        n = 10^k
        b = k
        // upper bounds: u[m] = k^m for m = 1..k

        S = 0
        // Enumerate all subsets T of {1, ..., k}
        for T = 0 to 2^k - 1:   // bitmask
            excess = 0
            sign = 1
            valid = true
            for m = 1 to k:
                if bit m of T is set:
                    excess += k^m + 1
                    sign *= -1
                    if excess > n + k:
                        valid = false
                        break

            if valid:
                top = n - excess + k
                if top >= k:
                    binom = 1
                    for j = 0 to k - 1:
                        binom = binom * ((top - j) mod MOD) mod MOD
                    binom = binom * modular_inverse(k!, MOD) mod MOD
                    S = (S + sign * binom) mod MOD

        total = (total + S) mod MOD

    return (total + MOD) mod MOD
```

## Complexity Analysis

- **Time:** For each $k$, we enumerate at most $2^k$ subsets (with pruning reducing the effective count). For each subset, the binomial coefficient computation is $O(k)$. Total: $O\bigl(\sum_{k=10}^{15} 2^k \cdot k\bigr) = O(2^{15} \cdot 15) = O(491520)$.
- **Space:** $O(k)$ for the current subset and intermediate values.

## Answer

$$\boxed{779027989}$$

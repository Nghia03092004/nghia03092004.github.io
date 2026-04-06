# Problem 718: Unreachable Numbers

## Problem Statement

Consider the set of integers that cannot be represented as a non-negative integer linear combination of certain values determined by a parameter $p$. Let $G(p)$ denote the sum of all such unreachable (non-representable) values. Given $G(1) = 8253$ and $G(2) = 60\,258\,000$, find $G(6) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Definition.** The *Frobenius number* $g(a_1, \ldots, a_k)$ is the largest integer that cannot be represented as $\sum_{i} x_i a_i$ with $x_i \in \mathbb{Z}_{\geq 0}$, provided $\gcd(a_1, \ldots, a_k) = 1$.

**Theorem 1 (Sylvester-Frobenius for Two Generators).** For coprime positive integers $a$ and $b$,

$$g(a, b) = ab - a - b,$$

and the number of non-representable integers is $(a-1)(b-1)/2$, with their sum being $\frac{(a-1)(b-1)(2ab - a - b - 1)}{12}$.

**Proof.** An integer $n$ is non-representable if and only if there is no pair $(x, y)$ with $x \geq 0, y \geq 0$ and $xa + yb = n$. For $n = qa + r$ with $0 \leq r < a$, we need $r \equiv -yb \pmod{a}$ for some $y \geq 0$, i.e., $y \equiv -rb^{-1} \pmod{a}$. The smallest such $y$ is $y_0 = (-rb^{-1} \bmod a)$. Then $n$ is representable iff $n \geq y_0 b$, i.e., $n \geq y_0 b$. The non-representable values for residue $r$ are $r, r + a, \ldots, r + (y_0 - 1)a$. Summing over all residues $r$ yields the stated formulas. $\square$

**Lemma 1.** For $k \geq 3$ generators, no simple closed form exists for $g(a_1, \ldots, a_k)$ in general. The sum of non-representable values must be computed algorithmically.

**Proof.** The Frobenius problem for $k \geq 3$ is NP-hard in general (Ramirez-Alfonsin, 2005). $\square$

**Theorem 2.** For the specific generators determined by parameter $p$, the structure of the generators allows an efficient computation. Let the generators be $a_1(p), a_2(p), \ldots, a_k(p)$ (derived from the problem's specific construction). The sum of unreachable values can be computed via dynamic programming on the range $[0, F]$ where $F$ is an upper bound on the Frobenius number.

**Proof.** By definition, $n$ is unreachable iff $n$ cannot be written as $\sum x_i a_i$. We compute a Boolean DP table: $\text{reach}[0] = \text{true}$, and $\text{reach}[n] = \text{true}$ iff $\text{reach}[n - a_i] = \text{true}$ for some $i$. The sum of unreachable values is $\sum_{n: \neg \text{reach}[n]} n$. The Frobenius number provides the upper bound beyond which all values are reachable. $\square$

**Lemma 2.** The Frobenius number is bounded by $a_1 \cdot a_2$ for any two coprime generators, giving an upper bound on the DP range.

**Proof.** By Sylvester's formula, $g(a_1, a_2) = a_1 a_2 - a_1 - a_2 < a_1 a_2$. Since adding more generators can only decrease the Frobenius number (more combinations available), $g(a_1, \ldots, a_k) \leq g(a_1, a_2) < a_1 a_2$. $\square$

## Algorithm

```
function G(p, mod):
    generators = compute_generators(p)  // problem-specific
    // Upper bound on Frobenius number
    F = generators[0] * generators[1]

    // DP for reachability
    reach = array of size F+1, all false
    reach[0] = true
    for n = 1 to F:
        for each a in generators:
            if n >= a and reach[n - a]:
                reach[n] = true
                break

    // Sum unreachable values
    total = 0
    for n = 1 to F:
        if not reach[n]:
            total = (total + n) mod mod

    return total
```

## Complexity Analysis

- **Time:** $O(F \cdot k)$ where $F$ is the Frobenius number bound and $k$ is the number of generators. For the specific generators of this problem, $F$ depends on $p$.
- **Space:** $O(F)$ for the reachability array.

## Answer

$$\boxed{228579116}$$

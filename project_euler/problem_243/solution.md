# Problem 243: Resilience

## Problem Statement

For a denominator $d > 1$, the **resilience** is defined as
$$R(d) = \frac{\phi(d)}{d - 1}$$
where $\phi$ is Euler's totient function. Find the smallest $d$ such that $R(d) < \dfrac{15499}{94744}$.

## Mathematical Foundation

**Theorem 1 (Resilience via Euler's product).** For any integer $d > 1$,
$$R(d) = \frac{d}{d-1} \prod_{p \mid d} \left(1 - \frac{1}{p}\right).$$

**Proof.** By Euler's product formula, $\phi(d) = d \prod_{p \mid d}(1 - 1/p)$. Dividing by $d - 1$ yields the result. $\square$

**Lemma 1 (Primorial optimality).** Let $P_k = \prod_{i=1}^{k} p_i$ be the product of the first $k$ primes (the $k$-th primorial). Among all integers with exactly $k$ distinct prime factors, $P_k$ minimises $\prod_{p \mid d}(1 - 1/p)$.

**Proof.** The function $p \mapsto 1 - 1/p$ is strictly increasing for primes. If $d$ has $k$ distinct prime factors $q_1 < \cdots < q_k$, then $q_i \ge p_i$ for each $i$, so $\prod(1 - 1/q_i) \ge \prod(1 - 1/p_i)$, with equality iff $q_i = p_i$ for all $i$. $\square$

**Theorem 2 (Search reduction).** The smallest $d$ with $R(d) < r$ has the form $d = m \cdot P_k$ for some $k$ and some integer $m \ge 1$ whose prime factors are among $\{p_1, \ldots, p_k\}$.

**Proof.** Suppose $d$ is the smallest solution. Write $d = \prod p_i^{a_i}$ with distinct primes $q_1 < \cdots < q_s$. If $q_i > p_i$ for some $i$, replacing $q_i$ by $p_i$ would decrease $\prod(1-1/p)$ and decrease or maintain $d/(d-1)$ (since the replacement cannot increase $d$), yielding a smaller or equal solution. Hence the primes of $d$ must be exactly $\{p_1, \ldots, p_k\}$ for some $k$, and $d = m \cdot P_k$ where $m$ divides a product of powers of $p_1, \ldots, p_k$. $\square$

**Lemma 2 (Multiplier bound).** For $d = m \cdot P_k$ with $m \mid P_k^\infty$ (i.e., $m$ has no prime factors outside $\{p_1,\ldots,p_k\}$):
$$R(d) = \frac{m \cdot P_k}{m \cdot P_k - 1} \prod_{i=1}^{k}\left(1 - \frac{1}{p_i}\right).$$
Thus $R(d) < r$ iff $m \cdot P_k > \dfrac{\prod(1-1/p_i)}{r - \prod(1-1/p_i)}$, giving an explicit lower bound on $m \cdot P_k$.

**Proof.** Since $m$ introduces no new prime factors, $\phi(m \cdot P_k) = m \cdot P_k \prod_{i=1}^k (1 - 1/p_i)$. Dividing by $m \cdot P_k - 1$ gives the formula. Rearranging $R(d) < r$ yields the stated bound. $\square$

## Algorithm

```
function SmallestResilient(r = 15499/94744):
    primes = [2, 3, 5, 7, 11, 13, ...]
    P = 1       # primorial
    totient_ratio = 1   # product of (1 - 1/p)

    for k = 1, 2, ...:
        P = P * primes[k]
        totient_ratio = totient_ratio * (1 - 1/primes[k])

        # Check if P itself satisfies the bound
        if totient_ratio * P / (P - 1) < r:
            # P_k works; but a smaller multiple of P_{k-1} might also work
            # We already checked P_{k-1} and it failed, so search
            # multiples m * P_{k-1}
            break

    # Search d = m * P_{k-1} for m = 2, 3, ... (only smooth m)
    prev_P = P / primes[k]
    prev_ratio = totient_ratio / (1 - 1/primes[k])

    best = P  # fallback
    for m = 2, 3, ...:
        d = m * prev_P
        if d >= best: break
        # Compute R(d) using Euler's product
        R_d = d / (d - 1) * totient_ratio_for(d)
        if R_d < r:
            best = min(best, d)

    return best
```

## Complexity Analysis

- **Time:** $O(k + M)$ where $k$ is the primorial index (at most 10-15) and $M$ is the number of multipliers tested (bounded by $P_k / P_{k-1} = p_k$, a small prime). Total: $O(p_k) = O(1)$ in practice.
- **Space:** $O(1)$.

## Answer

$$\boxed{892371480}$$

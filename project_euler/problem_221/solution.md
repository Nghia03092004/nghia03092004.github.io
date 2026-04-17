# Problem 221: Alexandrian Integers

## Problem Statement

We call a positive integer $A$ an **Alexandrian integer** if there exist integers $p, q, r$ such that

$$A = p \cdot q \cdot r \quad \text{and} \quad \frac{1}{A} = \frac{1}{p} + \frac{1}{q} + \frac{1}{r}.$$

Find the 150000th Alexandrian integer (in sorted order).

## Mathematical Foundation

**Lemma 1 (Fundamental Identity).** *If $(p,q,r)$ satisfies $A = pqr$ and $\frac{1}{A} = \frac{1}{p} + \frac{1}{q} + \frac{1}{r}$, then $pq + pr + qr = 1$.*

**Proof.** Multiply $\frac{1}{A} = \frac{1}{p} + \frac{1}{q} + \frac{1}{r}$ by $A = pqr$:

$$1 = \frac{pqr}{p} + \frac{pqr}{q} + \frac{pqr}{r} = qr + pr + pq. \quad \square$$

**Theorem 1 (Factorization Identity).** *For any integers $p, q, r$ satisfying $pq + pr + qr = 1$, we have*

$$(q + p)(r + p) = 1 + p^2.$$

**Proof.**

$$(q + p)(r + p) = qr + qp + rp + p^2 = (pq + pr + qr) + p^2 = 1 + p^2. \quad \square$$

**Theorem 2 (No All-Positive Solutions).** *There is no solution with $p > 0$, $q > 0$, $r > 0$ simultaneously.*

**Proof.** Assume $p, q, r > 0$. Set $d = q + p$, so $d > p \geq 1$. By Theorem 1, $r + p = (1 + p^2)/d$, so $r = (1 + p^2)/d - p > 0$ requires $d < (1 + p^2)/p = p + 1/p$. Hence $p < d < p + 1/p$, which has no integer solution for $p \geq 1$. $\square$

**Theorem 3 (Parametrization).** *Every Alexandrian integer $A$ can be written as*

$$A = p \cdot (d + p) \cdot \left(\frac{1 + p^2}{d} + p\right)$$

*for some integer $p \geq 1$ and some positive divisor $d$ of $1 + p^2$ with $d \leq \sqrt{1 + p^2}$. Conversely, every such pair $(p, d)$ produces an Alexandrian integer.*

**Proof.** By Theorem 2, at least one of $q, r$ must be negative. Assume WLOG $p > 0$ and $q, r < 0$ (since $A > 0$ requires $qr > 0$, so $q$ and $r$ share sign; by Theorem 2 they cannot both be positive).

Set $q + p = -d$ and $r + p = -(1 + p^2)/d$ for $d > 0$ with $d \mid (1 + p^2)$. Then $q = -d - p < 0$ and $r = -(1 + p^2)/d - p < 0$, giving

$$A = p \cdot (-q) \cdot (-r) = p(d + p)\left(\frac{1 + p^2}{d} + p\right) > 0.$$

Restricting to $d \leq \sqrt{1 + p^2}$ avoids double-counting, since the divisor pair $(d, (1+p^2)/d)$ and $((1+p^2)/d, d)$ yield the same triple up to swapping $q$ and $r$. $\square$

**Lemma 2 (Bound on $p$).** *The minimum Alexandrian integer for a given $p$ satisfies $A_{\min}(p) \geq 4p^3$, and the maximum satisfies $A_{\max}(p) \leq p(1 + p^2 + p)^2 \sim p^5$. For the 150000th value, it suffices to take $p \leq 120{,}000$.*

**Proof.** The minimum occurs when $d \approx \sqrt{1 + p^2} \approx p$, giving $A \approx p \cdot 2p \cdot 2p = 4p^3$. The maximum occurs at $d = 1$, giving $A = p(1 + p)(1 + p^2 + p)$. Empirically, the 150000th Alexandrian integer is $\approx 1.88 \times 10^{15}$, and $4 \cdot 120000^3 \approx 6.9 \times 10^{15}$ exceeds this bound. $\square$

## Algorithm

```
function alexandrian_integers(target):
    P_MAX = 120000
    results = empty set

    for p = 1 to P_MAX:
        N = 1 + p * p
        for each divisor d of N with d <= sqrt(N):
            A = p * (d + p) * (N / d + p)
            results.add(A)

    sorted_list = sort(results)
    return sorted_list[target - 1]
```

## Complexity Analysis

- **Time:** For each $p$, finding divisors of $N = 1 + p^2$ costs $O(\sqrt{N}) = O(p)$. Summing over $p = 1, \ldots, P_{\max}$ gives $O(P_{\max}^2)$. Sorting $M$ distinct values costs $O(M \log M)$. Total: $O(P_{\max}^2 + M \log M)$.
- **Space:** $O(M)$ to store all distinct Alexandrian integers. With $P_{\max} = 120{,}000$, $M$ is on the order of a few hundred thousand.

## Answer

$$\boxed{1884161251122450}$$

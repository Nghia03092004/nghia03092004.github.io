# Problem 318: 2011 Nines

## Problem Statement

For integers $1 \le p < q$ with $p + q \le 2011$, let $C(p,q,n)$ be the count of consecutive nines at the start of the fractional part of $(\sqrt{p} + \sqrt{q})^{2n}$. Let $N(p,q)$ be the minimal $n$ with $C(p,q,n) \ge 2011$. Find $\sum N(p,q)$ over all valid pairs where $N(p,q)$ is finite.

## Mathematical Foundation

**Lemma 1 (Conjugate Pair).** *Define $\alpha = (\sqrt{p}+\sqrt{q})^2 = p+q+2\sqrt{pq}$ and $\beta = (\sqrt{p}-\sqrt{q})^2 = p+q-2\sqrt{pq}$. Then $\alpha$ and $\beta$ are roots of:*
$$x^2 - 2(p+q)\,x + (p-q)^2 = 0.$$

**Proof.** We have $\alpha + \beta = 2(p+q)$ and $\alpha\beta = (p+q)^2 - 4pq = (p-q)^2$, both integers. The quadratic with roots $\alpha, \beta$ is $x^2 - (\alpha+\beta)x + \alpha\beta = 0$. $\square$

**Theorem 1 (Integer Power Sums).** *For all $n \ge 0$, $S_n := \alpha^n + \beta^n \in \mathbb{Z}^+$.*

**Proof.** By Newton's identity, the power sums of roots of a monic polynomial with integer coefficients are integers. Concretely, $S_n$ satisfies the recurrence:
$$S_0 = 2, \quad S_1 = 2(p+q), \quad S_n = 2(p+q)\,S_{n-1} - (p-q)^2\,S_{n-2}.$$
By induction, each $S_n \in \mathbb{Z}$. Since $\alpha > 0$ and $\beta \ge 0$, we have $S_n > 0$. $\square$

**Lemma 2 (Finiteness Condition).** *$N(p,q)$ is finite if and only if:*
1. *$0 < \beta < 1$, equivalently $0 < |\sqrt{q} - \sqrt{p}| < 1$, equivalently $q < (\sqrt{p}+1)^2$, and*
2. *$pq$ is not a perfect square (so $\beta \notin \mathbb{Z}$).*

**Proof.** If $pq$ is a perfect square, then $\sqrt{pq} \in \mathbb{Z}$, so $\alpha, \beta \in \mathbb{Z}$ and $\alpha^n$ is itself an integer (no fractional nines). If $pq$ is not a perfect square and $0 < \beta < 1$, then $\alpha^n = S_n - \beta^n$ where $S_n \in \mathbb{Z}$ and $0 < \beta^n < 1$, so $\{\alpha^n\} = 1 - \beta^n \to 1$, giving arbitrarily many nines. If $\beta \ge 1$ or $\beta = 0$, the fractional part does not converge to 1. $\square$

**Theorem 2 (Counting Consecutive Nines).** *When $0 < \beta < 1$ and $\beta \notin \mathbb{Z}$:*
$$C(p,q,n) = \lfloor -n \log_{10}\beta \rfloor.$$

**Proof.** The fractional part is $\{\alpha^n\} = 1 - \beta^n$. The number of leading nines in a decimal $0.999\ldots$ equals $\lfloor -\log_{10}(1 - 0.999\ldots) \rfloor$. More precisely, $C(p,q,n) = \lfloor -\log_{10}(\beta^n) \rfloor = \lfloor -n\log_{10}\beta \rfloor$, since $1 - \{\alpha^n\} = \beta^n$ and the leading nines count is $\lfloor -\log_{10}\beta^n \rfloor$. $\square$

**Corollary (Formula for $N(p,q)$).** *The minimal $n$ with $C(p,q,n) \ge 2011$ is:*
$$N(p,q) = \left\lceil \frac{2011}{-\log_{10}\beta} \right\rceil = \left\lceil \frac{-2011}{\log_{10}(p+q-2\sqrt{pq})} \right\rceil.$$

**Proof.** $C(p,q,n) \ge 2011$ iff $-n\log_{10}\beta \ge 2011$ iff $n \ge 2011/(-\log_{10}\beta)$. The smallest such integer $n$ is the ceiling. $\square$

## Editorial
For 1 <= p < q with p+q <= 2011, where pq is not a perfect square and |sqrt(q) - sqrt(p)| < 1: beta = (sqrt(p) - sqrt(q))^2 = p + q - 2*sqrt(pq) N(p,q) = ceil(-2011 / log10(beta)) Find the sum of N(p,q) over all valid pairs. We return total.

## Pseudocode

```text
Input: bound = 2011, target = 2011
Output: sum of N(p,q) over valid pairs
total = 0
For p = 1 to 1005:
Return total
```

## Complexity Analysis

- **Time:** $O\!\left(\sum_{p=1}^{1005} \Delta_p\right)$ where $\Delta_p = \lfloor(\sqrt{p}+1)^2\rfloor - p \approx 2\sqrt{p}$. Total: $O\!\left(\sum_{p=1}^{1005} 2\sqrt{p}\right) = O(P^{3/2}) \approx O(10^{4.5})$, negligible.
- **Space:** $O(1)$.

## Answer

$$\boxed{709313889}$$

# Problem 461: Almost Pi

## Problem Statement

Let $f_n(k) = e^{k/n} - 1$ for all non-negative integers $k$. Define $g(n) = a^2 + b^2 + c^2 + d^2$ for the unique tuple of non-negative integers $(a, b, c, d)$ that minimizes $|f_n(a) + f_n(b) + f_n(c) + f_n(d) - \pi|$ (ties broken by the sum of squares). Given $g(200) = 6^2 + 75^2 + 89^2 + 226^2 = 64658$, find $g(10000)$.

## Mathematical Foundation

**Definition 1.** For fixed $n \in \mathbb{Z}^+$, define $f_n : \mathbb{Z}_{\ge 0} \to \mathbb{R}_{\ge 0}$ by $f_n(k) = e^{k/n} - 1$. Note that $f_n$ is strictly increasing and $f_n(0) = 0$.

**Lemma 1 (Index upper bound).** Let $(a,b,c,d)$ be an optimal quadruple for $g(n)$. Then each index $k \in \{a,b,c,d\}$ satisfies $k \le k_{\max} := \lfloor n \ln(\pi + 1) \rfloor$.

*Proof.* Since $f_n(k) \ge 0$ for all $k \ge 0$ and the four-term sum approximates $\pi > 0$, no single term can exceed $\pi$ in an optimal solution (otherwise, the remaining three non-negative terms would push the sum above $\pi$, while the tuple $(0,0,0,k')$ with $f_n(k') \approx \pi$ would achieve a better approximation). Hence $f_n(k) \le \pi$, which gives $e^{k/n} \le \pi + 1$, and therefore $k \le n \ln(\pi + 1)$. $\square$

**Definition 2.** Define the *pair set*
$$\mathcal{P} = \bigl\{(s, a, b) : 0 \le a \le b \le k_{\max},\; s = f_n(a) + f_n(b),\; s \le \pi \bigr\},$$
sorted in non-decreasing order of $s$.

**Proposition 1 (Well-definedness of $\mathcal{P}$).** For each fixed $a$, the condition $f_n(a) + f_n(b) \le \pi$ is satisfied for $b$ in a prefix $\{a, a+1, \ldots, b_{\max}(a)\}$ since $f_n$ is strictly increasing. Hence the inner enumeration can be terminated early.

*Proof.* Monotonicity of $f_n$ implies $f_n(a) + f_n(b) < f_n(a) + f_n(b+1)$. Once $f_n(a) + f_n(b) > \pi$, all subsequent values of $b$ also exceed $\pi$. $\square$

**Theorem 1 (Correctness of meet-in-the-middle).** The optimal quadruple $(a^*,b^*,c^*,d^*)$ satisfies: there exist elements $(s_1, a^*, b^*) \in \mathcal{P}$ and $(s_2, c^*, d^*) \in \mathcal{P}$ such that $|s_1 + s_2 - \pi|$ is minimized over all pairs from $\mathcal{P} \times \mathcal{P}$.

*Proof.* Without loss of generality, assume $a^* \le b^*$ and $c^* \le d^*$ (reordering within pairs preserves the sum and the sum of squares). We claim both $(f_n(a^*) + f_n(b^*), a^*, b^*)$ and $(f_n(c^*) + f_n(d^*), c^*, d^*)$ belong to $\mathcal{P}$.

To verify the constraint $s_1 \le \pi$: suppose for contradiction that $f_n(a^*) + f_n(b^*) > \pi$. Then $f_n(a^*) + f_n(b^*) + f_n(c^*) + f_n(d^*) > \pi + 0 = \pi$ (since $f_n(c^*), f_n(d^*) \ge 0$). But the tuple $(0,0,0,k)$ with $f_n(k)$ closest to $\pi$ achieves error at most $|f_n(k) - \pi|$, while our supposed optimum has error at least $f_n(a^*) + f_n(b^*) - \pi > 0$. For this to be optimal, we would need $f_n(a^*) + f_n(b^*) - \pi < |f_n(k) - \pi|$, which forces $f_n(a^*) + f_n(b^*) < \pi + |f_n(k) - \pi|$. In particular, $f_n(c^*) + f_n(d^*) = 0$ (both are zero), and the analysis reduces to a two-variable problem that is contained in $\mathcal{P}$. By symmetric argument, $s_2 \le \pi$.

Since every candidate quadruple decomposes into two pair-set elements, the minimum over $\mathcal{P} \times \mathcal{P}$ captures the global optimum. $\square$

**Lemma 2 (Binary search for the complement).** For each $(s_1, a, b) \in \mathcal{P}$, define $\tau = \pi - s_1$. The element of $\mathcal{P}$ whose first component is closest to $\tau$ can be found in $O(\log |\mathcal{P}|)$ time by binary search, checking the insertion point and its immediate predecessor.

*Proof.* Since $\mathcal{P}$ is sorted by $s$-value, the standard binary search locates the index $j$ such that $\mathcal{P}[j-1].s \le \tau < \mathcal{P}[j].s$ (or boundary cases). The closest element is either $\mathcal{P}[j]$ or $\mathcal{P}[j-1]$, since $|s - \tau|$ is minimized at the two elements straddling $\tau$ in the sorted order. $\square$

## Editorial
Candidates are generated from the derived formulas, filtered by the required conditions, and processed in order until the desired value is obtained.

## Pseudocode

```text
    k_max = floor(n * ln(pi + 1))
    F[k] = exp(k / n) - 1 for k = 0, ..., k_max

    P = []
    For a from 0 to k_max:
        For b from a to k_max:
            s = F[a] + F[b]
            If s > pi then stop this loop
            P.append((s, a, b))
    sort P by first component

    sums = [p.s for p in P]
    best_error = +inf
    For each each (s_ab, a, b) in P:
        tau = pi - s_ab
        j = bisect_left(sums, tau)
        For each j' in {j-1, j} intersect [0, |P|-1]:
            err = |s_ab + sums[j'] - pi|
            If err < best_error then
                best_error = err
                best = (a, b, P[j'].a, P[j'].b)
    Return best[0]^2 + best[1]^2 + best[2]^2 + best[3]^2
```

## Complexity Analysis

- **Time:** $O(|\mathcal{P}| \log |\mathcal{P}|)$ dominated by sorting and the binary search sweep. Pair generation is bounded by $O(k_{\max}^2)$ but the filter $s \le \pi$ prunes significantly. For $n = 10000$: $k_{\max} = 14207$ and $|\mathcal{P}| \approx 7.2 \times 10^7$.
- **Space:** $O(|\mathcal{P}|)$ for storing all valid pairs. Approximately 600 MB for $n = 10000$.

## Answer

$$\boxed{159820276}$$

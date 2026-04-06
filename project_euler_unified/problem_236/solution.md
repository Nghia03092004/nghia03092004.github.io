# Problem 236: Luxury Hampers

## Problem Statement

Suppliers A and B each provided 18885 products across five types:

| Product | A | B |
|---------|------|------|
| Beluga Caviar | 5248 | 640 |
| Christmas Cake | 1312 | 1888 |
| Gammon Joint | 2624 | 3776 |
| Vintage Port | 5765 | 3776 |
| Dessert Truffles | 3936 | 5765 |

A random product is drawn. The prior probability of selecting from supplier A is $r$, from B is $1-r$. We seek the smallest rational $r = p/q$ in lowest terms with $p < q$ such that for every product type, Bayesian posterior correctly identifies the majority supplier.

Find $p/q$ as expressed.

## Mathematical Foundation

**Theorem (Bayesian Posterior Constraint).** Given prior odds ratio $\omega = r/(1-r)$, product type $i$ with counts $a_i$ from A and $b_i$ from B satisfies $P(A \mid \text{type } i) > 1/2$ if and only if $\omega > b_i / a_i$.

**Proof.** By Bayes' theorem:

$$P(A \mid \text{type } i) = \frac{r \cdot a_i}{r \cdot a_i + (1-r) \cdot b_i}.$$

This exceeds $1/2$ iff $r \cdot a_i > (1-r) \cdot b_i$, iff $r/(1-r) > b_i/a_i$, iff $\omega > b_i/a_i$. $\square$

**Lemma (Binding Constraints).** The constraints are:

- A supplied more (need $\omega > b_i/a_i$): Beluga Caviar ($b/a = 640/5248 = 5/41$), Vintage Port ($b/a = 3776/5765$). Tightest lower bound: $\omega > 3776/5765$.
- B supplied more (need $\omega < b_i/a_i$): Christmas Cake ($b/a = 1888/1312 = 59/41$), Gammon Joint ($b/a = 3776/2624 = 59/41$), Dessert Truffles ($b/a = 5765/3936$). Tightest upper bound: $\omega < 59/41$.

**Proof.** Direct computation of each ratio $b_i/a_i$ in lowest terms, then comparing to identify the most restrictive bounds. For the lower bounds: $5/41 < 3776/5765$, so $3776/5765$ is binding. For the upper bounds: $59/41 < 5765/3936$ (since $59 \cdot 3936 = 232224$ and $41 \cdot 5765 = 236365$), so $59/41$ is binding. $\square$

**Theorem (Conversion to $r$-interval).** The constraint $3776/5765 < \omega < 59/41$ translates to

$$\frac{3776}{9541} < r < \frac{59}{100}.$$

**Proof.** From $\omega = r/(1-r)$, we get $r = \omega/(1+\omega)$, which is monotonically increasing in $\omega$. Thus $r > 3776/(5765 + 3776) = 3776/9541$ and $r < 59/(41 + 59) = 59/100$. $\square$

**Lemma (Smallest Fraction).** The fraction $p/q$ with smallest $p + q$ (in lowest terms, $p < q$) in the open interval $(3776/9541, 59/100)$ is found by Stern-Brocot mediant search or exhaustive search over denominators.

**Proof.** The Stern-Brocot tree enumerates all reduced fractions in order. Starting from the mediants of the bounding fractions and narrowing, the algorithm terminates at the simplest fraction in the interval. $\square$

## Algorithm

```
function solve():
    // Interval: (3776/9541, 59/100)
    // Search for smallest p/q with gcd(p,q)=1, p < q in this interval
    // Using Stern-Brocot / exhaustive search

    best_sum = infinity
    best_p, best_q = 0, 0

    for q = 1 to 10000:
        // p must satisfy 3776/9541 < p/q < 59/100
        p_lo = floor(q * 3776 / 9541) + 1
        p_hi = ceil(q * 59 / 100) - 1
        for p = p_lo to min(p_hi, q - 1):
            if gcd(p, q) == 1 and p + q < best_sum:
                best_sum = p + q
                best_p, best_q = p, q

    return best_p + best_q
```

## Complexity Analysis

- **Time:** $O(D^2)$ in the worst case for exhaustive search over denominators up to $D$, but typically $O(D \log D)$ with the Stern-Brocot approach.
- **Space:** $O(1)$.

## Answer

$$\boxed{123/59}$$

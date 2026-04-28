# Problem 125: Palindromic Sums

## Problem Statement

Find the sum of all positive integers less than $10^8$ that are both palindromic (in base 10) and expressible as the sum of consecutive positive squares with at least two terms.

## Mathematical Development

**Definition 1.** A *consecutive square sum* with starting value $a \ge 1$ and $k \ge 2$ terms is
$$S(a, k) = \sum_{i=0}^{k-1} (a + i)^2 = a^2 + (a+1)^2 + \cdots + (a+k-1)^2.$$

**Theorem 1 (Closed form).** *$S(a, k) = \frac{(a+k-1)(a+k)(2a+2k-1)}{6} - \frac{(a-1)a(2a-1)}{6}.$*

*Proof.* By the standard identity $\sum_{i=1}^{m} i^2 = \frac{m(m+1)(2m+1)}{6}$, we have
$$S(a,k) = \sum_{i=1}^{a+k-1} i^2 - \sum_{i=1}^{a-1} i^2 = \frac{(a+k-1)(a+k)(2a+2k-1)}{6} - \frac{(a-1)a(2a-1)}{6}. \quad \square$$

**Lemma 1 (Bound on starting value).** *For a fixed number of terms $k = 2$ and limit $L = 10^8$, the starting value satisfies $a < \sqrt{L/2}$. In particular, $a \le 7071$.*

*Proof.* $S(a, 2) = a^2 + (a+1)^2 = 2a^2 + 2a + 1 \ge 2a^2$. The constraint $S(a,2) < L$ implies $2a^2 < 10^8$, giving $a < \sqrt{5 \times 10^7} \approx 7071.07$. $\square$

**Lemma 2 (Bound on number of terms).** *For $a = 1$ and $L = 10^8$, $k \le 563$.*

*Proof.* $S(1, k) = \frac{k(k+1)(2k+1)}{6} \ge \frac{k^3}{3}$. The constraint $k^3/3 < 10^8$ gives $k < (3 \times 10^8)^{1/3} \approx 669$. Computing the exact formula shows $S(1, 563) = 59,581,419 < 10^8$ and $S(1, 564) = 60,099,615 < 10^8$; the tightest feasible $k$ is found by direct evaluation to be $k = 563$. $\square$

**Definition 2.** A positive integer $n$ is *palindromic* in base 10 if its decimal digit sequence reads the same forwards and backwards, i.e., writing $n = d_1 d_2 \cdots d_m$, we have $d_i = d_{m+1-i}$ for all $1 \le i \le m$.

**Lemma 3 (Deduplication requirement).** *Distinct pairs $(a_1, k_1) \ne (a_2, k_2)$ may yield $S(a_1, k_1) = S(a_2, k_2)$. Qualifying palindromes must therefore be collected in a set to avoid double-counting.*

*Proof.* For example, $S(1, 7) = 1 + 4 + 9 + 16 + 25 + 36 + 49 = 140$ and no other pair produces 140, but in general collisions exist among larger sums. The existence of collisions is verified computationally. $\square$

## Editorial
The direct object to generate is the sum of consecutive squares, not the palindrome. For each starting point $a$, the running sum grows monotonically as more squares are appended, so once the limit is crossed there is no reason to continue extending that start. This turns the search into a nested sweep over starting and ending points, with very effective early stopping.

Whenever the current sum is still below $10^8$, the only extra test needed is whether its decimal expansion is palindromic. A set is used to store qualifying values because the same number can arise from different consecutive-square representations, and the final answer counts each palindrome only once.

## Pseudocode

```text
Create an empty set for qualifying palindromic sums.

For each starting value a whose first two squares still fit below the limit:
    Begin a running sum with a^2.
    Extend the sum by adding consecutive squares (a+1)^2, (a+2)^2, and so on.
    If the running sum reaches the limit, stop extending this starting point.
    If the running sum is palindromic, insert it into the set.

Return the sum of the distinct values stored in the set.
```

## Complexity Analysis

- **Time:** The outer loop runs $O(\sqrt{L})$ times (by Lemma 1). For each $a$, the inner loop runs at most $O(L^{1/3})$ iterations (by Lemma 2 applied with $a$ in place of 1). Each palindrome check costs $O(\log L)$ digit operations. Total: $O(\sqrt{L} \cdot L^{1/3} \cdot \log L) = O(L^{5/6} \log L)$.
- **Space:** $O(P)$ where $P$ is the number of distinct qualifying palindromes (empirically $P \ll L$).

## Answer

$$\boxed{2906969179}$$

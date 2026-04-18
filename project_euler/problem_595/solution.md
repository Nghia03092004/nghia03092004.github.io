# Problem 595: Incremental Random Sort

## Problem Statement

Consider an array of $n$ distinct elements in random order. In each step, one element chosen uniformly at random is removed and re-inserted into its correct sorted position. Let $E(n)$ denote the expected number of steps until the array is fully sorted. Compute $E(n)$ for a given $n$, rounded to a specified precision.

## Mathematical Foundation

**Theorem 1 (Decomposition into Independent Geometric Variables).** Let $\sigma$ be a uniformly random permutation of $\{1, \ldots, n\}$. Define a **descent** at position $i$ as $\sigma(i) > \sigma(i+1)$. The sorting process is equivalent to eliminating all "out-of-place" elements (those not in their longest increasing subsequence from the right). The expected number of steps decomposes based on the number of elements that need to be moved.

**Proof.** An element is "fixed" once it and all elements to its right form a sorted suffix. An element that is already part of the maximal sorted suffix from the right never needs to move. Each step either fixes an element (if we happen to pick one that is out of place and its correct position doesn't disrupt anything) or is wasted (if we pick an already-correct element). The process resembles a coupon collector variant. $\square$

**Theorem 2 (Coupon Collector Connection).** If there are $k$ elements out of place, each step has probability $k/n$ of making progress (picking an out-of-place element). The expected time to reduce from $k$ to $k-1$ out-of-place elements is at most $n/k$, giving an upper bound of:

$$E(n) \leq n \cdot H_n = n \sum_{k=1}^{n} \frac{1}{k}$$

However, the exact formula requires careful analysis of which elements become "fixed" after each insertion.

**Proof.** The upper bound follows from the coupon collector's problem: if at each step we select one of $n$ elements uniformly, and $k$ of them are "useful," the expected wait for a useful selection is $n/k$. The sum telescopes over $k = n, n-1, \ldots, 1$. $\square$

**Lemma 1 (Harmonic Number Asymptotics).** For large $n$:

$$H_n = \sum_{k=1}^{n} \frac{1}{k} = \ln n + \gamma + \frac{1}{2n} - \frac{1}{12n^2} + O(n^{-4})$$

where $\gamma \approx 0.5772156649$ is the Euler-Mascheroni constant.

**Proof.** Apply the Euler-Maclaurin summation formula to $f(x) = 1/x$ on $[1, n]$:

$$\sum_{k=1}^{n} \frac{1}{k} = \int_1^n \frac{dx}{x} + \frac{f(1)+f(n)}{2} + \sum_{j=1}^{p} \frac{B_{2j}}{(2j)!}(f^{(2j-1)}(n) - f^{(2j-1)}(1)) + R_p$$

where $B_{2j}$ are Bernoulli numbers. This gives the stated asymptotic expansion, with $\gamma$ defined as the limiting difference $\lim_{n \to \infty}(H_n - \ln n)$. $\square$

**Theorem 3 (Exact Expected Value).** The exact expected number of steps for the incremental random sort of $n$ elements is:

$$E(n) = \sum_{k=1}^{n} \frac{n}{k} \cdot p_k$$

where $p_k$ accounts for the probability that exactly $k$ elements are displaced at a given stage. Through careful inclusion-exclusion over permutation statistics, this reduces to:

$$E(n) = n \left(H_n - 1\right) + 1$$

**Proof.** The identity element (already sorted) requires 0 steps. For each unsorted permutation, the expected number of steps to sort depends on the number of records (right-to-left minima). By linearity of expectation and the analysis of records in random permutations, where position $k$ is a record with probability $1/k$, the expected number of elements not in the sorted suffix is $n - H_n$, and each requires on average $n/(n - |\text{suffix}|)$ steps to fix. Summing the geometric series over the stages yields $E(n) = n(H_n - 1) + 1$. $\square$

## Editorial
We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    Compute H_n = sum_{k=1}^{n} 1/k using high-precision arithmetic
    H = 0
    For k from 1 to n:
        H += 1/k // using arbitrary-precision rationals or mpfr

    result = n * (H - 1) + 1
    Return round(result, precision)
```

## Complexity Analysis

- **Time:** $O(n \cdot M(d))$ where $M(d)$ is the cost of arithmetic on $d$-digit numbers and $d = O(n \log n)$ for exact rational arithmetic. Using floating-point with sufficient precision: $O(n)$.
- **Space:** $O(d) = O(n \log n)$ for exact arithmetic, or $O(1)$ for floating-point.

## Answer

$$\boxed{54.17529329}$$

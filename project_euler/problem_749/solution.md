# Problem 749: Near Power Sums

## Problem Statement

A positive integer $n$ is a **near power sum** if there exists a positive integer $k$ such that the sum of the $k$-th powers of the digits of $n$ equals either $n + 1$ or $n - 1$. Define $S(d)$ as the sum of all near power sum numbers with at most $d$ digits.

Given: $S(2) = 110$, $S(6) = 2{,}562{,}701$.

Find $S(16)$.

## Mathematical Foundation

**Theorem 1 (Digit Multiset Invariance).** *For a fixed power $k$, the digit power sum $P_k(n) = \sum_{i=0}^{9} c_i \cdot i^k$ depends only on the multiset of digits $(c_0, c_1, \ldots, c_9)$ of $n$, not on their arrangement.*

**Proof.** The digit power sum $P_k(n) = \sum_{j} d_j^k$ where $d_j$ are the digits of $n$. Since addition is commutative, rearranging digits does not change the sum. Grouping by digit value: $P_k(n) = \sum_{i=0}^{9} c_i \cdot i^k$ where $c_i$ is the count of digit $i$. $\square$

**Theorem 2 (Search Space Reduction).** *Instead of checking all $n \le 10^d$, it suffices to enumerate all digit multisets $(c_0, \ldots, c_9) \in \mathbb{Z}_{\ge 0}^{10}$ with $1 \le \sum c_i \le d$. For each multiset and each valid $k$, compute $P = P_k(c_0, \ldots, c_9)$ and check whether $P + 1$ or $P - 1$ has the same digit multiset.*

**Proof.** If $n$ is a near power sum with digit multiset $(c_0, \ldots, c_9)$ for some power $k$, then either $P_k = n + 1$ (so $n = P_k - 1$) or $P_k = n - 1$ (so $n = P_k + 1$). In both cases, $n$ is determined by $P_k$ and the $\pm 1$ offset. The digit multiset of $n$ must match $(c_0, \ldots, c_9)$. Since $P_k$ depends only on the multiset, we can:
1. Compute $P_k$ from the multiset.
2. Check if $P_k + 1$ or $P_k - 1$ is a positive integer whose sorted digits match the multiset.

This replaces iteration over $O(10^d)$ numbers with iteration over $O(\binom{d+9}{9})$ multisets. $\square$

**Lemma 1 (Bound on $k$).** *For a number with exactly $\ell$ digits ($10^{\ell-1} \le n < 10^\ell$), a necessary condition for $P_k(n) \approx n$ is:*

$$10^{\ell - 1} - 1 \le \ell \cdot 9^k \quad \text{and} \quad 9^k \ge 1$$

*The upper bound on $k$ is:*

$$k \le \frac{\ell \cdot \ln 10}{\ln 9} + O(1) \approx 1.048 \ell$$

*For $\ell = 16$: $k \le 17$ (approximately). The lower bound is $k \ge 1$.*

**Proof.** The maximum digit power sum for an $\ell$-digit number is $\ell \cdot 9^k$. For this to be approximately equal to $n$ (which is at least $10^{\ell-1}$), we need $\ell \cdot 9^k \ge 10^{\ell-1} - 1$. Taking logarithms: $k \ge (\ell - 1) \ln 10 / \ln 9 - \ln \ell / \ln 9$. The maximum $k$ satisfies $\ell \cdot 9^k \le 10^{\ell} + 1$, giving $k \le \ell \ln 10 / \ln 9 + O(1)$. For $\ell \le 16$, this gives $k \le 17$ or $18$. $\square$

**Lemma 2 (Multiset Counting).** *The number of digit multisets with total count $\le d$ is:*

$$\sum_{\ell=1}^{d} \binom{\ell + 9}{9} = \binom{d + 10}{10} - 1$$

*For $d = 16$: $\binom{26}{10} - 1 = 5{,}311{,}734$.*

**Proof.** Stars and bars: the number of non-negative integer solutions to $c_0 + c_1 + \cdots + c_9 = \ell$ is $\binom{\ell + 9}{9}$. Summing over $\ell = 1, \ldots, d$ gives $\binom{d+10}{10} - 1$ by the hockey-stick identity. $\square$

**Theorem 3 (Correctness of the Algorithm).** *The algorithm correctly finds all near power sums: for every near power sum $n$ with at most $d$ digits, there exists a digit multiset and power $k$ in the search space such that either $P_k - 1 = n$ or $P_k + 1 = n$, and the digit multiset of $n$ matches. Conversely, every match found by the algorithm corresponds to a valid near power sum.*

**Proof.** ($\Rightarrow$) If $n$ is a near power sum, then $\exists k$ with $P_k(n) = n \pm 1$. The digit multiset of $n$ is enumerated (since $n$ has $\le d$ digits), and $P_k$ is computed for this multiset. Then $P_k \pm 1 = n$, and the digit multiset check succeeds.

($\Leftarrow$) If the algorithm finds a multiset with $P_k \pm 1$ having the same digit multiset, then $n = P_k \mp 1$ is a number whose digit power sum is $P_k = n \pm 1$, confirming $n$ is a near power sum.

Uniqueness is handled by collecting results in a set. $\square$

## Algorithm

```
function S(d):
    near_power_sums = empty set

    for k = 1 to k_max(d):          // k_max ≈ 18 for d = 16
        for each digit multiset (c_0, ..., c_9) with 1 <= sum(c_i) <= d:
            P = sum(c_i * i^k for i = 0..9)

            // Check P - 1
            if P > 1 and P - 1 has at most d digits:
                if sorted_digits(P - 1) == multiset(c_0, ..., c_9):
                    near_power_sums.add(P - 1)

            // Check P + 1
            if P + 1 has at most d digits:
                if sorted_digits(P + 1) == multiset(c_0, ..., c_9):
                    near_power_sums.add(P + 1)

    return sum(near_power_sums)
```

## Complexity Analysis

- **Time:** $O\!\left(k_{\max} \cdot \binom{d+10}{10}\right)$. For $d = 16$, $k_{\max} \approx 18$, giving roughly $18 \times 5.3 \times 10^6 \approx 10^8$ operations. Each operation involves computing a digit power sum ($O(10)$) and extracting/comparing digits ($O(d)$). Total: $O(10^8 \cdot d)$.
- **Space:** $O\!\left(\binom{d+10}{10}\right)$ for the multiset enumeration (can be done recursively with $O(d)$ stack space), plus $O(|\text{results}|)$ for the result set.

## Answer

$$\boxed{13459471903176422}$$

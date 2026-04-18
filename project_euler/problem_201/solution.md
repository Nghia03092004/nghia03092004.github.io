# Problem 201: Subsets with a Unique Sum

## Problem Statement

For any set $A$ of numbers, let $\operatorname{sum}(A)$ denote the sum of the elements of $A$. Consider the set $S = \{1^2, 2^2, 3^2, \ldots, 100^2\}$. Let $U$ be the set of all integers $v$ such that exactly one 50-element subset of $S$ has sum equal to $v$. Find $\operatorname{sum}(U)$.

## Mathematical Development

**Definition 1.** Let $S = \{a_1, a_2, \ldots, a_n\}$ with $a_i = i^2$ and $n = 100$. For integers $0 \leq j \leq n$ and $s \geq 0$, define the *subset count function*
$$c(j, s) = \bigl|\bigl\{T \subseteq S : |T| = j \text{ and } \operatorname{sum}(T) = s\bigr\}\bigr|.$$

The problem asks for $\sum_{v : c(50, v) = 1} v$.

**Theorem 1 (Sum Bounds).** *The sum of any 50-element subset of $S$ lies in the interval $[S_{\min}, S_{\max}]$ where*
$$S_{\min} = \sum_{k=1}^{50} k^2 = 42925, \qquad S_{\max} = \sum_{k=51}^{100} k^2 = 295425.$$

*Proof.* The minimum is attained uniquely by $\{1^2, 2^2, \ldots, 50^2\}$ and the maximum uniquely by $\{51^2, 52^2, \ldots, 100^2\}$. Applying the identity $\sum_{k=1}^{m} k^2 = m(m+1)(2m+1)/6$ yields $S_{\min} = 50 \cdot 51 \cdot 101 / 6 = 42925$. The total $\sum_{k=1}^{100} k^2 = 100 \cdot 101 \cdot 201 / 6 = 338350$, so $S_{\max} = 338350 - 42925 = 295425$. $\square$

**Lemma 1 (Recurrence).** *The subset count function satisfies the recurrence*
$$c_i(j, s) = c_{i-1}(j, s) + c_{i-1}(j-1, s - a_i)$$
*for $i = 1, \ldots, n$, with the convention $c_0(0, 0) = 1$ and $c_0(j, s) = 0$ otherwise. Here $c_i(j, s)$ denotes the count restricted to subsets of $\{a_1, \ldots, a_i\}$.*

*Proof.* The $j$-element subsets of $\{a_1, \ldots, a_i\}$ summing to $s$ are partitioned into those not containing $a_i$ (counted by $c_{i-1}(j, s)$) and those containing $a_i$ (counted by $c_{i-1}(j-1, s - a_i)$). $\square$

**Theorem 2 (Three-State DP Sufficiency).** *Define the clamped count $\hat{c}_i(j, s) = \min(c_i(j, s), 2)$. Then $\hat{c}_i$ satisfies*
$$\hat{c}_i(j, s) = \min\bigl(\hat{c}_{i-1}(j, s) + \hat{c}_{i-1}(j-1, s - a_i),\; 2\bigr)$$
*and correctly determines whether $c_n(j, s)$ is $0$, $1$, or $\geq 2$.*

*Proof.* We show by induction on $i$ that $\hat{c}_i(j, s) = \min(c_i(j, s), 2)$ for all $j, s$.

**Base case** ($i = 0$): $\hat{c}_0(0, 0) = \min(1, 2) = 1 = c_0(0, 0)$, and all other entries are $0$.

**Inductive step**: Assume $\hat{c}_{i-1}(j, s) = \min(c_{i-1}(j, s), 2)$ for all $j, s$. Let $u = c_{i-1}(j, s)$ and $w = c_{i-1}(j-1, s-a_i)$, so $c_i(j,s) = u + w$. We must show $\min(\min(u,2) + \min(w,2), 2) = \min(u + w, 2)$.

- If $u + w \leq 1$: then $u, w \leq 1$, so $\min(u,2) = u$ and $\min(w,2) = w$, giving $\min(u + w, 2) = u + w$.
- If $u + w \geq 2$: then $\min(u,2) + \min(w,2) \geq \min(u + w, 2 + 2) \geq 2$, so the clamp yields $2$. $\square$

**Corollary.** *The set $U = \{v : \hat{c}_n(50, v) = 1\}$ is precisely the set of sums achieved by exactly one 50-element subset.*

## Editorial
*Remark.** The reverse iteration over $j$ and $s$ in step 3 ensures that each element $a_i$ is used at most once per subset (the standard 0/1 knapsack trick). We return sum of all s in [S_min, S_max] with dp[k][s] == 1.

## Pseudocode

```text
Input: S = [1, 4, 9, ..., 10000], n = 100, k = 50
Output: sum of all v with exactly one k-element subset of S summing to v
Compute S_max = 295425
Initialize dp[0..k][0..S_max] = 0; set dp[0][0] = 1
For i = 1 to n:
Return sum of all s in [S_min, S_max] with dp[k][s] == 1
```

## Complexity Analysis

- **Time:** The triple loop executes $O(n \cdot k \cdot S_{\max})$ iterations. With $n = 100$, $k = 50$, and $S_{\max} = 295425$, this gives $\leq 1.48 \times 10^9$ constant-time operations (addition, comparison, and clamp).
- **Space:** The DP table has $(k+1)(S_{\max}+1) \approx 1.51 \times 10^7$ entries. Each entry is in $\{0, 1, 2\}$, representable in 2 bits; using one byte per entry requires approximately 15 MB.

## Answer

$$\boxed{115039000}$$

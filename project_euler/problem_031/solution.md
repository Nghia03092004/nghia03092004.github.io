# Problem 31: Coin Sums

## Problem Statement

In England the currency is made up of pound and pence. There are eight coins in general circulation:

> 1p, 2p, 5p, 10p, 20p, 50p, 100p (£1), and 200p (£2)

How many different ways can £2 (200p) be made using any number of these coins?

## Mathematical Development

**Definition 1.** Let $S = \{c_1, c_2, \ldots, c_k\}$ be a finite set of positive integers (coin denominations). A *representation* of a non-negative integer $n$ over $S$ is a tuple $(j_1, j_2, \ldots, j_k) \in \mathbb{Z}_{\ge 0}^k$ satisfying $\sum_{i=1}^{k} j_i c_i = n$. The *change-making function* $p(n; S)$ counts the number of such representations.

**Theorem 1 (Generating function).** *The change-making function satisfies*
$$p(n; S) = [x^n] \prod_{i=1}^{k} \frac{1}{1 - x^{c_i}},$$
*where $[x^n]$ denotes extraction of the coefficient of $x^n$.*

**Proof.** Each factor admits the geometric series expansion $\frac{1}{1 - x^{c_i}} = \sum_{j=0}^{\infty} x^{j c_i}$, valid for $|x| < 1$. The Cauchy product of $k$ such series yields
$$\prod_{i=1}^{k} \frac{1}{1 - x^{c_i}} = \sum_{(j_1, \ldots, j_k) \in \mathbb{Z}_{\ge 0}^k} x^{j_1 c_1 + j_2 c_2 + \cdots + j_k c_k}.$$
The coefficient of $x^n$ in this product equals the cardinality of $\{(j_1, \ldots, j_k) \in \mathbb{Z}_{\ge 0}^k : \sum_{i=1}^{k} j_i c_i = n\}$, which is $p(n; S)$ by Definition 1. $\square$

**Lemma 1 (Recurrence).** *Define $f(i, m)$ as the number of representations of $m$ using only denominations $\{c_1, \ldots, c_i\}$. Then:*
$$f(i, m) = \begin{cases} 1 & \text{if } m = 0, \\ 0 & \text{if } m > 0 \text{ and } i = 0, \\ f(i-1, m) & \text{if } 0 < m < c_i, \\ f(i-1, m) + f(i, m - c_i) & \text{if } m \ge c_i. \end{cases}$$

**Proof.** The case $m = 0$ admits exactly the empty sum. The case $i = 0$, $m > 0$ has no coins available. For $m \ge c_i$, the representations of $m$ over $\{c_1, \ldots, c_i\}$ partition into two disjoint classes:

1. Representations with $j_i = 0$: these are precisely the representations of $m$ over $\{c_1, \ldots, c_{i-1}\}$, counted by $f(i-1, m)$.
2. Representations with $j_i \ge 1$: subtracting one copy of $c_i$ gives a bijection with representations of $m - c_i$ over $\{c_1, \ldots, c_i\}$, counted by $f(i, m - c_i)$.

These classes are exhaustive and mutually exclusive, establishing the recurrence. The case $0 < m < c_i$ forces $j_i = 0$, collapsing to $f(i-1, m)$. $\square$

**Theorem 2 (Space-optimal computation).** *The function $f(k, n)$ can be computed in $O(kn)$ time and $O(n)$ space using a one-dimensional array $\mathrm{dp}[0..n]$.*

**Proof.** Initialize $\mathrm{dp}[0] = 1$ and $\mathrm{dp}[m] = 0$ for $1 \le m \le n$. This encodes $f(0, m)$ for all $m$.

**Claim.** After processing coin $c_i$ (for $i = 1, \ldots, k$) via the update $\mathrm{dp}[j] \mathrel{+}= \mathrm{dp}[j - c_i]$ for $j = c_i, c_i + 1, \ldots, n$ (in increasing order), we have $\mathrm{dp}[m] = f(i, m)$ for all $0 \le m \le n$.

*Proof of claim by induction on $i$.* The base case $i = 0$ holds by initialization. Suppose $\mathrm{dp}[m] = f(i-1, m)$ for all $m$ at the start of iteration $i$. Consider the update for a given $j \ge c_i$:
- Before $j$ is processed, $\mathrm{dp}[j] = f(i-1, j)$ (not yet updated in this iteration).
- Since $j - c_i < j$ and we process in increasing order, $\mathrm{dp}[j - c_i]$ has already been updated to $f(i, j - c_i)$.
- After the update: $\mathrm{dp}[j] = f(i-1, j) + f(i, j - c_i) = f(i, j)$ by Lemma 1.

For $j < c_i$, no update occurs, so $\mathrm{dp}[j] = f(i-1, j) = f(i, j)$ (again by Lemma 1). This completes the inductive step.

After all $k$ iterations, $\mathrm{dp}[n] = f(k, n) = p(n; S)$. The outer loop runs $k$ times, the inner loop at most $n$ times each, giving $O(kn)$ time. The array has $n + 1$ entries, giving $O(n)$ space. $\square$

## Algorithm

We use one-dimensional dynamic programming over the available coin denominations. The array `dp[t]` stores the number of ways to form the total $t$, and we process the coin values in increasing order so that each unordered combination is counted exactly once rather than once per permutation. For each coin, we traverse all reachable totals from that coin value up to the target and add the number of ways obtained by appending the current coin.

## Pseudocode

```text
Algorithm: Coin Change Count by One-dimensional Dynamic Programming
Require: A target sum n ≥ 0 and an ordered list of coin denominations C = (c_1, c_2, ..., c_m).
Ensure: The number of unordered representations of n using coins from C.
1: Initialize an array dp on {0, 1, ..., n} with dp[0] ← 1 and dp[t] ← 0 for t > 0.
2: For each coin denomination c in C do:
3:     For each total t with c ≤ t ≤ n, update dp[t] ← dp[t] + dp[t - c].
4: Return dp[n].
```

## Complexity Analysis

**Proposition.** *For $S = \{1, 2, 5, 10, 20, 50, 100, 200\}$ and $n = 200$:*
- *Time:* $\Theta(kn) = \Theta(8 \cdot 200) = \Theta(1600)$ *arithmetic operations.*
- *Space:* $\Theta(n) = \Theta(201)$ *integers.*

**Proof.** Follows directly from Theorem 2 with $k = 8$ and $n = 200$. Each arithmetic operation (addition, array access) is $O(1)$ since all intermediate values fit in a machine word ($f(8, 200) = 73682 < 2^{31}$). $\square$

## Answer

$$\boxed{73682}$$

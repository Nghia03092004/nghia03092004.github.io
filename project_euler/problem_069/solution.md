# Problem 69: Totient Maximum

## Problem Statement

Euler's totient function, $\varphi(n)$, counts the number of integers $k$ with $1 \le k \le n$ that are relatively prime to $n$. Find the value of $n \le 1{,}000{,}000$ for which $n / \varphi(n)$ is a maximum.

## Mathematical Analysis

### Theorem 1 (Ratio Formula)

*For $n = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$, the ratio $n/\varphi(n)$ depends only on the set of distinct prime divisors of $n$:*

$$\frac{n}{\varphi(n)} = \prod_{p \mid n} \frac{p}{p-1}.$$

**Proof.** By Euler's product formula, $\varphi(n) = n \prod_{p \mid n}(1 - 1/p)$. Dividing $n$ by $\varphi(n)$:

$$\frac{n}{\varphi(n)} = \frac{1}{\prod_{p \mid n}(1 - 1/p)} = \prod_{p \mid n} \frac{p}{p-1}.$$

The exponents $a_i$ do not appear in this expression, so the ratio depends only on the set $\{p_1, \ldots, p_k\}$. $\square$

### Theorem 2 (Monotonicity in Prime Factors)

*Let $P = \{p_1, \ldots, p_k\}$ be a set of distinct primes. Then:*
1. *Adding any prime $q \notin P$ strictly increases $\prod_{p \in P} \frac{p}{p-1}$ by the factor $\frac{q}{q-1} > 1$.*
2. *The factor $\frac{p}{p-1}$ is a strictly decreasing function of $p$, so smaller primes contribute larger multiplicative gains.*

**Proof.** (1) is immediate since $q/(q-1) > 1$ for all primes $q \geq 2$. For (2), observe that $p/(p-1) = 1 + 1/(p-1)$, which is strictly decreasing in $p$. $\square$

### Theorem 3 (Primorial Optimality)

*Among all positive integers $n \leq N$, the ratio $n/\varphi(n)$ is maximized when $n$ is the largest primorial not exceeding $N$. That is, $n^* = p_1 p_2 \cdots p_k$ where $p_1 < p_2 < \cdots$ are the primes in increasing order and $k$ is the largest index such that $p_1 \cdots p_k \leq N$.*

**Proof.** We establish optimality by a dominance argument.

**Claim 1:** *The optimizer $n^*$ is squarefree.* Suppose $n$ is not squarefree, so $n = p^2 m$ for some prime $p$ with $p \nmid m$. Then $n' = pm$ has $n' < n$ and $n'/\varphi(n') = n/\varphi(n)$ (by Theorem 1). Since $n' < n \leq N$, we can "free" a factor of $p$ in our budget without changing the ratio. Using this budget to include a new prime $q$ not dividing $n'$ (if one exists with $n'q \leq N$) strictly increases the ratio by Theorem 2(1). If no such $q$ exists, $n'$ achieves the same ratio with a smaller value, but adding back factors cannot help. In either case, we can assume $n^*$ is squarefree.

**Claim 2:** *The optimizer uses consecutive smallest primes.* Let $n = p_{j_1} \cdots p_{j_k}$ be squarefree with $j_1 < \cdots < j_k$. If some $p_{j_i}$ is not the $i$-th smallest prime (i.e., $j_i > i$), we can replace $p_{j_i}$ by $p_i$ (a smaller prime). This does not increase $n$ (since $p_i < p_{j_i}$), and by Theorem 2(2), the factor $p_i/(p_i - 1) > p_{j_i}/(p_{j_i} - 1)$ strictly increases the ratio. Repeating this argument yields $n^* = p_1 p_2 \cdots p_k$.

**Claim 3:** *$k$ is maximized subject to $p_1 \cdots p_k \leq N$.* By Theorem 2(1), adding an additional prime $p_{k+1}$ strictly increases the ratio, so we include as many consecutive primes as the bound $N$ allows. $\square$

### Computation

| Primes included | $n = \prod p_i$ | $n/\varphi(n)$ |
|---|---|---|
| $\{2\}$ | 2 | 2.000 |
| $\{2,3\}$ | 6 | 3.000 |
| $\{2,3,5\}$ | 30 | 3.750 |
| $\{2,3,5,7\}$ | 210 | 4.375 |
| $\{2,3,5,7,11\}$ | 2310 | 4.813 |
| $\{2,3,5,7,11,13\}$ | 30030 | 4.991 |
| $\{2,3,5,7,11,13,17\}$ | 510510 | 5.214 |
| $\{2,3,5,7,11,13,17,19\}$ | 9699690 | $> 10^6$ |

Since $510510 \le 1{,}000{,}000 < 9{,}699{,}690$, the answer is $n^* = 2 \times 3 \times 5 \times 7 \times 11 \times 13 \times 17 = 510510$.

## Editorial

The maximizing value is the largest primorial not exceeding the bound. We therefore multiply consecutive primes in increasing order, beginning with $2$, and stop as soon as the next prime would push the product past $1{,}000{,}000$. The product accumulated just before that overshoot is the desired value of $n$.

## Pseudocode

```text
Initialize the running product to 1.

Traverse the primes in increasing order:
    if multiplying by the next prime would exceed 1,000,000, stop the process
    otherwise incorporate that prime into the running product

Return the final product.
```

## Complexity

- **Time:** $O(\pi^{-1}(N))$ where $\pi^{-1}$ denotes the primorial index -- effectively $O(\log \log N)$ multiplications.
- **Space:** $O(1)$.

## Answer

$$\boxed{510510}$$

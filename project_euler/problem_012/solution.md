# Problem 12: Highly Divisible Triangular Number

## Problem Statement

The $n$-th triangular number is $T_n = \frac{n(n+1)}{2}$. Find the smallest $T_n$ with $\tau(T_n) > 500$, where $\tau(m)$ denotes the number of positive divisors of $m$.

## Definitions and Notation

**Definition 1.** For $m \in \mathbb{Z}^+$ with prime factorization $m = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$, the *divisor function* is $\tau(m) = \#\{d \in \mathbb{Z}^+ : d \mid m\}$.

**Definition 2.** The *$n$-th triangular number* is $T_n = \sum_{i=1}^{n} i = \frac{n(n+1)}{2}$.

## Theorems

**Theorem 1 (Divisor count formula).** If $m = p_1^{a_1} \cdots p_k^{a_k}$, then $\tau(m) = \prod_{i=1}^{k}(a_i + 1)$.

*Proof.* A positive divisor of $m$ has the form $\prod_{i=1}^{k} p_i^{b_i}$ where $0 \le b_i \le a_i$. Each exponent $b_i$ admits $a_i + 1$ independent choices, yielding $\prod_{i=1}^{k}(a_i + 1)$ divisors in total. $\square$

**Theorem 2 (Multiplicativity of $\tau$).** The function $\tau$ is multiplicative: if $\gcd(a,b) = 1$, then $\tau(ab) = \tau(a)\tau(b)$.

*Proof.* Write $a = \prod_i p_i^{\alpha_i}$ and $b = \prod_j q_j^{\beta_j}$. Since $\gcd(a,b) = 1$, the sets $\{p_i\}$ and $\{q_j\}$ are disjoint. Hence $ab = \prod_i p_i^{\alpha_i} \cdot \prod_j q_j^{\beta_j}$ is in canonical form, and

$$\tau(ab) = \prod_i (\alpha_i + 1) \cdot \prod_j (\beta_j + 1) = \tau(a)\,\tau(b). \quad \square$$

**Lemma 1 (Coprimality of consecutive integers).** For all $n \ge 1$, $\gcd(n, n+1) = 1$.

*Proof.* If $d \mid n$ and $d \mid (n+1)$, then $d \mid (n+1) - n = 1$, so $d = 1$. $\square$

**Theorem 3 (Coprime factorization of $T_n$).** For all $n \ge 1$:

$$\tau(T_n) = \begin{cases} \tau\!\left(\dfrac{n}{2}\right) \cdot \tau(n+1) & \text{if } 2 \mid n, \\[8pt] \tau(n) \cdot \tau\!\left(\dfrac{n+1}{2}\right) & \text{if } 2 \nmid n. \end{cases}$$

*Proof.* We have $T_n = \frac{n(n+1)}{2}$. Exactly one of $n, n+1$ is even. Write the factorization as $T_n = A \cdot B$ where the factor of $2$ is absorbed into the even term:

- If $2 \mid n$: set $A = n/2$, $B = n+1$.
- If $2 \nmid n$: set $A = n$, $B = (n+1)/2$.

We must verify $\gcd(A,B) = 1$ in each case. Consider the case $2 \mid n$. Every prime $p$ dividing $A = n/2$ also divides $n$. By Lemma 1, $\gcd(n, n+1) = 1$, so $p \nmid n+1 = B$. Hence $\gcd(A,B) = 1$. The odd case is symmetric. Applying Theorem 2 yields the result. $\square$

**Remark.** This decomposition is the key algorithmic insight: instead of factoring $T_n$ (which can be as large as $\sim n^2/2$), we factor two numbers of size $\sim n/2$ and $\sim n$ separately, reducing the trial division cost.

## Algorithm

```
FIRST-TRIANGLE-EXCEEDING-K-DIVISORS(K):
    function tau(m):
        if m = 0: return 0
        count <- 1
        for p <- 2, 3, 4, ... while p^2 <= m:
            e <- 0
            while p | m:
                m <- m / p
                e <- e + 1
            count <- count * (e + 1)
        if m > 1:
            count <- count * 2
        return count

    for n <- 1, 2, 3, ...:
        if 2 | n:
            d <- tau(n / 2) * tau(n + 1)
        else:
            d <- tau(n) * tau((n + 1) / 2)
        if d > K:
            return n * (n + 1) / 2
```

## Complexity Analysis

**Proposition.** The algorithm terminates in $O(N\sqrt{N})$ time and $O(1)$ auxiliary space, where $N$ is the index satisfying $\tau(T_N) > K$.

*Proof.* For each $n$ from $1$ to $N$, we compute $\tau$ of two numbers, each at most $n + 1$. Trial division of an integer $m$ requires $O(\sqrt{m})$ divisions. Hence the cost per iteration is $O(\sqrt{n})$, and the total cost is

$$\sum_{n=1}^{N} O(\sqrt{n}) = O\!\left(\int_1^N \sqrt{x}\, dx\right) = O(N^{3/2}) = O(N\sqrt{N}).$$

No auxiliary data structure is needed beyond $O(1)$ variables. $\square$

**Verification.** At $n = 12375$: $T_{12375} = 76\,576\,500 = 2^2 \cdot 3^2 \cdot 5^3 \cdot 7 \cdot 11 \cdot 13 \cdot 17$, giving $\tau = 3 \cdot 3 \cdot 4 \cdot 2 \cdot 2 \cdot 2 \cdot 2 = 576 > 500$.

## Answer

$$\boxed{76576500}$$

# Problem 297: Zeckendorf Representation

## Problem Statement

Every positive integer has a unique **Zeckendorf representation** as a sum of non-consecutive Fibonacci numbers. Let $z(n)$ denote the number of terms in this representation. For example, $z(5) = 1$, $z(14) = 2$, $z(100) = 3$ (since $100 = 3 + 8 + 89$).

Given $\sum_{0 < n < 10^6} z(n) = 7{,}894{,}453$, find $\sum_{0 < n < 10^{17}} z(n)$.

## Mathematical Foundation

We use the Fibonacci sequence $F_1 = 1, F_2 = 2, F_3 = 3, F_4 = 5, F_5 = 8, \ldots$ defined by $F_k = F_{k-1} + F_{k-2}$ with $F_1 = 1, F_2 = 2$.

**Theorem 1 (Zeckendorf's Theorem).** *Every positive integer $n$ has a unique representation $n = \sum_{i \in S} F_i$ where $S \subset \mathbb{N}$ and $S$ contains no two consecutive indices.*

**Proof.** (Existence) By strong induction. For $n = 1 = F_1$, the result holds. For $n > 1$, let $F_k$ be the largest Fibonacci number $\leq n$. Then $0 \leq n - F_k < F_{k-1}$ (since $F_{k+1} = F_k + F_{k-1} > n$). By induction, $n - F_k$ has a Zeckendorf representation using Fibonacci numbers $\leq F_{k-2}$ (no $F_{k-1}$ appears because $n - F_k < F_{k-1}$), so the representation is non-consecutive.

(Uniqueness) Suppose $n = \sum_{i \in S} F_i = \sum_{j \in T} F_j$ with $S \neq T$. Let $k$ be the largest index in $S \triangle T$. WLOG $k \in S \setminus T$. Then $\sum_{j \in T, j > k} F_j = 0$ (since $k$ is the largest in the symmetric difference and elements $> k$ match). But $\sum_{j \in T, j \leq k-2} F_j \leq F_{k-2} + F_{k-4} + \cdots < F_k$ (by a standard Fibonacci identity), a contradiction since the $T$-sum must account for $F_k$. $\square$

**Theorem 2 (Recurrence for $S(N)$ at Fibonacci Boundaries).** *Define $S(N) = \sum_{n=1}^{N-1} z(n)$. Then*

$$S(F_{k+1}) = S(F_k) + S(F_{k-1}) + F_{k-1}.$$

**Proof.** The integers in $[1, F_{k+1})$ split into $[1, F_k)$ and $[F_k, F_{k+1})$. The first group contributes $S(F_k)$.

For the second group: each integer $n \in [F_k, F_{k+1})$ can be written uniquely as $n = F_k + m$ where $0 \leq m < F_{k-1}$. Since $n \geq F_k$ and $n < F_{k+1} = F_k + F_{k-1}$, we have $m = n - F_k \in [0, F_{k-1})$. The Zeckendorf representation of $n$ is $F_k$ plus the Zeckendorf representation of $m$ (which uses only $F_1, \ldots, F_{k-2}$ since $m < F_{k-1}$, ensuring no consecutive indices with $F_k$).

Thus $z(n) = 1 + z(m)$ for $m > 0$, and $z(F_k) = 1$ (when $m = 0$). The total contribution is:

$$\sum_{m=0}^{F_{k-1}-1} (1 + z(m)) = F_{k-1} + \sum_{m=1}^{F_{k-1}-1} z(m) = F_{k-1} + S(F_{k-1}).$$

Hence $S(F_{k+1}) = S(F_k) + F_{k-1} + S(F_{k-1})$. $\square$

**Theorem 3 (General Recursion).** *For $F_k \leq N < F_{k+1}$:*

$$S(N) = S(F_k) + (N - F_k) + S(N - F_k).$$

**Proof.** Split $[1, N)$ into $[1, F_k)$ and $[F_k, N)$. The first contributes $S(F_k)$. For the second, each $n \in [F_k, N)$ has $z(n) = 1 + z(n - F_k)$ as above. There are $N - F_k$ such integers, each contributing the extra $1$ for the $F_k$ term, plus $z(n - F_k)$. The residuals $n - F_k$ range over $[0, N - F_k)$, contributing $S(N - F_k)$. $\square$

**Lemma 1 (Recursion Depth).** *The recursion in Theorem 3 terminates in $O(\log_\varphi N)$ steps, where $\varphi = (1+\sqrt{5})/2$.*

**Proof.** At each step, $N$ is replaced by $N - F_k$ where $F_k \geq N/\varphi$ (since $F_k > N - F_{k-1} \geq N - N/\varphi = N/\varphi^2$, and actually $F_k \geq N/\varphi$ follows from $F_{k+1} > N$ and $F_{k+1}/F_k \to \varphi$). Thus $N - F_k \leq N(1 - 1/\varphi) = N/\varphi^2$, and after $j$ steps, $N$ is reduced to at most $N/\varphi^{2j}$. The recursion terminates when $N < F_2 = 2$, requiring $O(\log_\varphi N)$ steps. $\square$

## Editorial
Fibonacci: F[0]=1, F[1]=2, F[2]=3, F[3]=5, ... S(N) = sum of z(n) for n in [1, N). At Fibonacci boundaries: S(F[k+1]) = S(F[k]) + S(F[k-1]) + F[k-1] General recursion (F[k] <= N < F[k+1]): S(N) = S(F[k]) + (N - F[k]) + S(N - F[k]). We precompute Fibonacci numbers and S(F_k). Finally, recursive computation of S(N).

## Pseudocode

```text
Precompute Fibonacci numbers and S(F_k)
Recursive computation of S(N)
```

## Complexity Analysis

- **Time:** $O(\log N)$ for precomputing $\sim \log_\varphi N \approx 83$ Fibonacci numbers and their $S$-values, plus $O(\log N)$ recursive steps.
- **Space:** $O(\log N)$ for storing Fibonacci numbers and $S(F_k)$ values (about 83 entries for $N = 10^{17}$).

## Answer

$$\boxed{2252639041804718029}$$

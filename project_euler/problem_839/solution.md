# Problem 839: Beans in Bowls

## Problem Statement

Count the number of solutions to $x_1 + x_2 + \cdots + x_k = n$ with $0 \le x_i \le m$ for all $i$. Denote this $B(n, k, m)$. Compute the answer modulo a prime $p$.

## Mathematical Foundation

**Theorem 1 (Stars and Bars).** *The number of solutions to $x_1 + \cdots + x_k = n$ with $x_i \ge 0$ (no upper bound) is $\binom{n+k-1}{k-1}$.*

**Proof.** Represent the $n$ units as stars and the $k-1$ dividers between groups as bars. Each arrangement of $n$ stars and $k-1$ bars in a row of $n+k-1$ positions corresponds bijectively to a solution $(x_1, \ldots, x_k)$, where $x_i$ is the number of stars between the $(i-1)$-th and $i$-th bar. The number of such arrangements is $\binom{n+k-1}{k-1}$. $\square$

**Theorem 2 (Bounded Compositions via Inclusion-Exclusion).** *The number of solutions to $x_1 + \cdots + x_k = n$ with $0 \le x_i \le m$ for all $i$ is*

$$B(n, k, m) = \sum_{j=0}^{\lfloor n/(m+1) \rfloor} (-1)^j \binom{k}{j} \binom{n - j(m+1) + k - 1}{k - 1}.$$

**Proof.** Let $A_i = \{(x_1, \ldots, x_k) : x_1 + \cdots + x_k = n,\; x_j \ge 0 \;\forall j,\; x_i \ge m+1\}$ for $i = 1, \ldots, k$. We seek $|\overline{A_1 \cup \cdots \cup A_k}|$, the number of solutions with no variable exceeding $m$. By inclusion-exclusion:

$$B(n,k,m) = \sum_{j=0}^{k} (-1)^j \sum_{|T|=j} |A_{i_1} \cap \cdots \cap A_{i_j}|.$$

For a set $T$ of $j$ indices, $\bigcap_{i \in T} A_i$ requires $x_i \ge m+1$ for all $i \in T$. Substituting $y_i = x_i - (m+1)$ for $i \in T$ (so $y_i \ge 0$) reduces the sum to $n - j(m+1)$. By Theorem 1, the count is $\binom{n - j(m+1) + k - 1}{k-1}$ when $n - j(m+1) \ge 0$, and 0 otherwise. Since there are $\binom{k}{j}$ choices of $T$, the formula follows. The sum terminates at $j = \lfloor n/(m+1) \rfloor$ because the binomial coefficient vanishes when the top argument is negative. $\square$

**Lemma 1 (Generating Function).** *The generating function for a single bounded variable is $g(x) = \frac{1 - x^{m+1}}{1 - x}$. The answer is the coefficient $[x^n]\, g(x)^k$.*

**Proof.** We have $g(x) = 1 + x + x^2 + \cdots + x^m$ (geometric sum). The coefficient of $x^n$ in $g(x)^k$ counts the number of ways to choose $x_1, \ldots, x_k \in \{0, 1, \ldots, m\}$ summing to $n$. Expanding via the binomial theorem:

$$g(x)^k = (1 - x^{m+1})^k (1-x)^{-k} = \left(\sum_{j=0}^{k} (-1)^j \binom{k}{j} x^{j(m+1)}\right) \left(\sum_{r=0}^{\infty} \binom{r+k-1}{k-1} x^r\right).$$

Extracting $[x^n]$ by convolving yields the inclusion-exclusion formula of Theorem 2. $\square$

**Lemma 2 (DP Recurrence).** *Define $f(i, s) =$ number of ways to assign values to bowls $1, \ldots, i$ totaling $s$. Then:*

$$f(i, s) = \sum_{x=0}^{\min(m, s)} f(i-1, s-x), \quad f(0, 0) = 1, \quad f(0, s) = 0 \text{ for } s > 0.$$

*With prefix-sum optimization, each row $f(i, \cdot)$ is computed from $f(i-1, \cdot)$ in $O(n)$ time.*

**Proof.** The recurrence follows by conditioning on the value of $x_i$. For the prefix-sum optimization, note that $f(i, s) = \sum_{x=0}^{\min(m,s)} f(i-1, s-x) = F(s) - F(s - m - 1)$ where $F(s) = \sum_{t=0}^{s} f(i-1, t)$ is the prefix sum. Computing $F$ takes $O(n)$, and each $f(i,s)$ is then $O(1)$. $\square$

## Editorial
Count distributions of n beans into k bowls, each with capacity m. B(n,k,m) = sum_{j=0}^{floor(n/(m+1))} (-1)^j * C(k,j) * C(n-j(m+1)+k-1, k-1). We method: Inclusion-exclusion with modular arithmetic. Finally, precompute factorials and inverse factorials mod p.

## Pseudocode

```text
Method: Inclusion-exclusion with modular arithmetic
Precompute factorials and inverse factorials mod p
```

## Complexity Analysis

- **Time:** $O(n + k)$ for factorial precomputation; $O(\min(k, n/(m+1)))$ for the inclusion-exclusion sum. Total: $O(n + k)$.
- **Space:** $O(n + k)$ for factorial tables.

## Answer

$$\boxed{150893234438294408}$$

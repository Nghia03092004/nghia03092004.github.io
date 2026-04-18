# Problem 743: Window into a Matrix

## Problem Statement

Consider a $2 \times n$ binary matrix where every $2 \times k$ contiguous submatrix (window) has entry sum exactly $k$. Let $A(k, n)$ count the number of such matrices.

Given: $A(3, 9) = 560$, $A(4, 20) = 1{,}060{,}870$.

Find $A(10^8, 10^{16}) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Theorem 1 (Column-Sum Periodicity).** *Let $c_j$ denote the column sum of column $j$ (so $c_j \in \{0, 1, 2\}$). The window constraint $\sum_{i=j}^{j+k-1} c_i = k$ for all valid $j$ implies that the column-sum sequence is periodic with period dividing $k$, and each period of $k$ consecutive sums totals $k$.*

**Proof.** For any $j$ with $1 \le j \le n - k$, we have:
$$\sum_{i=j}^{j+k-1} c_i = k \quad \text{and} \quad \sum_{i=j+1}^{j+k} c_i = k.$$
Subtracting: $c_{j+k} - c_j = 0$, so $c_{j+k} = c_j$ for all valid $j$. Hence the sequence $(c_j)$ is periodic with period dividing $k$. Within one period, $\sum_{i=1}^{k} c_i = k$. $\square$

**Theorem 2 (Decomposition into Column Patterns).** *A valid $2 \times n$ matrix is determined by:*
1. *A column-sum pattern $(c_1, \ldots, c_k)$ with $c_i \in \{0, 1, 2\}$, $\sum c_i = k$, periodic with period dividing $k$.*
2. *For each column with $c_i = 1$: a choice of which of the two cells is $1$ (2 choices).*
3. *Columns with $c_i = 0$ or $c_i = 2$ are fully determined.*

**Proof.** The window constraint is equivalent to column-sum periodicity by Theorem 1. Given the column sums, each column with sum $0$ is $(0,0)^T$, each with sum $2$ is $(1,1)^T$, and each with sum $1$ has exactly $2$ arrangements: $(1,0)^T$ or $(0,1)^T$. The window constraint does not impose further coupling between the specific cell values beyond the column sums (since the constraint is purely on sums). $\square$

**Lemma 1 (Counting Formula).** *Let $m$ denote the number of columns with $c_i = 1$ in one period. The total count of matrices for a given column-sum pattern with $n$ columns is $2^{m \cdot \lfloor n/k \rfloor + m'}$ where $m'$ accounts for partial periods. For $k \mid n$, this simplifies to $2^{m \cdot (n/k)}$.*

**Proof.** Each column with sum $1$ contributes a factor of $2$ independently. In $n/k$ complete periods, there are $m \cdot (n/k)$ such columns. $\square$

**Theorem 3 (Closed-Form for $A(k, n)$).** *When $k \mid n$:*

$$A(k, n) = \sum_{\substack{(c_1, \ldots, c_k) \\ c_i \in \{0,1,2\}, \sum c_i = k}} 2^{|\{i : c_i = 1\}| \cdot n/k}$$

*The inner sum counts compositions of $k$ into parts from $\{0, 1, 2\}$ of length $k$. Let $m$ denote the number of $1$'s among the $c_i$. If there are $m$ ones, then there are $k - m$ positions for $0$'s and $2$'s, with $(k - m)/2$ each (requiring $k - m$ even). The number of such patterns is $\binom{k}{m} \binom{k - m}{(k-m)/2}$. Thus:*

$$A(k, n) = \sum_{\substack{m = 0 \\ k - m \text{ even}}}^{k} \binom{k}{m} \binom{k-m}{(k-m)/2} \cdot 2^{mn/k}$$

**Proof.** We must choose which $m$ of the $k$ positions have $c_i = 1$, then distribute the remaining $k - m$ sum among $k - m$ positions using only $0$'s and $2$'s: this requires $(k-m)/2$ twos and $(k-m)/2$ zeros, giving $\binom{k-m}{(k-m)/2}$ arrangements. Each such pattern generates $2^{mn/k}$ matrices over $n/k$ periods. Summing over valid $m$ yields the formula. $\square$

**Lemma 2 (Modular Evaluation).** *For $k = 10^8$ and $n = 10^{16}$ (so $n/k = 10^8$), the sum involves binomial coefficients $\bmod (10^9+7)$ and modular exponentials $2^{m \cdot 10^8}$, computable via Lucas' theorem and fast exponentiation.*

**Proof.** Since $10^9 + 7$ is prime and $k = 10^8 < 10^9 + 7$, all binomial coefficients $\binom{k}{m}$ can be computed using precomputed factorials modulo $p$. The powers $2^{mn/k}$ are computed via fast modular exponentiation in $O(\log(mn/k))$ time. $\square$

## Editorial
A $2 \times n$ binary matrix where every $2 \times k$ contiguous window sums to exactly $k$. $A(k, n)$ counts such matrices. Given $A(3,9) = 560$ and $A(4,20) = 1060870$. Find $A(10^8, 10^{16}) \bmod. We precompute factorials mod p up to k. We then binom(k, m) * binom(k-m, half). Finally, 2^(m * ratio).

## Pseudocode

```text
Precompute factorials mod p up to k
binom(k, m) * binom(k-m, half)
2^(m * ratio)
```

## Complexity Analysis

- **Time:** $O(k)$ for factorial precomputation and the main summation loop. For $k = 10^8$, this requires approximately $10^8$ operations.
- **Space:** $O(k)$ for factorial and inverse factorial arrays.

## Answer

$$\boxed{259158998}$$

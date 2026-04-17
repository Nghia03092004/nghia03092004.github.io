# Problem 1: Multiples of 3 and 5

## Problem Statement

Find the sum of all the multiples of 3 or 5 below 1000. That is, compute

$$S = \sum_{\substack{1 \le k < 1000 \\ 3 \mid k \;\text{or}\; 5 \mid k}} k.$$

## Mathematical Development

### Preliminaries

**Definition 1.** For a positive integer $m$ and a positive integer $N$, define the set of positive multiples of $m$ strictly below $N$ as

$$A_m(N) = \{k \in \mathbb{Z}^+ : m \mid k \text{ and } k < N\} = \{m, 2m, 3m, \ldots, pm\}$$

where $p = \lfloor (N-1)/m \rfloor$.

**Definition 2.** For a finite set $X \subset \mathbb{Z}$, define the sum function $\sigma(X) = \sum_{x \in X} x$.

### The Arithmetic Series Identity

**Theorem 1 (Sum of the First $n$ Natural Numbers).** For every positive integer $n$,

$$\sum_{k=1}^{n} k = \frac{n(n+1)}{2}.$$

**Proof.** We proceed by induction on $n$.

*Base case.* For $n = 1$: $\sum_{k=1}^{1} k = 1 = \frac{1 \cdot 2}{2}$. The identity holds.

*Inductive step.* Assume the identity holds for some $n \ge 1$. Then

$$\sum_{k=1}^{n+1} k = \left(\sum_{k=1}^{n} k\right) + (n+1) = \frac{n(n+1)}{2} + (n+1) = (n+1)\left(\frac{n}{2} + 1\right) = \frac{(n+1)(n+2)}{2}.$$

This is the identity with $n$ replaced by $n+1$. By the principle of mathematical induction, the identity holds for all $n \ge 1$. $\square$

**Corollary 1 (Sum of Multiples).** Let $m, N$ be positive integers with $m \ge 1$ and $N \ge 2$. Then

$$\sigma(A_m(N)) = m \cdot \frac{p(p+1)}{2}, \quad \text{where } p = \left\lfloor \frac{N-1}{m} \right\rfloor.$$

**Proof.** The elements of $A_m(N)$ are exactly $m, 2m, \ldots, pm$. Factoring:

$$\sigma(A_m(N)) = \sum_{j=1}^{p} jm = m \sum_{j=1}^{p} j = m \cdot \frac{p(p+1)}{2}$$

where the last equality follows from Theorem 1. $\square$

### Inclusion-Exclusion

**Theorem 2 (Inclusion-Exclusion Principle for Sums over Two Sets).** Let $A$ and $B$ be finite subsets of $\mathbb{Z}$. Then

$$\sigma(A \cup B) = \sigma(A) + \sigma(B) - \sigma(A \cap B).$$

**Proof.** Partition $A \cup B$ into three pairwise disjoint sets:

$$A \cup B = (A \setminus B) \;\sqcup\; (B \setminus A) \;\sqcup\; (A \cap B).$$

Since these are disjoint, additivity of summation gives

$$\sigma(A \cup B) = \sigma(A \setminus B) + \sigma(B \setminus A) + \sigma(A \cap B). \tag{1}$$

Similarly, $A = (A \setminus B) \sqcup (A \cap B)$, so $\sigma(A) = \sigma(A \setminus B) + \sigma(A \cap B)$, whence

$$\sigma(A \setminus B) = \sigma(A) - \sigma(A \cap B). \tag{2}$$

Likewise, $\sigma(B \setminus A) = \sigma(B) - \sigma(A \cap B)$. Substituting into (1):

$$\sigma(A \cup B) = [\sigma(A) - \sigma(A \cap B)] + [\sigma(B) - \sigma(A \cap B)] + \sigma(A \cap B) = \sigma(A) + \sigma(B) - \sigma(A \cap B). \quad \square$$

### Characterizing the Intersection

**Lemma 1 (Intersection of Multiples via LCM).** For positive integers $m_1, m_2$ and $N \ge 2$,

$$A_{m_1}(N) \cap A_{m_2}(N) = A_{\operatorname{lcm}(m_1, m_2)}(N).$$

**Proof.** Let $\ell = \operatorname{lcm}(m_1, m_2)$. We show set equality by double inclusion.

($\supseteq$) If $k \in A_\ell(N)$, then $\ell \mid k$ and $k < N$. Since $m_1 \mid \ell$ and $m_2 \mid \ell$, we have $m_1 \mid k$ and $m_2 \mid k$, so $k \in A_{m_1}(N) \cap A_{m_2}(N)$.

($\subseteq$) If $k \in A_{m_1}(N) \cap A_{m_2}(N)$, then $m_1 \mid k$ and $m_2 \mid k$ and $k < N$. By the universal property of the least common multiple, $m_1 \mid k$ and $m_2 \mid k$ implies $\ell \mid k$. Hence $k \in A_\ell(N)$. $\square$

**Corollary 2.** Since $\gcd(3, 5) = 1$, we have $\operatorname{lcm}(3, 5) = 3 \cdot 5 / \gcd(3,5) = 15$. Therefore $A_3(N) \cap A_5(N) = A_{15}(N)$.

### Main Result

**Theorem 3 (Closed-Form Solution).** The sum of all positive multiples of 3 or 5 below $N$ is

$$S(N) = \sigma(A_3(N) \cup A_5(N)) = 3 \cdot \frac{p_3(p_3+1)}{2} + 5 \cdot \frac{p_5(p_5+1)}{2} - 15 \cdot \frac{p_{15}(p_{15}+1)}{2}$$

where $p_m = \lfloor (N-1)/m \rfloor$ for $m \in \{3, 5, 15\}$.

**Proof.** Apply Theorem 2 with $A = A_3(N)$ and $B = A_5(N)$:

$$\sigma(A_3(N) \cup A_5(N)) = \sigma(A_3(N)) + \sigma(A_5(N)) - \sigma(A_3(N) \cap A_5(N)).$$

By Corollary 2, $A_3(N) \cap A_5(N) = A_{15}(N)$. Then Corollary 1 gives

$$\sigma(A_3(N)) = 3 \cdot \frac{p_3(p_3+1)}{2}, \qquad
\sigma(A_5(N)) = 5 \cdot \frac{p_5(p_5+1)}{2}, \qquad
\sigma(A_{15}(N)) = 15 \cdot \frac{p_{15}(p_{15}+1)}{2},$$

where $p_m = \lfloor (N-1)/m \rfloor$. Substituting these three identities yields the stated formula. $\square$

### Numerical Evaluation

**Proposition 1.** $S(1000) = 233168$.

**Proof.** We compute each component:

- $p_3 = \lfloor 999/3 \rfloor = 333$, so $\sigma(A_3) = 3 \cdot \frac{333 \cdot 334}{2} = 3 \cdot 55611 = 166833$.
- $p_5 = \lfloor 999/5 \rfloor = 199$, so $\sigma(A_5) = 5 \cdot \frac{199 \cdot 200}{2} = 5 \cdot 19900 = 99500$.
- $p_{15} = \lfloor 999/15 \rfloor = 66$, so $\sigma(A_{15}) = 15 \cdot \frac{66 \cdot 67}{2} = 15 \cdot 2211 = 33165$.

Therefore $S(1000) = 166833 + 99500 - 33165 = 233168$. $\square$

## Algorithm

We use the closed-form arithmetic-series formula rather than scanning every integer. The procedure counts how many multiples of 3, 5, and 15 lie below the limit, converts each count into the corresponding sum of multiples, and then applies inclusion-exclusion to subtract the overlap. This is sufficient because every qualifying integer is a multiple of 3 or 5, and the only double-counted terms are multiples of 15.

**Theorem 4 (Algorithm Correctness).** `SumMultiples(N)` returns $\sigma(A_3(N) \cup A_5(N))$ for all $N \ge 2$.

**Proof.** The algorithm computes exactly the three integers

$$p_3 = \left\lfloor \frac{N-1}{3} \right\rfloor, \qquad
p_5 = \left\lfloor \frac{N-1}{5} \right\rfloor, \qquad
p_{15} = \left\lfloor \frac{N-1}{15} \right\rfloor,$$

and then returns

$$3 \cdot \frac{p_3(p_3+1)}{2} + 5 \cdot \frac{p_5(p_5+1)}{2} - 15 \cdot \frac{p_{15}(p_{15}+1)}{2}.$$

By Theorem 3, this quantity is precisely $\sigma(A_3(N) \cup A_5(N))$. Therefore the algorithm is correct. $\square$

## Pseudocode

```text
Algorithm: Sum of Multiples Below a Bound
Require: An integer N ≥ 2.
Ensure: The sum S of all positive integers below N that are divisible by 3 or 5.
1: For each modulus m in {3, 5, 15}, compute p_m ← floor((N - 1) / m), the number of positive multiples of m below N.
2: For each such m, evaluate T_m ← m · p_m(p_m + 1) / 2.
3: Form S ← T_3 + T_5 - T_15 by inclusion-exclusion.
4: Return S.
```

## Complexity Analysis

**Theorem 5.** `SumMultiples(N)` runs in $O(1)$ time and $O(1)$ space.

**Proof.** The algorithm performs exactly 3 floor divisions, 3 additions, 6 multiplications, 3 divisions by 2, 1 addition, and 1 subtraction. This is a fixed number of arithmetic operations independent of $N$. The algorithm stores exactly 6 intermediate values ($p_3, p_5, p_{15}$ and the three partial sums), which is $O(1)$ space.

*Remark.* A brute-force approach testing each integer in $\{1, 2, \ldots, N-1\}$ for divisibility by 3 or 5 requires $\Theta(N)$ time and $O(1)$ space. The closed-form solution removes the scan entirely and reduces the running time to $\Theta(1)$. $\square$

## Answer

$$\boxed{233168}$$

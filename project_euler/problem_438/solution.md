# Problem 438: Integer Part of Polynomial Equation's Solutions

## Problem Statement

For an $n$-tuple of integers $t = (a_1, \ldots, a_n)$, let $(x_1, \ldots, x_n)$ be the real solutions of
$$x^n + a_1 x^{n-1} + a_2 x^{n-2} + \cdots + a_{n-1}x + a_n = 0,$$
sorted in increasing order. Consider only tuples where all $n$ roots are real and satisfy $\lfloor x_i \rfloor = i$ for $1 \leq i \leq n$. Define $S(t) = \sum_{i=1}^n |a_i|$. Find $\sum_t S(t)$ for $n = 7$.

## Mathematical Foundation

**Theorem 1 (Vieta's Formulas).** If $x_1, \ldots, x_n$ are the roots of $x^n + a_1 x^{n-1} + \cdots + a_n = 0$, then
$$a_k = (-1)^k e_k(x_1, \ldots, x_n),$$
where $e_k$ is the $k$-th elementary symmetric polynomial.

**Proof.** Expanding $\prod_{i=1}^n (x - x_i) = x^n - e_1 x^{n-1} + e_2 x^{n-2} - \cdots + (-1)^n e_n$ and matching coefficients with $x^n + a_1 x^{n-1} + \cdots + a_n$ yields $a_k = (-1)^k e_k$. $\square$

**Lemma 1 (Fractional Part Decomposition).** Write $x_i = i + f_i$ with $0 \leq f_i < 1$. The constraint $\lfloor x_i \rfloor = i$ is equivalent to $f_i \in [0, 1)$.

**Proof.** By definition of the floor function, $\lfloor x_i \rfloor = i$ iff $i \leq x_i < i + 1$, iff $0 \leq x_i - i < 1$. $\square$

**Theorem 2 (Integrality Constraints on Fractional Parts).** The coefficients $a_k$ are all integers if and only if $e_k(1 + f_1, 2 + f_2, \ldots, n + f_n) \in \mathbb{Z}$ for all $1 \leq k \leq n$.

**Proof.** By Vieta's formulas, $a_k = (-1)^k e_k(x_1, \ldots, x_n)$. Substituting $x_i = i + f_i$, $a_k \in \mathbb{Z}$ iff $e_k(1 + f_1, \ldots, n + f_n) \in \mathbb{Z}$. $\square$

**Lemma 2 (Sum Constraint).** The sum of fractional parts $\sum_{i=1}^n f_i$ must be a non-negative integer less than $n$.

**Proof.** $e_1 = \sum (i + f_i) = n(n+1)/2 + \sum f_i$. For $a_1 = -e_1 \in \mathbb{Z}$, we need $\sum f_i \in \mathbb{Z}$. Since $0 \leq f_i < 1$, we have $0 \leq \sum f_i < n$, so $\sum f_i \in \{0, 1, \ldots, n - 1\}$. $\square$

**Theorem 3 (Newton's Identity Reduction).** The elementary symmetric polynomials $e_k$ are determined by the power sums $p_k = \sum_{i=1}^n x_i^k$ via Newton's identities:
$$k \cdot e_k = \sum_{j=1}^{k} (-1)^{j-1} e_{k-j} \, p_j, \quad e_0 = 1.$$
Thus all $e_k \in \mathbb{Z}$ if and only if all $p_k \in \mathbb{Z}$ (by induction, since $k \cdot e_k$ is an integer combination of $e_0, \ldots, e_{k-1}$ and $p_1, \ldots, p_k$, and $\gcd(1, 2, \ldots, n)$ considerations are handled by the identity).

**Proof.** Newton's identities are a classical result following from logarithmic differentiation of $\prod(1 - x_i t)$ or from the formal identity $\sum_{k \geq 1} p_k t^k = -t \frac{d}{dt} \ln \prod_i (1 - x_i t)$. The equivalence of integer $e_k$ and integer $p_k$ follows by inductive application of the identities. $\square$

**Lemma 3 (Finite Search Space).** For fixed $n$, the number of valid $n$-tuples $(f_1, \ldots, f_n)$ is finite. The fractional parts are constrained by $n$ polynomial equations (the integrality conditions for $e_1, \ldots, e_n$) with $n$ unknowns in $[0, 1)^n$.

**Proof.** Each integrality condition $e_k \in \mathbb{Z}$ constrains $\sum f_i, \sum f_i f_j$, etc. to discrete values. The number of solutions of a system of $n$ polynomial equations in $[0, 1)^n$ with integer constraints is finite by the discreteness of $\mathbb{Z}$ and the bounded domain. $\square$

## Editorial
For n=7, find sum of S(t) = sum|a_i| over all valid n-tuples t=(a1,...,a7) where the polynomial x^7 + a1*x^6 + ... + a7 = 0 has all real roots x_i with floor(x_i) = i when sorted. Approach:. We enumerate valid fractional part tuples. We then recursive search over f_1, ..., f_n in [0, 1). Finally, verify all e_k are integers.

## Pseudocode

```text
Enumerate valid fractional part tuples
Recursive search over f_1, ..., f_n in [0, 1)
with constraint sum(f_i) = k and all e_j integer
Verify all e_k are integers
Determine valid f_idx values from integrality of partial e_k
Use Newton's identities for pruning:
p_j(partial) must have fractional part consistent with integer e_j
```

## Complexity Analysis

- **Time:** The search space is bounded by the integrality constraints. For each candidate tuple, verification takes $O(n^2)$ via Vieta's formulas. The total number of valid tuples is small (empirically manageable for $n = 7$).
- **Space:** $O(n)$ for the recursion stack and partial configuration.

## Answer

$$\boxed{2046409616809}$$

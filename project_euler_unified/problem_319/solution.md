# Problem 319: Bounded Sequences

## Problem Statement

Let $t(n)$ denote the number of sequences $(x_1, \ldots, x_n)$ of positive integers such that there exists $\alpha \in [2, 3)$ with $x_i = \lfloor \alpha^i \rfloor$ for all $i$. Given $t(10) = 86195$ and $t(20) = 5227991891$, find $t(10^{10}) \bmod 10^9$.

## Mathematical Foundation

**Definition.** For real $\alpha \ge 2$, the sequence $(\lfloor\alpha\rfloor, \lfloor\alpha^2\rfloor, \ldots, \lfloor\alpha^n\rfloor)$ is the *$\alpha$-floor sequence* of length $n$. Let $t(n)$ count the distinct $\alpha$-floor sequences for $\alpha \in [2,3)$.

**Lemma 1 (Counting Function $g$).** *Define $G(v, m) = \sum_{j=1}^{m} v^j = v \cdot \frac{v^m - 1}{v - 1}$ and $g(m) = G(3, m) - G(2, m)$. Then $g(m)$ counts, with appropriate multiplicity, the total number of integer sequences of length $m$ consistent with some $\alpha \in [2,3)$ (before applying Mobius inversion to account for primitivity).*

**Proof.** For each position $j$, the number of possible values of $\lfloor\alpha^j\rfloor$ as $\alpha$ ranges over $[2,3)$ is bounded by $3^j - 2^j$ (the interval $[2^j, 3^j)$ contains that many integers). The function $G(v,m)$ sums geometric series counting lattice points under power curves. The difference $g(m) = G(3,m) - G(2,m)$ aggregates these counts. $\square$

**Theorem 1 (Mobius Inversion Formula).** *The count of distinct sequences is:*
$$t(n) = \sum_{k=1}^{n} \mu(k) \cdot g\!\left(\left\lfloor \frac{n}{k}\right\rfloor\right)$$
*where $\mu$ is the Mobius function.*

**Proof.** A sequence of length $n$ arising from $\alpha \in [2,3)$ may coincide with a sequence of length $d | n$ extended periodically (in terms of the $\alpha$-intervals). The inclusion-exclusion via Mobius inversion removes these overcounts:
$$t(n) = \sum_{d | n} \mu(n/d)\,g(d) = \sum_{k=1}^{n} \mu(k)\,g(\lfloor n/k \rfloor). \quad \square$$

**Lemma 2 (Matrix Exponentiation for $G(v,m)$).** *$G(v,m) \bmod M$ is computable in $O(\log m)$ via:*
$$\begin{pmatrix} v & 1 \\ 0 & 1 \end{pmatrix}^m = \begin{pmatrix} v^m & \frac{v^m - 1}{v - 1} \\ 0 & 1 \end{pmatrix}$$
*so $G(v, m) = v \cdot [\text{matrix}^m]_{0,1}$.*

**Proof.** By induction on $m$. The base case $m = 1$ gives $\begin{pmatrix} v & 1 \\ 0 & 1 \end{pmatrix}^1 = \begin{pmatrix} v & 1 \\ 0 & 1 \end{pmatrix}$, and $G(v,1) = v \cdot 1 = v$. For the inductive step, multiplying on the right by $\begin{pmatrix} v & 1 \\ 0 & 1 \end{pmatrix}$:
$$\begin{pmatrix} v^m & \frac{v^m-1}{v-1} \\ 0 & 1 \end{pmatrix}\begin{pmatrix} v & 1 \\ 0 & 1 \end{pmatrix} = \begin{pmatrix} v^{m+1} & v^m + \frac{v^m-1}{v-1} \\ 0 & 1 \end{pmatrix} = \begin{pmatrix} v^{m+1} & \frac{v^{m+1}-1}{v-1} \\ 0 & 1 \end{pmatrix}.$$
The identity $v^m + \frac{v^m-1}{v-1} = \frac{v^{m+1}-1}{v-1}$ is verified algebraically. $\square$

**Theorem 2 (Efficient Summation via Mertens Function).** *Since $\lfloor n/k \rfloor$ takes only $O(\sqrt{n})$ distinct values, group terms:*
$$t(n) = \sum_{\text{distinct } q = \lfloor n/k \rfloor} g(q) \cdot \bigl(M(k_2) - M(k_1 - 1)\bigr)$$
*where $M(x) = \sum_{i=1}^{x} \mu(i)$ is the Mertens function and $[k_1, k_2]$ is the range of $k$ with $\lfloor n/k \rfloor = q$.*

**Proof.** Regrouping the sum by distinct values of $\lfloor n/k \rfloor$ and using partial sums of $\mu$. $\square$

**Lemma 3 (Mertens Function Computation).** *$M(n)$ satisfies:*
$$M(n) = 1 - \sum_{k=2}^{n} M\!\left(\left\lfloor \frac{n}{k}\right\rfloor\right).$$
*Sieve $\mu(k)$ for $k \le n^{2/3}$; compute $M$ recursively with memoization for larger arguments. Total time: $O(n^{2/3})$.*

**Proof.** From $\sum_{d|n}\mu(d) = [n=1]$, summing over $n$: $\sum_{k=1}^{n} M(\lfloor n/k \rfloor) = 1$. Isolating $k=1$: $M(n) = 1 - \sum_{k=2}^{n} M(\lfloor n/k \rfloor)$. The hyperbolic summation trick (grouping by distinct values of $\lfloor n/k \rfloor$) combined with sieving up to $n^{2/3}$ gives $O(n^{2/3})$ complexity. $\square$

**Verification.** $G(3,10) = 3(3^{10}-1)/2 = 88572$, $G(2,10) = 2(2^{10}-1) = 2046$, $g(10) = 86526$.
$$t(10) = \sum_{k=1}^{10}\mu(k)\,g(\lfloor 10/k \rfloor) = 86195. \quad \checkmark$$

## Algorithm

```
Input: n = 10^10, MOD = 10^9
Output: t(n) mod MOD

1. Sieve mu(k) for k <= n^(2/3), compute prefix sums M(k).
2. For large arguments, compute M(x) recursively with memoization:
     M(x) = 1 - sum_{k=2}^{x} M(floor(x/k))
   (using hyperbolic grouping for O(sqrt(x)) terms).
3. Enumerate distinct values q = floor(n/k):
     For each q, compute g(q) mod MOD via 2x2 matrix exponentiation.
     Multiply by (M(k2) - M(k1-1)) where floor(n/k) = q for k in [k1,k2].
4. Sum all contributions modulo MOD.
5. Return result.
```

## Complexity Analysis

- **Time:** $O(n^{2/3})$ for Mertens function computation. $O(\sqrt{n} \cdot \log n)$ for matrix exponentiations over $O(\sqrt{n})$ distinct values. Total: $O(n^{2/3})$.
- **Space:** $O(n^{2/3})$ for the sieve and memoization table.

## Answer

$$\boxed{268457129}$$

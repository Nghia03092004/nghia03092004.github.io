# Problem 439: Sum of Sum of Divisors

## Problem Statement

Let $\sigma(k)$ denote the sum of divisors of $k$. Define
$$S(N) = \sum_{i=1}^{N} \sum_{j=1}^{N} \sigma(i \cdot j).$$
Given $S(10^3) = 563576517282$ and $S(10^5) \bmod 10^9 = 215766508$, find $S(10^{11}) \bmod 10^9$.

## Mathematical Foundation

**Theorem 1 (Multiplicative Convolution Identity).** For all positive integers $m, n$:
$$\sigma(mn) = \sum_{d \mid \gcd(m,n)} \mu(d) \cdot \sigma(m/d) \cdot \sigma(n/d).$$

**Proof.** Both sides are multiplicative in $(m, n)$ (viewed as a function of the pair). It suffices to verify for prime powers. Let $m = p^a$, $n = p^b$ with $a \leq b$. Then $\gcd(m,n) = p^a$ and:
$$\text{RHS} = \sum_{k=0}^{a} \mu(p^k) \sigma(p^{a-k}) \sigma(p^{b-k}) = \sigma(p^a)\sigma(p^b) - \sigma(p^{a-1})\sigma(p^{b-1}).$$
Using $\sigma(p^c) = (p^{c+1}-1)/(p-1)$, direct computation shows this equals $\sigma(p^{a+b}) = (p^{a+b+1}-1)/(p-1) = \text{LHS}$. $\square$

**Theorem 2 (Main Summation Formula).** Define $T(n) = \sum_{k=1}^n \sigma(k)$. Then
$$S(N) = \sum_{d=1}^{N} \mu(d) \cdot T\!\left(\left\lfloor \frac{N}{d} \right\rfloor\right)^2.$$

**Proof.** Substituting Theorem 1 into $S(N)$:
\begin{align*}
S(N) &= \sum_{i=1}^N \sum_{j=1}^N \sum_{d \mid \gcd(i,j)} \mu(d)\,\sigma(i/d)\,\sigma(j/d).
\end{align*}
Setting $i = da$, $j = db$:
\begin{align*}
S(N) &= \sum_{d=1}^N \mu(d) \sum_{a=1}^{\lfloor N/d\rfloor} \sigma(a) \sum_{b=1}^{\lfloor N/d\rfloor} \sigma(b) = \sum_{d=1}^N \mu(d) \cdot T(\lfloor N/d \rfloor)^2. \quad \square
\end{align*}

**Lemma 1 (Efficient Computation of $T(n)$).**
$$T(n) = \sum_{k=1}^n \sigma(k) = \sum_{d=1}^n d \cdot \left\lfloor \frac{n}{d} \right\rfloor,$$
which can be evaluated in $O(\sqrt{n})$ time by grouping values of $\lfloor n/d \rfloor$.

**Proof.** $T(n) = \sum_{k=1}^n \sum_{d \mid k} d = \sum_{d=1}^n d \cdot \lfloor n/d \rfloor$ by swapping the order of summation. The function $d \mapsto \lfloor n/d \rfloor$ takes $O(\sqrt{n})$ distinct values (since $\lfloor n/d \rfloor = q$ implies $d \in [n/(q+1), n/q]$). For each block of consecutive $d$-values sharing the same $\lfloor n/d \rfloor = q$, the sum $\sum_{d=\ell}^{r} d = r(r+1)/2 - \ell(\ell-1)/2$ is computable in $O(1)$. $\square$

**Theorem 3 (Sub-Linear Mertens Function).** The Mertens function $M(n) = \sum_{k=1}^n \mu(k)$ satisfies the recursion
$$M(n) = 1 - \sum_{k=2}^{n} M\!\left(\left\lfloor \frac{n}{k} \right\rfloor\right),$$
and can be computed in $O(n^{2/3})$ time with $O(n^{2/3})$ space using sieving for small arguments and memoization for large.

**Proof.** From the identity $\sum_{d=1}^n \mu(d) \lfloor n/d \rfloor = 1$ (a consequence of $\sum_{d \mid k} \mu(d) = [k = 1]$), we get $\sum_{k=1}^n M(\lfloor n/k \rfloor) = 1$, yielding the recursion. The number of distinct values of $\lfloor n/k \rfloor$ is $O(\sqrt{n})$. Sieving $\mu$ up to $n^{2/3}$ and recursively computing $M$ for the $O(n^{1/3})$ large arguments gives $O(n^{2/3})$ total time. $\square$

## Editorial
S(N) = sum_{i=1}^N sum_{j=1}^N d(i*j) Using the identity sigma(mn) = sum_{d|gcd(m,n)} mu(d) * sigma(m/d) * sigma(n/d): S(N) = sum_{d=1}^N mu(d) * T(N/d)^2 where T(n) = sum_{k=1}^n sigma(k) = sum_{d=1}^n d * floor(n/d) Find S(10^11) mod 10^9. We memoize Mertens function for large arguments. We then compute T(n) mod p in O(sqrt(n)). Finally, sum of d from d to d_max = d_max*(d_max+1)/2 - (d-1)*d/2.

## Pseudocode

```text
Sieve mu(d) for d <= N^{2/3}
Memoize Mertens function for large arguments
Compute T(n) mod p in O(sqrt(n))
sum of d from d to d_max = d_max*(d_max+1)/2 - (d-1)*d/2
Evaluate S(N) = sum_{d=1}^{N} mu(d) * T(N/d)^2
Group by values of floor(N/d)
sum of mu(d) for d in [d, d_max] = M(d_max) - M(d-1)
```

## Complexity Analysis

- **Time:** $O(N^{2/3})$ for the Mertens function computation (sieve + recursion). Each $T(q)$ call costs $O(\sqrt{q})$, and there are $O(\sqrt{N})$ distinct values of $q$, but the dominant cost is the Mertens sieve. Total: $O(N^{2/3})$.
- **Space:** $O(N^{2/3})$ for the sieve of $\mu$ and the memoization table for $M$.

## Answer

$$\boxed{968697378}$$

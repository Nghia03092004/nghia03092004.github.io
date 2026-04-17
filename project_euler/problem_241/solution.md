# Problem 241: Perfection Quotients

## Problem Statement

For a positive integer $n$, let $\sigma(n)$ denote the sum of all positive divisors of $n$. The **perfection quotient** of $n$ is defined as $p(n) = \sigma(n)/n$. A positive integer $n$ is called a **half-integer perfect number** if $p(n) = k + \tfrac{1}{2}$ for some non-negative integer $k$, or equivalently, $2\sigma(n) = n(2k+1)$ for some non-negative integer $k$.

Find the sum of all half-integer perfect numbers not exceeding $10^{18}$.

## Mathematical Development

**Definition 1 (Abundancy index).** For a positive integer $n$, the *abundancy index* is $\sigma(n)/n$. We say $n$ is *half-integer perfect* when $\sigma(n)/n \in \mathbb{Z} + \tfrac{1}{2}$.

**Theorem 1 (Characterisation).** A positive integer $n > 1$ is half-integer perfect if and only if the rational number $2\sigma(n)/n$ is an odd integer $\ge 3$.

*Proof.* Suppose $\sigma(n)/n = k + \tfrac{1}{2}$ for some non-negative integer $k$. Then $2\sigma(n)/n = 2k + 1$, which is an odd integer. Since $\sigma(n) \ge 1 + n > n$ for $n > 1$ (as $n$ has at least two divisors), we have $\sigma(n)/n > 1$, hence $2k + 1 \ge 3$.

Conversely, if $2\sigma(n)/n = 2k + 1$ for some integer $k \ge 1$, then $\sigma(n)/n = k + \tfrac{1}{2}$. $\blacksquare$

**Theorem 2 (Multiplicative decomposition).** Write $n = 2^a \cdot m$ with $m$ odd and $a \ge 0$. Since $\sigma$ is multiplicative and $\gcd(2^a, m) = 1$:
$$\sigma(n) = \sigma(2^a)\,\sigma(m) = (2^{a+1} - 1)\,\sigma(m).$$

The half-integer perfect condition $2\sigma(n)/n \in \{3, 5, 7, \ldots\}$ becomes:
$$\frac{2(2^{a+1} - 1)\,\sigma(m)}{2^a \cdot m} = \frac{(2^{a+1} - 1)\,\sigma(m)}{2^{a-1} \cdot m} \in \{3, 5, 7, \ldots\}.$$

*Proof.* The identity $\sigma(2^a) = 2^{a+1} - 1$ follows from the geometric series $\sum_{j=0}^{a} 2^j$. Multiplicativity of $\sigma$ for coprime arguments is standard. The algebraic manipulation is direct. $\blacksquare$

**Lemma 1 (Divisibility constraint on $\sigma(m)$).** For $a \ge 1$, the expression $(2^{a+1} - 1)\,\sigma(m) / (2^{a-1}\,m)$ can be an integer only if $2^{a-1} \mid \sigma(m)$.

*Proof.* Observe that $2^{a+1} - 1$ is odd and $m$ is odd, so $\gcd(2^{a-1},\, (2^{a+1} - 1) \cdot m) = 1$. For the fraction to be integral, the denominator $2^{a-1} \cdot m$ must divide the numerator $(2^{a+1} - 1)\,\sigma(m)$. Since $\gcd(2^{a-1}, (2^{a+1}-1)m) = 1$, the factor $2^{a-1}$ must divide $\sigma(m)$. $\blacksquare$

**Lemma 2 (Abundancy upper bound).** For $n = p_1^{e_1} \cdots p_k^{e_k}$,
$$\frac{\sigma(n)}{n} = \prod_{i=1}^{k} \frac{\sigma(p_i^{e_i})}{p_i^{e_i}} = \prod_{i=1}^{k} \sum_{j=0}^{e_i} p_i^{-j} < \prod_{i=1}^{k} \frac{p_i}{p_i - 1}.$$

*Proof.* Each factor $\sigma(p^e)/p^e = 1 + p^{-1} + \cdots + p^{-e}$ is a partial sum of the geometric series $\sum_{j=0}^{\infty} p^{-j} = p/(p-1)$, hence strictly less than $p/(p-1)$. $\blacksquare$

**Corollary 1 (Pruning criterion).** During a depth-first search that constructs $n$ by choosing odd prime factors in increasing order, if the product $\prod_{p \mid m,\, p \ge p_0} p/(p-1)$ (taken over all primes $\ge p_0$ dividing $m$ and all larger potential primes) is insufficient to bring $2\sigma(n)/n$ to any odd integer $\ge 3$, the branch can be pruned.

## Algorithm

```
function FindHalfIntegerPerfect(bound):
    results = []
    # Begin search: n = 1, ratio 2*sigma(1)/1 = 2/1
    # Build n by multiplying by prime powers p^e in increasing order of p
    DFS(n=1, num=2, den=1, min_prime=2, bound, results)
    return sum(results)

function DFS(n, num, den, min_prime, bound, results):
    # num/den = 2*sigma(n)/n in lowest terms
    if den == 1 and num is odd and num >= 3 and n > 1:
        results.append(n)

    if num > 200 * den:    # ratio too large, prune
        return

    max_p = bound / n
    if max_p < min_prime:
        return

    for each prime p >= min_prime with p <= max_p:
        for e = 1, 2, ...:
            pe = p^e
            if n * pe > bound: break
            sig_pe = sigma(p^e) = (p^{e+1} - 1) / (p - 1)
            # Update fraction: new_num/new_den = (num * sig_pe) / (den * pe)
            # Reduce by gcd
            new_num, new_den = reduce(num * sig_pe, den * pe)
            DFS(n * pe, new_num, new_den, next_prime(p), bound, results)
```

The search also tries "denominator-targeted" primes: for each divisor $d$ of the current denominator, the values $d \pm 1$ and $2d \pm 1$ are tested for primality, since such primes are likely to cancel denominator factors.

## Complexity Analysis

- **Time.** The search tree explores factorisations of odd parts $m$ with $2^a m \le 10^{18}$. The abundancy bound (Lemma 2) and divisibility constraint (Lemma 1) provide strong pruning: empirically, $O(10^6)$ nodes are visited across all branches.
- **Space.** $O(\log N)$ for the recursion stack. The maximum depth equals the number of distinct prime factors of $n$, which is at most $\lfloor \log_3(10^{18}) \rfloor = 38$.

## Answer

$$\boxed{482316491800641154}$$

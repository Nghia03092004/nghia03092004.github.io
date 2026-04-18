# Problem 131: Prime Cube Partnership

## Problem Statement

There are some primes $p$ for which there exists a positive integer $n$ such that the expression $n^3 + n^2 p$ is a perfect cube. How many primes below one million satisfy this property?

## Mathematical Development

**Theorem 1 (Characterization).** A prime $p$ satisfies $n^3 + n^2 p = k^3$ for some positive integers $n, k$ if and only if $p$ is a difference of consecutive cubes, i.e., $p = (s+1)^3 - s^3$ for some positive integer $s$.

**Proof.** ($\Rightarrow$) Suppose $n^3 + n^2 p = k^3$ for positive integers $n, k$. Factor the left-hand side:

$$n^2(n + p) = k^3. \tag{1}$$

Since $k > n$ (as $k^3 = n^3 + n^2 p > n^3$), write $k = n \cdot r/s$ where $\gcd(r, s) = 1$ and $r > s \geq 1$. Since $k$ is a positive integer and $s \mid n$ (as $\gcd(r,s) = 1$), set $n = s^3 q$ for a positive integer $q$. Then $k = s^2 q r$, and substituting into (1):

$$s^6 q^2 (s^3 q + p) = s^6 q^3 r^3.$$

Canceling $s^6 q^2 > 0$ yields $s^3 q + p = q r^3$, whence

$$p = q(r^3 - s^3). \tag{2}$$

Since $p$ is prime, $q \geq 1$, and $r^3 - s^3 \geq 1$, we consider two cases:

- **Case 1:** $q = 1$ and $r^3 - s^3 = p$. By the factorization $r^3 - s^3 = (r - s)(r^2 + rs + s^2)$, and since $r^2 + rs + s^2 \geq 1 + 1 + 1 = 3 > 1$ for $r, s \geq 1$, the primality of $p$ forces $r - s = 1$. Hence $r = s + 1$ and

$$p = (s+1)^3 - s^3 = 3s^2 + 3s + 1.$$

- **Case 2:** $r^3 - s^3 = 1$ and $q = p$. This requires $r = 1$ and $s = 0$, contradicting $s \geq 1$.

($\Leftarrow$) Conversely, suppose $p = (s+1)^3 - s^3$ for some positive integer $s$. Set $n = s^3$. Then:

$$n^3 + n^2 p = s^9 + s^6\bigl[(s+1)^3 - s^3\bigr] = s^6(s+1)^3 = \bigl[s^2(s+1)\bigr]^3,$$

which is a perfect cube with $k = s^2(s+1)$. $\square$

**Lemma 1 (Search Bound).** The condition $p = 3s^2 + 3s + 1 < 10^6$ is equivalent to $s \leq 577$.

**Proof.** The quadratic $3s^2 + 3s + 1 < 10^6$ gives $s < \frac{-3 + \sqrt{9 + 12(10^6 - 1)}}{6} \approx 577.35$. Since $s$ must be a positive integer, $s \leq 577$. Verification: $3(577)^2 + 3(577) + 1 = 998{,}788 < 10^6$ and $3(578)^2 + 3(578) + 1 = 1{,}002{,}469 > 10^6$. $\square$

## Editorial
A prime p satisfies n^3 + n^2*p = k^3 for some positive integer n if and only if p = (s+1)^3 - s^3 = 3s^2 + 3s + 1 for some positive integer s. Count such primes below 10^6.

## Pseudocode

```text
    sieve primes up to N
    count = 0
    for s = 1, 2, ...:
        p = 3*s^2 + 3*s + 1
        If p >= N then stop this loop
        if is_prime(p): count += 1
    Return count
```

## Complexity Analysis

- **Time:** The loop runs $O(\sqrt{N})$ iterations (by Lemma 1, $s = O(\sqrt{N})$). With a precomputed sieve of size $N$, each primality test is $O(1)$. The sieve costs $O(N \log\log N)$. Total: $O(N \log\log N)$.
- **Space:** $O(N)$ for the Boolean sieve array.

## Answer

$$\boxed{173}$$

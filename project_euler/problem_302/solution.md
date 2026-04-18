# Problem 302: Strong Achilles Numbers

## Problem Statement

A positive integer $n$ is **powerful** if for every prime $p$ dividing $n$, we have $p^2 \mid n$. An **Achilles number** is a powerful number that is not a perfect power (i.e., it cannot be expressed as $m^k$ for integers $m \ge 1$, $k \ge 2$). A **Strong Achilles number** is an Achilles number $n$ such that $\varphi(n)$ is also an Achilles number, where $\varphi$ denotes Euler's totient function.

How many Strong Achilles numbers are there below $10^{18}$?

## Mathematical Development

**Definition 1.** A positive integer $n$ is *powerful* if $v_p(n) \ge 2$ for every prime $p \mid n$, where $v_p(n)$ denotes the $p$-adic valuation.

**Theorem 1** (Characterization of powerful numbers). *A positive integer $n > 1$ is powerful if and only if every exponent in its canonical factorization $n = p_1^{e_1} \cdots p_k^{e_k}$ satisfies $e_i \ge 2$. Equivalently, $n$ can be written as $n = a^2 b^3$ for some positive integers $a, b$.*

*Proof.* The first equivalence is immediate from the definition: $p_i \mid n$ implies $p_i^2 \mid n$ if and only if $v_{p_i}(n) = e_i \ge 2$.

For the second equivalence, suppose every $e_i \ge 2$. If $e_i$ is even, write $e_i = 2 \cdot (e_i/2)$. If $e_i$ is odd, then $e_i \ge 3$, so write $e_i = 2 \cdot \frac{e_i - 3}{2} + 3$. Collecting the contributions from the even parts into $a^2$ and the residual factor of 3 into $b^3$ yields $n = a^2 b^3$. Conversely, if $n = a^2 b^3$, then $v_p(n) = 2 v_p(a) + 3 v_p(b) \ge 2$ whenever $p \mid n$. $\square$

**Lemma 1** (Perfect power criterion). *A positive integer $n = p_1^{e_1} \cdots p_k^{e_k}$ with $k \ge 1$ is a perfect power if and only if $g := \gcd(e_1, \ldots, e_k) \ge 2$.*

*Proof.* If $g \ge 2$, then $n = \bigl(\prod_{i=1}^k p_i^{e_i/g}\bigr)^g$, so $n$ is a $g$-th power. Conversely, if $n = m^d$ with $d \ge 2$, then $v_{p_i}(n) = d \cdot v_{p_i}(m)$, whence $d \mid e_i$ for all $i$, giving $g \ge d \ge 2$. $\square$

**Lemma 2** (Totient of a powerful number). *For $n = \prod_{i=1}^k p_i^{e_i}$ with all $e_i \ge 1$, we have $\varphi(n) = \prod_{i=1}^k p_i^{e_i - 1}(p_i - 1)$.*

*Proof.* This follows from the multiplicativity of $\varphi$ and the standard formula $\varphi(p^e) = p^{e-1}(p - 1)$. $\square$

**Theorem 2** (Strong Achilles criterion). *A positive integer $n = p_1^{e_1} \cdots p_k^{e_k}$ is a Strong Achilles number if and only if all of the following hold:*

1. *(Powerful)* $e_i \ge 2$ for all $i$.
2. *(Not a perfect power)* $\gcd(e_1, \ldots, e_k) = 1$.
3. *($\varphi(n)$ is powerful)* In the canonical factorization of $\varphi(n) = \prod_{i=1}^k p_i^{e_i - 1}(p_i - 1)$, every prime appears with exponent $\ge 2$.
4. *($\varphi(n)$ is not a perfect power)* The gcd of all exponents in the factorization of $\varphi(n)$ equals $1$.

*Proof.* Conditions (1)--(2) are equivalent to $n$ being Achilles, by Theorem 1 and Lemma 1. Conditions (3)--(4) state that $\varphi(n)$ is Achilles. Together, $n$ is Strong Achilles. $\square$

**Remark.** To verify condition (3), one must factor each $p_i - 1$ and merge exponents with the contributions $p_i^{e_i - 1}$. For primes $p_i$ appearing in the factorization of $n < 10^{18}$ with $e_i \ge 2$, we have $p_i \le 10^9$, so factoring $p_i - 1$ is efficient.

## Editorial
A Strong Achilles number n satisfies: (1) n is powerful (all prime exponents >= 2), (2) n is not a perfect power (gcd of exponents == 1), (3) phi(n) is also Achilles. Enumerate powerful numbers via DFS over prime factorizations, checking all four conditions at each node.

## Pseudocode

```text
    Set primes <- sieve of primes up to 10^6
    Set count <- 0

        If product has >= 2 prime factors and gcd_exp == 1 then
            If phi_factors is powerful and gcd(phi exponents) == 1 then
                Set count <- count + 1

        for each prime p = primes[idx], primes[idx+1], ... :
            If product * p^2 > N then stop this loop
            for e = 2, 3, ... while product * p^e <= N:
                Set new_gcd <- gcd(gcd_exp, e)
                Set new_phi <- phi_factors merged with factors of p^(e-1)*(p-1)
                dfs(idx + 1, product * p^e, new_gcd, new_phi)

    dfs(0, 1, 0, {})
    Return count
```

## Complexity Analysis

- **Time:** The number of powerful numbers below $N$ is $\Theta(\sqrt{N})$, approximately $c\sqrt{N}$ with $c \approx 1.94$ (Golomb, 1970). For $N = 10^{18}$, this is $\sim 2 \times 10^9$ in the worst case, but the Achilles and Strong Achilles conditions aggressively prune the DFS tree. In practice, the search terminates in manageable time.
- **Space:** $O(N^{1/3} / \ln N)$ for the prime sieve (primes up to $\sqrt{N} \approx 10^9$, though we need only primes up to $10^6$ since any prime with exponent $\ge 3$ satisfies $p \le N^{1/3}$). Recursion depth is $O(\log N / \log 2) = O(60)$.

## Answer

$$\boxed{1170060}$$

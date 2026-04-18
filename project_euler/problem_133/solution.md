# Problem 133: Repunit Nonfactors

## Problem Statement

A repunit $R(k) = \frac{10^k - 1}{9}$. Find the sum of all primes below $100{,}000$ that will never be a factor of $R(10^n)$ for any positive integer $n$.

## Mathematical Development

**Theorem 1 (5-Smooth Order Criterion).** For a prime $p$ with $p \nmid 10$ and $p \neq 3$, there exists a positive integer $n$ such that $p \mid R(10^n)$ if and only if $\operatorname{ord}_p(10)$ is 5-smooth (i.e., all prime factors of $\operatorname{ord}_p(10)$ lie in $\{2, 5\}$).

**Proof.** By the divisibility theory for repunits (Theorem 1 of Problem 132), $p \mid R(k)$ iff $\operatorname{ord}_p(10) \mid k$. Therefore:

$$\bigl(\exists\, n \geq 1 : p \mid R(10^n)\bigr) \iff \bigl(\exists\, n \geq 1 : \operatorname{ord}_p(10) \mid 10^n\bigr).$$

($\Rightarrow$) If $\operatorname{ord}_p(10) \mid 10^n = 2^n \cdot 5^n$ for some $n$, then every prime factor of $\operatorname{ord}_p(10)$ divides $2^n \cdot 5^n$, so $\operatorname{ord}_p(10)$ is 5-smooth.

($\Leftarrow$) If $\operatorname{ord}_p(10) = 2^a \cdot 5^b$, take $n = \max(a, b)$. Then $10^n = 2^n \cdot 5^n$ is divisible by $2^a \cdot 5^b = \operatorname{ord}_p(10)$, so $p \mid R(10^n)$. $\square$

**Lemma 1 (Permanent Nonfactors).** The primes $p = 2$, $p = 3$, and $p = 5$ never divide $R(10^n)$ for any $n \geq 1$.

**Proof.** The repunit $R(k) = \underbrace{11\ldots1}_k$ is odd (so $2 \nmid R(k)$) and ends in digit 1 (so $5 \nmid R(k)$), for all $k \geq 1$.

For $p = 3$: since $\operatorname{ord}_{27}(10) = 3$ (verifiable: $10^3 = 1000 \equiv 1 \pmod{27}$), we have $3 \mid R(k) \iff 3 \mid k$. Since $\gcd(3, 10) = 1$ implies $3 \nmid 10^n$ for all $n$, the prime 3 never divides $R(10^n)$. $\square$

**Lemma 2 (Efficient Testing).** For $p < 10^5$, the condition "$\operatorname{ord}_p(10)$ is 5-smooth" is equivalent to $10^{10^{20}} \equiv 1 \pmod{p}$, where $10^{20} = 2^{20} \cdot 5^{20}$.

**Proof.** Since $\operatorname{ord}_p(10) \mid p - 1 < 10^5$, any 5-smooth divisor of $p - 1$ is of the form $2^a \cdot 5^b$ with $a \leq \lfloor \log_2(10^5) \rfloor = 16$ and $b \leq \lfloor \log_5(10^5) \rfloor = 7$. Hence $\operatorname{ord}_p(10) \mid 2^{20} \cdot 5^{20} = 10^{20}$, and the claim follows from the definition of multiplicative order.

Conversely, if $10^{10^{20}} \equiv 1 \pmod{p}$, then $\operatorname{ord}_p(10) \mid 10^{20}$, so $\operatorname{ord}_p(10)$ is 5-smooth. $\square$

**Remark.** An equivalent approach computes $\operatorname{ord}_p(10)$ explicitly by factoring $p - 1$ and refining, then checks 5-smoothness by dividing out all factors of 2 and 5.

## Editorial
A prime p (not 2, 3, 5) can divide some R(10^n) iff ord_p(10) is 5-smooth. We iterate over p in primes.

## Pseudocode

```text
    N = 100000
    primes = sieve_primes(N)
    total = 0
    for p in primes:
        If p in {2, 3, 5} then
            total += p // permanent nonfactors
            continue
        d = multiplicative_order(10, p)
        If not is_5_smooth(d) then
            total += p // nonfactor
    Return total
```

## Complexity Analysis

- **Time:** $O(N \log\log N)$ for the sieve. For each prime $p$, computing $\operatorname{ord}_p(10)$ requires factoring $p - 1$ in $O(\sqrt{p})$ time and $O(\log p)$ modular exponentiations. Over all $\pi(N)$ primes, the total is $O(N^{3/2} / \log N)$ in the worst case, though typically much faster.
- **Space:** $O(N)$ for the sieve.

## Answer

$$\boxed{453647705}$$

# Problem 20: Factorial Digit Sum

## Problem Statement

Let $S(m)$ denote the sum of the decimal digits of a positive integer $m$. Compute $S(100!)$.

## Mathematical Development

**Definition 1 (Digit sum).** For a positive integer $m = \sum_{k=0}^{d-1} a_k \cdot 10^k$ with digits $a_k \in \{0, \ldots, 9\}$ and $a_{d-1} \ne 0$, define $S(m) = \sum_{k=0}^{d-1} a_k$.

**Theorem 1 (Digit count of $n!$).** The number of digits of $n!$ is

$$d(n!) = \lfloor \log_{10}(n!) \rfloor + 1, \quad \text{where } \log_{10}(n!) = \sum_{k=1}^{n} \log_{10}(k).$$

*Proof.* A positive integer $m$ has $\lfloor \log_{10} m \rfloor + 1$ digits. Applying the multiplicative property of logarithms: $\log_{10}(n!) = \log_{10}\!\bigl(\prod_{k=1}^{n} k\bigr) = \sum_{k=1}^{n} \log_{10}(k)$. $\square$

**Corollary 1.** By Stirling's approximation, $\log_{10}(n!) \approx n \log_{10}(n/e) + \frac{1}{2}\log_{10}(2\pi n)$. For $n = 100$: $\log_{10}(100!) \approx 157.97$, so $d(100!) = 158$.

**Theorem 2 (Digit sum congruence).** For every positive integer $m$, $S(m) \equiv m \pmod{9}$.

*Proof.* Since $10 \equiv 1 \pmod{9}$, we have $10^k \equiv 1 \pmod{9}$ for all $k \ge 0$, hence $m = \sum a_k \cdot 10^k \equiv \sum a_k = S(m) \pmod{9}$. $\square$

**Theorem 3 (Legendre's formula).** For a prime $p$ and positive integer $n$, the $p$-adic valuation of $n!$ is

$$\nu_p(n!) = \sum_{i=1}^{\infty} \left\lfloor \frac{n}{p^i} \right\rfloor.$$

*Proof.* Each factor $k \in \{1, \ldots, n\}$ contributes $\nu_p(k)$ to $\nu_p(n!)$. The number of multiples of $p^i$ in $\{1, \ldots, n\}$ is $\lfloor n/p^i \rfloor$. By inclusion, $\nu_p(n!) = \sum_{k=1}^{n} \nu_p(k) = \sum_{i=1}^{\infty} \lfloor n/p^i \rfloor$. (The sum is finite since $\lfloor n/p^i \rfloor = 0$ for $p^i > n$.) $\square$

**Proposition 1 ($100! \equiv 0 \pmod{9}$).** We have $\nu_3(100!) = \lfloor 100/3 \rfloor + \lfloor 100/9 \rfloor + \lfloor 100/27 \rfloor + \lfloor 100/81 \rfloor = 33 + 11 + 3 + 1 = 48$.

*Proof.* Direct application of Theorem 3 with $p = 3$. Since $\nu_3(100!) = 48 \ge 2$, we have $9 \mid 100!$, and by Theorem 2, $S(100!) \equiv 0 \pmod{9}$. $\square$

**Verification.** $S(100!) = 648 = 72 \times 9$, confirming $648 \equiv 0 \pmod{9}$.

**Theorem 4 (Trailing zeros of $n!$).** The number of trailing zeros of $n!$ is $\nu_5(n!) = \sum_{i=1}^{\infty} \lfloor n/5^i \rfloor$.

*Proof.* A trailing zero corresponds to a factor of $10 = 2 \times 5$. Since $\nu_2(n!) \ge \nu_5(n!)$ for all $n \ge 1$ (as multiples of 2 are more frequent than multiples of 5), the count of trailing zeros equals $\min\!\bigl(\nu_2(n!), \nu_5(n!)\bigr) = \nu_5(n!)$. $\square$

**Corollary 2.** $\nu_5(100!) = \lfloor 100/5 \rfloor + \lfloor 100/25 \rfloor = 20 + 4 = 24$. Hence $100!$ has 24 trailing zeros, which contribute nothing to $S(100!)$.

**Proposition 2 (Bounds on $S(100!)$).** Since $100!$ has 158 digits:

$$1 \le S(100!) \le 9 \times 158 = 1422.$$

The 134 non-trailing-zero digits have an average value near $4.5$, giving a heuristic estimate $\approx 603$. The actual value $648$ is modestly above this estimate.

*Proof.* Lower bound: at least one digit is nonzero. Upper bound: each digit $\le 9$. $\square$

## Algorithm

We form $n!$ by multiplying the integers from 2 through $n$, then extract its decimal digits and add them. The second phase repeatedly takes the last digit with a modulo-10 operation and removes it with integer division until no digits remain. This is sufficient because every digit of $n!$ is processed exactly once.

## Pseudocode

```text
function factorialDigitSum(n):
    factorial <- 1
    for k <- 2 to n:
        factorial <- factorial * k

    total <- 0
    while factorial > 0:
        total <- total + (factorial mod 10)
        factorial <- floor(factorial / 10)
    return total
```

## Complexity Analysis

**Proposition 3 (Time complexity).** The algorithm runs in $O(n^2 \log n)$ time.

*Proof.* Let $d = d(n!) = \Theta(n \log n)$ by Stirling's approximation. At step $k$, the running product has $\Theta(k \log k)$ digits, and multiplying it by $k$ (a number with $O(\log n)$ digits) costs $O(k \log k)$ using schoolbook multiplication. The total time is:
$$\sum_{k=2}^{n} O(k \log k) = O\!\left(\log n \sum_{k=2}^{n} k\right) = O(n^2 \log n).$$
The final digit-extraction loop runs in $O(d) = O(n \log n)$, which is dominated by the multiplication phase. $\square$

**Proposition 4 (Space complexity).** The algorithm uses $\Theta(n \log n)$ space.

*Proof.* The intermediate product requires $O(d) = O(n \log n)$ digits of storage. $\square$

## Answer

$$\boxed{648}$$

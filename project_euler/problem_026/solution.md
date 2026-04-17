# Problem 26: Reciprocal Cycles

## Problem Statement

Find the value of $d < 1000$ for which $1/d$ contains the longest recurring cycle in its decimal fraction part.

## Mathematical Development

**Definition 1 (Multiplicative Order).** Let $a, n \in \mathbb{Z}$ with $n \geq 2$ and $\gcd(a, n) = 1$. The *multiplicative order* of $a$ modulo $n$, denoted $\operatorname{ord}_n(a)$, is the smallest positive integer $k$ such that $a^k \equiv 1 \pmod{n}$. Existence is guaranteed by Euler's theorem, which gives $a^{\phi(n)} \equiv 1 \pmod{n}$.

**Definition 2 (Full Reptend Prime).** A prime $p$ with $\gcd(p, 10) = 1$ is a *full reptend prime* in base 10 if $\operatorname{ord}_p(10) = p - 1$, i.e., 10 is a primitive root modulo $p$.

**Theorem 1 (Cycle Length of Unit Fractions).** *Let $d \geq 2$ be a positive integer. Write $d = 2^\alpha \cdot 5^\beta \cdot d'$ where $\gcd(d', 10) = 1$. Then the length of the repeating block in the decimal expansion of $1/d$ is $\operatorname{ord}_{d'}(10)$ if $d' > 1$, and $0$ if $d' = 1$.*

*Proof.* Consider the long division of $1 \div d$. Define the remainder sequence by $r_0 = 1$ and $r_{k+1} = 10 \cdot r_k \bmod d$. The decimal expansion of $1/d$ consists of a non-repeating prefix of length $s = \max(\alpha, \beta)$ followed by a repeating block. To see this, note that $10^s \cdot (1/d) = q + 1/d'$ for some integer $q$, where $d' = d / (2^\alpha \cdot 5^\beta)$. Since $\gcd(d', 10) = 1$, the decimal expansion of $1/d'$ is purely repeating. Its period is the smallest $m > 0$ such that $10^m \equiv 1 \pmod{d'}$, which is precisely $\operatorname{ord}_{d'}(10)$. If $d' = 1$, then $1/d$ terminates and the cycle length is $0$. $\square$

**Theorem 2 (Fermat's Little Theorem).** *If $p$ is prime and $\gcd(a, p) = 1$, then $a^{p-1} \equiv 1 \pmod{p}$.*

*Proof.* The multiplicative group $(\mathbb{Z}/p\mathbb{Z})^*$ has order $p - 1$. By Lagrange's theorem, the order of every element divides the group order, so $a^{p-1} \equiv 1 \pmod{p}$. $\square$

**Lemma 1 (Divisibility of the Order).** *For a prime $p \nmid 10$, $\operatorname{ord}_p(10) \mid (p - 1)$.*

*Proof.* By Theorem 2, $10^{p-1} \equiv 1 \pmod{p}$. Let $k = \operatorname{ord}_p(10)$ and write $p - 1 = qk + r$ with $0 \leq r < k$. Then $10^r = 10^{p-1} \cdot (10^k)^{-q} \equiv 1 \cdot 1^{-q} \equiv 1 \pmod{p}$. Since $k$ is the smallest positive integer with $10^k \equiv 1 \pmod{p}$ and $0 \leq r < k$, we must have $r = 0$. Hence $k \mid (p - 1)$. $\square$

**Theorem 3 (Maximality of Cycle Length).** *For composite $d$ with $\gcd(d, 10) = 1$, $\operatorname{ord}_d(10) \leq \lambda(d) \leq \phi(d) < d - 1$, where $\lambda$ denotes the Carmichael function. Consequently, the cycle length of $1/d$ is strictly less than $d - 1$.*

*Proof.* By definition of the Carmichael function, $10^{\lambda(d)} \equiv 1 \pmod{d}$, so $\operatorname{ord}_d(10) \mid \lambda(d)$, giving $\operatorname{ord}_d(10) \leq \lambda(d)$. It is well known that $\lambda(d) \leq \phi(d)$ for all $d \geq 1$. For composite $d \geq 4$, Euler's totient satisfies $\phi(d) \leq d - \sqrt{d} < d - 1$ (since $d$ has a non-trivial factor). Thus $\operatorname{ord}_d(10) < d - 1$. $\square$

**Corollary 1.** *The maximum cycle length among all $d < N$ is achieved at a full reptend prime, i.e., a prime $p$ with $\operatorname{ord}_p(10) = p - 1$.*

*Proof.* By Theorem 3, composite $d$ yields cycle length strictly less than $d - 1$. A full reptend prime $p$ achieves cycle length $p - 1$. Since $p - 1 > q - 1 \geq \operatorname{ord}_q(10)$ for any $q < p$, the largest full reptend prime below $N$ maximizes the cycle length, provided we verify it by checking primes in descending order. $\square$

**Theorem 4 (Answer).** *The value $d < 1000$ that maximizes the cycle length of $1/d$ is $d = 983$, with cycle length $982$.*

*Proof.* By Corollary 1, we search for the largest full reptend prime below 1000. We check primes in decreasing order:
- $p = 997$: We compute $\operatorname{ord}_{997}(10)$ by checking divisors of $996 = 2^2 \cdot 3 \cdot 83$. We find $\operatorname{ord}_{997}(10) = 166 \neq 996$.
- $p = 991$: $990 = 2 \cdot 3^2 \cdot 5 \cdot 11$. We find $\operatorname{ord}_{991}(10) \neq 990$.
- $p = 983$: $982 = 2 \cdot 491$ where $491$ is prime. We verify $10^{982} \equiv 1 \pmod{983}$ (by Fermat), $10^{491} \not\equiv 1 \pmod{983}$, and $10^2 \not\equiv 1 \pmod{983}$. Since $\operatorname{ord}_{983}(10)$ divides $982$ and equals neither $1$, $2$, nor $491$, we conclude $\operatorname{ord}_{983}(10) = 982$. Thus $983$ is a full reptend prime.

Since $982 > p - 1$ for every prime $p < 983$, no smaller $d$ can produce a longer cycle. $\square$

## Algorithm

We test each denominator $d < N$ separately. After removing all factors of 2 and 5, terminating decimals are skipped; otherwise we simulate long division by repeatedly updating the remainder $r \mapsto 10r \bmod d'$ and recording the first step at which each remainder appears. The cycle length is the distance between repeated remainders, and the denominator with the largest such length is returned. This is sufficient because the repeating part of $1/d$ is completely determined by this remainder sequence.

## Pseudocode

```text
function longestRecurringCycle(limit):
    bestDenominator <- 0
    bestLength <- 0
    for d <- 2 to limit - 1:
        reduced <- d
        while reduced mod 2 = 0:
            reduced <- reduced / 2
        while reduced mod 5 = 0:
            reduced <- reduced / 5
        if reduced = 1:
            continue

        seen <- empty map
        remainder <- 1 mod reduced
        step <- 0
        while remainder not in seen:
            seen[remainder] <- step
            remainder <- (10 * remainder) mod reduced
            step <- step + 1
        cycleLength <- step - seen[remainder]
        if cycleLength > bestLength:
            bestLength <- cycleLength
            bestDenominator <- d
    return bestDenominator
```

## Complexity Analysis

**Proposition (Time Complexity).** *The algorithm runs in $O(N^2)$ time.*

*Proof.* For each $d \in \{2, \ldots, N-1\}$, the long-division simulation computes $\operatorname{ord}_{d'}(10)$ by iterating until a remainder repeats. Since the remainders lie in $\{0, 1, \ldots, d'-1\}$, the loop executes at most $d' \leq d$ iterations. Each iteration performs $O(1)$ arithmetic and hash-map operations (expected). Summing over all $d$: $\sum_{d=2}^{N-1} O(d) = O(N^2)$. $\square$

**Proposition (Space Complexity).** *The algorithm uses $O(N)$ auxiliary space.*

*Proof.* The hash map `seen` stores at most $d' - 1 < N$ entries for any single invocation. It is cleared between iterations, so peak usage is $O(N)$. $\square$

## Answer

$$\boxed{983}$$

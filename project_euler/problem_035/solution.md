# Problem 35: Circular Primes

## Problem Statement

The number 197 is called a circular prime because all rotations of its digits are prime: 197, 971, and 719.

How many circular primes are there below one million?

## Mathematical Development

**Definition 1 (Cyclic rotation).** For a $k$-digit positive integer $n$ with decimal representation $a_1 a_2 \ldots a_k$ (where $a_1 \ne 0$), the *cyclic rotation operator* $R$ is defined by
$$R(n) = (n \bmod 10) \cdot 10^{k-1} + \lfloor n / 10 \rfloor.$$
The *orbit* of $n$ under $R$ is $\mathrm{Orb}(n) = \{n, R(n), R^2(n), \ldots, R^{k-1}(n)\}$.

**Lemma 1 (Rotation formula).** *The operator $R$ maps $n = 10q + r$ (with $q = \lfloor n/10 \rfloor$, $r = n \bmod 10$) to $r \cdot 10^{k-1} + q$. Applying $R$ exactly $k$ times returns the original number: $R^k(n) = n$.*

**Proof.** The representation $n = 10q + r$ has digit string $a_1 a_2 \ldots a_{k-1} r$ where $q$ represents $a_1 a_2 \ldots a_{k-1}$. The map $R$ moves the last digit $r$ to the leading position: $R(n) = r \cdot 10^{k-1} + q = r\, a_1\, a_2 \ldots a_{k-1}$.

For the periodicity claim, observe that $R$ acts as a cyclic permutation on the digit string $a_1 a_2 \ldots a_k \mapsto a_k a_1 \ldots a_{k-1}$. Since cyclic permutations of $k$ elements have order $k$, $R^k$ is the identity. $\square$

**Definition 2 (Circular prime).** A prime $p$ is *circular* if every element of $\mathrm{Orb}(p)$ is prime.

**Lemma 2 (Single-digit circular primes).** *The single-digit circular primes are $2, 3, 5, 7$.*

**Proof.** A single-digit number $n$ has $\mathrm{Orb}(n) = \{n\}$. The single-digit primes are exactly $\{2, 3, 5, 7\}$, each trivially circular. $\square$

**Theorem 1 (Digit restriction).** *If $p$ is a circular prime with $k \ge 2$ digits, then every digit of $p$ belongs to $\{1, 3, 7, 9\}$.*

**Proof.** We show that no digit of $p$ can be $0, 2, 4, 5, 6$, or $8$.

*Even digits ($0, 2, 4, 6, 8$):* Suppose digit $a_i$ is even for some $1 \le i \le k$. The rotation $R^{k-i}(p)$ places $a_i$ in the units position, yielding an even number. Since $k \ge 2$, this number is $\ge 10$, hence an even number greater than 2, which is composite. This contradicts $R^{k-i}(p)$ being prime.

*Digit 5:* Suppose $a_i = 5$. Then $R^{k-i}(p)$ ends in 5 and is $\ge 10 > 5$, hence divisible by 5 but not equal to 5. This is composite, a contradiction.

*Digit 0:* Suppose $a_i = 0$. Then $R^{k-i}(p)$ has leading digit 0, making it a number with fewer than $k$ digits (or zero). But all rotations of a $k$-digit prime should be $k$-digit primes, since $p < 10^k$ implies all rotations are $< 10^k$, and a rotation with leading zero has value $< 10^{k-1}$, contradicting the expectation that it equals a specific $k$-digit rotation. More directly, $R^{k-i}(p) = 0 \cdot 10^{k-1} + q' < 10^{k-1}$, which could cause the subsequent rotation formula (which assumes $k$ digits) to malfunction. In any case, a number with leading digit 0 is not considered a $k$-digit number in standard notation.

Therefore, all digits must be odd and not 0 or 5, leaving $\{1, 3, 7, 9\}$. $\square$

**Theorem 2 (Sieve of Eratosthenes).** *For any positive integer $N$, the sieve of Eratosthenes correctly identifies all primes in $\{2, 3, \ldots, N-1\}$ in $O(N \log \log N)$ time using $O(N)$ space.*

**Proof.** (Standard.) The sieve initializes a boolean array of size $N$, then for each prime $p \le \lfloor\sqrt{N}\rfloor$, marks all multiples $p^2, p^2 + p, \ldots$ as composite. Correctness follows from the fact that every composite $m < N$ has a prime factor $\le \sqrt{m} \le \sqrt{N}$. The time complexity is $\sum_{p \le N} N/p = O(N \log \log N)$ by Mertens' second theorem. $\square$

**Theorem 3 (Correctness of the rotation-check algorithm).** *Given a prime sieve up to $N$, the following procedure correctly identifies all circular primes below $N$: for each prime $p < N$, compute all $k$ rotations of $p$ via Lemma 1; accept $p$ as circular if and only if every rotation $r$ satisfies $0 \le r < N$ and $r$ is marked prime.*

**Proof.** If $p$ is circular, then all rotations are prime and (since $p < N$ and all rotations of a $k$-digit number are also $k$-digit numbers with the same digit set) all rotations are $< N$. Conversely, if some rotation $r$ is composite or $r \ge N$, then $p$ is not circular. The sieve provides exact primality answers for all integers in $[0, N)$, so the procedure is correct. $\square$

## Algorithm

```
CIRCULAR-PRIMES(N):
    is_prime[0..N-1] <- SIEVE(N)
    count <- 0
    for p <- 2 to N-1:
        if not is_prime[p]: continue
        k <- number of digits of p
        pow10 <- 10^(k-1)
        r <- p
        is_circular <- true
        for j <- 1 to k-1:
            r <- (r mod 10) * pow10 + floor(r / 10)
            if r < 0 or r >= N or not is_prime[r]:
                is_circular <- false
                break
        if is_circular:
            count <- count + 1
    return count
```

## Complexity Analysis

**Proposition.** *For $N = 10^6$, the algorithm runs in $O(N \log \log N)$ time and $O(N)$ space.*

**Proof.** The sieve dominates at $O(N \log \log N)$. The rotation check iterates over all primes $p < N$; by the prime-counting function, there are $\pi(N) \approx N / \ln N \approx 78{,}498$ primes below $10^6$. For each prime, at most $k - 1 \le 5$ rotations are computed in $O(1)$ each. Thus the rotation phase costs $O(\pi(N) \cdot d_{\max}) = O(N / \ln N \cdot 6) = o(N)$, dominated by the sieve. Space is $O(N)$ for the boolean sieve array. $\square$

## Answer

$$\boxed{55}$$

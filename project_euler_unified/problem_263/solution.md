# Problem 263: An Engineers' Dream Come True

## Problem Statement

Define $n$ to be an **engineer's paradise** if:
1. $(n-9, n-3, n+3, n+9)$ are four consecutive primes (a "triple-pair" of sexy primes, each consecutive pair differing by exactly 6).
2. $n-8$, $n-4$, $n$, $n+4$, $n+8$ are all practical numbers.

Find the sum of the first four engineers' paradises.

## Mathematical Foundation

**Definition.** A positive integer $m$ is **practical** if every integer $1 \le k \le m$ can be represented as a sum of distinct divisors of $m$.

**Theorem 1 (Stewart's criterion).** *A positive integer $m \ge 2$ is practical if and only if $m$ is even and, writing $m = p_1^{a_1} p_2^{a_2} \cdots p_r^{a_r}$ with $p_1 < p_2 < \cdots < p_r$, we have for each $i \ge 2$:*

$$p_i \le 1 + \sigma(p_1^{a_1} \cdots p_{i-1}^{a_{i-1}})$$

*where $\sigma$ denotes the sum-of-divisors function.*

**Proof.** See B. M. Stewart, "Sums of distinct divisors," *Amer. J. Math.* **76** (1954), 779--785. The forward direction: if $p_i > 1 + \sigma(p_1^{a_1}\cdots p_{i-1}^{a_{i-1}})$, then the integer $1 + \sigma(p_1^{a_1}\cdots p_{i-1}^{a_{i-1}})$ cannot be represented. The reverse: if the condition holds for all $i$, an inductive construction shows every integer up to $\sigma(m)$ (hence up to $m$) is representable. $\square$

**Lemma 1 (Modular constraints on $n$).** *If $n-9, n-3, n+3, n+9$ are all primes greater than 5, then $n \equiv 10$ or $20 \pmod{30}$.*

**Proof.**
- *Modulo 2:* The four values $n \pm 3$, $n \pm 9$ must be odd, so $n$ is even.
- *Modulo 3:* Since $n - 9 \equiv n \pmod{3}$ and $n - 3 \equiv n \pmod{3}$, all four values are $\equiv n \pmod{3}$. For none to be divisible by 3, we need $n \not\equiv 0 \pmod{3}$.
- *Modulo 5:* The residues $n - 9, n - 3, n + 3, n + 9$ modulo 5 are $n+1, n+2, n+3, n+4$. For none to be $0 \pmod{5}$, we need $n \equiv 0 \pmod{5}$.

Combining: $n \equiv 0 \pmod{2}$, $n \not\equiv 0 \pmod{3}$, $n \equiv 0 \pmod{5}$. By the Chinese Remainder Theorem, $n \equiv 10$ or $20 \pmod{30}$. $\square$

**Lemma 2 (Consecutivity conditions).** *For the four primes to be consecutive, the six intermediate odd values $n - 7, n - 5, n - 1, n + 1, n + 5, n + 7$ must all be composite.*

**Proof.** Since $n$ is even, the only integers strictly between consecutive members of $\{n-9, n-3, n+3, n+9\}$ that could be prime are the odd ones: $n-7, n-5$ (between $n-9$ and $n-3$), $n-1, n+1$ (between $n-3$ and $n+3$), and $n+5, n+7$ (between $n+3$ and $n+9$). All must be composite for consecutivity. $\square$

## Algorithm

```
1. Sieve primes up to 1.2 * 10^9 using Sieve of Eratosthenes

2. results = []
   for n in arithmetic progression {10, 20, 40, 50, 70, 80, ...}
       (i.e., n = 10 or 20 mod 30, incrementing):
       if not (isPrime(n-9) and isPrime(n-3) and isPrime(n+3) and isPrime(n+9)):
           continue
       if isPrime(n-7) or isPrime(n-5) or isPrime(n-1) or
          isPrime(n+1) or isPrime(n+5) or isPrime(n+7):
           continue
       if not (isPractical(n-8) and isPractical(n-4) and isPractical(n)
               and isPractical(n+4) and isPractical(n+8)):
           continue
       results.append(n)
       if len(results) == 4: break

3. return sum(results)
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the Sieve of Eratosthenes up to $N \approx 1.2 \times 10^9$. The main loop iterates over $O(N/15)$ candidates. Each practical-number test factors $m$ in $O(\sqrt{m})$ and checks Stewart's criterion in $O(\log m)$. Total: $O(N \log \log N)$.
- **Space:** $O(N)$ for the prime sieve bitmap.

## Answer

$$\boxed{2039506520}$$

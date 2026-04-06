# Problem 211: Divisor Square Sum

## Problem Statement

For a positive integer $n$, let $\sigma_2(n)$ denote the sum of the squares of the divisors of $n$:

$$\sigma_2(n) = \sum_{d \mid n} d^2$$

For example, $\sigma_2(10) = 1^2 + 2^2 + 5^2 + 10^2 = 130$.

Find the sum of all $n$, $0 < n < 64{,}000{,}000$, such that $\sigma_2(n)$ is a perfect square.

## Mathematical Development

**Definition 1.** For $k \in \mathbb{Z}_{\geq 0}$, the *divisor power sum function* is $\sigma_k(n) = \sum_{d \mid n} d^k$. The case $k = 2$ gives the sum-of-squares-of-divisors function.

**Theorem 1 (Multiplicativity of $\sigma_2$).** *The function $\sigma_2$ is multiplicative: if $\gcd(a, b) = 1$, then $\sigma_2(ab) = \sigma_2(a) \cdot \sigma_2(b)$.*

*Proof.* Let $a, b \in \mathbb{Z}_{>0}$ with $\gcd(a, b) = 1$. We claim the map $\varphi \colon \{d_1 : d_1 \mid a\} \times \{d_2 : d_2 \mid b\} \to \{d : d \mid ab\}$ defined by $\varphi(d_1, d_2) = d_1 d_2$ is a bijection.

*Surjectivity.* Let $d \mid ab$. Write $d = \prod_p p^{e_p}$. Since $\gcd(a, b) = 1$, each prime $p$ divides at most one of $a, b$. Set $d_1 = \prod_{p \mid a} p^{e_p}$ and $d_2 = \prod_{p \mid b} p^{e_p}$. Then $d_1 \mid a$, $d_2 \mid b$, and $d = d_1 d_2$.

*Injectivity.* If $d_1 d_2 = d_1' d_2'$ with $d_1, d_1' \mid a$ and $d_2, d_2' \mid b$, then since $\gcd(a, b) = 1$ implies $\gcd(d_1, d_2') = 1$, we have $d_1 \mid d_1'$. By symmetry $d_1' \mid d_1$, so $d_1 = d_1'$ and thus $d_2 = d_2'$.

Therefore:

$$\sigma_2(ab) = \sum_{d \mid ab} d^2 = \sum_{d_1 \mid a} \sum_{d_2 \mid b} (d_1 d_2)^2 = \left(\sum_{d_1 \mid a} d_1^2\right)\left(\sum_{d_2 \mid b} d_2^2\right) = \sigma_2(a) \cdot \sigma_2(b). \qquad \square$$

**Lemma 1 (Geometric sum for prime powers).** *For a prime $p$ and integer $a \geq 0$,*

$$\sigma_2(p^a) = \sum_{j=0}^{a} p^{2j} = \frac{p^{2(a+1)} - 1}{p^2 - 1}.$$

*Proof.* The divisors of $p^a$ are precisely $1, p, p^2, \ldots, p^a$. Hence $\sigma_2(p^a) = \sum_{j=0}^{a} (p^j)^2 = \sum_{j=0}^{a} p^{2j}$. This is a finite geometric series with first term $1$, common ratio $p^2$, and $a+1$ terms, yielding the closed form $(p^{2(a+1)} - 1)/(p^2 - 1)$. $\square$

**Proposition 1 (Overflow bound).** *For $n < N = 6.4 \times 10^7$, we have $\sigma_2(n) \leq \sigma_2(n) \cdot 1 < n^2 \cdot d(n)$ where $d(n)$ is the number of divisors. Since $d(n) = O(n^{\epsilon})$ for any $\epsilon > 0$, and empirically $\sigma_2(n) < 2^{63}$ for $n < 6.4 \times 10^7$, unsigned 64-bit integers suffice.*

**Theorem 2 (Divisor sieve correctness).** *Let $N \in \mathbb{Z}_{>0}$. Define the array $S[1..N-1]$ by initializing $S[n] \leftarrow 0$ for all $n$, then for each $d = 1, 2, \ldots, N-1$ and each multiple $k = d, 2d, 3d, \ldots$ with $k < N$, executing $S[k] \leftarrow S[k] + d^2$. Upon completion, $S[n] = \sigma_2(n)$ for all $1 \leq n < N$.*

*Proof.* After the sieve terminates, $S[n] = \sum_{\substack{d=1 \\ d \mid n}}^{N-1} d^2$. Since $1 \leq n < N$, every divisor $d$ of $n$ satisfies $1 \leq d \leq n < N$, so the sum ranges over exactly the divisor set of $n$. Thus $S[n] = \sum_{d \mid n} d^2 = \sigma_2(n)$. $\square$

**Lemma 2 (Perfect square detection).** *A nonnegative integer $s$ is a perfect square if and only if $\lfloor \sqrt{s} \rfloor^2 = s$. Integer square root can be computed exactly via Newton's method or built-in `isqrt`.*

## Algorithm

```
function SolveProblem211(N):
    // Phase 1: Compute sigma_2 via divisor sieve
    S[1..N-1] = array of 64-bit unsigned integers, initialized to 0
    for d = 1 to N-1:
        for k = d, 2d, 3d, ... while k < N:
            S[k] += d * d

    // Phase 2: Check perfect squares and accumulate
    total = 0
    for n = 1 to N-1:
        r = isqrt(S[n])
        if r * r == S[n]:
            total += n

    return total
```

## Complexity Analysis

**Time.** The sieve loop executes $\sum_{d=1}^{N-1} \lfloor (N-1)/d \rfloor$ iterations. By the harmonic number estimate $\sum_{d=1}^{N} 1/d = \ln N + \gamma + O(1/N)$, this sum equals $N \ln N + O(N)$. The perfect-square check loop runs in $\Theta(N)$. Total time complexity: $\Theta(N \log N)$.

**Space.** The array $S$ stores $N$ unsigned 64-bit values, requiring $8N$ bytes. For $N = 6.4 \times 10^7$, this is approximately 512 MB. Auxiliary space is $O(1)$. Total: $\Theta(N)$.

## Answer

$$\boxed{1922364685}$$

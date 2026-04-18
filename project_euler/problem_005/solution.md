# Problem 5: Smallest Multiple

## Problem Statement

Determine the smallest positive integer divisible by every integer from 1 to 20. Formally, compute
$$L = \operatorname{lcm}(1, 2, 3, \ldots, 20).$$

## Mathematical Development

### Definitions and Notation

**Definition 1.** For a prime $p$ and a positive integer $n$, the *$p$-adic valuation* $v_p(n)$ is the largest exponent $e \ge 0$ such that $p^e \mid n$. By convention, $v_p(0) = +\infty$.

**Definition 2.** The *least common multiple* of positive integers $a_1, \ldots, a_k$ is the smallest positive integer $m$ such that $a_i \mid m$ for all $1 \le i \le k$. Equivalently, $v_p(\operatorname{lcm}(a_1, \ldots, a_k)) = \max_i v_p(a_i)$ for every prime $p$.

**Theorem 1** (Prime factorization of the LCM). *For any positive integer $N$,*
$$\operatorname{lcm}(1, 2, \ldots, N) = \prod_{\substack{p \le N \\ p \text{ prime}}} p^{\lfloor \log_p N \rfloor}.$$

*Proof.* Let $L = \prod_{p \le N,\, p \text{ prime}} p^{\lfloor \log_p N \rfloor}$. We must show (i) $L$ is a common multiple of $\{1, \ldots, N\}$, and (ii) $L$ is minimal among all such common multiples.

*(i) $L$ is a common multiple.* Let $m$ be any integer with $1 \le m \le N$, and let $p$ be any prime dividing $m$. Then $p^{v_p(m)} \mid m \le N$, so $v_p(m) \le \log_p N$, whence $v_p(m) \le \lfloor \log_p N \rfloor = v_p(L)$. Since this holds for every prime $p$, we have $m \mid L$.

*(ii) $L$ is minimal.* Suppose $L'$ is a common multiple of $\{1, \ldots, N\}$ with $L' < L$. Then there exists a prime $p$ with $v_p(L') < \lfloor \log_p N \rfloor$. Consider the integer $q = p^{\lfloor \log_p N \rfloor}$. By definition, $\lfloor \log_p N \rfloor$ is the largest integer $e$ with $p^e \le N$, so $q \le N$. Since $L'$ is a common multiple of $\{1, \ldots, N\}$, we require $q \mid L'$, giving $v_p(L') \ge \lfloor \log_p N \rfloor$. This contradicts $v_p(L') < \lfloor \log_p N \rfloor$. $\square$

**Lemma 1** (GCD-LCM identity). *For positive integers $a$ and $b$,*
$$\operatorname{lcm}(a, b) = \frac{a \cdot b}{\gcd(a, b)}.$$

*Proof.* For every prime $p$, write $\alpha = v_p(a)$ and $\beta = v_p(b)$. Then:
$$v_p\!\left(\frac{ab}{\gcd(a,b)}\right) = \alpha + \beta - \min(\alpha, \beta) = \max(\alpha, \beta) = v_p(\operatorname{lcm}(a,b)),$$
where the second equality uses the identity $x + y - \min(x,y) = \max(x,y)$, valid for all $x, y \in \mathbb{R}$. Since the $p$-adic valuations agree for every prime $p$, the two positive integers are equal. $\square$

**Lemma 2** (Iterative reduction). *Define $L_1 = 1$ and $L_k = \operatorname{lcm}(L_{k-1}, k)$ for $k = 2, 3, \ldots, N$. Then $L_N = \operatorname{lcm}(1, 2, \ldots, N)$.*

*Proof.* By induction on $N$.

*Base case:* $L_1 = 1 = \operatorname{lcm}(1)$.

*Inductive step:* Assume $L_{k-1} = \operatorname{lcm}(1, \ldots, k-1)$. Then
$$L_k = \operatorname{lcm}(L_{k-1}, k) = \operatorname{lcm}\!\bigl(\operatorname{lcm}(1, \ldots, k-1),\, k\bigr).$$
Since the LCM operation is associative (which follows from the characterization $v_p(\operatorname{lcm}(a_1, \ldots, a_m)) = \max_i v_p(a_i)$), this equals $\operatorname{lcm}(1, \ldots, k)$. $\square$

**Theorem 2** (Explicit computation for $N = 20$). *We have $\operatorname{lcm}(1, 2, \ldots, 20) = 232{,}792{,}560$.*

*Proof.* The primes up to 20 are $\{2, 3, 5, 7, 11, 13, 17, 19\}$. By Theorem 1,

$$L = \prod_{p \le 20} p^{\lfloor \log_p 20 \rfloor}.$$

We compute each prime power:

| Prime $p$ | $\lfloor \log_p 20 \rfloor$ | Justification | $p^{\lfloor \log_p 20 \rfloor}$ |
|---|---|---|---|
| 2 | 4 | $2^4 = 16 \le 20 < 32 = 2^5$ | 16 |
| 3 | 2 | $3^2 = 9 \le 20 < 27 = 3^3$ | 9 |
| 5 | 1 | $5^1 = 5 \le 20 < 25 = 5^2$ | 5 |
| 7 | 1 | $7^1 = 7 \le 20 < 49 = 7^2$ | 7 |
| 11 | 1 | $11^1 = 11 \le 20 < 121 = 11^2$ | 11 |
| 13 | 1 | $13^1 = 13 \le 20 < 169 = 13^2$ | 13 |
| 17 | 1 | $17^1 = 17 \le 20 < 289 = 17^2$ | 17 |
| 19 | 1 | $19^1 = 19 \le 20 < 361 = 19^2$ | 19 |

Therefore:
$$L = 16 \times 9 \times 5 \times 7 \times 11 \times 13 \times 17 \times 19.$$

Step-by-step evaluation:
$$16 \times 9 = 144, \quad 144 \times 5 = 720, \quad 720 \times 7 = 5{,}040,$$
$$5{,}040 \times 11 = 55{,}440, \quad 55{,}440 \times 13 = 720{,}720,$$
$$720{,}720 \times 17 = 12{,}252{,}240, \quad 12{,}252{,}240 \times 19 = 232{,}792{,}560. \quad\square$$

## Editorial

We build the least common multiple incrementally. Starting from $L = 1$, we traverse $k = 2, 3, \ldots, N$, compute $g = \gcd(L, k)$, and update $L$ to $(L / g)k$, which equals $\operatorname{lcm}(L, k)$. This is sufficient because after each step the accumulator is the least common multiple of all integers seen so far, so the final value is $\operatorname{lcm}(1, 2, \ldots, N)$.

**Remark.** The division by $g = \gcd(L,k)$ is exact by Lemma 1, because

$$\operatorname{lcm}(L,k) = \frac{Lk}{\gcd(L,k)}.$$

Computing `L <- (L / g) * k` is therefore mathematically sound and also reduces the risk of overflow in fixed-precision arithmetic.

**Theorem 3** (Algorithm correctness). *`SmallestMultiple(N)` returns $\operatorname{lcm}(1,2,\ldots,N)$.*

*Proof.* Let $L_k$ denote the value stored in `L` after the iteration with index $k$. Initially $L_1 = 1$. At iteration $k \ge 2$, the update rule sets

$$L_k = \frac{L_{k-1} \cdot k}{\gcd(L_{k-1}, k)} = \operatorname{lcm}(L_{k-1}, k)$$

by Lemma 1. By Lemma 2, this implies

$$L_k = \operatorname{lcm}(1,2,\ldots,k)$$

for every $k$. In particular, after the final iteration,

$$L_N = \operatorname{lcm}(1,2,\ldots,N).$$

Thus the algorithm is correct. $\square$

## Pseudocode

```text
Algorithm: Least Common Multiple of an Initial Segment
Require: An integer N ≥ 1.
Ensure: L = lcm(1, 2, ..., N).
1: Initialize L ← 1.
2: For each k in {2, 3, ..., N} do:
3:     Compute g ← gcd(L, k) and update L ← (L / g) · k.
4: Return L.
```

## Complexity Analysis

**Theorem 4** (Complexity). *Assuming machine-word arithmetic is constant time for the values encountered, `SmallestMultiple(N)` runs in $O(N \log N)$ time and $O(1)$ space.*

*Proof.* The loop executes exactly $N-1$ iterations. In the $k$-th iteration, the algorithm computes $\gcd(L_{k-1}, k)$. By the Euclidean algorithm, this requires $O(\log k)$ divisions because the running time depends on the smaller argument, namely $k$.

Therefore the total running time is

$$\sum_{k=2}^{N} O(\log k) = O(N \log N).$$

The algorithm stores only the variables `L`, `g`, and `k`, so the extra space is $O(1)$. For the present problem, $N = 20$ and the final value $232{,}792{,}560$ fits comfortably in a standard 64-bit integer. $\square$

## Answer

$$\boxed{232792560}$$

# Problem 3: Largest Prime Factor

## Problem Statement

What is the largest prime factor of the number $600\,851\,475\,143$?

## Mathematical Development

### Fundamental Theorems

**Definition 1.** An integer $p \ge 2$ is *prime* if its only positive divisors are 1 and $p$. An integer $n \ge 2$ that is not prime is *composite*.

**Theorem 1 (Fundamental Theorem of Arithmetic).** Every integer $n \ge 2$ has a unique factorization (up to order) as a product of primes:

$$n = p_1^{a_1} p_2^{a_2} \cdots p_r^{a_r}$$

where $p_1 < p_2 < \cdots < p_r$ are primes and $a_i \ge 1$.

*We take this as an axiom of $\mathbb{Z}$ (provable from the well-ordering principle via Bezout's identity). The results below are self-contained.*

**Theorem 2 (Smallest Divisor is Prime).** If $n \ge 2$ is an integer and $d$ is the smallest integer $\ge 2$ that divides $n$, then $d$ is prime.

**Proof.** Suppose for contradiction that $d$ is composite. Then there exist integers $a, b$ with $1 < a < d$ and $1 < b$ such that $d = ab$. Since $a \mid d$ and $d \mid n$, transitivity of divisibility gives $a \mid n$. But $2 \le a < d$, contradicting the minimality of $d$. $\square$

**Theorem 3 (Square Root Bound for Composite Numbers).** If $n \ge 2$ is composite, then $n$ has a prime factor $p$ satisfying $p \le \lfloor\sqrt{n}\rfloor$.

**Proof.** Since $n$ is composite, write $n = ab$ with integers $1 < a \le b < n$ (choosing $a \le b$ without loss of generality). Then $a^2 \le a \cdot b = n$, so $a \le \sqrt{n}$. Let $p$ be the smallest divisor of $a$ with $p \ge 2$. By Theorem 2, $p$ is prime. Since $p \mid a$ and $a \mid n$, we have $p \mid n$. Moreover, $p \le a \le \sqrt{n}$, so $p \le \lfloor\sqrt{n}\rfloor$. $\square$

**Corollary 1 (Primality Test).** An integer $n \ge 2$ is prime if and only if no prime $p \le \lfloor\sqrt{n}\rfloor$ divides $n$.

**Proof.** The forward direction is trivial (a prime has no divisors other than 1 and itself). The contrapositive of the reverse direction is Theorem 3. $\square$

### Trial Division

**Definition 2 (Trial Division Procedure).** Given input $n \ge 2$, the *trial division algorithm* operates as follows:

1. Initialize $d \leftarrow 2$.
2. While $d^2 \le n$:
   - (a) While $d \mid n$: replace $n \leftarrow n/d$ and record $d$ as a prime factor.
   - (b) Set $d \leftarrow d + 1$.
3. If $n > 1$, record $n$ as a prime factor.
4. Return the largest recorded prime factor.

**Theorem 4 (Correctness of Trial Division).** The trial division procedure produces the complete prime factorization of the input $n$, and in particular correctly identifies its largest prime factor.

**Proof.** Let $n_0$ denote the original input. We maintain the following loop invariant:

> **Invariant $\mathcal{I}(d)$:** The current value of $n$ satisfies $n \ge 1$, $n_0 = n \cdot Q$ where $Q$ is the product of all factors extracted so far, and $n$ has no prime factor strictly less than $d$.

*Initialization.* At $d = 2$, no factors have been extracted ($Q = 1$, $n = n_0$), and the condition "no prime factor $< 2$" is vacuous. So $\mathcal{I}(2)$ holds.

*Maintenance.* Suppose $\mathcal{I}(d)$ holds at the start of an iteration.

- *Case $d \mid n$:* By $\mathcal{I}(d)$, $n$ has no prime factor less than $d$, so the smallest divisor of $n$ that is $\ge 2$ is at least $d$. Since $d \mid n$ and $d \ge 2$, the smallest divisor of $n$ is at most $d$, hence it equals $d$. By Theorem 2, $d$ is prime. We divide $n$ by $d$ (possibly multiple times), extracting all powers of $d$. After this, $d \nmid n$, and no prime factor of $n$ is $\le d$.

- *Case $d \nmid n$:* The invariant is unaffected. We increment $d$.

In either case, after incrementing $d$ to $d+1$, invariant $\mathcal{I}(d+1)$ holds.

*Termination.* The loop exits when $d^2 > n$. At this point, $\mathcal{I}(d)$ holds: $n$ has no prime factor less than $d$. If $n > 1$, then $n$ is prime, because if $n$ were composite, Theorem 3 would guarantee a prime factor $p \le \sqrt{n} < d$, contradicting $\mathcal{I}(d)$. So $n$ is the final (and largest) prime factor.

*Completeness.* Every prime factor of $n_0$ is either extracted in step 2(a) or is the remaining $n$ in step 3. The largest among these is the answer. $\square$

**Theorem 5 (Termination).** The trial division procedure terminates for all inputs $n \ge 2$.

**Proof.** Define the measure function $\mu = n + (\lfloor\sqrt{n}\rfloor - d)$. In step 2(a), each division reduces $n$ by a factor of at least 2, so $n$ strictly decreases. In step 2(b), $d$ increases by 1. In both cases, $\mu$ strictly decreases. Since $\mu$ is a non-negative integer (the loop condition ensures $d \le \lfloor\sqrt{n}\rfloor$), the algorithm terminates. $\square$

### Factorization of 600,851,475,143

**Lemma 1.** $600\,851\,475\,143 = 71 \times 839 \times 1471 \times 6857$.

**Proof.** We verify by direct computation:

$$71 \times 839 = 59\,569.$$

$$59\,569 \times 1471 = 87\,625\,999.$$

$$87\,625\,999 \times 6857 = 600\,851\,475\,143. \quad \checkmark$$

$\square$

**Lemma 2.** Each of $71, 839, 1471, 6857$ is prime.

**Proof.** By Corollary 1, it suffices to check that no prime $p \le \lfloor\sqrt{n}\rfloor$ divides $n$ for each factor.

- **71:** $\lfloor\sqrt{71}\rfloor = 8$. Primes to check: $2, 3, 5, 7$. We have $71 = 2 \cdot 35 + 1$, $71 = 3 \cdot 23 + 2$, $71 = 5 \cdot 14 + 1$, $71 = 7 \cdot 10 + 1$. None divide 71. Hence 71 is prime.

- **839:** $\lfloor\sqrt{839}\rfloor = 28$. Primes to check: $2, 3, 5, 7, 11, 13, 17, 19, 23$. Since 839 is odd, not divisible by 3 ($8+3+9=20$, $3 \nmid 20$), does not end in 0 or 5, and direct verification confirms $7 \nmid 839$ ($839 = 7 \cdot 119 + 6$), $11 \nmid 839$ ($839 = 11 \cdot 76 + 3$), $13 \nmid 839$ ($839 = 13 \cdot 64 + 7$), $17 \nmid 839$ ($839 = 17 \cdot 49 + 6$), $19 \nmid 839$ ($839 = 19 \cdot 44 + 3$), $23 \nmid 839$ ($839 = 23 \cdot 36 + 11$). Hence 839 is prime.

- **1471:** $\lfloor\sqrt{1471}\rfloor = 38$. Primes to check: $2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37$. 1471 is odd; digit sum $1+4+7+1=13$, $3 \nmid 13$; does not end in 0 or 5. Checking: $7 \nmid 1471$ ($1471 = 7 \cdot 210 + 1$), $11 \nmid 1471$ ($1471 = 11 \cdot 133 + 8$), $13 \nmid 1471$ ($1471 = 13 \cdot 113 + 2$), $17 \nmid 1471$ ($1471 = 17 \cdot 86 + 9$), $19 \nmid 1471$ ($1471 = 19 \cdot 77 + 8$), $23 \nmid 1471$ ($1471 = 23 \cdot 63 + 22$), $29 \nmid 1471$ ($1471 = 29 \cdot 50 + 21$), $31 \nmid 1471$ ($1471 = 31 \cdot 47 + 14$), $37 \nmid 1471$ ($1471 = 37 \cdot 39 + 28$). Hence 1471 is prime.

- **6857:** $\lfloor\sqrt{6857}\rfloor = 82$. Primes to check: $2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79$. 6857 is odd; digit sum $6+8+5+7=26$, $3 \nmid 26$; does not end in 0 or 5. Checking representative cases: $7 \nmid 6857$ ($6857 = 7 \cdot 979 + 4$), $11 \nmid 6857$ ($6857 = 11 \cdot 623 + 4$), $13 \nmid 6857$ ($6857 = 13 \cdot 527 + 6$), $71 \nmid 6857$ ($6857 = 71 \cdot 96 + 41$), $73 \nmid 6857$ ($6857 = 73 \cdot 93 + 68$), $79 \nmid 6857$ ($6857 = 79 \cdot 86 + 63$). No prime up to 82 divides 6857. Hence 6857 is prime.

$\square$

**Theorem 6.** The largest prime factor of $600\,851\,475\,143$ is $6857$.

**Proof.** By Lemma 1, $600\,851\,475\,143 = 71 \times 839 \times 1471 \times 6857$. By Lemma 2, all four factors are prime. By the Fundamental Theorem of Arithmetic, this is the unique prime factorization. The largest factor is $6857$. $\square$

## Editorial

We remove prime factors by trial division. Starting with $d = 2$, we divide the current value of $n$ by $d$ as long as $d$ is a factor, then increment $d$ and continue while $d^2 \le n$. When the loop ends, any remaining value greater than 1 is the largest prime factor; otherwise the last divisor used is the answer. This works because every composite factor has a prime divisor at most the square root of the current remainder.

**Execution Trace for $n = 600\,851\,475\,143$:**

| Step | $d$ | Action | Remaining $n$ |
|------|-----|--------|---------------|
| 1 | 71 | $71 \mid n$; divide | $8\,462\,696\,833$ |
| 2 | 839 | $839 \mid n$; divide | $10\,086\,647$ |
| 3 | 1471 | $1471 \mid n$; divide | $6\,857$ |
| 4 | 1472 | $d^2 = 2\,166\,784 > 6857$; exit loop | $6\,857$ |

Return $n = 6857$.

## Pseudocode

```text
Algorithm: Largest Prime Factor
Require: An integer n > 1.
Ensure: The largest prime divisor of n.
1: Initialize m ← n, d ← 2, and p_max ← 1.
2: While d^2 ≤ m do:
3:     If d does not divide m, advance to the next divisor; otherwise set p_max ← d and remove the full power of d from m.
4: If m > 1, set p_max ← m.
5: Return p_max.
```

## Complexity Analysis

**Theorem 7.** Trial division runs in $O(\sqrt{n})$ time and $O(1)$ space in the worst case.

**Proof.**

*Time bound.* The outer loop increments $d$ from 2 up to at most $\lfloor\sqrt{n}\rfloor$ (where $n$ is the *current* value). We claim the total number of iterations of both the inner and outer loops combined is $O(\sqrt{n_0})$ where $n_0$ is the original input.

The outer variable $d$ increases by 1 each outer iteration and runs while $d^2 \le n \le n_0$, so there are at most $\lfloor\sqrt{n_0}\rfloor$ outer iterations. For the inner loop: each inner iteration divides $n$ by $d \ge 2$, so $n$ halves (at least) each time. Since $n$ starts at $n_0$ and each division produces a strict decrease, the total number of inner iterations across all values of $d$ is at most $\lfloor\log_2 n_0\rfloor$. The total iteration count is thus $O(\sqrt{n_0} + \log n_0) = O(\sqrt{n_0})$.

*Space bound.* The algorithm uses only the variables $d$ and $n$, which is $O(1)$ additional space.

*Tightness.* When $n_0$ is prime, the inner loop never executes, and the outer loop runs for $d = 2, 3, \ldots, \lfloor\sqrt{n_0}\rfloor$, giving $\Theta(\sqrt{n_0})$ iterations. Hence the worst-case time is $\Theta(\sqrt{n_0})$.

*Application.* For $n_0 = 600\,851\,475\,143$, the worst-case bound is $\sqrt{n_0} \approx 775\,146$ iterations. In practice, the first factor $d = 71$ is found early, reducing $n$ and the effective search space. $\square$

## Answer

$$\boxed{6857}$$

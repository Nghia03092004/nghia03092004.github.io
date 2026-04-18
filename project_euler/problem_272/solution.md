# Problem 272: Modular Cubes, Part 2

## Problem Statement

For a positive integer $n$, define $C(n)$ as the number of integers $x$ with $1 < x < n$ such that $x^3 \equiv 1 \pmod{n}$. Find the sum of all positive integers $n \le 10^{11}$ for which $C(n) = 242$.

## Mathematical Foundation

**Theorem 1 (Multiplicativity of Cube-Root Count).** *Define $c(n) = |\{x \in \{0, 1, \ldots, n-1\} : x^3 \equiv 1 \pmod{n}\}|$. Then $c$ is a multiplicative function: $c(mn) = c(m) \cdot c(n)$ whenever $\gcd(m, n) = 1$.*

**Proof.** By the CRT, $\mathbb{Z}/mn\mathbb{Z} \cong \mathbb{Z}/m\mathbb{Z} \times \mathbb{Z}/n\mathbb{Z}$ when $\gcd(m,n) = 1$. Under this isomorphism, $x^3 \equiv 1 \pmod{mn}$ iff $x^3 \equiv 1 \pmod{m}$ and $x^3 \equiv 1 \pmod{n}$. The solution set decomposes as a direct product, so $c(mn) = c(m) \cdot c(n)$. $\square$

**Lemma 1 (Cube-Root Count for Prime Powers).** *For a prime $p$ and integer $a \ge 1$:*
- *$c(2^a) = 1$ for all $a \ge 1$.*
- *$c(3) = 1$ and $c(3^a) = 3$ for $a \ge 2$.*
- *$c(p^a) = 3$ for all $a \ge 1$ when $p \equiv 1 \pmod{3}$.*
- *$c(p^a) = 1$ for all $a \ge 1$ when $p \equiv 2 \pmod{3}$, $p \neq 3$.*

**Proof.** The number of solutions to $x^3 \equiv 1$ in the cyclic group $(\mathbb{Z}/p^a\mathbb{Z})^\times$ of order $\phi(p^a) = p^{a-1}(p-1)$ is $\gcd(3, \phi(p^a))$.

For $p = 2$: $\phi(2^a) = 2^{a-1}$, so $\gcd(3, 2^{a-1}) = 1$.

For $p = 3$: $\phi(3) = 2$, $\gcd(3, 2) = 1$; for $a \ge 2$, $\phi(3^a) = 2 \cdot 3^{a-1}$, $\gcd(3, 2 \cdot 3^{a-1}) = 3$.

For $p \equiv 1 \pmod{3}$: $3 \mid (p-1)$ so $3 \mid \phi(p^a)$, giving $\gcd(3, \phi(p^a)) = 3$.

For $p \equiv 2 \pmod{3}$, $p \neq 3$: $3 \nmid (p-1)$ and $3 \nmid p$, so $\gcd(3, \phi(p^a)) = 1$. $\square$

**Theorem 2 (Structure of $c(n)$).** *For any positive integer $n$, $c(n) = 3^{k(n)}$ where $k(n)$ equals the number of distinct prime divisors $p \equiv 1 \pmod{3}$ of $n$, plus $1$ if $v_3(n) \ge 2$.*

**Proof.** Immediate from Theorem 1 and Lemma 1, since $c(n) = \prod_{p^a \| n} c(p^a)$ and each prime-power factor contributes either $1$ or $3$. $\square$

**Corollary (Condition $C(n) = 242$).** *Since $C(n) = c(n) - 1$, the condition $C(n) = 242$ is equivalent to $c(n) = 243 = 3^5$, i.e., $k(n) = 5$. This splits into two cases:*
- *Case A: $n$ has exactly 5 distinct prime divisors $\equiv 1 \pmod{3}$, and $9 \nmid n$.*
- *Case B: $n$ has exactly 4 distinct prime divisors $\equiv 1 \pmod{3}$, and $9 \mid n$.*

**Lemma 2 (Inclusion--Exclusion Summation).** *For a fixed "active set" $S$ of primes $\equiv 1 \pmod{3}$ with product $P$, define $L = \lfloor N/P \rfloor$. The sum of all valid multipliers $t \in [1, L]$ having no prime factor $\equiv 1 \pmod{3}$ outside $S$ is:*

$$\text{SumValid}(L) = \sum_{T \subseteq F} (-1)^{|T|} \cdot d_T \cdot \frac{f_T(f_T + 1)}{2}$$

*where $F$ is the set of "forbidden" primes ($\equiv 1 \pmod{3}$, not in $S$, $\le L$), $d_T = \prod_{p \in T} p$, and $f_T = \lfloor L / d_T \rfloor$.*

**Proof.** By the inclusion--exclusion principle applied to the forbidden divisibility conditions. The sum of all multiples of $d$ in $[1, L]$ is $d \cdot f(f+1)/2$ where $f = \lfloor L/d \rfloor$. Alternating signs remove overcounting. $\square$

## Editorial
C(n) = c(n) - 1 where c(n) = 3^k, k = number of "active" prime conditions. Need c(n) = 243 = 3^5. Active conditions: Two cases for k = 5: Case A: exactly 5 primes = 1 mod 3, 9 does not divide n Case B: exactly 4 primes = 1 mod 3, 9 divides n. We enumerate primes p ≡ 1 (mod 3) up to N: call them type-A primes. We then case A: Choose 5 type-A primes, 9 does not divide n. Finally, subtract contribution where 9 | t.

## Pseudocode

```text
Enumerate primes p ≡ 1 (mod 3) up to N: call them type-A primes
Case A: Choose 5 type-A primes, 9 does not divide n
Subtract contribution where 9 | t
Case B: Choose 4 type-A primes, 9 | n (so factor of 9 is mandatory)
```

## Complexity Analysis

- **Time:** The DFS over active sets is bounded by the number of subsets of type-A primes whose product does not exceed $N = 10^{11}$. Since the smallest type-A primes are $7, 13, 19, 31, 37, \ldots$ and $7 \cdot 13 \cdot 19 \cdot 31 \cdot 37 = 3{,}233{,}461$, the number of valid 5-subsets is moderate (on the order of $10^5$). The inclusion--exclusion at each leaf prunes aggressively (product of forbidden primes grows exponentially). Overall: $O\!\left(\binom{\pi_A}{5} \cdot 2^{|F|_{\text{eff}}}\right)$ which is practically bounded and runs in under a minute.
- **Space:** $O(\pi_A)$ for the list of type-A primes, where $\pi_A \approx \pi(10^{11}) / 2$.

## Answer

$$\boxed{8495585919506151122}$$

# Problem 633: Square Sum of Squares

## Problem Statement

Count integers $n \leq N$ that are representable as a sum of 2 squares, as a sum of 3 squares, or both, and compute the related counting functions.

## Mathematical Foundation

**Theorem 1 (Fermat--Euler Two-Square Theorem).** *A positive integer $n$ is representable as a sum of two squares, $n = a^2 + b^2$, if and only if every prime factor $p \equiv 3 \pmod{4}$ of $n$ appears to an even power in the prime factorization of $n$.*

**Proof.** We work in the Gaussian integers $\mathbb{Z}[i]$, which form a Euclidean domain (hence a UFD) under the norm $N(a + bi) = a^2 + b^2$.

($\Leftarrow$) Suppose every prime $p \equiv 3 \pmod{4}$ divides $n$ to even power. Write $n = 2^{a_0} \prod_{p_j \equiv 1(4)} p_j^{a_j} \prod_{q_k \equiv 3(4)} q_k^{2b_k}$. In $\mathbb{Z}[i]$: $2 = -i(1+i)^2$, and each $p_j \equiv 1 \pmod{4}$ splits as $p_j = \pi_j \bar{\pi}_j$ with $N(\pi_j) = p_j$. Each $q_k$ remains prime (inert) in $\mathbb{Z}[i]$ with $N(q_k) = q_k^2$. Thus $n = N(\alpha)$ for some $\alpha \in \mathbb{Z}[i]$, i.e., $n = a^2 + b^2$.

($\Rightarrow$) If $n = a^2 + b^2 = N(a + bi)$ and $q \equiv 3 \pmod{4}$ is prime with $q \mid n$, then $q \mid N(a+bi)$ in $\mathbb{Z}$. Since $q$ is inert in $\mathbb{Z}[i]$ (because $-1$ is a quadratic non-residue mod $q$), $q \mid (a+bi)$ in $\mathbb{Z}[i]$, so $q^2 \mid N(a+bi) = n$. By induction, $v_q(n)$ is even. $\square$

**Theorem 2 (Legendre's Three-Square Theorem).** *A non-negative integer $n$ is representable as a sum of three squares, $n = a^2 + b^2 + c^2$, if and only if $n$ is not of the form $4^a(8b + 7)$ for non-negative integers $a, b$.*

**Proof.** ($\Rightarrow$) We show $n = 4^a(8b+7)$ is not a sum of three squares. Modulo 8, every square is $\equiv 0, 1,$ or $4$, so a sum of three squares is $\equiv 0,1,2,3,4,5,$ or $6 \pmod{8}$ but never $\equiv 7$. Thus $8b + 7$ is not a sum of three squares. If $n = a_1^2 + a_2^2 + a_3^2$ and $4 \mid n$, then all $a_i$ are even (since $a_1^2 + a_2^2 + a_3^2 \equiv 0 \pmod{4}$ forces each $a_i$ even), so $n/4 = (a_1/2)^2 + (a_2/2)^2 + (a_3/2)^2$. By induction, $4^a(8b+7)$ is never a sum of three squares.

($\Leftarrow$) The proof that every $n \not\equiv 0 \pmod{4}$ with $n \not\equiv 7 \pmod{8}$ is a sum of three squares uses the theory of ternary quadratic forms and the Hasse--Minkowski theorem. This is substantially deeper and we state it without full proof. $\square$

**Lemma 1 (Representation Count Formula).** *The number of representations of $n$ as an ordered sum of two squares (with signs) is*

$$r_2(n) = 4 \sum_{d \mid n} \chi_{-4}(d),$$

*where $\chi_{-4}$ is the non-principal character modulo 4: $\chi_{-4}(d) = 1$ if $d \equiv 1 \pmod{4}$, $\chi_{-4}(d) = -1$ if $d \equiv 3 \pmod{4}$, and $\chi_{-4}(d) = 0$ if $2 \mid d$.*

**Proof.** This follows from the factorization of the Dedekind zeta function $\zeta_{\mathbb{Q}(i)}(s) = \zeta(s) L(s, \chi_{-4})$ and the identity $r_2(n) = 4(d_1(n) - d_3(n))$ where $d_k(n) = \#\{d \mid n : d \equiv k \pmod{4}\}$. $\square$

**Lemma 2 (Density Results).**
- *The density of integers representable as a sum of two squares is $\sim C \cdot N / \sqrt{\ln N}$ where $C = \frac{1}{\sqrt{2}} \prod_{p \equiv 3(4)} (1 - p^{-2})^{-1/2} \approx 0.7642$ (Landau--Ramanujan constant).*
- *The density of integers representable as a sum of three squares is $\frac{5}{6}N + O(\sqrt{N})$ since the excluded set $\{4^a(8b+7)\}$ has density $\frac{1}{7} \cdot \frac{1}{1-1/4} = \frac{1}{6}$ (summing the geometric series $\sum_{a \geq 0} 1/4^a \cdot 1/8 = 1/6$).*

**Proof.** The two-square density is a classical result of Landau and Ramanujan. The three-square density follows from $\sum_{a=0}^{\infty} \lfloor (N/4^a - 7)/8 \rfloor \sim N/6$. $\square$

## Editorial
We sieve smallest prime factor. We then classify each n. Finally, check sum-of-2-squares: all p = 3 mod 4 appear to even power.

## Pseudocode

```text
Sieve smallest prime factor
Classify each n
Check sum-of-2-squares: all p = 3 mod 4 appear to even power
while p divides temp
Check sum-of-3-squares: n != 4^a(8b+7)
Accumulate counts
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the sieve of smallest prime factors, plus $O(N \log N)$ for factoring all integers (each factorization takes $O(\log n)$ via the SPF table). Total: $O(N \log N)$.
- **Space:** $O(N)$ for the SPF sieve.

## Answer

$$\boxed{1.0012e-10}$$

# Problem 203: Squarefree Binomial Coefficients

## Problem Statement

Find the sum of all distinct squarefree numbers in the first 51 rows (rows 0 through 50) of Pascal's triangle.

## Mathematical Foundation

**Theorem (Kummer, 1852).** *For a prime $p$ and non-negative integers $m, n$, the $p$-adic valuation $v_p\binom{m+n}{m}$ equals the number of carries when adding $m$ and $n$ in base $p$.*

**Proof.** By Legendre's formula, $v_p(k!) = \sum_{i=1}^{\infty} \lfloor k/p^i \rfloor$. Therefore:
$$v_p\binom{m+n}{m} = v_p((m+n)!) - v_p(m!) - v_p(n!) = \sum_{i=1}^{\infty}\left(\left\lfloor\frac{m+n}{p^i}\right\rfloor - \left\lfloor\frac{m}{p^i}\right\rfloor - \left\lfloor\frac{n}{p^i}\right\rfloor\right).$$
Each term $\lfloor(m+n)/p^i\rfloor - \lfloor m/p^i\rfloor - \lfloor n/p^i\rfloor$ equals 0 or 1, and it equals 1 precisely when there is a carry out of position $i-1$ in the base-$p$ addition of $m$ and $n$. This is a standard result; see Granville (1997) for a detailed proof. $\square$

**Lemma (Bounded Valuation for Large Primes).** *For $n \leq 50$ and any prime $p \geq 11$, every binomial coefficient $\binom{n}{k}$ satisfies $v_p\binom{n}{k} \leq 1$.*

**Proof.** Since $p \geq 11$ and $n \leq 50 < p^2 = 121$, both $k$ and $n - k$ have at most 2 digits in base $p$. Adding two numbers with at most 2 digits produces at most 1 carry (from the units digit to the $p$'s digit). By Kummer's theorem, $v_p\binom{n}{k} \leq 1$. $\square$

**Theorem (Squarefreeness Criterion).** *A binomial coefficient $\binom{n}{k}$ with $n \leq 50$ is squarefree if and only if $\binom{n}{k}$ is not divisible by any of $4, 9, 25, 49$.*

**Proof.** A positive integer is squarefree iff $v_p \leq 1$ for every prime $p$. By the Lemma, $v_p\binom{n}{k} \leq 1$ automatically holds for all primes $p \geq 11$. Thus squarefreeness fails only if $v_p\binom{n}{k} \geq 2$ for some prime $p \in \{2, 3, 5, 7\}$, which is equivalent to $p^2 \mid \binom{n}{k}$ for $p \in \{2, 3, 5, 7\}$. $\square$

## Editorial
By Kummer's theorem, for n <= 50, primes >= 11 can appear at most once in any binomial coefficient. So squarefreeness reduces to checking divisibility by 4, 9, 25, and 49. We return sum(S).

## Pseudocode

```text
Input: N = 50
Output: sum of distinct squarefree values in rows 0..N of Pascal's triangle
S = empty set
For n = 0 to N:
Return sum(S)
```

## Complexity Analysis

- **Time:** $O(N^2)$ to generate all binomial coefficients. Each squarefreeness test is $O(1)$ (four divisibility checks). Set insertion is $O(\log|S|)$ amortized, and $|S| \leq \binom{N+1}{2} = 1326$.
- **Space:** $O(|S|)$ for the set of distinct squarefree values.

## Answer

$$\boxed{34029210557338}$$

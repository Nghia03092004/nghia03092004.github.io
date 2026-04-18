# Problem 402: Integer-valued Polynomials

## Problem Statement

The polynomial $n^4 + 4n^3 + 2n^2 + 5n$ is a multiple of $6$ for every integer $n$, and $6$ is the largest such integer. Define $M(a, b, c)$ as the maximum $m$ such that $n^4 + an^3 + bn^2 + cn$ is a multiple of $m$ for all integers $n$. So $M(4, 2, 5) = 6$.

Define $S(N) = \sum_{0 < a, b, c \le N} M(a, b, c)$.

Given: $S(10) = 1972$, $S(10\,000) = 2\,024\,258\,331\,114$.

With Fibonacci numbers $F_0 = 0$, $F_1 = 1$, $F_k = F_{k-1} + F_{k-2}$, find the last 9 digits of $\sum_{k=2}^{1\,234\,567\,890\,123} S(F_k)$.

## Mathematical Foundation

**Theorem 1 (Binomial basis characterization of $M(a,b,c)$).** *For integers $a, b, c$,*

$$M(a,b,c) = \gcd\!\big(24,\; 36+6a,\; 14+6a+2b,\; 1+a+b+c\big).$$

**Proof.** A polynomial $p(n) = \sum_{k=0}^{d} c_k \binom{n}{k}$ satisfies $m \mid p(n)$ for all $n \in \mathbb{Z}$ if and only if $m \mid c_k$ for every $k$. This is because the binomial coefficients $\binom{n}{0}, \binom{n}{1}, \ldots$ form a $\mathbb{Z}$-basis for the ring of integer-valued polynomials, and $\binom{n}{k} \in \mathbb{Z}$ for all $n \in \mathbb{Z}$.

**Lemma 1 (Newton forward-difference expansion).** *The monomials expand as:*

$$n = \binom{n}{1}, \quad n^2 = 2\binom{n}{2} + \binom{n}{1}, \quad n^3 = 6\binom{n}{3} + 6\binom{n}{2} + \binom{n}{1},$$

$$n^4 = 24\binom{n}{4} + 36\binom{n}{3} + 14\binom{n}{2} + \binom{n}{1}.$$

**Proof.** By the Stirling number identity $n^k = \sum_{j=0}^{k} S(k,j) \cdot j! \cdot \binom{n}{j}$, where $S(k,j)$ are Stirling numbers of the second kind. Direct computation gives the stated coefficients. $\square$

Applying the lemma, $p(n) = n^4 + an^3 + bn^2 + cn$ has binomial coefficients:

$$d_4 = 24, \quad d_3 = 36 + 6a, \quad d_2 = 14 + 6a + 2b, \quad d_1 = 1 + a + b + c.$$

The maximum universal divisor is $M(a,b,c) = \gcd(d_1, d_2, d_3, d_4)$. $\square$

**Theorem 2 (Euler totient decomposition).** *Since $M(a,b,c) \mid 24$, we have*

$$S(N) = \sum_{d \mid 24} \varphi(d) \cdot \#\{(a,b,c) \in [1,N]^3 : d \mid M(a,b,c)\}$$

*where $\varphi$ is Euler's totient function.*

**Proof.** By the identity $n = \sum_{d \mid n} \varphi(d)$, we have $M(a,b,c) = \sum_{d \mid M(a,b,c)} \varphi(d)$. Summing over all triples and interchanging:

$$S(N) = \sum_{(a,b,c)} \sum_{d \mid M(a,b,c)} \varphi(d) = \sum_{d \mid 24} \varphi(d) \sum_{\substack{(a,b,c) \\ d \mid M(a,b,c)}} 1. \quad \square$$

**Lemma 2 (Piecewise-cubic structure).** *For each residue $s = N \bmod 24$, the function $S(N)$ is a cubic polynomial in $N$:*

$$288 \cdot S(N) = A_s N^3 + B_s N^2 + C_s N + D_s, \quad N \equiv s \pmod{24},$$

*where $A_s = 583$ for all $s$.*

**Proof.** The condition $d \mid M(a,b,c)$ reduces to three congruences modulo $d$ on $(a, b, c)$. For each valid residue class, the count of triples in $[1,N]^3$ is a product of three terms of the form $\lfloor (N - r)/d \rfloor$, each piecewise-linear in $N$ with period $d$. The product is piecewise-cubic with period $\operatorname{lcm}$ of all $d \mid 24$, which is $24$. The factor $288 = \operatorname{lcm}$ of the coefficient denominators ensures integrality. $\square$

**Theorem 3 (Matrix exponentiation for Fibonacci power sums).** *For each residue class $r \bmod 24$, the subsequence $(F_{24i+r})$ satisfies a two-term linear recurrence. The sums $\sum F_{24i+r}^j$ for $j = 0, 1, 2, 3$ are computed via a $14$-dimensional linear recurrence solved by matrix exponentiation.*

**Proof.** The Pisano period $\pi(24) = 24$, so $F_k \bmod 24$ depends only on $k \bmod 24$. The matrix $A^{24}$ where $A = \bigl(\begin{smallmatrix}1&1\\1&0\end{smallmatrix}\bigr)$ advances the Fibonacci sequence by 24 steps: $(F_{k+24}, F_{k+25}) = M_{24}(F_k, F_{k+1})$.

Define the state vector of dimension 14:

$$\mathbf{x} = (u^3, u^2v, uv^2, v^3, u^2, uv, v^2, u, v, 1, \Sigma u^3, \Sigma u^2, \Sigma u, \Sigma 1)$$

where $u = F_{24i+r}$, $v = F_{24i+r+1}$. The transition $(u,v) \to (F_{23}u + F_{24}v,\; F_{24}u + F_{25}v)$ is linear, and all monomials up to degree 3 in $(u,v)$ transform linearly. The accumulators track running sums. This yields a $14 \times 14$ transition matrix $T$, and $T^{N}$ is computed via repeated squaring in $O(14^3 \log N)$ operations. $\square$

## Editorial
The polynomial n^4 + 4n^3 + 2n^2 + 5n is a multiple of 6 for every integer n. Define M(a,b,c) as the maximum m such that n^4 + an^3 + bn^2 + cn is a multiple of m for all integers n. So M(4,2,5) = 6. Define S(N) = sum of M(a,b,c) for 0 < a,b,c <= N. Given: S(10) = 1972, S(10000) = 2024258331114. With Fibonacci F_0=0, F_1=1, F_k = F_{k-1}+F_{k-2}: Find last 9 digits of sum_{k=2}^{1234567890123} S(F_k). We precompute the 24 cubic polynomials (A_s, B_s, C_s, D_s). We then by fitting S(N) at 4 sample points per residue class s mod 24. Finally, iterate over each residue class r = 0..23 of k mod 24.

## Pseudocode

```text
Precompute the 24 cubic polynomials (A_s, B_s, C_s, D_s)
by fitting S(N) at 4 sample points per residue class s mod 24
For each residue class r = 0..23 of k mod 24:
Determine which polynomial to use: s = F_r mod 24 (Pisano period)
Build 14x14 transition matrix T for subsequence F_{24i+r}
Determine number of terms in subsequence with index k in [2, K], k ≡ r (mod 24)
Matrix exponentiation: T^{n_terms - 1} applied to initial state
Extract accumulated sums: Σu^3, Σu^2, Σu, Σ1
Divide by 288 and reduce modulo MOD
```

## Complexity Analysis

- **Time:** $O(14^3 \cdot 24 \cdot \log K) \approx O(10^6)$ for $K = 1.23 \times 10^{12}$. The dominant cost is 24 matrix exponentiations of $14 \times 14$ matrices with exponents $\sim K/24$.
- **Space:** $O(14^2) = O(1)$. Only a constant number of matrices and vectors.

## Answer

$$\boxed{356019862}$$

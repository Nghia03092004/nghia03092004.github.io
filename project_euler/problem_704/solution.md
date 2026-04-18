# Problem 704: Factors of Two in Binomial Coefficients

## Problem Statement

Define $g(n, m)$ as the largest integer $k$ such that $2^k \mid \binom{n}{m}$. Let $F(n) = \max\{g(n, m) : 0 \le m \le n\}$ and $S(N) = \sum_{n=1}^{N} F(n)$. Given $F(10) = 3$, $F(100) = 6$, $S(100) = 389$, $S(10^7) = 203\,222\,840$, find $S(10^{16})$.

## Mathematical Foundation

**Theorem 1 (Kummer, 1852).** *The $p$-adic valuation $\nu_p\binom{n}{m}$ equals the number of carries when adding $m$ and $n - m$ in base $p$.*

**Proof.** By Legendre's formula, $\nu_p(k!) = \sum_{i=1}^{\infty}\lfloor k/p^i \rfloor$. Therefore
$$\nu_p\binom{n}{m} = \sum_{i=1}^{\infty}\left(\left\lfloor \frac{n}{p^i}\right\rfloor - \left\lfloor \frac{m}{p^i}\right\rfloor - \left\lfloor \frac{n-m}{p^i}\right\rfloor\right).$$
Each summand equals the carry out of position $i-1$ when adding $m$ and $n-m$ in base $p$. This is Kummer's classical result. $\square$

**Theorem 2 (Maximum 2-adic Valuation).** *For $n \ge 1$,*
$$F(n) = \lfloor \log_2 n \rfloor - \nu_2(n),$$
*where $\nu_2(n)$ is the 2-adic valuation of $n$ (the exponent of 2 in the factorization of $n$).*

**Proof.** Write $n$ in binary as $n = (b_L b_{L-1} \cdots b_1 b_0)_2$ where $b_L = 1$ and $L = \lfloor \log_2 n \rfloor$. Let $\nu_2(n) = t$, so bits $b_0 = b_1 = \cdots = b_{t-1} = 0$ and $b_t = 1$.

By Kummer's theorem, $\nu_2\binom{n}{m}$ counts the carries in $m + (n-m)$ in binary. A carry at position $i$ occurs when $m_i + (n-m)_i + c_{i-1} \ge 2$, where $c_{i-1}$ is the incoming carry.

*Upper bound:* The total number of carries is at most $L - t$. Position $t$ is the lowest 1-bit of $n$. At positions $0, 1, \ldots, t-1$, we have $n_i = 0$, so $m_i + (n-m)_i = m_i + (n-m)_i$ must produce digit $0$. If there is an incoming carry, the outgoing carry simply propagates but does not create a "net" carry. The bit at position $L$ is the highest bit and cannot generate a carry out. Hence at most $L - t$ positions can contribute carries.

*Achievability:* Choose $m$ such that in binary addition $m + (n-m)$, carries are generated at every position from $t$ through $L-1$. For example, take $m = (0 \, b_{L-1} \, b_{L-2} \cdots b_{t+1} \, 0 \, \underbrace{1 \cdots 1}_{t})_2$ (a specific construction that maximizes carry chain length). This produces exactly $L - t$ carries. $\square$

**Lemma 1 (Summation Decomposition).** *$S(N) = A(N) - B(N)$, where*
$$A(N) = \sum_{n=1}^{N} \lfloor \log_2 n \rfloor, \qquad B(N) = \sum_{n=1}^{N} \nu_2(n).$$

**Proof.** Immediate from $F(n) = \lfloor \log_2 n \rfloor - \nu_2(n)$ and linearity of summation. $\square$

**Lemma 2 (Closed Form for $A(N)$).** *Let $L = \lfloor \log_2 N \rfloor$. Then*
$$A(N) = (L - 2) \cdot 2^L + 2 + L(N - 2^L + 1).$$

**Proof.** For $b = 0, 1, \ldots, L-1$, the integers $n$ with $\lfloor \log_2 n \rfloor = b$ are $\{2^b, 2^b + 1, \ldots, 2^{b+1} - 1\}$, contributing $b \cdot 2^b$. The remaining integers $\{2^L, \ldots, N\}$ contribute $L(N - 2^L + 1)$. Thus:
$$A(N) = \sum_{b=0}^{L-1} b \cdot 2^b + L(N - 2^L + 1).$$
The identity $\sum_{b=0}^{L-1} b \cdot 2^b = (L-2)\cdot 2^L + 2$ follows by differentiating the geometric series $\sum x^b$ and substituting $x = 2$. $\square$

**Lemma 3 (Closed Form for $B(N)$).**
$$B(N) = \sum_{k=1}^{L} \left\lfloor \frac{N}{2^k} \right\rfloor.$$

**Proof.** Each integer $n$ contributes $\nu_2(n)$ to $B(N)$. The value $\nu_2(n) \ge k$ iff $2^k \mid n$, so $B(N) = \sum_{k=1}^{\infty} \lfloor N/2^k \rfloor$, which terminates at $k = L$. $\square$

## Editorial
We compute A(N). Finally, compute B(N). We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
Compute A(N)
Compute B(N)
```

## Complexity Analysis

- **Time:** $O(\log N)$, since the loop runs $L = \lfloor \log_2 N \rfloor$ iterations and all arithmetic is $O(1)$ (with big-integer support for $N = 10^{16}$).
- **Space:** $O(1)$.

## Answer

$$\boxed{501985601490518144}$$

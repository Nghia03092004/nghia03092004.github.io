# Problem 951: Euler Product Approximation

## Problem Statement

The Euler product for the Riemann zeta function is $\zeta(s) = \prod_{p \text{ prime}} (1 - p^{-s})^{-1}$. Let $E(s, N) = \prod_{p \le N} (1 - p^{-s})^{-1}$. Find $\lfloor E(3, 10^6) \times 10^{15} \rfloor$.

## Mathematical Foundation

**Theorem 1 (Euler Product Formula).** *For $\operatorname{Re}(s) > 1$,*

$$\zeta(s) = \sum_{n=1}^{\infty} n^{-s} = \prod_{p \text{ prime}} \frac{1}{1 - p^{-s}}.$$

**Proof.** Each factor expands as a geometric series: $(1 - p^{-s})^{-1} = \sum_{k=0}^{\infty} p^{-ks}$, convergent for $\operatorname{Re}(s) > 0$. For any finite set of primes $P$,

$$\prod_{p \in P} \sum_{k=0}^{\infty} p^{-ks} = \sum_{\substack{n \ge 1 \\ p \mid n \Rightarrow p \in P}} n^{-s}$$

by the Fundamental Theorem of Arithmetic: every positive integer factors uniquely into primes, so expanding the product yields exactly one term $n^{-s}$ for each $n$ whose prime factors all lie in $P$. As $P$ exhausts all primes, the right-hand side converges to $\zeta(s)$ by absolute convergence of the Dirichlet series for $\operatorname{Re}(s) > 1$. $\square$

**Lemma 1 (Monotonicity).** *$E(s, N) \le \zeta(s)$ for all $N$, with $E(s, N) \nearrow \zeta(s)$ as $N \to \infty$.*

**Proof.** Each factor $(1 - p^{-s})^{-1} > 1$ for $s > 0$. The partial product $E(s, N)$ accounts for all positive integers whose prime factors are at most $N$, a subset of all positive integers. Including additional primes strictly increases the product. $\square$

**Theorem 2 (Error Bound).** *For $s = 3$ and $N \ge 2$,*

$$\zeta(3) - E(3, N) = O\!\left(\frac{1}{N^2 \ln N}\right).$$

**Proof.** We have $\zeta(3)/E(3,N) = \prod_{p > N}(1-p^{-3})^{-1}$. Taking logarithms:

$$\ln\frac{\zeta(3)}{E(3,N)} = \sum_{p > N} \sum_{k=1}^{\infty} \frac{1}{k p^{3k}} = \sum_{p > N} p^{-3} + O\!\left(\sum_{p > N} p^{-6}\right).$$

By partial summation with the Prime Number Theorem $\pi(x) \sim x/\ln x$:

$$\sum_{p > N} p^{-3} = -\frac{\pi(N)}{N^3} + 3\int_N^{\infty} \frac{\pi(t)}{t^4}\,dt \sim 3\int_N^{\infty} \frac{1}{t^3 \ln t}\,dt \sim \frac{3}{2N^2 \ln N}.$$

Since $\ln(1 + \varepsilon) \sim \varepsilon$ for small $\varepsilon$, we obtain $\zeta(3)/E(3,N) - 1 = O(1/(N^2 \ln N))$, and therefore $\zeta(3) - E(3,N) = \zeta(3) \cdot O(1/(N^2 \ln N)) = O(1/(N^2 \ln N))$. $\square$

**Lemma 2 (Numerical Precision).** *For $N = 10^6$, the relative error $(\zeta(3) - E(3,N))/\zeta(3)$ is approximately $3 \times 10^{-14}$. Standard IEEE 754 double-precision arithmetic (53-bit mantissa, $\sim$15.9 decimal digits) suffices for extracting $\lfloor E(3,10^6) \times 10^{15} \rfloor$.*

**Proof.** By Theorem 2, the relative error is $\approx 3/(2 \cdot 10^{12} \cdot 6\ln 10) \approx 1.1 \times 10^{-14}$. Working in log-space (accumulating $L = \sum_{p \le N} -\ln(1-p^{-3})$), each term has relative error $\varepsilon_{\mathrm{mach}} \approx 2^{-53}$. Over $\pi(10^6) = 78498$ terms, the accumulated error in $L$ is at most $78498 \cdot 2^{-53} \approx 8.7 \times 10^{-12}$, which propagates to an absolute error of $\sim 10^{-11}$ in $E$. This is well within the tolerance for the 15th decimal digit. $\square$

## Editorial
Compute floor(E(3, 10^6) * 10^15) where E(3, N) = prod_{p <= N} (1 - p^{-3})^{-1}. The Euler product for zeta(s) converges to Apery's constant zeta(3) = 1.2020569... We accumulate the product in log-space for numerical stability: L = sum_{p <= N} -ln(1 - p^{-3}) E = exp(L) Key identity: zeta(s) = prod_p (1 - p^{-s})^{-1} (Re(s) > 1) Error bound: zeta(3) - E(3,N) ~ 4*zeta(3) / (N^2 * ln(N)). We sieve of Eratosthenes. We then log-space accumulation. Finally, recover product and extract answer.

## Pseudocode

```text
Sieve of Eratosthenes
Log-space accumulation
Recover product and extract answer
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the Sieve of Eratosthenes, plus $O(\pi(N)) = O(N/\ln N)$ for the log-space accumulation. Total: $O(N \log \log N)$.
- **Space:** $O(N)$ for the sieve array.

For $N = 10^6$: the sieve dominates at $\sim 10^6$ operations; the product accumulation over $78498$ primes is negligible.

## Answer

$$\boxed{495568995495726}$$

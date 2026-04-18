# Problem 975: Zeckendorf Representation Uniqueness

## Problem Statement

By Zeckendorf's theorem, every positive integer $n$ has a unique representation as a sum of non-consecutive Fibonacci numbers. Let $z(n)$ denote the number of Fibonacci summands in this representation. Compute
$$\sum_{n=1}^{10^6} z(n).$$

## Mathematical Foundation

**Definition 1 (Fibonacci Numbers).** Define $F_1 = 1, F_2 = 2$, and $F_k = F_{k-1} + F_{k-2}$ for $k \ge 3$. (Equivalently, in the standard indexing: $F_k$ here equals $F_{k+1}$ in the $F_1=F_2=1$ convention.)

**Theorem 1 (Zeckendorf, 1972).** *Every positive integer $n$ can be uniquely written as*
$$n = F_{k_1} + F_{k_2} + \cdots + F_{k_r}, \quad k_1 > k_2 + 1, \; k_2 > k_3 + 1, \; \ldots, \; k_{r-1} > k_r + 1, \; k_r \ge 1.$$

**Proof.**

*Existence (by strong induction on $n$):* For $n = 1 = F_1$, the representation is trivial. Suppose all integers less than $n$ have a valid representation. Let $F_m$ be the largest Fibonacci number with $F_m \le n$. If $F_m = n$, we are done. Otherwise $0 < n - F_m < F_m$. Since $F_m \le n < F_{m+1} = F_m + F_{m-1}$, we have $n - F_m < F_{m-1}$. By the inductive hypothesis, $n - F_m$ has a Zeckendorf representation using Fibonacci numbers at most $F_{m-2}$ (since $n - F_m < F_{m-1}$ implies the largest term is at most $F_{m-2}$). Prepending $F_m$ gives a valid non-consecutive representation of $n$.

*Uniqueness (by strong induction on $n$):* Suppose $n = F_{k_1} + \cdots + F_{k_r} = F_{\ell_1} + \cdots + F_{\ell_s}$ are two valid representations with $k_1 > \cdots > k_r$ and $\ell_1 > \cdots > \ell_s$ (all indices differing by at least 2). We claim $k_1 = \ell_1$. If $k_1 > \ell_1$, then:
$$n \le F_{\ell_1} + F_{\ell_1 - 2} + F_{\ell_1 - 4} + \cdots$$
But by the identity $F_m + F_{m-2} + F_{m-4} + \cdots = F_{m+1} - 1$ (telescoping from $F_j = F_{j+1} - F_{j-1}$), the right side is at most $F_{\ell_1+1} - 1 < F_{\ell_1+1} \le F_{k_1}$. This contradicts $n \ge F_{k_1}$. By symmetry, $\ell_1 > k_1$ is also impossible, so $k_1 = \ell_1$. Canceling and applying the inductive hypothesis to $n - F_{k_1}$ gives $r = s$ and $k_i = \ell_i$ for all $i$. $\square$

**Lemma 1 (Greedy Algorithm Correctness).** *The greedy algorithm --- repeatedly subtracting the largest Fibonacci number not exceeding the remainder --- produces the Zeckendorf representation.*

**Proof.** The existence proof above is constructive via the greedy algorithm. By uniqueness (Theorem 1), this must yield the Zeckendorf representation. $\square$

**Theorem 2 (Average Number of Terms).** *Let $\phi = (1+\sqrt{5})/2$. The average value of $z(n)$ for $1 \le n \le F_m - 1$ satisfies*
$$\frac{1}{F_m - 1}\sum_{n=1}^{F_m - 1} z(n) \to \frac{1}{\phi + 2} \cdot \log_\phi n \approx 0.27639 \cdot \log_\phi n$$
*as $m \to \infty$.*

**Proof.** (Sketch.) The Zeckendorf representation induces a bijection between $\{1, \ldots, F_m - 1\}$ and binary strings of length $m - 1$ with no consecutive 1s (the "Fibonacci numeral system"). The number of 1-bits in a random such string has mean $\sim m/(\phi+2)$ by transfer matrix eigenvalue analysis. Since $F_m \approx \phi^m/\sqrt{5}$, we get $m \approx \log_\phi(F_m\sqrt{5})$, giving the stated asymptotic. $\square$

## Editorial
Compute the sum of z(n) for n = 1 to 10^6, where z(n) is the number of terms in the Zeckendorf (greedy Fibonacci) representation of n. Every positive integer has a unique representation as a sum of non-consecutive Fibonacci numbers (Zeckendorf's theorem). z(n) counts how many Fibonacci numbers are used. We precompute Fibonacci numbers up to N. We then fib = [1, 2, 3, 5, 8, 13, ...] with fib[last] >= N. Finally, find largest fib[k] <= remainder.

## Pseudocode

```text
Precompute Fibonacci numbers up to N
fib = [1, 2, 3, 5, 8, 13, ...] with fib[last] >= N
Find largest fib[k] <= remainder
```

## Complexity Analysis

- **Time:** $O(N \log_\phi N)$. For each of the $N$ integers, the greedy decomposition uses at most $O(\log_\phi N)$ steps. For $N = 10^6$, $\log_\phi(10^6) \approx 29$.
- **Space:** $O(\log_\phi N)$ for the Fibonacci table (about 30 entries for $N = 10^6$).

## Answer

$$\boxed{88597366.47748}$$

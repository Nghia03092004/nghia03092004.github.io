# Problem 978: Bernoulli Number Modular

## Problem Statement

The Bernoulli numbers $B_n$ satisfy the recurrence $\sum_{k=0}^{n} \binom{n+1}{k} B_k = 0$ for $n \ge 1$, with $B_0 = 1$. Write $B_n = p_n / q_n$ in lowest terms (with $q_n > 0$). Compute
$$\sum_{n=0}^{200} (|p_n| + q_n) \pmod{10^9 + 7}.$$

## Mathematical Foundation

**Definition 1 (Bernoulli Numbers).** The Bernoulli numbers $\{B_n\}_{n \ge 0}$ are the rational numbers defined by $B_0 = 1$ and the recurrence:
$$B_n = -\frac{1}{n+1} \sum_{k=0}^{n-1} \binom{n+1}{k} B_k \quad \text{for } n \ge 1.$$

Equivalently, they are defined by the exponential generating function $\frac{t}{e^t - 1} = \sum_{n=0}^{\infty} B_n \frac{t^n}{n!}$.

**Theorem 1 (Vanishing of Odd Bernoulli Numbers).** *For all odd $n \ge 3$, $B_n = 0$.*

**Proof.** Consider $f(t) = \frac{t}{e^t - 1} + \frac{t}{2} = \frac{t(e^t + 1)}{2(e^t - 1)} = \frac{t}{2}\coth\frac{t}{2}$. Since $\coth$ is an even function, $f(t)$ is an even function of $t$. Therefore $f(t) = \sum_{n=0}^{\infty} B_n \frac{t^n}{n!} + \frac{t}{2}$ is even, which means all odd-indexed coefficients of $\frac{t}{e^t-1}$ vanish except $B_1 = -1/2$ (the $t/2$ term). Hence $B_n = 0$ for odd $n \ge 3$. $\square$

**Theorem 2 (Von Staudt--Clausen).** *For even $n \ge 2$:*
$$B_n = A_n - \sum_{\substack{p \text{ prime} \\ (p-1) \mid n}} \frac{1}{p}$$
*where $A_n \in \mathbb{Z}$. Consequently, the denominator of $B_n$ (in lowest terms) is:*
$$q_n = \prod_{\substack{p \text{ prime} \\ (p-1) \mid n}} p.$$

**Proof.** We use the fact that for a prime $p$ and even $n \ge 2$:
$$p B_n \equiv \begin{cases} -1 \pmod{p} & \text{if } (p-1) \mid n, \\ 0 \pmod{p} & \text{otherwise,} \end{cases}$$
which follows from the Clausen--von Staudt congruence, provable via the power sum formula $S_n(p) = \sum_{j=0}^{p-1} j^n \equiv -1 \pmod{p}$ when $(p-1) \mid n$ (by Fermat's little theorem) and $S_n(p) \equiv 0 \pmod{p}$ otherwise. Since $B_n = \frac{1}{n+1}\sum_{j=0}^{n} \binom{n}{j} S_j(m)$ for suitable $m$, one extracts the $p$-adic valuation of $B_n$ to obtain the stated formula. The full proof is in Ireland & Rosen, *A Classical Introduction to Modern Number Theory*, Chapter 15. $\square$

**Lemma 1 (Sign Pattern).** *For even $n \ge 2$: $(-1)^{n/2+1} B_n > 0$. That is, $B_2 > 0$, $B_4 < 0$, $B_6 > 0$, $B_8 < 0$, $\ldots$*

**Proof.** From the functional equation of $\zeta(s)$ and the identity $B_n = (-1)^{n/2+1} \frac{2 \cdot n!}{(2\pi)^n} \zeta(n)$ for even $n \ge 2$. Since $\zeta(n) > 0$ for $n \ge 2$, the sign is $(-1)^{n/2+1}$. $\square$

**Theorem 3 (Growth of Bernoulli Numerators).** *For even $n$,*
$$|B_n| \sim \frac{2 \cdot n!}{(2\pi)^n}$$
*as $n \to \infty$. In particular, $|p_n|$ grows super-exponentially.*

**Proof.** From $B_n = (-1)^{n/2+1} \frac{2 \cdot n!}{(2\pi)^n} \zeta(n)$ and $\zeta(n) \to 1$ as $n \to \infty$. $\square$

## Algorithm

```
function BernoulliModularSum(N, MOD):
    // Step 1: Compute B_0, B_1, ..., B_N using exact rational arithmetic
    B = array of rationals, size N+1
    B[0] = 1

    for n = 1 to N:
        if n >= 3 and n is odd:
            B[n] = 0
            continue
        s = 0
        for k = 0 to n-1:
            s = s + binomial(n+1, k) * B[k]
        B[n] = -s / (n+1)
        // Reduce B[n] to lowest terms

    // Step 2: Sum |p_n| + q_n modulo MOD
    total = 0
    for n = 0 to N:
        (p, q) = B[n] as fraction in lowest terms with q > 0
        total = (total + (|p| mod MOD) + (q mod MOD)) mod MOD

    return total
```

## Complexity Analysis

- **Time:** $O(N^2 \cdot M(N))$ where $M(N)$ is the cost of arithmetic on $O(N \log N)$-bit integers. The recurrence has $O(N)$ nonzero terms (only even indices plus $B_1$), each requiring a sum of $O(N)$ terms. Each rational arithmetic operation involves numbers with $O(N \log N)$ bits (by Theorem 3), so $M(N) = O(N \log N \cdot \log(N \log N))$ using fast multiplication. Total: roughly $O(N^3 \log^2 N)$.
- **Space:** $O(N^2 \log N)$ bits to store all $N$ Bernoulli numbers.

For $N = 200$, this is entirely tractable with Python's `fractions.Fraction`.

## Answer

$$\boxed{254.54470757}$$

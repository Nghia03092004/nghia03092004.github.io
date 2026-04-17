# Problem 183: Maximum Product of Parts

## Problem Statement

For a positive integer $N$, partition $N$ into $k$ equal real-valued parts of size $N/k$, yielding product $(N/k)^k$. Let $M(N)$ be the maximum of $(N/k)^k$ over all positive integers $k$. Define
$$D(N) = \begin{cases} -N & \text{if } M(N) \text{ is a terminating decimal,} \\ +N & \text{otherwise.} \end{cases}$$
Find $\displaystyle\sum_{N=5}^{10000} D(N)$.

## Mathematical Development

**Theorem 1 (Continuous Optimum).** Define $g(t) = t \ln(N/t)$ for $t > 0$. Then $g$ has a unique global maximum at $t^* = N/e$.

*Proof.* Differentiating: $g'(t) = \ln(N/t) - 1$. Setting $g'(t) = 0$ yields $\ln(N/t) = 1$, hence $t = N/e$. The second derivative $g''(t) = -1/t < 0$ for all $t > 0$, confirming strict concavity. A strictly concave function on $(0, \infty)$ has at most one critical point, which must be the global maximum. $\square$

**Corollary 1 (Discrete Optimum).** The optimal integer $k^*$ satisfying $\max_{k \in \mathbb{Z}_{>0}} (N/k)^k$ is either $\lfloor N/e \rfloor$ or $\lceil N/e \rceil$.

*Proof.* Since $\ln\bigl((N/k)^k\bigr) = k \ln(N/k) = g(k)$ and $g$ is strictly concave, the restriction of $g$ to positive integers attains its maximum at one of the two integers adjacent to the continuous maximizer $N/e$. $\square$

**Theorem 2 (Terminating Decimal Criterion).** A rational number $a/b$ in lowest terms (with $\gcd(a,b) = 1$ and $b > 0$) has a terminating decimal expansion if and only if $b$ has no prime factors other than $2$ and $5$.

*Proof.* The fraction $a/b$ terminates if and only if there exists $n \in \mathbb{Z}_{\geq 0}$ such that $a \cdot 10^n / b \in \mathbb{Z}$, equivalently $b \mid 10^n$. Since $10^n = 2^n \cdot 5^n$, this holds if and only if every prime divisor of $b$ lies in $\{2, 5\}$. $\square$

**Theorem 3 (Termination of $(N/k)^k$).** Let $d = \gcd(N, k)$. Then $(N/k)^k$ is a terminating decimal if and only if $k/d$ has no prime factors other than $2$ and $5$.

*Proof.* Write $N/k = (N/d)/(k/d)$ where $\gcd(N/d, k/d) = 1$, so $N/k$ is in lowest terms with denominator $k/d$. By Theorem 2, $N/k$ terminates if and only if $k/d = 2^\alpha 5^\beta$ for some $\alpha, \beta \geq 0$.

It remains to show that $(N/k)^k$ terminates if and only if $N/k$ terminates. Write $N/k = a/b$ in lowest terms. Then $(N/k)^k = a^k / b^k$ with $\gcd(a^k, b^k) = 1$ (since $\gcd(a, b) = 1$ implies $\gcd(a^k, b^k) = 1$). The denominator $b^k = (2^\alpha 5^\beta)^k = 2^{k\alpha} 5^{k\beta}$ has only factors $2$ and $5$ if and only if $b$ does. Conversely, if $b$ has a prime factor $r \notin \{2, 5\}$, then $b^k$ has the same prime factor, and $(N/k)^k$ is non-terminating. $\square$

## Algorithm

```
function ComputeDSum(N_max):
    total := 0
    for N := 5 to N_max:
        k1 := floor(N / e)
        k2 := k1 + 1
        if k1 * ln(N / k1) >= k2 * ln(N / k2):
            k_opt := k1
        else:
            k_opt := k2

        d := gcd(N, k_opt)
        denom := k_opt / d
        while denom mod 2 = 0: denom := denom / 2
        while denom mod 5 = 0: denom := denom / 5
        if denom = 1:
            total -= N
        else:
            total += N

    return total
```

## Complexity Analysis

- **Time:** $O(N_{\max} \log N_{\max})$. For each $N$, we perform $O(1)$ floating-point comparisons, one GCD computation in $O(\log N)$, and trial division by $2$ and $5$ in $O(\log N)$.
- **Space:** $O(1)$.

## Answer

$$\boxed{48861552}$$

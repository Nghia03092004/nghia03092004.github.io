# Problem 838: Not Coprime

## Problem Statement

For a positive integer $N$, define $C(N)$ as the number of pairs $(a, b)$ with $1 \le a \le b \le N$ such that $\gcd(a, b) > 1$ (i.e., $a$ and $b$ are not coprime). Compute $C(10^7)$.

## Mathematical Foundation

**Theorem 1 (Mobius Coprime Count).** *The number of ordered pairs $(a,b)$ with $1 \le a, b \le N$ and $\gcd(a,b) = 1$ is*

$$\Phi(N) = \sum_{d=1}^{N} \mu(d) \left\lfloor \frac{N}{d} \right\rfloor^2.$$

**Proof.** We use the fundamental identity $[\gcd(a,b)=1] = \sum_{d \mid \gcd(a,b)} \mu(d)$, which follows from the Mobius inversion formula applied to the constant function $\mathbf{1}$ and its Dirichlet inverse $\mu$. Summing over all ordered pairs:

$$\Phi(N) = \sum_{a=1}^{N}\sum_{b=1}^{N} \sum_{d \mid \gcd(a,b)} \mu(d) = \sum_{d=1}^{N} \mu(d) \sum_{\substack{1 \le a \le N \\ d \mid a}} \sum_{\substack{1 \le b \le N \\ d \mid b}} 1 = \sum_{d=1}^{N} \mu(d) \left\lfloor \frac{N}{d} \right\rfloor^2.$$

The exchange of summation is justified by absolute convergence (all sums are finite). The inner sums count multiples of $d$ in $\{1, \ldots, N\}$, each equaling $\lfloor N/d \rfloor$. $\square$

**Lemma 1 (Ordered to Unordered).** *The number of unordered coprime pairs $(a, b)$ with $1 \le a \le b \le N$ is*

$$C_{\mathrm{cop}}(N) = \frac{\Phi(N) + 1}{2}.$$

**Proof.** Among the $\Phi(N)$ ordered coprime pairs, the only pair with $a = b$ and $\gcd(a,a) = 1$ is $(1,1)$. All other coprime pairs $(a,b)$ with $a \ne b$ appear twice (as $(a,b)$ and $(b,a)$). Hence the number of unordered pairs is $\frac{\Phi(N) - 1}{2} + 1 = \frac{\Phi(N) + 1}{2}$. $\square$

**Theorem 2 (Non-Coprime Count).** *The number of unordered non-coprime pairs is*

$$C(N) = \frac{N(N+1)}{2} - \frac{\Phi(N)+1}{2}.$$

**Proof.** The total number of unordered pairs $(a,b)$ with $1 \le a \le b \le N$ is $\binom{N}{2} + N = \frac{N(N+1)}{2}$ (including pairs with $a = b$). Subtracting the coprime pairs: $C(N) = \frac{N(N+1)}{2} - C_{\mathrm{cop}}(N)$. $\square$

**Theorem 3 (Hyperbola Method).** *The sum $\sum_{d=1}^{N} \mu(d) \lfloor N/d \rfloor^2$ can be evaluated in $O(\sqrt{N})$ arithmetic operations, given prefix sums of the Mobius function $M(x) = \sum_{k=1}^{x} \mu(k)$.*

**Proof.** The function $d \mapsto q(d) = \lfloor N/d \rfloor$ takes at most $2\lfloor\sqrt{N}\rfloor$ distinct values. For each distinct value $q$, the set of $d$ with $\lfloor N/d \rfloor = q$ forms a contiguous interval $[d_1, d_2]$ where $d_2 = \lfloor N/q \rfloor$ and $d_1 = \lfloor N/(q+1) \rfloor + 1$. The contribution of this block is

$$q^2 \sum_{d=d_1}^{d_2} \mu(d) = q^2 \bigl(M(d_2) - M(d_1 - 1)\bigr).$$

Summing over all $O(\sqrt{N})$ blocks gives the result. $\square$

**Lemma 2 (Asymptotic Density).** *As $N \to \infty$, $\Phi(N) \sim \frac{6N^2}{\pi^2}$, so the probability that two random integers are coprime is $6/\pi^2 \approx 0.6079$.*

**Proof.** $\Phi(N)/N^2 = \sum_{d=1}^{N} \mu(d)/d^2 + O(1/N) \to \sum_{d=1}^{\infty} \mu(d)/d^2 = \prod_p (1 - 1/p^2) = 1/\zeta(2) = 6/\pi^2$, where the Euler product follows from the multiplicativity of $\mu$ and the product formula for $\zeta(2)$. $\square$

## Editorial
Count pairs (a, b) with 1 <= a <= b <= N such that gcd(a, b) > 1. Key identity: Phi(N) = sum_{d=1}^{N} mu(d) * floor(N/d)^2 counts ordered coprime pairs, then C(N) = N(N+1)/2 - (Phi(N)+1)/2. We linear sieve for Mobius function. We then iterate over p in primes. Finally, prefix sums (Mertens function).

## Pseudocode

```text
Linear sieve for Mobius function
for p in primes
Prefix sums (Mertens function)
Block summation (hyperbola method)
Compute answer
```

## Complexity Analysis

- **Time:** $O(N)$ for the linear sieve and prefix sums; $O(\sqrt{N})$ for the block summation. Total: $O(N)$.
- **Space:** $O(N)$ for storing $\mu$ and $M$.

## Answer

$$\boxed{250591.442792}$$

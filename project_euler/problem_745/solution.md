# Problem 745: Sum of Squares II

## Problem Statement

For a positive integer $n$, define $g(n)$ to be the maximum perfect square that divides $n$. For example, $g(18) = 9$, $g(19) = 1$.

Define $S(N) = \sum_{n=1}^{N} g(n)$.

Given: $S(10) = 24$, $S(100) = 767$.

Find $S(10^{14}) \bmod 1{,}000{,}000{,}007$.

## Mathematical Foundation

**Theorem 1 (Unique Squarefree Decomposition).** *Every positive integer $n$ can be written uniquely as $n = a^2 b$ where $b$ is squarefree. In this decomposition, $g(n) = a^2$.*

**Proof.** Write $n = \prod p_i^{e_i}$. Set $a = \prod p_i^{\lfloor e_i / 2 \rfloor}$ and $b = \prod p_i^{e_i \bmod 2}$. Then $n = a^2 b$, $b$ is squarefree, and $a^2$ is the largest perfect square dividing $n$. Uniqueness follows from the uniqueness of prime factorization. $\square$

**Theorem 2 (Jordan Totient Reformulation).** *Define the Jordan totient function of order $2$:*

$$J_2(e) = \sum_{a \mid e} a^2 \mu(e/a) = e^2 \prod_{p \mid e} \left(1 - \frac{1}{p^2}\right)$$

*Then:*

$$S(N) = \sum_{e=1}^{\lfloor \sqrt{N} \rfloor} J_2(e) \left\lfloor \frac{N}{e^2} \right\rfloor$$

**Proof.** From Theorem 1:

$$S(N) = \sum_{a=1}^{\lfloor \sqrt{N} \rfloor} a^2 \sum_{\substack{b \le N/a^2 \\ b \text{ squarefree}}} 1 = \sum_{a=1}^{\lfloor \sqrt{N} \rfloor} a^2 \sum_{d=1}^{\lfloor \sqrt{N/a^2} \rfloor} \mu(d) \left\lfloor \frac{N}{a^2 d^2} \right\rfloor$$

where we used the standard Mobius inversion formula $Q(m) = \sum_{d=1}^{\lfloor \sqrt{m} \rfloor} \mu(d) \lfloor m/d^2 \rfloor$ for the squarefree counting function. Substituting $e = ad$:

$$S(N) = \sum_{e=1}^{\lfloor \sqrt{N} \rfloor} \left\lfloor \frac{N}{e^2} \right\rfloor \sum_{a \mid e} a^2 \mu(e/a) = \sum_{e=1}^{\lfloor \sqrt{N} \rfloor} J_2(e) \left\lfloor \frac{N}{e^2} \right\rfloor$$

The identity $\sum_{a \mid e} a^2 \mu(e/a) = J_2(e)$ is the definition of the Jordan totient as the Dirichlet convolution $\mathrm{id}_2 * \mu$. The multiplicative formula $J_2(e) = e^2 \prod_{p \mid e}(1 - 1/p^2)$ follows from multiplicativity and evaluation at prime powers: $J_2(p^k) = p^{2k} - p^{2(k-1)}$. $\square$

**Lemma 1 (Sieve for $J_2$).** *The values $J_2(e)$ for $1 \le e \le L$ can be computed by a multiplicative sieve analogous to the Euler totient sieve:*
1. *Initialize $J_2(e) = e^2$ for all $e$.*
2. *For each prime $p \le L$: for each multiple $m$ of $p$, set $J_2(m) \leftarrow J_2(m) \cdot (p^2 - 1) / p^2$.*

*This runs in $O(L \log \log L)$ time.*

**Proof.** The sieve correctly computes $J_2(e) = e^2 \prod_{p \mid e}(1 - 1/p^2)$ by multiplying in the factor $(1 - 1/p^2) = (p^2-1)/p^2$ for each prime divisor. The time complexity is the same as the Euler sieve. The division by $p^2$ is exact in modular arithmetic since we track the factor $(p^2-1)/p^2$ as a modular inverse multiplication. $\square$

## Algorithm

```
function S(N, p):
    L = floor(sqrt(N))          // L = 10^7 for N = 10^14

    // Step 1: Sieve J_2(e) mod p for e = 1..L
    J2 = array[1..L]
    for e = 1 to L:
        J2[e] = e^2 mod p

    // Sieve: for each prime, multiply in (1 - 1/p^2) factor
    is_prime = sieve_of_eratosthenes(L)
    for each prime q in is_prime:
        inv_q2 = mod_inverse(q^2, p)
        factor = (q^2 - 1) * inv_q2 mod p
        for m = q, 2q, 3q, ..., L:
            J2[m] = J2[m] * factor mod p

    // Step 2: Compute S(N) = sum_{e=1}^{L} J2[e] * floor(N / e^2) mod p
    result = 0
    for e = 1 to L:
        result = (result + J2[e] * (N / (e * e) mod p)) mod p

    return result
```

## Complexity Analysis

- **Time:** $O(\sqrt{N} \log \log \sqrt{N})$ for the sieve, $O(\sqrt{N})$ for the final summation. Total: $O(\sqrt{N} \log \log \sqrt{N})$. For $N = 10^{14}$, $\sqrt{N} = 10^7$.
- **Space:** $O(\sqrt{N})$ for the $J_2$ array and the prime sieve.

## Answer

$$\boxed{94586478}$$

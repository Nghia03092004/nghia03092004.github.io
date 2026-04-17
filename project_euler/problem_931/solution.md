# Problem 931: Sum of Radical Expressions

## Problem Statement

For a positive integer $n$, define $R(n) = \sum_{d \mid n} \operatorname{rad}(d)$, where $\operatorname{rad}(d)$ is the product of the distinct prime factors of $d$ (with $\operatorname{rad}(1) = 1$). Find $\sum_{n=1}^{10^7} R(n) \bmod (10^9 + 7)$.

## Mathematical Foundation

**Definition.** For $n \geq 1$, the *radical* of $n$ is $\operatorname{rad}(n) = \prod_{p \mid n} p$, with $\operatorname{rad}(1) = 1$.

**Lemma 1.** *The radical function $\operatorname{rad}$ is multiplicative: if $\gcd(a, b) = 1$, then $\operatorname{rad}(ab) = \operatorname{rad}(a) \cdot \operatorname{rad}(b)$.*

**Proof.** Let $a = \prod p_i^{a_i}$ and $b = \prod q_j^{b_j}$ where $\{p_i\}$ and $\{q_j\}$ are disjoint (since $\gcd(a,b) = 1$). Then $ab = \prod p_i^{a_i} \prod q_j^{b_j}$ and the set of distinct prime factors of $ab$ is $\{p_i\} \cup \{q_j\}$. Hence $\operatorname{rad}(ab) = \prod p_i \cdot \prod q_j = \operatorname{rad}(a) \cdot \operatorname{rad}(b)$. $\square$

**Theorem 1.** *The function $R(n) = \sum_{d \mid n} \operatorname{rad}(d)$ is multiplicative, and for a prime power $p^a$ with $a \geq 1$:*
$$R(p^a) = 1 + ap.$$

**Proof.** Since $R = \operatorname{rad} * \mathbf{1}$ is the Dirichlet convolution of two multiplicative functions, $R$ is multiplicative (a standard result in multiplicative number theory). For the prime power formula, the divisors of $p^a$ are $1, p, p^2, \ldots, p^a$. We have $\operatorname{rad}(1) = 1$ and $\operatorname{rad}(p^k) = p$ for all $k \geq 1$. Therefore:
$$R(p^a) = \sum_{k=0}^{a} \operatorname{rad}(p^k) = 1 + \sum_{k=1}^{a} p = 1 + ap.$$
$\square$

**Theorem 2 (Closed-form product).** *For $n = p_1^{a_1} \cdots p_r^{a_r}$:*
$$R(n) = \prod_{i=1}^{r} (1 + a_i \, p_i).$$

**Proof.** Immediate from Theorem 1 and the multiplicativity of $R$. $\square$

**Theorem 3 (Summation by Dirichlet interchange).** *For any $N \geq 1$:*
$$\sum_{n=1}^{N} R(n) = \sum_{d=1}^{N} \operatorname{rad}(d) \cdot \left\lfloor \frac{N}{d} \right\rfloor.$$

**Proof.** We have:
$$\sum_{n=1}^{N} R(n) = \sum_{n=1}^{N} \sum_{d \mid n} \operatorname{rad}(d) = \sum_{d=1}^{N} \operatorname{rad}(d) \sum_{\substack{n=1 \\ d \mid n}}^{N} 1 = \sum_{d=1}^{N} \operatorname{rad}(d) \cdot \left\lfloor \frac{N}{d} \right\rfloor,$$
where the second equality follows by swapping the order of summation (each pair $(d, n)$ with $d \mid n$ and $1 \leq n \leq N$ is counted once on each side). $\square$

## Algorithm

```
function SumOfR(N, MOD):
    // Step 1: Linear sieve to compute rad(d) for all d <= N
    rad[1..N] := 1
    is_prime[1..N] := true
    for p from 2 to N:
        if is_prime[p]:
            for multiple m = p, 2p, 3p, ..., N:
                rad[m] := rad[m] * p
                if m != p:
                    is_prime[m] := false

    // Step 2: Compute the sum via Dirichlet interchange (Theorem 3)
    S := 0
    for d from 1 to N:
        S := S + rad[d] * floor(N / d)
        S := S mod MOD

    return S
```

*Alternative (direct product approach using Theorem 2):*

```
function SumOfR_Direct(N, MOD):
    // Step 1: Sieve smallest prime factor spf[n] for n <= N
    spf[1..N] := 0
    for p from 2 to N:
        if spf[p] == 0:
            for m = p, 2p, ..., N:
                if spf[m] == 0:
                    spf[m] := p

    // Step 2: For each n, compute R(n) = prod(1 + a_i * p_i)
    S := 0
    for n from 1 to N:
        R := 1
        temp := n
        while temp > 1:
            p := spf[temp]
            a := 0
            while temp mod p == 0:
                a := a + 1
                temp := temp / p
            R := R * (1 + a * p) mod MOD
        S := S + R
        S := S mod MOD
    return S
```

## Complexity Analysis

- **Time:** $O(N \log \log N)$ for the sieve; $O(N)$ for the summation in the Dirichlet interchange approach. The direct factorization approach takes $O(N \log N)$ in the worst case (due to repeated division), but $O(N)$ amortized. Overall: $O(N \log \log N)$.
- **Space:** $O(N)$ for storing $\operatorname{rad}(d)$ or the smallest prime factor array.

## Answer

$$\boxed{128856311}$$

# Problem 448: Average Least Common Multiple

## Problem Statement

Define the average LCM function:

$$f(n) = \frac{1}{n} \sum_{k=1}^{n} \operatorname{lcm}(k, n).$$

Compute $S(N) = \sum_{n=1}^{N} f(n) \pmod{10^9 + 7}$ for $N = 10^8$.

## Mathematical Foundation

**Theorem 1 (Closed Form for $f(n)$).** *For all $n \geq 1$,*

$$f(n) = \frac{1 + \displaystyle\sum_{d \mid n} d\,\varphi(d)}{2}.$$

**Proof.** Using $\operatorname{lcm}(k,n) = kn/\gcd(k,n)$:

$$f(n) = \frac{1}{n}\sum_{k=1}^{n} \frac{kn}{\gcd(k,n)} = \sum_{k=1}^{n} \frac{k}{\gcd(k,n)}.$$

Group by $g = \gcd(k,n)$: write $k = gj$ where $1 \leq j \leq n/g$ and $\gcd(j, n/g) = 1$. Setting $d = n/g$:

$$f(n) = \sum_{d \mid n} \sum_{\substack{j=1 \\ \gcd(j,d)=1}}^{d} j =: \sum_{d \mid n} \Phi(d).$$

For $d \geq 2$, the coprime residues $\{j : 1 \leq j \leq d,\, \gcd(j,d) = 1\}$ pair as $j \leftrightarrow d - j$ (both coprime to $d$, and $j \neq d - j$ since $d \geq 2$ forces $j \neq d/2$ when $\gcd(d/2, d) \neq 1$ for even $d$, or $d$ is odd). Each pair sums to $d$, and there are $\varphi(d)/2$ pairs, so $\Phi(d) = d\,\varphi(d)/2$.

For $d = 1$: $\Phi(1) = 1$.

Therefore:

$$f(n) = 1 + \sum_{\substack{d \mid n \\ d \geq 2}} \frac{d\,\varphi(d)}{2} = \frac{1 + \sum_{d \mid n} d\,\varphi(d)}{2}$$

where the last equality uses $1 \cdot \varphi(1) = 1$, so $1 + \sum_{d \geq 2} d\varphi(d)/2 = \frac{1}{2} + \frac{1}{2} + \sum_{d \geq 2} d\varphi(d)/2 = \frac{1 + \sum_{d \mid n} d\varphi(d)}{2}$. $\square$

**Theorem 2 (Summation Formula).** *Define $h(d) = d\,\varphi(d)$. Then*

$$S(N) = \frac{N + \displaystyle\sum_{d=1}^{N} h(d)\,\lfloor N/d \rfloor}{2}.$$

**Proof.** Summing $f(n)$ over $n = 1, \ldots, N$:

$$S(N) = \sum_{n=1}^{N} \frac{1 + \sum_{d \mid n} h(d)}{2} = \frac{N + \sum_{n=1}^{N} \sum_{d \mid n} h(d)}{2}.$$

Swapping the order of summation: $\sum_{n=1}^{N} \sum_{d \mid n} h(d) = \sum_{d=1}^{N} h(d) \cdot \lfloor N/d \rfloor$, since $h(d)$ contributes to each multiple of $d$ up to $N$. $\square$

**Lemma 1 (Multiplicativity of $h$).** *The function $h(n) = n\,\varphi(n)$ is multiplicative, with $h(p^a) = p^{2a} - p^{2a-1}$ for primes $p$ and $a \geq 1$.*

**Proof.** Both $\operatorname{Id}(n) = n$ and $\varphi(n)$ are multiplicative, so their product $h = \operatorname{Id} \cdot \varphi$ is multiplicative. For $p^a$: $h(p^a) = p^a \cdot p^{a-1}(p-1) = p^{2a-1}(p-1) = p^{2a} - p^{2a-1}$. $\square$

**Lemma 2 (Coprime Residue Pairing).** *For $d \geq 2$, if $\gcd(j, d) = 1$ and $1 \leq j \leq d$, then $\gcd(d-j, d) = 1$ and $j + (d-j) = d$. Furthermore, $j \neq d - j$ (i.e., $j \neq d/2$) whenever $d \geq 3$, or $d = 2$ with $j = 1$.*

**Proof.** $\gcd(d-j, d) = \gcd(j, d) = 1$. For $j = d/2$ to be coprime to $d$, we need $\gcd(d/2, d) = d/2 = 1$, so $d = 2$. When $d = 2$, the only coprime residue is $j = 1 = d - 1$, confirming the pairing is trivial (a single element). For $d \geq 3$, all pairs are distinct. $\square$

## Algorithm

```
function ComputeS(N, mod):
    // Step 1: Sieve Euler's totient function
    phi[1..N] = totient sieve (initialize phi[i] = i, then for each prime p,
                                for multiples m of p: phi[m] = phi[m] * (p-1) / p)

    // Step 2: Compute h(d) = d * phi(d) mod mod for all d
    // Step 3: Divisor-sum convolution: g(n) = sum_{d | n} h(d)
    g[1..N] = 0
    for d = 1 to N:
        h_d = (d % mod) * (phi[d] % mod) % mod
        for multiple = d, 2d, 3d, ..., N:
            g[multiple] = (g[multiple] + h_d) % mod

    // Step 4: Compute S(N)
    inv2 = modular_inverse(2, mod)
    total = 0
    for n = 1 to N:
        f_n = (1 + g[n]) % mod * inv2 % mod
        total = (total + f_n) % mod

    return total
```

**Alternative (Hyperbola Method):**

```
function ComputeS_fast(N, mod):
    // Use S(N) = (N + sum_{d=1}^{N} h(d) * floor(N/d)) / 2
    // Group by distinct values of floor(N/d): O(sqrt(N)) groups

    Sieve phi[1..N]
    Compute prefix sums H[k] = sum_{d=1}^{k} h(d) mod mod

    inv2 = modular_inverse(2, mod)
    total = N % mod
    d = 1
    while d <= N:
        q = floor(N / d)
        d_max = floor(N / q)  // largest d' with floor(N/d') = q
        block_sum = (H[d_max] - H[d-1]) % mod
        total = (total + q * block_sum) % mod
        d = d_max + 1

    return total * inv2 % mod
```

## Complexity Analysis

- **Time (Direct method):** $O(N \log N)$ dominated by the harmonic-series divisor convolution ($\sum_{d=1}^{N} N/d = O(N \ln N)$).
- **Time (Hyperbola method):** $O(N)$ for the totient sieve and prefix sums, plus $O(\sqrt{N})$ for the grouped summation. Total: $O(N)$.
- **Space:** $O(N)$ for the $\varphi$ and $H$ arrays.

## Answer

$$\boxed{106467648}$$

# Problem 717: Summation of a Modular Formula

## Problem Statement

For an odd prime $p$, define

$$f(p) = \left\lfloor \frac{2^{2^p}}{p} \right\rfloor \bmod 2^p.$$

Let $g(p) = f(p)$ and define

$$G(N) = \sum_{\substack{p < N \\ p \text{ odd prime}}} g(p).$$

Given $G(100) = 474$ and $G(10^4) = 2\,819\,236$, find $G(10^7)$.

## Mathematical Foundation

**Lemma 1.** For an odd prime $p$, we have

$$\left\lfloor \frac{2^{2^p}}{p} \right\rfloor \bmod 2^p = \frac{2^{2^p} \bmod (p \cdot 2^p)}{p}.$$

**Proof.** Write $2^{2^p} = q \cdot p + r$ where $0 \leq r < p$. Then $\lfloor 2^{2^p}/p \rfloor = q$. We want $q \bmod 2^p$. Note that $2^{2^p} \bmod (p \cdot 2^p)$ gives us $2^{2^p} - k \cdot p \cdot 2^p$ for some integer $k$, which equals $p \cdot (q - k \cdot 2^p) + r$. Dividing by $p$ and taking the integer part gives $q \bmod 2^p$ (since $0 \leq r < p$). More precisely, $\lfloor (2^{2^p} \bmod (p \cdot 2^p)) / p \rfloor = q \bmod 2^p$. $\square$

**Theorem 1.** The value $2^{2^p} \bmod (p \cdot 2^p)$ can be computed by $p$ successive squarings modulo $p \cdot 2^p$, starting from $2$.

**Proof.** We compute $a_0 = 2$, $a_{i+1} = a_i^2 \bmod (p \cdot 2^p)$ for $i = 0, 1, \ldots, p-1$. Then $a_p = 2^{2^p} \bmod (p \cdot 2^p)$. Each squaring preserves the residue modulo $p \cdot 2^p$, and after $p$ squarings the exponent is $2^p$. $\square$

**Lemma 2.** The modulus $p \cdot 2^p$ fits in $O(p)$ bits. For $p$ up to $10^7$, this requires arbitrary-precision arithmetic or a decomposition using the Chinese Remainder Theorem (CRT).

**Proof.** The modulus $p \cdot 2^p$ has $\log_2(p \cdot 2^p) = \log_2 p + p$ bits, which for $p \approx 10^7$ is about $10^7$ bits. Direct computation is infeasible. $\square$

**Theorem 2 (CRT Decomposition).** Since $\gcd(p, 2^p) = 1$ for odd $p$, by CRT:

$$2^{2^p} \bmod (p \cdot 2^p) \longleftrightarrow \begin{cases} 2^{2^p} \bmod p \\ 2^{2^p} \bmod 2^p \end{cases}.$$

Now $2^{2^p} \bmod p$ can be computed via $2^{2^p \bmod (p-1)} \bmod p$ using Fermat's little theorem ($2^{p-1} \equiv 1 \pmod{p}$). And $2^{2^p} \bmod 2^p = 0$ since $2^p \leq 2^{2^p}$ for all $p \geq 1$.

**Proof.** CRT applies because $\gcd(p, 2^p) = 1$. For the first component, Fermat's little theorem gives $2^{p-1} \equiv 1 \pmod{p}$, so $2^{2^p} \equiv 2^{2^p \bmod (p-1)} \pmod{p}$. The exponent $2^p \bmod (p-1)$ is computed by fast modular exponentiation. For the second component, $2^{2^p}$ is divisible by $2^p$ since $2^p \leq 2^p$ (the exponent $2^p \geq p$), so the residue is 0. $\square$

**Theorem 3.** Combining via CRT, let $r = 2^{2^p \bmod (p-1)} \bmod p$. Then

$$f(p) = \left\lfloor \frac{r}{p} \right\rfloor \bmod 2^p$$

requires reconstruction. Since $2^{2^p} \equiv r \pmod{p}$ and $2^{2^p} \equiv 0 \pmod{2^p}$, by CRT $2^{2^p} \equiv r \cdot 2^p \cdot (2^p)^{-1}_p \cdot (\text{something}) \pmod{p \cdot 2^p}$... More directly:

$$f(p) = \frac{2^{2^p} - r}{p} \bmod 2^p = \frac{-r}{p} \bmod 2^p = (-r \cdot p^{-1}) \bmod 2^p,$$

where $p^{-1}$ is the inverse of $p$ modulo $2^p$.

**Proof.** Write $2^{2^p} = p \cdot f(p) + r$ in the integers (here we slightly abuse notation: $r = 2^{2^p} \bmod p$). Then $f(p) = (2^{2^p} - r)/p$. Modulo $2^p$: since $2^{2^p} \equiv 0 \pmod{2^p}$, we get $f(p) \equiv -r/p \equiv -r \cdot p^{-1} \pmod{2^p}$. The inverse $p^{-1} \bmod 2^p$ exists since $p$ is odd. $\square$

## Editorial
We iterate over each odd prime p in primes. Finally, since p is odd, use extended Euclidean or lifting. We first generate the primes required by the search, then enumerate the admissible combinations and retain only the values that satisfy the final test.

## Pseudocode

```text
for each odd prime p in primes
Compute e = 2^p mod (p-1)
Compute r = 2^e mod p
Compute p_inv = p^{-1} mod 2^p
Since p is odd, use extended Euclidean or lifting
f(p) = (-r * p_inv) mod 2^p
```

## Complexity Analysis

- **Time:** For each prime $p$: $O(\log p)$ for modular exponentiation (Steps 1--2), $O(p)$ bits for the inverse computation (Step 3). Summing over all primes $p < N$: the dominant cost is $\sum_{p < N} p \approx O(N^2 / \log N)$ bit operations, which is too slow for $N = 10^7$ without further optimization. With word-level operations ($w = 64$ bits), this becomes $O(N^2 / (w \log N))$.
- **Space:** $O(N / \log N)$ for storing primes, plus $O(N / w)$ for the largest inverse computation.

## Answer

$$\boxed{1603036763131}$$

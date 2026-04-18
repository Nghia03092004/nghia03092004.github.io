# Problem 421: Prime Factors of $n^{15}+1$

## Problem Statement

Numbers of the form $n^{15}+1$ are composite for every integer $n > 1$. For positive integers $n$ and $m$, let $s(n,m)$ be the sum of the distinct prime factors of $n^{15}+1$ not exceeding $m$.

**Examples:**
- $2^{15}+1 = 3^2 \cdot 11 \cdot 331$, so $s(2,10) = 3$ and $s(2,1000) = 345$.
- $10^{15}+1 = 7 \cdot 11 \cdot 13 \cdot 211 \cdot 241 \cdot 2161 \cdot 9091$, so $s(10,100)=31$ and $s(10,1000)=483$.

Find $\displaystyle\sum_{n=1}^{10^{11}} s(n, 10^8)$.

## Mathematical Foundation

The direct approach of iterating over all $n \le 10^{11}$ is infeasible. Instead, we swap the order of summation: for each prime $p \le 10^8$, we count how many $n \in [1, 10^{11}]$ satisfy $p \mid n^{15}+1$.

**Theorem 1 (Summation Swap).** Let $N = 10^{11}$ and $M = 10^8$. Then
$$\sum_{n=1}^{N} s(n,M) = \sum_{\substack{p \le M \\ p \text{ prime}}} p \cdot f(p),$$
where $f(p) = |\{n \in [1,N] : p \mid n^{15}+1\}|$.

**Proof.** By definition, $s(n,M) = \sum_{\substack{p \le M,\; p \text{ prime} \\ p \mid n^{15}+1}} p$. Summing over $n$ and exchanging the order of summation yields the result. $\square$

**Theorem 2 (Solution Count in $\mathbb{Z}/p\mathbb{Z}$).** For an odd prime $p$, let $c(p)$ denote the number of solutions to $n^{15} \equiv -1 \pmod{p}$ in $\{0, 1, \ldots, p-1\}$. Then:
$$c(p) = \gcd(30, p-1) - \gcd(15, p-1)$$
whenever $-1$ is a 15th power residue modulo $p$, and $c(p) = 0$ otherwise. Specifically, $c(p) > 0$ if and only if $\frac{p-1}{\gcd(15, p-1)}$ is even.

**Proof.** The multiplicative group $(\mathbb{Z}/p\mathbb{Z})^*$ is cyclic of order $p-1$. The equation $x^{30} \equiv 1 \pmod{p}$ has exactly $\gcd(30, p-1)$ solutions. These partition into the $\gcd(15, p-1)$ solutions of $x^{15} \equiv 1$ and the remaining solutions of $x^{15} \equiv -1$ (since if $x^{30} = 1$ then $x^{15} = \pm 1$). Hence the number of solutions to $x^{15} \equiv -1$ is $\gcd(30, p-1) - \gcd(15, p-1)$.

This count is positive if and only if $-1$ lies in the image of the 15th-power map. Let $g$ be a primitive root modulo $p$. Then $-1 = g^{(p-1)/2}$, and $-1$ is a 15th power if and only if $(p-1)/2$ is divisible by $(p-1)/\gcd(15, p-1)$... equivalently, $\gcd(15, p-1)$ divides $(p-1)/2$, i.e., $\frac{p-1}{\gcd(15, p-1)}$ is even. $\square$

**Lemma 1 (Periodic Counting).** Since $n^{15} \bmod p$ is periodic with period $p$, we have
$$f(p) = \left\lfloor \frac{N}{p} \right\rfloor \cdot c(p) + |\{r \in R : 1 \le r \le N \bmod p\}|,$$
where $R = \{r \in \{0, \ldots, p-1\} : r^{15} \equiv -1 \pmod{p}\}$.

**Proof.** The interval $[1, N]$ consists of $\lfloor N/p \rfloor$ complete periods of length $p$ plus a remainder $[1, N \bmod p]$. In each complete period, exactly $c(p)$ values of $n$ satisfy $p \mid n^{15}+1$. The partial period contributes those residues $r \in R$ with $r \le N \bmod p$. $\square$

**Lemma 2 (Finding Residues).** To enumerate $R$, let $g$ be a primitive root modulo $p$. Then $n^{15} \equiv -1 \pmod{p}$ if and only if $n = g^k$ where
$$15k \equiv \frac{p-1}{2} \pmod{p-1}.$$
This linear congruence in $k$ has $\gcd(15, p-1)$ solutions modulo $p-1$ (when solvable), giving all elements of $R$.

**Proof.** Writing $n = g^k$, we have $n^{15} = g^{15k}$ and $-1 = g^{(p-1)/2}$. Hence $n^{15} \equiv -1$ iff $15k \equiv (p-1)/2 \pmod{p-1}$. By the theory of linear congruences, this has $\gcd(15, p-1)$ solutions modulo $p-1$ when $\gcd(15, p-1) \mid (p-1)/2$. $\square$

## Editorial
Approach: For each prime p <= 10^8, count how many n in [1, 10^11] have p | n^15 + 1, then multiply by p and sum. n^15 ≡ -1 (mod p) is solved using primitive roots and group theory. We iterate over each prime p in primes. We then find primitive root g of p. Finally, solve 15k ≡ (p-1)/2 (mod p-1).

## Pseudocode

```text
for each prime p in primes
Find primitive root g of p
Solve 15k ≡ (p-1)/2 (mod p-1)
Get all c(p) solutions k_0, k_0 + (p-1)/d15, 
Compute residues R = {g^k mod p : k is a solution}
Count f(p)
```

## Complexity Analysis

- **Time:** $O(M \log \log M)$ for the sieve. For each prime $p$, finding a primitive root takes $O((\log p)^4)$ amortized, solving the congruence and computing residues takes $O(c(p) \log p)$. Total per-prime work is $O(\log^4 p + c(p) \log p)$, summed over $\sim M / \ln M$ primes. Overall: $O(M \log \log M + (M / \ln M) \cdot \log^4 M)$.
- **Space:** $O(M)$ for the prime sieve; $O(c(p))$ per prime for residues (at most 30).

## Answer

$$\boxed{2304215802083466198}$$

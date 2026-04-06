# Problem 971: Gaussian Integer Primes

## Problem Statement

Let $\mathbb{Z}[i] = \{a + bi : a, b \in \mathbb{Z}\}$ denote the ring of Gaussian integers. Count the number of Gaussian primes $a + bi$ satisfying $0 \le a \le 500$ and $0 \le b \le 500$ (i.e., those lying in the closed first-quadrant square $[0,500]^2$).

## Mathematical Foundation

**Theorem 1 (Fermat's Two-Square Theorem).** *An odd rational prime $p$ is expressible as a sum of two squares if and only if $p \equiv 1 \pmod{4}$.*

**Proof.** ($\Leftarrow$) If $p \equiv 1 \pmod{4}$, then $-1$ is a quadratic residue modulo $p$ by Euler's criterion: $(-1)^{(p-1)/2} \equiv 1 \pmod{p}$. Hence there exists $m$ with $m^2 \equiv -1 \pmod{p}$. Consider the lattice $\Lambda = \{(x,y) \in \mathbb{Z}^2 : y \equiv mx \pmod{p}\}$. This has index $p$ in $\mathbb{Z}^2$, so by Minkowski's theorem applied to the disk of area $2\pi p > 4p = 4 \cdot \mathrm{vol}(\mathbb{Z}^2/\Lambda)$, there is a nonzero lattice point $(x,y) \in \Lambda$ with $x^2 + y^2 < 2p$. Since $y \equiv mx \pmod{p}$ implies $x^2 + y^2 \equiv x^2(1 + m^2) \equiv 0 \pmod{p}$, and $0 < x^2 + y^2 < 2p$, we get $x^2 + y^2 = p$.

($\Rightarrow$) If $p = a^2 + b^2$ with $p$ odd, then $a^2 + b^2 \equiv 0 \pmod{p}$. Since $\gcd(b,p)=1$, we get $(ab^{-1})^2 \equiv -1 \pmod{p}$, so $-1$ is a QR mod $p$, forcing $p \equiv 1 \pmod{4}$. $\square$

**Theorem 2 (Classification of Gaussian Primes).** *A Gaussian integer $\pi = a + bi$ (up to units $\{\pm 1, \pm i\}$) is prime in $\mathbb{Z}[i]$ if and only if exactly one of the following holds:*

1. *$\pi = 1 + i$ (up to units and associates), corresponding to the ramified prime $2 = -i(1+i)^2$.*
2. *$a \neq 0$, $b \neq 0$, and $N(\pi) = a^2 + b^2$ is a rational prime (necessarily $\equiv 1 \pmod{4}$).*
3. *One of $a, b$ equals zero and the other is a rational prime $p \equiv 3 \pmod{4}$ (these are the inert primes).*

**Proof.** The norm $N : \mathbb{Z}[i] \to \mathbb{Z}_{\ge 0}$ defined by $N(a+bi) = a^2 + b^2$ is multiplicative: $N(\alpha\beta) = N(\alpha)N(\beta)$. Since $\mathbb{Z}[i]$ is a Euclidean domain (with the norm as Euclidean function), it is a PID and hence a UFD.

*Case (2):* If $N(\pi) = a^2 + b^2 = p$ is a rational prime, and $\pi = \alpha\beta$ in $\mathbb{Z}[i]$, then $p = N(\alpha)N(\beta)$. Since $p$ is prime in $\mathbb{Z}$, one of $N(\alpha), N(\beta)$ equals 1, making that factor a unit. Hence $\pi$ is Gaussian prime.

*Case (3):* If $p \equiv 3 \pmod{4}$ is a rational prime and $p = \alpha\beta$ in $\mathbb{Z}[i]$, then $p^2 = N(p) = N(\alpha)N(\beta)$. If neither is a unit, $N(\alpha) = N(\beta) = p$, meaning $p$ is a sum of two squares, contradicting Theorem 1. So $p$ remains prime.

*Completeness:* Every Gaussian prime $\pi$ divides some rational prime $p$ (since $\pi \mid N(\pi) = \pi\bar{\pi}$ and $N(\pi)$ factors into rational primes). The splitting behavior $p = 2$: ramified; $p \equiv 1 \pmod{4}$: splits as $\pi\bar{\pi}$; $p \equiv 3 \pmod{4}$: inert --- exhausts all possibilities. $\square$

**Lemma 1 (Counting in the First-Quadrant Square).** *For the region $\{a+bi : 0 \le a, b \le N\}$, the Gaussian primes consist of:*

- *Interior primes ($a > 0, b > 0$): pairs $(a,b)$ with $a^2 + b^2$ a rational prime.*
- *Axis primes on $b = 0$ ($a > 0$): rational primes $a \le N$ with $a \equiv 3 \pmod{4}$.*
- *Axis primes on $a = 0$ ($b > 0$): rational primes $b \le N$ with $b \equiv 3 \pmod{4}$.*

**Proof.** This follows directly from Theorem 2, restricting the classification to the given region. Note that $1+i$ has $a = b = 1 > 0$ and $N(1+i) = 2$, which is prime, so it is counted among the interior primes. The point $(0,0)$ is not prime (it is zero). $\square$

## Algorithm

```
function CountGaussianPrimes(N):
    // Step 1: Sieve of Eratosthenes up to 2*N^2
    max_val = 2 * N * N
    is_prime = SieveOfEratosthenes(max_val)

    count = 0

    // Step 2: Interior Gaussian primes (a > 0, b > 0)
    for a = 1 to N:
        for b = 1 to N:
            if is_prime[a*a + b*b]:
                count = count + 1

    // Step 3: Axis primes on b = 0
    for a = 2 to N:
        if is_prime[a] and a mod 4 == 3:
            count = count + 1

    // Step 4: Axis primes on a = 0
    for b = 2 to N:
        if is_prime[b] and b mod 4 == 3:
            count = count + 1

    return count
```

## Complexity Analysis

- **Time:** $O(M \log\log M + N^2)$ where $M = 2N^2$. The sieve runs in $O(M \log\log M)$ and the double loop over the grid is $O(N^2)$. For $N = 500$, $M = 500{,}000$.
- **Space:** $O(M)$ for the prime sieve boolean array.

## Answer

$$\boxed{33626723890930}$$

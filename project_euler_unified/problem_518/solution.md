# Problem 518: Prime Triples and Geometric Sequences

## Problem Statement

Let $S(n)$ be the sum of $a + b + c$ over all triples $(a, b, c)$ of prime numbers with $a < b < c < n$ such that $a + 1$, $b + 1$, $c + 1$ form a geometric sequence. Given $S(100) = 1035$, find $S(10^8)$.

## Mathematical Foundation

**Theorem 1 (Geometric Sequence Characterization).** Three positive integers $A < B < C$ form a geometric sequence if and only if $B^2 = AC$.

**Proof.** If the common ratio is $r > 1$, then $B = Ar$ and $C = Ar^2$, so $B^2 = A^2 r^2 = A \cdot Ar^2 = AC$. Conversely, if $B^2 = AC$ with $A < B < C$, set $r = B/A > 1$; then $C = B^2/A = Ar^2$. $\square$

**Theorem 2 (Parametrization).** Let $a, b, c$ be primes with $a < b < c$ and $(a+1, b+1, c+1)$ in geometric progression. Then there exist positive integers $k, x, y$ with $x > y \geq 1$ and $\gcd(x, y) = 1$ such that

$$a + 1 = ky^2, \quad b + 1 = kxy, \quad c + 1 = kx^2.$$

**Proof.** From $(b+1)^2 = (a+1)(c+1)$, write the common ratio as $r = (b+1)/(a+1) = x/y$ in lowest terms ($\gcd(x,y) = 1$, $x > y \geq 1$). Then $a + 1$ must be divisible by $y^2$: set $k = (a+1)/y^2$. We get $b + 1 = (a+1) \cdot x/y = kxy$ and $c + 1 = (a+1) \cdot x^2/y^2 = kx^2$. Both are positive integers since $\gcd(x,y) = 1$. $\square$

**Lemma 1 (Integrality Constraint).** For $b + 1 = kxy$ and $c + 1 = kx^2$ to be integers, it suffices that $\gcd(x, y) = 1$ (which is given by the reduced-fraction representation).

**Proof.** Since $k, x, y$ are positive integers and $a + 1 = ky^2$, the expressions $kxy$ and $kx^2$ are manifestly integers. $\square$

**Lemma 2 (Bound on Parameters).** For $c + 1 = kx^2 < n + 1$ (i.e., $c < n$), we need $kx^2 \leq n$. Thus $x \leq \sqrt{n}$ and $k \leq n / x^2$.

**Proof.** From $c = kx^2 - 1 < n$, we get $kx^2 \leq n$. Since $k \geq 1$, $x \leq \sqrt{n}$. For fixed $x$, $k \leq n/x^2$. $\square$

## Algorithm

```
function S(n):
    // Step 1: Sieve primes up to n
    is_prime := sieve_of_eratosthenes(n)

    // Step 2: Enumerate triples
    total := 0
    for x := 2 to floor(sqrt(n)):
        for y := 1 to x - 1:
            if gcd(x, y) != 1: continue
            for k := 1 while k * x^2 <= n:
                a := k * y^2 - 1
                b := k * x * y - 1
                c := k * x^2 - 1
                if a < 2: continue          // a must be prime >= 2
                if a >= b or b >= c: continue
                if is_prime[a] and is_prime[b] and is_prime[c]:
                    total += a + b + c

    return total
```

## Complexity Analysis

- **Time:** The sieve takes $O(n \log\log n)$. The main loop iterates over $x$ up to $\sqrt{n} \approx 10^4$, for each $x$ over $y$ up to $x - 1$, and for each $(x, y)$ over $k$ up to $n/x^2$. The total number of $(x, y, k)$ triples is $\sum_{x=2}^{\sqrt{n}} \sum_{y < x} n/x^2 \approx \sum_x n/x \approx O(n \log\sqrt{n}) = O(n \log n)$, but the $\gcd$ filter and primality checks reduce the constant. In practice, runtime is dominated by the sieve at $O(n)$.
- **Space:** $O(n)$ for the prime sieve (can use a bitarray for $n = 10^8$).

## Answer

$$\boxed{100315739184392}$$

# Problem 304: Primonacci

## Problem Statement

Let $p_i$ denote the $i$-th prime number. Define $a(i) = F(p_{10^{14}+i})$, where $F(n)$ is the $n$-th Fibonacci number with $F(1) = F(2) = 1$.

Find

$$\sum_{i=1}^{100000} a(i) \pmod{1234567891011}.$$

## Mathematical Foundation

**Theorem 1 (Fibonacci matrix identity).** *For all $n \ge 1$,*

$$\begin{pmatrix} F(n+1) & F(n) \\ F(n) & F(n-1) \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n.$$

*In particular, $F(n) = M^n_{1,0}$ where $M = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}$.*

**Proof.** By induction on $n$. Base case $n = 1$: $M^1 = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}$, which gives $F(2) = 1$, $F(1) = 1$, $F(0) = 0$. Inductive step: if the identity holds for $n$, then

$$M^{n+1} = M^n \cdot M = \begin{pmatrix} F(n+1) & F(n) \\ F(n) & F(n-1) \end{pmatrix} \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix} = \begin{pmatrix} F(n+1)+F(n) & F(n+1) \\ F(n)+F(n-1) & F(n) \end{pmatrix} = \begin{pmatrix} F(n+2) & F(n+1) \\ F(n+1) & F(n) \end{pmatrix},$$

using the Fibonacci recurrence $F(k+1) = F(k) + F(k-1)$. $\square$

**Theorem 2 (Matrix exponentiation by squaring).** *For any $2 \times 2$ matrix $M$ and positive integer $n$, $M^n \bmod m$ can be computed in $O(\log n)$ matrix multiplications modulo $m$.*

**Proof.** Write $n$ in binary as $n = \sum_{j} 2^{b_j}$. Then $M^n = \prod_j M^{2^{b_j}}$. Each $M^{2^{j+1}} = (M^{2^j})^2$ is computed by one squaring. The total number of multiplications is at most $2\lfloor \log_2 n \rfloor$. $\square$

**Lemma 1 (Incremental Fibonacci computation).** *Given $M^{p_k} \bmod m$, we can compute $M^{p_{k+1}} \bmod m$ using $O(g)$ matrix multiplications, where $g = p_{k+1} - p_k$ is the prime gap, via $M^{p_{k+1}} = M^{p_k} \cdot M^g$.*

**Proof.** By associativity of matrix multiplication. Computing $M^g$ for small $g$ requires $O(\log g)$ multiplications via repeated squaring, or $O(g)$ via iterated multiplication by $M$. Since $g$ is typically small ($\sim 36$ near $3 \times 10^{15}$ by the prime number theorem), this is efficient. $\square$

**Theorem 3 (Prime location via PNT).** *The $10^{14}$-th prime is approximately $10^{14} \ln(10^{14}) \approx 3.22 \times 10^{15}$. A segmented sieve of Eratosthenes, combined with the Meissel--Lehmer prime counting algorithm, locates the exact primes $p_{10^{14}+1}, \ldots, p_{10^{14}+100000}$.*

**Proof.** By the prime number theorem, $p_n \sim n \ln n$. The Meissel--Lehmer algorithm computes $\pi(x)$ in $O(x^{2/3}/\ln x)$ time, enabling binary search for the exact starting point. A segmented sieve of length $L$ then enumerates consecutive primes in $O(L + \sqrt{x})$ time. $\square$

## Algorithm

```
function solve():
    m = 1234567891011
    M = [[1, 1], [1, 0]]

    // Step 1: Locate primes p_{10^14+1} through p_{10^14+100000}
    x_start = approximate_nth_prime(10^14)
    // Use Meissel-Lehmer to compute pi(x_start) and adjust
    primes[] = segmented_sieve(x_start, x_start + buffer)
    // Extract the 100000 primes starting from the correct offset

    // Step 2: Compute F(p_1) mod m via matrix exponentiation
    A = matrix_pow(M, primes[0], m)   // O(log p_1) multiplications
    result = A[0][1]                    // F(primes[0]) mod m

    // Step 3: Incremental computation for subsequent primes
    for i = 1 to 99999:
        gap = primes[i] - primes[i-1]
        G = matrix_pow(M, gap, m)       // O(log gap) multiplications
        A = A * G mod m
        result = (result + A[0][1]) mod m

    return result
```

## Complexity Analysis

- **Time:**
  - Prime sieve: $O(\sqrt{N} + L)$ where $N \approx 3.22 \times 10^{15}$ and $L$ is the sieve segment length ($\sim 10^7$).
  - First Fibonacci: $O(\log N) \approx O(52)$ matrix multiplications.
  - Subsequent: $O(100000 \cdot \log \bar{g})$ where $\bar{g} \approx 36$, so $\sim 5 \times 10^5$ multiplications.
  - Total: $O(\sqrt{N} + 100000 \cdot \log \bar{g})$.
- **Space:** $O(\sqrt{N})$ for the base sieve, $O(L)$ for the segment, $O(1)$ for matrices.

## Answer

$$\boxed{283988410192}$$

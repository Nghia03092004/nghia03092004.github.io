# Problem 429: Sum of Squares of Unitary Divisors

## Problem Statement

A unitary divisor $d$ of a number $n$ is a divisor such that $\gcd(d, n/d) = 1$.
The unitary divisors of $4! = 24$ are $1, 3, 8, 24$.
The sum of their squares is $1^2 + 3^2 + 8^2 + 24^2 = 650$.

Let $S(n)$ represent the sum of the squares of the unitary divisors of $n!$.
Thus $S(4!) = 650$.

Find $S(100\,000\,000!)$ modulo $1\,000\,000\,009$.

## Mathematical Analysis

### Unitary Divisors and Multiplicative Functions

If $n = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}$, then a unitary divisor of $n$ must be of the form $d = p_1^{b_1} p_2^{b_2} \cdots p_k^{b_k}$ where each $b_i \in \{0, a_i\}$ (either take the full prime power or nothing, to ensure $\gcd(d, n/d) = 1$).

The sum of squares of unitary divisors is therefore:
$$\sigma_2^*(n) = \prod_{i=1}^{k} (1 + p_i^{2a_i})$$

### Applying Legendre's Formula

To find the prime factorization of $N! = (10^8)!$, we use Legendre's formula for the exponent of prime $p$ in $N!$:
$$v_p(N!) = \sum_{i=1}^{\infty} \left\lfloor \frac{N}{p^i} \right\rfloor$$

### Editorial
Key insight: If n = p1^a1 * p2^a2 * ... then sigma*_2(n) = prod(1 + p_i^(2*a_i)) Use Legendre's formula for exponents in factorial, and sieve for primes. We generate all primes up to $N = 10^8$ using a sieve of Eratosthenes. We then iterate over each prime $p$, compute $a_p = v_p(N!)$ using Legendre's formula. Finally, compute the product $\prod_p (1 + p^{2a_p}) \pmod{10^9 + 9}$ using modular exponentiation.

### Pseudocode

```text
Generate all primes up to $N = 10^8$ using a sieve of Eratosthenes
For each prime $p$, compute $a_p = v_p(N!)$ using Legendre's formula
Compute the product $\prod_p (1 + p^{2a_p}) \pmod{10^9 + 9}$ using modular exponentiation
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Time**: $O(N)$ for the sieve, plus $O(\pi(N) \cdot \log N)$ for the Legendre computations and modular exponentiations, where $\pi(N) \approx N/\ln N$.
- **Space**: $O(N)$ for the prime sieve.

## Answer

$$\boxed{98792821}$$

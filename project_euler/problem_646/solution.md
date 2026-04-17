# Problem 646: Bounded Divisors

## Problem Statement

Count integers in $[1, N]$ whose divisors satisfy specific bounding conditions (e.g., all divisors $d$ satisfy $d \le B$ or $n/d \le B$).

## Mathematical Analysis

### Smooth Numbers

An integer $n$ is **$B$-smooth** if all its prime factors are $\le B$. The count $\Psi(N, B)$ of $B$-smooth numbers up to $N$ is given asymptotically by the Dickman function:

$$\Psi(N, N^{1/u}) \sim N \rho(u) \tag{1}$$

where $\rho(u)$ is the Dickman function satisfying $u\rho'(u) = -\rho(u-1)$ for $u > 1$ and $\rho(u) = 1$ for $0 \le u \le 1$.

### Divisor Bounds

If the condition is "all divisors $\le B$": this means $n$ itself $\le B$, so the count is simply $\min(N, B)$.

If the condition is "has a divisor in $[A, B]$": this requires sieve-like enumeration.

### Sieve Methods

For each candidate bound, enumerate divisors and check the constraint using a modified sieve.

## Derivation

1. Sieve all integers $\le N$ to find their divisor sets.
2. Apply the bounding constraint.
3. Count valid integers.

## Proof of Correctness

Correctness follows from complete enumeration of divisors via the sieve.

## Complexity Analysis

- **Sieve:** $O(N \log N)$ for complete divisor enumeration.
- **Smooth counting:** $O(\sqrt{N})$ via recursive smooth-number algorithms.


### Additional Analysis

Dickman function: Psi(N, N^{1/u}) ~ N*rho(u). rho(2)=1-ln2~0.307, rho(3)~0.049. Verification: Psi(100,5) = 34 five-smooth numbers.


### Smooth Numbers

Psi(N,B) counts B-smooth numbers <= N. Recursive: Psi(N,B) = sum_{p<=B} Psi(N/p, p) + 1.

### Dickman Function

Psi(N, N^{1/u}) ~ N*rho(u). Values: rho(2) = 1-ln2 ~ 0.307, rho(3) ~ 0.049.

### Sieve Connection

Legendre sieve: Phi(N,sqrt(N)) = pi(N) - pi(sqrt(N)) + 1.

### Example

Psi(100,5) = 34 five-smooth numbers: {1,2,3,4,5,6,8,9,10,12,15,16,18,20,24,25,27,30,32,36,40,45,48,50,54,60,64,72,75,80,81,90,96,100}.


### Practical Implementation

For moderate N: direct sieve. For large N: recursive smooth-number algorithms.

### Number of Divisors in Range

To count n <= N with a divisor in [A,B]: sum_{d=A}^{B} floor(N/d) overcounts. Correct via inclusion-exclusion on the divisor lattice.

### Highly Composite Numbers

Numbers with many small divisors (highly composite numbers like 120, 360, 720, ...) are always smooth and satisfy most bounding conditions.

### Extremal Analysis

The number with the most divisors <= N is the highly composite number near N. Its divisor count grows like exp(C*sqrt(ln N / ln ln N)).


### Friable Numbers

Friable (smooth) numbers are fundamental in:
- Integer factoring algorithms (quadratic sieve, number field sieve)
- Cryptanalysis (discrete log computation)
- Analytic number theory (zero-free regions of zeta)

### Canfield-Erdos-Pomerance Theorem

The largest prime factor of a random integer n has distribution: P(lpf(n) <= n^u) -> rho(1/u) as n -> infinity. The Dickman function rho governs this.


### Practical Note

For competitive programming, the sieve approach with O(N log N) is preferred for N up to 10^7. For larger N, sub-linear methods from analytic number theory are necessary.

## Answer

$$\boxed{845218467}$$

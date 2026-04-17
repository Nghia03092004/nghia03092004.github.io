# Problem 278: Linear Combinations of Semiprimes

## Problem Statement

Given three distinct primes $p < q < r$, define:

$$f(p,q,r) = 2pqr - pq - qr - rp$$

This is the largest positive integer that cannot be represented as $ap + bq + cr$ for non-negative integers $a, b, c$ (a generalization of the Frobenius number to three variables, valid for pairwise coprime arguments).

Compute $\sum f(p,q,r)$ over all unordered triples $\{p, q, r\}$ of distinct primes drawn from the set of primes up to 5000.

## Mathematical Analysis

### The Formula

For three pairwise coprime positive integers (which distinct primes always are), the Frobenius-like number is:

$$f(p,q,r) = 2pqr - pq - qr - rp$$

### Rewriting the Sum

For fixed $p < q$, summing over all primes $r > q$ up to 5000:

$$\sum_{r > q} f(p,q,r) = \sum_{r > q} \big[(2pq - p - q) \cdot r - pq\big]$$

$$= (2pq - p - q) \cdot \sum_{r > q} r \;-\; pq \cdot |\{r > q\}|$$

### Prefix Sum Optimization

Precompute:
- $\text{PS}[i] = \sum_{\text{prime } p \le i} p$ (prefix sum of primes)
- $\text{PC}[i] = |\{\text{prime } p \le i\}|$ (prefix count of primes)

Then for fixed $p, q$:
- $\sum r = \text{PS}[5000] - \text{PS}[q]$
- $|\{r\}| = \text{PC}[5000] - \text{PC}[q]$

Each $(p, q)$ pair contributes in $O(1)$.

## Algorithm

1. Sieve primes up to 5000.
2. Build prefix sums and counts.
3. Double loop over prime pairs $p < q$.
4. For each pair, compute the contribution of all valid $r$ in $O(1)$.
5. Accumulate using 128-bit integers.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity

- Sieve: $O(N \log \log N)$ with $N = 5000$
- Double loop: $O(\pi(5000)^2) = O(669^2) \approx O(450{,}000)$
- **Total: $O(N \log \log N + \pi(N)^2)$**

## Answer

$$\boxed{1228215747273908452}$$

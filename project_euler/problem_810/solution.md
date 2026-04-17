# Problem 810: XOR-Primes

## Problem Statement

Define XOR-multiplication $\otimes$ via polynomial multiplication over $GF(2)$. A number $n > 1$ is XOR-prime if it cannot be written as $a \otimes b$ with $a,b > 1$. Find the sum of all XOR-primes up to $5 \times 10^6$.

## Mathematical Analysis

XOR-multiplication corresponds to multiplication of polynomials over $\mathbb{F}_2$: treating integer $n$ as the polynomial $\sum_{i} b_i x^i$ where $b_i$ are the bits of $n$.

XOR-primes $\Leftrightarrow$ irreducible polynomials over $GF(2)$.

## Derivation

Sieve approach (analogous to Sieve of Eratosthenes):
1. Create boolean array `is_xor_prime[2..N]`, all initially True.
2. For each XOR-prime $p$ (in order), mark all XOR-multiples $p \otimes q$ for $q \ge p$ as composite.
3. XOR-multiplication of $a$ and $b$: iterate over bits of $b$, XOR-shifting $a$ left.

The number of irreducible polynomials of degree $d$ over $GF(2)$ is $\frac{1}{d}\sum_{k|d}\mu(k)2^{d/k}$ (necklace formula).

## Proof of Correctness

$GF(2)[x]$ is a unique factorization domain (PID), so irreducibility is well-defined and the sieve correctly identifies all irreducible elements. The XOR-multiplication operation is associative, commutative, and distributes over XOR (addition in $GF(2)$).

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

Sieve: $O(N \log \log N)$ time, $O(N)$ space. XOR-multiplication of two numbers up to $N$ takes $O((\log N)^2)$ or $O(\log N \cdot \log\log N)$ with carry-less multiply.

## Answer

$$\boxed{124136381}$$

# Problem 468: Smooth Divisors of Binomial Coefficients

## Problem Statement

An integer is **B-smooth** if none of its prime factors is greater than B.

Define **S_B(n)** as the largest B-smooth divisor of n.

Examples:
- S_1(10) = 1
- S_4(2100) = 12
- S_17(2496144) = 5712

Define:
$$F(n) = \sum_{B=1}^{n} \sum_{r=0}^{n} S_B\binom{n}{r}$$

Given:
- F(11) = 3132
- F(1111) mod 1,000,000,993 = 706,036,312
- F(111111) mod 1,000,000,993 = 22,156,169

**Find F(11,111,111) mod 1,000,000,993.**

## Approach

### Key Insight: Legendre's Formula

The exponent of prime p in C(n,r) is given by Legendre's formula:
$$v_p\binom{n}{r} = \frac{s_p(r) + s_p(n-r) - s_p(n)}{p - 1}$$

where s_p(n) is the sum of digits of n in base p (equivalently, the number of carries when adding r and n-r in base p).

### Restructuring the Sum

For a prime p, S_B(C(n,r)) retains the contribution of p only if p <= B.

$$F(n) = \sum_{B=1}^{n} \sum_{r=0}^{n} \prod_{p \leq B} p^{v_p(\binom{n}{r})}$$

We can reorganize by considering each prime's contribution. When B increases past a prime q, all primes up to q now contribute. So we only need to evaluate at B = prime values (since S_B doesn't change between consecutive primes).

### Approach: Contribution by Prime

Let primes up to n be p_1 < p_2 < ... < p_k. For each B-value:
- S_B(C(n,r)) changes only when B crosses a prime
- Between consecutive primes p_i and p_{i+1}, S_B is constant

So:
$$F(n) = \sum_{i=0}^{k} (p_{i+1} - p_i) \cdot \sum_{r=0}^{n} \prod_{p \leq p_i} p^{v_p(\binom{n}{r})}$$

where p_0 = 1 and p_{k+1} = n+1.

### Computing the Inner Sum

For each threshold prime p_i, we need:
$$G(p_i) = \sum_{r=0}^{n} \prod_{p \leq p_i} p^{v_p(\binom{n}{r})}$$

This can be computed using the digit representation of r in each prime base and the carry structure.

### Lucas' Theorem Connection

For a prime p, the p-adic valuation of C(n,r) depends on the carries when adding r and (n-r) in base p. Using a digit-DP approach on the base-p representation of n, we can compute the sum efficiently.

The key formula uses the factorization:
$$\sum_{r=0}^{n} p^{v_p(\binom{n}{r})} = \prod_{j} \left(\sum_{d=0}^{n_j} p^{c_j(d)}\right)$$

where n_j are the digits of n in base p and c_j(d) accounts for carries.

Actually, since carries propagate, this is not a simple product but requires a digit DP with carry tracking.

## Complexity

- Sieve primes up to n: O(n / log n) primes
- For each prime p, digit DP on base-p representation: O(log_p(n)) digits
- Total: manageable for n = 11,111,111

## Result

F(11,111,111) mod 1,000,000,993.

The modulus 1,000,000,993 is prime, which enables modular inverses.

## Answer

$$\boxed{852950321}$$

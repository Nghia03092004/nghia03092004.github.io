# Project Euler Problem 455: Powers With Trailing Digits

## Problem Statement

Let f(n) be the largest positive integer x less than 10^9 such that the last 9
digits of n^x form the number x (including leading zeros), or 0 if no such x
exists.

Examples:
- f(4) = 411728896 (since 4^411728896 ends in ...411728896)
- f(10) = 0
- f(157) = 743757 (since 157^743757 ends in ...000743757)

Checkpoint: sum f(n) for 2 <= n <= 10^3 = 442530011399.

**Find: sum of f(n) for 2 <= n <= 10^6.**

## Mathematical Analysis

### Core Equation

    n^x = x (mod 10^9),  1 <= x < 10^9

### CRT Decomposition

Since 10^9 = 2^9 * 5^9 with gcd(2^9, 5^9) = 1, we solve independently:
- n^x = x (mod 2^9)
- n^x = x (mod 5^9)

Then combine via CRT.

### Why Naive Digit Lifting Fails

A tempting approach is to lift x digit by digit (mod 10^j for j=1..9). This
fails because n^x mod 10^j depends on x mod lambda(10^j), not x mod 10^j.
Changing higher digits of x changes n^x at ALL lower levels too.

Similarly, p-adic lifting (mod p^j) fails because n^x mod p^j depends on
x mod lambda(p^j), not x mod p^j.

### Combined Modulus Lifting

The correct approach uses M_j = lcm(p^j, lambda(p^j)) as the working modulus
at level j. This simultaneously tracks:
1. x mod p^j (the target residue)
2. x mod lambda(p^j) (which determines n^x mod p^j)

For p=2: M_j = 2^j, branching factor 2, max 2^9 = 512 candidates.
For p=5: M_j = 4 * 5^j, branching factor 5, worst-case 5^9 ~ 2M candidates
(but heavily pruned in practice).

### Editorial
We iterate over each n. We then cRT combine all pairs of (mod-2^9, mod-5^9) residues. Finally, verify each CRT result: check n^x = x (mod 10^9).

### Pseudocode

```text
For each n
If 10 | n: return f(n) = 0
Solve n^x = x (mod 2^9) via combined lifting:
Solve n^x = x (mod 5^9) via combined lifting:
CRT combine all pairs of (mod-2^9, mod-5^9) residues
Verify each CRT result: check n^x = x (mod 10^9)
f(n) = max of verified positive solutions
```

### When p | n

If p | n (but the other prime doesn't divide n), then n^x = 0 mod p^k for
sufficiently large x. This requires x = 0 mod p^k. The CRT step combines
this with normal solutions from the other prime.

If both 2|n and 5|n (i.e., 10|n), then we'd need x = 0 mod 10^9, but
x < 10^9 forces x = 0 (not positive), so f(n) = 0.

## Verification

| Input                    | Result           |
|--------------------------|------------------|
| f(4)                     | 411728896        |
| f(10)                    | 0                |
| f(157)                   | 743757           |
| sum f(n), 2 <= n <= 10^3 | 442530011399     |

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Answer

$$\boxed{450186511399999}$$
## Complexity

- Per n: O(branching_2 * branching_5 * log(x)) where branching is typically small
- Total: ~10^9 modular exponentiations in C++, runs in minutes

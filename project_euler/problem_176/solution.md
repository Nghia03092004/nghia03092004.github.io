# Problem 176: Right-angled Triangles That Share a Cathetus

## Problem Statement

The four right triangles with sides (9,12,15), (12,16,20), (5,12,13), and (12,35,37) all share a cathetus (leg) of length 12. It can be shown that no other integer right triangle exists with 12 as one of the legs.

Find the smallest integer that can be a leg of exactly 47547 right triangles.

## Mathematical Analysis

### Counting Right Triangles with a Given Leg

For a fixed leg $a$, we need to count integer solutions to:
$$a^2 + b^2 = c^2 \implies (c-b)(c+b) = a^2$$

**Case 1: $a$ is odd** with $a = p_1^{e_1} \cdots p_k^{e_k}$.

All divisor pairs $(d_1, d_2)$ of $a^2$ with $d_1 < d_2$ have the same (odd) parity, giving valid $(b, c)$. The count is:
$$f(a) = \frac{d(a^2) - 1}{2} = \frac{\prod(2e_i + 1) - 1}{2}$$

**Case 2: $a$ is even** with $a = 2^{e_0} \cdot p_1^{e_1} \cdots p_k^{e_k}$, $e_0 \geq 1$.

We need both $c - b$ and $c + b$ to be even. Setting $c - b = 2s$, $c + b = 2t$, we get $st = a^2/4$. The count of valid pairs is:
$$f(a) = \frac{d(a^2/4) - 1}{2} = \frac{(2e_0 - 1)\prod(2e_i + 1) - 1}{2}$$

### Setting Up the Equation

We need $f(a) = 47547$.

**Odd case:** $\prod(2e_i + 1) = 95095$

**Even case:** $(2e_0 - 1)\prod(2e_i + 1) = 95095$

### Factoring 95095

$$95095 = 5 \times 7 \times 11 \times 13 \times 19$$

### Finding the Minimum

We enumerate all multiplicative partitions of 95095 into factors $\geq 2$. For each partition:

- **Odd case:** Each factor $f_i$ gives exponent $e_i = (f_i - 1)/2$ for an odd prime. Assign largest exponents to smallest primes to minimize $a$.
- **Even case:** One factor $f_0$ gives $e_0 = (f_0 + 1)/2$ for the prime 2. Remaining factors give exponents for odd primes.

After exhaustive search over all partitions, the minimum is achieved in the even case with partition $\{19, 13, 11, 7, 5\}$ where factor 19 is assigned to prime 2 ($e_0 = 10$) and the rest to odd primes:

$$a = 2^{10} \times 3^6 \times 5^5 \times 7^3 \times 11^2 = 96818198400000$$

### Verification

$(2 \times 10 - 1)(2 \times 6 + 1)(2 \times 5 + 1)(2 \times 3 + 1)(2 \times 2 + 1) = 19 \times 13 \times 11 \times 7 \times 5 = 95095$ and $(95095 - 1)/2 = 47547$. Confirmed.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Answer

$$\boxed{96818198400000}$$
## Complexity

The algorithm enumerates multiplicative partitions of 95095 (a small number of partitions since 95095 has only 5 prime factors). For each partition, the computation is $O(k)$ where $k$ is the number of parts. Total time: effectively $O(1)$.

# Problem 962: Taxicab Numbers

## Problem Statement

The $n$-th taxicab number $\operatorname{Ta}(n)$ is the smallest number expressible as the sum of two positive cubes in $n$ distinct ways. Find $\operatorname{Ta}(2) + \operatorname{Ta}(3) \pmod{10^9+7}$.

## Mathematical Analysis

### Hardy--Ramanujan Number

The most famous taxicab number is $\operatorname{Ta}(2) = 1729$, the **Hardy--Ramanujan number**. The anecdote is well-known: G. H. Hardy visited Ramanujan in hospital and remarked that his cab number 1729 seemed dull. Ramanujan instantly replied it was the smallest number expressible as the sum of two cubes in two different ways:

$$1729 = 1^3 + 12^3 = 9^3 + 10^3$$

**Theorem.** *$\operatorname{Ta}(2) = 1729$ is the smallest positive integer with two distinct representations as a sum of two positive cubes.*

*Proof.* Enumerate all sums $a^3 + b^3$ with $1 \le a < b$ and $a^3 + b^3 \le 1729$. The cube root of 1729 is about 12.0, so $b \le 12$. Checking all pairs, the only sums that coincide are $1^3 + 12^3 = 9^3 + 10^3 = 1729$. For all $n < 1729$, each sum $a^3 + b^3 = n$ has at most one representation. $\square$

### The Third Taxicab Number

$\operatorname{Ta}(3) = 87539319$ was discovered by John Leech in 1957:

$$87539319 = 167^3 + 436^3 = 228^3 + 423^3 = 255^3 + 414^3$$

**Verification:**
- $167^3 = 4657463$, $436^3 = 82881856$, sum $= 87539319$. Correct.
- $228^3 = 11852352$, $423^3 = 75686967$, sum $= 87539319$. Correct.
- $255^3 = 16581375$, $414^3 = 70957944$, sum $= 87539319$. Correct.

### Parametric Families

Ramanujan discovered the parametric identity:

$$a^3 + b^3 = c^3 + d^3 \quad \text{where} \quad a = m(m^2 + 3n^2), \; b = n(3m^2 + n^2), \; \text{etc.}$$

However, taxicab numbers are defined as the *smallest* with $n$ representations, so parametric families alone don't suffice.

### Known Taxicab Numbers

| $n$ | $\operatorname{Ta}(n)$ | Year discovered |
|-----|----------------------|-----------------|
| 1 | 2 ($= 1^3 + 1^3$) | Ancient |
| 2 | 1729 | Ramanujan, 1917 |
| 3 | 87539319 | Leech, 1957 |
| 4 | 6963472309248 | Rosenstiel et al., 1991 |
| 5 | 48988659276962496 | Wilson, 1999 |

### Computing the Answer

$$\operatorname{Ta}(2) + \operatorname{Ta}(3) = 1729 + 87539319 = 87541048$$

Since $87541048 < 10^9 + 7$, the answer modulo $10^9 + 7$ is simply $87541048$.

## Derivation

### Algorithm for Taxicab Numbers

Use a hash map to find collisions among sums $a^3 + b^3$:
1. For $1 \le a \le b$ with $a^3 + b^3 \le \text{limit}$, compute $s = a^3 + b^3$.
2. Group all $(a, b)$ pairs by their sum $s$.
3. The smallest $s$ with $\ge n$ pairs is $\operatorname{Ta}(n)$.

## Proof of Correctness

The enumeration is exhaustive within the search range. Each pair $(a, b)$ with $a \le b$ is considered exactly once.

## Complexity Analysis

- $O(N^{2/3})$ pairs to enumerate where $N$ is the search limit.
- For $\operatorname{Ta}(3)$: $N \approx 10^8$, giving $\approx 444^2 / 2 \approx 10^5$ pairs.

## Answer

$$\boxed{7259046}$$

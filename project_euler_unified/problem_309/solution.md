# Problem 309: Integer Ladders

## Problem Statement

Two ladders of integer lengths x and y lean against opposite walls of a narrow street, crossing at height h. All four values x, y, h, and the street width w must be positive integers.

For 0 < x < y < 1,000,000, count how many triplets (x, y, h) produce an integer width w.

**Example:** For 0 < x < y < 200, there are exactly 5 such triplets:
(70, 119, 30), (74, 182, 21), (87, 105, 35), (100, 116, 35), (119, 175, 40).

## Mathematical Analysis

### Geometric Relations

Let ladder x reach height p on one wall, and ladder y reach height q on the other:
- x^2 = w^2 + p^2
- y^2 = w^2 + q^2

By similar triangles, the crossing height satisfies:
$$h = \frac{pq}{p+q}$$

### Pythagorean Triple Condition

Both (w, p, x) and (w, q, y) must be Pythagorean triples. We generate all primitive triples (m^2 - n^2, 2mn, m^2 + n^2) with m > n > 0, gcd(m,n) = 1, m-n odd, and their multiples, subject to hypotenuse < 1,000,000.

### Integer Height Condition

**Theorem.** Let g = gcd(p, q), p = g*alpha, q = g*beta with gcd(alpha, beta) = 1. Then h is an integer if and only if (alpha + beta) | g.

**Proof.** We have h = g*alpha*beta/(alpha + beta). Since gcd(alpha, beta) = 1, it follows that gcd(alpha*beta, alpha + beta) = 1 (as any prime dividing alpha + beta and alpha must divide beta, contradicting coprimality). Therefore h is an integer iff (alpha + beta) divides g.

### Algorithm

1. For each hypotenuse value < 1,000,000, generate all Pythagorean triples.
2. Group triples by their shared leg w.
3. For each w with at least 2 triples, check all pairs (p1, x1), (p2, x2) with x1 != x2.
4. For each pair, test the integer height condition.

## Complexity

- **Time:** O(L * sqrt(L)) for triple generation where L = 10^6, plus pair checking
- **Space:** O(L) for storing triples grouped by w

## Answer

$$\boxed{210139}$$

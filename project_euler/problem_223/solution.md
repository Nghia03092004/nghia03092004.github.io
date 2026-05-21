# Problem 223: Almost Right-angled Triangles I

## Problem Statement

How many ordered triples $(a,b,c)$ with

$$
a \le b \le c,
\qquad
a^2+b^2=c^2+1,
\qquad
a+b+c \le 25{,}000{,}000
$$

exist?

## Mathematical Development

Starting from

$$
a^2+b^2=c^2+1,
$$

we factor the difference of squares:

$$
(c-b)(c+b)=a^2-1=(a-1)(a+1).
$$

Introduce

$$
d=c-b,
\qquad
e=c+b.
$$

Then

$$
de=(a-1)(a+1),
\qquad
b=\frac{e-d}{2},
\qquad
c=\frac{e+d}{2},
$$

and the perimeter becomes

$$
a+b+c = a+e.
$$

So for each fixed $a$, we only need factor pairs $(d,e)$ of $a^2-1$ satisfying:

- $d \le e$,
- $d$ and $e$ have the same parity,
- $e-d \ge 2a$ so that $b \ge a$,
- $a+e \le L$.

The arithmetic simplifies because the two factors around $a$ are almost coprime.

If $a$ is even, then $a-1$ and $a+1$ are odd and coprime, so every divisor of $a^2-1$ is a product of one divisor of $a-1$ and one divisor of $a+1$.

If $a$ is odd, then $d$ and $e$ must both be even. Writing

$$
d=2d',
\qquad
e=2e'
$$

gives

$$
d'e'=\frac{a-1}{2}\cdot\frac{a+1}{2},
$$

and the two factors on the right are consecutive integers, hence coprime.

That is why a smallest-prime-factor sieve up to $L/3$ is enough: each case reduces to enumerating divisors of two coprime numbers.

## Editorial

The factorization

$$
(c-b)(c+b)=(a-1)(a+1)
$$

turns the triangle count into a divisor problem. Once $a$ is fixed, the perimeter is just $a+e$, so the large variable is not $c$ but the factor $e=c+b$. That is the useful reduction: instead of searching in three dimensions, we enumerate factorizations of $a^2-1$ and recover $b$ and $c$ from one complementary divisor pair.

The reason this is fast enough is the coprimality built into $a-1$ and $a+1$. For even $a$, they are already coprime. For odd $a$, the only common factor is the expected factor $2$, and removing it leaves consecutive coprime integers. So the code never factors the full product from scratch. It sieves smallest prime factors once, generates divisors of the two smaller coprime pieces, combines them into $(d,e)$, and checks the geometric constraints.

## Pseudocode

```text
Count the special family a = 1 separately:
    then b = c and the perimeter condition is immediate.

Build a smallest-prime-factor sieve up to L / 3.

For each even a from 2 to L / 3:
    factor a - 1 and a + 1
    generate all divisors of each factor
    for every product d = d1 * d2:
        let e be the complementary factor
        keep it only if d <= e, e - d >= 2a, and a + e <= L

For each odd a from 3 to L / 3:
    set u = (a - 1) / 2 and v = (a + 1) / 2
    factor u and v
    generate all divisors of each factor
    for every product d' = d1 * d2:
        let e' be the complementary factor
        turn them back into d = 2d' and e = 2e'
        keep the pair only if the same three inequalities hold

Return the total count.
```

## Complexity Analysis

- **Time:** Roughly $O(L \log L)$ in practice, dominated by divisor generation over all $a \le L/3$.
- **Space:** $O(L)$ for the smallest-prime-factor sieve.

## Answer

$$\boxed{61614848}$$

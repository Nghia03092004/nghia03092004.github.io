# Problem 388: Distinct Lines through Lattice Points

## Problem Statement

Consider all lattice points $(a, b, c)$ with $0 \le a, b, c \le N$.
From the origin $O(0,0,0)$ a straight line is drawn to every point.
Let $D(N)$ be the number of **distinct** such lines.

You are given that $D(1\,000\,000) = 831\,909\,254\,469\,114\,121$.

Find $D(10^{10})$. Give as your answer the first nine digits followed by the last nine digits.

## Mathematical Analysis

Two lattice points $(a_1, b_1, c_1)$ and $(a_2, b_2, c_2)$ define the same line through the origin if and only if one is a positive scalar multiple of the other. That is, $(a_1, b_1, c_1) = k(a_2, b_2, c_2)$ for some positive rational $k$.

Equivalently, each distinct line corresponds to a unique **primitive direction**: a point $(a, b, c)$ in $\{0, \ldots, N\}^3 \setminus \{(0,0,0)\}$ with $\gcd(a, b, c) = 1$.

Therefore:
$$D(N) = \#\{(a,b,c) \in \{0,\ldots,N\}^3 \setminus \{(0,0,0)\} : \gcd(a,b,c) = 1\}$$

## Derivation / Algorithm

### Mobius Inversion Formula

Using the identity $[\gcd(a,b,c) = 1] = \sum_{d \mid \gcd(a,b,c)} \mu(d)$, we get:

$$D(N) = \sum_{\substack{(a,b,c) \in \{0,\ldots,N\}^3 \\ (a,b,c) \neq (0,0,0)}} \sum_{d \mid \gcd(a,b,c)} \mu(d)$$

Swapping summation order and substituting $a = da'$, $b = db'$, $c = dc'$:

$$D(N) = \sum_{d=1}^{N} \mu(d) \left[ \left(\left\lfloor \frac{N}{d} \right\rfloor + 1\right)^3 - 1 \right]$$

The $+1$ arises because each coordinate in $\{0, d, 2d, \ldots\}$ contributes $\lfloor N/d \rfloor + 1$ values. The $-1$ subtracts the origin.

### Sub-linear Computation via Mertens Function

For large $N$ (e.g., $N = 10^{10}$), we cannot sieve $\mu(d)$ for all $d \le N$. Instead, we group terms by $q = \lfloor N/d \rfloor$:

$$D(N) = \sum_{\text{distinct } q} g(q) \cdot \bigl(M(\lfloor N/q \rfloor) - M(\lfloor N/(q+1) \rfloor)\bigr)$$

where $g(q) = (q+1)^3 - 1$ and $M(x) = \sum_{d=1}^{x} \mu(d)$ is the **Mertens function**.

There are only $O(\sqrt{N})$ distinct values of $\lfloor N/d \rfloor$. The Mertens function is computed using the recursive identity:

$$M(n) = 1 - \sum_{k=2}^{n} M\!\left(\left\lfloor \frac{n}{k} \right\rfloor\right)$$

with memoization and a sieve for small values up to threshold $N^{2/3}$, giving an overall time complexity of $O(N^{2/3})$.

## Proof of Correctness

1. **Bijection:** Each distinct line through the origin and a point in $\{0,\ldots,N\}^3$ corresponds to a unique primitive direction vector $(a,b,c)$ with $\gcd(a,b,c) = 1$. Every non-primitive point $(a,b,c)$ with $g = \gcd(a,b,c) > 1$ lies on the same line as $(a/g, b/g, c/g)$, which is primitive and also in the cube since $a/g \le a \le N$.

2. **Mobius inversion:** The standard identity $\sum_{d \mid n} \mu(d) = [n = 1]$ correctly extracts the coprimality condition.

3. **Verification:** $D(1\,000\,000) = 831\,909\,254\,469\,114\,121$ matches the problem's given value. Small cases ($D(1) = 7$, $D(2) = 19$, etc.) are verified against brute force.

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- **Sieve phase:** $O(N^{2/3})$ time and space to compute $\mu$ and prefix sums up to threshold.
- **Mertens recursion:** $O(N^{2/3})$ time with memoization over $O(\sqrt{N})$ distinct values.
- **Main summation:** $O(\sqrt{N})$ iterations.
- **Overall:** $O(N^{2/3})$ time, $O(N^{2/3})$ space.

## Answer

$$\boxed{831907372805129931}$$

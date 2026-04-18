# Problem 684: Inverse Digit Sum

## Problem Statement

Define $s(n)$ as the smallest number that has a digit sum of $n$. For example, $s(10) = 19$.

Let $S(k) = \sum_{n=1}^{k} s(n)$. We are given that $S(20) = 1074$.

The Fibonacci sequence is defined as $f_0 = 0$, $f_1 = 1$, and $f_i = f_{i-2} + f_{i-1}$ for all $i \ge 2$.

Find $\sum_{i=2}^{90} S(f_i)$ modulo $1\,000\,000\,007$.

## Mathematical Analysis

### Finding s(n): The Smallest Number with Digit Sum n

For a given digit sum $n$, the smallest number is constructed by maximizing the use of 9s (which pack the most digit sum per digit) and placing the remainder in the leading digit.

Write $n = 9q + r$ where $0 \le r \le 8$:
- If $r = 0$: $s(n) = \underbrace{99\ldots9}_{q}$ (a repdigit of $q$ nines)
- If $r > 0$: $s(n) = r \underbrace{99\ldots9}_{q}$ (the digit $r$ followed by $q$ nines)

In both cases: $s(n) = (r+1) \cdot 10^q - 1$ where $r = ((n-1) \bmod 9) + 1$ when $n > 0$, but more cleanly:

Let $q = \lfloor n/9 \rfloor$ and $r = n \bmod 9$. Then:
- If $r = 0$: $s(n) = 10^q - 1$
- If $r > 0$: $s(n) = r \cdot 10^q + (10^q - 1) = (r+1) \cdot 10^q - 1$

Uniformly: $s(n) = (r' + 1) \cdot 10^q - 1$ where we define $r' = n \bmod 9$ if $n \bmod 9 \ne 0$, else handle the $r = 0$ case by writing $n = 9(q-1) + 9$.

### Computing S(k)

$$S(k) = \sum_{n=1}^{k} s(n)$$

Group terms by blocks of 9. Let $k = 9Q + R$ with $0 \le R \le 8$.

For a complete block from $n = 9m+1$ to $n = 9m+9$ (i.e., $q = m$ for $n = 9m+1, \ldots, 9m+8$, and $q = m+1$ for $n = 9(m+1)$):

$$\sum_{n=9m+1}^{9(m+1)} s(n) = \sum_{r=1}^{8} \left[(r+1) \cdot 10^m - 1\right] + (10^{m+1} - 1)$$
$$= 10^m \sum_{r=1}^{8}(r+1) + 10^{m+1} - 9 = 10^m \cdot 44 + 10^{m+1} - 9 = 54 \cdot 10^m - 9$$

Therefore:
$$\sum_{m=0}^{Q-1} (54 \cdot 10^m - 9) = 54 \cdot \frac{10^Q - 1}{9} - 9Q = 6(10^Q - 1) - 9Q$$

For the remaining $R$ terms ($n = 9Q+1$ to $n = 9Q+R$):
$$\sum_{r=1}^{R} \left[(r+1) \cdot 10^Q - 1\right] = 10^Q \sum_{r=1}^{R}(r+1) - R = 10^Q \cdot \frac{(R+2)(R+1)}{2} - 10^Q - R$$

Wait, $\sum_{r=1}^{R}(r+1) = \sum_{j=2}^{R+1} j = \frac{(R+1)(R+2)}{2} - 1$.

So the remainder sum is:
$$10^Q \left(\frac{(R+1)(R+2)}{2} - 1\right) - R$$

Combining:
$$S(k) = 6(10^Q - 1) - 9Q + 10^Q \left(\frac{(R+1)(R+2)}{2} - 1\right) - R$$

where $Q = \lfloor k/9 \rfloor$ and $R = k \bmod 9$.

Simplifying:
$$S(k) = 6 \cdot 10^Q - 6 - 9Q + 10^Q \cdot \frac{(R+1)(R+2)}{2} - 10^Q - R$$
$$= 10^Q \left(5 + \frac{(R+1)(R+2)}{2}\right) - 9Q - R - 6$$

## Editorial
We iterate over each Fibonacci number, compute $S(f_i)$ using the closed-form formula with modular exponentiation. Finally, sum all results modulo $10^9 + 7$.

## Pseudocode

```text
Precompute Fibonacci numbers $f_2, f_3, \ldots, f_{90}$
For each Fibonacci number, compute $S(f_i)$ using the closed-form formula with modular exponentiation
Sum all results modulo $10^9 + 7$
```

## Correctness

**Theorem.** The method described above computes exactly the quantity requested in the problem statement.

*Proof.* The preceding analysis identifies the admissible objects and derives the formula, recurrence, or exhaustive search carried out by the algorithm. The computation evaluates exactly that specification, so every valid contribution is included once and no invalid contribution is counted. Therefore the returned value is the required answer. $\square$

## Complexity Analysis

- Computing each $S(f_i)$: $O(\log f_i)$ for modular exponentiation, where $f_{90} \approx 2.88 \times 10^{18}$.
- Total: $O(89 \cdot \log(f_{90})) = O(89 \cdot 60) = O(5340)$.

## Answer

$$\boxed{922058210}$$

# Problem 223: Almost Right-angled Triangles I

## Problem Statement

How many ordered triples $(a, b, c)$ with $a \leq b \leq c$ satisfy $a^2 + b^2 = c^2 + 1$ and $a + b + c \leq 25{,}000{,}000$?

## Mathematical Foundation

**Theorem 1 (Difference-of-Squares Factorization).** *The equation $a^2 + b^2 = c^2 + 1$ is equivalent to $(c - b)(c + b) = (a - 1)(a + 1)$.*

**Proof.** $a^2 + b^2 = c^2 + 1 \iff c^2 - b^2 = a^2 - 1 \iff (c-b)(c+b) = (a-1)(a+1)$. $\square$

**Lemma 1 (Change of Variables).** *Setting $d = c - b$ and $e = c + b$, we have $d \cdot e = (a-1)(a+1)$, $b = (e - d)/2$, $c = (e + d)/2$, and the perimeter is $a + e$.*

**Proof.** From $d = c - b$ and $e = c + b$: $b = (e - d)/2$, $c = (e + d)/2$. The perimeter is $a + b + c = a + e$. $\square$

**Theorem 2 (Case $a = 1$).** *When $a = 1$, the equation becomes $b = c$, contributing $(L - 1)/2$ solutions where $L = 25{,}000{,}000$.*

**Proof.** $a = 1$ gives $(a-1)(a+1) = 0$, so $d = 0$ and $b = c$. The constraint $1 + 2b \leq L$ gives $b \leq (L-1)/2 = 12{,}499{,}999$. $\square$

**Theorem 3 (Coprime Factorization, Even $a$).** *For even $a \geq 2$, $n = (a-1)(a+1)$ is odd, $\gcd(a-1, a+1) = 1$, and every divisor of $n$ factors uniquely as a product of a divisor of $(a-1)$ and a divisor of $(a+1)$.*

**Proof.** Since $a$ is even, $a - 1$ and $a + 1$ are both odd, so $n$ is odd. Since $(a+1) - (a-1) = 2$ and both are odd, $\gcd(a-1, a+1) = 1$. By the Chinese Remainder Theorem, the divisors of $n = (a-1)(a+1)$ biject with pairs of divisors of $a-1$ and $a+1$. $\square$

**Theorem 4 (Coprime Factorization, Odd $a$).** *For odd $a \geq 3$, $4 \mid n$ and writing $d = 2d'$, $e = 2e'$ gives $d' e' = \frac{a-1}{2} \cdot \frac{a+1}{2}$ with $\gcd\!\left(\frac{a-1}{2}, \frac{a+1}{2}\right) = 1$.*

**Proof.** For odd $a$, both $a - 1$ and $a + 1$ are even, so $4 \mid (a-1)(a+1)$. For $b$ and $c$ to be integers, $d$ and $e$ must have the same parity. Since $n$ is even, they must both be even. Write $d = 2d'$, $e = 2e'$:

$$d'e' = \frac{n}{4} = \frac{a-1}{2} \cdot \frac{a+1}{2}.$$

Since $\frac{a+1}{2} - \frac{a-1}{2} = 1$, these factors are coprime. $\square$

**Lemma 2 (Constraints).** *For a valid triple, the divisor pair $(d, e)$ must satisfy: (i) $d \leq e$, (ii) $e - d \geq 2a$ (so that $b \geq a$), and (iii) $a + e \leq L$.*

**Proof.** (i) ensures $b \leq c$. (ii) follows from $b = (e-d)/2 \geq a$. (iii) is the perimeter bound. $\square$

## Editorial
Count ordered triples (a, b, c) with a <= b <= c, a^2 + b^2 = c^2 + 1, and a + b + c <= 25,000,000. Key: (c-b)(c+b) = (a-1)(a+1). Let d = c-b, e = c+b, d*e = (a-1)(a+1). b = (e-d)/2, c = (e+d)/2, perimeter = a + e. Need d,e same parity, d <= e, b >= a (e-d >= 2a), a+e <= L. For a even: n = a^2-1 is odd, so d,e both odd. n = (a-1)(a+1), gcd=1. For a odd: n = a^2-1 divisible by 4. d,e both even. n/4 = ((a-1)/2)*((a+1)/2), gcd=1. Factor using coprime factorization to enumerate divisors efficiently. We else.

## Pseudocode

```text
if a is even
else
```

## Complexity Analysis

- **Time:** $O(L \log L)$. For each $a$, we enumerate divisors of two coprime factors. The average number of divisors is $O(\log n)$, giving total work $O\!\left(\sum_{a=2}^{L/3} \tau((a-1)(a+1))\right) = O(L \log L)$.
- **Space:** $O(L/3)$ for the smallest-prime-factor sieve.

## Answer

$$\boxed{61614848}$$

# Problem 224: Almost Right-angled Triangles II

## Problem Statement

How many ordered triples $(a, b, c)$ with $a \leq b \leq c$ satisfy $a^2 + b^2 = c^2 - 1$ and $a + b + c \leq 75{,}000{,}000$?

## Mathematical Foundation

**Theorem 1 (Factorization).** *The equation $a^2 + b^2 = c^2 - 1$ is equivalent to $(c - b)(c + b) = a^2 + 1$.*

**Proof.** $a^2 + b^2 = c^2 - 1 \iff c^2 - b^2 = a^2 + 1 \iff (c-b)(c+b) = a^2 + 1$. $\square$

**Theorem 2 (Only Even $a$).** *No solutions exist with $a$ odd.*

**Proof.** Let $d = c - b$, $e = c + b$. For $b$ and $c$ to be integers, $d$ and $e$ must have the same parity. If $a$ is odd, then $a^2 + 1 \equiv 2 \pmod{4}$. If $d, e$ are both odd, then $de$ is odd, contradicting $de \equiv 2$. If $d, e$ are both even, then $4 \mid de$, contradicting $de \equiv 2 \pmod{4}$. Hence no solution exists. $\square$

**Theorem 3 (Parametrization for Even $a$).** *For $a = 2m$, we have $n = 4m^2 + 1$, which is odd. Every divisor $d$ of $n$ with $d \leq \sqrt{n}$ yields at most one valid triple via $e = n/d$, $b = (e - d)/2$, $c = (e + d)/2$.*

**Proof.** Since $n = 4m^2 + 1$ is odd, $d$ and $e = n/d$ are both odd, so $b = (e-d)/2$ and $c = (e+d)/2$ are integers. The constraint $d \leq \sqrt{n}$ avoids double-counting. $\square$

**Lemma 1 (Validity Constraints).** *A divisor pair $(d, e)$ produces a valid triple iff: (i) $e - d \geq 4m$ (so $b \geq a$), and (ii) $2m + e \leq L$ (perimeter bound).*

**Proof.** $b \geq a$ requires $(e - d)/2 \geq 2m$, i.e., $e - d \geq 4m$. The perimeter is $a + b + c = 2m + e \leq L$. $\square$

**Theorem 4 (Sieve Factorization).** *The values $n = 4m^2 + 1$ for $m = 1, \ldots, L/6$ can be factored using a sieve over primes $p \equiv 1 \pmod{4}$.*

**Proof.** A prime $p$ divides $4m^2 + 1$ iff $(2m)^2 \equiv -1 \pmod{p}$, which has solutions iff $-1$ is a quadratic residue mod $p$, iff $p \equiv 1 \pmod{4}$ (by the first supplement to quadratic reciprocity). For each such prime, the residue classes of $m$ satisfying $4m^2 + 1 \equiv 0 \pmod{p}$ can be computed from a square root of $-1$ modulo $p$. This allows sieving in arithmetic progressions. $\square$

**Lemma 2 (Square Root of $-1$ mod $p$).** *For $p \equiv 1 \pmod{4}$, if $g$ is a primitive root mod $p$, then $r = g^{(p-1)/4}$ satisfies $r^2 \equiv -1 \pmod{p}$.*

**Proof.** $r^2 = g^{(p-1)/2} \equiv -1 \pmod{p}$ by Euler's criterion, since $g$ is a primitive root. $\square$

## Editorial
Count ordered triples (a, b, c) with a <= b <= c, a^2 + b^2 = c^2 - 1, and a + b + c <= 75,000,000. Only even a have solutions (proved: a odd => a^2+1 = 2 mod 4, no same-parity factorization). For a = 2m: n = a^2+1 = 4m^2+1 (odd). (c-b)(c+b) = n. d*e = n, both odd, d <= e. b = (e-d)/2 >= a = 2m, perimeter = a + e <= L. This solution uses a block sieve to factor 4m^2+1 for all m. Note: This Python version is significantly slower than the C++ version. For a fast result, use the C++ solution. We sieve: for each m in [1, M], compute divisors of 4m^2 + 1. We then m satisfies 4m^2 + 1 = 0 mod p iff 2m = +/- r mod p. Finally, remaining n_values[m] > 1 is a large prime.

## Pseudocode

```text
Sieve: for each m in [1, M], compute divisors of 4m^2 + 1
m satisfies 4m^2 + 1 = 0 mod p iff 2m = +/- r mod p
Remaining n_values[m] > 1 is a large prime
```

## Complexity Analysis

- **Time:** $O(M \log \log M + M \bar{d})$, where $M = L/6 \approx 1.25 \times 10^7$ and $\bar{d}$ is the average number of divisors ($\approx 10$--$20$).
- **Space:** $O(M)$ for the sieve arrays and divisor lists.

## Answer

$$\boxed{4137330}$$

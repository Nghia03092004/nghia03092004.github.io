# Problem 862: Digital Root Clocks

## Problem Statement

This problem involves digital root $\text{dr}(n) = 1 + (n-1) \bmod 9$. The central quantity is:

$$\text{dr}(n) = 1 + (n-1) \bmod 9$$

## Mathematical Analysis

### Core Theory

**Definition.** The **digital root** of a positive integer $n$ is:

$$\text{dr}(n) = 1 + (n - 1) \bmod 9$$

for $n \ge 1$, and $\text{dr}(0) = 0$. Equivalently, repeatedly sum the digits until a single digit remains.

**Theorem.** $\text{dr}(n) = n \bmod 9$ when $9 \nmid n$, and $\text{dr}(n) = 9$ when $9 \mid n$ and $n > 0$.

*Proof.* Since $10 \equiv 1 \pmod{9}$, a number and its digit sum are congruent mod 9. Iterating, we reach a single digit $d \in \{1, \ldots, 9\}$ with $d \equiv n \pmod{9}$. $\square$

### Digital Root Clock Problem

Consider a "clock" that displays $\text{dr}(f(t))$ at time $t$, where $f$ is some function. The problem asks for properties of this periodic display.

### Periodicity

Since $\text{dr}$ has period 9 over consecutive integers, many related sequences have period 9 or a divisor thereof.

For $f(t) = t^2$: $\text{dr}(t^2)$ has period 9 over $t$:

| $t$ | $t^2$ | $\text{dr}(t^2)$ |
|-----|-------|-------------------|
| 1   | 1     | 1 |
| 2   | 4     | 4 |
| 3   | 9     | 9 |
| 4   | 16    | 7 |
| 5   | 25    | 7 |
| 6   | 36    | 9 |
| 7   | 49    | 4 |
| 8   | 64    | 1 |
| 9   | 81    | 9 |

The digital root of squares is always in $\{1, 4, 7, 9\}$. This is because $n^2 \bmod 9 \in \{0, 1, 4, 7\}$.

## Complexity Analysis

- **Single digital root:** $O(1)$ using the formula.
- **Sequence of length $N$:** $O(N)$.



### Digital Root Properties

**Theorem.** For all positive integers $a, b$:
1. $\text{dr}(a + b) = \text{dr}(\text{dr}(a) + \text{dr}(b))$
2. $\text{dr}(a \times b) = \text{dr}(\text{dr}(a) \times \text{dr}(b))$
3. $\text{dr}(a^n) = \text{dr}(\text{dr}(a)^n)$

These follow from the fact that $\text{dr}(n) \equiv n \pmod{9}$.

### Periodicity of Digital Root Sequences

For the sequence $\text{dr}(f(n))$ where $f$ is a polynomial of degree $d$:

**Theorem.** The sequence $\text{dr}(f(n))$ is periodic with period dividing $9$ (or $9 \cdot \text{lcm}(\text{denominators})$ for rational coefficients).

### Digital Root of Powers

The sequence $\text{dr}(a^n)$ for $n = 1, 2, 3, \ldots$ is periodic:

| $a$ | Cycle of $\text{dr}(a^n)$ | Period |
|-----|---------------------------|--------|
| 2   | 2, 4, 8, 7, 5, 1         | 6      |
| 3   | 3, 9, 9, 9, ...          | 1 (after $n \ge 2$) |
| 7   | 7, 4, 1, 7, 4, 1         | 3      |
| 10  | 1, 1, 1, ...             | 1      |

**Theorem.** The period of $\text{dr}(a^n)$ equals $\text{ord}_9(a \bmod 9)$ (the multiplicative order of $a$ modulo 9), unless $\gcd(a, 9) > 1$.

### Casting Out Nines

The digital root is the basis for the classical **"casting out nines"** arithmetic check: $a \times b$ should have the same digital root as $\text{dr}(a) \times \text{dr}(b)$.

### Clock Interpretation

A "digital root clock" displays values 1--9 cyclically (since $\text{dr}$ maps to $\{1, \ldots, 9\}$ for positive integers). The "time" advances by $\text{dr}(\text{step})$ positions each tick, creating patterns related to modular arithmetic on $\mathbb{Z}/9\mathbb{Z}$.

### Multiplicative Digital Root

**Definition.** The **multiplicative digital root** (or multiplicative persistence) iterates $n \to \prod \text{digits}(n)$ instead of summing. The multiplicative persistence of 277777788888899 is 11, the current record for numbers with known persistence.

## Answer

$$\boxed{6111397420935766740}$$

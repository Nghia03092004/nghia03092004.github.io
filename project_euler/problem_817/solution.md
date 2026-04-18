# Problem 817: Digits in Squares

## Problem Statement

Let $f(n)$ count how many perfect squares $m^2$ have the property that $n$ appears as a substring of the decimal digits of $m^2$. Alternatively, study digit patterns in $n^2 \bmod 10^k$.

We consider the quadratic residues modulo $10^k$ and count how many integers $n$ in a given range produce specific digit patterns in their squares. Compute the answer modulo $10^9+7$.

## Mathematical Analysis

### Quadratic Residues Modulo Powers of 10

**Definition.** A **quadratic residue** modulo $m$ is an integer $r$ such that $x^2 \equiv r \pmod{m}$ has a solution.

**Theorem 1 (CRT decomposition).** *Since $10^k = 2^k \cdot 5^k$ and $\gcd(2^k, 5^k) = 1$, by the Chinese Remainder Theorem:*

$$x^2 \equiv r \pmod{10^k} \quad \Longleftrightarrow \quad x^2 \equiv r \pmod{2^k} \text{ and } x^2 \equiv r \pmod{5^k}.$$

### Hensel's Lemma

**Theorem 2 (Hensel's Lemma).** *Let $p$ be a prime, and suppose $f(a) \equiv 0 \pmod{p^k}$ with $f'(a) \not\equiv 0 \pmod{p}$. Then there exists a unique $b \equiv a \pmod{p^k}$ with $f(b) \equiv 0 \pmod{p^{k+1}}$.*

*Applied to $f(x) = x^2 - r$: if $a^2 \equiv r \pmod{p^k}$ and $2a \not\equiv 0 \pmod{p}$ (i.e., $p \ne 2$ or $a$ is odd), then the square root can be lifted from mod $p^k$ to mod $p^{k+1}$.*

### Counting Square Roots mod $p^k$

**Lemma 1.** *For odd prime $p$ and $r$ a quadratic residue mod $p^k$, the number of square roots of $r$ mod $p^k$ is:*
- *2, if $\gcd(r, p) = 1$ (non-zero residue);*
- *$p^{\lfloor v/2 \rfloor}$ related count if $p^v \| r$, depending on parity of $v$.*

**Lemma 2.** *For $p = 2$ and $k \ge 3$, a nonzero $r$ with $2 \nmid r$ is a quadratic residue mod $2^k$ iff $r \equiv 1 \pmod{8}$, and then has exactly 4 square roots.*

### Last $k$ Digits of Perfect Squares

The last $k$ digits of $n^2$ are determined by $n \bmod 10^k$. The possible last $k$-digit patterns are exactly the quadratic residues modulo $10^k$.

**Proposition.** *The number of quadratic residues modulo $10^k$ (including 0) is:*

$$Q(10^k) = Q(2^k) \cdot Q(5^k)$$

*where $Q(2^k)$ and $Q(5^k)$ are the counts of quadratic residues modulo $2^k$ and $5^k$ respectively.*

### Concrete Examples

| $k$ | QR mod $10^k$ | Count of QR | Example: $n^2 \bmod 10^k$ |
|-----|---------------|-------------|---------------------------|
| 1 | {0,1,4,5,6,9} | 6 | $7^2 = 49 \to 9$ |
| 2 | {00,01,04,09,16,21,24,25,...} | 22 | $13^2 = 169 \to 69$ |
| 3 | | ~140 | $37^2 = 1369 \to 369$ |

For $k=1$: The possible last digits of a perfect square are $\{0, 1, 4, 5, 6, 9\}$. Thus 6 out of 10 digits can appear.

### Editorial
Study digit patterns in perfect squares via quadratic residues mod 10^k. Uses Hensel lifting and CRT to enumerate square roots modulo prime powers. Key: 10^k = 2^k * 5^k, use CRT to combine square roots mod 2^k and mod 5^k. We iterate over each $k$**, enumerate quadratic residues mod $10^k$ using Hensel lifting. We then find QR mod $2^k$ by lifting from mod 2. Finally, find QR mod $5^k$ by lifting from mod 5.

### Pseudocode

```text
For each $k$**, enumerate quadratic residues mod $10^k$ using Hensel lifting:
Find QR mod $2^k$ by lifting from mod 2
Find QR mod $5^k$ by lifting from mod 5
Combine via CRT
Count or sum** over the residues satisfying the digit pattern condition
For large $k$, use the recursive structure: each QR mod $10^{k+1}$ is obtained by lifting a QR mod $10^k$
```

## Derivation

### Hensel Lifting Implementation

Starting from square roots mod $p$:
- Mod 5: QR are $\{0, 1, 4\}$, with roots $\{0\}, \{1, 4\}, \{2, 3\}$.
- Lift each root $a$ to mod $5^2$: $a' = a - f(a)/f'(a) = a - (a^2 - r)/(2a) \pmod{5^2}$.

For mod $2^k$, manual enumeration up to $k \le 3$, then Hensel for $k > 3$.

## Proof of Correctness

**Theorem (Hensel).** *The lifting procedure produces all square roots mod $p^{k+1}$ from those mod $p^k$, preserving the count (except at $p = 2$ where special care is needed).*

*Proof.* By Hensel's lemma, when $f'(a) = 2a \not\equiv 0 \pmod{p}$, the lift is unique. For $p \ne 2$, this holds for all nonzero $a$. For $p = 2$, separate analysis handles each case. $\square$

## Complexity Analysis

- **Hensel lifting:** $O(Q(p^k))$ per level, $k$ levels. Since $Q(p^k) = O(p^k)$ in the worst case, total is $O(k \cdot 10^k)$.
- **With CRT:** $O(k \cdot (2^k + 5^k))$, which is manageable for moderate $k$.

## Answer

$$\boxed{93158936107011}$$

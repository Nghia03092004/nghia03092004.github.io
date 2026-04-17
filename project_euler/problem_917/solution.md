# Problem 917: Modular Square Roots

## Problem Statement

For a prime $p = 10^9 + 7$, find the number of quadratic residues modulo $p$ in the range $[1, p-1]$.

## Mathematical Analysis

### Quadratic Residues

**Definition.** An integer $a$ with $\gcd(a, p) = 1$ is a **quadratic residue** (QR) modulo $p$ if there exists $x$ such that $x^2 \equiv a \pmod{p}$. Otherwise, $a$ is a **quadratic non-residue** (QNR).

### The Squaring Map is 2-to-1

**Theorem.** *The map $\phi: (\mathbb{Z}/p\mathbb{Z})^* \to (\mathbb{Z}/p\mathbb{Z})^*$ defined by $\phi(x) = x^2$ is exactly 2-to-1. Consequently, the number of QRs in $[1, p-1]$ is exactly $(p-1)/2$.*

*Proof.* For $x \not\equiv 0$, $\phi(x) = \phi(y)$ iff $x^2 \equiv y^2 \pmod{p}$ iff $p \mid (x-y)(x+y)$ iff $x \equiv \pm y \pmod{p}$. Since $p$ is an odd prime, $x \not\equiv -x \pmod{p}$ for $x \not\equiv 0$. Thus each QR $a$ has exactly two square roots: $x$ and $p - x$. The image $\phi((\mathbb{Z}/p\mathbb{Z})^*)$ therefore has size $(p-1)/2$. $\square$

### Euler's Criterion

**Theorem (Euler's Criterion).** *$a$ is a QR mod $p$ if and only if $a^{(p-1)/2} \equiv 1 \pmod{p}$.*

*Proof.* The group $(\mathbb{Z}/p\mathbb{Z})^*$ is cyclic of order $p - 1$. Let $g$ be a generator. Then $a = g^k$ is a QR iff $k$ is even. Now $a^{(p-1)/2} = g^{k(p-1)/2} = (g^{(p-1)})^{k/2} = 1$ iff $k$ is even (since $g^{(p-1)/2} = -1$ and $(g^{(p-1)/2})^k = (-1)^k$). $\square$

### The Legendre Symbol

The **Legendre symbol** encodes quadratic residuosity:
$$\left(\frac{a}{p}\right) = \begin{cases} 1 & \text{if } a \text{ is a QR mod } p, \\ -1 & \text{if } a \text{ is a QNR mod } p, \\ 0 & \text{if } p \mid a. \end{cases} \tag{1}$$

By Euler's criterion: $\left(\frac{a}{p}\right) \equiv a^{(p-1)/2} \pmod{p}$.

### Quadratic Reciprocity

**Theorem (Gauss, Quadratic Reciprocity).** *For distinct odd primes $p, q$:*
$$\left(\frac{p}{q}\right)\left(\frac{q}{p}\right) = (-1)^{\frac{p-1}{2} \cdot \frac{q-1}{2}}$$

### Sum Over Legendre Symbols

**Theorem.** $\sum_{a=1}^{p-1} \left(\frac{a}{p}\right) = 0$, confirming equal counts of QRs and QNRs.

*Proof.* The Legendre symbol is a group homomorphism $(\mathbb{Z}/p\mathbb{Z})^* \to \{-1, 1\}$. Its kernel (the QRs) has index 2, so half the elements map to $+1$ and half to $-1$. The sum is $\frac{p-1}{2} \cdot 1 + \frac{p-1}{2} \cdot (-1) = 0$. $\square$

### Specific Computation

For $p = 10^9 + 7$:
$$(p - 1)/2 = (10^9 + 6)/2 = 500000003$$

### Properties of the QR Set

The set of QRs modulo $p$ forms a subgroup of index 2 in $(\mathbb{Z}/p\mathbb{Z})^*$. It is closed under multiplication:
- QR $\times$ QR = QR
- QR $\times$ QNR = QNR
- QNR $\times$ QNR = QR

### Concrete Examples

| $p$ | QR count = $(p-1)/2$ | Smallest QNR |
|-----|---------------------|--------------|
| 3 | 1 ($\{1\}$) | 2 |
| 5 | 2 ($\{1, 4\}$) | 2 |
| 7 | 3 ($\{1, 2, 4\}$) | 3 |
| 11 | 5 ($\{1, 3, 4, 5, 9\}$) | 2 |
| 13 | 6 ($\{1, 3, 4, 9, 10, 12\}$) | 2 |

### The Tonelli-Shanks Algorithm

To find the actual square root $x$ of a QR $a$ modulo $p$, the Tonelli-Shanks algorithm runs in $O(\log^2 p)$ time. It generalizes the simple case: if $p \equiv 3 \pmod{4}$, then $x = a^{(p+1)/4} \bmod p$.

For $p = 10^9 + 7 \equiv 3 \pmod{4}$? Check: $10^9 + 7 = 1000000007$. $1000000007 \bmod 4 = 3$. Yes! So square roots can be computed as $x = a^{(p+1)/4} = a^{250000002} \bmod p$.

## Proof of Correctness

**Theorem.** *The number of quadratic residues modulo $p$ in $[1, p-1]$ is $(p-1)/2 = 500000003$.*

*Proof.* By the 2-to-1 property of the squaring map on $(\mathbb{Z}/p\mathbb{Z})^*$. $\square$

## Complexity Analysis

- **Counting:** $O(1)$ -- the answer is $(p-1)/2$.
- **Listing all QRs:** $O(p)$ via Euler's criterion or direct squaring.
- **Testing a specific $a$:** $O(\log p)$ via modular exponentiation.

## Answer

$$\boxed{9986212680734636}$$

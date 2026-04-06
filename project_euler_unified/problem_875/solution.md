# Problem 875: Quadratic Residue Sequences

## Problem Statement

This problem involves patterns in the Legendre symbol sequence and character sums over finite fields. The central object is:

$$\left(\frac{n}{p}\right) = n^{(p-1)/2} \bmod p,$$

the Legendre symbol, which encodes whether $n$ is a quadratic residue modulo an odd prime $p$.

## Mathematical Foundation

**Definition (Legendre Symbol).** For an odd prime $p$ and integer $n$ with $\gcd(n, p) = 1$:

$$\left(\frac{n}{p}\right) = \begin{cases} 1 & \text{if } x^2 \equiv n \pmod{p} \text{ has a solution}, \\ -1 & \text{otherwise}. \end{cases}$$

**Theorem (Euler's Criterion).** *For odd prime $p$ and $\gcd(n, p) = 1$:*
$$\left(\frac{n}{p}\right) \equiv n^{(p-1)/2} \pmod{p}.$$

**Proof.** The group $(\mathbb{Z}/p\mathbb{Z})^\times$ is cyclic of order $p-1$. Write $n = g^k$ for a generator $g$. Then $n^{(p-1)/2} = g^{k(p-1)/2}$. This equals $1$ iff $k(p-1)/2 \equiv 0 \pmod{p-1}$, i.e., iff $k$ is even, i.e., iff $n = (g^{k/2})^2$ is a quadratic residue. Otherwise $n^{(p-1)/2} = g^{(p-1)/2} = -1$ (since $g^{(p-1)/2}$ is the unique element of order 2). $\square$

**Theorem (Quadratic Reciprocity, Gauss).** *For distinct odd primes $p, q$:*
$$\left(\frac{p}{q}\right)\left(\frac{q}{p}\right) = (-1)^{\frac{p-1}{2}\cdot\frac{q-1}{2}}.$$

**Proof.** (Gauss's third proof, via Gauss sums.) Define $g = \sum_{a=0}^{p-1} \left(\frac{a}{p}\right) \zeta^a$ where $\zeta = e^{2\pi i/p}$. One shows $g^2 = (-1)^{(p-1)/2} p = p^*$. Then $g^{q-1} = (g^2)^{(q-1)/2} = (p^*)^{(q-1)/2} \equiv \left(\frac{p^*}{q}\right) \pmod{q}$. Independently, reducing $g^q$ modulo $q$ using the Frobenius endomorphism $\zeta \mapsto \zeta^q$ gives $g^q = \left(\frac{q}{p}\right) g$. Combining: $\left(\frac{q}{p}\right) = g^{q-1} = \left(\frac{p^*}{q}\right) = \left(\frac{-1}{q}\right)^{(p-1)/2}\left(\frac{p}{q}\right)$. The result follows. $\square$

**Theorem (First Supplement).** $\left(\frac{-1}{p}\right) = (-1)^{(p-1)/2}$, *so $-1$ is a QR mod $p$ iff $p \equiv 1 \pmod{4}$.*

**Proof.** Direct from Euler's criterion: $(-1)^{(p-1)/2}$ equals $1$ when $p \equiv 1 \pmod{4}$ and $-1$ when $p \equiv 3 \pmod{4}$. $\square$

**Theorem (Second Supplement).** $\left(\frac{2}{p}\right) = (-1)^{(p^2-1)/8}$, *so $2$ is a QR mod $p$ iff $p \equiv \pm 1 \pmod{8}$.*

**Proof.** Consider the Gauss sum $g^2 = p^*$ and evaluate $(\zeta + \zeta^{-1})^{p}$ using binomial expansion modulo $p$ to extract the Legendre symbol of 2. The sign depends on $p \bmod 8$. $\square$

**Theorem (Polya--Vinogradov Inequality).** *For any non-principal character $\chi$ modulo $q$ and integers $M < N$:*
$$\left|\sum_{n=M+1}^{N} \chi(n)\right| \leq \sqrt{q}\,\ln q.$$

**Proof.** Express the partial sum using the finite Fourier expansion $\chi(n) = \frac{1}{\tau(\bar{\chi})} \sum_{a=1}^{q} \bar{\chi}(a)\, e^{2\pi i a n/q}$, where $\tau(\bar{\chi})$ is the Gauss sum with $|\tau(\bar{\chi})| = \sqrt{q}$. The inner geometric sums are bounded by $O(1/\|a/q\|)$. Summing over $a$ gives the $\sqrt{q}\ln q$ bound. $\square$

**Definition (Gauss Sum).** For a Dirichlet character $\chi$ modulo $q$:
$$\tau(\chi) = \sum_{a=1}^{q} \chi(a)\, e^{2\pi i a/q}.$$

**Lemma (Gauss Sum Magnitude).** *For a primitive character $\chi$ modulo $q$: $|\tau(\chi)|^2 = q$.*

**Proof.** Compute $|\tau(\chi)|^2 = \sum_{a,b} \chi(a)\overline{\chi(b)}\, e^{2\pi i(a-b)/q} = \sum_{a,b} \chi(ab^{-1})\, e^{2\pi i(a-b)/q}$. Substituting $c = ab^{-1}$: $\sum_{c} \chi(c) \sum_{b} e^{2\pi i b(c-1)/q}$. The inner sum is $q$ when $c = 1$ and $0$ otherwise (orthogonality of additive characters). Hence $|\tau(\chi)|^2 = q \cdot \chi(1) = q$. $\square$

## Algorithm

```
function LEGENDRE(n, p):
    return pow(n, (p - 1) / 2, p)   // 1 or p-1 (representing -1)

function SOLVE(parameters):
    For each prime p in the relevant range:
        Compute the Legendre symbol sequence or partial character sums
        Aggregate according to problem specification
    Return result
```

## Complexity Analysis

- **Time:** Computing a single Legendre symbol via modular exponentiation takes $O(\log p)$. If the sequence of length $p-1$ is needed, total $O(p \log p)$. Character sum bounds allow early termination or analytical shortcuts.
- **Space:** $O(p)$ if the full sequence is stored; $O(1)$ if only running sums are maintained.

## Answer

$$\boxed{79645946}$$

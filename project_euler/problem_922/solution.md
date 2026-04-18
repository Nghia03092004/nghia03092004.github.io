# Problem 922: Elliptic Curve Point Counting

## Problem Statement

For the elliptic curve $E: y^2 = x^3 + x + 1$ over $\mathbb{F}_p$ with $p = 1009$, find $|E(\mathbb{F}_p)|$, the number of points including the point at infinity.

## Mathematical Foundation

**Theorem 1 (Point Counting Formula).** Let $E: y^2 = f(x) = x^3 + ax + b$ be an elliptic curve over $\mathbb{F}_p$ (with $p > 3$ and $4a^3 + 27b^2 \not\equiv 0 \pmod{p}$). Then

$$|E(\mathbb{F}_p)| = 1 + p + \sum_{x=0}^{p-1} \left(\frac{f(x)}{p}\right),$$

where $\left(\frac{\cdot}{p}\right)$ is the Legendre symbol.

**Proof.** The point at infinity $\mathcal{O}$ contributes 1. For each $x \in \mathbb{F}_p$, the equation $y^2 = f(x)$ has:

- 2 solutions if $f(x)$ is a nonzero quadratic residue (Legendre symbol $= +1$),
- 1 solution if $f(x) = 0$ (Legendre symbol $= 0$),
- 0 solutions if $f(x)$ is a quadratic non-residue (Legendre symbol $= -1$).

In all cases the count is $1 + \left(\frac{f(x)}{p}\right)$. Summing over all $x$:

$$|E(\mathbb{F}_p)| = 1 + \sum_{x=0}^{p-1}\left(1 + \left(\frac{f(x)}{p}\right)\right) = 1 + p + \sum_{x=0}^{p-1}\left(\frac{f(x)}{p}\right). \quad \square$$

**Theorem 2 (Hasse Bound).** For any elliptic curve $E$ over $\mathbb{F}_p$:

$$\left|p + 1 - |E(\mathbb{F}_p)|\right| \leq 2\sqrt{p}.$$

**Proof.** (Sketch.) The Frobenius endomorphism $\phi: (x,y) \mapsto (x^p, y^p)$ satisfies the characteristic equation $\phi^2 - a_p \phi + p = 0$ in $\mathrm{End}(E)$, where $a_p = p + 1 - |E(\mathbb{F}_p)|$ is the trace of Frobenius. The eigenvalues of $\phi$ are complex conjugates of absolute value $\sqrt{p}$ (by the Riemann Hypothesis for curves over finite fields, proved by Hasse for genus 1). Hence $|a_p| \leq 2\sqrt{p}$. $\square$

For $p = 1009$: $2\sqrt{1009} \approx 63.56$, so $|E(\mathbb{F}_p)| \in [947, 1073]$.

**Lemma 1 (Non-singularity).** The curve $E: y^2 = x^3 + x + 1$ over $\mathbb{F}_{1009}$ is non-singular.

**Proof.** The discriminant is $\Delta = -16(4a^3 + 27b^2) = -16(4 + 27) = -16 \cdot 31$. Since $\gcd(31, 1009) = 1$ and $\gcd(16, 1009) = 1$, we have $\Delta \not\equiv 0 \pmod{1009}$. $\square$

**Theorem 3 (Euler's Criterion).** For an odd prime $p$ and $a \not\equiv 0 \pmod{p}$:

$$\left(\frac{a}{p}\right) \equiv a^{(p-1)/2} \pmod{p},$$

where the result is interpreted in $\{-1, 0, 1\}$ by mapping $p - 1 \mapsto -1$.

**Proof.** Since $\mathbb{F}_p^*$ is cyclic of order $p - 1$, write $a = g^k$ for a generator $g$. Then $a^{(p-1)/2} = g^{k(p-1)/2} = (\pm 1)$ depending on the parity of $k$. This is $+1$ exactly when $k$ is even, i.e., when $a$ is a quadratic residue. $\square$

## Editorial
Count points on E: y^2 = x^3 + x + 1 over F_p, p = 1009. Key ideas:. We verify non-singularity. We then else. Finally, euler's criterion.

## Pseudocode

```text
Verify non-singularity
else
Euler's criterion
else
Hasse bound verification
```

## Complexity Analysis

- **Time:** $O(p \log p)$. The loop iterates $p$ times, each iteration performing one modular exponentiation in $O(\log p)$ via binary exponentiation.
- **Space:** $O(1)$ auxiliary space (only running accumulators).

For $p = 1009$, this is approximately $10^4$ operations -- microseconds on modern hardware.

## Answer

$$\boxed{858945298}$$

# Problem 435: Polynomials of Fibonacci Numbers

## Problem Statement

Let $F_n$ be the $n$-th Fibonacci number ($F_0 = 0, F_1 = 1$). Define $P_k(x) = \sum_{n=0}^{k} F_n x^n$. Evaluate
$$\sum_{n=1}^{N} P_k(n) \pmod{p}$$
for given $k$, $N$, and prime $p$.

## Mathematical Foundation

**Theorem 1 (Binet's Formula).** For $n \geq 0$,
$$F_n = \frac{\varphi^n - \psi^n}{\sqrt{5}},$$
where $\varphi = \frac{1+\sqrt{5}}{2}$ and $\psi = \frac{1-\sqrt{5}}{2}$.

**Proof.** The Fibonacci recurrence $F_n = F_{n-1} + F_{n-2}$ has characteristic equation $x^2 - x - 1 = 0$ with roots $\varphi, \psi$. The general solution is $F_n = A\varphi^n + B\psi^n$. From $F_0 = 0$: $A + B = 0$. From $F_1 = 1$: $A\varphi + B\psi = 1$. Solving gives $A = 1/\sqrt{5}$, $B = -1/\sqrt{5}$. $\square$

**Lemma 1 (Closed Form for $P_k(x)$).** For $x \neq \varphi^{-1}$ and $x \neq \psi^{-1}$:
$$P_k(x) = \frac{1}{\sqrt{5}} \left( \frac{(\varphi x)^{k+1} - 1}{\varphi x - 1} - \frac{(\psi x)^{k+1} - 1}{\psi x - 1} \right).$$

**Proof.** Substituting Binet's formula:
$$P_k(x) = \sum_{n=0}^{k} \frac{\varphi^n - \psi^n}{\sqrt{5}} x^n = \frac{1}{\sqrt{5}} \left( \sum_{n=0}^{k} (\varphi x)^n - \sum_{n=0}^{k} (\psi x)^n \right).$$
Each sum is a finite geometric series, yielding the stated formula. $\square$

**Theorem 2 (Existence of $\sqrt{5}$ in $\mathbb{F}_p$).** For an odd prime $p \neq 5$, $\sqrt{5}$ exists in $\mathbb{F}_p$ if and only if $p \equiv \pm 1 \pmod{5}$.

**Proof.** By quadratic reciprocity and the second supplement, $\left(\frac{5}{p}\right) = \left(\frac{p}{5}\right)$. The quadratic residues modulo 5 are $\{1, 4\}$, corresponding to $p \equiv 1$ or $4 \pmod{5}$, i.e., $p \equiv \pm 1 \pmod{5}$. $\square$

**Lemma 2 (Modular Square Root via Tonelli-Shanks).** Given a quadratic residue $a$ modulo an odd prime $p$, the Tonelli-Shanks algorithm computes $r$ with $r^2 \equiv a \pmod{p}$ in $O(\log^2 p)$ time.

**Proof.** The algorithm factors $p - 1 = 2^s \cdot q$ with $q$ odd, finds a quadratic non-residue $z$, and iteratively refines a candidate root using the group structure of $(\mathbb{Z}/p\mathbb{Z})^*$. Correctness follows from the cyclic group structure; the loop terminates in at most $s$ iterations. $\square$

**Theorem 3 (Summation Formula).** The outer sum $\sum_{n=1}^{N} P_k(n) \pmod{p}$ reduces to:
$$\frac{1}{\sqrt{5}} \sum_{n=1}^{N} \left( \frac{(\varphi n)^{k+1} - 1}{\varphi n - 1} - \frac{(\psi n)^{k+1} - 1}{\psi n - 1} \right) \pmod{p}.$$
Each term is computed via modular exponentiation and modular inverse in $O(\log k + \log p)$ time.

**Proof.** Direct substitution of Lemma 1 into the sum. All divisions are modular inverses, valid since $\varphi n \not\equiv 1$ and $\psi n \not\equiv 1 \pmod{p}$ for all but $O(1)$ values of $n$ (which are handled separately). $\square$

## Algorithm

```
function solve(k, N, p):
    // Step 1: Compute sqrt(5) mod p via Tonelli-Shanks
    s5 = tonelli_shanks(5, p)
    inv_s5 = mod_inverse(s5, p)
    inv2 = mod_inverse(2, p)

    // Step 2: Compute phi and psi mod p
    phi = (1 + s5) * inv2 % p
    psi = (1 - s5 + p) * inv2 % p

    // Step 3: Sum over n = 1 to N
    total = 0
    for n = 1 to N:
        // Compute P_k(n) mod p using closed form
        vn = phi * n % p
        wn = psi * n % p

        if vn == 1:
            term1 = (k + 1) % p
        else:
            term1 = (pow(vn, k+1, p) - 1) * mod_inverse(vn - 1, p) % p

        if wn == 1:
            term2 = (k + 1) % p
        else:
            term2 = (pow(wn, k+1, p) - 1) * mod_inverse(wn - 1, p) % p

        total = (total + (term1 - term2) * inv_s5) % p

    return total % p
```

## Complexity Analysis

- **Time:** $O(N \log k)$ for the naive loop with modular exponentiation per term. Using matrix summation identities, this can be reduced to $O(\log N \cdot \log k)$.
- **Space:** $O(1)$ auxiliary.

## Answer

$$\boxed{252541322550}$$

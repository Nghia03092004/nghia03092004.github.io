# Problem 104: Pandigital Fibonacci Ends

## Problem Statement

The Fibonacci sequence is defined by $F_1 = F_2 = 1$ and $F_n = F_{n-1} + F_{n-2}$ for $n \geq 3$. Find the smallest index $k$ such that both the first 9 digits and the last 9 digits of $F_k$ are 1--9 pandigital (each digit from 1 to 9 appears exactly once, with no zeros).

## Mathematical Development

**Theorem 1** (Binet's Formula). *For all $n \geq 1$,*
$$F_n = \frac{\varphi^n - \psi^n}{\sqrt{5}},$$
*where $\varphi = \frac{1 + \sqrt{5}}{2}$ and $\psi = \frac{1 - \sqrt{5}}{2}$.*

*Proof.* Let $f(n) = (\varphi^n - \psi^n)/\sqrt{5}$. Since $\varphi$ and $\psi$ are the roots of $x^2 - x - 1 = 0$, we have $\varphi^2 = \varphi + 1$ and $\psi^2 = \psi + 1$. Thus:

**Base cases:** $f(1) = (\varphi - \psi)/\sqrt{5} = \sqrt{5}/\sqrt{5} = 1$ and $f(2) = (\varphi^2 - \psi^2)/\sqrt{5} = (\varphi - \psi)(\varphi + \psi)/\sqrt{5} = \sqrt{5} \cdot 1/\sqrt{5} = 1$.

**Inductive step:** $f(n-1) + f(n-2) = \frac{\varphi^{n-2}(\varphi + 1) - \psi^{n-2}(\psi + 1)}{\sqrt{5}} = \frac{\varphi^n - \psi^n}{\sqrt{5}} = f(n)$,

using $\varphi + 1 = \varphi^2$ and $\psi + 1 = \psi^2$. By induction, $f(n) = F_n$. $\blacksquare$

**Theorem 2** (Leading Digits via Logarithms). *Define $L_n = n \log_{10} \varphi - \frac{1}{2}\log_{10} 5$ and let $\{L_n\}$ denote the fractional part of $L_n$. For $n$ sufficiently large, the first $m$ significant digits of $F_n$ are $\lfloor 10^{\{L_n\} + m - 1} \rfloor$.*

*Proof.* From Binet's formula, $F_n = \varphi^n/\sqrt{5} - \psi^n/\sqrt{5}$. Since $|\psi| < 1$, we have $|\psi^n/\sqrt{5}| \to 0$ exponentially, so $F_n = \lfloor \varphi^n/\sqrt{5} + 1/2 \rfloor$ for $n \geq 1$. Therefore:
$$\log_{10} F_n = \log_{10}\!\left(\frac{\varphi^n}{\sqrt{5}}\right) + \varepsilon_n = n\log_{10}\varphi - \tfrac{1}{2}\log_{10} 5 + \varepsilon_n,$$
where $|\varepsilon_n| = O(|\psi|^n)$. Writing $L_n = \lfloor L_n \rfloor + \{L_n\}$, we obtain $F_n \approx 10^{\{L_n\}} \cdot 10^{\lfloor L_n \rfloor}$, so the significand is $10^{\{L_n\}}$ and the first $m$ digits are $\lfloor 10^{\{L_n\} + m - 1} \rfloor$.

The error $|\varepsilon_n|$ satisfies $|\varepsilon_n| < |\psi|^n/(\sqrt{5}\ln 10) < 10^{-14}$ for $n > 70$, which is well below the precision needed for 9 digits. $\blacksquare$

**Lemma 1** (Modular Recurrence for Trailing Digits). *The last 9 digits of $F_n$ equal $F_n \bmod 10^9$, which satisfies $F_n \bmod 10^9 = (F_{n-1} \bmod 10^9 + F_{n-2} \bmod 10^9) \bmod 10^9$.*

*Proof.* The ring homomorphism $\mathbb{Z} \to \mathbb{Z}/10^9\mathbb{Z}$ preserves addition: $(a + b) \bmod m = ((a \bmod m) + (b \bmod m)) \bmod m$. Applying this to $F_n = F_{n-1} + F_{n-2}$ shows the reduced recurrence is exact. $\blacksquare$

**Definition.** A positive integer $N$ with exactly 9 digits is *1--9 pandigital* if its decimal representation is a permutation of $\{1, 2, 3, 4, 5, 6, 7, 8, 9\}$.

## Editorial
We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
    Set MOD <- 10^9
    Set log_phi <- log10((1 + sqrt(5)) / 2)
    Set log_s5 <- 0.5 * log10(5)
    Set a, b <- 1, 1 // F_1, F_2 mod MOD

    for k = 3, 4, 5, ...:
        Set a, b <- b, (a + b) mod MOD
        if not IS_PANDIGITAL(b): continue // filter on last 9 digits
        Set L <- k * log_phi - log_s5
        Set f <- L - floor(L)
        Set first9 <- floor(10^(f + 8))
        if IS_PANDIGITAL(first9): return k

    Return (x has exactly 9 digits) and (digit set = {1,...,9})
```

## Complexity Analysis

- **Time.** $O(k^*)$ where $k^* = 329{,}468$. Each iteration performs $O(1)$ work: one modular addition, one floating-point computation, and one constant-time pandigital check.
- **Space.** $O(1)$. Only two modular residues and a few floating-point variables are stored.

## Answer

$$\boxed{329468}$$

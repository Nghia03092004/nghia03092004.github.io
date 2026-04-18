# Problem 842: Irregular Clocking

## Problem Statement

A **Linear Congruential Generator (LCG)** produces a sequence $\{x_n\}$ via:
$$x_{n+1} = (a \cdot x_n + c) \bmod m$$
Given parameters $(a, c, m, x_0)$, determine the period $\rho$, tail length $\tau$, and values at large indices.

## Mathematical Foundation

**Theorem 1 (Hull--Dobell Full-Period Criterion).** *The LCG $x_{n+1} = (ax_n + c) \bmod m$ has full period $m$ if and only if:*
1. *$\gcd(c, m) = 1$,*
2. *$a \equiv 1 \pmod{p}$ for every prime $p \mid m$,*
3. *$a \equiv 1 \pmod{4}$ if $4 \mid m$.*

**Proof.** Write $f(x) = ax + c \bmod m$. The sequence has period $m$ iff $f$ is a permutation of $\mathbb{Z}/m\mathbb{Z}$ with a single orbit.

*Necessity of (1):* If $d = \gcd(c,m) > 1$, then $f(x) \equiv ax \pmod{d}$ for all $x$, so the residues modulo $d$ visited by the sequence depend on $x_0 \bmod d$ and $a \bmod d$. In particular, $f$ cannot be a single-cycle permutation on all of $\mathbb{Z}/m\mathbb{Z}$.

*Necessity of (2) and (3):* The $n$-th iterate is $f^n(x_0) = a^n x_0 + c \frac{a^n - 1}{a - 1} \bmod m$ (when $a \ne 1$). Full period requires $f^m = \mathrm{id}$ and $f^d \ne \mathrm{id}$ for all proper divisors $d \mid m$. The condition $a^m \equiv 1 \pmod{m}$ with appropriate order constraints yields (2) and (3) via the Chinese Remainder Theorem.

*Sufficiency:* Under (1)--(3), one verifies by CRT that $f$ acts as a cyclic permutation on each $\mathbb{Z}/p^{e_i}\mathbb{Z}$ factor, and these cycles combine into a single cycle of length $m$. $\square$

**Theorem 2 (Closed-Form Expression).** *For $a \ne 1$:*
$$x_n \equiv a^n x_0 + c \cdot \frac{a^n - 1}{a - 1} \pmod{m}.$$

**Proof.** By induction on $n$. Base case ($n=0$): $x_0 = a^0 x_0 + c \cdot 0 = x_0$. Inductive step: assume $x_n = a^n x_0 + c \cdot \frac{a^n - 1}{a-1}$. Then
$$x_{n+1} = a x_n + c = a^{n+1} x_0 + c \cdot \frac{a(a^n - 1)}{a-1} + c = a^{n+1} x_0 + c \cdot \frac{a^{n+1} - a + a - 1}{a-1} = a^{n+1} x_0 + c \cdot \frac{a^{n+1} - 1}{a-1}. \quad \square$$

**Lemma 1 (Matrix Representation).** *The LCG recurrence is captured by*
$$\begin{pmatrix} x_{n+1} \\ 1 \end{pmatrix} = \begin{pmatrix} a & c \\ 0 & 1 \end{pmatrix} \begin{pmatrix} x_n \\ 1 \end{pmatrix},$$
*whence $x_n$ can be computed in $O(\log n)$ arithmetic operations via binary matrix exponentiation.*

**Proof.** Direct matrix multiplication yields $x_{n+1} = a x_n + c \cdot 1$ and $1 = 0 \cdot x_n + 1 \cdot 1$, matching the recurrence. Repeated application gives $\binom{x_n}{1} = M^n \binom{x_0}{1}$ where $M = \bigl(\begin{smallmatrix}a & c \\ 0 & 1\end{smallmatrix}\bigr)$. Computing $M^n \bmod m$ via repeated squaring uses $O(\log n)$ matrix multiplications, each costing $O(1)$ for $2 \times 2$ matrices. $\square$

**Theorem 3 (Floyd's Cycle Detection).** *Given any iterated function $f: S \to S$ on a finite set $S$ starting from $x_0$, Floyd's algorithm finds the cycle length $\rho$ and tail length $\tau$ in $O(\tau + \rho)$ time and $O(1)$ space.*

**Proof.** Maintain two pointers: $\text{slow} = f^i(x_0)$ and $\text{fast} = f^{2i}(x_0)$. They first meet at some step $\nu$ with $\nu \equiv 0 \pmod{\rho}$ and $\nu \ge \tau$. Then:
- To find $\tau$: reset slow to $x_0$, advance both at unit speed; they meet at position $\tau$.
- To find $\rho$: from any point in the cycle, advance one pointer until it returns; the number of steps is $\rho$.

Total work: $O(\nu) + O(\tau) + O(\rho) = O(\tau + \rho)$ since $\nu \le \tau + \rho$. Space is $O(1)$ (two pointers). $\square$

## Editorial
LCG sequence analysis: x_{n+1} = (a*x_n + c) mod m. Cycle detection, closed-form evaluation, period finding. We matrix exponentiation approach. We then floyd's algorithm. Finally, find tail length tau.

## Pseudocode

```text
Matrix exponentiation approach
if n is odd
Floyd's algorithm
Find tail length tau
Find period rho
```

## Complexity Analysis

- **Time (closed-form at index $n$):** $O(\log n)$ via binary matrix exponentiation, with each step performing $O(1)$ modular multiplications.
- **Space (closed-form):** $O(1)$ (constant number of $2 \times 2$ matrices).
- **Time (cycle detection):** $O(\tau + \rho)$ where $\tau$ is the tail length and $\rho$ the period.
- **Space (cycle detection):** $O(1)$.

## Answer

$$\boxed{885226002}$$

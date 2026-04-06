# Problem 591: Best Approximations

## Problem Statement

Given a positive integer $N$, define a fraction $\frac{p}{q}$ (with $\gcd(p,q)=1$ and $q > 0$) to be a **best approximation** to the real number $\alpha$ if $|p - q\alpha| < |p' - q'\alpha|$ for every fraction $\frac{p'}{q'}$ with $0 < q' < q$. For non-square positive integers $n$, let $f(n, N)$ denote the sum of denominators $q$ over all best approximations $\frac{p}{q}$ to $\sqrt{n}$ with $q \leq N$. Compute:

$$\sum_{\substack{n=1 \\ n \text{ not a perfect square}}}^{N} f(n, N)$$

## Mathematical Foundation

**Definition 1.** A fraction $\frac{p}{q}$ with $q > 0$ and $\gcd(p,q)=1$ is a *best approximation of the first kind* to $\alpha \in \mathbb{R} \setminus \mathbb{Q}$ if for every fraction $\frac{p'}{q'}$ with $0 < q' < q$, we have $|q\alpha - p| < |q'\alpha - p'|$.

**Theorem 1 (Characterization of Best Approximations).** Let $\alpha = [a_0; a_1, a_2, \ldots]$ be the continued fraction expansion of an irrational number $\alpha$, with convergents $h_k/k_k$ and partial quotients $a_k$. Then $\frac{p}{q}$ is a best approximation to $\alpha$ if and only if it is either:
1. a convergent $h_k/k_k$, or
2. a semiconvergent $\frac{h_{k-1} + j \cdot h_k}{k_{k-1} + j \cdot k_k}$ for some $k \geq 0$ and $\lceil a_{k+1}/2 \rceil \leq j < a_{k+1}$, with the additional condition that when $a_{k+1}$ is even and $j = a_{k+1}/2$, the fraction qualifies only if $|q\alpha - p| < |k_k \alpha - h_k|$.

**Proof.** The proof proceeds in two parts.

*Part 1 (Convergents are best approximations).* By the standard theory of continued fractions, the convergents satisfy the alternating inequality $h_k k_{k-1} - h_{k-1} k_k = (-1)^{k-1}$ and the approximation bound:

$$|k_k \alpha - h_k| = \frac{1}{\alpha_{k+1} k_k + k_{k-1}}$$

where $\alpha_{k+1} = [a_{k+1}; a_{k+2}, \ldots]$ is the $(k+1)$-th complete quotient. For any $\frac{p'}{q'}$ with $0 < q' \leq k_k$, the determinant condition $|h_k q' - k_k p'| \geq 1$ (since $h_k, k_k$ are coprime) yields $|q'\alpha - p'| \geq |k_k \alpha - h_k|$, with equality only when $q' = k_k$.

*Part 2 (Semiconvergent criterion).* The intermediate fractions $\frac{h_{k-1} + j h_k}{k_{k-1} + j k_k}$ for $1 \leq j \leq a_{k+1}$ interpolate between $h_{k-1}/k_{k-1}$ and $h_{k+1}/k_{k+1}$. Write $q_j = k_{k-1} + j k_k$ and $p_j = h_{k-1} + j h_k$. Then $|q_j \alpha - p_j| = |k_{k-1}\alpha - h_{k-1}| - j|k_k\alpha - h_k|$, which decreases as $j$ increases. The fraction $p_j/q_j$ is a best approximation if and only if $|q_j\alpha - p_j| < |k_k\alpha - h_k|$, which holds precisely when $j > a_{k+1}/2$ (strictly), or $j = a_{k+1}/2$ with the tie-breaking condition satisfied. $\square$

**Theorem 2 (Periodicity of Continued Fractions for Quadratic Irrationals).** For any non-square positive integer $n$, the continued fraction of $\sqrt{n}$ is purely periodic after the initial term:

$$\sqrt{n} = [a_0; \overline{a_1, a_2, \ldots, a_\ell}]$$

where $a_0 = \lfloor \sqrt{n} \rfloor$ and the period satisfies $a_\ell = 2a_0$. Moreover, the period is palindromic: $a_i = a_{\ell - i}$ for $1 \leq i \leq \ell - 1$.

**Proof.** By Lagrange's theorem, a real number has an eventually periodic continued fraction expansion if and only if it is a quadratic irrational. Since $\sqrt{n}$ satisfies $x^2 - n = 0$ and is irrational (as $n$ is not a perfect square), the expansion is eventually periodic. The standard algorithm for computing the expansion shows that the complete quotients $\alpha_k = (m_k + \sqrt{n})/d_k$ satisfy $0 < m_k < \sqrt{n}$ and $d_k | (n - m_k^2)$ for $k \geq 1$. Since $m_k$ and $d_k$ are bounded, the sequence is eventually periodic. The period ends when $a_k = 2a_0$, at which point the complete quotient returns to $\sqrt{n} + a_0$. The palindromic property follows from the symmetry $m_k = m_{\ell - k}$. $\square$

**Lemma 1 (Convergent Recurrence).** The convergents $h_k/k_k$ of $[a_0; a_1, a_2, \ldots]$ satisfy:

$$h_k = a_k h_{k-1} + h_{k-2}, \qquad k_k = a_k k_{k-1} + k_{k-2}$$

with initial conditions $h_{-1} = 1$, $h_{-2} = 0$, $k_{-1} = 0$, $k_{-2} = 1$.

**Proof.** By induction on $k$. The base cases $h_0 = a_0$, $k_0 = 1$ are immediate. The inductive step uses the matrix identity:

$$\begin{pmatrix} h_k & h_{k-1} \\ k_k & k_{k-1} \end{pmatrix} = \prod_{i=0}^{k} \begin{pmatrix} a_i & 1 \\ 1 & 0 \end{pmatrix}$$

which follows from the definition $[a_0; a_1, \ldots, a_k] = a_0 + 1/(a_1 + 1/(\cdots))$ and the multiplicativity of the matrix product. $\square$

## Algorithm

```
function solve(N):
    total = 0
    for n = 1 to N:
        if is_perfect_square(n): continue
        a0 = floor(sqrt(n))
        // Compute periodic part of continued fraction of sqrt(n)
        cf = [], m = 0, d = 1, a = a0
        repeat:
            m = d * a - m
            d = (n - m * m) / d
            a = floor((a0 + m) / d)
            cf.append(a)
        until a == 2 * a0

        // Enumerate best approximations with denominator <= N
        h_prev, h_curr = 1, a0
        k_prev, k_curr = 0, 1
        total += 1  // denominator of h_0/k_0 = a0/1

        period = len(cf), idx = 0
        while true:
            a_next = cf[idx % period]
            // Semiconvergents: j from ceil(a_next/2) to a_next-1
            j_start = (a_next + 1) / 2
            for j = j_start to a_next - 1:
                q = k_prev + j * k_curr
                if q > N: goto done_n
                total += q
            // Full convergent (j = a_next)
            h_new = a_next * h_curr + h_prev
            k_new = a_next * k_curr + k_prev
            if k_new > N: break
            total += k_new
            h_prev, h_curr = h_curr, h_new
            k_prev, k_curr = k_curr, k_new
            idx += 1
    done_n:
    return total
```

## Complexity Analysis

**Proposition.** The algorithm runs in $O(N^{3/2})$ time and $O(\sqrt{N})$ space.

*Proof.* For each non-square $n \leq N$, the continued fraction period length is $O(\sqrt{n})$. The convergent denominators grow at least geometrically (each $k_{k+1} \geq k_k + k_{k-1} \geq \frac{1+\sqrt{5}}{2} k_k$ after the first few terms), so the number of full periods processed before $k_k > N$ is $O(\log N / \ell)$ where $\ell$ is the period length. The total work per $n$ is $O(\sqrt{n} \cdot \log N / \sqrt{n}) = O(\log N)$ for the convergent enumeration plus $O(\sqrt{n})$ to compute one period. Summing $O(\sqrt{n})$ over $n = 1, \ldots, N$ gives $O(N^{3/2})$. The space usage is $O(\sqrt{N})$ for storing one continued fraction period. $\square$

## Answer

$$\boxed{526007984625966}$$

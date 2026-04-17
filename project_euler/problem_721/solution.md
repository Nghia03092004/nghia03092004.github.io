# Problem 721: High Powers of Irrational Numbers

## Problem Statement

Given a positive integer $a$ that is not a perfect square, define

$$f(a, n) = \left\lfloor \left(\lceil \sqrt{a} \rceil + \sqrt{a}\right)^n \right\rfloor$$

where $\lfloor \cdot \rfloor$ and $\lceil \cdot \rceil$ denote the floor and ceiling functions. Compute

$$G(N) = \sum_{\substack{a=1 \\ a \text{ not a perfect square}}}^{N} f(a, a^2) \pmod{999\,999\,937}.$$

We are given $f(5,2)=27$, $f(5,5)=3935$, $G(1000) \equiv 163\,861\,845 \pmod{999\,999\,937}$.

Find $G(5\,000\,000) \pmod{999\,999\,937}$.

## Mathematical Foundation

**Theorem 1 (Companion Integer Sequence).** Let $a$ be a positive integer that is not a perfect square and set $m = \lceil\sqrt{a}\rceil$. Define $\alpha = m + \sqrt{a}$ and $\beta = m - \sqrt{a}$. Then the sequence $T_n = \alpha^n + \beta^n$ satisfies the linear recurrence

$$T_n = 2m\,T_{n-1} - (m^2 - a)\,T_{n-2}, \quad T_0 = 2,\; T_1 = 2m,$$

and $T_n \in \mathbb{Z}$ for all $n \ge 0$.

**Proof.** The values $\alpha$ and $\beta$ are the two roots of the quadratic

$$x^2 - (\alpha + \beta)x + \alpha\beta = x^2 - 2mx + (m^2 - a) = 0.$$

By Newton's identity for power sums of roots, $T_n = (\alpha+\beta)T_{n-1} - \alpha\beta\,T_{n-2} = 2m\,T_{n-1} - (m^2-a)\,T_{n-2}$. The initial conditions are $T_0 = \alpha^0 + \beta^0 = 2$ and $T_1 = \alpha + \beta = 2m$. Since $2m$ and $m^2 - a$ are integers and $T_0, T_1 \in \mathbb{Z}$, induction on $n$ gives $T_n \in \mathbb{Z}$ for all $n \ge 0$. $\square$

**Lemma 1 (Floor Extraction).** For non-perfect-square $a$ and $n \ge 1$, we have $f(a,n) = T_n - 1$.

**Proof.** Since $a$ is not a perfect square, $m = \lceil\sqrt{a}\rceil$ satisfies $m > \sqrt{a}$, so $\beta = m - \sqrt{a} > 0$. Moreover, $m \le \sqrt{a} + 1$ (as $m$ is the least integer $\ge \sqrt{a}$), which gives $\beta = m - \sqrt{a} \le 1$. The inequality is strict because $a$ is not a perfect square, so $\sqrt{a} \notin \mathbb{Z}$ and thus $m - \sqrt{a} < 1$. Hence $0 < \beta < 1$, and therefore $0 < \beta^n < 1$ for all $n \ge 1$.

Now $\alpha^n = T_n - \beta^n$ where $T_n$ is a positive integer and $0 < \beta^n < 1$. Therefore $T_n - 1 < \alpha^n < T_n$, which gives $\lfloor \alpha^n \rfloor = T_n - 1$. $\square$

**Lemma 2 (Matrix Recurrence).** The recurrence $T_n = 2m\,T_{n-1} - c\,T_{n-2}$ (where $c = m^2 - a$) admits the matrix form

$$\begin{pmatrix} T_n \\ T_{n-1} \end{pmatrix} = \begin{pmatrix} 2m & -c \\ 1 & 0 \end{pmatrix}^{n-1} \begin{pmatrix} 2m \\ 2 \end{pmatrix}.$$

**Proof.** Direct verification: the matrix equation encodes $T_n = 2m\,T_{n-1} - c\,T_{n-2}$ in the first row, and $T_{n-1} = T_{n-1}$ in the second row. Induction on $n$ yields the stated power formula. $\square$

## Algorithm

```
function G(N, p):
    precompute perfect_squares = {1, 4, 9, 16, ...} up to N
    result = 0
    for a = 1 to N:
        if a in perfect_squares: continue
        m = ceil(sqrt(a))
        c = m*m - a
        // Compute T_{a^2} mod p via matrix exponentiation
        M = [[2*m mod p, (-c) mod p], [1, 0]]
        v = [2*m mod p, 2]
        v = matrix_power(M, a*a - 1, p) * v
        T = v[0]
        result = (result + T - 1) mod p
    return result
```

```
function matrix_power(M, exp, p):
    R = identity_2x2
    while exp > 0:
        if exp is odd:
            R = R * M mod p
        M = M * M mod p
        exp = exp >> 1
    return R
```

## Complexity Analysis

- **Time:** For each non-perfect-square $a \le N$, matrix exponentiation computes $M^{a^2-1}$ using $O(\log(a^2)) = O(\log a)$ multiplications of $2 \times 2$ matrices over $\mathbb{Z}/p\mathbb{Z}$. Each multiplication is $O(1)$. There are $N - O(\sqrt{N})$ non-perfect-square values. Total: $O(N \log N)$.
- **Space:** $O(1)$ auxiliary space per value of $a$ (two $2\times 2$ matrices). Overall $O(\sqrt{N})$ for the perfect-square set, or $O(1)$ if squares are detected on the fly.

## Answer

$$\boxed{700792959}$$

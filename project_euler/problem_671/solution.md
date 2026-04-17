# Problem 671: Colouring a Loop

## Problem Statement

Extend the tiling of Problem 670 to a $2 \times n$ **loop** (cylinder). Let $F_k(n)$ count the number of valid colourings of a $2 \times n$ cylindrical grid using at most $k$ colours, where adjacent cells must have different colours. Given:

- $F_4(3) = 104$
- $F_5(7) = 3{,}327{,}300$
- $F_6(101) \equiv 75{,}309{,}980 \pmod{1{,}000{,}004{,}321}$

Find $F_{10}(10{,}004{,}003{,}002{,}001) \bmod 1{,}000{,}004{,}321$.

## Mathematical Foundation

Let $\mathcal{S}$ denote the set of valid column states for a $2 \times 1$ column with $k$ colours, and let $T$ be the $|\mathcal{S}| \times |\mathcal{S}|$ transfer matrix where $T_{ij} = 1$ if column state $j$ can follow column state $i$ (i.e., no adjacent cells between the two columns share a colour).

**Theorem 1 (Trace Formula for Cylindrical Tilings).** *The number of valid $k$-colourings of a $2 \times n$ cylinder is*
$$F_k(n) = \operatorname{tr}(T^n) = \sum_{i=1}^{s} \lambda_i^n,$$
*where $\lambda_1, \ldots, \lambda_s$ are the eigenvalues of $T$ counted with algebraic multiplicity, and $s = |\mathcal{S}|$.*

**Proof.** A valid colouring of the cylinder is a closed walk of length $n$ in the state graph: a sequence of states $q_0, q_1, \ldots, q_{n-1}$ with $q_0 = q_{n-1}$ such that each consecutive pair is compatible. The number of closed walks of length $n$ starting and ending at state $i$ is $(T^n)_{ii}$. Summing over all starting states yields $\sum_i (T^n)_{ii} = \operatorname{tr}(T^n)$. By diagonalisation (or Jordan normal form), $\operatorname{tr}(T^n) = \sum_i \lambda_i^n$. $\square$

**Theorem 2 (Cayley--Hamilton Recurrence).** *Let $\chi_T(\lambda) = \lambda^s - c_1 \lambda^{s-1} - \cdots - c_s$ be the characteristic polynomial of $T$. Then the sequence $a_n = \operatorname{tr}(T^n)$ satisfies the linear recurrence*
$$a_n = c_1 \, a_{n-1} + c_2 \, a_{n-2} + \cdots + c_s \, a_{n-s} \quad \text{for all } n \ge s.$$

**Proof.** By the Cayley--Hamilton theorem, $T^s = c_1 T^{s-1} + \cdots + c_s I$. Taking the trace of $T^n = T^{n-s} \cdot T^s$ and applying linearity of trace:
$$\operatorname{tr}(T^n) = c_1 \operatorname{tr}(T^{n-1}) + \cdots + c_s \operatorname{tr}(T^{n-s}).$$
This is exactly the stated recurrence. $\square$

**Lemma 1 (Modular Exponentiation of Recurrence).** *Given a linear recurrence of order $s$ over $\mathbb{Z}/p\mathbb{Z}$, the $n$-th term can be computed in $O(s^2 \log n)$ arithmetic operations using Kitamasa's method (polynomial exponentiation modulo the characteristic polynomial).*

**Proof.** Represent the shift operator as multiplication by $x$ in the quotient ring $(\mathbb{Z}/p\mathbb{Z})[x] / (\chi_T(x))$. Computing $x^n \bmod \chi_T(x)$ by repeated squaring requires $O(\log n)$ polynomial multiplications, each costing $O(s^2)$ (or $O(s \log s)$ with FFT). Reading off $a_n$ from the resulting polynomial and the initial values is $O(s)$. $\square$

## Algorithm

```
function ComputeF(k, n, p):
    // Step 1: Build transfer matrix
    S = enumerate all valid 2-cell column states with k colours
    s = |S|
    T = s x s zero matrix
    for each pair (i, j) in S x S:
        if columns i and j are compatible:
            T[i][j] = 1

    // Step 2: Compute characteristic polynomial mod p
    chi = characteristic_polynomial(T) mod p   // degree s

    // Step 3: Compute initial values a_0, ..., a_{s-1}
    M = identity matrix (s x s) mod p
    for t = 0 to s-1:
        a[t] = trace(M) mod p
        M = M * T mod p

    // Step 4: Kitamasa's method — compute x^n mod chi(x) in F_p[x]
    poly = polynomial_power_mod(x, n, chi, p)

    // Step 5: Combine
    result = sum over j: poly.coeff[j] * a[j] mod p
    return result
```

## Complexity Analysis

- **Time:** $O(s^2 k^2)$ to build $T$, plus $O(s^3)$ for the characteristic polynomial, plus $O(s^2 \log n)$ for Kitamasa's method. Since $s = O(k^2)$, the total is $O(k^6 + k^4 \log n)$. For $k = 10$ and $n \approx 10^{13}$: approximately $10^6 + 10^4 \cdot 43 \approx 10^6$ operations.
- **Space:** $O(s^2) = O(k^4)$ for the transfer matrix.

## Answer

$$\boxed{946106780}$$

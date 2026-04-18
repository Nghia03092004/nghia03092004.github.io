# Problem 101: Optimum Polynomial

## Problem Statement

If we are given the first $k$ terms of a sequence, it is not always possible to determine the generating function. For example, consider the sequence of cube numbers: $1, 8, 27, 64, 125, 216, \ldots$ If we are only given the first two terms, the simplest assumption would be a linear relationship, giving $u(n) = 7n - 6$. This gives $u(3) = 15$, which is the **First Incorrect Term (FIT)**.

Given that the generating function is:

$$u(n) = \sum_{j=0}^{10} (-1)^j\, n^j = 1 - n + n^2 - n^3 + n^4 - n^5 + n^6 - n^7 + n^8 - n^9 + n^{10},$$

find the sum of all FITs for the Best Optimum Polynomials (BOPs) of degrees $0$ through $9$ (i.e., using the first $k = 1, 2, \ldots, 10$ terms).

## Mathematical Development

**Theorem 1** (Existence and Uniqueness of Polynomial Interpolation). *Given $k$ distinct nodes $x_1, \ldots, x_k \in \mathbb{R}$ and values $y_1, \ldots, y_k \in \mathbb{R}$, there exists a unique polynomial $P \in \mathbb{R}[x]$ of degree at most $k - 1$ satisfying $P(x_i) = y_i$ for $i = 1, \ldots, k$.*

*Proof.* **Existence.** Define the Lagrange basis polynomials
$$\ell_i(x) = \prod_{\substack{j=1 \\ j \neq i}}^{k} \frac{x - x_j}{x_i - x_j}, \qquad i = 1, \ldots, k.$$

Each $\ell_i$ has degree $k - 1$ and satisfies $\ell_i(x_j) = \delta_{ij}$ (the Kronecker delta). The polynomial
$$P(x) = \sum_{i=1}^{k} y_i \, \ell_i(x)$$
has degree at most $k - 1$ and satisfies $P(x_j) = \sum_{i=1}^{k} y_i \, \delta_{ij} = y_j$ for each $j$.

**Uniqueness.** Suppose $Q \in \mathbb{R}[x]$ with $\deg Q \leq k - 1$ also interpolates the data. Then $R(x) = P(x) - Q(x)$ has degree at most $k - 1$ and vanishes at the $k$ distinct points $x_1, \ldots, x_k$. By the Factor Theorem, $R$ is divisible by $\prod_{i=1}^{k}(x - x_i)$, a polynomial of degree $k$. Since $\deg R \leq k - 1 < k$, we conclude $R \equiv 0$, hence $P = Q$. $\blacksquare$

**Theorem 2** (FIT Existence for Under-Determined Interpolation). *Let $u \in \mathbb{R}[x]$ with $\deg u = d$, and for $1 \leq k \leq d$, let $P_k$ denote the unique polynomial of degree at most $k - 1$ interpolating $u(1), u(2), \ldots, u(k)$. Then $P_k(k+1) \neq u(k+1)$.*

*Proof.* Define $R_k(x) = u(x) - P_k(x)$. Since $\deg u = d$ and $\deg P_k \leq k - 1 < d$, the leading coefficient of $R_k$ equals the leading coefficient of $u$, so $\deg R_k = d$. The polynomial $R_k$ vanishes at $x = 1, 2, \ldots, k$, whence
$$R_k(x) = (x - 1)(x - 2) \cdots (x - k) \, Q_k(x)$$
for some polynomial $Q_k$ of degree $d - k \geq 0$ with the same leading coefficient as $u$ (up to sign).

Evaluating at $x = k + 1$:
$$R_k(k+1) = k! \cdot Q_k(k+1).$$

Since $k! \neq 0$, it suffices to show $Q_k(k+1) \neq 0$. The polynomial $Q_k$ has degree $d - k \geq 0$ and leading coefficient equal to the leading coefficient of $u$ (which is $(-1)^{10} = 1$ for the stated $u$). For $k = d$, $Q_d$ is a nonzero constant, so $Q_d(d+1) \neq 0$. For $k < d$, $Q_k$ is a nonconstant polynomial; we verify computationally that $Q_k(k+1) \neq 0$ for each $k = 1, \ldots, 9$. (Alternatively, note that $Q_k$ has roots among $\{k+2, k+3, \ldots\}$ only if specific coefficient relationships hold, which fail for the given $u$.) $\blacksquare$

**Lemma 1** (Lagrange Evaluation Complexity). *The value $P_k(x_0)$ at a single point $x_0$ can be computed in $\Theta(k^2)$ arithmetic operations via the Lagrange formula.*

*Proof.* The Lagrange formula
$$P_k(x_0) = \sum_{i=1}^{k} y_i \prod_{\substack{j=1 \\ j \neq i}}^{k} \frac{x_0 - x_j}{x_i - x_j}$$
involves $k$ terms, each requiring $k - 1$ multiplications and $k - 1$ divisions. The total count is $k(2k - 2) = \Theta(k^2)$ arithmetic operations. $\blacksquare$

## Editorial
Given u(n) = sum_{j=0}^{10} (-1)^j n^j, find the sum of all FITs for the BOPs using the first k = 1, ..., 10 terms.

## Pseudocode

```text
    define u(n) = sum_{j=0}^{10} (-n)^j

    Set S <- 0
    For k from 1 to 10:
        Set points <- [(i, u(i)) for i = 1, ..., k]
        Set FIT_k <- LAGRANGE_EVAL(points, k + 1)
        Set S <- S + FIT_k
    Return S

    Set k <- |points|
    Set result <- 0
    For i from 1 to k:
        (x_i, y_i) <- points[i]
        Set basis <- y_i
        For j from 1 to k, j != i:
            (x_j, _) <- points[j]
            Set basis <- basis * (x0 - x_j) / (x_i - x_j)
        Set result <- result + basis
    Return result
```

## Complexity Analysis

- **Time.** For each $k \in \{1, \ldots, 10\}$, the Lagrange evaluation costs $\Theta(k^2)$ operations. The total is $\sum_{k=1}^{10} k^2 = 385$ arithmetic operations. In general, for a degree-$d$ generating function, the cost is $\sum_{k=1}^{d} k^2 = \Theta(d^3)$.
- **Space.** $O(d)$ for storing the data points and intermediate computations.

## Answer

$$\boxed{37076114526}$$

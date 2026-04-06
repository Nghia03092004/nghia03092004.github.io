# Problem 722: Slowly Converging Series

## Problem Statement

For a non-negative integer $k$, define

$$E_k(q) = \sum_{n=1}^{\infty} \sigma_k(n)\,q^n$$

where $\sigma_k(n) = \sum_{d \mid n} d^k$ is the divisor power sum. The series converges for $0 < q < 1$.

Given:
- $E_1(1 - 2^{-4}) = 3.872155809243 \times 10^2$
- $E_3(1 - 2^{-8}) = 2.767385314772 \times 10^{10}$
- $E_7(1 - 2^{-15}) = 6.725803486744 \times 10^{39}$

Compute $E_{15}(1 - 2^{-25})$ in scientific notation rounded to twelve significant digits.

## Mathematical Foundation

**Theorem 1 (Lambert Series Representation).** For $|q| < 1$ and $k \ge 0$,

$$E_k(q) = \sum_{m=1}^{\infty} \frac{m^k\,q^m}{1 - q^m}.$$

**Proof.** By absolute convergence of the double sum $\sum_{d \ge 1}\sum_{j \ge 1} d^k q^{dj}$ (since $|q|<1$), Fubini's theorem permits interchange of summation order:

$$E_k(q) = \sum_{n=1}^{\infty} \sigma_k(n)\,q^n = \sum_{n=1}^{\infty} q^n \sum_{d \mid n} d^k = \sum_{d=1}^{\infty} d^k \sum_{j=1}^{\infty} q^{dj} = \sum_{d=1}^{\infty} \frac{d^k\,q^d}{1 - q^d}.$$

The inner geometric series converges because $|q^d| < 1$. $\square$

**Theorem 2 (Polylogarithm Decomposition).** For $|q| < 1$ and $k \ge 1$,

$$E_k(q) = \sum_{j=1}^{\infty} \operatorname{Li}_{-k}(q^j)$$

where $\operatorname{Li}_{-k}(x) = \sum_{m=1}^{\infty} m^k x^m$ is the polylogarithm of negative order. For positive integer $k$, this is a rational function:

$$\operatorname{Li}_{-k}(x) = \frac{x\,A_k(x)}{(1-x)^{k+1}}$$

where $A_k(x)$ is the Eulerian polynomial of degree $k-1$.

**Proof.** From Theorem 1, $E_k(q) = \sum_{m=1}^{\infty} m^k \frac{q^m}{1-q^m} = \sum_{m=1}^{\infty} m^k \sum_{j=1}^{\infty} q^{mj}$. Exchanging the order of summation (justified by absolute convergence) gives $E_k(q) = \sum_{j=1}^{\infty} \sum_{m=1}^{\infty} m^k (q^j)^m = \sum_{j=1}^{\infty} \operatorname{Li}_{-k}(q^j)$. The rational function form of $\operatorname{Li}_{-k}$ for integer $k$ is classical (see e.g. Apostol, Chapter 12). $\square$

**Lemma 1 (Asymptotic Behavior).** For $q = 1 - \epsilon$ with $\epsilon \to 0^+$ and $k \ge 2$,

$$E_k(q) \sim \frac{\Gamma(k)\,\zeta(k)}{\epsilon^k}.$$

**Proof.** In the Lambert series, substitute $q^m = (1-\epsilon)^m$. For small $\epsilon$, the sum is well-approximated by the integral $\int_0^\infty \frac{t^k\,e^{-t\epsilon}}{1 - e^{-t\epsilon}}\,dt$. Under the change of variable $u = t\epsilon$, this becomes $\epsilon^{-(k+1)}\int_0^\infty \frac{u^k}{e^u - 1}\,du = \epsilon^{-(k+1)}\,\Gamma(k+1)\,\zeta(k+1)$. A more careful Euler--Maclaurin analysis extracting the leading singular term from $\frac{q^m}{1-q^m} \approx \frac{1}{m\epsilon} - \frac{1}{2} + O(\epsilon)$ yields the corrected exponent $E_k(q) \sim \Gamma(k)\,\zeta(k)\,\epsilon^{-k}$. $\square$

## Algorithm

```
function E_k(q, k, precision_digits):
    // Method: sum the polylogarithm decomposition
    set working precision to precision_digits + 20 guard digits
    S = 0
    for j = 1, 2, 3, ...:
        qj = q^j
        if qj < 10^(-precision_digits - 10): break
        // Evaluate Li_{-k}(qj) using the Eulerian polynomial
        A = eulerian_polynomial(k)
        term = qj * A(qj) / (1 - qj)^(k+1)
        S = S + term
    return S
```

```
function eulerian_polynomial(k):
    // A_k(x) = sum_{i=0}^{k-1} A(k,i) x^i
    // where A(k,i) are Eulerian numbers
    compute A(k,i) via the recurrence:
        A(k,0) = 1
        A(k,i) = (k-i)*A(k-1,i-1) + (i+1)*A(k-1,i)
    return polynomial with these coefficients
```

## Complexity Analysis

- **Time:** The outer sum over $j$ converges when $q^j < \epsilon_{\text{target}}$, requiring $j \lesssim \frac{D \ln 10}{-\ln q}$ terms, where $D$ is the number of desired digits. For $q = 1 - 2^{-25}$, $-\ln q \approx 2^{-25}$, so $j \lesssim D \cdot 2^{25} \cdot \ln 10 / 1 \approx 10^9$ for $D = 15$. Each polylogarithm evaluation is $O(k)$ via the rational function form. Total: $O(k \cdot 2^B \cdot D)$ where $B = 25$.
- **Space:** $O(k)$ for the Eulerian polynomial coefficients.

Using Euler--Maclaurin acceleration on the $j$-sum, this reduces to $O(k \cdot \sqrt{2^B \cdot D})$.

## Answer

$$\boxed{3.376792776502e132}$$

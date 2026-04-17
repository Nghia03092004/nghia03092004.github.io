# Problem 65: Convergents of e

## Problem Statement

Find the sum of digits in the numerator of the 100th convergent of the continued fraction for $e$.

## Mathematical Foundation

**Theorem 1 (Euler, 1737).** The continued fraction expansion of $e$ is

$$e = [2; 1, 2, 1, 1, 4, 1, 1, 6, 1, 1, 8, \ldots]$$

where the partial quotients are $a_0 = 2$ and for $k \ge 1$:

$$a_k = \begin{cases} \frac{2(k+1)}{3} & \text{if } k \equiv 2 \pmod{3} \\ 1 & \text{otherwise.} \end{cases}$$

**Proof.** Euler derived this from the generalized continued fraction

$$e = 2 + \cfrac{1}{1 + \cfrac{1}{2 + \cfrac{1}{1 + \cfrac{1}{1 + \cfrac{1}{4 + \cdots}}}}}$$

using the relationship between $e$ and the confluent hypergeometric function. Specifically, consider the series $e = \sum_{n=0}^{\infty} 1/n!$ and apply Euler's method of converting power series to continued fractions. The proof relies on the identity

$$\frac{e + 1}{e - 1} = [2; 6, 10, 14, \ldots]$$

from which the stated continued fraction for $e$ can be derived via equivalence transformations. A complete proof appears in Perron's \emph{Die Lehre von den Kettenbr\"uchen} (1929). $\square$

**Theorem 2 (Convergent Recurrence).** The convergents $h_n/k_n = [a_0; a_1, \ldots, a_n]$ satisfy the recurrence

$$h_n = a_n h_{n-1} + h_{n-2}, \quad k_n = a_n k_{n-1} + k_{n-2}$$

with initial conditions $h_{-1} = 1$, $h_0 = a_0$, $k_{-1} = 0$, $k_0 = 1$.

**Proof.** Define the matrix

$$M_i = \begin{pmatrix} a_i & 1 \\ 1 & 0 \end{pmatrix}.$$

We claim that

$$\begin{pmatrix} h_n & h_{n-1} \\ k_n & k_{n-1} \end{pmatrix} = M_0 M_1 \cdots M_n.$$

*Base case ($n = 0$):* $M_0 = \begin{pmatrix} a_0 & 1 \\ 1 & 0 \end{pmatrix}$, giving $h_0 = a_0$, $h_{-1} = 1$, $k_0 = 1$, $k_{-1} = 0$. Since $[a_0] = a_0/1$, this is correct.

*Inductive step:* Assume the product $M_0 \cdots M_{n-1}$ gives the correct matrix. Then

$$M_0 \cdots M_n = \begin{pmatrix} h_{n-1} & h_{n-2} \\ k_{n-1} & k_{n-2} \end{pmatrix} \begin{pmatrix} a_n & 1 \\ 1 & 0 \end{pmatrix} = \begin{pmatrix} a_n h_{n-1} + h_{n-2} & h_{n-1} \\ a_n k_{n-1} + k_{n-2} & k_{n-1} \end{pmatrix}$$

which gives $h_n = a_n h_{n-1} + h_{n-2}$ and $k_n = a_n k_{n-1} + k_{n-2}$. To see that $h_n/k_n = [a_0; a_1, \ldots, a_n]$, note that replacing $a_n$ by $a_n + 1/\alpha$ in the matrix product and using the inductive hypothesis on $[a_0; a_1, \ldots, a_{n-1}, a_n + 1/\alpha] = (h_{n-1}(a_n + 1/\alpha) + h_{n-2})/(k_{n-1}(a_n + 1/\alpha) + k_{n-2})$ confirms the identity. $\square$

**Lemma 1 (Coprimality).** For all $n \ge 0$, $\gcd(h_n, k_n) = 1$.

**Proof.** Taking determinants of the matrix product:

$$h_n k_{n-1} - h_{n-1} k_n = \det(M_0) \cdots \det(M_n) = (-1)^{n+1}.$$

Since $h_n k_{n-1} - h_{n-1} k_n = \pm 1$, we have $\gcd(h_n, k_n) = 1$. $\square$

**Lemma 2 (Growth Rate).** The numerator $h_{99}$ has approximately $\sum_{k=0}^{99} \log_{10}(a_k)$ digits, which for the $e$ continued fraction yields about 57--58 digits.

**Proof.** From the recurrence $h_n = a_n h_{n-1} + h_{n-2} \ge a_n h_{n-1}$, so $\log_{10} h_n \ge \log_{10} a_n + \log_{10} h_{n-1}$. By induction, $\log_{10} h_n \ge \sum_{k=0}^n \log_{10} a_k$. The partial quotients for $e$ at indices $k \equiv 2 \pmod{3}$ are $2, 4, 6, \ldots, 66$ (for $k$ up to 98), contributing $\sum_{j=1}^{33} \log_{10}(2j) \approx 42$. The remaining 67 terms contribute $\log_{10}(1) = 0$ each (except $a_0 = 2$). Total: $\approx 42 + \log_{10}(2) \approx 42.3$. The actual numerator has 58 digits due to the additive term $h_{n-2}$ accumulating additional magnitude. $\square$

## Algorithm

```
CONVERGENTS_OF_E(target=100):
    // Generate partial quotients a_0 through a_99
    a[0] = 2
    for k = 1 to 99:
        if k mod 3 == 2:
            a[k] = 2 * (k + 1) / 3
        else:
            a[k] = 1

    // Compute numerator using recurrence
    h_prev2 = 1     // h_{-1}
    h_prev1 = a[0]  // h_0
    for k = 1 to 99:
        h_curr = a[k] * h_prev1 + h_prev2
        h_prev2 = h_prev1
        h_prev1 = h_curr

    // Sum digits of h_99
    return digit_sum(h_prev1)
```

## Pseudocode

```text
Define the partial quotient a_k by
    a_0 = 2
    a_k = 2(k + 1) / 3 when k ≡ 2 (mod 3)
    a_k = 1 otherwise

h_prev2 = 1
h_prev1 = a_0

for k = 1, 2, ..., 99:
    h_curr = a_k * h_prev1 + h_prev2
    shift (h_prev2, h_prev1) forward to (h_prev1, h_curr)

return the sum of the decimal digits of h_prev1
```

## Complexity Analysis

**Time:** The algorithm computes 100 iterations of the recurrence. Each iteration involves one big-integer multiplication and one big-integer addition. Since $h_{99}$ has $d \approx 58$ digits, each arithmetic operation costs $O(d)$ with schoolbook multiplication (or $O(d \log d)$ with FFT-based multiplication, though unnecessary here). Total: $O(100 \cdot d) = O(100 \cdot 58) = O(5800)$.

**Space:** $O(d) = O(58)$ for storing three big integers ($h_{n-2}$, $h_{n-1}$, $h_n$).

## Answer

$$\boxed{272}$$

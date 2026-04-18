# Problem 16: Power Digit Sum

## Problem Statement

Let $S(m)$ denote the sum of the decimal digits of a positive integer $m$. Compute $S(2^{1000})$.

## Mathematical Development

**Definition 1.** For a positive integer $m$ with base-10 representation $m = \sum_{k=0}^{d-1} a_k \cdot 10^k$, where $0 \le a_k \le 9$ and $a_{d-1} \ne 0$, define the *digit sum* $S(m) = \sum_{k=0}^{d-1} a_k$.

**Definition 2.** The *digit length* of a positive integer $m$ is $d(m) = \lfloor \log_{10} m \rfloor + 1$.

**Theorem 1 (Digit length of $2^n$).** For all $n \ge 0$,
$$d(2^n) = \lfloor n \log_{10} 2 \rfloor + 1.$$

*Proof.* By Definition 2, $d(2^n) = \lfloor \log_{10}(2^n) \rfloor + 1 = \lfloor n \log_{10} 2 \rfloor + 1$, where the second equality follows from the logarithmic identity $\log_{10}(2^n) = n \log_{10} 2$. $\square$

**Corollary 1.** $d(2^{1000}) = \lfloor 1000 \cdot 0.30102999\ldots \rfloor + 1 = \lfloor 301.029\ldots \rfloor + 1 = 302$.

**Theorem 2 (Digit sum congruence).** For every positive integer $m$,
$$S(m) \equiv m \pmod{9}.$$

*Proof.* Write $m = \sum_{k=0}^{d-1} a_k \cdot 10^k$. Since $10 \equiv 1 \pmod{9}$, we have $10^k \equiv 1^k = 1 \pmod{9}$ for all $k \ge 0$. Therefore $m \equiv \sum_{k=0}^{d-1} a_k \cdot 1 = S(m) \pmod{9}$. $\square$

**Lemma 1 (Order of 2 modulo 9).** The multiplicative order of $2$ in $\mathbb{Z}/9\mathbb{Z}$ is $\mathrm{ord}_9(2) = 6$.

*Proof.* We compute successive powers: $2^1 \equiv 2$, $2^2 \equiv 4$, $2^3 \equiv 8$, $2^4 \equiv 7$, $2^5 \equiv 5$, $2^6 \equiv 1 \pmod{9}$. Since $2^k \not\equiv 1 \pmod{9}$ for $1 \le k \le 5$ and $2^6 \equiv 1$, the order is exactly $6$. $\square$

**Proposition 1 (Residue of $2^{1000}$ modulo 9).** $2^{1000} \equiv 7 \pmod{9}$.

*Proof.* By Lemma 1, $2^{1000} \equiv 2^{1000 \bmod 6} \pmod{9}$. Since $1000 = 166 \cdot 6 + 4$, we have $2^{1000} \equiv 2^4 = 16 \equiv 7 \pmod{9}$. $\square$

**Theorem 3 (Bounds on $S(2^n)$).** For all $n \ge 1$,
$$1 \le S(2^n) \le 9 \cdot d(2^n) = 9\bigl(\lfloor n \log_{10} 2 \rfloor + 1\bigr).$$

*Proof.* The lower bound holds because $2^n \ge 2$, so at least one digit is nonzero and $S(2^n) \ge 1$. The upper bound follows from $a_k \le 9$ for each of the $d(2^n)$ digits. $\square$

**Corollary 2.** $1 \le S(2^{1000}) \le 9 \cdot 302 = 2718$.

**Verification.** The computed answer $S(2^{1000}) = 1366$ satisfies $1366 = 151 \cdot 9 + 7$, hence $S(2^{1000}) \equiv 7 \pmod{9}$, consistent with Proposition 1 and Theorem 2.

## Editorial

We compute $b^n$ exactly and then sum its decimal digits. The reference pseudocode uses a digit array in base 10: each multiplication by $b$ propagates carries across the current digits, and the final digit array is summed. This is sufficient because repeated exact multiplication constructs the decimal expansion of $b^n$ without losing information.

## Pseudocode

```text
Algorithm: Digit Sum of an Exact Power
Require: Integers b ≥ 2 and n ≥ 1.
Ensure: The sum of the decimal digits of b^n.
1: Represent the current power by a decimal digit array A initialized to the value 1.
2: Repeat n times:
3:     Multiply A by b and normalize carries across its decimal digits.
4: After the final multiplication, compute S ← the sum of the digits stored in A.
5: Return S.
```

## Complexity Analysis

**Proposition 2 (Time complexity).** The algorithm runs in $\Theta(n \cdot d)$ time, where $d = d(b^n) = \Theta(n \log b)$.

*Proof.* The outer loop executes $n$ iterations. At iteration $i$, the digit array has length $d(b^i) = \lfloor i \log_{10} b \rfloor + 1$. The inner loop over digits at iteration $i$ performs $\Theta(d(b^i))$ constant-time operations. The total work is
$$\sum_{i=1}^{n} \Theta(i \log_{10} b) = \Theta\!\left(\log_{10} b \cdot \frac{n(n+1)}{2}\right) = \Theta(n^2 \log b).$$
For $b = 2$, this is $\Theta(n^2)$. $\square$

**Proposition 3 (Space complexity).** The algorithm uses $\Theta(d) = \Theta(n \log b)$ space.

*Proof.* The digit array stores at most $d(b^n) = \Theta(n \log b)$ digits. No other data structure grows with $n$. $\square$

## Answer

$$\boxed{1366}$$

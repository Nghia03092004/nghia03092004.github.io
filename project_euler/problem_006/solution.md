# Problem 6: Sum Square Difference

## Problem Statement

Compute the difference between the square of the sum and the sum of the squares of the first 100 natural numbers:
$$D(100) = \left(\sum_{i=1}^{100} i\right)^{\!2} - \sum_{i=1}^{100} i^2.$$

## Mathematical Development

### Notation

Throughout, we write
$$S_1(n) = \sum_{i=1}^{n} i, \qquad S_2(n) = \sum_{i=1}^{n} i^2, \qquad D(n) = S_1(n)^2 - S_2(n).$$

**Lemma 1** (Sum of the first $n$ natural numbers). *For all $n \ge 1$,*
$$S_1(n) = \frac{n(n+1)}{2}.$$

*Proof.* By induction on $n$.

*Base case:* $S_1(1) = 1 = 1 \cdot 2 / 2$.

*Inductive step:* Assume $S_1(k) = k(k+1)/2$. Then
$$S_1(k+1) = S_1(k) + (k+1) = \frac{k(k+1)}{2} + (k+1) = \frac{k(k+1) + 2(k+1)}{2} = \frac{(k+1)(k+2)}{2}. \quad\square$$

*Alternative proof (Gauss's pairing argument).* Write the sum in two orders:
$$S_1 = 1 + 2 + \cdots + n, \qquad S_1 = n + (n-1) + \cdots + 1.$$
Adding termwise: $2S_1 = \sum_{i=1}^{n}(n+1) = n(n+1)$, so $S_1 = n(n+1)/2$. $\square$

**Lemma 2** (Sum of squares). *For all $n \ge 1$,*
$$S_2(n) = \frac{n(n+1)(2n+1)}{6}.$$

*Proof.* Consider the telescoping identity
$$(k+1)^3 - k^3 = 3k^2 + 3k + 1.$$
Summing both sides for $k = 1, 2, \ldots, n$:
$$\sum_{k=1}^{n}\bigl[(k+1)^3 - k^3\bigr] = 3S_2(n) + 3S_1(n) + n.$$

The left-hand side telescopes:
$$\sum_{k=1}^{n}\bigl[(k+1)^3 - k^3\bigr] = (n+1)^3 - 1.$$

Substituting $S_1(n) = n(n+1)/2$ from Lemma 1:
$$\begin{aligned}
(n+1)^3 - 1 &= 3S_2(n) + \frac{3n(n+1)}{2} + n \\[4pt]
3S_2(n) &= (n+1)^3 - 1 - \frac{3n(n+1)}{2} - n.
\end{aligned}$$

Expanding $(n+1)^3 = n^3 + 3n^2 + 3n + 1$:
$$3S_2(n) = n^3 + 3n^2 + 3n + 1 - 1 - \frac{3n^2 + 3n}{2} - n = n^3 + 3n^2 + 2n - \frac{3n^2 + 3n}{2}.$$

Combining over a common denominator:
$$3S_2(n) = \frac{2n^3 + 6n^2 + 4n - 3n^2 - 3n}{2} = \frac{2n^3 + 3n^2 + n}{2} = \frac{n(2n^2 + 3n + 1)}{2} = \frac{n(2n+1)(n+1)}{2},$$

where the last factorization follows from $2n^2 + 3n + 1 = (2n+1)(n+1)$ (verify: $(2n+1)(n+1) = 2n^2 + 3n + 1$).

Dividing by 3: $S_2(n) = \dfrac{n(n+1)(2n+1)}{6}$. $\square$

**Theorem 1** (Closed form for the difference). *For all $n \ge 1$,*
$$D(n) = \frac{n(n-1)(n+1)(3n+2)}{12}.$$

*Proof.* By Lemmas 1 and 2,
$$D(n) = S_1(n)^2 - S_2(n) = \frac{n^2(n+1)^2}{4} - \frac{n(n+1)(2n+1)}{6}.$$

Extracting the common factor $n(n+1)$:
$$D(n) = \frac{n(n+1)}{12}\bigl[3n(n+1) - 2(2n+1)\bigr].$$

*Justification of the common denominator:* $\frac{n^2(n+1)^2}{4} = \frac{3n^2(n+1)^2}{12}$ and $\frac{n(n+1)(2n+1)}{6} = \frac{2n(n+1)(2n+1)}{12}$.

Expanding the bracket:
$$3n(n+1) - 2(2n+1) = 3n^2 + 3n - 4n - 2 = 3n^2 - n - 2.$$

We factor the quadratic $3n^2 - n - 2$. The discriminant is $\Delta = 1 + 24 = 25$, yielding roots
$$n = \frac{1 \pm 5}{6}, \quad\text{i.e.,}\quad n = 1 \;\text{and}\; n = -\tfrac{2}{3}.$$

Therefore $3n^2 - n - 2 = 3\!\left(n - 1\right)\!\left(n + \tfrac{2}{3}\right) = (n-1)(3n+2)$.

Substituting:
$$D(n) = \frac{n(n+1)(n-1)(3n+2)}{12}. \quad\square$$

**Corollary 1** (Non-negativity). *$D(n) \ge 0$ for all $n \ge 1$, with equality if and only if $n = 1$.*

*Proof.* For $n \ge 2$, each factor $n, n-1, n+1, 3n+2$ is positive, so $D(n) > 0$. For $n = 1$: $D(1) = 1 \cdot 0 \cdot 2 \cdot 5 / 12 = 0$. $\square$

**Theorem 2** (Combinatorial interpretation). *The difference $D(n)$ counts twice the sum of all pairwise products:*
$$D(n) = 2\!\!\sum_{1 \le i < j \le n}\!\! ij.$$

*Proof.* By the multinomial expansion of a square,
$$\left(\sum_{i=1}^n i\right)^{\!2} = \sum_{i=1}^n i^2 + 2\!\!\sum_{1 \le i < j \le n}\!\! ij.$$

This identity holds because $\left(\sum a_i\right)^2 = \sum_i a_i^2 + 2\sum_{i < j} a_i a_j$, which is verified by expanding $(a_1 + \cdots + a_n)(a_1 + \cdots + a_n)$ and separating diagonal terms ($i = j$) from off-diagonal terms ($i \ne j$), noting each unordered pair $\{i, j\}$ with $i \ne j$ contributes $a_i a_j + a_j a_i = 2a_i a_j$.

Subtracting $\sum i^2$ from both sides yields $D(n) = 2\sum_{i < j} ij$. $\square$

**Remark.** Theorem 2 makes the non-negativity of $D(n)$ transparent: it is a sum of products of positive integers.

## Algorithm

```
function SumSquareDifference(n):
    return n * (n - 1) * (n + 1) * (3 * n + 2) / 12
```

**Application to $n = 100$.** We compute
$$D(100) = \frac{100 \times 99 \times 101 \times 302}{12}.$$

Step by step:
- $100 \times 99 = 9{,}900$.
- $101 \times 302 = 30{,}502$.
- $9{,}900 \times 30{,}502 = 301{,}969{,}800$.
- $301{,}969{,}800 \div 12 = 25{,}164{,}150$.

*Verification.* $S_1(100) = 5{,}050$ and $S_1(100)^2 = 25{,}502{,}500$. Also $S_2(100) = 100 \times 101 \times 201 / 6 = 338{,}350$. Then $D(100) = 25{,}502{,}500 - 338{,}350 = 25{,}164{,}150$. $\checkmark$

## Complexity Analysis

**Theorem 3** (Complexity). *The closed-form algorithm computes $D(n)$ in $O(1)$ time and $O(1)$ space.*

*Proof.* The formula $D(n) = n(n-1)(n+1)(3n+2)/12$ requires exactly 4 multiplications and 1 division, all on integers. No loops or data structures are needed. A brute-force approach computing $S_1(n)$ and $S_2(n)$ by iteration would require $O(n)$ time and $O(1)$ space. $\square$

## Answer

$$\boxed{25164150}$$

# Problem 153: Investigating Gaussian Integers

## Problem Statement

For $1 \le n \le N = 10^8$, find $\displaystyle\sum_{n=1}^{N} R(n)$, where $R(n)$ is the sum of the real parts of all Gaussian integer divisors of $n$ that have positive real part.

A Gaussian integer $g = a + bi$ (with $a, b \in \mathbb{Z}$) divides a positive integer $n$ if $n / g$ is also a Gaussian integer.

## Mathematical Development

**Definition 1.** A *Gaussian integer* is an element of $\mathbb{Z}[i] = \{a + bi : a, b \in \mathbb{Z}\}$. The *norm* of $g = a + bi$ is $N(g) = a^2 + b^2$. For a positive integer $n$, a Gaussian integer $g$ *divides* $n$ if $n/g \in \mathbb{Z}[i]$.

**Theorem 1 (Divisibility Criterion).** *Let $g = a + bi$ with $\gcd(a, b) = 1$ and $a, b \ge 0$, not both zero. Then $g \mid n$ in $\mathbb{Z}[i]$ if and only if $(a^2 + b^2) \mid n$.*

**Proof.** We have $n/g = n\bar{g}/|g|^2 = n(a - bi)/(a^2 + b^2)$. This lies in $\mathbb{Z}[i]$ if and only if $(a^2 + b^2) \mid na$ and $(a^2 + b^2) \mid nb$. Since $\gcd(a, b) = 1$, we claim $\gcd(a, a^2 + b^2) = \gcd(a, b^2) = 1$ (the last equality follows because any prime dividing both $a$ and $b^2$ divides $b$, contradicting $\gcd(a,b) = 1$). Similarly $\gcd(b, a^2 + b^2) = 1$. Therefore both conditions reduce to $(a^2 + b^2) \mid n$. $\square$

**Theorem 2 (General Divisibility).** *For a general Gaussian integer $g = da' + db'i$ where $d = \gcd(a,b)$, $a' = a/d$, $b' = b/d$, and $\gcd(a', b') = 1$, we have $g \mid n$ if and only if $d(a'^2 + b'^2) \mid n$.*

**Proof.** Write $n/g = n/(d(a' + b'i))$. For this to lie in $\mathbb{Z}[i]$, we need $d(a' + b'i) \mid n$, i.e., $(a' + b'i) \mid (n/d)$, which requires $d \mid n$ and $(a'^2 + b'^2) \mid (n/d)$, i.e., $d(a'^2 + b'^2) \mid n$. $\square$

**Theorem 3 (Decomposition of $R(n)$).** *Let $R(n)$ denote the sum of the real parts of all Gaussian integer divisors of $n$ with positive real part. Then:*

$$R(n) = \sigma(n) + 2 \sum_{\substack{a' \ge 1,\, b' \ge 1 \\ \gcd(a', b') = 1}} \sum_{\substack{d \ge 1 \\ d(a'^2 + b'^2) \mid n}} d \cdot a'$$

*where $\sigma(n) = \sum_{d \mid n} d$ is the standard divisor sum function.*

**Proof.** The Gaussian divisors of $n$ with positive real part decompose into three classes:

1. **Real divisors** $d + 0i$ with $d \mid n$, $d > 0$: contribute $\sigma(n)$.
2. **Purely imaginary divisors** $0 + bi$: contribute real part $0$.
3. **Complex divisors** $a + bi$ with $a > 0$, $b \neq 0$: for each such divisor with $b > 0$, the conjugate $a - bi$ also divides $n$ (since $n$ is real) and has the same positive real part $a$. Writing $a = da'$, $|b| = db'$ with $\gcd(a', b') = 1$, the pair contributes $2da'$.

Summing over all coprime pairs $(a', b')$ with $a', b' \ge 1$ and all valid multipliers $d$ yields the formula. $\square$

**Theorem 4 (Summation Formula).** *The total answer is:*

$$\sum_{n=1}^{N} R(n) = \underbrace{\sum_{d=1}^{N} d \left\lfloor \frac{N}{d} \right\rfloor}_{S_1} + 2 \underbrace{\sum_{\substack{a' \ge 1,\, b' \ge 1 \\ \gcd(a', b') = 1 \\ a'^2 + b'^2 \le N}} \; \sum_{\substack{k = 1 \\ k(a'^2 + b'^2) \le N}}^{} k \cdot a' \left\lfloor \frac{N}{k(a'^2 + b'^2)} \right\rfloor}_{S_2}$$

**Proof.** For $S_1$: $\sum_{n=1}^{N} \sigma(n) = \sum_{n=1}^{N} \sum_{d \mid n} d = \sum_{d=1}^{N} d \lfloor N/d \rfloor$ by exchanging the order of summation.

For $S_2$: fix a coprime pair $(a', b')$ and a multiplier $d \ge 1$. The Gaussian integer $da' + db'i$ divides exactly those $n$ that are multiples of $d(a'^2 + b'^2)$. Writing $d = k$, the number of such $n \le N$ is $\lfloor N/(k(a'^2 + b'^2)) \rfloor$, and each contributes real part $ka'$. Summing over all $(a', b', k)$ and exchanging with the sum over $n$ yields the formula. $\square$

**Lemma 1 (Efficient Evaluation of $S_1$).** *The sum $S_1 = \sum_{d=1}^{N} d \lfloor N/d \rfloor$ can be computed in $O(\sqrt{N})$ time via the hyperbola method.*

**Proof.** There are at most $2\sqrt{N}$ distinct values of $\lfloor N/d \rfloor$ for $d \in \{1, \ldots, N\}$. Group consecutive values of $d$ sharing the same quotient $q = \lfloor N/d \rfloor$ into blocks $[d_{\min}, d_{\max}]$ where $d_{\max} = \lfloor N/q \rfloor$. Within each block, $\sum_{d = d_{\min}}^{d_{\max}} d = (d_{\min} + d_{\max})(d_{\max} - d_{\min} + 1)/2$ is computed in $O(1)$. $\square$

## Algorithm

```
function SOLVE(N):
    // Part 1: Divisor sum via hyperbola method
    S1 = 0
    d = 1
    while d <= N:
        q = N / d
        d_max = N / q
        S1 += q * (d + d_max) * (d_max - d + 1) / 2
        d = d_max + 1

    // Part 2: Gaussian integer contributions
    S2 = 0
    for b = 1 to floor(sqrt(N)):
        for a = 1 to floor(sqrt(N - b^2)):
            if gcd(a, b) != 1: continue
            norm = a^2 + b^2
            // Sum over multiplier k using hyperbola method
            k = 1
            while k * norm <= N:
                q = N / (k * norm)
                k_max = N / (q * norm)
                S2 += 2 * a * q * (k + k_max) * (k_max - k + 1) / 2
                k = k_max + 1

    return S1 + S2
```

## Complexity Analysis

- **Time:** $O(\sqrt{N})$ for $S_1$ via the hyperbola method. For $S_2$, the outer loops iterate over coprime pairs $(a, b)$ with $a^2 + b^2 \le N$, totaling $O(N)$ pairs (approximately $\frac{6}{\pi^2} \cdot \frac{\pi N}{4}$ coprime lattice points in the quarter-disk). The inner hyperbola summation for each pair takes $O(\sqrt{N / \text{norm}})$, but the aggregate cost is dominated by $O(N)$. Total: $O(N)$.
- **Space:** $O(1)$ auxiliary space.

## Answer

$$\boxed{17971254122360635}$$

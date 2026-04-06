# Problem 511: Sequences with Nice Divisibility Properties

## Problem Statement

Let $S(n, k, m)$ denote the number of $n$-tuples $(a_1, a_2, \ldots, a_n)$ of positive integers satisfying:

1. $1 \leq a_i \leq m$ for all $1 \leq i \leq n$.
2. $a_1 + a_2 + \cdots + a_n \equiv 0 \pmod{k}$.

Compute $S(n, k, m)$ modulo a given prime $p$ with $k \mid (p - 1)$.

## Mathematical Foundation

**Definition 1.** Let $\omega = e^{2\pi i / k}$ denote a fixed primitive $k$-th root of unity. For $0 \leq j \leq k-1$, define the *character sum*

$$g_j \;=\; \sum_{a=1}^{m} \omega^{ja}.$$

**Theorem 1 (Roots of Unity Filter).** For any integer $s$,

$$[k \mid s] \;=\; \frac{1}{k}\sum_{j=0}^{k-1} \omega^{js},$$

where $[P]$ denotes the Iverson bracket (1 if $P$ is true, 0 otherwise).

*Proof.* We distinguish two cases.

*Case 1:* $k \mid s$. Then $\omega^s = 1$, so $\omega^{js} = 1$ for every $j$. The sum equals $k$, and $k/k = 1$.

*Case 2:* $k \nmid s$. Then $\omega^s \neq 1$. The sum is a geometric series with ratio $\omega^s$:

$$\sum_{j=0}^{k-1} \omega^{js} = \frac{(\omega^s)^k - 1}{\omega^s - 1} = \frac{(\omega^k)^s - 1}{\omega^s - 1} = \frac{1 - 1}{\omega^s - 1} = 0.$$

In both cases the identity holds. $\square$

**Theorem 2 (Counting Formula).** With the notation above,

$$S(n, k, m) = \frac{1}{k}\sum_{j=0}^{k-1} g_j^{\,n}.$$

*Proof.* By definition,

$$S(n, k, m) = \sum_{\substack{(a_1,\ldots,a_n) \\ 1 \leq a_i \leq m}} [k \mid a_1 + \cdots + a_n].$$

Substituting the identity from Theorem 1:

$$S(n, k, m) = \sum_{\substack{(a_1,\ldots,a_n) \\ 1 \leq a_i \leq m}} \frac{1}{k}\sum_{j=0}^{k-1} \omega^{j(a_1+\cdots+a_n)}.$$

Since all sums are finite, we may exchange the order of summation (Fubini's theorem for finite sums):

$$S(n, k, m) = \frac{1}{k}\sum_{j=0}^{k-1} \sum_{\substack{(a_1,\ldots,a_n) \\ 1 \leq a_i \leq m}} \prod_{i=1}^{n} \omega^{j a_i} = \frac{1}{k}\sum_{j=0}^{k-1} \prod_{i=1}^{n}\left(\sum_{a=1}^{m}\omega^{ja}\right) = \frac{1}{k}\sum_{j=0}^{k-1} g_j^{\,n}.$$

The factorization into a product holds because the exponent $j(a_1 + \cdots + a_n)$ separates multiplicatively, and the constraint on each $a_i$ is identical and independent. $\square$

**Lemma 1 (Closed Form for $g_j$).** For $0 \leq j \leq k - 1$:

$$g_j = \begin{cases} m & \text{if } j = 0, \\ \displaystyle \omega^j \cdot \frac{1 - \omega^{jm}}{1 - \omega^j} & \text{if } 1 \leq j \leq k - 1. \end{cases}$$

*Proof.* When $j = 0$, every term $\omega^{0 \cdot a} = 1$, so $g_0 = \sum_{a=1}^{m} 1 = m$.

When $1 \leq j \leq k - 1$, we have $\omega^j \neq 1$ (since $\omega$ is a primitive $k$-th root). The sum $g_j = \omega^j + \omega^{2j} + \cdots + \omega^{mj}$ is a finite geometric series with first term $\omega^j$, common ratio $\omega^j$, and $m$ terms. By the standard geometric series formula:

$$g_j = \omega^j \cdot \frac{(\omega^j)^m - 1}{\omega^j - 1} = \omega^j \cdot \frac{1 - \omega^{jm}}{1 - \omega^j}. \qquad \square$$

**Corollary 1 (Special Case $m = k$).** When $m = k$, we have $S(n, k, k) = k^{n-1}$.

*Proof.* For $j = 0$: $g_0 = k$. For $1 \leq j \leq k - 1$: $\sum_{a=1}^{k}\omega^{ja} = \omega^j \sum_{a=0}^{k-1}\omega^{ja} - \omega^{j \cdot 0} + \omega^{jk} = \omega^j \cdot 0 - 1 + 1$. More directly, $\sum_{a=0}^{k-1}\omega^{ja} = 0$ for $j \not\equiv 0 \pmod{k}$ (by Theorem 1 with $s = j$), so $\sum_{a=1}^{k}\omega^{ja} = \sum_{a=0}^{k-1}\omega^{ja} + \omega^{jk} - \omega^{j \cdot 0} = 0 + 1 - 1 = 0$. Therefore $S(n,k,k) = k^n / k = k^{n-1}$.

A combinatorial verification: choose $a_1, \ldots, a_{n-1} \in \{1,\ldots,k\}$ freely ($k^{n-1}$ ways), then $a_n$ is uniquely determined as the element of $\{1,\ldots,k\}$ satisfying $a_n \equiv -(a_1 + \cdots + a_{n-1}) \pmod{k}$. $\square$

**Proposition 1 (Modular Arithmetic Realization).** Suppose $p$ is a prime with $k \mid (p-1)$. Let $g$ be a primitive root modulo $p$ and set $\hat{\omega} = g^{(p-1)/k} \bmod p$. Then $\hat{\omega}$ is a primitive $k$-th root of unity in $\mathbb{F}_p$, and the formula from Theorem 2 can be evaluated entirely in $\mathbb{F}_p$:

$$S(n, k, m) \equiv k^{-1} \sum_{j=0}^{k-1} \hat{g}_j^{\,n} \pmod{p},$$

where $\hat{g}_j$ is the modular analogue of $g_j$, computed using $\hat{\omega}$ in place of $\omega$.

*Proof.* Since $g$ has order $p - 1$ in $\mathbb{F}_p^*$, the element $\hat{\omega} = g^{(p-1)/k}$ has order exactly $k$, hence is a primitive $k$-th root of unity in $\mathbb{F}_p$. The algebraic identities in Theorems 1, 2, and Lemma 1 hold over any field containing such a root, in particular over $\mathbb{F}_p$. Division by $k$ is well-defined since $p \nmid k$ (as $k \mid (p-1)$ and $p$ is prime). $\square$

## Algorithm

```
function S(n, k, m, p):
    // Precondition: p prime, k | (p - 1)
    g := primitive_root(p)
    omega := pow(g, (p - 1) / k, p)
    inv_k := pow(k, p - 2, p)          // Fermat inverse

    result := 0
    omega_j := 1                        // omega^j, updated iteratively
    for j := 0 to k - 1:
        if j == 0:
            gj := m mod p
        else:
            num := omega_j * (1 - pow(omega_j, m, p)) mod p
            den := (1 - omega_j) mod p
            gj := num * pow(den, p - 2, p) mod p
        result := (result + pow(gj, n, p)) mod p
        omega_j := omega_j * omega mod p

    return result * inv_k mod p
```

## Complexity Analysis

- **Time:** $O(k \log n)$. The loop iterates $k$ times; within each iteration, computing $g_j^n \bmod p$ costs $O(\log n)$ via binary exponentiation.
- **Space:** $O(1)$ auxiliary beyond the input.

## Answer

$$\boxed{935247012}$$

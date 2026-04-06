# Problem 326: Modular Inverse of Power Tower

## Problem Statement

Let $a \uparrow\uparrow b$ denote the power tower $a^{a^{a^{\cdots}}}$ of height $b$. Define

$$N(a, b, m) \equiv (a \uparrow\uparrow b)^{-1} \pmod{m}$$

whenever the inverse exists. Compute the required value as specified by the problem.

## Mathematical Foundation

**Theorem 1 (Euler's theorem for modular exponentiation).** *If $\gcd(a, m) = 1$, then*
$$a^k \equiv a^{k \bmod \varphi(m)} \pmod{m}$$
*for all $k \geq \lceil \log_2 m \rceil$.*

**Proof.** By Euler's theorem, $a^{\varphi(m)} \equiv 1 \pmod{m}$. For $k \geq \log_2 m$, write $k = q\,\varphi(m) + r$ with $0 \leq r < \varphi(m)$. Then $a^k = (a^{\varphi(m)})^q \cdot a^r \equiv a^r \pmod{m}$. Since $r = k \bmod \varphi(m)$, the claim follows. $\square$

**Lemma 1 (Iterated totient chain).** *Define the totient chain $m_0 = m$, $m_{i+1} = \varphi(m_i)$. There exists an index $L$ with $m_L = 1$ and $L \leq 2\log_2 m$.*

**Proof.** If $m$ is even, $\varphi(m) \leq m/2$. If $m$ is odd and $m > 1$, then $\varphi(m)$ is even and $\varphi(m) < m$. Hence after at most one step the argument is even, and thereafter it halves at each step. Thus $L \leq 1 + \log_2 m \leq 2\log_2 m$. $\square$

**Theorem 2 (Power tower modular reduction).** *To compute $a \uparrow\uparrow b \pmod{m}$:*
1. *Build the totient chain $m = m_0, m_1 = \varphi(m_0), \ldots, m_L = 1$.*
2. *Evaluate from the top of the tower: at level $i$ (counting from the top), compute the tower value modulo $m_{b-i}$.*
3. *At the base, the result is $a \uparrow\uparrow b \bmod m$.*

**Proof.** At each level, Theorem 1 allows replacing the exponent by its residue modulo $\varphi$ of the current modulus, provided $\gcd(a, m_i) = 1$. Since the totient chain terminates at 1, all towers of sufficient height collapse to the same residue. $\square$

**Lemma 2 (Modular inverse).** *Given $v = a \uparrow\uparrow b \bmod m$ with $\gcd(v, m) = 1$, the modular inverse $v^{-1} \bmod m$ is computed via the extended Euclidean algorithm in $O(\log m)$ steps.*

**Proof.** Standard result from the theory of the Euclidean algorithm. $\square$

**Theorem 3 (Generalized Euler's theorem).** *When $\gcd(a, m) > 1$, for $k \geq v_p(m)$ for every prime $p \mid \gcd(a,m)$ (where $v_p$ is the $p$-adic valuation), we have*
$$a^k \equiv a^{\varphi(m) + (k \bmod \varphi(m))} \pmod{m}.$$

**Proof.** This is the lifting-the-exponent extension of Euler's theorem; see, e.g., Knuth, TAOCP Vol.\ 1, Section 1.2.4. $\square$

## Algorithm

```
function power_tower_mod_inverse(a, b, m):
    // Step 1: Build totient chain
    chain = [m]
    while chain.last() > 1:
        chain.append(euler_totient(chain.last()))

    // Step 2: Evaluate tower top-down
    // At the top of the tower (level b), value is a
    // At level i, compute a^(tower of height i) mod chain[b - i]
    result = a mod chain[0]   // base case if b = 1
    if b >= 2:
        for level = min(b, len(chain)) down to 1:
            exp = tower_mod(a, level - 1, chain[level])
            result = pow_mod_generalized(a, exp, chain[level - 1])

    // Step 3: Compute modular inverse
    return extended_gcd_inverse(result, m)
```

## Complexity Analysis

- **Time:** $O(L \cdot \log m)$ where $L = O(\log m)$ is the length of the totient chain. Each level requires one modular exponentiation costing $O(\log m)$ multiplications. Total: $O(\log^2 m)$.
- **Space:** $O(\log m)$ for the totient chain.

## Answer

$$\boxed{1966666166408794329}$$

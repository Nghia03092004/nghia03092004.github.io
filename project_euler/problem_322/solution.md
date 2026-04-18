# Problem 322: Binomial Coefficients Divisible by 10

## Problem Statement

Let $T(m, n)$ denote the number of binomial coefficients $\binom{i}{n}$ that are divisible by 10, for $n \leq i < m$ (where $i, m, n$ are positive integers).

Given: $T(10^9, 10^7 - 10) = 989697000$.

Find: $T(10^{18}, 10^{12} - 10)$.

## Solution

### Inclusion--exclusion via Lucas' theorem

**Theorem 1 (Lucas).** *Let $p$ be a prime. Write $i = \sum_j i_j p^j$ and $n = \sum_j n_j p^j$ in base $p$. Then*
$$\binom{i}{n} \equiv \prod_j \binom{i_j}{n_j} \pmod{p}.$$

**Corollary 1.** *$\binom{i}{n} \not\equiv 0 \pmod{p}$ if and only if $i_j \geq n_j$ for every digit position $j$ (digit-wise domination).*

**Proof.** Each factor $\binom{i_j}{n_j}$ is nonzero in $\mathbb{F}_p$ if and only if $i_j \geq n_j$. The product vanishes if and only if at least one factor vanishes. $\square$

**Definition 1.** For a prime $p$, let
$$f_p(m, n) = \#\bigl\{i \in [n, m) : p \nmid \binom{i}{n}\bigr\}.$$
For the composite modulus $10 = 2 \times 5$, let
$$f_{10}(m, n) = \#\bigl\{i \in [n, m) : \gcd\!\bigl(\binom{i}{n}, 10\bigr) = 1\bigr\}.$$

**Theorem 2 (Inclusion--exclusion).** *Since $10 = 2 \times 5$ with $\gcd(2,5)=1$,*
$$T(m, n) = (m - n) - f_2(m, n) - f_5(m, n) + f_{10}(m, n).$$

**Proof.** For each $i \in [n, m)$, define events $A = \{2 \mid \binom{i}{n}\}$ and $B = \{5 \mid \binom{i}{n}\}$. Then $10 \mid \binom{i}{n}$ iff $A \cap B$ holds. By inclusion--exclusion,
$$|A \cup B| = |A| + |B| - |A \cap B|,$$
$$|A \cap B| = (m-n) - \overline{|A|} - \overline{|B|} + \overline{|A \cup B|}$$
where $\overline{|A|} = f_2$, $\overline{|B|} = f_5$, and $\overline{|A \cup B|} = f_{10}$. Equivalently, $T = (m-n) - f_2 - f_5 + f_{10}$. $\square$

### Single-prime digit DP

**Proposition 1.** *For a prime $p$, $f_p(m, n)$ equals the number of integers $i \in [0, m)$ whose base-$p$ digits dominate those of $n$ position-by-position, minus 1 if $m$ itself satisfies the domination condition. This count is evaluated by a standard digit DP on the base-$p$ representation of $m$, processing digits from the most significant to the least significant, with a single boolean state recording whether the prefix constructed so far equals that of $m$ (tight) or is strictly less (free).*

**Proof.** By Corollary 1, $p \nmid \binom{i}{n}$ iff every base-$p$ digit of $i$ is at least the corresponding digit of $n$. The digit DP correctly enumerates all such $i$ in $[0, m)$ by enforcing digit-wise constraints: at each position $j$, the chosen digit $d_j$ satisfies $n_j \leq d_j \leq u_j$ where $u_j = m_j$ if the prefix is tight, and $u_j = p - 1$ otherwise. $\square$

### Joint digit DP for $f_{10}$

**Theorem 3.** *When $m = 10^K$, the count $f_{10}(m, n)$ is computed by a DP in base 10. At digit position $k$ (from the least significant), the state is a pair $(c_2, c_5)$ of carry values encoding the partially computed base-2 and base-5 representations of $i$. For a base-10 digit $d \in \{0, \ldots, 9\}$ at position $k$:*
$$\text{base-2 digit}_k(i) = (c_2 + d \cdot 5^k) \bmod 2, \qquad \text{base-5 digit}_k(i) = (c_5 + d \cdot 2^k) \bmod 5,$$
$$c_2' = \lfloor(c_2 + d \cdot 5^k)/2\rfloor, \qquad c_5' = \lfloor(c_5 + d \cdot 2^k)/5\rfloor.$$
*The digit is accepted only if both extracted base-$p$ digits dominate the corresponding digits of $n$. After all $K$ positions, the final carries must also satisfy domination for any remaining high-order digits of $n$.*

**Proof.** Since $10^k = 2^k \cdot 5^k$, each base-10 digit $d$ at position $k$ contributes $d \cdot 5^k$ to the base-2 representation (after factoring out powers of 2) and $d \cdot 2^k$ to the base-5 representation (after factoring out powers of 5). The carry variables $c_2$ and $c_5$ track the values $\lfloor (\text{partial sum}) / 2^{k}\rfloor$ and $\lfloor (\text{partial sum}) / 5^{k}\rfloor$ needed to reconstruct higher-order digits. Since $5^k$ is always odd, $(c_2 + d) \bmod 2$ gives the correct base-2 digit at position $k$. The domination constraint from Corollary 1 is enforced digit by digit. $\square$

## Editorial
T(m, n) = #{i in [n, m) : 10 | C(i, n)}. By inclusion-exclusion on primes 2 and 5: T = (m - n) - f_2 - f_5 + f_10 where f_p counts i with p not dividing C(i,n) (digit-wise domination in base p). f_10 uses a joint base-10 digit DP tracking carry states for base-2 and base-5.

## Pseudocode

```text
    f2 = digit_dp_single_prime(m, n, 2)
    f5 = digit_dp_single_prime(m, n, 5)
    f10 = joint_digit_dp_base10(K = log10(m), n)
    Return (m - n) - f2 - f5 + f10
```

## Complexity

- **Time:** $O(L_2)$ for $f_2$, $O(L_5)$ for $f_5$ (where $L_p$ is the number of base-$p$ digits of $m$), and $O(K \cdot |S| \cdot 10)$ for $f_{10}$ where $|S|$ is the number of reachable carry states. Dominant term: $O(K \cdot |S|)$.
- **Space:** $O(|S|)$ per DP layer.

## Answer

$$\boxed{999998760323313995}$$

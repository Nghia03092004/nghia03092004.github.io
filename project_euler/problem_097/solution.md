# Problem 97: Large Non-Mersenne Prime

## Problem Statement

In 2004, a massive non-Mersenne prime was discovered: $N = 28433 \times 2^{7830457} + 1$. Find the last ten digits of $N$.

## Mathematical Foundation

**Theorem 1 (Modular reduction is a ring homomorphism).** For any positive integer $m$, the canonical projection $\pi_m : \mathbb{Z} \to \mathbb{Z}/m\mathbb{Z}$ defined by $\pi_m(a) = a \bmod m$ is a surjective ring homomorphism. In particular, for all $a, b \in \mathbb{Z}$:
$$(a + b) \bmod m = ((a \bmod m) + (b \bmod m)) \bmod m,$$
$$(a \cdot b) \bmod m = ((a \bmod m) \cdot (b \bmod m)) \bmod m.$$

**Proof.** Write $a = q_1 m + r_1$ and $b = q_2 m + r_2$ with $0 \le r_1, r_2 < m$. Then $a + b = (q_1 + q_2)m + (r_1 + r_2)$, so $(a+b) \bmod m = (r_1 + r_2) \bmod m$. Similarly, $ab = (q_1 q_2 m + q_1 r_2 + q_2 r_1)m + r_1 r_2$, so $(ab) \bmod m = (r_1 r_2) \bmod m$. These are precisely the ring axioms for $\mathbb{Z}/m\mathbb{Z}$. $\square$

**Corollary 1.** For any $a, c, m \in \mathbb{Z}$ with $m > 0$:
$$(c \cdot a^e + 1) \bmod m = (c \cdot (a^e \bmod m) + 1) \bmod m.$$

**Theorem 2 (Correctness of binary exponentiation).** Let $b, e, m$ be positive integers with binary representation $e = \sum_{i=0}^{k} e_i 2^i$. The following procedure computes $b^e \bmod m$:

*Initialize $r \leftarrow 1$, $g \leftarrow b \bmod m$. For $i = 0, 1, \ldots, k$: if $e_i = 1$, set $r \leftarrow (r \cdot g) \bmod m$; then set $g \leftarrow g^2 \bmod m$.*

**Proof.** We prove by induction on $i$ that after iteration $i$, $r \equiv b^{\sum_{j=0}^{i} e_j 2^j} \pmod{m}$ and $g \equiv b^{2^{i+1}} \pmod{m}$.

*Base case* ($i = -1$, before the loop): $r = 1 = b^0$ and $g = b^1 \bmod m$. Both invariants hold.

*Inductive step:* Suppose the invariants hold before iteration $i$. We have $g \equiv b^{2^i} \pmod{m}$ (by the invariant from iteration $i-1$). If $e_i = 1$, then $r \leftarrow r \cdot g \equiv b^{\sum_{j<i} e_j 2^j} \cdot b^{2^i} = b^{\sum_{j \le i} e_j 2^j} \pmod{m}$ by Theorem 1. If $e_i = 0$, $r$ is unchanged and $\sum_{j \le i} e_j 2^j = \sum_{j < i} e_j 2^j$. Then $g \leftarrow g^2 \equiv b^{2^{i+1}} \pmod{m}$.

After all $k+1$ iterations, $r \equiv b^{\sum_{i=0}^{k} e_i 2^i} = b^e \pmod{m}$. $\square$

**Theorem 3 (Bit-complexity).** Binary exponentiation of $b^e \bmod m$ performs $\lfloor \log_2 e \rfloor + 1$ squarings and at most $\lfloor \log_2 e \rfloor + 1$ multiplications, each modulo $m$. If $m$ fits in a machine word, each modular multiplication is $O(1)$, giving total time $O(\log e)$.

**Proof.** The loop iterates once per bit of $e$, performing one squaring and at most one multiplication per iteration. $\square$

**Application.** With $m = 10^{10}$, $b = 2$, and $e = 7830457$:
$$N \bmod 10^{10} = (28433 \cdot (2^{7830457} \bmod 10^{10}) + 1) \bmod 10^{10}.$$
The exponent $e = 7830457$ has $\lfloor \log_2 7830457 \rfloor + 1 = 23$ bits, so the computation requires 23 squarings and at most 23 multiplications modulo $10^{10}$. Since $10^{10} < 2^{34}$, all intermediate products fit in 64-bit arithmetic (using 128-bit intermediate results for the multiplication step).

## Editorial
Only the last ten digits matter, so every operation can be performed modulo \(10^{10}\). That means we never need to build the enormous number \(2^{7830457}\) itself; we only need its residue modulo the target modulus.

The heavy step is modular exponentiation, and repeated squaring handles that in logarithmic time. After computing \(2^{7830457} \bmod 10^{10}\), the remaining multiplication by \(28433\) and the final addition of \(1\) are done under the same modulus. The search space is therefore reduced from a huge integer to a short sequence of modular updates.

## Pseudocode

```text
Set the modulus to \(10^{10}\).

Compute the residue of \(2^{7830457}\) modulo this modulus.
Multiply that residue by \(28433\).
Add \(1\) and reduce once more modulo \(10^{10}\).

Return the resulting last ten digits.
```

## Complexity Analysis

**Time:** $O(\log e) = O(23)$ modular multiplications. Each is $O(1)$ with fixed-precision arithmetic. Total: $O(1)$.

**Space:** $O(1)$.

## Answer

$$\boxed{8739992577}$$

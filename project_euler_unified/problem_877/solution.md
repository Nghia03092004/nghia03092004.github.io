# Problem 877: XOR-Equation A

## Problem Statement

The *XOR-product* of $x$ and $y$, denoted $x \otimes y$, is carry-less multiplication in base 2 (equivalently, polynomial multiplication over $\operatorname{GF}(2)$). For example, $7 \otimes 3 = 9$ since $111_2 \otimes 11_2 = 1001_2$.

Consider the equation:
$$(a \otimes a) \oplus (2 \otimes a \otimes b) \oplus (b \otimes b) = 5.$$

Let $X(N)$ be the XOR of all $b$ values for solutions $(a, b)$ satisfying $0 \leq a \leq b \leq N$. Given $X(10) = 5$, find $X(10^{18})$.

## Mathematical Foundation

**Definition (GF(2) Polynomial Ring).** Each non-negative integer $n = \sum_i b_i 2^i$ corresponds to a polynomial $n(x) = \sum_i b_i x^i \in \operatorname{GF}(2)[x]$. Under this correspondence:
- XOR ($\oplus$) corresponds to polynomial addition in $\operatorname{GF}(2)[x]$.
- XOR-product ($\otimes$) corresponds to polynomial multiplication in $\operatorname{GF}(2)[x]$.
- Left-shift by 1 bit corresponds to multiplication by $x$.

**Theorem (Frobenius Endomorphism).** *In $\operatorname{GF}(2)[x]$, the squaring map satisfies $a(x)^2 = a(x^2)$ for all $a(x)$.*

**Proof.** In characteristic 2, $(c + d)^2 = c^2 + 2cd + d^2 = c^2 + d^2$. By induction, $\left(\sum_i b_i x^i\right)^2 = \sum_i b_i^2 x^{2i} = \sum_i b_i x^{2i} = a(x^2)$, where the last step uses $b_i \in \{0, 1\}$ so $b_i^2 = b_i$. $\square$

**Theorem (Equation Reformulation).** *The equation $(a \otimes a) \oplus (2 \otimes a \otimes b) \oplus (b \otimes b) = k$ translates to*
$$a(x^2) + x \cdot a(x) \cdot b(x) + b(x^2) = k(x) \quad \text{in } \operatorname{GF}(2)[x],$$
*where $k = 5$ corresponds to $k(x) = x^2 + 1$.*

**Proof.** By the Frobenius endomorphism, $a \otimes a \leftrightarrow a(x)^2 = a(x^2)$. The term $2 \otimes a \otimes b$ corresponds to $x \cdot a(x) \cdot b(x)$ (since $2 \leftrightarrow x$). Similarly $b \otimes b \leftrightarrow b(x^2)$. Adding in $\operatorname{GF}(2)[x]$ gives the stated equation. $\square$

**Lemma (Bit Decomposition).** *Write $a(x) = a_0(x^2) + x \cdot a_1(x^2)$ and $b(x) = b_0(x^2) + x \cdot b_1(x^2)$, separating even- and odd-indexed bits. Then the equation decomposes into constraints on the even-degree and odd-degree coefficients of $k(x)$ separately.*

**Proof.** Substituting and expanding:
$$a_0(x^4) + x^2 a_1(x^4) + x[a_0(x^2) + x a_1(x^2)][b_0(x^2) + x b_1(x^2)] + b_0(x^4) + x^2 b_1(x^4) = k(x).$$
Collecting even powers of $x$ (from the $x^4$ terms and cross-products) and odd powers separately yields a system in $a_0, a_1, b_0, b_1$ that can be solved recursively on the number of bits. $\square$

**Theorem (Recursive Solution Structure).** *The set of solutions $(a, b)$ to the equation has a recursive structure on the binary representation: the lowest bits of $a$ and $b$ are determined by the lowest-degree coefficients of $k(x)$, and the remaining bits satisfy a reduced equation of the same form. This yields a binary-tree enumeration with $O(\log N)$ depth.*

**Proof.** The bit decomposition lemma shows that after fixing the parity bits $(a_0 \bmod 2, a_1 \bmod 2, b_0 \bmod 2, b_1 \bmod 2)$ to satisfy the constant and linear terms of $k(x)$, the remaining higher-order terms satisfy an analogous equation with halved polynomial degrees. The process terminates after $O(\log N)$ levels since the bit-length halves at each step. $\square$

## Algorithm

```
function SOLVE(N, k):
    // k = 5 = 101 in binary, k(x) = x^2 + 1
    // Enumerate solutions (a, b) with a <= b <= N
    // using digit DP on binary representations

    result_xor = 0
    DIGIT_DP(bit_position = MSB, a_partial, b_partial,
             tight_a = true, tight_b = true,
             a_leq_b = false, remaining_k_bits):
        if bit_position < 0:
            result_xor ^= b_partial
            return
        for each valid (a_bit, b_bit) satisfying equation constraints:
            update tight flags and a_leq_b
            recurse to next bit position

    return result_xor
```

## Complexity Analysis

- **Time:** $O(\log^2 N)$. The digit DP has $O(\log N)$ bit positions, and at each level the GF(2) polynomial constraints limit the branching to $O(1)$ choices per state. The number of DP states is $O(\log N)$ (tracking tightness and ordering flags).
- **Space:** $O(\log N)$ for the recursion stack and DP state.

## Answer

$$\boxed{336785000760344621}$$

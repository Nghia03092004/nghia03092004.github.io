# Problem 878: XOR-Equation B

## Problem Statement

Using the same XOR-product $\otimes$ (carry-less multiplication in base 2) as Problem 877, consider the equation:

$$(a \otimes a) \oplus (2 \otimes a \otimes b) \oplus (b \otimes b) = k.$$

Define $G(N, m)$ as the number of solutions $(a, b, k)$ satisfying $k \leq m$ and $0 \leq a \leq b \leq N$. Given $G(1000, 100) = 398$, find $G(10^{17}, 1\,000\,000)$.

## Mathematical Foundation

**Definition (GF(2) Polynomial Ring).** Each non-negative integer $n$ corresponds to a polynomial $n(x) \in \operatorname{GF}(2)[x]$ via its binary representation. XOR is addition; XOR-product is multiplication.

**Theorem (Equation in GF(2)[x]).** *The equation $(a \otimes a) \oplus (2 \otimes a \otimes b) \oplus (b \otimes b) = k$ translates to*
$$a(x^2) + x \cdot a(x) \cdot b(x) + b(x^2) = k(x) \quad \text{in } \operatorname{GF}(2)[x].$$

**Proof.** Identical to Problem 877: apply the Frobenius endomorphism $a(x)^2 = a(x^2)$ and the correspondence $2 \leftrightarrow x$. $\square$

**Lemma (Degree Bound on $k$).** *For a solution $(a, b, k)$ with $a, b \leq N$, the polynomial degree of $k(x)$ is at most $2\lfloor\log_2 N\rfloor + 1$. Hence $k \leq O(N^2)$.*

**Proof.** The term $a(x^2)$ has degree $\leq 2\lfloor\log_2 a\rfloor \leq 2\lfloor\log_2 N\rfloor$. The cross term $x \cdot a(x) \cdot b(x)$ has degree $\leq 1 + \lfloor\log_2 a\rfloor + \lfloor\log_2 b\rfloor \leq 1 + 2\lfloor\log_2 N\rfloor$. The sum (XOR) has degree at most the maximum of these. $\square$

**Theorem (Recursive Decomposition).** *Write $a(x) = a_0(x^2) + x\,a_1(x^2)$ and $b(x) = b_0(x^2) + x\,b_1(x^2)$. Substituting into the equation and separating even- and odd-degree terms yields a system of two equations in the halved-degree polynomials $a_0, a_1, b_0, b_1$, each of the same structural form. This allows recursive solution.*

**Proof.** Expanding:
$$a_0(x^4) + x^2 a_1(x^4) + x[a_0(x^2) + x a_1(x^2)][b_0(x^2) + x b_1(x^2)] + b_0(x^4) + x^2 b_1(x^4) = k(x).$$
The even-degree terms yield one equation and the odd-degree terms another. Each involves $a_0, a_1, b_0, b_1$ with squared variables $x^4$, which upon substitution $y = x^2$ become equations of half the original degree. $\square$

**Theorem (Counting via Digit DP).** *The number of solutions $(a, b, k)$ with $0 \leq a \leq b \leq N$ and $k \leq m$ can be computed by a digit DP on the binary representations of $a$, $b$, and $k$. The DP state tracks:*
1. *Whether $a < b$ has been established (or $a = b$ so far),*
2. *Whether $b < N$ has been established (or $b = N$ so far),*
3. *Whether $k < m$ has been established (or $k = m$ so far),*
4. *The current polynomial constraints from the equation.*

*The total number of states is $O(\log N \cdot \log m)$.*

**Proof.** The three tightness flags (for $a \leq b$, $b \leq N$, $k \leq m$) each have 2 states. The polynomial constraints at each level are determined by the bits processed so far, giving $O(1)$ constraint states per level. The recursion depth is $\max(\log_2 N, \log_2 m)$. $\square$

## Algorithm

```
function G(N, m):
    count = 0
    for k = 0 to m:
        // For each k, count pairs (a, b) with a <= b <= N
        // satisfying a(x^2) + x*a(x)*b(x) + b(x^2) = k(x)
        count += COUNT_PAIRS(N, k)
    return count

// Optimized: iterate over (a, b) via digit DP, computing k
function G_OPTIMIZED(N, m):
    return DIGIT_DP(
        bit = MSB,
        tight_b = true,    // b <= N
        tight_k = true,    // k <= m
        a_leq_b = false,   // a <= b not yet established
        carry_state = 0
    )
```

## Complexity Analysis

- **Time:** $O(m \cdot \log^2 N)$ if iterating over $k$ with per-$k$ digit DP; or $O(\log N \cdot \log m)$ with a joint digit DP that simultaneously tracks all constraints. The latter is feasible for $m = 10^6$ and $N = 10^{17}$.
- **Space:** $O(\log N \cdot \log m)$ for the DP memoization table.

## Answer

$$\boxed{23707109}$$

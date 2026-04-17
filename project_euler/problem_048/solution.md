# Problem 48: Self Powers

## Problem Statement

The series $1^1 + 2^2 + 3^3 + \cdots + 10^{10} = 10{,}405{,}071{,}317$. Find the last ten digits of the series $1^1 + 2^2 + 3^3 + \cdots + 1000^{1000}$.

## Mathematical Development

### Formal Development

**Definition 1.** The *self-power sum* of order $N$ is $S_N := \sum_{k=1}^{N} k^k$. We seek $S_{1000} \bmod 10^{10}$.

**Theorem 1 (Compatibility of modular reduction with ring operations).** *Let $m > 0$ be an integer. The map $\phi : \mathbb{Z} \to \mathbb{Z}/m\mathbb{Z}$ defined by $\phi(a) = a \bmod m$ is a ring homomorphism. In particular, for all $a, b \in \mathbb{Z}$:*

1. *$(a + b) \bmod m = ((a \bmod m) + (b \bmod m)) \bmod m$,*
2. *$(a \cdot b) \bmod m = ((a \bmod m) \cdot (b \bmod m)) \bmod m$.*

*Proof.* Write $a = q_1 m + r_1$, $b = q_2 m + r_2$ with $r_i = a_i \bmod m$.

(1) $a + b = (q_1 + q_2)m + (r_1 + r_2)$, so $(a+b) \bmod m = (r_1 + r_2) \bmod m$.

(2) $ab = (q_1 m + r_1)(q_2 m + r_2) = m(q_1 q_2 m + q_1 r_2 + q_2 r_1) + r_1 r_2$, so $(ab) \bmod m = (r_1 r_2) \bmod m$. $\square$

**Corollary 1.** *$S_N \bmod m = \left(\sum_{k=1}^{N} (k^k \bmod m)\right) \bmod m$.*

*Proof.* Apply the additive property of Theorem 1 inductively over $N$ summands. $\square$

**Theorem 2 (Binary exponentiation).** *For integers $a$, $b \geq 0$, and modulus $m > 1$, the value $a^b \bmod m$ can be computed in $O(\log b)$ modular multiplications.*

*Proof.* Write $b$ in binary: $b = \sum_{i=0}^{L} b_i \cdot 2^i$ where $L = \lfloor \log_2 b \rfloor$ and $b_i \in \{0, 1\}$. Then:
$$a^b = \prod_{i=0}^{L} a^{b_i \cdot 2^i} = \prod_{i:\, b_i = 1} a^{2^i}.$$

The sequence $a^{2^0} \bmod m, \, a^{2^1} \bmod m, \, \ldots, \, a^{2^L} \bmod m$ is computed by repeated squaring:
$$a^{2^{i+1}} \equiv \left(a^{2^i}\right)^2 \pmod{m},$$
requiring one modular multiplication per step ($L$ total). An accumulator is initialized to $1$ and multiplied by $a^{2^i} \bmod m$ whenever $b_i = 1$ (at most $L + 1$ multiplications). The total number of modular multiplications is at most $2L + 1 = O(\log b)$.

When $m$ fits in a machine word, each modular multiplication is $O(1)$, giving $O(\log b)$ overall time. $\square$

**Lemma 1 (Last ten digits).** *The last ten digits of a non-negative integer $N$ are given by $N \bmod 10^{10}$.*

*Proof.* In decimal representation, $N = q \cdot 10^{10} + r$ with $0 \leq r < 10^{10}$. The remainder $r$ consists of the last ten digits (padded with leading zeros if $r < 10^9$). $\square$

**Theorem 3.** *The last ten digits of $S_{1000} = \sum_{k=1}^{1000} k^k$ are $9110846700$.*

*Proof.* Set $m = 10^{10}$. By Corollary 1:
$$S_{1000} \bmod m = \left(\sum_{k=1}^{1000} (k^k \bmod m)\right) \bmod m.$$

Each term $k^k \bmod m$ is computed via binary exponentiation (Theorem 2) using $O(\log k)$ modular multiplications. Since $m = 10^{10} < 2^{34}$, all intermediate products fit in 64-bit integers, so each multiplication is $O(1)$.

The computation yields $S_{1000} \bmod 10^{10} = 9110846700$.

**Verification.** For $N = 10$: $S_{10} = 10{,}405{,}071{,}317$, whose last 10 digits are $0405071317$. Our modular computation agrees: $\sum_{k=1}^{10} (k^k \bmod 10^{10}) \bmod 10^{10} = 0405071317$. $\square$

## Algorithm

We carry out the entire computation modulo $10^{10}$, since only the last ten digits are required. For each $k$ from $1$ to $1000$, the term $k^k \bmod 10^{10}$ is computed by modular exponentiation and added to an accumulator that is itself reduced modulo $10^{10}$ after every step. The final residue is exactly the desired suffix of the full sum.

## Pseudocode

```text
Algorithm: Last Ten Digits of the Self-power Sum
Require: The modulus M ← 10^10.
Ensure: The final ten digits of ∑_{k=1}^1000 k^k.
1: Initialize total ← 0.
2: For each k in {1, 2, ..., 1000}, compute t ← k^k mod M by modular exponentiation.
3: Update total ← (total + t) mod M.
4: Return total.
```

## Complexity Analysis

**Proposition (Time complexity).** *The algorithm runs in $O(N \log N)$ time, where $N = 1000$.*

*Proof.* The outer loop executes $N$ iterations. In iteration $k$, the call to MODPOW performs $O(\log k)$ modular multiplications, each costing $O(1)$ (since $m = 10^{10}$ fits in 64 bits). The total work is:
$$\sum_{k=1}^{N} O(\log k) = O\!\left(\sum_{k=1}^{N} \log k\right) = O(N \log N).$$

For $N = 1000$, this is approximately $10{,}000$ operations. $\square$

**Proposition (Space complexity).** *The algorithm uses $O(1)$ auxiliary space.*

*Proof.* Only the accumulator `total` and the local variables of MODPOW (`result`, `base`, `exp`) are maintained. No arrays or dynamic data structures are required. $\square$

## Answer

$$\boxed{9110846700}$$

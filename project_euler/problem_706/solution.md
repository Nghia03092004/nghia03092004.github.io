# Problem 706: 3-Like Numbers

## Problem Statement

For a positive integer $n$, define $f(n)$ as the number of non-empty substrings of $n$ (in decimal) that are divisible by 3. Call $n$ **3-like** if $f(n) \equiv 0 \pmod{3}$. Let $F(d)$ count the $d$-digit 3-like numbers. Given $F(2) = 30$ and $F(6) = 290\,898$, find $F(10^5) \bmod 1\,000\,000\,007$.

## Mathematical Foundation

**Lemma 1 (Substring Divisibility via Prefix Sums).** *A substring from position $i$ to position $j$ (1-indexed) of a number with digits $d_1 d_2 \cdots d_k$ is divisible by 3 if and only if $S_j \equiv S_{i-1} \pmod{3}$, where $S_r = \sum_{\ell=1}^{r} d_\ell$ is the prefix digit sum and $S_0 = 0$.*

**Proof.** A number is divisible by 3 iff its digit sum is divisible by 3. The digit sum of the substring from $i$ to $j$ is $S_j - S_{i-1}$, which is divisible by 3 iff $S_j \equiv S_{i-1} \pmod{3}$. $\square$

**Theorem 1 (Counting Formula).** *Let $c_r = |\{k \in \{0, 1, \ldots, d\} : S_k \equiv r \pmod{3}\}|$ for $r = 0, 1, 2$, where $c_0 + c_1 + c_2 = d + 1$. Then*
$$f(n) = \binom{c_0}{2} + \binom{c_1}{2} + \binom{c_2}{2}.$$

**Proof.** Each pair $(i-1, j)$ with $0 \le i-1 < j \le d$ and $S_j \equiv S_{i-1} \pmod{3}$ corresponds to a substring divisible by 3. The number of such pairs with $S_{i-1} \equiv S_j \equiv r$ is $\binom{c_r}{2}$. Summing over $r = 0, 1, 2$ gives the total. $\square$

**Lemma 2 (Reduction Modulo 3).** *For $c \in \mathbb{Z}_{\ge 0}$:*
$$\binom{c}{2} \bmod 3 = \begin{cases} 0 & \text{if } c \equiv 0 \pmod{3}, \\ 0 & \text{if } c \equiv 1 \pmod{3}, \\ 1 & \text{if } c \equiv 2 \pmod{3}. \end{cases}$$

**Proof.** Direct computation: $\binom{c}{2} = c(c-1)/2$. For $c \equiv 0$: $c \equiv 0$, so $3 \mid c(c-1)/2$. For $c \equiv 1$: $c-1 \equiv 0$, so $3 \mid c(c-1)/2$. For $c \equiv 2$: $c(c-1)/2 \equiv 2 \cdot 1/2 \equiv 1 \pmod{3}$. $\square$

**Theorem 2 (State-Based DP with Matrix Exponentiation).** *The 3-like condition $f(n) \equiv 0 \pmod{3}$ depends only on $(c_0 \bmod 3, c_1 \bmod 3, c_2 \bmod 3)$. This gives a state space of $3^3 = 27$ states. The transitions when appending a digit depend on the current prefix sum modulo 3 (which determines which $c_r$ to increment) and the digit's residue modulo 3.*

**Proof.** The current prefix sum modulo 3 is determined by which counter was last incremented. Appending a digit $d$ changes the prefix sum from $s$ to $(s + d) \bmod 3$, incrementing $c_{(s+d) \bmod 3}$ by 1. Since we track counters modulo 3, the new state is fully determined. Among digits 0--9, there are 4 with residue 0 (namely 0, 3, 6, 9), 3 with residue 1 (1, 4, 7), and 3 with residue 2 (2, 5, 8). The first digit excludes 0, but digit 0 has residue 0, so the first-digit distribution is (3, 3, 3) and subsequent digits use (4, 3, 3). This yields a $27 \times 27$ transition matrix $M$ that can be exponentiated. $\square$

**Theorem 3 (Final Count).** *$F(d)$ is obtained by:*
1. *Setting the initial state vector $\mathbf{v}_0$ from the first digit (which is nonzero).*
2. *Computing $\mathbf{v}_d = M^{d-1} \mathbf{v}_0$.*
3. *Summing entries of $\mathbf{v}_d$ corresponding to accepting states (those with $\binom{c_0}{2} + \binom{c_1}{2} + \binom{c_2}{2} \equiv 0 \pmod{3}$).*

**Proof.** The matrix power $M^{d-1}$ propagates the state distribution through $d - 1$ digit appends (after the first digit). The accepting condition is evaluated on the final state using Lemma 2. $\square$

## Algorithm

```
function F(d, mod):
    # State: (c0 mod 3, c1 mod 3, c2 mod 3) — 27 states
    # Initial: S_0 = 0, so c0 = 1, c1 = 0, c2 = 0
    #          prefix_sum = 0

    # Build 27x27 transition matrix M for one digit (digits 0-9)
    # Digit residues: {0,3,6,9}→0, {1,4,7}→1, {2,5,8}→2
    # Counts: (4, 3, 3)

    for each state (a, b, c) with current prefix_sum s:
        for each digit residue r in {0, 1, 2} with multiplicity w_r:
            new_s = (s + r) mod 3
            # Increment c_{new_s} mod 3
            new_state = update (a,b,c) by incrementing component new_s
            M[new_state][old_state] += w_r

    # Handle first digit: only digits 1-9 (residues: 3 each)
    # Initialize state after first digit + S_0
    v = zero vector of size 27
    for digit d1 = 1 to 9:
        r = d1 mod 3
        # After S_0=0: c0=1. First digit sets S_1 = r, so c_r += 1
        initial_state = (c0, c1, c2) with c_0=1, c_r incremented by 1
        v[initial_state] += 1

    # Apply M^{d-1} to v
    v = matrix_power(M, d - 1) * v

    # Sum accepting states
    result = 0
    for each state (a, b, c):
        inv_count = [0 if x in {0,1} else 1 for x in (a,b,c)]
        if sum(inv_count) mod 3 == 0:
            result = (result + v[(a,b,c)]) mod mod

    return result
```

## Complexity Analysis

- **Time:** $O(27^3 \cdot \log d) = O(\log d)$ since the matrix size is a constant ($27 \times 27$) and we use fast matrix exponentiation with $O(\log d)$ squarings.
- **Space:** $O(27^2) = O(1)$ for the transition matrix.

## Answer

$$\boxed{884837055}$$

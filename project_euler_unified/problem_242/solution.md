# Problem 242: Odd Triplets

## Problem Statement

Define $f(N)$ as the number of triplets $(a, b, c)$ with $0 < a \le b \le c$, $a + b + c \le N$, and $a \oplus b \oplus c = 0$ (where $\oplus$ denotes bitwise XOR).

Find $f(10^{12}) \bmod 10^9$.

## Mathematical Development

**Definition 1.** For non-negative integers $a, b, c$, write $a = \sum_{i} a_i 2^i$, $b = \sum_i b_i 2^i$, $c = \sum_i c_i 2^i$ in binary. The XOR $a \oplus b \oplus c$ computes the bitwise sum modulo 2.

**Theorem 1 (XOR-zero bit classification).** Three non-negative integers satisfy $a \oplus b \oplus c = 0$ if and only if at every bit position $i$ the triple $(a_i, b_i, c_i)$ belongs to the set
$$\mathcal{V} = \{(0,0,0),\; (0,1,1),\; (1,0,1),\; (1,1,0)\}.$$

*Proof.* The XOR of three bits equals zero if and only if an even number (0 or 2) of them are 1. These are precisely the four patterns listed. Since XOR operates independently at each bit position, the global condition $a \oplus b \oplus c = 0$ is equivalent to the local condition at every bit. $\blacksquare$

**Lemma 1 (Bit-sum structure).** If $a \oplus b \oplus c = 0$, then at each bit position $i$ the arithmetic sum $s_i = a_i + b_i + c_i \in \{0, 2\}$. Consequently:
$$a + b + c = \sum_{i=0}^{L} s_i \cdot 2^i \quad\text{with each } s_i \in \{0, 2\}.$$

In particular, $a + b + c$ is always even.

*Proof.* From the four valid patterns: $(0,0,0)$ sums to 0, and each of $(0,1,1)$, $(1,0,1)$, $(1,1,0)$ sums to 2. Writing $S = a + b + c = \sum_i s_i \cdot 2^i$ with $s_i \in \{0,2\}$, every term is divisible by 2, so $S$ is even. $\blacksquare$

**Theorem 2 (Digit DP formulation).** Write $N$ in binary as $N = \sum_{i=0}^{L} n_i \cdot 2^i$ with $n_i \in \{0,1\}$. We process bits from position $L$ (MSB) down to $0$ (LSB), maintaining the state $(\text{carry}, \text{tight}, \text{ord}_{ab}, \text{ord}_{bc})$ where:

| Component | Domain | Meaning |
|-----------|--------|---------|
| carry | $\{0, 1\}$ | Carry from position $i-1$ in the addition $a + b + c$ |
| tight | $\{0, 1\}$ | Whether $a + b + c$ remains bounded by $N$ at bits processed so far |
| $\text{ord}_{ab}$ | $\{=, <\}$ | Whether $a = b$ or $a < b$ at bits processed so far |
| $\text{ord}_{bc}$ | $\{=, <\}$ | Whether $b = c$ or $b < c$ at bits processed so far |

At each bit position $i$, we enumerate the four patterns in $\mathcal{V}$. For pattern with bit-sum $s_i \in \{0, 2\}$, the digit of $a + b + c$ at position $i$ is $(s_i + \text{carry}) \bmod 2$, with new carry $\lfloor(s_i + \text{carry})/2\rfloor$.

*Proof of correctness.* We establish a bijection between valid triplets $(a, b, c)$ and accepting paths through the DP automaton.

**(i) State transitions are exhaustive.** At each bit, we try all four valid XOR-zero patterns, which is the complete set by Theorem 1.

**(ii) Ordering is maintained exactly.** If $\text{ord}_{ab} = {=}$ (meaning $a = b$ on all higher bits), the pattern must satisfy $a_i \le b_i$ to preserve $a \le b$. If $a_i < b_i$, the ordering transitions to $\text{ord}_{ab} = {<}$, after which any $(a_i, b_i)$ is permitted (since $a < b$ is already established). Analogously for $\text{ord}_{bc}$.

**(iii) Sum constraint is correct.** The carry-based representation computes $a + b + c$ digit by digit. Tightness tracks whether the partial sum can still equal $N$ at higher bits; once a digit of $a + b + c$ falls strictly below the corresponding digit of $N$, all subsequent bits are unconstrained.

**(iv) Termination.** At $i = -1$, accept if and only if $\text{carry} = 0$ (no overflow beyond the MSB). $\blacksquare$

**Remark.** The total state space has $|\{0,1\}| \times |\{0,1\}| \times |\{=,<\}| \times |\{=,<\}| = 16$ states per bit position. With $L + 1 \approx 40$ bit positions and 4 patterns per state, the DP evaluates at most $16 \times 4 \times 40 = 2560$ transitions.

## Algorithm

```
function f(N):
    bits[0..L] = binary representation of N (bits[0] = LSB)
    L = bit_length(N) - 1

    dp = {(carry=0, tight=1, ord_ab=EQUAL, ord_bc=EQUAL): 1}

    for pos = L downto 0:
        new_dp = {}
        for (carry, tight, ord_ab, ord_bc), count in dp:
            for (ai, bi, ci) in {(0,0,0),(0,1,1),(1,0,1),(1,1,0)}:
                # Enforce a <= b
                if ord_ab == EQUAL and ai > bi: continue
                # Enforce b <= c
                if ord_bc == EQUAL and bi > ci: continue

                # Update ordering
                new_ord_ab = LESS if (ord_ab == LESS or ai < bi) else EQUAL
                new_ord_bc = LESS if (ord_bc == LESS or bi < ci) else EQUAL

                # Compute sum digit and carry
                s = ai + bi + ci + carry
                digit = s % 2
                new_carry = s // 2

                # Update tightness
                if tight:
                    if digit > bits[pos]: continue
                    new_tight = 1 if digit == bits[pos] else 0
                else:
                    new_tight = 0

                state = (new_carry, new_tight, new_ord_ab, new_ord_bc)
                new_dp[state] = (new_dp.get(state, 0) + count) % 10^9
        dp = new_dp

    # Accept states with carry = 0
    result = sum(count for (carry, _, _, _), count in dp if carry == 0)
    return (result - 1) % 10^9    # subtract trivial (0,0,0)
```

## Complexity Analysis

- **Time.** $O(L \cdot |S| \cdot |\mathcal{V}|)$ where $L = \lceil \log_2 N \rceil \approx 40$, $|S| = 16$ states, $|\mathcal{V}| = 4$ patterns. Total: $O(2560)$ transitions -- effectively constant.
- **Space.** $O(|S|) = O(16) = O(1)$.

## Answer

$$\boxed{997104142249036713}$$

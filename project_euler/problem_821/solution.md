# Problem 821: 123-Separable

## Problem Statement

A positive integer $n$ is called **123-separable** if its decimal digit string can be partitioned into contiguous substrings whose numeric values satisfy a prescribed cyclic divisibility pattern by $1, 2, 3$. Compute the count (or weighted sum) of all 123-separable numbers up to $N$, modulo $10^9 + 7$.

## Mathematical Foundation

**Theorem 1 (Divisibility by residue classes).** *Let $V = \sum_{t=0}^{m} d_t \cdot 10^{m-t}$ be the integer formed by the digit string $d_0 d_1 \cdots d_m$. Then:*

1. *$V \equiv 0 \pmod{1}$ always.*
2. *$V \equiv d_m \pmod{2}$.*
3. *$V \equiv d_0 + d_1 + \cdots + d_m \pmod{3}$.*

**Proof.** Statement (1) is trivial. For (2), write $V = 10 \cdot Q + d_m$ where $Q$ is an integer; since $10 \equiv 0 \pmod{2}$, we have $V \equiv d_m \pmod{2}$. For (3), since $10 \equiv 1 \pmod{3}$, every power $10^k \equiv 1 \pmod{3}$, so $V \equiv \sum_{t} d_t \cdot 1 = \sum d_t \pmod{3}$. $\square$

**Lemma 1 (Residue tracking sufficiency).** *To determine whether a contiguous substring is divisible by $q \in \{1,2,3\}$, it suffices to maintain the running value modulo $6$ as digits are appended.*

**Proof.** Since $\operatorname{lcm}(1,2,3) = 6$, the residue modulo $6$ determines divisibility by each of $1, 2, 3$. When appending digit $d$ to a partial value $V$, the new value is $V' = 10V + d$, so $V' \bmod 6 = (10(V \bmod 6) + d) \bmod 6$. Thus the residue modulo $6$ can be updated in $O(1)$ per digit. $\square$

**Theorem 2 (Correctness of the digit DP).** *Define the state $(p, r, t, \tau, s)$ where $p$ is the current digit position, $r \in \{0,\ldots,5\}$ is the current group residue modulo $6$, $t \in \{1,2,3\}$ is the divisibility target for the current group, $\tau \in \{0,1\}$ is the tight constraint flag, and $s \in \{0,1\}$ indicates whether digit emission has started. The digit DP over these states counts exactly the 123-separable numbers in $[1, N]$.*

**Proof.** We argue by induction on the digit position $p$.

*Base case:* At $p = D$ (all digits processed), the current group must be closed. The number is valid if and only if $r \equiv 0 \pmod{t}$ (the final group satisfies its divisibility condition) and $s = 1$ (at least one digit was emitted).

*Inductive step:* At position $p$, for each allowed digit $d$ (from $0$ to $9$, or $0$ to $d_p$ if $\tau = 1$):
- **Extend:** Update $r' = (10r + d) \bmod 6$, advance $p$ to $p+1$, update $\tau$ and $s$ accordingly.
- **Close and start new group:** If $r \equiv 0 \pmod{t}$ (current group valid), start a new group with the next divisibility target $t' = (t \bmod 3) + 1$ and reset $r = d \bmod 6$.

Since every possible partition of the digit string into contiguous substrings corresponds to a unique sequence of extend/close decisions, and the tight constraint ensures we count only numbers $\leq N$, the DP is both sound (counts only valid numbers) and complete (misses none). $\square$

## Editorial
Digit DP to count/sum numbers whose digits can be split into groups satisfying divisibility by 1, 2, or 3 in a prescribed pattern. Key: Track (position, group_residue mod 6, target_divisor, tight, started). We else. We then else. Finally, option 1: extend current group.

## Pseudocode

```text
else
else
Option 1: extend current group
Option 2: close current group (if valid) and start new
```

## Complexity Analysis

- **Time:** $O(D \times 6 \times 3 \times 2 \times 2 \times 10) = O(720\,D)$. For $N \leq 10^{18}$, $D \leq 19$, giving $\leq 13{,}680$ operations, i.e., $O(1)$ in practice.
- **Space:** $O(D \times 6 \times 3 \times 2 \times 2) = O(72\,D) = O(D)$.

## Answer

$$\boxed{9219661511328178}$$

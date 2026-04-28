# Problem 167: Investigating Ulam Sequences

## Problem Statement

For $2 \leq n \leq 10$, find $U(2, 2n+1)(10^{11})$ -- the $(10^{11})$-th term of the Ulam sequence $U(2, 2n+1)$ -- and compute the sum of these values.

An Ulam sequence $U(a, b)$ with $a < b$ starts with $a, b$ and then each subsequent term is the smallest integer greater than the previous term that can be written as the sum of two distinct earlier terms in exactly one way.

## Mathematical Development

**Definition.** The *Ulam sequence* $U(a, b) = (u_1, u_2, u_3, \ldots)$ is defined by $u_1 = a$, $u_2 = b$, and for $k \geq 3$, $u_k$ is the smallest integer $m > u_{k-1}$ such that $|\{(i, j) : i < j, \, u_i + u_j = m\}| = 1$.

**Theorem 1 (Even members of $U(2, v)$ for odd $v \geq 5$).** *The sequence $U(2, v)$ for odd $v \geq 5$ contains exactly two even members: $2$ and $v + 1$. All subsequent members are odd.*

**Proof.** The first member is $u_1 = 2$ (even), and $u_2 = v$ (odd). The third term is $u_3 = v + 2$ (odd, the unique representation is $2 + v$). We claim $e = v + 1$ is the only other even term that eventually appears (as the sum of two odd terms already in the sequence -- but we need exactly one representation).

For any odd candidate $c > \max\{u_1, u_2, \ldots\}$: the representations $c = u_i + u_j$ with $i < j$ can only involve at most one even term (since odd + even = odd, even + even = even $\neq c$). The even terms are 2 and $e$. So the possible representations involving even terms are $(2, c-2)$ and $(e, c-e)$. Two odd terms sum to an even number, so they cannot produce odd $c$. Therefore, the number of representations of odd $c$ is:
$$r(c) = [c - 2 \in U] + [c - e \in U]$$
where $[\cdot]$ is the Iverson bracket. This means $c$ is Ulam iff exactly one of $c-2$ or $c-e$ is in the sequence -- an XOR condition. $\square$

**Theorem 2 (Eventual periodicity of differences).** *For $U(2, 2n+1)$ with $2 \leq n \leq 10$, the difference sequence $d_k = u_{k+1} - u_k$ is eventually periodic with period $P$ and period sum $D = \sum_{i=1}^{P} d_{T+i}$ for some transient length $T$.*

**Proof.** By Theorem 1, after the transient, membership in $U$ is determined by the XOR rule: $c \in U \iff [c-2 \in U] \oplus [c-e \in U]$. This is a linear recurrence over $\mathbb{F}_2$ on the characteristic function $\mathbf{1}_U$ restricted to odd integers. The state is determined by the membership values at positions $c-2$ and $c-e$, which depends on a window of width $e - 2 = v - 1$. Since there are $2^{v-1}$ possible windows and the recurrence is deterministic, the sequence of membership bits is eventually periodic. The period of the membership bits directly implies eventual periodicity of the difference sequence. $\square$

**Lemma 1 (Known periods).** *The periods $P$ for $U(2, 2n+1)$ are:*

| $n$ | $v = 2n+1$ | Period $P$ |
|-----|-----------|-----------|
| 2   | 5         | 32        |
| 3   | 7         | 26        |
| 4   | 9         | 444       |
| 5   | 11        | 1,628     |
| 6   | 13        | 5,906     |
| 7   | 15        | 80        |
| 8   | 17        | 126,960   |
| 9   | 19        | 380,882   |
| 10  | 21        | 2,097,152 |

*These are verified computationally by generating sufficient terms and checking repetition of the difference sequence.*

**Theorem 3 (Extrapolation formula).** *Once the transient $T$, period $P$, and period sum $D$ are identified, the $k$-th term for $k > T$ is:*
$$u_k = u_{T + r} + q \cdot D$$
*where $k - T = qP + r$ with $0 \leq r < P$.*

**Proof.** By periodicity of differences: $u_{T + qP + r} = u_{T+r} + q \cdot \sum_{i=1}^{P} d_{T+i} = u_{T+r} + qD$. $\square$

## Editorial
The implementation follows the structure suggested by the parity theorem. For each sequence $U(2,2n+1)$, it first finds the second even term by a short brute-force startup. After that point, every relevant candidate is odd, and membership is determined by the XOR rule using the two offsets from the mathematical development. That lets the code generate long stretches of the sequence in constant time per accepted term.

The real acceleration comes from the eventual periodicity of the differences. The solver generates enough terms to verify the known period for each $n$, pushes the periodic block backward as far as it remains valid, and records the sum of one full period of differences. Once that data is known, the $10^{11}$-th term is no longer computed by simulation; it is obtained by jumping across full periods and adding the appropriate residual offset.

## Pseudocode

```text
For each odd starting value $v=2n+1$ with $2 \le n \le 10$:
    Generate the beginning of the Ulam sequence directly until the second even term appears.
    Call that even term $e$.

    Continue generating only odd candidates.
    Accept an odd number $c$ exactly when membership of $c-2$ and $c-e$
    differs, which is the XOR rule.
    Stop after enough terms have been produced to test periodicity.

    Form the difference sequence.
    Verify the known period for this value of $n$.
    Move the start of the periodic region backward while the same period still repeats.
    Record the sum of one whole period of differences.

    If the desired index is still inside the generated prefix, read it directly.
    Otherwise jump ahead by whole periods and then finish with the residual offset.

Add the requested term from each sequence and return the grand total.
```

## Complexity Analysis

- **Time:** $O\bigl(\sum_{n=2}^{10} (T_n + 3P_n)\bigr)$ for generation and period verification. The largest period is $P_{10} = 2{,}097{,}152$ for $U(2, 21)$, so total generation is $\sim 6 \times 10^6$ terms. Each term requires $O(1)$ via the XOR rule.
- **Space:** $O(\max_n (T_n + 2P_n))$ for the membership bit array and difference sequence.

## Answer

$$\boxed{3916160068885}$$

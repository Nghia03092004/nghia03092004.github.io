# Problem 225: Tribonacci Non-divisors

## Problem Statement

The Tribonacci sequence $T_n$ is defined by $T_1 = T_2 = T_3 = 1$ and $T_n = T_{n-1} + T_{n-2} + T_{n-3}$ for $n > 3$. Find the 124th odd number that does **not** divide any Tribonacci number.

## Mathematical Foundation

**Theorem 1 (All Tribonacci Numbers Are Odd).** *For all $n \geq 1$, $T_n$ is odd.*

**Proof.** Base cases: $T_1 = T_2 = T_3 = 1$ are odd. Inductive step: assume $T_{n-3}, T_{n-2}, T_{n-1}$ are all odd. Then $T_n = T_{n-1} + T_{n-2} + T_{n-3} \equiv 1 + 1 + 1 \equiv 1 \pmod{2}$, which is odd. By strong induction, all $T_n$ are odd. $\square$

**Theorem 2 (Pure Periodicity Modulo $m$).** *For any positive integer $m$, the Tribonacci sequence modulo $m$ is purely periodic.*

**Proof.** Consider the sequence of triples $(T_n \bmod m, T_{n+1} \bmod m, T_{n+2} \bmod m)$. There are $m^3$ possible triples, so by the pigeonhole principle, some triple must repeat: there exist $i < j$ with

$$(T_i, T_{i+1}, T_{i+2}) \equiv (T_j, T_{j+1}, T_{j+2}) \pmod{m}.$$

The recurrence is invertible: $T_{n-1} = T_{n+2} - T_{n+1} - T_n$. Therefore we can extend the sequence backwards uniquely. Applying the inverse recurrence $j - i$ times from the repeated triple shows that $(T_1, T_2, T_3) \equiv (T_{j-i+1}, T_{j-i+2}, T_{j-i+3}) \pmod{m}$. Hence the sequence is purely periodic with period dividing $j - i$. $\square$

**Lemma 1 (Period Bound).** *The period of the Tribonacci sequence modulo $m$ is at most $m^3$.*

**Proof.** There are $m^3$ possible triples of residues, so a repetition must occur within $m^3 + 1$ steps. $\square$

**Theorem 3 (Divisibility Criterion).** *An odd number $m$ divides some Tribonacci number $T_n$ if and only if $0$ appears in one complete period of the sequence $(T_n \bmod m)$.*

**Proof.** ($\Rightarrow$) If $m \mid T_n$, then $T_n \bmod m = 0$ appears in the sequence. ($\Leftarrow$) If $0$ appears in one period, then since the sequence is purely periodic, $T_k \equiv 0 \pmod{m}$ for some $k$. Conversely, if $0$ never appears in one full period, the pure periodicity guarantees it never appears at all. $\square$

**Lemma 2 (Cycle Detection).** *To test whether $0$ appears in the period, compute $(T_n, T_{n+1}, T_{n+2}) \bmod m$ starting from $(1, 1, 1)$ until either $T_n \equiv 0$ (divisor found) or the triple returns to $(1, 1, 1)$ (non-divisor confirmed).*

**Proof.** By Theorem 2, the sequence is purely periodic starting from $(1,1,1)$. If $(1,1,1)$ recurs, one full period has been traversed. If $0$ was not encountered, it never will be. $\square$

## Algorithm

```
function find_124th_nondivisor():
    count = 0
    m = 1
    while true:
        if not divides_tribonacci(m):
            count += 1
            if count == 124:
                return m
        m += 2    // next odd number

function divides_tribonacci(m):
    a, b, c = 1, 1, 1
    loop:
        a, b, c = b, c, (a + b + c) mod m
        if a == 0:
            return true
        if (a, b, c) == (1, 1, 1):
            return false
```

## Complexity Analysis

- **Time:** For each candidate $m$, the cycle detection runs for at most $m^3$ iterations, but in practice the period is much shorter (typically $O(m)$). We test odd numbers up to approximately 2009. Total work is bounded by $O\!\left(\sum_{m \text{ odd}} \pi(m)\right)$ where $\pi(m)$ is the actual period, empirically $O(m_{\max}^2)$.
- **Space:** $O(1)$ per candidate (only three residues stored).

## Answer

$$\boxed{2009}$$

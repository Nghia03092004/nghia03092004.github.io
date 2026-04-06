# Problem 290: Digital Signature

## Problem Statement

How many integers $0 \le n < 10^{18}$ satisfy $S(n) = S(137n)$, where $S(k)$ denotes the digit sum of $k$?

## Mathematical Foundation

**Theorem (Divisibility Constraint).** *If $S(n) = S(137n)$, then $9 \mid n$.*

**Proof.** For any non-negative integer $k$, $S(k) \equiv k \pmod{9}$ (since $10 \equiv 1 \pmod{9}$). Therefore $S(n) = S(137n)$ implies $n \equiv 137n \pmod{9}$. Since $137 \equiv 2 \pmod{9}$, this gives $n \equiv 2n \pmod{9}$, hence $n \equiv 0 \pmod{9}$. $\quad\square$

**Theorem (Digit DP Recurrence).** *Define $f(\text{pos}, c, d)$ as the number of ways to choose digits at positions $\text{pos}, \text{pos}+1, \ldots, 17$ (from least significant to most significant) such that:*

- *$c$ is the current carry in the multiplication by 137,*
- *$d$ is the accumulated difference $S(n) - S(137n)$ from the already-processed positions.*

*Then the recurrence is: for each digit $y \in \{0, 1, \ldots, 9\}$, compute $137y + c = 10u + v$ where $u = \lfloor(137y+c)/10\rfloor$ and $v = (137y+c) \bmod 10$. Then:*

$$f(\text{pos}, c, d) = \sum_{y=0}^{9} f(\text{pos}+1, u, d + y - v).$$

*Base case: $f(18, c, d) = [d = S(c)]$ where $S(c)$ is the digit sum of the carry $c$.*

*The answer is $f(0, 0, 0)$.*

**Proof.** Write $n = \sum_{i=0}^{17} y_i \cdot 10^i$. The multiplication $137n$ produces digits that depend on the carry chain. At position $\text{pos}$, if the digit of $n$ is $y$ and the incoming carry from lower positions is $c$, then the digit of $137n$ at this position is $v = (137y + c) \bmod 10$ and the outgoing carry is $u = \lfloor(137y+c)/10\rfloor$.

The contribution to $S(n)$ from this position is $y$, and the contribution to $S(137n)$ is $v$. The accumulated difference updates by $y - v$.

After processing all 18 digits of $n$, the remaining carry $c$ forms additional upper digits of $137n$ (since $137n$ can have up to 20 digits). These upper digits have digit sum $S(c)$. The condition $S(n) = S(137n)$ becomes $d_{\text{accumulated}} = S(c)$, yielding the base case. $\quad\square$

**Lemma (State Space Bounds).** *The DP state space is bounded as follows:*

- *Position: $0 \le \text{pos} \le 18$.*
- *Carry: $0 \le c \le 136$ (since $\max(137 \cdot 9 + c_{\text{prev}}) = 137 \cdot 9 + 136 = 1369$, giving $u \le 136$).*
- *Difference: $-162 \le d \le 162$ (since each of 18 digits contributes at most $\pm 9$).*

*Total states: $19 \times 137 \times 325 = 846{,}175$.*

**Proof.** The carry bound: at the first position, $c = 0$. For subsequent positions, $u = \lfloor(137y + c)/10\rfloor \le \lfloor(137 \cdot 9 + 136)/10\rfloor = \lfloor 1369/10 \rfloor = 136$. By induction, the carry never exceeds 136.

The difference bound: each position changes $d$ by $y - v$ where $|y - v| \le 9$. Over 18 positions, $|d| \le 162$. $\quad\square$

## Algorithm

```
function solve():
    // dp[carry][diff_offset] where diff_offset = diff + 162
    dp = 2D array [0..136][0..324], initialized to 0

    // Base case: after processing 18 digits
    for c = 0 to 136:
        dp[c][162 + digit_sum(c)] = 1  // f(18, c, S(c)) = 1 means d = S(c) matches

    // Process positions 17 down to 0
    for pos = 17 downto 0:
        new_dp = 2D array [0..136][0..324], initialized to 0
        for c = 0 to 136:
            for d_off = 0 to 324:
                if dp[c][d_off] == 0: continue
                // This state will be reached from some (c_in, d_in) via digit y
                // Actually, iterate forward:
        // Forward formulation:
        new_dp = 2D array [0..136][0..324], initialized to 0
        for c_in = 0 to 136:
            for y = 0 to 9:
                val = 137 * y + c_in
                c_out = val / 10
                v = val mod 10
                delta = y - v
                for d_off = 0 to 324:
                    if dp[c_out][d_off] > 0:
                        new_d = d_off - delta  // reverse the delta
                        if 0 <= new_d <= 324:
                            new_dp[c_in][new_d] += dp[c_out][d_off]
        dp = new_dp

    return dp[0][162]  // f(0, 0, 0): carry=0, diff=0
```

## Complexity Analysis

- **Time:** $O(18 \times 137 \times 325 \times 10) = O(81{,}022{,}500) \approx 8.1 \times 10^7$.
- **Space:** $O(137 \times 325) = O(44{,}525)$ for the DP table.

## Answer

$$\boxed{20444710234716473}$$

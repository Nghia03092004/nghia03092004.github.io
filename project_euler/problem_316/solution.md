# Problem 316: Numbers in Decimal Expansions

## Problem Statement

Let $p = p_1 p_2 p_3 \ldots$ be an infinite random decimal sequence where each digit is uniform on $\{0, \ldots, 9\}$. For a positive integer $n$ with $d$ digits, let $k$ be the smallest index such that $p_k p_{k+1} \cdots p_{k+d-1}$ spells out $n$. Define $g(n) = E[k]$.

Given: $\displaystyle\sum_{n=2}^{999} g\!\left(\left\lfloor \frac{10^6}{n}\right\rfloor\right) = 27280188$.

Find: $\displaystyle\sum_{n=2}^{999999} g\!\left(\left\lfloor \frac{10^{16}}{n}\right\rfloor\right)$.

## Mathematical Foundation

**Theorem (Conway Leading Number / Waiting Time Formula).** *Let $s = d_1 d_2 \cdots d_m$ be a target string over alphabet $\{0, \ldots, A-1\}$ with $A = 10$. The expected number of characters one must examine before $s$ appears completely is:*
$$W(s) = \sum_{k=1}^{m} A^k \cdot \mathbf{1}\bigl[d_1 \cdots d_k = d_{m-k+1} \cdots d_m\bigr].$$

**Proof.** (Martingale / Casino argument.) Consider an infinite sequence of gamblers. Gambler $j$ arrives at position $j$ and bets \$1 that $p_j = d_1$. If correct (probability $1/A$), the stake is multiplied by $A$ (fair odds) and wagered on $p_{j+1} = d_2$, etc. If any bet fails, the gambler is ruined.

Define the cumulative wealth process $X_t = \sum_{j=1}^{t} (\text{payout of gambler } j \text{ by time } t) - t$. This is a martingale with $E[X_t] = 0$.

Let $T$ be the first time the pattern $s$ completes. At time $T$:
- Gambler $j$ has a nonzero payout iff the substring $p_j \cdots p_T$ equals a suffix of $s$ that matches a prefix. Specifically, the gambler starting at position $T - k + 1$ (for $1 \le k \le m$) has payout $A^k$ iff $d_1 \cdots d_k = d_{m-k+1} \cdots d_m$.
- The total amount paid in is $T$.

By the optional stopping theorem (the martingale is bounded in expectation and $E[T] < \infty$):
$$E[T] = E[\text{total payout at time } T] = \sum_{k=1}^{m} A^k \cdot \mathbf{1}[\text{prefix}_k = \text{suffix}_k]. \quad \square$$

**Lemma (Relationship to $g(n)$).** *$W(s)$ gives the expected position of the last character of the first occurrence. Since $g(n)$ is the expected position of the first character:*
$$g(n) = W(s) - d + 1$$
*where $s$ is the decimal representation of $n$ and $d = |s|$.*

**Proof.** If the pattern first completes at position $T$ (the last character), the first character is at position $T - d + 1$. Linearity of expectation gives $g(n) = E[T] - d + 1 = W(s) - d + 1$. $\square$

**Verification.** For $n = 535$, $s = \texttt{"535"}$, $d = 3$:
- $k = 1$: prefix "5" = suffix "5" $\checkmark \Rightarrow +10$
- $k = 2$: prefix "53" $\neq$ suffix "35" $\times$
- $k = 3$: prefix "535" = suffix "535" $\checkmark \Rightarrow +1000$

$W = 1010$, so $g(535) = 1010 - 3 + 1 = 1008$. $\checkmark$

## Editorial
g(n) for integer n with decimal string s of length d: g(n) = W(s) - d + 1 where W(s) = sum of 10^k for each k in 1..d where the length-k prefix of s equals the length-k suffix of s (Conway Leading Number). Compute: sum of g(floor(10^16 / n)) for n = 2 to 999999. We return total.

## Pseudocode

```text
Input: N_max = 999999, T = 10^16
Output: sum of g(floor(T/n)) for n = 2 to N_max
total = 0
For n = 2 to N_max:
Return total
```

## Complexity Analysis

- **Time:** $O(N \cdot d^2)$ where $N = 999{,}998$ and $d \le 16$ (digits of $\lfloor 10^{16}/n \rfloor$). The inner loop over $k$ is $O(d)$, and each prefix-suffix comparison is $O(k)$, giving $O(d^2)$ per value. With KMP preprocessing: $O(N \cdot d)$.
- **Space:** $O(d)$ for the string representation.

## Answer

$$\boxed{542934735751917735}$$

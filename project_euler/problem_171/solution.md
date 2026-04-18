# Problem 171: Finding Numbers for Which the Sum of the Squares of the Digits Is a Perfect Square

## Problem Statement

For a positive integer $n$, let $f(n)$ denote the sum of the squares of the digits of $n$ in base 10. Find the last nine digits of
$$\sum_{\substack{0 < n < 10^{20} \\ f(n) \text{ is a perfect square}}} n.$$

## Mathematical Development

**Definition 1.** For $n$ with base-10 digits $d_1 d_2 \cdots d_D$, define $f(n) = \sum_{i=1}^{D} d_i^2$.

**Theorem 1** (Digit-square-sum bound). *For any non-negative integer $n$ representable with at most $D$ decimal digits, $f(n) \leq 81D$. Consequently, the set of attainable perfect-square values of $f$ is $\{j^2 : 1 \leq j \leq \lfloor\sqrt{81D}\rfloor\}$.*

*Proof.* Each digit $d_i \in \{0,1,\ldots,9\}$ satisfies $d_i^2 \leq 9^2 = 81$. Summing over $D$ digits yields $f(n) \leq 81D$. The perfect squares in $\{1,2,\ldots,M\}$ are exactly $\{1^2,2^2,\ldots,\lfloor\sqrt{M}\rfloor^2\}$, giving the claimed set. $\square$

**Corollary 1.** For $D = 20$, we have $f(n) \leq 1620$ and $\lfloor\sqrt{1620}\rfloor = 40$. The target set of perfect-square sums is $\{1, 4, 9, \ldots, 1600\}$.

**Lemma 1** (Digit-append identity). *Let $\mathcal{S}$ be any set of non-negative integers and let $d \in \{0,1,\ldots,9\}$. Then*
$$\sum_{x \in \mathcal{S}} (10x + d) = 10\sum_{x \in \mathcal{S}} x + d \cdot |\mathcal{S}|.$$

*Proof.* By linearity of finite sums:
$$\sum_{x \in \mathcal{S}} (10x + d) = 10 \sum_{x \in \mathcal{S}} x + \sum_{x \in \mathcal{S}} d = 10 \sum_{x \in \mathcal{S}} x + d \cdot |\mathcal{S}|. \quad \square$$

**Theorem 2** (Digit DP correctness). *Define the following quantities for $k \geq 0$ and $0 \leq s \leq 81D$:*

- *$C[k][s]$ := the number of $k$-character strings over $\{0,1,\ldots,9\}$ whose digit-square-sum equals $s$.*
- *$T[k][s]$ := the sum, modulo $10^9$, of the numerical values of all such strings.*

*Initialize $C[0][0] = 1$ and $T[0][0] = 0$, with all other entries zero. The recurrence for appending digit $d \in \{0,\ldots,9\}$ is:*
$$C[k+1][s + d^2] \mathrel{+}= C[k][s], \qquad T[k+1][s + d^2] \mathrel{+}= 10 \cdot T[k][s] + d \cdot C[k][s].$$

*Then the answer to the problem is*
$$\sum_{j=1}^{40} T[20][j^2] \pmod{10^9}.$$

*Proof.* We represent every integer $n \in \{0, 1, \ldots, 10^{20}-1\}$ uniquely as a 20-character zero-padded decimal string. The DP constructs these strings one character at a time, left to right. At layer $k$, $C[k][s]$ counts strings of length $k$ with digit-square-sum $s$, and $T[k][s]$ holds their aggregate numerical value modulo $10^9$.

The correctness of the transition for $T$ follows from Lemma 1: when we extend every string $x$ in $\mathcal{S} = \{x : \text{length } k,\; f(x) = s\}$ by appending digit $d$, the new numerical value of each extended string is $10x + d$, and the new digit-square-sum is $s + d^2$. The sum over $\mathcal{S}$ of these new values is $10 \cdot T[k][s] + d \cdot C[k][s]$, as required.

After processing all $D = 20$ positions, $T[20][j^2]$ accumulates the sum of all 20-character strings with digit-square-sum $j^2$. Summing over $j = 1, \ldots, 40$ yields the desired total modulo $10^9$.

Finally, the string $00\cdots0$ representing $n = 0$ has $f(0) = 0$, which is a perfect square ($0 = 0^2$). However, since $n = 0$ contributes $0$ to the sum, the exclusion $n > 0$ in the problem statement requires no correction. $\square$

## Editorial
Is a Perfect Square Find the last nine digits of the sum of all n, 0 < n < 10^20, such that f(n) = sum of squares of digits of n is a perfect square. Method: Digit DP tracking (count, value_sum) keyed by digit-square-sum.

## Pseudocode

```text
MOD = 10^9
S_MAX = 1620

C[0..S_MAX] = 0; T[0..S_MAX] = 0
C[0] = 1

For k from 1 to 20:
    C'[0..S_MAX] = 0; T'[0..S_MAX] = 0
    For s from 0 to S_MAX:
        If C[s] == 0 then continue
        For d from 0 to 9:
            s' = s + d*d
            if s' > S_MAX: break // digits are in order, so d^2 increases
            C'[s'] += C[s]
            T'[s'] = (T'[s'] + 10*T[s] + d*C[s]) mod MOD
    C = C'; T = T'

answer = 0
For j from 1 to 40:
    answer = (answer + T[j*j]) mod MOD
Return answer
```

## Complexity Analysis

- **Time:** The triple loop executes $D \cdot S_{\max} \cdot 10$ iterations in the worst case. With $D = 20$ and $S_{\max} = 1620$, this gives $20 \times 1621 \times 10 = 324{,}200$ operations. Hence $O(D \cdot S_{\max})$.
- **Space:** Two arrays of size $S_{\max} + 1 = 1621$ suffice (rolling-array technique), giving $O(S_{\max})$.

## Answer

$$\boxed{142989277}$$

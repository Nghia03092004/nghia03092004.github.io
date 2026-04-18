# Problem 217: Balanced Numbers

## Problem Statement

A positive integer with $k$ digits (base 10) is **balanced** if the sum of the first $\lfloor k/2 \rfloor$ digits equals the sum of the last $\lfloor k/2 \rfloor$ digits. For odd $k$, the middle digit is ignored.

All 1-digit numbers are trivially balanced (both halves are empty, summing to 0).

Find the sum of all balanced numbers less than $10^{47}$, modulo $3^{15} = 14348907$.

## Mathematical Foundation

**Definition.** For a $k$-digit number, let $h = \lfloor k/2 \rfloor$. Write the number as:
- Even $k = 2h$: $N = L \cdot 10^h + R$, where $L$ is an $h$-digit number ($10^{h-1} \leq L < 10^h$) and $0 \leq R < 10^h$.
- Odd $k = 2h+1$: $N = L \cdot 10^{h+1} + m \cdot 10^h + R$, where $L$ is $h$ digits, $m \in \{0, \ldots, 9\}$, $0 \leq R < 10^h$.

Balanced means $\operatorname{digitsum}(L) = \operatorname{digitsum}(R)$.

**Theorem 1 (Decomposition of balanced number sum).** *Define for $h$-digit strings (with possible leading zeros):*
- *$C(h, s) = |\{x : 0 \leq x < 10^h,\; \operatorname{digitsum}(x) = s\}|$*
- *$\Sigma(h, s) = \sum_{\substack{0 \leq x < 10^h \\ \operatorname{digitsum}(x) = s}} x$*

*For the left half (no leading zeros): $C_L(h, s) = C(h, s) - C(h-1, s)$ and $\Sigma_L(h, s) = \Sigma(h, s) - \Sigma(h-1, s)$ (subtracting strings with leading zero).*

*Then for even $k = 2h$:*

$$\text{Sum}_{2h} = \sum_{s=0}^{9h} \bigl[\Sigma_L(h,s) \cdot 10^h \cdot C(h,s) + C_L(h,s) \cdot \Sigma(h,s)\bigr]$$

*For odd $k = 2h+1$:*

$$\text{Sum}_{2h+1} = \sum_{s=0}^{9h} \bigl[10 \cdot \Sigma_L(h,s) \cdot 10^{h+1} \cdot C(h,s) + 45 \cdot 10^h \cdot C_L(h,s) \cdot C(h,s) + 10 \cdot C_L(h,s) \cdot \Sigma(h,s)\bigr]$$

**Proof.** For even $k = 2h$: a balanced number $N = L \cdot 10^h + R$ with $\operatorname{digitsum}(L) = \operatorname{digitsum}(R) = s$. Summing over all such $N$:

$$\sum_N N = \sum_N (L \cdot 10^h + R) = 10^h \sum_L L \cdot |\{R : \operatorname{digitsum}(R) = s\}| + \sum_R R \cdot |\{L : \operatorname{digitsum}(L) = s, L \geq 10^{h-1}\}|$$

$$= 10^h \cdot \Sigma_L(h,s) \cdot C(h,s) + C_L(h,s) \cdot \Sigma(h,s)$$

Summing over $s$ gives the formula. For odd $k = 2h+1$: $N = L \cdot 10^{h+1} + m \cdot 10^h + R$, summing over $m = 0, \ldots, 9$ (giving a factor of 10 for $L$ and $R$ terms, and $\sum m = 45$ for the middle digit term). $\square$

**Lemma 1 (DP for digit-sum statistics).** *The functions $C(h, s)$ and $\Sigma(h, s)$ satisfy the recurrences:*

$$C(h, s) = \sum_{d=0}^{9} C(h-1, s-d)$$

$$\Sigma(h, s) = \sum_{d=0}^{9} \bigl[10 \cdot \Sigma(h-1, s-d) + d \cdot C(h-1, s-d)\bigr]$$

*with base cases $C(0, 0) = 1$, $C(0, s) = 0$ for $s \neq 0$, $\Sigma(0, 0) = 0$.*

**Proof.** The last digit $d$ of an $h$-digit string contributes $d$ to the digit sum and $d$ to the numeric value. Removing the last digit gives an $(h-1)$-digit string with digit sum $s - d$ and value $v$. The original string has value $10v + d$. Summing: $\Sigma(h, s) = \sum_d \sum_v (10v + d) = \sum_d [10 \cdot \Sigma(h-1, s-d) + d \cdot C(h-1, s-d)]$. The count recurrence is analogous. $\square$

## Editorial
Sum of all balanced numbers below 10^47, mod 3^15 = 14348907. A k-digit number is balanced if the digit sum of the first floor(k/2) digits equals the digit sum of the last floor(k/2) digits. For odd k, the middle digit is ignored. We compute C(h, s) and Sigma(h, s) via DP. We then also compute C(h-1, s) and Sigma(h-1, s). Finally, we build the tables C[0..h][0..max_s] and Sigma[0..h][0..max_s].

## Pseudocode

```text
1-digit balanced numbers: 1+2+...+9 = 45
Compute C(h, s) and Sigma(h, s) via DP
Also compute C(h-1, s) and Sigma(h-1, s)
DP: build tables C[0..h][0..max_s], Sigma[0..h][0..max_s]
Compute left-half tables (no leading zeros)
Accumulate sum for this digit length
if k is even
```

## Complexity Analysis

- **Time:** For each digit length $k$ (up to 47), the DP builds tables of size $h \times 9h$ where $h \leq 23$. Each entry requires $O(10)$ work. Total: $O\bigl(\sum_{k=2}^{47} h \cdot 9h \cdot 10\bigr) = O(47 \cdot 23^2 \cdot 90) \approx O(2.2 \times 10^6)$.
- **Space:** $O(h \cdot 9h) = O(h^2)$ per digit length, reusable. Maximum $O(23^2) \approx O(500)$.

## Answer

$$\boxed{6273134}$$

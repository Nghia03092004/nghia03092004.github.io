# Problem 40: Champernowne's Constant

## Problem Statement

An irrational decimal fraction is created by concatenating the positive integers:

$$0.123456789101112131415161718192021\ldots$$

If $d_n$ represents the $n$-th digit of the fractional part, find the value of:
$$d_1 \times d_{10} \times d_{100} \times d_{1000} \times d_{10000} \times d_{100000} \times d_{1000000}$$

## Mathematical Foundation

**Definition.** *Champernowne's constant* (base 10) is $C_{10} = 0.123456789101112\ldots$, formed by concatenating all positive integers in order. We denote by $d_n$ the $n$-th digit of the fractional part.

**Theorem 1 (Block structure).** *The digits of Champernowne's constant are partitioned into blocks by digit count $k$. The $k$-digit positive integers range from $10^{k-1}$ to $10^k - 1$, contributing a total of $9 \times 10^{k-1} \times k$ digits.*

**Proof.** The number of $k$-digit positive integers is $10^k - 10^{k-1} = 9 \times 10^{k-1}$. Each contributes $k$ digits to the concatenation, giving $9 \times 10^{k-1} \times k$ digits total from the $k$-digit block. $\square$

**Lemma 1 (Cumulative digit positions).** *Let $L_k = \sum_{j=1}^{k} 9 \times 10^{j-1} \times j$ be the cumulative number of digits contributed by all integers with at most $k$ digits. Then:*

| $k$ | $9 \times 10^{k-1} \times k$ | $L_k$ |
|-----|------------------------------|-------|
| 1 | 9 | 9 |
| 2 | 180 | 189 |
| 3 | 2700 | 2889 |
| 4 | 36000 | 38889 |
| 5 | 450000 | 488889 |
| 6 | 5400000 | 5888889 |

**Proof.** Direct computation: $L_1 = 9$, $L_2 = 9 + 180 = 189$, $L_3 = 189 + 2700 = 2889$, etc. $\square$

**Theorem 2 (Digit extraction algorithm).** *To find $d_n$:*
1. *Find the unique $k$ such that $L_{k-1} < n \le L_k$ (with $L_0 = 0$).*
2. *Compute the offset $r = n - L_{k-1}$ within the $k$-digit block.*
3. *The integer containing the $n$-th digit is $q = 10^{k-1} + \lfloor (r - 1) / k \rfloor$.*
4. *The position within $q$ is $j = (r - 1) \bmod k$ (0-indexed from the left).*
5. *Then $d_n$ is the $j$-th digit (from the left) of $q$.*

**Proof.** After consuming $L_{k-1}$ digits from all integers with fewer than $k$ digits, position $n$ falls within the $k$-digit block. The offset $r$ counts how many digits into this block position $n$ lies. Since each $k$-digit integer contributes exactly $k$ digits, the $\lfloor (r-1)/k \rfloor$-th integer in the block (0-indexed) is $10^{k-1} + \lfloor (r-1)/k \rfloor$. Within this integer, the digit position is $(r-1) \bmod k$ from the left. $\square$

**Theorem 3 (Computation of required digits).** *Applying Theorem 2 to each required position:*

- $d_1$: $k=1$, $r = 1$. Integer $= 1 + \lfloor 0/1 \rfloor = 1$. Digit index $0$ of "1" $\Rightarrow d_1 = 1$.

- $d_{10}$: $k=2$, $r = 10 - 9 = 1$. Integer $= 10 + \lfloor 0/2 \rfloor = 10$. Digit index $0$ of "10" $\Rightarrow d_{10} = 1$.

- $d_{100}$: $k=2$, $r = 100 - 9 = 91$. Integer $= 10 + \lfloor 90/2 \rfloor = 10 + 45 = 55$. Digit index $90 \bmod 2 = 0$ of "55" $\Rightarrow d_{100} = 5$.

- $d_{1000}$: $k=3$, $r = 1000 - 189 = 811$. Integer $= 100 + \lfloor 810/3 \rfloor = 100 + 270 = 370$. Digit index $810 \bmod 3 = 0$ of "370" $\Rightarrow d_{1000} = 3$.

- $d_{10000}$: $k=4$, $r = 10000 - 2889 = 7111$. Integer $= 1000 + \lfloor 7110/4 \rfloor = 1000 + 1777 = 2777$. Digit index $7110 \bmod 4 = 2$ of "2777" $\Rightarrow d_{10000} = 7$.

- $d_{100000}$: $k=5$, $r = 100000 - 38889 = 61111$. Integer $= 10000 + \lfloor 61110/5 \rfloor = 10000 + 12222 = 22222$. Digit index $61110 \bmod 5 = 0$ of "22222" $\Rightarrow d_{100000} = 2$.

- $d_{1000000}$: $k=6$, $r = 1000000 - 488889 = 511111$. Integer $= 100000 + \lfloor 511110/6 \rfloor = 100000 + 85185 = 185185$. Digit index $511110 \bmod 6 = 0$ of "185185" $\Rightarrow d_{1000000} = 1$.

**Proof.** Each computation follows directly from Theorem 2 with the values from Lemma 1. $\square$

## Algorithm

```
CHAMPERNOWNE-DIGIT(n):
    k ← 1
    count ← 9
    start ← 1
    while n > count × k:
        n ← n - count × k
        k ← k + 1
        count ← count × 10
        start ← start × 10
    // n is now the offset within the k-digit block
    number ← start + ⌊(n - 1) / k⌋
    digit_index ← (n - 1) mod k
    return digit at position digit_index in number

SOLVE():
    product ← 1
    for i ← 0 to 6:
        product ← product × CHAMPERNOWNE-DIGIT(10^i)
    return product
```

## Complexity Analysis

- **Time:** $O(\log_{10} n)$ per digit query, since we iterate through at most $\lfloor \log_{10} n \rfloor + 1$ blocks. With 7 queries and $n \le 10^6$, total time is $O(7 \times 6) = O(1)$.
- **Space:** $O(1)$. Only a constant number of variables are needed.

## Answer

$$\boxed{210}$$

# Problem 491: Double Pandigital Number Divisible by 11

## Problem Statement

We call a positive integer *double pandigital* if it uses all the digits $0$ to $9$ exactly twice (with no leading zero). How many double pandigital numbers are divisible by $11$?

## Notation

Throughout, a *double pandigital number* is a 20-digit string $d_1 d_2 \cdots d_{20}$ (with $d_1 \neq 0$) in which every digit $0, 1, \ldots, 9$ appears exactly twice. Positions $1, 3, 5, \ldots, 19$ are called *odd positions* and positions $2, 4, 6, \ldots, 20$ are called *even positions*. We write $O = \sum_{i \text{ odd}} d_i$ and $E = \sum_{i \text{ even}} d_i$.

## Mathematical Foundation

**Theorem 1 (Divisibility rule for 11).** An integer $N$ with decimal representation $d_1 d_2 \cdots d_n$ satisfies $11 \mid N$ if and only if $\sum_{i=1}^{n}(-1)^{i+1} d_i \equiv 0 \pmod{11}$.

*Proof.* Since $10 \equiv -1 \pmod{11}$, we have $10^k \equiv (-1)^k \pmod{11}$ for all $k \geq 0$. Therefore
$$N = \sum_{i=1}^{n} d_i \cdot 10^{n-i} \equiv \sum_{i=1}^{n} d_i \cdot (-1)^{n-i} \pmod{11}.$$
For $n = 20$ (even), $(-1)^{n-i} = (-1)^{-i} = (-1)^i$, so $N \equiv \sum_{i=1}^{20} (-1)^i d_i = E - O \pmod{11}$. Hence $11 \mid N$ if and only if $O \equiv E \pmod{11}$. $\square$

**Lemma 1 (Reduction to a single congruence).** The divisibility condition $O \equiv E \pmod{11}$ is equivalent to $O \equiv 1 \pmod{11}$.

*Proof.* Since every digit $0, 1, \ldots, 9$ appears exactly twice, the total digit sum is $2(0 + 1 + \cdots + 9) = 90$. Thus $O + E = 90$, giving $E = 90 - O$. The condition $O \equiv E \pmod{11}$ becomes $O \equiv 90 - O \pmod{11}$, i.e., $2O \equiv 90 \pmod{11}$. Now $90 = 8 \cdot 11 + 2$, so $90 \equiv 2 \pmod{11}$. Hence $2O \equiv 2 \pmod{11}$. Since $\gcd(2, 11) = 1$, we may divide both sides by $2$ (equivalently, multiply by $2^{-1} \equiv 6 \pmod{11}$) to obtain $O \equiv 1 \pmod{11}$. $\square$

**Definition (Assignment vector).** An *assignment vector* is a tuple $\mathbf{c} = (c_0, c_1, \ldots, c_9) \in \{0, 1, 2\}^{10}$ satisfying $\sum_{d=0}^{9} c_d = 10$, where $c_d$ denotes the number of copies of digit $d$ placed at odd positions. The complementary count at even positions is $2 - c_d$.

**Lemma 2 (Feasible values of $O$).** Under any assignment vector $\mathbf{c}$, the odd-position sum is $O(\mathbf{c}) = \sum_{d=0}^{9} d \cdot c_d$. Since $0 \leq c_d \leq 2$ and $\sum c_d = 10$, we have $0 \leq O \leq 90$. The values satisfying $O \equiv 1 \pmod{11}$ in $[0, 90]$ are exactly
$$O \in \{1, 12, 23, 34, 45, 56, 67, 78, 89\}.$$

*Proof.* The minimum of $O$ is $0$ (attained when $c_d = 2$ for $d = 0, 1, 2, 3, 4$ and $c_d = 0$ otherwise) and the maximum is $90$ (attained when $c_d = 2$ for $d = 5, 6, 7, 8, 9$). The integers congruent to $1 \pmod{11}$ in $\{0, 1, \ldots, 90\}$ are $1, 12, 23, 34, 45, 56, 67, 78, 89$. The value $O = 0$ does not satisfy the congruence, nor does $O = 90$. $\square$

**Theorem 2 (Counting formula).** For a valid assignment vector $\mathbf{c}$ (i.e., $\sum c_d = 10$ and $\sum d \cdot c_d \equiv 1 \pmod{11}$), the number of double pandigital numbers with no leading zero is
$$N(\mathbf{c}) = \underbrace{\frac{10!}{\prod_{d=0}^{9} c_d!}}_{\text{odd-position permutations}} \cdot \underbrace{\frac{10!}{\prod_{d=0}^{9} (2-c_d)!}}_{\text{even-position permutations}} \;-\; \mathbf{1}_{c_0 \geq 1} \cdot \frac{9!}{(c_0 - 1)!\, \prod_{d=1}^{9} c_d!} \cdot \frac{10!}{\prod_{d=0}^{9} (2-c_d)!}.$$

*Proof.* Given $\mathbf{c}$, the 10 odd-position slots receive $c_d$ copies of digit $d$ for each $d \in \{0, \ldots, 9\}$. The number of distinct permutations of these 10 items is the multinomial coefficient $\binom{10}{c_0, c_1, \ldots, c_9} = 10!/\prod c_d!$. Similarly, the 10 even-position slots receive $2 - c_d$ copies of each digit $d$, yielding $10!/\prod (2 - c_d)!$ permutations. Since the choices for odd and even positions are independent, the total number of 20-digit strings (including those with a leading zero) is the product of these two multinomials.

To exclude leading zeros: position 1 is an odd-position slot. A leading zero occurs precisely when digit $0$ occupies position 1. If $c_0 \geq 1$, we can fix one zero at position 1 and permute the remaining $9$ odd-position digits (consisting of $c_0 - 1$ zeros and $c_d$ copies of each $d \geq 1$) in $9!/((c_0-1)! \prod_{d \geq 1} c_d!)$ ways. Multiplying by the even-position count and subtracting yields the formula. If $c_0 = 0$, no odd-position slot contains a zero, so the leading digit is never zero and no subtraction is needed. $\square$

**Corollary.** The answer is $\displaystyle \sum_{\mathbf{c} \text{ valid}} N(\mathbf{c})$.

## Editorial
We enumerate the admissible parameter range, discard candidates that violate the derived bounds or arithmetic constraints, and update the final set or total whenever a candidate passes the acceptance test.

## Pseudocode

```text
answer = 0
For each each (c_0, c_1, ..., c_9) in {0,1,2}^10:
    If sum(c_d) != 10 then continue
    O = sum(d * c_d for d = 0..9)
    If O mod 11 != 1 then continue
    odd_perms = 10! / prod(c_d!)
    even_perms = 10! / prod((2 - c_d)!)
    total = odd_perms * even_perms
    If c_0 >= 1 then
        leading_zero = 9! / ((c_0 - 1)! * prod(c_d! for d=1..9)) * even_perms
        total -= leading_zero
    answer += total
Return answer
```

## Complexity Analysis

- **Time:** $O(3^{10} \cdot 10) = O(590490)$. There are $3^{10} = 59049$ candidate vectors, each processed in $O(10)$ arithmetic operations.
- **Space:** $O(1)$ auxiliary (beyond the precomputed table of $10! , 9!, \ldots, 0!$).

## Answer

$$\boxed{194505988824000}$$

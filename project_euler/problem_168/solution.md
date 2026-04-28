# Problem 168: Number Rotations

## Problem Statement

Find all integers $n$ with $10 \leq n < 10^{100}$ such that moving the last digit of $n$ to the front produces a number that is an exact multiple of $n$. Sum the last 5 digits of all such $n$.

## Mathematical Development

**Theorem 1 (Algebraic formulation).** *Let $n$ be a $d$-digit number ($d \geq 2$) with last digit $r$. Write $n = 10q + r$ where $q = \lfloor n/10 \rfloor$. The rotated number is $m = r \cdot 10^{d-1} + q$. The condition $m = kn$ for a positive integer $k$ is equivalent to:*
$$q = \frac{r(10^{d-1} - k)}{10k - 1}$$

**Proof.** From $m = kn$:
$$r \cdot 10^{d-1} + q = k(10q + r)$$
$$r \cdot 10^{d-1} - kr = 10kq - q$$
$$r(10^{d-1} - k) = q(10k - 1)$$
$$q = \frac{r(10^{d-1} - k)}{10k - 1}$$
$\square$

**Lemma 1 (Bounds on $k$).** *The multiplier $k$ satisfies $1 \leq k \leq 9$.*

**Proof.** Since $m$ and $n$ are both $d$-digit numbers (or $m$ could have fewer digits if $r = 0$, but $r = 0$ leads to $m = q$ which has $d-1$ digits while $kn \geq n \geq 10^{d-1}$, a contradiction for $d \geq 2$), we need $r \geq 1$. Then $m < 10^d$ and $n \geq 10^{d-1}$, so $k = m/n < 10^d / 10^{d-1} = 10$. Also $k \geq 1$ since $m \geq 10^{d-1} \geq n/10$ and actually $m = kn \geq n$ requires $k \geq 1$. So $1 \leq k \leq 9$. $\square$

**Theorem 2 (Digit-by-digit cyclic construction).** *For fixed $k \in \{1, \ldots, 9\}$ and last digit $r \in \{1, \ldots, 9\}$, the digits of $n$ satisfy a carry-propagation recurrence. Starting from the rightmost digit $a_d = r$, each preceding digit $a_{d-1}, a_{d-2}, \ldots$ is determined by:*
$$k \cdot a_i + c_i = 10 c_{i-1} + a_{i-1}$$
*where $c_i$ is the carry from position $i$. The sequence of digits is eventually periodic since the carry $c_i \in \{0, 1, \ldots, 8\}$ is bounded.*

**Proof.** The condition $m = kn$ in terms of individual digits: multiplying $n = a_1 a_2 \cdots a_d$ by $k$ must produce $m = a_d a_1 a_2 \cdots a_{d-1}$. Column-by-column multiplication from right to left:
- Position $d$: $k \cdot a_d = 10c_{d-1} + a_{d-1}$
- Position $i$ ($2 \leq i \leq d-1$): $k \cdot a_i + c_i = 10c_{i-1} + a_{i-1}$
- Position $1$: $k \cdot a_1 + c_1 = a_d$ (with final carry 0 and result digit $a_d$)

Since each $a_i \in \{0, \ldots, 9\}$ and $c_i \in \{0, \ldots, 8\}$ (because $k \cdot 9 + 8 = 89 < 90$, so $c \leq 8$), the carry sequence determines the digit sequence. With 9 possible carry values, the sequence becomes periodic with period at most 9. For each valid cycle closing (where $c_1$ produces $a_d = r$), we get a valid $n$ of length equal to the cycle length. $\square$

**Lemma 2.** *The last digit $r$ must satisfy $r \geq 1$, and for each $(k, r)$ pair, valid numbers $n$ exist for all cycle lengths $d$ that are multiples of the fundamental period of the carry recurrence, up to $d = 99$ (since $n < 10^{100}$).*

**Proof.** As shown in Lemma 1, $r = 0$ is impossible. For each $(k, r)$, the carry recurrence starting from $c_d = 0$, $a_d = r$ generates digits $a_{d-1}, a_{d-2}, \ldots$ with carries $c_{d-1}, c_{d-2}, \ldots$. A valid $n$ of length $d$ requires returning to carry $c = 0$ with the final digit being $a_d = r$ after $d$ steps. This happens at all multiples of the fundamental cycle length. $\square$

## Editorial
The solver works digit by digit from the rightmost end. Once the multiplier $k$ and the final digit $r$ are fixed, the multiplication relation determines the next digit and carry uniquely. So each pair $(k,r)$ generates a deterministic digit stream rather than a branching search tree.

As the digits are produced, the program keeps the last five digits of the current candidate modulo $100000$, because that is all the final sum needs. Whenever the recurrence closes cleanly, meaning the carry vanishes and the left end reconnects to the original last digit, a valid rotated multiple has been found. Enumerating all $k \in \{1,\dots,9\}$ and $r \in \{1,\dots,9\}$, and extending each chain up to 100 digits, covers every admissible number below $10^{100}$.

## Pseudocode

```text
Initialize the answer modulo 100000 to zero.

For each multiplier $k$ from 1 to 9:
    For each possible last digit $r$ from 1 to 9:
        Start the recurrence with current digit $r$ and carry 0.
        Build the number from right to left for lengths up to 100 digits,
        updating the stored last-five-digit residue as new digits are appended.

        After each step, compute the next digit and next carry from the multiplication rule.
        Whenever the chain has length at least 2 and the new state closes back to digit $r$
        with zero carry, record the current residue as one valid number.

Return the final sum modulo 100000.
```

## Complexity Analysis

- **Time:** $O(9 \times 9 \times 100) = O(8{,}100)$ arithmetic operations. Effectively $O(1)$.
- **Space:** $O(100)$ for storing digits of the current candidate.

## Answer

$$\boxed{59206}$$

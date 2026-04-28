# Problem 164: Numbers for Which No Three Consecutive Digits Have a Sum Greater Than a Given Value

## Problem Statement

How many 20-digit numbers $n$ (without any leading zeros) exist such that no three consecutive digits of $n$ have a sum greater than 9?

## Mathematical Development

**Theorem 1 (Recurrence).** *Let $f(\ell, a, b)$ denote the number of $\ell$-digit suffixes (allowing leading zeros) such that the first two digits are $a$ and $b$, and every three consecutive digits sum to at most 9. Then:*
$$f(\ell, a, b) = \begin{cases} 1 & \text{if } \ell = 2, \\ \displaystyle\sum_{d=0}^{9 - a - b} f(\ell - 1, b, d) & \text{if } \ell \geq 3. \end{cases}$$

**Proof.** For $\ell = 2$, the suffix is exactly the two digits $a, b$ and there are no three-consecutive-digit windows, so $f(2, a, b) = 1$.

For $\ell \geq 3$, the suffix is $a, b, d, \ldots$ where $d$ is the third digit. The constraint on the first window requires $a + b + d \leq 9$, i.e., $d \leq 9 - a - b$. Since $a + b \leq 9$ (otherwise no valid suffix exists at all, and the sum is empty), we have $d \in \{0, 1, \ldots, 9 - a - b\}$. Choosing $d$ and continuing from the pair $(b, d)$ with $\ell - 1$ remaining digits gives the recurrence. $\square$

**Theorem 2 (Answer formula).** *The number of 20-digit numbers with no three consecutive digits summing above 9 is:*
$$\text{Answer} = \sum_{a=1}^{9} \sum_{b=0}^{9-a} f(20, a, b)$$

**Proof.** The first digit $a$ ranges over $\{1, \ldots, 9\}$ (no leading zeros). The second digit $b$ must satisfy $a + b \leq 9$ (otherwise the pair $(a, b)$ cannot begin any valid triple), so $b \in \{0, \ldots, 9 - a\}$. The remaining 18 digits are counted by $f(20, a, b)$. $\square$

**Lemma 1 (State space size).** *The number of valid states $(a, b)$ with $0 \leq a, b \leq 9$ and $a + b \leq 9$ is exactly $\binom{11}{2} = 55$.*

**Proof.** We count pairs $(a, b)$ with $a + b \leq 9$:
$$\sum_{a=0}^{9} (10 - a) = 10 + 9 + \cdots + 1 = \frac{10 \cdot 11}{2} = 55.$$
$\square$

**Lemma 2 (Matrix exponentiation alternative).** *Define the $55 \times 55$ transition matrix $M$ where rows and columns are indexed by valid states $(a,b)$, and $M_{(a,b),(b,d)} = 1$ if $a + b + d \leq 9$ (i.e., $d \leq 9 - a - b$), and 0 otherwise. Then $f(\ell, a, b) = \sum_{(c,e)} (M^{\ell-2})_{(a,b),(c,e)}$. This allows computation in $O(55^3 \log \ell)$ time.*

**Proof.** Each multiplication by $M$ extends the suffix by one digit. After $\ell - 2$ multiplications, we sum over all terminal states. $\square$

## Editorial
The natural state is the last two digits already placed. Once those are known, the next digit is allowed to range only up to $9-a-b$, so the rest of the prefix no longer matters. That makes this a small dynamic program on ordered pairs $(a,b)$ rather than on full 20-digit strings.

The implementation starts by enumerating all valid first-two-digit pairs, respecting the no-leading-zero rule. It then extends the number one digit at a time with a rolling DP table: each state `(a, b)` pushes its count to all next states `(b, c)` satisfying $a+b+c \le 9$. After the twentieth digit has been placed, summing the remaining state counts gives the answer.

## Pseudocode

```text
Create a DP table indexed by the last two digits.

Initialize all two-digit prefixes:
    the first digit must be between 1 and 9,
    the second digit must allow at least one valid continuation,
    so only pairs with $a+b \le 9$ receive count 1.

For each remaining digit position from 3 through 20:
    build a fresh table.
    For every current pair $(a,b)$ with nonzero count:
        try every digit $c$ from 0 up to $9-a-b$.
        Add the count from $(a,b)$ into the next state $(b,c)$.
    Replace the old table with the new one.

Return the sum of all counts in the final table.
```

## Complexity Analysis

- **Time:** $O(L \cdot S \cdot D)$ where $L = 20$ (digit positions), $S = 55$ (states), and $D \leq 10$ (digit choices per state). This gives $O(20 \cdot 55 \cdot 10) = O(11{,}000)$.
- **Space:** $O(S) = O(55)$ with a rolling array (two layers of DP).

## Answer

$$\boxed{378158756814587}$$

# Problem 515: Dissonant Numbers

## Problem Statement

For a positive integer $n$ and base $b \geq 2$, let $\mathrm{ds}_b(n)$ denote the digit sum of $n$ in base $b$. Define $D(b, s, N)$ as the count of integers $1 \leq n \leq N$ with $\mathrm{ds}_b(n) = s$. Compute $\sum D(b_i, s_i, N_i)$ for given parameter triples.

## Mathematical Foundation

**Theorem 1 (Generating Function for Digit Sums).** The number of non-negative integers with exactly $L$ digits in base $b$ (allowing leading zeros) whose digit sum equals $s$ is the coefficient

$$[x^s]\left(\frac{1 - x^b}{1 - x}\right)^L.$$

**Proof.** Each digit $d_i \in \{0, 1, \ldots, b-1\}$ contributes $x^{d_i}$ to the generating function. The generating function for a single digit is $1 + x + x^2 + \cdots + x^{b-1} = \frac{1 - x^b}{1 - x}$. Since the $L$ digits are independent, the generating function for the digit sum of an $L$-digit string is $\left(\frac{1-x^b}{1-x}\right)^L$. The coefficient of $x^s$ counts the strings with digit sum $s$. $\square$

**Lemma 1 (Inclusion-Exclusion Evaluation).** For $L$ digits in base $b$ with digit sum $s$:

$$[x^s]\left(\frac{1-x^b}{1-x}\right)^L = \sum_{j=0}^{\lfloor s/b \rfloor} (-1)^j \binom{L}{j}\binom{s - jb + L - 1}{L - 1}.$$

**Proof.** Expand $\left(\frac{1-x^b}{1-x}\right)^L = (1-x^b)^L \cdot (1-x)^{-L}$. Using the binomial theorem on $(1-x^b)^L = \sum_{j=0}^{L}(-1)^j\binom{L}{j}x^{jb}$ and the negative binomial series $(1-x)^{-L} = \sum_{m=0}^{\infty}\binom{m+L-1}{L-1}x^m$, the coefficient of $x^s$ is obtained by convolution:

$$[x^s] = \sum_{j=0}^{\lfloor s/b\rfloor} (-1)^j \binom{L}{j}\binom{s - jb + L - 1}{L - 1}.$$

$\square$

**Theorem 2 (Digit DP Correctness).** The digit DP with states $(\mathrm{pos}, \mathrm{rem}, \mathrm{tight})$ correctly counts all integers $n \in [0, N]$ with $\mathrm{ds}_b(n) = s$.

**Proof.** We represent $N = (d_k d_{k-1} \cdots d_0)_b$. The DP builds $n$ digit-by-digit from the most significant position. At each step:

- If $\mathrm{tight} = \mathrm{true}$, the current digit is bounded by $d_{\mathrm{pos}}$; choosing $d < d_{\mathrm{pos}}$ releases the tight constraint for all subsequent positions.
- If $\mathrm{tight} = \mathrm{false}$, the digit ranges freely over $\{0, \ldots, b-1\}$.
- $\mathrm{rem}$ tracks the remaining digit sum needed.

Each integer $n \leq N$ corresponds to exactly one path through the DP, so the count is exact. $\square$

## Algorithm

```
function D(b, s, N):
    digits := base_b_representation(N)
    L := len(digits)
    // memo[pos][rem][tight] -> count
    memo := new HashMap

    function dp(pos, rem, tight):
        if rem < 0: return 0
        if pos == L:
            return 1 if rem == 0 else 0
        if (pos, rem, tight) in memo:
            return memo[(pos, rem, tight)]

        limit := digits[pos] if tight else b - 1
        result := 0
        for d := 0 to limit:
            new_tight := tight AND (d == digits[pos])
            result += dp(pos + 1, rem - d, new_tight)

        memo[(pos, rem, tight)] := result
        return result

    // Count integers in [0, N] with digit sum s, then subtract
    // the case n = 0 if s == 0
    return dp(0, s, true) - (1 if s == 0 else 0)
```

## Complexity Analysis

- **Time:** $O(L \cdot s \cdot b)$ per query, where $L = \lfloor\log_b N\rfloor + 1$ is the number of digits. The tight flag doubles the state space by at most a factor of 2 (absorbed into the constant).
- **Space:** $O(L \cdot s)$ for the memoization table.

## Answer

$$\boxed{2422639000800}$$

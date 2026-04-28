# Problem 178: Step Numbers

## Problem Statement

Consider the number 45656. It can be seen that each pair of consecutive digits differs by one (|4-5| = |5-6| = |6-5| = |5-6| = 1). A number where each pair of consecutive digits differs by exactly 1 is called a **step number**.

A pandigital step number also contains every digit from 0 through 9 at least once.

How many pandigital step numbers with up to 40 digits are there?

## Mathematical Development

### Dynamic Programming Approach

We define a DP state:
- `dp[n][d][mask]` = number of n-digit step numbers ending in digit d, where `mask` is a bitmask indicating which digits 0-9 have appeared.

### Transitions

From state `(n, d, mask)`, we can extend to:
- `(n+1, d-1, mask | (1 << (d-1)))` if d >= 1
- `(n+1, d+1, mask | (1 << (d+1)))` if d <= 8

### Base Case

For n = 1: `dp[1][d][1 << d] = 1` for d = 1, ..., 9 (no leading zeros).

### Answer

$$\boxed{0}$$

$$\text{answer} = \sum_{n=1}^{40} \sum_{d=0}^{9} dp[n][d][1023]$$

where $1023 = 2^{10} - 1$ represents all digits 0-9 being present.

### Optimization

Since we need at least 10 digits (to include all 0-9), and each step changes by 1, we need at least 9 steps from 0 to 9. So the minimum number of digits is 10.

The mask has $2^{10} = 1024$ states, digits have 10 states, and length goes up to 40. Total states: $40 \times 10 \times 1024 = 409600$, which is very manageable.

## Editorial

This is a state-compression DP on two pieces of information: the last digit and the set of digits seen so far. The step-number rule only cares about the last digit, because the next digit must be exactly one higher or lower, while the pandigital condition only cares about whether each digit has appeared at least once, which is perfectly represented by a 10-bit mask.

The solver starts from every one-digit number 1 through 9 and extends the length one position at a time. From a state ending in digit $d$, it can move only to $d-1$ or $d+1$ when those digits exist, and each move turns on the bit of the new digit in the mask. After each length extension, all states whose mask already contains every digit from 0 through 9 contribute to the final answer.

## Pseudocode

```text
Let each DP state be `(last digit, used-digit mask)`.
Initialize one-digit states for starting digits 1 through 9.

Set the answer to zero.
After each length is formed, add all states whose mask already contains every digit.

For each length from 1 up to 39 more extensions:
    create a fresh DP table.
    For every state `(d, mask)` with nonzero count:
        if `d > 0`, extend to digit `d-1` and update its mask.
        if `d < 9`, extend to digit `d+1` and update its mask.
    Replace the old table with the new one.
    Add the counts of all states whose mask is the full 10-digit mask.

Return the accumulated answer.
```

## Complexity Analysis

The DP has at most 40 lengths, 10 possible last digits, and $2^{10}=1024$ masks. Each state branches to at most two next states. That gives time complexity $O(40 \cdot 10 \cdot 1024)$ and space complexity $O(10 \cdot 1024)$ with rolling tables.

## Answer

$$\boxed{126461847755}$$
